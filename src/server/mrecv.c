/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** myrecv Impl
*/

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "myftp/server.h"
#include "myftp/utils.h"
#include "myftp/socket.h"
#include "myftp/alloc.h"

//
// Utility function to disconnect a client that probably lost connection
// or disconnected and to free the currently used buffer
//
static struct ftp_buffer disconnect_client_mrecv_internal(
        struct ftp_server *server, unsigned int client_index,
        struct ftp_buffer *buf)
{
    if (buf->bytes != NULL)
        free(buf->bytes);
    memset(buf, 0, sizeof(struct ftp_buffer));
    ftp_disconnect_client(server, client_index);
    return *buf;
}

//
// Is used to either disconnect the client nothing was received from him
// and to add a \0 char to the string if the receiving_raw_text option is set
//
static struct ftp_buffer mrecv_end(struct ftp_server *server,
        unsigned int clientidx, int receiving_raw_text, struct ftp_buffer *buf)
{
    if (receiving_raw_text) {
        buf->bytes = xrealloc(buf->bytes, sizeof(char) * (buf->size + 1));
        buf->bytes[buf->size] = 0;
    }
    return buf->size == 0 || buf->bytes == NULL ?
        disconnect_client_mrecv_internal(server, clientidx, buf) : *buf;
}

//
// Recieve a command from the client
// If the received size is of size 0
// Disconnect the user
// And returns a struct equivalent to
// struct ftp_buffer {
//   char buf = NULL;
//   size_t size = 0;
// };
//
// Otherwise it returns 0
// If receiving raw text the mrecv command will either
// read until read returns 0 or that the last two characters that has been
// catched were \r\n
//
// The other one will simply read until read returns 0
// It will try to read block per block the data (BUFSIZ)
//
struct ftp_buffer mrecv(struct ftp_server *server,
        unsigned int clientidx, int receiving_raw_text)
{
    struct ftp_client *client = &server->selector.clients_data[clientidx];
    struct ftp_buffer buf = {0};
    char tmp[BUFSIZ] = {0};
    ssize_t current = 0;

    while (true) {
        current = read(client->sockfd, tmp, sizeof(tmp));
        if (current == -1)
            return disconnect_client_mrecv_internal(server, clientidx, &buf);
        if (current == 0)
            break;
        buf.bytes = xrealloc(buf.bytes, sizeof(char) * (buf.size + current));
        memcpy(buf.bytes + buf.size, tmp, current);
        buf.size += current;
        if (receiving_raw_text && buf.size >= 2 &&
            memcmp(buf.bytes + buf.size - 2, "\r\n", 2) == 0)
            break;
    }
    return mrecv_end(server, clientidx, receiving_raw_text, &buf);
}
