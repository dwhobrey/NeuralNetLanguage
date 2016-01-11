/* Name: globals.h   Type: header file   Date: 30 March 89
*  NeSeL Network Specification Language Preprocessor
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <process.h>
#include <malloc.h>

/* The type used for user functions & real exp evaluation
*  Change each occurrence of the four "float"s or "double"s below */
typedef double reals;
#define funstr1 "float (*func_pts[])()={\n"
#define funstr2 "(float (*)())%s,\n"
#define funstr3 "(float (*)())0};\n"

/* Termination number codes */
#define EXIT_OK ((int)0) /* exit successfully */
#define EXIT_ERRERR ((int)1) /* unknown error */
#define EXIT_INT ((int)2) /* exit on interrupt */
#define EXIT_ERRS ((int)3) /* num errs exceeded cutoff limit */
#define EXIT_BADFILE ((int)4) /* no (accessible) input/output file */
#define EXIT_OSPACE ((int)5) /* Ran out of memory space */
#define EXIT_ROOT ((int)6) /* Root network not found */
#define EXIT_FILE ((int)7) /* Access to file went wrong */
#define EXIT_INT_ERR ((int)8) /* Something has gone wrong internally */
#define EXIT_TYP_EOF ((int)9) /* EOF in typedef ',' or ';' */
#define EXIT_TYP_LBR ((int)10) /* EOF in struct '{' */
#define EXIT_TYP_RBR ((int)11) /* EOF in struct '}' */
#define EXIT_INITIAL ((int)12) /* could not initialise */
#define EXIT_SUBLVL ((int)13) /* too many struct sub levels */
#define EXIT_UNDEF ((int)14) /* undefined modules in net */
#define EXIT_SALOVR ((int)15) /* too many lists within list */

#define ERR_BASE ((int)20) /* first nonterminating error code */
#define ERR_OK ((int)0) /* must have same code as EXIT_OK */


/* Expression evaluation error numbers */
#define ERR_PAR (ERR_BASE+1) /* yacc couldn't parse it correctly */
#define ERR_ARG (ERR_BASE+2) /* too many args to function */
#define ERR_MEM (ERR_BASE+3) /* not enough memory to install function */
#define ERR_FILE (ERR_BASE+4) /* access to file went wrong */
#define ERR_MESS (ERR_BASE+5) /* display user supplied err message */
#define ERR_SEMIC (ERR_BASE+6) /* Semicolon expected */
#define ERR_MACRO (ERR_BASE+7) /* Unable to macro expand, for some reason */
#define ERR_TAG (ERR_BASE+8) /* struct defs not allowed in node def, use tag */
#define ERR_BRAK (ERR_BASE+9) /* run away bracket */
#define ERR_VAR_EXP (ERR_BASE+10) /* bad consstant expression */
#define ERR_VAR_FIX (ERR_BASE+11) /* exp err in decl array */
#define ERR_VAR_BIT (ERR_BASE+12) /* exp err in decl bitfield exp*/
#define ERR_VAR_RSB (ERR_BASE+13) /* ']' expected in decl ary decl*/
#define ERR_BUF_OVR (ERR_BASE+14) /* Buffer overflow */
#define ERR_COM_DIS (ERR_BASE+15) /* an internal var has disappeared!? */
#define ERR_SEC_REC (ERR_BASE+16) /* recursively defined module */
#define ERR_RED_WORD (ERR_BASE+17) /* Attempt to redefine reserved word */
#define ERR_ARY_AND (ERR_BASE+18) /* adrs op '&' not allowed in path list */
#define ERR_NER_SYN (ERR_BASE+20) /* syntax node def: '}' ';' expected */
#define ERR_NER_ID (ERR_BASE+21) /* node name expected */
#define ERR_TYP_SYN (ERR_BASE+22) /* '}' expected in typedef */
#define ERR_TYP_ID  (ERR_BASE+23) /* name expected in type declaration */
#define ERR_TYP_UDF (ERR_BASE+24) /* undefined type */
#define ERR_MOD_MEM (ERR_BASE+25) /* Not enough mem to load module */
#define ERR_MOD_BRK (ERR_BASE+26) /* Missing bracket in module */
#define ERR_MOD_COMM (ERR_BASE+27) /* err in module parms */
#define ERR_MOD_TYP (ERR_BASE+28) /* Missing mod type decl */
#define ERR_APP_ARY (ERR_BASE+29) /* terminal ary in app path not allowed */
#define ERR_PAR_NET (ERR_BASE+30) /* err parsing net module */
#define ERR_PAR_FUN (ERR_BASE+31) /* err parsing function initialisers */
#define ERR_PAR_ARY (ERR_BASE+32) /* err parsing array */
#define ERR_PAR_STU (ERR_BASE+33) /* err parsing struct */
#define ERR_PAR_COM (ERR_BASE+34) /* err parsing comms */
#define ERR_STU_DIS (ERR_BASE+35) /* struct has disappeared!*/
#define ERR_STU_ERR (ERR_BASE+36) /* struct err, internal weird error */
#define ERR_STU_REC (ERR_BASE+37) /* struct recursively defed */
#define ERR_STU_UDS (ERR_BASE+38) /* struct undefined */
#define ERR_STU_SEM (ERR_BASE+39) /* ';' expected after struct def */
#define ERR_MAC_SYN (ERR_BASE+40) /* name expected, macro def syntax err */
#define ERR_MAC_RBR (ERR_BASE+41) /* '}' macro def syntax err */
#define ERR_USR_ARG (ERR_BASE+42) /* user func arg undefined or inconsistent */
#define ERR_USR_UID (ERR_BASE+43) /* user func arg undefined */
#define ERR_USR_MID (ERR_BASE+44) /* user func arg multiply defined */
#define ERR_USR_BRK (ERR_BASE+45) /* right bracket missing in user func */
#define ERR_USR_PARMS (ERR_BASE+46) /* too many args to user func */
#define ERR_PAR_NOP (ERR_BASE+47) /* err parsing node parms */
#define ERR_NUL_NET (ERR_BASE+48) /* non aux null net encountered */

#define WRN_BASE (ERR_BASE+50) /* first nonterminating warning code */
#define WRN_PAR_MUL (WRN_BASE+0) /* multiply defined labels in module */
#define WRN_PAR_UDS (WRN_BASE+1) /* undefined labels/modules in module */
#define WRN_HASH (WRN_BASE+2) /* include file was inserted in a weird spot*/
#define WRN_SEL_AND (WRN_BASE+3) /* sels and adrs op used in a var exp*/
#define WRN_OBJ_SIZ (WRN_BASE+4) /* object size less than 1, taking 1 */
#define WRN_OBJ_ARG (WRN_BASE+5) /* arg applied to object must have a name */
#define WRN_VAR_NAME (WRN_BASE+6) /* var name expected */
#define WRN_VAR_SYN (WRN_BASE+7) /* unexpected chr in declaration */
#define WRN_TYP_MUL (WRN_BASE+8) /* typedef label multiply defined */
#define WRN_NOD_STAR (WRN_BASE+9) /* pts to nodes not allowed */
#define WRN_RESYN (WRN_BASE+10) /* could not recover from previous err */
#define WRN_TYP_UNK (WRN_BASE+11) /* unknown type, treating as int */
#define WRN_MAC_UNX (WRN_BASE+12) /* didn't expand a macro */
#define WRN_NUL_STRUC (WRN_BASE+13) /* null struct {} initialiser */

#define max_ident 80/* Size of identifiers */
#define namesize 40 /* max chrs per filename  */
#define buffsize 250 /* max chrs in input line buffer */
#define argbuffsize 2000 /* func args must fit in this buff */
#define bigbuffsize 2000

#define fcat2(a,b,c) fcat(fcat(a,b),c)
#define fcat3(a,b,c,d) fcat(fcat(fcat(a,b),c),d)
#define fcat4(a,b,c,d,e) fcat(fcat(fcat(fcat(a,b),c),d),e)
#define fcat5(a,b,c,d,e,f) fcat(fcat(fcat(fcat(fcat(a,b),c),d),e),f)
#define fcat6(a,b,c,d,e,f,g) fcat(fcat(fcat(fcat(fcat(fcat(a,b),c),d),e),f),g)

#ifndef BOOL
typedef enum {FALSE=0,TRUE=1} BOOL;
#endif

/* Macro for marking current position for error reporting.*/
#define error_mark() {err_file_num=cur_file_num; err_line_num=cur_line_num;}

extern void append_list_name(),sort_list(),insert_hole(),signal_error();
extern BOOL isident();
extern int enlarge_array(),find_name(),add_list_name(),search_name(),
  update_list_name();
extern char *strsave(),*fcat(),comm_code();
