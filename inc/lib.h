#pragma once

#include <stddef.h>
#include <stdint.h>

// stdlib.h
__attribute__((noreturn)) void abort(void);

// stdio.h
#define EOF (-1)
int printf(const char *, ...);
int putchar(int);
int puts(const char *);

// string.h
int memcmp(const void *, const void *, size_t);
void *memcpy(void *, const void *, size_t);
void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);
size_t strlen(const char *);
