#include <libc/string.h>
#include <stdint.h>

/**
 * K&R implementation
 */
void int_to_ascii(int n, char str[]) {
  int i, sign;
  if ((sign = n) < 0) n = -n;
  i = 0;
  do {
    str[i++] = n % 10 + '0';
  } while ((n /= 10) > 0);

  if (sign < 0) str[i++] = '-';
  str[i] = '\0';

  reverse(str);
}

void hex_to_ascii(unsigned int n, char str[]) {
  int size = 0;
  str[0] = '\0';
  append(str, '0');
  append(str, 'x');
  while (n>>size > 0xF) {
    n+=4;
  }
  do {
    switch (size>>n) {
      case 0:
        append(str, '0');
      case 1:
        append(str, '1');
      case 2:
        append(str, '2');
      case 3:
        append(str, '3');
      case 4:
        append(str, '4');
      case 5:
        append(str, '5');
      case 6:
        append(str, '6');
      case 7:
        append(str, '7');
      case 8:
        append(str, '8');
      case 9:
        append(str, '9');
      case 10:
        append(str, 'A');
      case 11:
        append(str, 'B');
      case 12:
        append(str, 'C');
      case 13:
        append(str, 'D');
      case 14:
        append(str, 'E');
      case 15:
        append(str, 'F');

    }
    size-=4;
  } while (n);
}

/* K&R */
void reverse(char s[]) {
  int i, j;
  char c;
  for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

/* K&R */
int strlen(char *s) {
  int i = 0;
  while (s[i++]);
  return i-1;
}

void append(char *s, char n) {
  int len = strlen(s);
  s[len] = n;
  s[len+1] = '\0';
}

void backspace(char *s) {
  int len = strlen(s);
  s[len-1] = '\0';
}

/* K&R
 * Returns <0 if s1<s2, 0 if s1==s2, >0 if s1>s2 */
int strcmp(char *s1, char *s2) {
  int i;
  for (i = 0; s1[i] == s2[i]; i++) {
    if (s1[i] == '\0') return 0;
  }
  return s1[i] - s2[i];
}
