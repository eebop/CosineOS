#include <libc/definitions.h>

struct Data {
  char *string;
  int used;
  int max;
};

typedef struct Data Data;

Data *input;

void commandline_init(void) {
  input = malloc(sizeof(Data));
  input->used = 0;
  input->max = 32;
  input->string = malloc(sizeof(char) * input->max);
}

void appendchar(Data *string, char in) {
  if (string->used == string->max) {
    string->max *= 2;
    realloc(string->string, string->max);
  }
  string->string[string->used++] = in;
}

void popchar(Data *string) {
  string->used--;
}

void clear(Data *string) {
  string->max = 32;

  string->used = 0;
  free(string->string);
  input->string = malloc(sizeof(char) * input->max);
}

void system(Data *string) {
  int i;
  for (i=0;i!=string->used;i++) {
    kprint_char(string->string[i]);
  }
}

void update(void) {
  int in = getchar();
  if (in == (int)'\b') {
    popchar(input);
  } else {
    if (in != -1) {
      appendchar(input, (char) in);
    }
  }
  if (in == (int)'\n') {
    system(input);
    clear(input);
  }
}
