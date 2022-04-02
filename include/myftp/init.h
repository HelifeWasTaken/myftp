/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** init header file
*/

#pragma once

//
// Check wheter the port is valid
// then tries create the SOCKET with AF_INET and SOCK_STREAM
// AF_INET: Is the IPv4 internet protocol
// SOCK_STREAM: Provides a way to send information and receive information
//              from a byte protocol
//              out-of-band data transmission MAY be support
//
// getprotobyname("TCP"): reads the next entry from the protocol db
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
struct ftp_server *init_ftp_socket(char const *path, char const *port);

//
// Binds the FTP/TCP socket created with init_ftp_socket
// and launch listen on it with SOMAXCONN
//
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
void launch_server(struct ftp_server *server);
