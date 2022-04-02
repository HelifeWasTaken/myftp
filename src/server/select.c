/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** Select manager
*/

#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/select.h>

#include "myftp/socket.h"
#include "myftp/server.h"
#include "myftp/utils.h"

//
// Checks wheter select has got a fatal eror while being used
// If -1 is not set in rc there was no error
//
// If errno = EINTR and rc = -1 The select error can be ignored
// Otherwise close the server print the current error in the
// error output
//
static void select_error(struct ftp_server *server, int rc)
{
    if (rc != -1)
        return;
    if (errno == EINTR)
        return;
    fprintf(stderr,
            "ftp: select(FD_SETSIZE, &worker_set, NULL, NULL, NULL) "
            "= -1: %s", strerror(errno));
    close_server(server, 84);
}

//
// Call selects to get information about all currently set fds
// Go through all the possible fds in the FD_SET
// and check if one of the fds is currently set
// If the current set fd is the server fd
// Try to accept a new client
// Otherwise manage a client already connected
//
void ftp_manage_select(struct ftp_server *server)
{
    select_error(server,
        select(FTP_STANDARD_MAX_CLIENTS, &server->selector.worker_set,
            NULL, NULL, NULL));
    for (int i = 0; i < FTP_STANDARD_MAX_CLIENTS; i++)
        if (FD_ISSET(i, &server->selector.worker_set))
            i == server->sockfd ? ftp_accept_client(server) :
                ftp_manage_client(server, i);
}
