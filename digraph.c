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

void addChar(struct Str *str, char c) {
  if (!isalpha(c) && (c != ')' && c != '(') && c != '$') {
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
  addChar(str,n->c);
  if (n->isWord) {
    addChar(str,'$');
  }
};

struct Str*recurSerialize(struct Str *str, struct Node *dict) {
  addChar(str,'(');
  addNodeChar(str,dict);
  if (dict->v != NULL) {
    recurSerialize(str,dict->v);
  }
  while (dict->h != NULL) {
    dict = dict->h;
    addNodeChar(str,dict);
    if (dict->v != NULL) {
      recurSerialize(str,dict->v);
    }
  }
  addChar(str,')');
  return str;
}

struct Str * serialize(struct Node *dict) {
  return recurSerialize(initStr(),dict);
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

struct Node *parse(FILE *f) {
  int c;
  struct Node *base = initBlankNode();
  struct Node *n = base;
  // a b (c d e))
  // c (a (t))
  while ((c = getNext(f)) != ')') {
    if (isalpha(c)) {
      n->h = initBlankNode();
      n = n->h;
      n->c = c;
    }
    else if (c == '$') {
      n->isWord = 1;
    }
    else if (c == '(') {
      n->v = parse(f);
    }
  }
  if (c != ')') {
    return NULL;
  }
  return base->h;
}

struct Node *deserialize(FILE *f) {
  int c; 
  //(a b c)
  // a b c)
  fgetc(f);
  return parse(f);
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
