#include <libc/definitions.h>
#include <command-line/bigstring.h>
#include <command-line/parser.h>

bigstring *input;
Parser *p;

void commandline_init(void) {
  input = NewBigString();
}

void system(bigstring *string) {
  int i;
  char *str = to_string(string);
  p = NewParser(str);
  string = NextToken(p);
  FreeParser(p);
  if (string == NULL) {
    printf("\n");
    return;
  }
  for (i=0;i!=string->used;i++) {
    kprint_char(string->string[i]);
  }
  printf("\n");
}

void update(void) {
  int in = getchar();
  if (in == (int)'\b') {
    popchar(input);
  } else {
    if (in != -1 && in != (int) '\n') {
      appendchar(input, (char) in);
    }
  }
  if (in == (int)'\n') {
    system(input);
    clear(input);
  }
}
