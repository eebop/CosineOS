#ifndef _DEFINITIONS_H
#define _DEFINITIONS_H
#ifndef NULL
#define NULL ((void *) 0)
#endif
void every_tick(int tick);
int getchar(void);
void kprint(char *message);
void int_to_ascii(int n, char str[]);
void kprint_dont_reset(char *message);
void kreset(void);
#include <drivers/screen.h>
#endif
