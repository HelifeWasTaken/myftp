/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** Delete a file on the server
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "myftp/server.h"
#include "myftp/socket.h"
#include "myftp/utils.h"
#include "ftp_path_client_cwd_cdup_cmd.h"

//
// Starts by resolving the path by removing the file
// e.g: folder1/folder2/file.png becomes folder1/folder2
//      Then if the path could be resolved by
//      ftp_manage_client_cmd_cwd_no_reply it tries to remove the file/folder
//      by using remove
//
//      If something fails it returns an rfc compliant reply
//      such as: 550 Path resolution failed or file not found or
//                   missing permission
//               450 If the file could be found but is currently used
//                   used by another processus
//               250 removes tells the user that the file has
//                   been removed
//
static void ftp_manage_client_cmd_dele_internal(char *rootfolder,
        char *file_to_delete, struct ftp_client *client)
{
    enum ftp_path_status status = ftp_resolve_file_path(client, rootfolder,
            file_to_delete);

    if (status != FTP_PATH_VALID)
        return rfc959(client, 550);
    if (remove(client->path) == 0)
        return rfc959(client, 250);
    return errno == EBUSY ? rfc959(client, 450) : rfc959(client, 550);
}

//
// Deletes a file on the server
//
// The program will fork itself so the child will resolve the path
// to the directory containing the file
//
// But the parent will still have it's folder unchanged
// So it's easier to manage the path
//
// It kinds of simulate the Data Stream method -> upload and download
//
// The path resolving should make sure the user does not delete the root
// folder and does not access files that it does not have rights on
//
// If something fails it returns an rfc compliant reply
//      such as: 550 Path resolution failed or file not found or
//                   missing permission
//               450 If the file could be found but is currently used
//                   used by another processus
//               250 Tells the user that the file has
//                   been removed
//
void ftp_manage_client_cmd_dele(struct ftp_server *server,
        struct ftp_client *client, int argc, char **argv)
{
    int pid;

    if (argc != 2)
        return rfc959(client, 501);
    pid = fork();
    if (pid < 0) {
        perror("DELE command could not fork program");
        return rfc959(client, 450);
    }
    if (pid != 0)
        return;
    ftp_manage_client_cmd_dele_internal(server->home, argv[1], client);
    exit(0);
}
