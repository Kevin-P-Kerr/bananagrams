#include <stdio.h>
#include <stdlib.h>
#include "dict.h"


int main(void) {
  //struct Node*n = initStandardDict();
  //FILE *f = fopen("./dictionary.sx","r");
  FILE *f = fopen("./serialized2.txt","r");
  struct Node *n = deserialize(f);
  struct Str *s = serialize(n);
  fprintf(stderr,"%s\n", getStr(s));
  return isWord(n,"coo");
}
