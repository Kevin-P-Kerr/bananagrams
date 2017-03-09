#include <stdlib.h>
#include "dict.h"

struct StrContainer {
  struct Str *l;
  int size;
};

struct StrContainer *initStrContainer () {
  struct StrContainer * sc= malloc(sizeof(struct StrContainer));
  sc->size = 1;
  sc->l = malloc(sizeof(struct Str));
  return sc;
};

void addStr(struct Str *s, struct StrContainer *sc) {
  sc->l[sc->size-1] = *s;
  sc->size++;
  sc->l = realloc(sc->l,sizeof(struct Str)*sc->size);
};

struct StrContainer *walk (struct Node *g) {
  struct StrContainer *s = initStrContainer();
  if (g->h != NULL) {
    walkHorizontal(g,s);
  }
  else {
    walkVertically(g,s);
  }
  return s;
};

void walkHorizontal(struct Node *g, struct StrContainer *s) {
  struct Str * s =  malloc(sizeof(struct Node));
  while (g!= NULL) {
    addChar(s,g->c);
    if (g->up != NULL) {
      walkVertical(g,s);
    }
    if (g->down != NULL) {
      walkVertical(g,s);
    }
    g = g->h;
  }
};

int eval(struct Node *g) {
 struct Str *s  = walkHorizontal(g);
 struct Str *v  = walkVertical(g);
};


int correct(struct Node *g) {
  fixup(g);
  return eval(g);
};
