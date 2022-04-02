/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** xstrdup impl
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "myftp/str.h"

//
// Behaves likes the function strdup
// but if strdup returns NULL
// The program exit(84) and print a message in stderr
//
char *xstrdup(char const *s)
{
    char *nstr = strdup(s);

    if (nstr == NULL) {
        perror("ftp: xstrdup");
        exit(84);
    }
    return nstr;
}

//
// Behaves likes the function strdup
// but if strndup returns NULL
// The program exit(84) and print a message in stderr
//
char *xstrndup(char const *s, size_t n)
{
    char *nstr = strndup(s, n);

    if (nstr == NULL) {
        perror("ftp: xstrdup");
        exit(84);
    }
    return nstr;
}
