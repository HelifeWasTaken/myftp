/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** Manage client
*/

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "myftp/socket.h"
#include "myftp/server.h"
#include "myftp/utils.h"
#include "myftp/str.h"

struct ftp_client_commands {
    char *cmd;
    void (*exe)(struct ftp_server *, struct ftp_client *, int, char **);
};

static const struct ftp_client_commands FTP_CLIENT_COMMANDS[] = {
    {"USER", ftp_manage_client_cmd_user},
    {"PASS", ftp_manage_client_cmd_pass},
    {"CWD",  ftp_manage_client_cmd_cwd},
    {"CDUP", ftp_manage_client_cmd_cdup},
    {"DELE", ftp_manage_client_cmd_dele},
    {"PWD",  ftp_manage_client_cmd_pwd},
    {"NOOP", ftp_manage_client_cmd_noop}
};

static const size_t FTP_CLIENT_COMMANDS_SIZE =
    sizeof(FTP_CLIENT_COMMANDS) / sizeof(*FTP_CLIENT_COMMANDS);

//
// Get the function pointer that matches the given string
//
static const struct ftp_client_commands *ftp_get_matched_command(char *cmd)
{
    for (unsigned int i = 0; i < FTP_CLIENT_COMMANDS_SIZE; i++)
        if (strcmp(FTP_CLIENT_COMMANDS[i].cmd, cmd) == 0)
            return &FTP_CLIENT_COMMANDS[i];
    return NULL;
}

//
// Handle the commands of the client
// Calls mrecv which receive the plain string of the current client
// If command.bytes is null the client must have disconnected so
// we return doing nothing
//
// If the given command is invalid reply with the rfc595 code 500
//
void ftp_manage_client(struct ftp_server *server, unsigned int clientidx)
{
    struct ftp_client *client = &server->selector.clients_data[clientidx];
    struct ftp_buffer command = mrecv(server, clientidx, true);
    char **argv = NULL;
    const struct ftp_client_commands *cmd = NULL;

    if (command.bytes == NULL)
        return;
    argv = str_split(command.bytes, " "CRLF);
    fprintf(stderr, "Client (%s:%hu) used: %s%s",
        inet_ntoa(client->sockin.sin_addr), ntohs(client->sockin.sin_port),
        argv[0], CRLF);
    cmd = ftp_get_matched_command(argv[0]);
    if (cmd == NULL) {
        vector_array_free(argv);
        return rfc959(client, 500);
    }
    cmd->exe(server, client, vector_array_len(argv), argv);
    vector_array_free(argv);
}
