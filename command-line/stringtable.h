#ifndef _HASH_H
#define _HASH_H

typedef struct StringTable StringTable;

struct StringTable {
  void *element;
  StringTable **subs;
};

StringTable *NewStringTable(void);

void *getElement(StringTable *tab, char *str);

void  setElement(StringTable *tab, char *str, void *element);

void *delElement(StringTable *tab, char *str);

void FreeStringTable(StringTable *tab);

#endif
