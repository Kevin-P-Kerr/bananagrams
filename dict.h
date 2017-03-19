// macros
#define DEBUG 0
// dictionary structures
struct Node  {
  char c;
  struct Node *h; // horizontal (rightward) traversal
  struct Node *l; // horizontal (leftward) traversal
  struct Node *v; // vertical (downward) traversal
  struct Node *u; // vertical (upward) traversal
  int isWord;
};

struct Str {
  char *c;
  int len;
};
// function prototypes
struct Str *initStr (void);

void addChar(struct Str *str, char c);

struct Str *recurSerialize(struct Str *str, struct Node* dict);

struct Str *serialize(struct Node *dict);

struct Node *parse(FILE *f, char peek);

struct Node *deserialize(FILE *f);

struct Node *lookup(struct Node* n, char c);

struct Node *lookupOrAdd(struct Node *n, char c);

struct Node* initStandardDict(void);

int isWord(struct Node *dict, char *str);
