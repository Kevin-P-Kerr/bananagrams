struct Node  {
  char c;
  struct Node *h; // horizontal traversal
  struct Node *v; // vertical traversal
};

struct Str {
  char *c;
  int len;
};

struct Str *init (void);

void addChar(struct Str *str, char c);
