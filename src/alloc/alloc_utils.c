/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** Utilities for lazy allocators
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "myftp/alloc.h"

//
// Behave like the malloc function except that
// If the allocation fails it exits the program and returns 84
//
void *xmalloc(size_t size)
{
    void *ptr = malloc(size);

    if (ptr == NULL) {
        perror("ftp: Allocation failed");
        exit(84);
    }
    return ptr;
}

//
// Behave like the calloc function except that
// If the allocation fails it exits the program and returns 84
//
void *xcalloc(size_t size, size_t nmemb)
{
    return memset(xmalloc(size * nmemb), 0, size * nmemb);
}

//
// Behave like the realloc function except that
// If the allocation fails it exits the program and returns 84
//
void *xrealloc(void *p, size_t size)
{
    void *ptr = realloc(p, size);

    if (ptr == NULL) {
        perror("ftp: Reallocation failed");
        exit(84);
    }
    return ptr;
}
