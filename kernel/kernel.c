#include <cpu/memory.h>
#include <cpu/isr.h>
#include <drivers/screen.h>
#include <kernel/kernel.h>
#include <libc/string.h>
#include <libc/mem.h>
#include <stdint.h>
#include <libc/definitions.h>
#include <cpu/timer.h>
#include <command-line/main.h>

uint32_t tick;

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
  commandline_init();
  run();
}

void run(void) {
  uint32_t old_tick = tick;
  while (1) {
    if (tick - old_tick > 3) {
      update();
      old_tick = tick;
    }
  }
}
