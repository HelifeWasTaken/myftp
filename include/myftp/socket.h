/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** socket definition - Client and server impl
*/

#pragma once

#include <stdbool.h>
#include <netinet/in.h>
#include <sys/select.h>

#define FTP_STANDARD_MAX_CLIENTS    FD_SETSIZE
#define FTP_STANDARD_PATH_LIMIT     8192
#define FTP_STANDARD_USERNAME_LIMIT 8192
#define FTP_STANDARD_PASSWORD_LIMIT 100

enum ftp_client_status {
    FTP_NOT_CONNECTED,
    FTP_WAITING_FOR_USERNAME,
    FTP_WAITING_FOR_PASSWORD,
    FTP_CONNECTED
};

struct ftp_client {
    int sockfd;
    struct sockaddr_in sockin;
    char path[FTP_STANDARD_PATH_LIMIT + 1];
    char username[FTP_STANDARD_USERNAME_LIMIT + 1];
    char password[FTP_STANDARD_PASSWORD_LIMIT + 1];
    char *input;
    size_t buffer_size;
    enum ftp_client_status status;
    bool is_active;
    struct {
        struct sockaddr_in sockin;
        int sockfd;
    } active_state;
};

struct ftp_select_handler {
    fd_set base_set;
    fd_set worker_set;
    struct ftp_client clients_data[FTP_STANDARD_MAX_CLIENTS];
    int max_fd;
};

struct ftp_server {
    int sockfd;
    int clientfd;
    uint16_t port;
    struct protoent *proto;
    struct sockaddr_in in;
    socklen_t socklen;
    char home[FTP_STANDARD_PATH_LIMIT + 1];
    struct ftp_select_handler selector;
};
