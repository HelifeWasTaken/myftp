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
#include <stdlib.h>

#include "myftp/socket.h"
#include "myftp/server.h"
#include "myftp/utils.h"
#include "myftp/str.h"

struct ftp_client_commands {
    char *cmd;
    void (*exe)(struct ftp_server *, struct ftp_client *, int, char **);
    enum ftp_client_status permission_level;
    bool require_mode;
};

static const struct ftp_client_commands FTP_CLIENT_COMMANDS[] = {
    {"NOOP", ftp_manage_client_cmd_noop, FTP_WAITING_FOR_USERNAME, false},
    {"QUIT", ftp_manage_client_cmd_quit, FTP_WAITING_FOR_USERNAME, false},
    {"USER", ftp_manage_client_cmd_user, FTP_WAITING_FOR_USERNAME, false},
    {"HELP", ftp_manage_client_cmd_help, FTP_WAITING_FOR_USERNAME, false},
    {"PASS", ftp_manage_client_cmd_pass, FTP_WAITING_FOR_PASSWORD, false},
    {"CWD",  ftp_manage_client_cmd_cwd,  FTP_CONNECTED, false},
    {"CDUP", ftp_manage_client_cmd_cdup, FTP_CONNECTED, false},
    {"DELE", ftp_manage_client_cmd_dele, FTP_CONNECTED, false},
    {"PWD",  ftp_manage_client_cmd_pwd,  FTP_CONNECTED, false},
    {"PORT", ftp_manage_client_cmd_port, FTP_CONNECTED, false},
    {"PASV", ftp_manage_client_cmd_pasv, FTP_CONNECTED, false},
    {"RETR", ftp_manage_client_cmd_retr, FTP_CONNECTED, true},
    {"STOR", ftp_manage_client_cmd_stor, FTP_CONNECTED, true},
    {"LIST", ftp_manage_client_cmd_list, FTP_CONNECTED, true}
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
    if (cmd->permission_level > client->status)
        return rfc959(client, 530);
    if (cmd->require_mode && client->mode == FTP_CLIENT_NO_MODE)
        return rfc959(client, 425);
    return cmd->exe(server, client, vector_array_len(argv), argv);
}

//
// Convinience function free the server buffer
// and prints an rfc compliant message if code != -1
//
static void ftp_manage_client_end(struct ftp_buffer *buf,
        char **cmd, int rfc_code, struct ftp_client *client)
{
    if (rfc_code != -1)
        rfc959(client, rfc_code);
    if (cmd != NULL)
        vector_array_free(cmd);
    if (buf != NULL && buf->bytes != NULL)
        free(buf->bytes);
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
        return ftp_manage_client_end(&command, argv, -1, client);
    argv = str_split(command.bytes, " "CRLF);
    if (argv == NULL)
        return ftp_manage_client_end(&command, argv, -1, client);
    fprintf(stderr, "Client (%s:%hu) used: [",
        inet_ntoa(client->sockin.sin_addr), ntohs(client->sockin.sin_port));
    for (int i = 0; argv[i]; i++)
        fprintf(stderr, "%s%s", argv[i], argv[i + 1] ? ", " : "]"CRLF);
    if ((cmd = ftp_get_matched_command(argv[0])) == NULL)
        return ftp_manage_client_end(&command, argv, 500, client);
    ftp_execute_cmd_if_has_permission(server, client, cmd, argv);
    return ftp_manage_client_end(&command, argv, -1, client);
}
