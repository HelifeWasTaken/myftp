/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** Server implementation
*/

#include <stdio.h>
#include <stdbool.h>
#include <sys/select.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "myftp/server.h"
#include "myftp/socket.h"
#include "myftp/utils.h"
#include "myftp/init.h"

//
// Resets the fd_set then set the server to the base_set
// Finally in a loop copy in the worker_set the base_set to reset to the
// currently connected fds and calls select
//
static void run_server(struct ftp_server *server)
{
    FD_ZERO(&server->selector.base_set);
    FD_SET(server->sockfd, &server->selector.base_set);
    server->selector.max_fd = server->sockfd;
    while (true) {
        memcpy(&server->selector.worker_set,
            &server->selector.base_set, sizeof(fd_set));
        ftp_manage_select(server);
    }
}

//
// Inits the server with init_ftp_socket
// thens binds the server sockfd with launch server
// Logs connection then runs the server
//
void ftp(char const *port, char const *path)
{
    struct ftp_server *server = init_ftp_socket(path, port);

    launch_server(server);
    printf("Server connected at: %s%s", inet_ntoa(server->in.sin_addr), CRLF);
    run_server(server);
    close_server(server, 0);
}
