/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** Disconnect safely client
*/

#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "myftp/server.h"
#include "myftp/socket.h"
#include "myftp/utils.h"

//
// Recalculate the fd max for select
// Should be called when a FD_CLR has happend
// on the base_set (fd_set)
//
static void ftp_recalculate_fd_max_select(struct ftp_server *server)
{
    server->selector.max_fd = server->sockfd;
    for (int i = 0; i < FTP_STANDARD_MAX_CLIENTS; i++)
        if (FD_ISSET(i, &server->selector.base_set) &&
            server->selector.max_fd < i)
            server->selector.max_fd = i;
}

//
// Desactivates the client passive or active state
// Is basically the same thing as ftp_disconnect_client
// for the shutdown close clear and deconnection
//
void ftp_disconnect_client_mode_state(struct ftp_server *server,
        unsigned int clientidx)
{
    struct ftp_client *client = &server->selector.clients_data[clientidx];

    if (client->mod.sockfd != 0 &&
        client->mod.sockfd != -1) {
        shutdown(client->mod.sockfd, SHUT_RDWR);
        close(client->mod.sockfd);
    }
    client->mode = FTP_CLIENT_NO_MODE;
    client->mod.sockfd = -1;
    memset(&client->mod.sockin, 0,
        sizeof(client->mod.sockin));
}

//
// Disconnect a client and shutdown his sockfd it is != 0 and != -1
// then closes their sockfd
// It also closes the sockfd of the active state
// (generally used for data transfer)
// Logs the deconnection fo the client
// and finally resets the max fd for a better use of select
//
void ftp_disconnect_client(struct ftp_server *server, unsigned int clientidx)
{
    struct ftp_client *client = &server->selector.clients_data[clientidx];

    if (client->sockfd != 0 && client->sockfd != -1) {
        shutdown(client->sockfd, SHUT_RDWR);
        close(client->sockfd);
        FD_CLR(client->sockfd, &server->selector.base_set);
    }
    ftp_disconnect_client_mode_state(server, clientidx);
    fprintf(stderr, "User disconnected: %s:%hu%s",
        inet_ntoa(client->sockin.sin_addr), ntohs(client->sockin.sin_port),
        CRLF);
    memset(client, 0, sizeof(struct ftp_client));
    client->sockfd = -1;
    client->mod.port = -1;
    ftp_recalculate_fd_max_select(server);
}

//
// Disconnect the current client and closes his data stream connection
// Should always return 220 except if the client give more than 1 arguments
//
void ftp_manage_client_cmd_quit(struct ftp_server *server,
    struct ftp_client *client, int argc, char **argv UNUSED)
{
    if (argc > 2)
        return rfc959(client, 501);
    rfc959(client, 200);
    ftp_disconnect_client(server, client - server->selector.clients_data);
}
