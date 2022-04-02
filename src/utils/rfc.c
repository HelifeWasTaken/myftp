/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** print a rfc compliant message
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdarg.h>

#include "myftp/server.h"
#include "myftp/socket.h"
#include "myftp/utils.h"

struct ftp_rfc959_table {
    int code;
    char *format;
};

static const struct ftp_rfc959_table RFC959_TABLE[] = {
    {200,   "Command okay."},
    {202,   "Command not implemented, superfluous at this site."},
    {211,   "System status, or system help reply."},
    {212,   "Directory status."},
    {213,   "File status."},
    {214,   "214 message not implemented"},
    {215,   "215 message not implemented"},
    {220,   "Service ready for new user."},
    {221,   "Service closing control connection."CRLF
            "Logged out if appropriate."},
    {225,   "Data connection open; no transfer in profress."},
    {226,   "Closing data connection."CRLF"Requested file action successful"},
    {227,   "Entering Passive  Mode (%d,%d,%d,%d,%d,%d)."},
    {230,   "User logged in, proceed."},
    {250,   "Requested file action okay, completed."},
    {257,   "%s"},
    {332,   "Need account for login."},
    {350,   "Requested file action pending further information."},
    {421,   "Service not available, closing control connection."},
    {425,   "Can't open data connection."},
    {426,   "Connection closed; transfer aborted."},
    {450,   "Requested action aborted: local error in processing."},
    {452,   "Requested action not taken."CRLF
            "Insufficient storage space in system"},
    {500,   "Syntax error, command unrecognized"},
    {501,   "Syntax error in parameters or arguments"},
    {502,   "Command not implemented"},
    {503,   "Bad sequence of commands"},
    {504,   "Command not implemented for that parameter."},
    {530,   "%s"},
    {532,   "Need account for storing files."},
    {550,   "Requested action not taken."},
    {552,   "Requested file action aborted."},
    {553,   "Requested action not taken."},
    {-1,    "Unhandled reply code"}
};

//
// Prints an rfc959 messagle compliant
// Some messages may require arguments so this function takes
// a variadic number of argument depending of the code
//
void rfc959(struct ftp_client *client, int code, ...)
{
    char buf[BUFSIZ + 1] = {0};
    va_list ap;
    unsigned int i = 0;
    int tmp = 0;

    for (; RFC959_TABLE[i].code != -1; i++)
        if (RFC959_TABLE[i].code == code)
            break;
    va_start(ap, code);
    tmp += snprintf(buf, BUFSIZ, "%d", RFC959_TABLE[i].code);
    vsnprintf(buf + tmp, BUFSIZ - tmp, RFC959_TABLE[i].format, ap);
    va_end(ap);
    fprintf(stderr, "%s: %s:%hu%s", buf,
        inet_ntoa(client->sockin.sin_addr), ntohs(client->sockin.sin_port),
        CRLF);
    socket_puts(client->sockfd, buf);
    socket_puts(client->sockfd, CRLF);
}
