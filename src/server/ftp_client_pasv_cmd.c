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
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <netdb.h>

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
int ftp_manage_client_cmd_pasv_internal(struct ftp_server *server UNUSED,
    struct ftp_client *client)
{
    struct protoent *proto = getprotobyname("TCP");
    socklen_t s = sizeof(client->mod.sockin);

    client->mod.sockin = (struct sockaddr_in){ .sin_port = htons(0),
        .sin_family = AF_INET, .sin_addr = { .s_addr = INADDR_ANY } };
    if (proto == NULL)
        return 1;
    if (client->mod.sockfd == -1) {
        close(client->mod.sockfd);
        client->mod.sockfd = -1;
    }
    client->mod.sockfd = socket(AF_INET, SOCK_STREAM, proto->p_proto);
    if (client->mod.sockfd == -1 ||
        bind(client->mod.sockfd, (const void *)&client->mod.sockin,
        sizeof(client->mod.sockin)) == -1 ||
        listen(client->mod.sockfd, 1) == -1 ||
        getsockname(client->mod.sockfd, (void *)&client->mod.sockin, &s) == -1)
        return 1;
    client->mod.port = ntohs(client->mod.sockin.sin_port);
    return 0;
}

void ftp_manage_client_cmd_pasv(struct ftp_server *server,
    struct ftp_client *client, int argc, char **argv UNUSED)
{
    if (argc > 1 || ftp_manage_client_cmd_pasv_internal(server, client) == 1) {
        rfc959(client, 500);
        return;
    }
    client->mode = FTP_CLIENT_PASSIVE;
    rfc959(client, 227, client->sockin.sin_addr.s_addr & 0xFF,
        (client->sockin.sin_addr.s_addr & 0xFF00) >> 8,
        (client->sockin.sin_addr.s_addr & 0xFF0000) >> 16,
        (client->sockin.sin_addr.s_addr & 0xff000000) >> 24,
        client->mod.port / 256, client->mod.port % 256);
}
