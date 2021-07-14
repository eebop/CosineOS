#ifndef _DEFINITIONS_H
#define _DEFINITIONS_H
#ifndef NULL
#define NULL ((void *) 0)
#endif
int getchar(void);
void kprint(char *message);
void int_to_ascii(int n, char str[]);
#include <libc/mem.h>
#include <drivers/screen.h>
#endif
