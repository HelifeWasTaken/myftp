/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** Print current working directory of the client
*/

#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "myftp/server.h"
#include "myftp/socket.h"
#include "myftp/utils.h"
#include "ftp_path_client_cwd_cdup_cmd.h"

//
// Tries to check if the current path is valid
// with the help of ftp_filesystem_check_current_path
// If the path is invalid reply with the 550 code
// otherwise reply with 257 code and the path
//
// The implementation of the rfc959(257) is simply:
// "257 %s" so it's possible to put the command arguments
//
void ftp_manage_client_cmd_pwd(struct ftp_server *server,
    struct ftp_client *client, int argc, char **argv UNUSED)
{
    char *tmp = client->path;

    if (argc != 1)
        return rfc959(client, 501);
    if (ftp_filesystem_check_current_path(server->home, client->path, true) !=
        FTP_PATH_VALID)
        return rfc959(client, 550);
    tmp += strlen(server->home);
    return *tmp ? rfc959(client, 257, tmp) : rfc959(client, 257, "/");
}
