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


struct Node* initBlankNode () { 
  struct Node *n = malloc(sizeof(struct Node));
  n->h = NULL;
  n->v = NULL;
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

struct Node* initDict() {
  FILE *f = fopen("/usr/share/dict/american-english","r");
  struct Node *base =  initBlankNode();
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
  /*
  struct Node*n = initDict();
  return isWord(n,"wingspan");
 */
  FILE *f = fopen("./serialized.txt","r");
  struct Node *n = deserialize(f);
  return isWord(n,"cattt");
}
