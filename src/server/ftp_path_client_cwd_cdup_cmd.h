/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** private ftp path handler header
*/

#pragma once

#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "myftp/server.h"
#include "myftp/socket.h"
#include "myftp/utils.h"
#include "myftp/str.h"

//
// Checks whether the current given path is a valid path
// if the current path is not a valid path it sets the return status
// accordingly
// A path is considered invalid when: the current_path does not have the
// root folder as it's beginning
// When a path has for example two '/' glued together such as
// /folder//folder2
// When the opendir function does not work on it
//
enum ftp_path_status ftp_filesystem_check_current_path(char const *root,
        char const *current_path, int check_is_dir);

//
// Tries to go to the parent directory if it is already
// at the root does nothing and succeed
//
enum ftp_path_status ftp_filesystem_path_cdup_internal(char const *root,
    char *current_path);

//
// Tries to go inside of a directory also handle things such as
// - "/" that goes back to home
// - ".." that calls ftp_filesystem_path_cdup_internal
// - "." Does nothing and always succeed
//
enum ftp_path_status ftp_filesystem_path_cwd_internal(char const *root,
        char *current_path, char const *target_path);

//
// A silent version of the ftp_manage_client_cmd_cwd
// It can be used to resolve paths silently so user does not notice
// anything while processing the path resolving
//
// It is generally used in a fork so the child is modified
// but not the original client (e.g see REM command to see how to use it)
//
enum ftp_path_status ftp_manage_client_cmd_cwd_no_reply(
    struct ftp_client *client, char const *home, char const *destination);

//
// Resolve the path to a file / folder
// E.G: folder1/folder2/folder3
// Resolve a silent CWD to folder1/folder2
// This does not check if folder3 is existant but only resolve the path
// towards it!
//
// This function is destructive for the client path so it recommended
// to do it either in a fork or a copied path
//
enum ftp_path_status ftp_resolve_file_path(
    struct ftp_client *client, char const *home, char const *destination);
