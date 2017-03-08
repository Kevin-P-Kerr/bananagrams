#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Node  {
  char c;
  struct Node *h; // horizontal traversal
  struct Node *v; // vertical traversal
};

char * serialize(struct Node *dict) {
  /*
   *
   */
}

int getNext(FILE *f) {
  int c;
  while (isWhite((c=fgetc(f)))) {
    continue;
  }
  return c;
}

int isWhite(int c) {
  if (c == ' ' || c == '\t' || c == '\n') {
    return 1;
  }
  return 0;
}

struct Node *parse(struct Node* n, FILE *f) {
  int c;
  struct Node *base = n;
  // a b (c d e))
  while ((c = getNext(f)) != ')') {
    if (isalpha(c)) {
      n->c = c;
      n->h = malloc(sizeof(struct Node));
      n = n->h;
    }
    if (c == '(') {
      struct Node *v = malloc(sizeof(struct Node));
      n->v = v;
      parse(v,f);
    }
  }
  if (c != ')') {
    return NULL;
  }
  return base;
}


struct Node *deserialize(FILE *f) {
  int c; 
  struct Node *base = malloc(sizeof(struct Node));
  // (a (b (c d) c...) b)
  while ((c = getNext(f)) != EOF) {
    if ((c == '(')) {
      base->v = malloc(sizeof(struct Node));
      parse(base->v,FILE);
    }
  }
  return base->v;
}



struct Node* initNode (char c) {
  struct Node *n = malloc(sizeof(struct Node));
  n->c = c;
  n->h = NULL;
  n->v = NULL;
}

// look up horizontally or fail
struct Node* lookup(struct Node* n, char c) {
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

struct Node* initDict() {
  FILE *f = fopen("/usr/share/dict/american-english","r");
  struct Node *base = initNode('a');
  int c;
  while ((c = fgetc(f)) != EOF) {
    struct Node *head = lookupOrAdd(base,c);
    while ((c = fgetc(f)) != '\n') {
      if (head->v == NULL) {
        head->v = initNode(c);
      }
      else {
        head = lookupOrAdd(head->v, c);
      }
    }
  }
  return base;
}

int isWord(struct Node *dict,char *str) {
  int s = strlen(str);
  int i,ii;
  for (i=0, ii=s; i<ii; i++) {
    if ((dict = lookup(dict, str[i])) == NULL) {
      return 0;
    }
    dict  = dict->v;
  }
  return 1;
}


int main(void) {
  struct Node*n = initDict();
  return isWord(n,"wingspaan");
}
