#ifndef _PARSER_H
#define _PARSER_H
#include <command-line/bigstring.h>

typedef struct {
  char *str;
  int index;
} Parser;

Parser *NewParser(char *string);

bigstring *NextToken(Parser *par);

void FreeParser(Parser *par);

#endif
