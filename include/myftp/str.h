/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** str utils to manage strings and vector strings array
*/

#pragma once

#include <stddef.h>
#include <stdbool.h>

//
// Tells whether a whole string is a number
//
bool str_is_num(char const *s);

//
// Returns the size of the given vector array
//
size_t vector_array_len(char **array);

//
// Returns a new vector array with a new string added
// at the front of the vector and frees the old one
//
// You can chose to duplicate the string toadd passed as parameter or
// just to copy the given pointer
//
char **vector_array_append_front(char **array, char *toadd, int dup_to_add);

//
// Returns a new vector array with a new string added
// at the end of the vector and frees the old one
//
// You can chose to duplicate the string toadd passed as parameter or
// just to copy the given pointer
//
char **vector_array_append_back(char **array, char *toadd, int dup_to_add);

//
// Frees the memory allocated to the given vector string array
//
void vector_array_free(char **array);

//
// Behaves likes the function strdup
// but if strndup returns NULL
// The program exit(84) and print a message in stderr
//
char *xstrndup(char const *s, size_t n);

//
// Behaves likes the function strdup
// but if strdup returns NULL
// The program exit(84) and print a message in stderr
//
char *xstrdup(char const *s);

//
// Returns a splitted string
// with the given tokens
// If there was an allocation error prints it in the standard output
// and then exit(84)
//
char **str_split(char const *raw, char const *tokens);
