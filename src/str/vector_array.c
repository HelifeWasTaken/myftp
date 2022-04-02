/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** array management
*/

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "myftp/str.h"
#include "myftp/alloc.h"

//
// Returns the size of the given vector array
// Handles null values (return 0)
//
size_t vector_array_len(char **array)
{
    size_t i = 0;

    if (array)
        for (; array[i]; i++);
    return i;
}

//
// Returns a new vector array with a new string added
// at the front of the vector and frees the old one
//
// You can chose to duplicate the string toadd passed as parameter or
// just to copy the given pointer
//
char **vector_array_append_front(char **array, char *toadd, int dup_to_add)
{
    const size_t array_len = vector_array_len(array);
    char **narray = xcalloc(sizeof(char *), array_len + 2);

    if (array_len)
        for (size_t i = 1; array[i - 1]; i++)
            narray[i] = array[i - 1];
    narray[0] = dup_to_add ? xstrdup(toadd) : toadd;
    free(array);
    return narray;
}

//
// Returns a new vector array with a new string added
// at the end of the vector and frees the old one
//
// You can chose to duplicate the string toadd passed as parameter or
// just to copy the given pointer
//
char **vector_array_append_back(char **array, char *toadd, int dup_to_add)
{
    const size_t array_len = vector_array_len(array);

    array = xrealloc(array, sizeof(char *) * (array_len + 2));
    array[array_len] = dup_to_add ? xstrdup(toadd) : toadd;
    array[array_len + 1] = NULL;
    return array;
}

//
// Frees the memory allocated to the given vector string array
//
void vector_array_free(char **array)
{
    if (array) {
        for (size_t i = 0; array[i]; i++)
            free(array[i]);
        free(array);
    }
}

//
// Returns a splitted string
// with the given tokens
// If there was an allocation error prints it in the standard output
// and then exit(84)
//
char **str_split(char const *raw, char const *tokens)
{
    char **new_vec = NULL;
    char *copy = xstrdup(raw);
    char *old = copy;

    for (char *item = strtok(copy, tokens); item; item = strtok(NULL, tokens))
        new_vec = vector_array_append_back(new_vec, item, true);
    free(old);
    return new_vec;
}
