/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** Ftp client user and password entry (Login manager)
*/

#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "myftp/server.h"
#include "myftp/socket.h"
#include "myftp/utils.h"

//
// Try to log the user
// As it is now the ftp server does not handle multiple home directories
// and does not handle multitple users so the connection is simply
// Anonymous and an empty password
//
// In reality we should use something such as a mysql database
// to manage login but as it is now we can simply make the connection
// by putting raw username and password in the program
//
// Client status in the end might either be:
// - FTP_WAITING_FOR_USERNAME (Failure) -> Resets username and password
// - FTP_CONNECTED (Success)
//
static void ftp_try_validate_login(struct ftp_client *client, int force_reset)
{
    if (strcmp(client->username, "Anonymous") == 0 &&
        strcmp(client->password, "") == 0) {
        client->status = FTP_CONNECTED;
        rfc959(client, 230);
    } else if (force_reset) {
        memset(client->username, 0, sizeof(client->username));
        memset(client->password, 0, sizeof(client->password));
        client->status = FTP_WAITING_FOR_USERNAME;
        rfc959(client, 530);
    }
}

//
// Manage the USER Command
// Argc should NEVER be at 0
// If argc = 1 username == ""
// If argc = 2 username = argv[1] only if:
// strlen(argv[1]) < FTP_STANDARD_USERNAME_LIMIT
// Otherwise it is considered as an error of EPATHTOOLONG
//
// When USER is called and is a valid command the client status is either:
//  - FTP_WAITING_FOR_USERNAME (failure)
//  - FTP_WAITING_FOR_PASSWORD (success)
//
void ftp_manage_client_cmd_user(struct ftp_server *server UNUSED,
    struct ftp_client *client, int argc, char **argv)
{
    if (argc > 2)
        return rfc959(client, 501);
    client->status = FTP_WAITING_FOR_USERNAME;
    if (argv[1] != NULL && strlen(argv[1]) >= FTP_STANDARD_USERNAME_LIMIT)
        return rfc959(client, 500);
    if (argv[1] == NULL)
        client->username[0] = '\0';
    else
        strcpy(client->username, argv[1]);
    client->status = FTP_WAITING_FOR_PASSWORD;
    rfc959(client, 331);
}

//
// Manage the PASS command
// Argc should NEVER be at 0
// If argc = 1 password == ""
// If argc = 2 password = argv[1] only if:
// strlen(argv[1]) < FTP_STANDARD_PASSWORD_LIMIT
// Otherwise it is considered as an error of EPATHTOOLONG
//
// When USER is called and is a valid command the client status is either:
// - FTP_WAITING_FOR_USERNAME (Failure)
// - FTP_CONNECTED (Success) -> Depends on ftp_try_validate_login
//
void ftp_manage_client_cmd_pass(struct ftp_server *server UNUSED,
    struct ftp_client *client, int argc, char **argv)
{
    if (argc > 2)
        return rfc959(client, 501);
    client->status = FTP_WAITING_FOR_USERNAME;
    if (argv[1] != NULL && strlen(argv[1]) > FTP_STANDARD_PASSWORD_LIMIT)
        return rfc959(client, 500);
    strcpy(client->password, argv[1] ? argv[1] : "");
    ftp_try_validate_login(client, 1);
}
