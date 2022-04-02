/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** closes the server
*/

#include <unistd.h>
#include <stdlib.h>

#include "myftp/utils.h"
#include "myftp/server.h"
#include "myftp/socket.h"

//
// Closes the current file descriptor of socket
// frees the server and exit with the specified exit_code
//
// It also try to close all the current connections with
// the current clients
//
void close_server(struct ftp_server *server, int exit_code)
{
    for (int i = 0; i < server->selector.max_fd; i++)
        ftp_disconnect_client(server, i);
    shutdown(server->sockfd, SHUT_RDWR);
    close(server->sockfd);
    free(server);
    exit(exit_code);
}
