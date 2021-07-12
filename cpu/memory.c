#include <stdint.h>
#include <libc/definitions.h>

void switch_to_pm(void);

void reset(void) {
  asm volatile (
    "cli               \n"
    "mov %%cr0,   %%eax\n"
    "xor $0x01,  %%al\n"
    "mov %%eax,   %%cr0\n"
    "mov $0x00,    %%ah\n"
    "mov $0x00,    %%dh\n"
    "sti               \n"
    "int $0x13         \n"
    "cli               \n"
    "mov %%cr0,   %%eax\n"
    "or  $0x1,     %%al\n"
    "mov %%eax,   %%cr0\n"
    "sti               \n"

  : : : "%ah", "%dh"
  );

}

char *read_512(int cylinder, int sector) {
  char *out = NULL;
  uint32_t cylinder_sector_code;
  uint32_t segment;
  uint32_t offset;
  if (cylinder > 1023 || cylinder < 0) return NULL; /*invalid cylinder*/
  if (sector > 63 || sector < 0) return NULL; /*invalid sector*/
  /*reset();*/
  cylinder_sector_code = sector | (cylinder << 6);
  segment = ((uint32_t) out) >>  4;
  offset  = ((uint32_t) out) & 0xF;
  asm volatile (
    "cli              \n"
    "mov  %%cr0, %%edx\n"
    "xor  $0x01, %%dl \n"
    "mov  %%edx, %%cr0\n"
    "sti              \n"
    "mov  %%bx , %%cx \n"
    "mov  %%eax, %%es \n"
    "mov  %%cx , %%bx \n"
    "mov  $0x02, %%ah \n"
    "mov  $0x01, %%al \n"
    "mov  $0x00, %%dx \n"
    "int  $0x13       \n"
    "call *%P0        \n"
    : : "a" (segment), "c" (offset), "b" (cylinder_sector_code), "i" (switch_to_pm) : "%edx"
  );
  out[512] = '\0';
  return out;
}
