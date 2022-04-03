/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** retr command implementation
*/

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>

#include "myftp/server.h"
#include "myftp/socket.h"
#include "myftp/utils.h"
#include "ftp_path_client_cwd_cdup_cmd.h"

//
// Connect to the distant server (if in active mode) and
// send the binary data in the given file descriptor
//
// If the server could not connect itself to the other server
// it replies with 425
// If the connection is interrupted replies 426
//
static void ftp_manage_client_retr_internal_send(
    struct ftp_server *server, struct ftp_client *client, int data)
{
    socklen_t size = sizeof(client->active_state.sockin);
    int fd = -1;
    ssize_t towrite = 0;
    char buf[BUFSIZ] = {0};

    if (client->is_active) {
        fd = accept(server->sockfd,
            (struct sockaddr *)&client->active_state.sockin, &size);
    } else {
        rfc959(client, 125);
        fd = client->sockfd;
    }
    if (fd == -1)
        return rfc959(client, 425);
    while ((towrite = read(data, buf, BUFSIZ)) > 0)
        socket_write(fd, buf, towrite);
    towrite != -1 ? rfc959(client, 250) : rfc959(client, 426);
    ftp_disconnect_client_active_state(server,
            client - server->selector.clients_data);
}

//
// Intermediate function that resolve the path and tries to open the file
// If it succeed it tries to send the given file in parameter
//
static void ftp_manage_client_retr_internal(struct ftp_server *server,
    struct ftp_client *client, char *rootfolder, char *file_to_send)
{
    enum ftp_path_status status;
    int fd = -1;

    status = ftp_resolve_file_path(client, rootfolder, file_to_send);
    if (status != FTP_PATH_VALID)
        return rfc959(client, 550);
    if ((fd = open(client->path, O_RDONLY)) == -1)
        return errno == EBUSY ? rfc959(client, 450) : rfc959(client, 550);
    rfc959(client, 150);
    ftp_manage_client_retr_internal_send(server, client, fd);
    close(fd);
}

//
// Forks the program and initialize the data stream connection if the arguments
// are compliant
//
// The program will then try before to resolve the given path
// Will open the file and finally intialize the connection between the
// two servers
//
void ftp_manage_client_cmd_retr(struct ftp_server *server,
    struct ftp_client *client, int argc, char **argv)
{
    pid_t pid = -1;

    if (argc != 2)
        rfc959(client, 501);
    pid = fork();
    if (pid < 0) {
        perror("Fork");
        return rfc959(client, 550);
    }
    if (pid == 0) {
        ftp_manage_client_retr_internal(server, client,
            server->home, argv[1]);
        exit(0);
    }
}
