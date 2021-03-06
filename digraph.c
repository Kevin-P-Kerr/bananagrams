#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dict.h"

struct Str *initStr() {
  struct Str *s = malloc(sizeof(struct Str));
  s->c = malloc(sizeof(char));
  s->len = 1;
  return s;
}

void addTerminalChar(struct Str *str) {
  char c = '\0';
  str->c[str->len-1] = c;
  str->len++;
  str->c = realloc(str->c, sizeof(char)*str->len);
};

char *getStr(struct Str *str) {
  addTerminalChar(str);
  return str->c;
};

int isValidChar(char c) { 
  return !(c!='[' && c!=']' && c!= ')' && c != '(' && c != '$' && c != '{' && c != '}');
};

void addChar(struct Str *str, char c) {
  if (!isalpha(c) && !isValidChar(c)){
    if (DEBUG) {
      fprintf(stderr,"warning: illegal char in string: %c\n", c);
    }
    return;
  }
  str->c[str->len-1] = c;
  str->len++;
  str->c = realloc(str->c, sizeof(char)*str->len);
};

void addNodeChar(struct Str *str, struct Node *n) {
  if (n->isWord) {
    addChar(str,'$');
  }
  addChar(str,n->c);
};

struct Str*recurSerialize(struct Str *str, struct Node *dict) {
  addNodeChar(str,dict);
  if (dict->u != NULL) {
    addChar(str,'{');
    recurSerialize(str,dict->u);
    addChar(str,'}');
  }
  if (dict->l != NULL) {
    addChar(str,'[');
    recurSerialize(str,dict->l);
    addChar(str,']');
  }
  if (dict->v != NULL) {
    addChar(str,'(');
    recurSerialize(str,dict->v);
    addChar(str,')');
  }
  if (dict->h != NULL) {
    recurSerialize(str,dict->h);
  }
  return str;
}

struct Str * serialize(struct Node *dict) {
  struct Str *s = initStr();
  addChar(s,'(');
  recurSerialize(s,dict);
  addChar(s,')');
  return s;
}

int isWhite(int c) {
  if (c == ' ' || c == '\t' || c == '\n') {
    return 1;
  }
  return 0;
}

int getNext(FILE *f) {
  int c;
  while (isWhite((c=fgetc(f)))) {
    continue;
  }
  return c;
}

struct Node* initBlankNode () { 
  struct Node *n = malloc(sizeof(struct Node));
  n->h = NULL;
  n->v = NULL;
  n->l = NULL;
  n->u = NULL;
  n->isWord = 0;
  return n;
}

struct Node* initNode (char c) {
  struct Node *n = initBlankNode();
  n->c = c;
  return n;
}

struct Node *parse(FILE *f, char peek) {
  struct Node *base = initBlankNode();
  struct Node *n = base;
  // a b (c d e))
  // c (a (t))
  if (peek == ')') {
    return NULL;
  }
  if (peek == '}') {
    return NULL;
  }
  if (peek == ']') {
    return NULL;
  }
  if (peek == '$') {
    n->isWord = 1;
    peek = getNext(f);
  }
  if (isalpha(peek)) {
    n->c = peek;
    peek = getNext(f);
  }
  // set up next node
  while (peek == '(' || peek == '{' || peek == '[') {
    if (peek == '(') {
      n->v = parse(f,getNext(f));
      peek = getNext(f);
    }
    if (peek == '{') {
      n->u = parse(f,getNext(f));
      peek = getNext(f);
    }
    if (peek == '[') {
      n->l = parse(f,getNext(f));
      peek = getNext(f);
    }
  }
  if (isalpha(peek) || peek == '$') {
    n->h = parse(f,peek);
  }
  return base;
};

void doubleLink (struct Node *g) { 
  if (g->u != NULL && g->u->v == NULL) {
    g->u->v = g;
    doubleLink(g->u);
  }
  if (g->v != NULL && g->v->u == NULL) {
    g->v->u = g;
    doubleLink(g->v);
  }
  if (g->l != NULL && g->l->h == NULL) {
    g->l->h = g;
    doubleLink(g->l);
  }
  if (g->h != NULL && g->h->l == NULL) {
    g->h->l = g;
    doubleLink(g->h);
  }
};

struct Node *deserialize(FILE *f) {
  //(a b c)
  // a b c)
  fgetc(f);
  return parse(f,getNext(f));
}

// look up horizontally or fail
struct Node* lookup(struct Node* n, char c) {
  if (n == NULL) {
    return NULL;
  }
  while (n->c != c) {
    if (n->h == NULL) {
      return NULL;
    }
    n = n->h;
  }
  return n;
}

struct Node *lookupOrAdd(struct Node*n, char c) {
  struct Node *attempt = lookup(n,c);
  if (attempt != NULL) {
    return attempt;
  }
  // get to the end of n
  while (n->h != NULL) {
    n = n->h;
  }
  n->h = initNode(c);
  return n->h;
}

struct Node* initStandardDict() {
  FILE *f = fopen("/usr/share/dict/american-english","r");
  struct Node *base =  initBlankNode();
  int c;
  while ((c = fgetc(f)) != EOF) {
    struct Node *head = lookupOrAdd(base,c);
    while ((c = fgetc(f)) != '\n') {
      if (head->v == NULL) {
        head->v = initNode(c);
        head = head->v;
      }
      else {
        head = lookupOrAdd(head->v, c);
      }
    }
    head->isWord = 1;
  }
  return base;
}

int isWord(struct Node *dict,char *str) {
  int s = strlen(str);
  int i,ii;
  struct Node *b = malloc(sizeof(struct Node));
  b->v = dict;
  dict=  b;
  for (i=0, ii=s; i<ii; i++) {
    dict  = dict->v;
    if ((dict = lookup(dict, str[i])) == NULL) {
      return 0;
    }
  }
  return dict->isWord;
}
