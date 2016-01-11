
#include <stdlib.h>
#include "global.h"

#ifdef DEBUG
#define FREED	0x8000
#endif


typedef struct BlockOf_Token {
  struct BlockOf_Token *next;
  Token block[256];
} BlockOf_Token;

static struct {
  BlockOf_Token *blocklist;
  Token *freelist;
#ifdef DEBUG
  int num_blocks;
#endif
} Arena_Token = {0, 0
#ifdef DEBUG
		  , 0
#endif
		     };

static void getblock_Token()
{
  register BlockOf_Token *B =
	(BlockOf_Token *)mallok(sizeof (BlockOf_Token));
  register int i;

  for (i = 0; i < 256 - 1; i++) {
    B->block[i].next = &(B->block[i + 1]);
#ifdef DEBUG
    B->block[i].flags = FREED;
#endif
  }
  B->block[256 - 1].next = Arena_Token.freelist;
#ifdef DEBUG
  B->block[256 - 1].flags = FREED;
#endif
  Arena_Token.freelist = B->block;
  B->next = Arena_Token.blocklist;
  Arena_Token.blocklist = B;
#ifdef DEBUG
  Arena_Token.num_blocks++;
#endif
}

Token *alloc_Token()
{
  register Token *T = Arena_Token.freelist;

  if (!T) {getblock_Token(); T = Arena_Token.freelist;}
  Arena_Token.freelist = T->next; T->next = 0;
#ifdef DEBUG
  T->flags &= ~FREED;
#endif
  return T;
}

void dealloc_Token(T) register Token *T;
{
  T->next = Arena_Token.freelist;
  Arena_Token.freelist = T;
#ifdef DEBUG
  T->flags |= FREED;
#endif
}

void cleanup_Token()
{
  register BlockOf_Token *B1, *B2;

  for (B1 = Arena_Token.blocklist; B1; B1 = B2) {
#ifdef DEBUG
    register int i, n = 0;

    for (i = 0; i < 256; i++) {
      if (!(B1->block[i].flags & FREED))
	n++;
    }
    fprintf(stderr, "%d Token blocks allocated\n", Arena_Token.num_blocks);
    if (n > 0)
      bugchk("%d Token struct(s) leaked", n);
#endif
    B2 = B1->next;
    free(B1);
  }
}


typedef struct BlockOf_Macro {
  struct BlockOf_Macro *next;
  Macro block[256];
} BlockOf_Macro;

static struct {
  BlockOf_Macro *blocklist;
  Macro *freelist;
#ifdef DEBUG
  int num_blocks;
#endif
} Arena_Macro = {0, 0
#ifdef DEBUG
		  , 0
#endif
		     };

static void getblock_Macro()
{
  register BlockOf_Macro *B =
	(BlockOf_Macro *)mallok(sizeof (BlockOf_Macro));
  register int i;

  for (i = 0; i < 256 - 1; i++) {
    B->block[i].next = &(B->block[i + 1]);
#ifdef DEBUG
    B->block[i].flags = FREED;
#endif
  }
  B->block[256 - 1].next = Arena_Macro.freelist;
#ifdef DEBUG
  B->block[256 - 1].flags = FREED;
#endif
  Arena_Macro.freelist = B->block;
  B->next = Arena_Macro.blocklist;
  Arena_Macro.blocklist = B;
#ifdef DEBUG
  Arena_Macro.num_blocks++;
#endif
}

Macro *alloc_Macro()
{
  register Macro *T = Arena_Macro.freelist;

  if (!T) {getblock_Macro(); T = Arena_Macro.freelist;}
  Arena_Macro.freelist = T->next; T->next = 0;
#ifdef DEBUG
  T->flags &= ~FREED;
#endif
  return T;
}

void dealloc_Macro(T) register Macro *T;
{
  T->next = Arena_Macro.freelist;
  Arena_Macro.freelist = T;
#ifdef DEBUG
  T->flags |= FREED;
#endif
}

void cleanup_Macro()
{
  register BlockOf_Macro *B1, *B2;

  for (B1 = Arena_Macro.blocklist; B1; B1 = B2) {
#ifdef DEBUG
    register int i, n = 0;

    for (i = 0; i < 256; i++) {
      if (!(B1->block[i].flags & FREED))
	n++;
    }
    fprintf(stderr, "%d Macro blocks allocated\n", Arena_Macro.num_blocks);
    if (n > 0)
      bugchk("%d Macro struct(s) leaked", n);
#endif
    B2 = B1->next;
    free(B1);
  }
}
 
typedef struct BlockOf_Hash {
  struct BlockOf_Hash *next;
  Hash block[256];
} BlockOf_Hash;

static struct {
  BlockOf_Hash *blocklist;
  Hash *freelist;
#ifdef DEBUG
  int num_blocks;
#endif
} Arena_Hash = {0, 0
#ifdef DEBUG
		  , 0
#endif
		     };

static void getblock_Hash()
{
  register BlockOf_Hash *B =
	(BlockOf_Hash *)mallok(sizeof (BlockOf_Hash));
  register int i;

  for (i = 0; i < 256 - 1; i++) {
    B->block[i].next = &(B->block[i + 1]);
#ifdef DEBUG
    B->block[i].flags = FREED;
#endif
  }
  B->block[256 - 1].next = Arena_Hash.freelist;
#ifdef DEBUG
  B->block[256 - 1].flags = FREED;
#endif
  Arena_Hash.freelist = B->block;
  B->next = Arena_Hash.blocklist;
  Arena_Hash.blocklist = B;
#ifdef DEBUG
  Arena_Hash.num_blocks++;
#endif
}

Hash *alloc_Hash()
{
  register Hash *T = Arena_Hash.freelist;

  if (!T) {getblock_Hash(); T = Arena_Hash.freelist;}
  Arena_Hash.freelist = T->next; T->next = 0;
#ifdef DEBUG
  T->flags &= ~FREED;
#endif
  return T;
}

void dealloc_Hash(T) register Hash *T;
{
  T->next = Arena_Hash.freelist;
  Arena_Hash.freelist = T;
#ifdef DEBUG
  T->flags |= FREED;
#endif
}

void cleanup_Hash()
{
  register BlockOf_Hash *B1, *B2;

  for (B1 = Arena_Hash.blocklist; B1; B1 = B2) {
#ifdef DEBUG
    register int i, n = 0;

    for (i = 0; i < 256; i++) {
      if (!(B1->block[i].flags & FREED))
	n++;
    }
    fprintf(stderr, "%d Hash blocks allocated\n", Arena_Hash.num_blocks);
    if (n > 0)
      bugchk("%d Hash struct(s) leaked", n);
#endif
    B2 = B1->next;
    free(B1);
  }
}
