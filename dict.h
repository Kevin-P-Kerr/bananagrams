// dictionary structures
struct Node  {
  char c;
  struct Node *h; // horizontal traversal
  struct Node *v; // vertical traversal
  int isWord;
};

struct Str {
  char *c;
  int len;
};
// function prototypes
struct Str *init (void);

void addChar(struct Str *str, char c);

struct Str *recurSerialize(struct Str *str, struct Node* dict);

struct Str *serialize(struct Node *dict);

struct Node *parse(FILE *f);

struct Node *deserialize(FILE *f);

struct Node *lookup(struct Node* n, char c);

struct Node *lookupOrAdd(struct Node *n, char c);

struct Node* initStandardDict(void);

int isWord(struct Node *dict, char *str);
