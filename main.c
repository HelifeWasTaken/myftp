/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** Main - Entry
*/

#include <stdio.h>
#include <errno.h>

#include "myftp/utils.h"
#include "myftp/server.h"

int main(int ac, char **av)
{
    if (ac != 3)
        ftp_usage_and_exit(84);
    ftp(av[1], av[2]);
    return 0;
}
