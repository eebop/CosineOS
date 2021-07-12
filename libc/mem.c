#include <libc/mem.h>
#include <libc/definitions.h>

#define sizesize (4)
#define beforesize (4)
#define typesize (8)
#define allsize (8)
#define getsize(ptr) ((size_t *) ptr-sizesize)
#define gettype(ptr) ((uint8_t *) ptr-typesize)

void *memcpy(uint8_t *dest, uint8_t *source, size_t nbytes) {
  size_t i;
  for (i = 0; i < nbytes; i++) {
    *(dest + i) = *(source + i);
  }
  return source;
}

void *memmove(uint8_t *dest, uint8_t *source, size_t nbytes) {
  size_t i;
  /* we have to do a double cast because of the C99 standard */
  if ((uintptr_t) (void *) source < (uintptr_t) (void *) dest) {
    for (i = 0; i < nbytes; i++) {
      *(dest + i) = *(source + i);
    }
  } else {
    for (i = nbytes-1; i+1 > 0; i--) {
      *(dest + i) = *(source + i);
    }
  }
  return source;
}

void *memset(uint8_t *dest, uint8_t val, size_t len) {
  uint8_t *temp = dest;
  for (; len != 0; len--) *temp++ = val;
  return dest;
}

int memcmp(uint8_t *first, uint8_t *last, size_t nbytes) {
  size_t i;
  for (i=0; i!=nbytes; i++) {
    if (*(first + i) != *(last + i)) {
      return *(first + i) - *(last + i);
    }
  }
  return 0;
}

/* This should be computed at link time, but a hardcoded
 * value is fine for now. Remember that our kernel starts
 * at 0x1000 as defined on the Makefile */
#define MALLOCSTART (uint8_t *) 0x800000


void kmsetup(void) {
  *((uint8_t *) MALLOCSTART - typesize) = 0x55;
  *((size_t  *) MALLOCSTART - sizesize) = 0;
}


/* format:
 *
 * type  buf size data
 * [  metadata  ] [data]
 *                ptr
 *
 *
 * type:
 *      0x00 = free
 *      0xFF = alloced
 *      0x55 = begin unused area
 *
 * size:
 *      a size_t of the amount of allocated area
 *
 * data:
 *      user data */

/* TODO: Add a lock because malloc() is not threadsafe (yet) */
void *malloc(size_t size) {
  uint8_t *ptr = MALLOCSTART;
  /*round size up to the nearest multiple of four*/
  size = (size & ~3) + (4 & (size<<1 | size<<2)) + allsize;
  while (*gettype(ptr) == 0xFF || (*getsize(ptr) < size && *gettype(ptr) == 0x00)) {
    ptr += *getsize(ptr);
    ptr += allsize;
  }
  if (*(gettype(ptr)) == 0x55) {
    *((uint8_t *) ptr+size    ) = 0x55;
    *((size_t  *) ptr-sizesize) = size;
  } else {
    if ((*getsize(ptr)) - size > allsize) {
      *((uint8_t *) ptr+size) = 0x00;
      *((size_t  *) ptr+size+beforesize) = *getsize(ptr) - (size+typesize);
      *((size_t  *) ptr-sizesize) = size;
    }
  }
  *gettype(ptr) = 0xFF;
  return (void *) ptr;
}


void *realloc(void *ptr, size_t size) {
  void *new;
  if (size < *getsize(ptr)) {
    new = malloc(size);
    memcpy(new, ptr, size);
    free(ptr);
    return new;
  } else {
    return ptr; /* not worth it */
  }
}

void free(void *ptr) {
  /* gettype is a macro */
  *((uint8_t *)ptr - typesize) = 0x00;
}


/*
 * Implementation is just a pointer to some free memory which
 * keeps growing *
uint32_t malloc_mop(size_t size, int align, uint32_t *phys_addr) {
  * Pages are aligned to 4K, or 0x1000 *
  char str[32];
  uint32_t ret;
  int_to_ascii(free_mem_addr, str);
  kprint(str);
  if (align == 1 && (free_mem_addr & 0x00000FFF)) {
    free_mem_addr &= 0xFFFFF000;
    free_mem_addr += 0x1000;
  }
  * Save also the physical address *
  if (phys_addr) *phys_addr = free_mem_addr;

  ret = free_mem_addr;
  free_mem_addr += size; * Remember to increment the pointer *
  return ret;
}
*/
