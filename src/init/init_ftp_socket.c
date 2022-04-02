/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** Init ftp socket
*/

#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <netdb.h>

#include "myftp/init.h"
#include "myftp/utils.h"
#include "myftp/socket.h"
#include "myftp/alloc.h"
#include "myftp/str.h"

//
// Check wheter the port is valid
// then tries create the SOCKET with AF_INET and SOCK_STREAM
// AF_INET: Is the IPv4 internet protocol
// SOCK_STREAM: Provides a way to send information and receive information
//              from a byte protocol
//              out-of-band data transmission MAY be support
//
// getprotobyname(TCP): reads the next entry from the protocol db
// and returns the good protoent struct
//
// The setup of the sockaddr_in struct:
//
// ip(7): INADDR_ANY is used when you don't care about the ip of someone
// htons: transform a given port number to a valid uint16_t standard unix port
// AF_INET: same as up
//
// Might need to use setsockopt with
// REUSEADDR and REUSEPORT
//
struct ftp_server *init_ftp_socket(char const *path, char const *port)
{
    struct ftp_server *s = xcalloc(sizeof(struct ftp_server), 1);
    int tmpport = -1;

    ftp_assert(chdir(path) != -1, true, "ftp: chdir(%s) = -1", path);
    ftp_assert(getcwd(s->home, FTP_STANDARD_PATH_LIMIT) != NULL, true,
        "ftp: error: getcwd(s->home, FTP_STANDARD_PATH_LIMIT) = NULL");
    ftp_assert(str_is_num(port), false,
        "ftp: Invalid numerical value for port: \"%s\"", port);
    tmpport = atoi(port);
    ftp_assert(!(tmpport < 0 || tmpport > USHRT_MAX), false,
            "ftp: port: %d is not in range 0..%d", s->port, SHRT_MAX);
    s->port = tmpport;
    ftp_assert((s->sockfd = socket(AF_INET, SOCK_STREAM, 0)) != -1, true,
        "ftp: socket(AF_INET, SOCK_STREAM, 0) = -1");
    ftp_assert((s->proto = getprotobyname("TCP")), true,
            "ftp: getprotobyname(\"TCP\") = NULL");
    s->in.sin_addr.s_addr = INADDR_ANY;
    s->in.sin_port = htons(s->port);
    s->in.sin_family = AF_INET;
    return s;
}
