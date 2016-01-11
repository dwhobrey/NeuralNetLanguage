
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
| token.c -- transform raw input to preprocessor tokens			|
\*---------------------------------------------------------------------*/

/*
   There are (for better or worse) three interfaces to the tokenizing
   mechanism, at increasing levels of abstraction.  _one_token() tokenizes
   directly out of the input line buffer, and should generally only be used
   while handling preprocessor directive lines.  token() pre-tokenizes an
   entire line of input at a time and doles it out one token at a time.  Note
   that tokens returned by token() may or may not have undergone macro
   expansion; use it when you need access to pre-expanded input tokens.
   exp_token() fully expands each token before returning it; this is usually
   the best way to access input.
*/

#include "global.h"
#include "ztype.h"
#include "alloc.h"

#define BASE10	1
#define BASE8	2
#define BASE16	3

#define GRANULARITY 256

extern char *next_c;

static int tok_flags = 0;
static TokenP pushback_list;
static int wrapped_string = 0;

/* mk_Token() -- allocate and initialize space for a Token */
TokenP mk_Token()
{
  register TokenP T = alloc_Token();

  T->val = T->hashval = T->type = T->subtype = 0;
  T->_txt.out_of_line = T->_ws.out_of_line = NULL;
  T->flags = (INLINE_TXT | INLINE_WS);
  T->_txt.in_line[0] = T->_ws.in_line[0] = '\0';
#if 0			/* already done in alloc_Token() */
  T->next = NULL;
#endif
  return T;
}

/* clear_txt() -- clear the text space of a token */
void clear_txt(T)
  register TokenP T;
{
  if (!(T->flags & INLINE_TXT) && T->_txt.out_of_line)
    free(T->_txt.out_of_line);
  T->flags |= INLINE_TXT;
  T->_txt.in_line[0] = '\0';
}

/* clear_ws() -- clear the white space of a token */
void clear_ws(T)
  register TokenP T;
{
  if (!(T->flags & INLINE_WS) && T->_ws.out_of_line)
    free(T->_ws.out_of_line);
  T->flags |= INLINE_WS;
  T->_ws.in_line[0] = '\0';
}

/* set_txt() -- set the text of a token to |s|, copying if necessary,
   and deleting the current text, if any */
void set_txt(T, s)
  register TokenP T;
  register const char *s;
{
  if (!(T->flags & INLINE_TXT) && T->_txt.out_of_line)
    free(T->_txt.out_of_line);
  if (strlen(s) <= 7) {
    T->flags |= INLINE_TXT;
    strcpy(T->_txt.in_line, s);
  } else {
    T->flags &= ~INLINE_TXT;
    T->_txt.out_of_line = strdup(s);
  }
}

/* set_txt_n() -- set the text of a token to the first |n| characters
   of |s|, copying if necessary, and deleting the current text, if any */
void set_txt_n(T, s, n)
  register TokenP T;
  register const char *s;
  int n;
{
  if (!(T->flags & INLINE_TXT) && T->_txt.out_of_line)
    free(T->_txt.out_of_line);
  if (n <= 7) {
    T->flags |= INLINE_TXT;
    strncpy(T->_txt.in_line, s, n);
    T->_txt.in_line[n] = '\0';
  } else {
    T->flags &= ~INLINE_TXT;
    T->_txt.out_of_line = mallok(n + 1);
    strncpy(T->_txt.out_of_line, s, n);
    T->_txt.out_of_line[n] = '\0';
  }
}

/* set_ws() -- set the white space of a token to |s|, copying if
   necessary, and deleting the current white space, if any */
void set_ws(T, s)
  register TokenP T;
  register const char *s;
  { if(!T) return; /*DJRW 17/12/96: if macro called with less args than parms, this will be null */
  if (!(T->flags & INLINE_WS) && T->_ws.out_of_line)
    free(T->_ws.out_of_line);
  if (strlen(s) <= 3) {
    T->flags |= INLINE_WS;
    strcpy(T->_ws.in_line, s);
  } else {
    T->flags &= ~INLINE_WS;
    T->_ws.out_of_line = strdup(s);
  }
}

/* free_token() -- return an allocated Token to the free list */
void free_token(T)
  register TokenP T;
{
  T->next = NULL;
  free_tlist(T);
}

/* free_tlist() -- return a list of Token's to the free list */
void free_tlist(T)
  register TokenP T;
{
  register TokenP T1;

  for (; T; T = T1) {
    T1 = T->next;
    clear_txt(T);
    clear_ws(T);
    dealloc_Token(T);
  }
}

/*
   copy_token() -- return a new Token that is a duplicate of the given token
*/
TokenP copy_token(T1)
  register TokenP T1;
{
  register TokenP T2 = mk_Token();

  *T2 = *T1;
  if (!(T1->flags & INLINE_WS))
    T2->_ws.out_of_line = strdup(T1->_ws.out_of_line);
  if (!(T1->flags & INLINE_TXT))
    T2->_txt.out_of_line = strdup(T1->_txt.out_of_line);
  T2->next = NULL;
  return T2;
}

/* copy_tlist() -- create a duplicate of a list of Token's */
TokenP copy_tlist(T1)
  register TokenP T1;
{
  Token head;
  register TokenP T2 = &head;

  for (T2->next = NULL; T1; T1 = T1->next, T2 = T2->next)
    T2->next = copy_token(T1);
  return head.next;
}

/* tok_shutdown() -- free all space allocated for Token's */
void tok_shutdown()
{
#ifdef DEBUG		/* explicitly clean up, to check for memory leaks */
#if 0
  register TokenP T, T1;
  register int i;

  for (T1 = T = tok_blocks; T; T = T1) {
    T1 = T->next;
    for (i = 1; i < GRANULARITY; i++) {
      if (T[i].flags & IN_USE) {
	fprintf(stderr, "@@@ Token not freed:  ");
	dump_token(&T[i]);
	fputc('\n', stderr);
      }
      clear_txt(&T[i]);
      clear_ws(&T[i]);
    }
    free(T);
  }
  fprintf(stderr, "%d total blocks allocated\n", num_blocks);
#else /* 0 */
  cleanup_Token();
#endif /* 0 */
#endif
}

/*
   push_tlist() -- "un-read" the list of Token's |T|; token() will return all
   of these tokens in order before reading another token from the input file
*/
void push_tlist(T)
  register TokenP T;
{
  register TokenP t;

  if (!T)
    return;
  t = T;
  while (t->next)
    t = t->next;
  t->next = pushback_list;
  pushback_list = T;
}

/* mk_eof() -- makes and returns an EOF_ token */
static TokenP mk_eof()
{
  register TokenP T = mk_Token();

  T->type = EOF_;
  T->flags |= INLINE_TXT | INLINE_WS;
  T->_ws.in_line[0] = T->_txt.in_line[0] = '\0';
  return T;
}

/*
   mk_stopper() -- makes and returns a STOP token.  See expand_tlist() for
   further information.
*/
TokenP mk_stopper()
{
  register TokenP T = mk_Token();

  T->type = STOP;
  T->flags |= INLINE_TXT | INLINE_WS;
  T->_ws.in_line[0] = T->_txt.in_line[0] = '\0';
  return T;
}

/*
   mk_unmarker() -- makes and returns a special token that informs the
   tokenizer to unmark the macro text associated with token |T|.  See
   expand() for further information.
*/
TokenP mk_unmarker(T)
  register TokenP T;
{
  register TokenP T1 = copy_token(T);

  T1->type = UNMARK;
  T->flags |= INLINE_TXT | INLINE_WS;
  T->_ws.in_line[0] = T->_txt.in_line[0] = '\0';
  return T1;
}

/*
   mk_printable() -- makes and returns an untyped token with an
   arbitrary text body, for purposes of printing directly via
   print_token().
*/

TokenP mk_printable(s)
  const char *s;
{
  register TokenP T = mk_Token();

  T->type = DONT_CARE;
  set_ws(T, " ");
  set_txt(T, s);
  return T;
}

/* flush_tokenizer() -- discard all Tokens pushed back by push_tlist() */
void flush_tokenizer()
{
  free_tlist(pushback_list);
  pushback_list = NULL;
}

/*
   pp_number_check() -- check that the token between |s| and |s_end|
   conforms strictly to the grammar for preprocessing numbers listed in
   ANSI/ISO 6.1.8
*/
static void pp_number_check(s, s_end)
  register char *s;
  char *s_end;
{
  char *s_begin = s;

  if (*s == '.')
    s++;
  for (s++; ; s++) {
    if ((*s == 'e' || *s == 'E') && (s[1] == '-' || s[1] == '+')) {
      s++;
      continue;
    }
    if (is_ctok(*s))
      continue;
    if (*s == '.')
      continue;
    break;
  }
  if (s != s_end) {
    warning("(pedantic) Preprocessor token `%.*s' is not a syntactic token",
	    (int)(s - s_begin), s_begin);
    message("Insert whitespace after `%.*s'", (int)(s_end - s_begin),
	    s_begin);
  }
}

/*
   number() -- copies from |s| into the token |T| a string of characters
   denoting an integer or floating-point constant.  Returns a pointer to the
   first uncopied character.
*/
static char *number(s, T)
  register char *s;
  register TokenP T;
{
  int numtype = BASE10, fpflag = 0;
  char *t, *s_begin = s;

  T->type = NUMBER;
  if (*s == '0') {
    /* check for octal or hexadecimal constant */
    if ((s[1] == 'x' || s[1] == 'X') && isxdigit(s[2])) {
      numtype = BASE16;
      T->flags |= UNS_VAL;
    } else if (is_octal(s[1])) {
      numtype = BASE8;
      T->flags |= UNS_VAL;
    }
  }
  T->val = strtol(s, &t, 0);
  s = t;
  if (numtype != BASE10 || is_isuff(*s)) {

    /*
       if we're not in base 10, or the next characters are integer constant
       suffixes, this can't be a floating-point constant
    */
    while (is_isuff(*s)) {
      if (*s == 'u' || *s == 'U')
	T->flags |= UNS_VAL;
      s++;
    }
    if (pedantic)
      pp_number_check(s_begin, s);
    return s;
  }
  /* check to see if the number is actually floating point */
  if (*s == '.') {
    fpflag = 1;
    do
      s++;
    while (isdigit(*s));
  }
  if (*s == 'e' || *s == 'E') {
    register char *u = s;

    u++;
    if (*u == '-' || *u == '+')
      u++;
    if (isdigit(*u)) {
      fpflag = 1;
      do
	u++;
      while (isdigit(*u));
      s = u;
    }
  }
  if (fpflag) {
    T->type = FP_NUM;
    if (is_fsuff(*s))
      s++;
  }
  if (pedantic)
    pp_number_check(s_begin, s);
  return s;
}

/*
   char_constant() -- copy from |s| into the token |T| a string of characters
   denoting a character constant.  We do not translate escape sequences at
   this point, though we might need to
*/
static char *char_constant(s, T)
  register char *s;
  register TokenP T;
{
  T->type = CHAR_CON;
  for (; *s; s++) {
    if (*s == '\'')
      return s + 1;
    if (*s == '\\')
      s++;
  }
  error("unterminated character constant");
  return s;
}

/*
   string_literal() -- copy from |s| into the token |T| a string of
   characters denoting a string literal.  We do not translate escape
   sequences at this point, though we might need to
*/
static char *string_literal(s, T)
  register char *s;
  register TokenP T;
{
  T->type = STR_CON;
  for (; *s; s++) {
    if (*s == '"') {
      wrapped_string = 0;
      return s + 1;
    }
    if (*s == '\\')
      s++;
  }
  if (gcc_strings)
    wrapped_string = 1;
  else
    error("unterminated string literal");
  return s;
}

/*
   include_name() -- copy from |s| into the token |T| a string of characters
   denoting an #include file specifier enclosed in <>. |s| points to the
   character after the '>'.
*/
static char *include_name(s, T)
  register char *s;
  register TokenP T;
{
  T->type = INC_NAM;
  for (; *s; s++) {
    if (*s == '>')
      return s + 1;
  }
  error("unterminated include file name");
  return s;
}

/* set_mode() -- set the tokenizer flags to |m| */
void set_mode(m)
  int m;
{
  tok_flags = m;
}

/*
   change_mode() -- twiddle the tokenizer flags; in particular, set the flags
   specified in |raise| and clear the flags specified in |lower|
*/
void change_mode(raise, lower)
  int raise, lower;
{
  tok_flags |= raise;
  tok_flags &= (~lower);
}

/* get_mode() -- return the current value of the tokenizer flags */
int get_mode()
{
  return tok_flags;
}

/*
   xlate_token() -- determines the type of the next preprocessor token in the
   string pointed to by |s|.  Information about the token found is placed in
   the Token |T|.  Returns a pointer to the first character not in the token
   read.
*/
static char *xlate_token(s, T)
  register char *s;
  register TokenP T;
{
  if (wrapped_string)
    return string_literal(s, T);

  if (is_ctoks(*s)) {
    char *t;

    T->hashval = hash_id(s, &t);
    T->type = ID;
    return t;
  }

  if (isdigit(*s))
    return number(s, T);

  switch (*s++) {
    case '.':
      T->subtype = '.';
      if (*s == '.' && s[1] == '.') {
	s += 2;
	T->type = DONT_CARE;
      } else if (isdigit(*s))
	s = number(s - 1, T);
      else
	T->type = DONT_CARE;
      break;
    case '#':
      if (*s == '#') {
	s++;
	T->type = TOK_CAT;
      } else
	T->type = POUND;
      break;
    case '&':
      T->subtype = '&';
      if (*s == '&') {
	s++;
	T->type = L_AND_OP;
      } else if (*s == '=') {
	s++;
	T->type = DONT_CARE;
      } else
	T->type = B_AND_OP;
      break;
    case '|':
      T->subtype = '|';
      if (*s == '|') {
	s++;
	T->type = L_OR_OP;
      } else if (*s == '=') {
	s++;
	T->type = DONT_CARE;
      } else
	T->type = B_OR_OP;
      break;
    case '+':
      T->subtype = '+';
      if (*s == s[-1] || *s == '=') {
	s++;
	T->type = DONT_CARE;
      } else
	T->type = ADD_OP;
      break;
    case '~':
      T->type = UNARY_OP;
      T->subtype = '~';
      break;
    case ',':
      T->type = COMMA;
      T->subtype = ',';
      break;
    case '(':
      T->type = LPAREN;
      T->subtype = '(';
      break;
    case ')':
      T->type = RPAREN;
      T->subtype = ')';
      break;
    case '!':
      T->subtype = '!';
      if (*s == '=') {
	s++;
	T->type = EQ_OP;
      } else
	T->type = UNARY_OP;
      break;
    case '=':
      T->subtype = '=';
      if (*s == '=') {
	s++;
	T->type = EQ_OP;
      } else
	T->type = DONT_CARE;
      break;
    case '*':
    case '/':
    case '%':
      T->subtype = s[-1];
      if (*s == '=') {
	s++;
	T->type = DONT_CARE;
      } else
	T->type = MUL_OP;
      break;
    case '^':
      T->subtype = '^';
      if (*s == '=') {
	s++;
	T->type = DONT_CARE;
      } else
	T->type = B_XOR_OP;
      break;
    case '-':
      T->subtype = '-';
      if (*s == '-' || *s == '=' || *s == '>') {
	s++;
	T->type = DONT_CARE;
      } else
	T->type = ADD_OP;
      break;
    case '<':
      if (tok_flags & INCLUDE_LINE) {
	s = include_name(s, T);
	break;
      }
      /* else fall through */
    case '>':
      T->subtype = s[-1];
      T->type = REL_OP;
      if (*s == s[-1]) {
	s++;
	T->type = SHIFT_OP;
      }
      if (*s == '=') {
	s++;
	if (T->type == REL_OP)
	  T->subtype = (T->subtype == '<' ? '(' : ')');
	else
	  T->type = DONT_CARE;
      }
      break;
    case '\'':
      s = char_constant(s, T);
      break;
    case '"':
      s = string_literal(s, T);
      break;
    case '[':
    case ']':
    case '{':
    case '}':
    case ';':
    case ':':
    case '?':
      T->type = DONT_CARE;
      break;
    default:
      T->type = UNKNOWN;
  }
  return s;
}

/* print_token() -- write token |T| to the output file */
void print_token(T)
  register TokenP T;
{
  if (in_config_file)
    return;
  if (T->type == STOP)
    bugchk("STOP token in output stream?");
  fputs(token_ws(T), outf);
  fputs(token_txt(T), outf);
  if (T->flags & TRAIL_SPC)
    fputc(' ', outf);
}

/*
   merge_tokens() -- Perform token pasting on Token's |T1| and |T2|. Returns
   the resulting token.
*/
TokenP merge_tokens(T1, T2)
  register TokenP T1, T2;
{
  register TokenP T = mk_Token();
  register char *t;

  set_ws(T, token_ws(T1));
  t = mallok(strlen(token_txt(T1)) + strlen(token_txt(T2)) + 1);
  strcpy(t, token_txt(T1));
  strcat(t, token_txt(T2));
  set_txt(T, t);
  free(t);
  t = xlate_token(token_txt(T), T);
  if (*t != '\0') {
    if (w_bad_concat)
      warning("Invalid token \"%s\" created by concatenation",
	      token_txt(T));
    T->type = UNKNOWN;
  }
  return T;
}

TokenP _one_token()
{
  register TokenP T = mk_Token();
  register char *s = next_c, *t, *u;
  int n;

  t = suck_ws(s, T);
  if (!t || !*t) {
    set_txt(T, "\n");
    T->type = EOL;
    T->subtype = '\n';
    next_c = t;
    return T;
  }
  u = xlate_token(t, T);
  n = (int)(u - t);
  if (T->type == UNKNOWN && w_bad_chars)
    error("Unrecognized character 0x%02x='%c'", *t, *t);
  set_txt_n(T, t, n);
  next_c = u;
  return T;
}

void _tokenize_line()
{
  Token head;
  register TokenP T = &head;

  head.next = NULL;
  do {
    T = T->next = _one_token();
  } while (T->type != EOL);
  push_tlist(head.next);
}

TokenP token()
{
  register TokenP T;
  register char *s;

  while (pushback_list) {
    T = pushback_list;
    pushback_list = T->next;
    T->next = NULL;
    if (T->type == UNMARK) {
      Macro *M;

      M = lookup(token_txt(T), T->hashval);
      if (!M)
	bugchk("UNMARK on non-macro token %s", token_txt(T));
      if (!(M->flags & MARKED))
	bugchk("UNMARK on unmarked macro %s", token_txt(T));
      M->flags ^= MARKED;
      free_token(T);
      continue;
    } else {
      return T;
    }
  }

  /*
     if we get to here, the pushback list is empty, and we need to read in
     another line
  */
  next_c = s = getline();
  if (!s)
    return mk_eof();
  T = _one_token();
  if (T->type == EOL) {
    return T;
  }
  /*
     If the line is a preprocessor directive, don't pre-tokenize the
     rest of the line, as we need it in "raw" form.  If the line is not
     a preprocessor directive and we're in the false branch of an #if
     conditional, don't bother pre-tokenizing the line, because it will
     just get thrown away (Note that we can't do this in the presence of
     -fimplicit-newlines, because we need to know if the last token on
     the line is an unterminated string).  If we're in the middle of
     reading an argument of a parametrized macro, preprocessor
     directives get trodden on anyway, so go ahead and pre-tokenize.
  */
  if ((T->type != POUND && (cond_true() || gcc_strings)) ||
      (get_mode() & SLURP))
    _tokenize_line();
  return T;
}

TokenP exp_token()
{
  register TokenP T = token();
  register Macro *M;

  if (T->type == ID && !(T->flags & BLUEPAINT) &&
      (M = lookup(token_txt(T), T->hashval))) {
    expand(T, M);
    free_token(T);
    return exp_token();
  } else
    return T;
}

#ifdef DEBUG

/*
   debugging routines to display tokens in internal format
*/

void dump_token(T)
  TokenP T;
{
  static char *type_names[] =
  {"<<< ERROR >>>", "UNKNOWN", "DONT_CARE",
   "EOL", "NUMBER", "FP_NUM", "ID", "STR_CON", "CHAR_CON", "UNARY_OP",
   "MUL_OP", "ADD_OP", "SHIFT_OP", "REL_OP", "EQ_OP", "B_AND_OP",
   "B_XOR_OP", "B_OR_OP", "L_AND_OP", "L_OR_OP", "LPAREN", "RPAREN",
   "COMMA", "INC_NAM", "POUND", "TOK_CAT", "MACRO_ARG", "EOF_", "STOP",
   "UNMARK"
  };

  fprintf(stderr, "[%s, '%c', \"%s\", \"%s\", %ld, %x]@%p",
	  type_names[T->type], (T->subtype ? T->subtype : ' '),
	  (token_ws(T) ? token_ws(T) : "(null)"),
	  (token_txt(T) ? token_txt(T) : "(null)"),
	  T->val, T->flags, (void *)T
      );
}

void dump_tlist(T)
  TokenP T;
{
  while (T) {
    dump_token(T);
    fputc('\n', stderr);
    T = T->next;
  }
}

void dump_pushback()
{
  dump_tlist(pushback_list);
}

#endif /* DEBUG */
