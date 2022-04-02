/*
** EPITECH PROJECT, 2022
** Utils
** File description:
** Ftp utils header file
*/

#pragma once

#include <stddef.h>
#include <stdbool.h>

struct ftp_client;

//
// Prints the usage of the myftp
// And exits the code
//
void ftp_usage_and_exit(int exit_code);

//
// Write nbytes to a socket
// If write returns -1 it aborts and consider that the socket is not
// aviable anymore for now
//
void socket_write(int sockfd, const void *msg, size_t bytes);

//
// Just write a plain string in the socket
//
void socket_puts(int sock, char const *msg);

//
// Prints an error and exit(84) if the assertion given is false
// append_str_error adds the errno message information
// the assert works like a printf and can bu used like one after the first
// two parameters
//
void ftp_assert(bool assertion, bool append_str_error, char const *format, ...);

//
// Prints an rfc959 messagle compliant
// Some messages may require arguments so this function takes
// a variadic number of argument depending of the code
//
void rfc959(struct ftp_client *client, int code, ...);

#define CRLF "\r\n"

#define UNUSED __attribute__((unused))
