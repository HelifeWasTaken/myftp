/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** Wrapper around disconnect client
*/

#include "myftp/server.h"
#include "myftp/socket.h"
#include "myftp/utils.h"

//
// Disconnect the given client and logs it
// (This is the proper for a client to disconnect to use that command)
//
void ftp_manage_client_cmd_exit(struct ftp_server *server,
        struct ftp_client *client, int argc, char **argv)
{
    if (argc != 1)
        return rfc959(client, 500);
    rfc959(client, 221);
    ftp_disconnect_client(server, client - server->selector.clients_data);
}
