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
    enum ftp_client_status permission_level;
};

static const struct ftp_client_commands FTP_CLIENT_COMMANDS[] = {
    {"USER", ftp_manage_client_cmd_user, FTP_WAITING_FOR_USERNAME},
    {"PASS", ftp_manage_client_cmd_pass, FTP_WAITING_FOR_PASSWORD},
    {"CWD",  ftp_manage_client_cmd_cwd,  FTP_CONNECTED},
    {"CDUP", ftp_manage_client_cmd_cdup, FTP_CONNECTED},
    {"DELE", ftp_manage_client_cmd_dele, FTP_CONNECTED},
    {"PWD",  ftp_manage_client_cmd_pwd,  FTP_CONNECTED},
    {"NOOP", ftp_manage_client_cmd_noop, FTP_WAITING_FOR_USERNAME},
    {"PORT", ftp_manage_client_cmd_port, FTP_CONNECTED},
    {"PASV", ftp_manage_client_cmd_pasv, FTP_CONNECTED},
    {"RETR", ftp_manage_client_cmd_retr, FTP_CONNECTED},
    {"STOR", ftp_manage_client_cmd_stor, FTP_CONNECTED},
    {"LIST", ftp_manage_client_cmd_list, FTP_CONNECTED}
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

static void ftp_execute_cmd_if_has_permission(struct ftp_server *server,
        struct ftp_client *client, const struct ftp_client_commands *cmd,
        char **argv)
{
    if (cmd->permission_level <= client->status)
        return cmd->exe(server, client, vector_array_len(argv), argv);
    rfc959(client, 530);
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
    if (argv == NULL)
        return;
    fprintf(stderr, "Client (%s:%hu) used: [",
        inet_ntoa(client->sockin.sin_addr), ntohs(client->sockin.sin_port));
    for (int i = 0; argv[i]; i++)
        fprintf(stderr, "%s%s", argv[i], argv[i + 1] ? ", " : "]"CRLF);
    cmd = ftp_get_matched_command(argv[0]);
    if (cmd == NULL) {
        vector_array_free(argv);
        return rfc959(client, 500);
    }
    ftp_execute_cmd_if_has_permission(server, client, cmd, argv);
    vector_array_free(argv);
}
