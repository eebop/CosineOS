#ifndef _BIGSTRING_H
#define _BIGSTRING_H
struct bigstring {
  char *string;
  int used;
  int max;
};

typedef struct bigstring bigstring;

bigstring *NewBigString(void);

void appendchar(bigstring *string, char in);

void popchar(bigstring *string);

void clear(bigstring *string);

char *to_string(bigstring *string);

#endif
