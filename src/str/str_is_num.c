/*
** EPITECH PROJECT, 2022
** str is num
** File description:
** str
*/

#include <ctype.h>

#include "myftp/utils.h"

//
// Tells whether a whole string is a number
//
bool str_is_num(char const *s)
{
    for (size_t i = 0; s[i]; i++)
        if (!isdigit(s[i]))
            return false;
    return true;
}
