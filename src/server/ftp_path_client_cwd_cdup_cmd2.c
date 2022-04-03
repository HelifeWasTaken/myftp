/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** ftp_path handler part 2
*/


#include "ftp_path_client_cwd_cdup_cmd.h"

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
        char const *current_path, int check_is_dir)
{
    const size_t size_root = strlen(root);
    DIR *dir = NULL;

    if (strncmp(root, current_path, size_root) != 0)
        return FTP_PATH_DOES_NOT_CONTAIN_ROOT;
    if (current_path[size_root] != '/' && current_path[size_root] != '\0')
        return FTP_PATH_CONTAINS_ROOT_BUT_IS_LONGER;
    for (int i = size_root; current_path[i]; i++)
        if (current_path[i] == '/' && current_path[i + 1] == '/')
            return FTP_PATH_IS_INVALID;
    if (!check_is_dir)
        return FTP_PATH_VALID;
    dir = opendir(current_path);
    if (dir != NULL) {
        closedir(dir);
        return FTP_PATH_VALID;
    }
    return errno == ENOENT ? FTP_DIRECTORY_DOES_NOT_EXISTS : errno == EACCES ?
        FTP_DIRECTORY_PERMISSION_DENIED : FTP_DIRECTORY_UNAVIABLE;
}

//
// Tries to go to the parent directory if it is already
// at the root does nothing and succeed
//
enum ftp_path_status ftp_filesystem_path_cdup_internal(char const *root,
    char *current_path)
{
    char tmp[FTP_STANDARD_PATH_LIMIT + 1] = {0};
    char *ptr = NULL;
    enum ftp_path_status status = FTP_PATH_VALID;

    memcpy(tmp, current_path, FTP_STANDARD_PATH_LIMIT);
    if ((status = ftp_filesystem_check_current_path(root, tmp, true)) !=
        FTP_PATH_VALID)
        return status;
    ptr = strrchr(tmp, '/');
    if (ptr == current_path)
        return FTP_PATH_VALID;
    *ptr = 0;
    if ((status = ftp_filesystem_check_current_path(root, tmp, true)) !=
        FTP_PATH_VALID)
        return status;
    memcpy(current_path, tmp, FTP_STANDARD_PATH_LIMIT);
    return FTP_PATH_VALID;
}

//
// Tries to go inside of a directory also handle things such as
// - "/" that goes back to home
// - ".." that calls ftp_filesystem_path_cdup_internal
// - "." Does nothing and always succeed
//
enum ftp_path_status ftp_filesystem_path_cwd_internal(char const *root,
        char *current_path, char const *target_path)
{
    const size_t size_target = strlen(target_path);

    if (strcmp(".", target_path) == 0)
        return FTP_PATH_VALID;
    if (strcmp("/", target_path) == 0) {
        memcpy(current_path, root, FTP_STANDARD_PATH_LIMIT);
        return FTP_PATH_VALID;
    }
    if (strcmp("..", target_path) == 0)
        return ftp_filesystem_path_cdup_internal(root, current_path);
    if (strlen(current_path) + size_target + 1 >= FTP_STANDARD_PATH_LIMIT)
        return FTP_PATH_TOO_LONG;
    strcat(strcat(current_path, "/"), target_path);
    return ftp_filesystem_check_current_path(root, current_path, true);
}

//
// A silent version of the ftp_manage_client_cmd_cwd
// It can be used to resolve paths silently so user does not notice
// anything while processing the path resolving
//
// It is generally used in a fork so the child is modified
// but not the original client (e.g see REM command to see how to use it)
//
enum ftp_path_status ftp_manage_client_cmd_cwd_no_reply(
    struct ftp_client *client, char const *home, char const *destination)
{
    char **array = NULL;
    char tmp[FTP_STANDARD_PATH_LIMIT] = {0};
    enum ftp_path_status status = FTP_PATH_VALID;

    memcpy(tmp, client->path, FTP_STANDARD_PATH_LIMIT);
    array = str_split(destination, "/");
    if (*destination == '/')
        array = vector_array_append_front(array, "/", true);
    for (int i = 0; array[i]; i++) {
        status = ftp_filesystem_path_cwd_internal(home, tmp, array[i]);
        if (status != FTP_PATH_VALID)
            break;
    }
    vector_array_free(array);
    if (status == FTP_PATH_VALID)
        memcpy(client->path, tmp, FTP_STANDARD_PATH_LIMIT);
    return status;
}

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
// Good luck to debug that (it works don't touch it)
//
enum ftp_path_status ftp_resolve_file_path(
    struct ftp_client *client, char const *home, char const *destination)
{
    char tmp[FTP_STANDARD_PATH_LIMIT + 1];
    char *ptr;
    enum ftp_path_status status;

    strncpy(tmp, destination, FTP_STANDARD_PATH_LIMIT);
    ptr = strrchr(tmp, '/');
    if (ptr == NULL) {
        if (strlen(tmp) + 3 > FTP_STANDARD_PATH_LIMIT) return FTP_PATH_TOO_LONG;
        memcpy(memmove(tmp + 2, tmp,
                FTP_STANDARD_PATH_LIMIT - 2) - 2, "./", 2);
        ptr = strrchr(tmp, '/');
    }
    if (strcmp(client->path, tmp) == 0) return FTP_PATH_VALID;
    if (tmp != ptr) *ptr = 0;
    status = ftp_manage_client_cmd_cwd_no_reply(client, home, tmp);
    if (status != FTP_PATH_VALID ||
        strlen(client->path) + strlen(ptr + 1) + 1 >= FTP_STANDARD_PATH_LIMIT)
        return FTP_PATH_TOO_LONG;
    if (tmp != ptr) strcat(strcat(client->path, "/"), ptr + 1);
    return FTP_PATH_VALID;
}
