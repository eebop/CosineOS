#include <cpu/timer.h>
#include <cpu/isr.h>
#include <cpu/ports.h>
#include <libc/definitions.h>

uint32_t tick = 0;
uint32_t divisor = 0;

static void timer_callback(registers_t *regs) {
  every_tick(tick);
  tick++;
}

void init_timer(uint32_t freq) {
  uint8_t low;
  uint8_t high;
  /* Install the function we just wrote */
  register_interrupt_handler(IRQ0, timer_callback);

  /* Get the PIT value: hardware clock at 1193180 Hz */
  divisor = 1193180 / freq;
  low  = (uint8_t)(divisor & 0xFF);
  high = (uint8_t)( (divisor >> 8) & 0xFF);
  /* Send the command */
  port_byte_out(0x43, 0x36); /* Command port */
  port_byte_out(0x40, low);
  port_byte_out(0x40, high);
}
