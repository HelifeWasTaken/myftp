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
// Activates the active_state of the current client and checks wheter
// the port is valid (this does not connect to the socket of the other program)
// Active state is a state where in data transfer the stream
// is directly directed on the specified port of the client
//
// If the command succeed we reply a 200
// If the command was wrongly typed we reply a 501
// If the port is invalid we reply a 500
//
void ftp_manage_client_cmd_port(struct ftp_server *server UNUSED,
    struct ftp_client *client, int argc, char **argv)
{
    const char *current_ip = inet_ntoa(client->sockin.sin_addr);
    int tmpport = -1;

    if (argc != 2)
        return rfc959(client, 501);
    tmpport = atoi(argv[1]);
    if (str_is_num(argv[1]) == 0 || tmpport < 0 || tmpport > USHRT_MAX)
        return rfc959(client, 500);
    client->is_active = true;
    memcpy(&client->active_state.sockin, &client->sockin,
        sizeof(client->sockin));
    client->active_state.sockin.sin_port = tmpport;
    client->active_state.sockfd = -1;
    return rfc959(client, 200);
}
