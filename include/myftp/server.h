/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** server header file
*/

#pragma once

#include <stddef.h>

struct ftp_server;
struct ftp_client;

struct ftp_buffer {
    char *bytes;
    size_t size;
};

enum ftp_path_status {
    FTP_PATH_VALID,
    FTP_PATH_DOES_NOT_CONTAIN_ROOT,
    FTP_PATH_CONTAINS_ROOT_BUT_IS_LONGER,
    FTP_PATH_IS_INVALID,
    FTP_DIRECTORY_DOES_NOT_EXISTS,
    FTP_DIRECTORY_PERMISSION_DENIED,
    FTP_DIRECTORY_UNAVIABLE,
    FTP_CWD_TARGET_FORMAT_INVALID,
    FTP_PATH_TOO_LONG
};

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
        unsigned int clientidx, int receiving_raw_text);

//
// Is called if the FD_SET found that the server fd is set
// like so:
// - FD_ISSET(index, &server->selector.worker_set) && i == server->sockfd
// It tries to accept the new client if the fd got is bigger than
// FTP_STANDARD_MAX_CLIENTS it returns an error and reject it's connection
// Otherwise we sets the new clients in the array and add it to
// base_set to prepare it for the next iteration
//
void ftp_accept_client(struct ftp_server *server);

//
// Disconnect a client and shutdown his sockfd it is != 0 and != -1
// then closes their sockfd
// It also closes the sockfd of the active state
// (generally used for data transfer)
// Logs the deconnection fo the client
// and finally resets the max fd for a better use of select
//
void ftp_disconnect_client(struct ftp_server *server, unsigned int clientidx);

//
// Desactivates the client active state
// Is basically the same thing as ftp_disconnect_client
// for the shutdown close clear and deconnection but only desactivates
// The active port state
//
void ftp_disconnect_client_active_state(struct ftp_server *server,
        unsigned int client_index);

//
// Call selects to get information about all currently set fds
// Go through all the possible fds in the FD_SET
// and check if one of the fds is currently set
// If the current set fd is the server fd
// Try to accept a new client
// Otherwise manage a client already connected
//
// https://stackoverflow.com/questions/36321791/
// using-select-for-multiple-socket-connections-c
// to check it has information about handling multiples fds
//
void ftp_manage_select(struct ftp_server *server);

//
// Closes the current file descriptor of socket
// frees the server and exit with the specified exit_code
//
// It also try to close all the current connections with
// the current clients
//
void close_server(struct ftp_server *server, int exit_code);

//
// Inits the server with init_ftp_socket
// thens binds the server sockfd with launch server
// Logs connection then runs the server
//
void ftp(char const *port, char const *path);

// --- CMDS ---

//
// Manage the USER Command
// Argc should NEVER be at 0
// If argc = 1 username == ""
// If argc = 2 username = argv[1] only if:
// strlen(argv[1]) < FTP_STANDARD_USERNAME_LIMIT
// Otherwise it is considered as an error of EPATHTOOLONG
//
// When USER is called and is a valid command the client status is either:
//  - FTP_WAITING_FOR_USERNAME (failure)
//  - FTP_WAITING_FOR_PASSWORD (success)
//
void ftp_manage_client_cmd_user(struct ftp_server *server,
    struct ftp_client *client, int argc, char **argv);

//
// Manage the PASS command
// Argc should NEVER be at 0
// If argc = 1 password == ""
// If argc = 2 password = argv[1] only if:
// strlen(argv[1]) < FTP_STANDARD_PASSWORD_LIMIT
// Otherwise it is considered as an error of EPATHTOOLONG
//
// When USER is called and is a valid command the client status is either:
// - FTP_WAITING_FOR_USERNAME (Failure)
// - FTP_CONNECTED (Success) -> Depends on ftp_try_validate_login
//
void ftp_manage_client_cmd_pass(struct ftp_server *server,
    struct ftp_client *client, int argc, char **argv);

//
// Tries to go to the parent directory if it is already
// at the root does nothing and succeed
//
// And finally print a rfc compliant reply
//
void ftp_manage_client_cmd_cdup(struct ftp_server *server,
    struct ftp_client *client, int argc, char **argv);

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
void ftp_manage_client_cmd_cwd(struct ftp_server *server,
    struct ftp_client *client, int argc, char **argv);

//
// Disconnect the given client and logs it
// (This is the proper for a client to disconnect to use that command)
//
void ftp_manage_client_cmd_exit(struct ftp_server *server,
    struct ftp_client *client, int argc, char **argv);

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
        struct ftp_client *client, int argc, char **argv);

//
// Tries to check if the current path is valid
// with the help of ftp_filesystem_check_current_path
// If the path is invalid reply with the 550 code
// otherwise reply with 257 code and the path
//
// The implementation of the rfc959(257) is simply:
// "257 %s" so it's possible to put the command arguments
//
void ftp_manage_client_cmd_pwd(struct ftp_server *server,
    struct ftp_client *client, int argc, char **argv);

//
// A command just to know if you can get a reply from the server
// it is kind of dummy
// You can either get a 200 or a 500 response
//
void ftp_manage_client_cmd_noop(struct ftp_server *server,
    struct ftp_client *client, int argc, char **argv);

//
// Activates the active_state of the current client and checks wheter
// the port is valid (this does not connect to the socket of the other program)
// Active state is a state where in data transfer the stream
// is directly directed on the specified port of the client
//
// If the command succeed we reply a 200
// If the command was wrongly typed we reply a 501
// If the port is invalid we reply a 500
//
void ftp_manage_client_cmd_port(struct ftp_server *server,
    struct ftp_client *client, int argc, char **argv);

//
// Returns in passive state
// Passive state is a state where in data transfer the stream
// is directly directed on antoher mini server in the ftp in local
//
// If the command succeed we reply a 227
// (In such format: (h1,h2,h3,h4,p1,p2))
// If the command was wrongly typed we reply a 501
//
// The ip can be retrieved by bitmasking each of the parts of the
// sockin.sin_addr.s_addr
//
// The port can be retrieved by dividing by 256 for the first part
// and with a modulus on 256 for the second part
//
void ftp_manage_client_cmd_pasv(struct ftp_server *server,
    struct ftp_client *client, int argc, char **argv);

//
// Forks the program and initialize the data stream connection if the arguments
// are compliant
//
// The program will then try before to resolve the given path
// Will open the file and finally intialize the connection between the
// two servers
//
void ftp_manage_client_cmd_retr(struct ftp_server *server,
    struct ftp_client *client, int argc, char **argv);

//
// Forks the program and initialize the data stream connection if the arguments
// are compliant
//
// The program will then try before to resolve the given path
// Will open the file and finally intialize the connection between the
// two servers
//
void ftp_manage_client_cmd_stor(struct ftp_server *server,
    struct ftp_client *client, int argc, char **argv);

//
// Forks the program and initialize the data stream connection if the arguments
// are compliant
//
// The program will then try before to resolve the given path
// Will open the file and finally intialize the connection between the
// two servers
//
void ftp_manage_client_cmd_list(struct ftp_server *server,
    struct ftp_client *client, int argc, char **argv);

//
// Handle the commands of the client
// Calls mrecv which receive the plain string of the current client
// If command.bytes is null the client must have disconnected so
// we return doing nothing
//
void ftp_manage_client(struct ftp_server *server, unsigned int clientidx);
