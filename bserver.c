#include <stdio.h>
#include <stdlib.h>
#include "dict.h"
#include "eval.h"

struct Node * getDict() {
  //struct Node*n = initStandardDict();
  FILE *f = fopen("./dictionary.sx","r");
  //FILE *f = fopen("./serialized2.txt","r");
  struct Node *n = deserialize(f);
  //struct Str *s = serialize(n);
  //fprintf(stderr,"%s\n", getStr(s));
  //return isWord(n,"cacao");
  return n;
};

int main(void) {
  FILE *f = fopen("sample.bana","r");
  struct Node* n = deserialize(f);
  doubleLink(n);
  return correct(n,getDict());
}
