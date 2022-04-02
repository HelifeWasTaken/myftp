/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** socket write
*/

#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "myftp/utils.h"

//
// Write nbytes to a socket
// If write returns -1 it aborts and consider that the socket is not
// aviable anymore for now
//
void socket_write(int sockfd, const void *msg, size_t bytes)
{
    ssize_t nbytes = 0;
    ssize_t current = 0;

    do {
        nbytes = write(sockfd, msg + current, bytes - current);
        if (nbytes == -1) {
            fprintf(stderr, "422 Write returned -1 while writing message to "
                    "sockfd(%d)%s", sockfd, CRLF);
            return;
        }
        current += nbytes;
    } while (current != (ssize_t)bytes);
}

//
// Just write a plain string in the socket
//
void socket_puts(int sock, char const *msg)
{
    socket_write(sock, msg, strlen(msg));
}
