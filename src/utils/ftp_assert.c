/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** Assert without abort
*/

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "myftp/utils.h"

//
// Prints an error and exit(84) if the assertion given is false
// append_str_error adds the errno message information
// the assert works like a printf and can bu used like one after the first
// two parameters
//
void ftp_assert(bool assertion, bool append_str_error, char const *format, ...)
{
    va_list ap;
    const char *error = strerror(errno);

    if (assertion)
        return;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    append_str_error ? fprintf(stderr, ": %s%s", error, CRLF) :
        fprintf(stderr, "%s", CRLF);
    va_end(ap);
    exit(84);
}
