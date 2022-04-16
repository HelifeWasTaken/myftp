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
#include <netdb.h>

#include "myftp/socket.h"
#include "myftp/server.h"
#include "myftp/utils.h"
#include "myftp/str.h"

int ftp_manage_client_cmd_port_internal(struct ftp_server *server UNUSED,
    struct ftp_client *client, char **fullip)
{
    struct protoent *proto = getprotobyname("TCP");
    const int port[2] = { atoi(fullip[4]), atoi(fullip[5]) };
    char *ip = NULL;
    socklen_t size = sizeof(client->mod.sockin);

    if (proto == NULL || port[0] < 0 || port[1] > USHRT_MAX ||
        port[1] < 0 || port[1] > USHRT_MAX)
        return 1;
    client->mod.sockfd = socket(AF_INET, SOCK_STREAM, proto->p_proto);
    if (client->mod.sockfd == -1 || asprintf(&ip, "%s.%s.%s.%s", fullip[0],
        fullip[1], fullip[2], fullip[3]) == -1 || ip == NULL)
        return 1;
    client->mod.sockin.sin_port = htons(port[0] * 256 + port[1]);
    client->mod.sockin.sin_family = AF_INET;
    client->mod.sockin.sin_addr.s_addr = inet_addr(ip);
    free(ip);
    if (connect(client->mod.sockfd, (void *)&client->mod.sockin, size) == -1)
        return 1;
    return 0;
}

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
    struct ftp_client *client, int argc UNUSED, char **argv UNUSED)
{
    char **fullip = NULL;

    if (argc != 2)
        return rfc959(client, 500);
    fullip = str_split(argv[1], ",\n");
    if (fullip == NULL)
        return rfc959(client, 500);
    if (vector_array_len(fullip) != 6) {
        free(fullip);
        return rfc959(client, 500);
    }
    ftp_disconnect_client_mode_state(server, client -
            server->selector.clients_data);
    if (ftp_manage_client_cmd_port_internal(server, client, fullip)) {
        vector_array_free(fullip);
        return rfc959(client, 500);
    }
    vector_array_free(fullip);
    client->mode = FTP_CLIENT_ACTIVE;
    return rfc959(client, 200);
}
