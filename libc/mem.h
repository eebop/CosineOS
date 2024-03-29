#ifndef MEM_H
#define MEM_H

#include <stdint.h>
#include <stddef.h>

void *memcpy(uint8_t *dest, uint8_t *source, size_t nbytes);
void *memset(uint8_t *dest, uint8_t val, uint32_t len);

void kmsetup(void);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
#endif
