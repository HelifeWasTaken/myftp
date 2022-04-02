/*
** EPITECH PROJECT, 2022
** myftp
** File description:
** allocators
*/

#pragma once

#include <stddef.h>

//
// Behave like the malloc function except that
// If the allocation fails it exits the program and returns 84
//
void *xmalloc(size_t size);

//
// Behave like the calloc function except that
// If the allocation fails it exits the program and returns 84
//
void *xcalloc(size_t size, size_t nmemb);

//
// Behave like the realloc function except that
// If the allocation fails it exits the program and returns 84
//
void *xrealloc(void *p, size_t size);
