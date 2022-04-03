/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** accept a new client
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "myftp/socket.h"
#include "myftp/server.h"
#include "myftp/utils.h"
#include "myftp/alloc.h"

//
// Should only be called after ftp_accept_client
// Set the currently new client inside the worker and
// in insde the base_set for the select management
//
static void ftp_accept_client_internal_set(struct ftp_server *server,
        struct ftp_client *client)
{
    if (server->selector.max_fd < client->sockfd)
        server->selector.max_fd = client->sockfd;
    FD_SET(client->sockfd, &server->selector.base_set);
    client->status = FTP_WAITING_FOR_USERNAME;
    strncpy(client->path, server->home, FTP_STANDARD_PATH_LIMIT);
    memcpy(&server->selector.clients_data[client->sockfd],
        client, sizeof(struct ftp_client));
    rfc959(client, 220);
    free(client);
}

//
// Is called if the FD_SET found that the server fd is set
// like so:
// - FD_ISSET(index, &server->selector.worker_set) && i == server->sockfd
// It tries to accept the new client if the fd got is bigger than
// FTP_STANDARD_MAX_CLIENTS it returns an error and reject it's connection
// Otherwise we sets the new clients in the array and add it to
// base_set to prepare it for the next iteration
//
void ftp_accept_client(struct ftp_server *server)
{
    socklen_t len = sizeof(struct sockaddr_in);
    struct ftp_client *client = xcalloc(sizeof(struct ftp_client), 1);

    client->sockfd = accept(server->sockfd, (void *)&client->sockin, &len);
    if (client->sockfd == -1) {
        free(client);
        fprintf(stderr, "Access failed Could not accept client%s", CRLF);
    } else if (client->sockfd >= FTP_STANDARD_MAX_CLIENTS &&
    server->selector.clients_data[client->sockfd].status != FTP_NOT_CONNECTED) {
        shutdown(client->sockfd, SHUT_RDWR);
        close(client->sockfd);
        free(client);
        fprintf(stderr, "A client tried a connection but the maximum of"
                " clients has already reached it's peak: %d%s",
                FTP_STANDARD_MAX_CLIENTS, CRLF);
    } else {
        ftp_accept_client_internal_set(server, client);
    }
}
