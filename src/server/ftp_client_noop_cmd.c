/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** Command to check if you logged well
*/

#include "myftp/server.h"
#include "myftp/socket.h"
#include "myftp/utils.h"

//
// A command just to know if you can get a reply from the server
// it is kind of dummy
// You can either get a 200 or a 500 response
//
void ftp_manage_client_cmd_noop(struct ftp_server *server UNUSED,
    struct ftp_client *client, int argc, char **argv UNUSED)
{
    return argc == 1 ? rfc959(client, 200) : rfc959(client, 500);
}
