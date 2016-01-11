
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
| pound.c -- handle preprocessor directives				|
\*---------------------------------------------------------------------*/

#include "global.h"

#define IF_STACK_SIZE 10
#define N_DIRS nelems(d_table)
#define IF_STATE (if_sp[-1])

#define COND_FALSE	0
#define COND_ELSE_SEEN	4
#define COND_DONE_TRUE	8

/* The hash values for the directives depend on the prevailing int size.
   Make sure that __MSHORT__ is #define'd if sizeof(int) == 2. */
#ifdef __MSHORT__
#define	POUND_DEFINE	782
#define	POUND_UNDEF	200
#define	POUND_INCLUDE	713
#define	POUND_IF	645
#define	POUND_IFDEF	362
#define	POUND_IFNDEF	1032
#define	POUND_ELSE	559
#define	POUND_ELIF	230
#define	POUND_ENDIF	750
#define	POUND_LINE	675
#define	POUND_ERROR	679
#define	POUND_PRAGMA	1039
#define	POUND_EMPTY	0
#else
#define	POUND_DEFINE	252
#define	POUND_UNDEF	383
#define	POUND_INCLUDE	386
#define	POUND_IF	645
#define	POUND_IFDEF	324
#define	POUND_IFNDEF	424
#define	POUND_ELSE	793
#define	POUND_ELIF	464
#define	POUND_ENDIF	1017
#define	POUND_LINE	713
#define	POUND_ERROR	799
#define	POUND_PRAGMA	864
#define	POUND_EMPTY	0
#endif


static int *if_stack, if_stack_size;
int *if_sp;

extern void do_include __PROTO((void));
extern void do_define __PROTO((void));
extern void do_undefine __PROTO((void));

/* set up the conditional-compile stack */
void cond_setup()
{
  if_sp = if_stack = (int *)mallok((if_stack_size = IF_STACK_SIZE) * sizeof (int));

  *if_sp++ = COND_TRUE;
}

/*
   endif_check() -- see if there are states left on the stack; each
   corresponds to a missing #endif
*/
void endif_check()
{
  int i;

  i = (int)(if_sp - if_stack) - 1;
  if (i) {
    while (i--)
      error("missing #endif");
  }
  if_sp = if_stack;
  *if_sp++ = COND_TRUE;
}

/* cond_shutdown() -- free the conditional-compile stack() */
void cond_shutdown()
{
  free(if_stack);
}

/* cond_push() -- push a conditional-compile state onto the stack */
static void cond_push(s)
  int s;
{
  if (if_sp - if_stack >= if_stack_size) {
    ptrdiff_t dp;

    dp = if_sp - if_stack;
    if_stack = reallok(if_stack, (if_stack_size *= 2) * sizeof (int));

    if_sp = if_stack + dp;
  }
  *if_sp++ = s;
}

/* cond_pop() -- remove a conditional-compile state from the stack */
static void cond_pop()
{
  if (if_sp - if_stack <= 1)
    error("unmatched #endif");
  else
    if_sp--;
}

/* do_if() -- handle an #if directive */
static void do_if()
{
  cond_push(!cond_true()? COND_NESTED :
	    if_expr()? COND_TRUE | COND_DONE_TRUE :
	    COND_FALSE
  );
}

/* do_ifdef() -- handle an #ifdef directive */
static void do_ifdef()
{
  TokenP T;

  T = _one_token();
  if (T->type != ID)
    error("argument \"%s\" to #ifdef is not an identifier", token_txt(T));
  else {
    cond_push(!cond_true()? COND_NESTED :
	      lookup(token_txt(T), T->hashval) ? COND_TRUE | COND_DONE_TRUE :
	      COND_FALSE
	);
  }
  free_token(T);
  T = _one_token();
  if (T->type != EOL)
    warning("garbage after #ifdef");
  free_token(T);
}

/* do_ifndef() -- handle an #ifndef directive */
static void do_ifndef()
{
  TokenP T;

  T = _one_token();
  if (T->type != ID)
    error("argument \"%s\" to #ifndef is not an identifier", token_txt(T));
  else {
    cond_push(!cond_true()? COND_NESTED :
	      lookup(token_txt(T), T->hashval) ? COND_FALSE :
	      COND_TRUE | COND_DONE_TRUE
	);
  }
  free_token(T);
  T = _one_token();
  if (T->type != EOL)
    warning("garbage after #ifndef");
  free_token(T);
}

/* do_else() -- handle an #else directive */
static void do_else()
{
  TokenP T;

  if (IF_STATE & COND_ELSE_SEEN)
    error("#else after #else");
  if (IF_STATE & COND_DONE_TRUE)
    IF_STATE &= (~COND_TRUE);
  else
    IF_STATE |= (COND_TRUE | COND_DONE_TRUE);
  T = _one_token();
  if (T->type != EOL)
    warning("garbage after #else");
  free_token(T);
}

/* do_elif() -- handle an #elif directive */
static void do_elif()
{
  if (IF_STATE & COND_ELSE_SEEN)
    error("#elif after #else");
  if (IF_STATE & COND_DONE_TRUE)
    IF_STATE &= (~COND_TRUE);
  else if (if_expr())
    IF_STATE |= (COND_TRUE | COND_DONE_TRUE);
  else
    IF_STATE &= (~COND_TRUE);
}

/* do_endif() -- handle an #endif directive */
static void do_endif()
{
  TokenP T;

  cond_pop();
  T = _one_token();
  if (T->type != EOL)
    warning("garbage after #endif");
  free_token(T);
}

/* do_line() -- handle a #line directive */
static void do_line()
{
  TokenP Tn, Tf;
  int l;

  _tokenize_line();
  Tn = exp_token();
  if (Tn->type != NUMBER) {
    error("malformed number \"%s\" in #line directive", token_txt(Tn));
    free_token(Tn);
    return;
  }
  Tf = exp_token();
  if (Tf->type != STR_CON && Tf->type != EOL) {
    error("malformed filename \"%s\" in #line directive", token_txt(Tf));
    free_token(Tn);
    free_token(Tf);
    return;
  }
  if (Tf->type == STR_CON) {
    l = strlen(token_txt(Tf)) - 2;
    free(cur_file);
    cur_file = mallok(l + 1);
    strncpy(cur_file, token_txt(Tf) + 1, l);
    cur_file[l] = '\0';
    free_token(Tf);
    Tf = exp_token();
    if (Tf->type != EOL)
      error("garbage after #line");
    free_token(Tf);
  }
  this_line = next_line = Tn->val;
  sync_line(0);
  free_token(Tn);
}

/* do_error() -- handle an #error directive */
static void do_error()
{
  error("%s", rest_of_line());
}

/* write_pragma_text() -- write a token contaning the text |s| directly
   to the output file
*/
static void write_pragma_text(s)
  const char *s;
{
  register TokenP T = mk_printable(s);

  print_token(T);
  free_token(T);
}

/* do_pragma() -- handle a #pragma directive */
static void do_pragma()
{
  TokenP T = _one_token();
  int recognized = 0;

  if (T->type == EOL) {
    warning("empty #pragma directive");
    free_token(T);
    return;
  }
  if (in_config_file) {
    /* pragmas that are enabled only in config file */
    if (streq(token_txt(T), "CPP_cmdline_arg")) {
      char *s = rest_of_line();

      recognized = 1;
      while (isspace(*s))
	s++;
      if (*s == '-')
	do_cmdline_arg(s);
      else
	error("invalid cmdline arg \"%s\" in #pragma CPP_cmdline_arg", s);
    } else if (streq(token_txt(T), "CPP_delayed")) {
      recognized = 1;
      if (Argc_end != 0)
	error("#pragma CPP_delayed cannot be repeated");
      else
	do_all_cmdline_args();
    }
  }
  if (fluff_mode && streq(token_txt(T), "fluff")) {
    /* pragmas for use with fluff */
    TokenP T0 = _one_token();

    recognized = 1;
    if (T0->type == EOL) {
      error("empty `#pragma fluff' directive");
    } else if (streq(token_txt(T0), "varargs")) {
      write_pragma_text("__FLUFF_varargs");
      write_pragma_text("\n");
      last_line++;
    } else {
      error("invalid `#pragma fluff directive \"%s\"", token_txt(T0));
    }
    free_token(T0);
  }
  if (!recognized && w_pragma) {
    warning("unrecognized #pragma directive:  '%s'", token_txt(T));
  }
  free_token(T);
  return;
}

/*
   directive() -- perform the directive on the current input line
*/
void directive()
{
  TokenP T;

  T = _one_token();
  if (T->type == EOL) {
    free_token(T);
    return;
  }
  switch (T->hashval + token_txt(T)[0]) {
  case POUND_DEFINE:
    /* 1/30/95 sb -- To accommodate the unspeakably obnoxious GCC-style
       implicit newlines in string literals, we have to tokenize the
       entire line even inside a false #if, just in case there's a
       wrapped string in it.  Grmbl... */
    if (cond_true())
      do_define();
    else if (gcc_strings)
      _tokenize_line();
    break;
  case POUND_UNDEF:
    if (cond_true())
      do_undefine();
    break;
  case POUND_INCLUDE:
    if (cond_true())
      do_include();
    break;
  case POUND_IF:
    do_if();
    break;
  case POUND_IFDEF:
    do_ifdef();
    break;
  case POUND_IFNDEF:
    do_ifndef();
    break;
  case POUND_ELSE:
    do_else();
    break;
  case POUND_ELIF:
    do_elif();
    break;
  case POUND_ENDIF:
    do_endif();
    break;
  case POUND_LINE:
    if (cond_true())
      do_line();
    break;
  case POUND_ERROR:
    if (cond_true())
      do_error();
    break;
  case POUND_PRAGMA:
    if (cond_true())
      do_pragma();
    break;
  case POUND_EMPTY:
    break;
  default:
    fatal("unrecognized preprocessor directive #%s", token_txt(T));
  }
  free_token(T);
  flush_line();
}
