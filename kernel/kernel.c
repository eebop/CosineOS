#include <cpu/memory.h>
#include <cpu/isr.h>
#include <drivers/screen.h>
#include <kernel/kernel.h>
#include <libc/string.h>
#include <libc/mem.h>
#include <stdint.h>
#include <libc/definitions.h>

void start(void) {
  isr_install();
  irq_install();
# ifdef DEBUG
  asm("int $2");
  asm("int $3");
# else
  clear_screen();
# endif
  kmsetup();
  kprint("> ");
}

void every_tick(int tick) {
  char *var1;
  char *var2;
  if ((tick % 6) == 0) {
    if (getchar() != -1) {
      /*printf("blag\n", 'a');*/

      var1 = (char *) kmalloc(sizeof(char) * (strlen("Hello, World!") + 1));
      memcpy((uint8_t *) var1, (uint8_t *) "Hello, World!", (strlen("Hello, World!")+1));
      var2 = (char *) kmalloc(sizeof(char) * (strlen("Hello, W_rld!") + 1));
      memcpy((uint8_t *) var2, (uint8_t *) "Hello, W_rld!", (strlen("Hello, W_rld!")+1));
      kprinti((int) var1);
      kprinti((int) var2);
      kprint(var1);
      kprint("\n");
      kprint(var2);
      kprint("\n");

      kfree(var1);
      kfree(var2);
    }
  }
}
