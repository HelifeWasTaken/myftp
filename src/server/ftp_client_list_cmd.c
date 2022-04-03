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
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

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
static void ftp_manage_client_list_internal_send(
    struct ftp_server *server, struct ftp_client *client, DIR *data)
{
    socklen_t size = sizeof(client->active_state.sockin);
    struct dirent *dir;
    int fd;

    if (client->is_active) {
        fd = accept(server->sockfd,
            (struct sockaddr *)&client->active_state.sockin, &size);
    } else {
        rfc959(client, 125);
        fd = client->sockfd;
    }
    if (fd == -1)
        return rfc959(client, 425);
    while ((dir = readdir(data)) != NULL) {
        socket_puts(fd, dir->d_name);
        socket_puts(fd, CRLF);
    }
    rfc959(client, 250);
    ftp_disconnect_client_active_state(server,
            client - server->selector.clients_data);
}

//
// Intermediate function that resolve the path and tries to open the directory
// If it succeed it tries to send the given folder content in parameter
//
static void ftp_manage_client_list_internal(struct ftp_server *server,
    struct ftp_client *client, char *rootfolder, char *directory_to_check)
{
    enum ftp_path_status status;
    struct stat st;
    DIR *dir = NULL;

    setvbuf(stdout, NULL, _IONBF, BUFSIZ);
    status = ftp_resolve_file_path(client, rootfolder, directory_to_check);
    if (status != FTP_PATH_VALID)
        return rfc959(client, 550);
    if (stat(client->path, &st) == -1 ||
        (st.st_mode & S_IFMT) != S_IFDIR ||
        (dir = opendir(client->path)) == NULL)
        return errno == EBUSY ? rfc959(client, 450) : rfc959(client, 550);
    rfc959(client, 150);
    ftp_manage_client_list_internal_send(server, client, dir);
    closedir(dir);
}

//
// Forks the program and initialize the data stream connection if the arguments
// are compliant
//
// The program will then try before to resolve the given path
// Will open the file and finally intialize the connection between the
// two servers
//
void ftp_manage_client_cmd_list(struct ftp_server *server,
    struct ftp_client *client, int argc, char **argv)
{
    pid_t pid = -1;

    if (argc > 2)
        rfc959(client, 501);
    pid = fork();
    if (pid < 0) {
        perror("Fork");
        return rfc959(client, 550);
    }
    if (pid == 0) {
        ftp_manage_client_list_internal(server, client,
            server->home, argv[1] ? argv[1] : "./");
        exit(0);
    }
}
