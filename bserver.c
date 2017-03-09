#include <stdio.h>
#include "dict.h"


int main(void) {
  //struct Node*n = initStandardDict();
  FILE *f = fopen("./dictionary.sx","r");
  //FILE *f = fopen("./serialized.txt","r");
  struct Node *n = deserialize(f);
  //struct Str *s = serialize(n);
  //fprintf(stderr,"%s\n", s->c);
  return isWord(n,"food");
}
