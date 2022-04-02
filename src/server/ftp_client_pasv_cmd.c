/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** Activate a port
*/

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <limits.h>
#include <stdlib.h>

#include "myftp/socket.h"
#include "myftp/server.h"
#include "myftp/utils.h"
#include "myftp/str.h"

//
// Returns in passive state
// Passive state is a state where in data transfer the stream
// is directly directed on antoher mini server in the ftp in local
//
// If the command succeed we reply a 227
// (In such format: (h1,h2,h3,h4,p1,p2))
// If the command was wrongly typed we reply a 501
//
// The ip can be retrieved by bitmasking each of the parts of the
// sockin.sin_addr.s_addr
//
// The port can be retrieved by dividing by 256 for the first part
// and with a modulus on 256 for the second part
//
void ftp_manage_client_cmd_pasv(struct ftp_server *server UNUSED,
    struct ftp_client *client, int argc, char **argv)
{
    const int ip = client->sockin.sin_addr.s_addr;
    const int port = client->sockin.sin_port;
    const int full_ip[6] = {
        ip & 0xFF, (ip & 0xFF00) >> 8,
        (ip & 0xFF0000) >> 16, (ip & 0xff000000) >> 24,
        port / 256, port % 256
    };

    if (argc != 1)
        return rfc959(client, 501);
    ftp_disconnect_client_active_state(server,
        client - server->selector.clients_data);
    return rfc959(client, 227, full_ip[0], full_ip[1], full_ip[2], full_ip[3],
            full_ip[4], full_ip[5]);
}
