/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** Usage display
*/

#include <stdlib.h>
#include <stdio.h>

#include "myftp/utils.h"

//
// Prints the usage of the myftp
// And exits the code
//
void ftp_usage_and_exit(int exit_code)
{
    fprintf(stderr,
        "USAGE: ftp port path"CRLF
        "\tport is the port number on which the server socket listens"CRLF
        "\tpath is the path to the home directory for the Anonymous user"CRLF);
    exit(exit_code);
}
