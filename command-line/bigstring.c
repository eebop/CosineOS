#include <command-line/bigstring.h>
#include <libc/definitions.h>

bigstring *NewBigString(void) {
  bigstring *input = (bigstring *) malloc(sizeof(bigstring));
  input->used = 0;
  input->max = 32;
  input->string = malloc(sizeof(char) * input->max);
  return input;
}

void appendchar(bigstring *string, char in) {
  if (string->used == string->max) {
    string->max *= 2;
    realloc(string->string, string->max);
  }
  string->string[string->used++] = in;
}

void popchar(bigstring *string) {
  string->used--;
}

char *to_string(bigstring *string) {
  string->string[string->used] = '\0';
  return string->string;
}

void clear(bigstring *string) {
  string->max = 32;

  string->used = 0;
  free(string->string);
  string->string = malloc(sizeof(char) * string->max);
}
