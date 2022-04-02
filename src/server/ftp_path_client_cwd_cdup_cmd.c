/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** Ftp filesystem handler
*/

#include "ftp_path_client_cwd_cdup_cmd.h"

//
// Prints a rfc compliant reply to the client and logs it in the server
// based on the status (and if the command was cwd or cdup)
//
static void ftp_print_cwd_cdup_response_to_client(struct ftp_client *client,
        enum ftp_path_status status, bool iscdup)
{
    if (status == FTP_PATH_VALID)
        return iscdup ? rfc959(client, 200) : rfc959(client, 250);
    return rfc959(client, 550);
}

//
// Cuts the given argument with /
// A path like this: home/hello/world
// Would be cutted like this: "home" "hello" "world" and then
// The program will try to virtualy enter each directories
// If the path starts with "/" it will start from the
// ftp_server root directory
//
// And finally print a rfc compliant reply
//
void ftp_manage_client_cmd_cwd(struct ftp_server *server UNUSED,
    struct ftp_client *client, int argc, char **argv)
{
    char **array = NULL;
    char tmp[FTP_STANDARD_PATH_LIMIT] = {0};
    enum ftp_path_status status = FTP_PATH_VALID;

    memcpy(tmp, client->path, FTP_STANDARD_PATH_LIMIT);
    if (argc != 2)
        return rfc959(client, 501);
    array = str_split(argv[1], "/");
    if (argv[1][0] == '/')
        array = vector_array_append_front(array, "/", true);
    for (int i = 0; array[i]; i++) {
        status = ftp_filesystem_path_cwd_internal(server->home, tmp, array[i]);
        if (status != FTP_PATH_VALID)
            break;
    }
    vector_array_free(array);
    if (status == FTP_PATH_VALID)
        memcpy(client->path, tmp, FTP_STANDARD_PATH_LIMIT);
    ftp_print_cwd_cdup_response_to_client(client, status, false);
}

//
// Tries to go to the parent directory if it is already
// at the root does nothing and succeed
//
// And finally print a rfc compliant reply
//
void ftp_manage_client_cmd_cdup(struct ftp_server *server UNUSED,
    struct ftp_client *client, int argc, char **argv UNUSED)
{
    char tmp[FTP_STANDARD_PATH_LIMIT] = {0};
    enum ftp_path_status status = FTP_PATH_VALID;

    memcpy(tmp, client->path, FTP_STANDARD_PATH_LIMIT);
    if (argc != 1)
        return rfc959(client, 501);
    status = ftp_filesystem_path_cwd_internal(server->home, tmp, "..");
    if (status == FTP_PATH_VALID)
        memcpy(client->path, tmp, FTP_STANDARD_PATH_LIMIT);
    ftp_print_cwd_cdup_response_to_client(client, status, true);
}
