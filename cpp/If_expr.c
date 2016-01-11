
/*---------------------------------------------------------------------*\
|									|
| CPP -- a stand-alone C preprocessor					|
| Copyright (c) 1993-95 Hacker Ltd.		Author: Scott Bigham	|
|									|
| Permission is granted to anyone to use this software for any purpose	|
| on any computer system, and to redistribute it freely, with the	|
| following restrictions:						|
| - No charge may be made other than reasonable charges for repro-	|
|     duction.								|
| - Modified versions must be clearly marked as such.			|
| - The author is not responsible for any harmful consequences of	|
|     using this software, even if they result from defects therein.	|
|									|
| if_expr.c -- evaluate expression in an #if directive			|
\*---------------------------------------------------------------------*/

#include "global.h"
#include "ztype.h"

typedef struct {
  long l;
  char uns;
} Long;

static jmp_buf punt;

static char char_const(s)
  register char *s;
{
  char val = 0;

  for (; *s != '\''; s++)
    switch (*s) {
    case '\\':
      switch (*++s) {
      case 'n':
	val = '\n';
	break;
      case 't':
	val = '\t';
	break;
      case 'v':
	val = '\v';
	break;
      case 'b':
	val = '\b';
	break;
      case 'r':
	val = '\r';
	break;
      case 'f':
	val = '\f';
	break;
      case 'a':
	val = '\a';
	break;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
	val = (*s++) - '0';
	if (is_octal(*s))
	  val = (val << 3) + ((*s++) - '0');
	if (is_octal(*s))
	  val = (val << 3) + ((*s++) - '0');
	break;
      case 'x':
	val = 0;
	for (s++; isxdigit(*s); s++) {
	  val = (val << 4) + (
			       isdigit(*s) ? (*s - '0') :
			       islower(*s) ? (*s - 'a' + 10) :
			       (*s - 'A' + 10)
	      );
	}
	s--;
	break;
      default:
	val = *s;
      }
      break;
    default:
      val = *s;
    }
  return val;
}

static char match(tok)
  int tok;
{
  register TokenP T = exp_token();
  char c = T->subtype;

  if (T->type == tok) {
    free_token(T);
    return c;
  }
  if (tok == UNARY_OP && T->type == ADD_OP) {
    free_token(T);
    return c;
  }
  push_tlist(T);
  return '\0';
}

static void fmatch(tok)
  int tok;
{
  if (!match(tok))
    longjmp(punt, 1);
}

static Long l_or_expr __PROTO((void));

static Long primary_expr()
{
  Long L /* ={0L,'\0'} */ ;
  register TokenP T;

  L.l = 0;
  L.uns = '\0';
  T = exp_token();
  switch (T->type) {
  case ID:
    free_token(T);
    return L;
  case NUMBER:
    L.l = T->val;
    L.uns == !!(T->flags & UNS_VAL);
    free_token(T);
    return L;
  case CHAR_CON:
    L.l = char_const(token_txt(T) + 1);
    free_token(T);
    return L;
  case LPAREN:
    L = l_or_expr();
    fmatch(RPAREN);
    free_token(T);
    return L;
  default:
    free_token(T);
    longjmp(punt, 1);
  }
}

static Long unary_expr()
{
  Long L;
  char op;

  if (!(op = match(UNARY_OP)))
    return primary_expr();
  switch (op) {
  case '!':
    L = unary_expr();
    L.uns = 0;
    L.l = !L.l;
    return L;
  case '~':
    L = unary_expr();
    L.l = ~L.l;
    return L;
  case '+':
    return unary_expr();
  case '-':
    L = unary_expr();
    L.l = -L.l;
    return L;
  default:
    bugchk("'%c' is UNARY_OP in #if expr", op);
    longjmp(punt, 2);
  }
}

static Long mul_expr()
{
  Long L1, L;
  char op;

  L1 = unary_expr();
  while ((op = match(MUL_OP)) != 0) {
    L = unary_expr();
    if (L.uns)
      L1.uns = 1;
    switch (op) {
    case '*':
      L1.l *= L.l;
      break;
    case '%':
      L1.l %= L.l;
      break;
    case '/':
      if (L.l == 0) {
	error("divide by zero in #if expr");
	longjmp(punt, 2);
      }
      L1.l /= L.l;
      break;
    default:
      bugchk("'%c' is MUL_OP in #if expr", op);
      longjmp(punt, 2);
    }
  }
  return L1;
}

static Long add_expr()
{
  Long L1, L;
  char op;

  L1 = mul_expr();
  while ((op = match(ADD_OP)) != 0) {
    L = mul_expr();
    if (L.uns)
      L1.uns = 1;
    switch (op) {
    case '+':
      L1.l += L.l;
      break;
    case '-':
      L1.l -= L.l;
      break;
    default:
      bugchk("'%c' is ADD_OP in #if expr", op);
      longjmp(punt, 2);
    }
  }
  return L1;
}

static Long shift_expr()
{
  Long L1, L;
  char op;

  L1 = add_expr();
  while ((op = match(SHIFT_OP)) != 0) {
    L = add_expr();
    if (L.uns)
      L1.uns = 1;
    switch (op) {
    case '<':
      L1.l <<= L.l;
      break;
    case '>':
      L1.l >>= L.l;
      break;
    default:
      bugchk("'%c' is SHIFT_OP in #if expr", op);
      longjmp(punt, 2);
    }
  }
  return L1;
}

#define compare(L1,op,L2) \
 ((L1.uns || L2.uns) ? (unsigned long)L1.l op (unsigned long)L2.l : \
		     L1.l op L2.l)

static Long rel_expr()
{
  Long L1, L;
  char op;

  L1 = shift_expr();
  while ((op = match(REL_OP)) != 0) {
    L = shift_expr();
    switch (op) {
    case '<':
      L1.l = compare(L1, <, L);
      L1.uns = 0;
      break;
    case '>':
      L1.l = compare(L1, >, L);
      L1.uns = 0;
      break;
    case '(':
      L1.l = compare(L1, <=, L);
      L1.uns = 0;
      break;
    case ')':
      L1.l = compare(L1, >=, L);
      L1.uns = 0;
      break;
    default:
      bugchk("'%c' is REL_OP in #if expr", op);
      longjmp(punt, 2);
    }
  }
  return L1;
}

static Long eq_expr()
{
  Long L1, L;
  char op;

  L1 = rel_expr();
  while ((op = match(EQ_OP)) != 0) {
    L = rel_expr();
    switch (op) {
    case '=':
      L1.l = compare(L1, ==, L);
      L1.uns = 0;
      break;
    case '!':
      L1.l = compare(L1, !=, L);
      L1.uns = 0;
      break;
    default:
      bugchk("'%c' is EQ_OP in #if expr", op);
      longjmp(punt, 2);
    }
  }
  return L1;
}

static Long b_and_expr()
{
  Long L1, L;
  char op;

  L1 = eq_expr();
  while ((op = match(B_AND_OP)) != 0) {
    L = eq_expr();
    if (L.uns)
      L1.uns = 1;
    switch (op) {
    case '&':
      L1.l &= L.l;
      break;
    default:
      bugchk("'%c' is B_AND_OP in #if expr", op);
      longjmp(punt, 2);
    }
  }
  return L1;
}

static Long b_xor_expr()
{
  Long L1, L;
  char op;

  L1 = b_and_expr();
  while ((op = match(B_XOR_OP)) != 0) {
    L = b_and_expr();
    if (L.uns)
      L1.uns = 1;
    switch (op) {
    case '^':
      L1.l ^= L.l;
      break;
    default:
      bugchk("'%c' is B_XOR_OP in #if expr", op);
      longjmp(punt, 2);
    }
  }
  return L1;
}

static Long b_or_expr()
{
  Long L1, L;
  char op;

  L1 = b_xor_expr();
  while ((op = match(B_OR_OP)) != 0) {
    L = b_xor_expr();
    if (L.uns)
      L1.uns = 1;
    switch (op) {
    case '|':
      L1.l |= L.l;
      break;
    default:
      bugchk("'%c' is B_OR_OP in #if expr", op);
      longjmp(punt, 2);
    }
  }
  return L1;
}

static Long l_and_expr()
{
  Long L1, L;
  char op;

  L1 = b_or_expr();
  while ((op = match(L_AND_OP)) != 0) {
    L = b_or_expr();
    switch (op) {
    case '&':
      L1.l = L1.l && L.l;
      L1.uns = 0;
      break;
    default:
      bugchk("'%c' is L_AND_OP in #if expr", op);
      longjmp(punt, 2);
    }
  }
  return L1;
}

static Long l_or_expr()
{
  Long L1, L;
  char op;

  L1 = l_and_expr();
  while ((op = match(L_OR_OP)) != 0) {
    L = l_and_expr();
    switch (op) {
    case '|':
      L1.l = L1.l || L.l;
      L1.uns = 0;
      break;
    default:
      bugchk("'%c' is L_OR_OP in #if expr", op);
      longjmp(punt, 2);
    }
  }
  return L1;
}

static Long expr()
{
  Long L;

  L = l_or_expr();
  if (!match(EOL))
    longjmp(punt, 1);
  return L;
}

int if_expr()
{
  Long L;
  int r;

  change_mode(IF_EXPR, 0);
  _tokenize_line();
  switch (setjmp(punt)) {
  case 0:
    L = expr();
    r = !!L.l;
    break;
  case 1:
    error("syntax error in #if expr");
    /* and fall through */
  default:
    r = 0;
  }
  change_mode(0, IF_EXPR);
  return r;
}
