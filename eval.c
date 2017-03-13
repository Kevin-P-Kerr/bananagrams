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

void walkVertical(struct Node *g, struct StrContainer *strContainer) {
  // remember where we started from
  struct Node *initNodeAddr = g;
  // walk as far up as possible
  while (g->u != NULL) {
    g = g->u;
  }
  struct Str * s =  initStr(); 
    while (g != NULL) {
    addChar(s,g->c);
    if (g != initNodeAddr) {
      if (g->h != NULL || g->l != NULL) {
        walkHorizontal(g,strContainer);
      }
    }
    g = g->v;
  }
  addStr(s,strContainer);
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

void walkHorizontal(struct Node *g, struct StrContainer *strContainer) {
  struct Node *initNodeAddr = g;
  // walk as far left as possible
  while (g->l != NULL) {
    g = g->l;
  }
  struct Str * s = initStr(); 
  while (g!= NULL) {
    addChar(s,g->c);
    if (g != initNodeAddr) {
      if (g->u != NULL || g->v != NULL) {
        walkVertical(g,s);
      }
    }
    g = g->h;
  }
  addStr(s,strContainer);
};

int eval(struct Node *g) {
  struct StrContainer sc =  initStrContainer();
};


int correct(struct Node *g) {
  fixup(g);
  return eval(g);
};
