#include <drivers/screen.h>
#include <cpu/ports.h>
#include <libc/mem.h>
#include <libc/definitions.h>
#include <stdint.h>
#include <stdarg.h>

/* Declaration of private functions */
int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

/**********************************************************
 * Public Kernel API functions                            *
 **********************************************************/

/**
 * Print a message on the specified location
 * If col, row, are negative, we will use the current offset
 */

int printf(const char *format, ...) {
  va_list argp;
  char var[32];
  char *str;
  va_start(argp, format);
  while (*format) {
    if (*format == '%') {
      format++;
      if (*format == '%') {
        kprint_char('%');
      } else if (*format == 'c') {
        kprint_char(va_arg(argp, int));
      } else if (*format == 's') {
        str = (char *) va_arg(argp, uintptr_t);
        if (str == NULL) {
          kprint("<null>");
        } else {
          kprint(str);
        }
      } else if (*format == 'd') {
        int_to_ascii(va_arg(argp, int), var);
        kprint(var);
      } else {
        printf("<Unknown format specifier '%c'>", *format);
      }
    } else {
      kprint_char(*format);
    }
    format++;
  }
  va_end(argp);
  return 0;
}

void kprint_at(char *message, int col, int row) {
  /* Set cursor if col/row are negative */
  int offset;
  int i;
  if (col >= 0 && row >= 0)
    offset = get_offset(col, row);
  else {
    offset = get_cursor_offset();
    row = get_offset_row(offset);
    col = get_offset_col(offset);
  }

  /* Loop through message and print it */
  i = 0;
  while (message[i]) {
    offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
    /* Compute row/col for next iteration */
    row = get_offset_row(offset);
    col = get_offset_col(offset);
  }
}

void kprint(char *message) {
  kprint_at(message, -1, -1);
}

void kprint_char(char c) {
  char *str = "E";
  str[0] = c;
  kprint(str);
}

void kprinti(int value) {
  char var[32];
  int_to_ascii(value, var);
  kprint(var);
  kprint("\n");
}

void kprint_backspace(void) {
  int offset = get_cursor_offset()-2;
  int row = get_offset_row(offset);
  int col = get_offset_col(offset);
  print_char(0x08, col, row, WHITE_ON_BLACK);
}

void _kprint_char(char c) {
  int offset = get_cursor_offset()-2;
  int row = get_offset_row(offset);
  int col = get_offset_col(offset);
  print_char(c, col, row, WHITE_ON_BLACK);
}

/**************************
 *Private kernel functions*
 **************************/


/**
 * Innermost print function for our kernel, directly accesses the video memory
 *
 * If 'col' and 'row' are negative, we will print at current cursor location
 * If 'attr' is zero it will use 'white on black' as default
 * Returns the offset of the next character
 * Sets the video cursor to the returned offset
 */
int print_char(char c, int col, int row, char attr) {
  int i;
  int offset;
  char *last_line;
  uint8_t *vidmem = (uint8_t*) VIDEO_ADDRESS;
  if (!attr) attr = WHITE_ON_BLACK;

  /* Error control: print a red 'E' if the coords aren't right */
  if (col >= MAX_COLS || row >= MAX_ROWS) {
    vidmem[2*(MAX_COLS)*(MAX_ROWS)-2] = 'E';
    vidmem[2*(MAX_COLS)*(MAX_ROWS)-1] = RED_ON_WHITE;
    return get_offset(col, row);
  }

  if (col >= 0 && row >= 0) offset = get_offset(col, row);
  else offset = get_cursor_offset();

  if (c == '\n') {
    row = get_offset_row(offset);
    offset = get_offset(0, row+1);
  } else if (c == '\b') { /* Backspace */
    offset-=2;
    vidmem[offset] = ' ';
    vidmem[offset+1] = attr;
  } else {
    vidmem[offset] = c;
    vidmem[offset+1] = attr;
    offset += 2;
  }

  /* Check if the offset is over screen size and scroll */
  if (offset >= MAX_ROWS * MAX_COLS * 2) {
    for (i = 1; i < MAX_ROWS; i++)
      memcpy((uint8_t*)(get_offset(0, i-1) + VIDEO_ADDRESS),
             (uint8_t*)(get_offset(0, i  ) + VIDEO_ADDRESS),
             MAX_COLS * 2);

    /* Blank last line */
    last_line = (char*) (get_offset(0, MAX_ROWS-1) + (uint8_t*) VIDEO_ADDRESS);
    for (i = 0; i < MAX_COLS * 2; i++) last_line[i] = 0;

    offset -= 2 * MAX_COLS;
  }

  set_cursor_offset(offset);
  return offset;
}

int get_cursor_offset(void) {
  /* Use the VGA ports to get the current cursor position
   * 1. Ask for high byte of the cursor offset (data 14)
   * 2. Ask for low byte (data 15)
   */
  int offset;
  port_byte_out(REG_SCREEN_CTRL, 14);
  offset = port_byte_in(REG_SCREEN_DATA) << 8; /* High byte: << 8 */
  port_byte_out(REG_SCREEN_CTRL, 15);
  offset += port_byte_in(REG_SCREEN_DATA);
  return offset * 2; /* Position * size of character cell */
}

void set_cursor_offset(int offset) {
  /* Similar to get_cursor_offset, but instead of reading we write data */
  offset /= 2;
  port_byte_out(REG_SCREEN_CTRL, 14);
  port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset >> 8));
  port_byte_out(REG_SCREEN_CTRL, 15);
  port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset & 0xff));
}

void clear_screen(void) {
  int screen_size = MAX_COLS * MAX_ROWS;
  int i;
  uint8_t *screen = (uint8_t*) VIDEO_ADDRESS;

  for (i = 0; i < screen_size; i++) {
    screen[i*2] = ' ';
    screen[i*2+1] = WHITE_ON_BLACK;
  }
  set_cursor_offset(get_offset(0, 0));
}


int get_offset(int col, int row) {
  return 2 * (row * MAX_COLS + col);
}
int get_offset_row(int offset) {
  return offset / (2 * MAX_COLS);
}
int get_offset_col(int offset) {
  return (offset - (get_offset_row(offset)*2*MAX_COLS))/2;
}
