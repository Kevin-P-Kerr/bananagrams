#include <stdio.h>
#include <stdlib.h>
#include "dict.h"
#include "eval.h"

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

// define prototype for mutual recursion
void walkHorizontal(struct Node *g, struct StrContainer *s, int recurred);

void walkVertical(struct Node *g, struct StrContainer *strContainer) {
  // remember where we started from
  struct Node *initNodeAddr = g;
  // walk as far up as possible
  while (g->u != NULL) {
    g = g->u;
  }
  struct Str *s =  initStr(); 
  while (g != NULL) {
    addChar(s,g->c);
    if (g != initNodeAddr) {
      if (g->h != NULL || g->l != NULL) {
        walkHorizontal(g,strContainer,1);
      }
    }
    g = g->v;
  }
  addStr(s,strContainer);
};

struct StrContainer *walk (struct Node *g) {
  struct StrContainer *s = initStrContainer();
  if (g->h != NULL) {
    walkHorizontal(g,s,0);
  }
  else {
    fprintf(stderr,"ho! %c\n",g->c);
    walkVertical(g,s);
  }
  return s;
};

int level_db = 0;
void walkHorizontal(struct Node *g, struct StrContainer *strContainer, int recurred) {
  level_db++;
  struct Node *initNodeAddr = g;
  // walk as far left as possible
  while (g->l != NULL) {
    g = g->l;
  }
  struct Str * s = initStr(); 
  while (g!= NULL) {
    if (g != initNodeAddr || !recurred) {
      recurred = 1;
      if (g->u != NULL || g->v != NULL) {
        walkVertical(g,strContainer);
      }
    }
    if (level_db ==1) {
      fprintf(stderr,"%c\n",g->c);
    }
    addChar(s,g->c);
    g = g->h;
  }
  addStr(s,strContainer);
  level_db--;
};

int eval(struct Node *g, struct Node *dict) {
  struct StrContainer *s = walk(g);
  int i,ii;
  int v= 1;
  ii = s->size-1;
  for (i=0;i<ii;i++) {
    struct Str *w = &s->l[i];
    char *str = getStr(w);
    if (!isWord(dict,str)) {
      v=0;
      fprintf(stderr,"%s is not a word\n",str);
    }
    else {
      fprintf(stderr,"%s is a word\n",str);
    }
  }
  fprintf(stderr,"done\n");
  return v;
};

int correct(struct Node *g, struct Node *dict) {
  return eval(g, dict);
};
