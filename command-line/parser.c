#include <command-line/parser.h>
#include <libc/definitions.h>
#include <libc/string.h>
#include <command-line/bigstring.h>

Parser *NewParser(char *string) {
  Parser *par = (Parser *) malloc(sizeof(Parser));
  par->str = (char *) malloc(sizeof(char) * strlen(string)+1);
  memcpy((uint8_t *) par->str, (uint8_t *) string, strlen(string)+1);
  par->index = 0;
  return par;
}

bigstring *NextToken(Parser *par) {
  bigstring *str = NewBigString();
  char type = ' ';
  int backslash = 0;
  if (par->str[par->index] == '\0') {
    return NULL;
  }
  if (par->str[par->index] == '\'') {
    type = '\'';
  } else if (par->str[par->index] == '"') {
    type = '"';
  }
  if (type != ' ') {
    par->index++;
  }
  while (par->str[par->index]) {
    if (backslash) {
      backslash = 0;
      appendchar(str, par->str[par->index++]);
    } else {
      if (par->str[par->index] == '\\') {
        backslash = 1;
      }
      if (par->str[par->index] == type) {
        break;
      }
      appendchar(str, par->str[par->index]);
    }
    par->index++;
  }
  while (par->str[par->index]==' ') par->index++;
  return str;
}

void FreeParser(Parser *par) {
  free(par->str);
  free(par);
}
