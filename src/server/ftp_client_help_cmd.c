/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** Help command
*/

#include "myftp/server.h"
#include "myftp/socket.h"
#include "myftp/utils.h"

#include <string.h>

struct ftp_help_data {
    char *cmd;
    char *help;
};

static const struct ftp_help_data FTP_HELP_CMD_NO_DATA_STREAM[] = {
    { "USER",   "USER <SP> <username> <CRLF>: Specify user"
        "for authentication" },
    { "PASS",   "PASS <SP> <password> <CRLF>: Specify password"
        " for authentication" },
    { "CWD",    "CWD <SP> <pathname> <CRLF>: Change working directory" },
    { "CDUP",   "CDUP <CRLF>: Change working directory to parent directory" },
    { "QUIT",   "QUIT <CRLF>: Disconnection"},
    { "DELE",   "DELE <SP> <pathname> <CRLF>: Delete a file on the server" },
    { "PWD",    "PWD <CRLF>: Print working directory" },
    { "PASV",   "PASV <CRLF>: Enable \"passive\" mode for data transfer" },
    { "PORT",   "PORT <SP> <host-port> <CRLF>: Enable \"active\" mode "
        "for data transfer" },
    { "HELP",   "HELP [<SP> <string>] <CRLF>: List available commands" },
    { "NOOP",   "NOOP <CRLF>: Do nothing"}
};

static const size_t SIZE_FTP_HELP_CMD_NO_DATA_STREAM =
sizeof(FTP_HELP_CMD_NO_DATA_STREAM) / sizeof(*FTP_HELP_CMD_NO_DATA_STREAM);

static const struct ftp_help_data FTP_HELP_CMD_DATA_STREAM[] = {
    { "RETR",   "RETR <SP> <pathname> <CRLF>: Download file from server to "
        "client" },
    { "STOR",   "STOR <SP> <pathname> <CRLF>: Upload file from client to "
        "server" },
    { "LIST",   "LIST [<SP> <pathname>]: List files in the current working "
        "directory" }
};

static const size_t SIZE_FTP_HELP_CMD_DATA_STREAM =
sizeof(FTP_HELP_CMD_DATA_STREAM) / sizeof(*FTP_HELP_CMD_DATA_STREAM);

//
// Display the help and return an rfc959(214) compliant code and message
//
static void ftp_manage_client_cmd_help_no_arg(struct ftp_client *client)
{
    for (size_t i = 0; i < SIZE_FTP_HELP_CMD_NO_DATA_STREAM; i++) {
        socket_puts(client->sockfd, FTP_HELP_CMD_NO_DATA_STREAM[i].help);
        socket_puts(client->sockfd, CRLF);
    }
    socket_puts(client->sockfd,
            "(the following are commands using data transfer)"CRLF);
    for (size_t i = 0; i < SIZE_FTP_HELP_CMD_DATA_STREAM; i++) {
        socket_puts(client->sockfd, FTP_HELP_CMD_NO_DATA_STREAM[i].help);
        socket_puts(client->sockfd, CRLF);
    }
    return rfc959(client, 214);
}

//
// Display the help for a specific given command if it does not exists
// return an rfc959(501) compliant message
// Otherwise displays it and return an rfc959(214) compliant message
//
static void ftp_manage_client_cmd_help_arg(struct ftp_client *client, char *arg)
{
    const struct ftp_help_data *data = NULL;

    for (size_t i = 0; i < SIZE_FTP_HELP_CMD_NO_DATA_STREAM && !data; i++)
        if (strcmp(FTP_HELP_CMD_NO_DATA_STREAM[i].cmd, arg) == 0)
            data = FTP_HELP_CMD_NO_DATA_STREAM + i;
    for (size_t i = 0; i < SIZE_FTP_HELP_CMD_DATA_STREAM && !data; i++)
        if (strcmp(FTP_HELP_CMD_DATA_STREAM[i].cmd, arg) == 0)
            data = FTP_HELP_CMD_NO_DATA_STREAM + i;
    if (!data)
        return rfc959(client, 501);
    socket_puts(client->sockfd, data->help);
    return rfc959(client, 214);
}

//
// If argc > 2 command is invalid
// If argc == 2 Display the help for a specific command (argv[1])
// If specified command does not exists rfc959(501)
// If argc == 1 Display the help for all the commands
//
void ftp_manage_client_cmd_help(struct ftp_server *server UNUSED,
        struct ftp_client *client, int argc, char **argv)
{
    switch (argc) {
        case 1: return ftp_manage_client_cmd_help_no_arg(client);
        case 2: return ftp_manage_client_cmd_help_arg(client, argv[1]);
        default: return rfc959(client, 501);
    }
}
