/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** Launching the server with bind and listen
*/

#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "myftp/server.h"
#include "myftp/socket.h"
#include "myftp/init.h"
#include "myftp/utils.h"

//
// (void *)
// Casting is required because of inheritance with structs in C
// sockaddr_in implements the firsts variables
// of sockaddr
//
// struct a {
//   int a, b, c;
// }
// struct b {
//    struct a;
// }
//
// struct b = { {5, 2, 1} };
// struct *a = &b;
//
// Is valid because no padding is at the start of the struct
// Or is valid when all the values are aligned in order to have aboslutely
// no padding between values
// The last possiblity is the struct is packed with __attibute__((packed))
// but this is considered not portable
//
// bind: Assing the socket address to a socket indentified to the descriptor
// socket that has no local socket assigned
//
// listen: Marks a connection-mode socket specified by the socket argument
// The second argument provides an hint of the implementation to use
// to limit the number of connections in the socket listen queue.
// Implementations shall support values of backlog up to SOMAXCONN
// see: <sys/socket.h>
// Any backlog < 0: behave like backlog == 0
// A backlog of 0 allows a queue of "implementation-defined minimum value"
//
void launch_server(struct ftp_server *server)
{
    if (bind(server->sockfd, (void *)&server->in,
                sizeof(struct sockaddr_in)) == -1) {
        fprintf(stderr, "ftp: bind(server->sockfd: %d, "
                "(void *)&server->in: %p, "
                "sizeof(struct sockaddr_in): %zu) = -1: %s%s",
                server->sockfd, &server->in, sizeof(struct sockaddr_in),
                strerror(errno), CRLF);
        close_server(server, 84);
    }
    if (listen(server->sockfd, SOMAXCONN) == -1) {
        fprintf(stderr, "ftp: listen(server->sockfd: %d, "
                "SOMAXCONN: %u) = -1: %s%s",
                server->sockfd, SOMAXCONN, strerror(errno), CRLF);
        close_server(server, 84);
    }
}
