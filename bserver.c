#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node  {
  char c;
  struct Node *h; // horizontal traversal
  struct Node *v; // vertical traversal
};

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
  return isWord(n,"catzz");
}
