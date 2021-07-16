#include <command-line/stringtable.h>
#include <libc/definitions.h>

/* Simple prefix table */

StringTable *NewStringTable(void) {
  int i;
  StringTable *tab = (StringTable *) malloc(sizeof(StringTable));
  tab->element = NULL;
  tab->subs = malloc(sizeof(StringTable *) * 256);
  for (i=0;i!=256;i++) {
    tab->subs[i] = NULL;
  }
  return tab;
}


void *getElement(StringTable *tab, char *str) {
  for (;*str;str++) {
    if ((tab=tab->subs[(int) *str]) == NULL) {
      return NULL;
    }
  }
  return tab->element;
}

void setElement(StringTable *tab, char *str, void *element) {
  for (;*str;str++) {
    if ((tab->subs[(int) *str]) == NULL) {

      tab->subs[(int) *str] = NewStringTable();
    }
    tab = tab->subs[(int) *str];
  }
  tab->element = element;
}

/* Note: You must independly free the objects deleted */

void *delElement(StringTable *tab, char *str) {
  void *new;
  for (;*str;str++) {
    if ((tab=tab->subs[(int) *str]) == NULL) {
      return NULL;
    }
  }
  new = tab->element;
  tab->element = NULL;
  return new;
}

/* Note: Only guarented to not leave some allocated space if all entries are delled first*/

void FreeStringTable(StringTable *tab) {
  int i;
  if (tab == NULL) {
    return;
  }
  for (i=0;i!=256;i++) {
    FreeStringTable(tab->subs[i]);
  }
  free(tab->subs);
  free(tab);
}
