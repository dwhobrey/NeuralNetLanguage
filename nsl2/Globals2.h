/* Name: globals.h   Type: header file   Date: 30 March 89
*   Nesel Network Specification Language Preprocessor
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <process.h>
#include <malloc.h>
#include <signal.h>

#ifndef size_t
typedef unsigned int size_t;
#endif

typedef unsigned char boolean; /* Just for clarity */
typedef unsigned char byte; /* Hopefully an 8-bit value */
typedef char siby; /* signed byte value */
typedef unsigned short int word; /* Hopefully 16-bit value */
typedef word token; /* tokens must be words */
typedef short int swin; /* signed 16-bit value */
typedef short int mile; /* array indexing values */
/*NOTE iohi2.c depends on type of mile as does nial2.c !! */
typedef long yard; /* node number values */
typedef long foot; /* file position pointer values */
typedef double reals; /* the type of reals */

#define T (boolean)1 /* value of boolean True */
#define F (boolean)0 /* value of boolean False */

/* Termination number codes */
#define EXIT_OK ((int)0) /* exit successfully */
#define EXIT_ERRERR ((int)1) /* unknown error */
#define EXIT_INT ((int)2) /* exit on interrupt */
#define EXIT_BADFILE ((int)3) /* No (accessible) input/output file */
#define EXIT_OSPACE ((int)4) /* Ran out of memory space */
#define EXIT_EOF ((int)5) /* EOF encountered prematurely */
#define EXIT_PARSE ((int)6) /* Non recoverable parsing error occurred */
#define EXIT_ROOT ((int)7) /* Root network not found */
#define EXIT_FILE ((int)8) /* Access to file went wrong */
#define EXIT_INT_ERR ((int)9) /* Something has gone wrong internally */
#define EXIT_INT_ARY ((int)10) /* Iternal err: Tried to access a null array */
#define EXIT_CONX ((int)+11) /* Ds's conx account inconsistent */
#define EXIT_REC_MOD ((int)13) /* Module was recursively defined */
#define EXIT_VERSION ((int)14) /* Bad nsl1 object module version number */
#define EXIT_DSP_FUL ((int)+15) /* Ds pool queue overflow */
#define EXIT_NOGO ((int)+16) /* Func etc not implemented yet */
#define EXIT_BADNSS ((int)+17) /* bad nss file */

#define ERR_BASE ((int)20) /* first nonterminating error code */
#define ERR_OK ((int)0) /* must have same code as EXIT_OK */

/* Expression evaluation error numbers */
#define ERR_ILL (ERR_BASE+0) /* badly formed expression, returns 0.0 */
#define ERR_PAR (ERR_BASE+1) /* yacc couldn't parse it correctly */
#define ERR_YER (ERR_BASE+2) /* yacc error occurred */
#define ERR_TOO (ERR_BASE+3) /* too many user functions defined */
#define ERR_DEF (ERR_BASE+4) /* function already defined */
#define ERR_ARG (ERR_BASE+5) /* too many args to function */
#define ERR_MEM (ERR_BASE+6) /* not enough memory to install function */
#define ERR_ARY (ERR_BASE+7) /* error parsing array list */
#define ERR_NET_CHK (ERR_BASE+8) /* net consistancy is suspect */

#define ERR_OPTH_NAM (ERR_BASE+10) /* name expected in path */
#define ERR_NON_OPTH (ERR_BASE+11) /* no matching outpath found */
#define ERR_LAR_BAD (ERR_BASE+12) /* label elm not in map list */
#define ERR_TER_OPTH (ERR_BASE+13) /* path terminated unexpectantly */
#define ERR_ILL_GRP (ERR_BASE+14) /* bad group member */


#define ERR_ILL_SEL (ERR_BASE+20) /* selector op illegal in lary def */
#define ERR_NO_MAT (ERR_BASE+21) /* no matching path found */
#define ERR_UA_ENC (ERR_BASE+22) /* unbound array encountered unexpectedly */
#define ERR_CXIN_INDX (ERR_BASE+23) /* source conx index out of range */
#define ERR_ATM_CON (ERR_BASE+24) /* atom I/V conflict */

#define RAISE_TOP (ERR_BASE+29) /* max err number that causes a raise_error */

#define ERR_ARY_ORG (ERR_BASE+30) /* list element out of range */
#define ERR_ARY_MIS (ERR_BASE+31) /* list elms size mismatch */

#define WRN_BASE (ERR_BASE+40) /* first nonterminating warning code */

#define WRN_BAD_OPTH (WRN_BASE+1) /* opath was bad?? */
#define WRN_FLO_OVR (WRN_BASE+2) /* floated Inpath above root */
#define WRN_OUT_OFL (WRN_BASE+3) /* opath floated above root */
#define WRN_PRT_OOR (WRN_BASE+4) /* sel partnum out of range */
#define WRN_SEL_OOR (WRN_BASE+5) /* selector out of range */
#define WRN_SEL_REC (WRN_BASE+6) /* sel eval leads to inf rec, returns (-1)*/
#define WRN_SEL_BUN (WRN_BASE+7) /* tried to bypass [*] but couldn't */
#define WRN_LRY_BYP (WRN_BASE+8) /* couldn't bypass a lary */
#define WRN_LST_UNF (WRN_BASE+9) /* tried to append to list past end */
#define WRN_AUX_MIN (WRN_BASE+10) /* tried to del an aux def */
#define WRN_AUX_UND (WRN_BASE+11) /* aux undefined */
#define WRN_SEL_ASG (WRN_BASE+12) /* can only assign to $!@ and $!# sels */
#define WRN_VAR_SEL (WRN_BASE+13) /* selector not allowed in var decls */
#define WRN_VAR_OUT (WRN_BASE+14) /* no out path exists for var decls */
#define WRN_VAR_LVL (WRN_BASE+15) /* DS & it's initialiser lvl mismatch */
#define WRN_VAR_UNI (WRN_BASE+16) /* unions currently not supported */
#define WRN_ELM_OOR (WRN_BASE+17) /* decl index out of range */
#define WRN_ELM_ABR (WRN_BASE+18) /* had to abort decl ary, elms oor */
#define WRN_VAR_NOD (WRN_BASE+19) /* struct expected */
#define WRN_VAR_ATM (WRN_BASE+20) /* atom expected */
#define WRN_VAR_ARY (WRN_BASE+21) /* array cast not expected */
#define WRN_VAR_FAIL (WRN_BASE+22) /* var decl path failed to reach node */
#define WRN_USR_FUN (WRN_BASE+23)  /* user func evaluation failed */
#define WRN_VAR_MIS (WRN_BASE+24) /* struct init member missing */
#define WRN_SFLO_OFL (WRN_BASE+25) /* sel path abs past root */
#define WRN_SNO_TAIL (WRN_BASE+26) /* sel path beyond tail */
#define WRN_ARY_TYP (WRN_BASE+27) /* mixing list elm types */
#define WRN_INL_VAR (WRN_BASE+28) /* Op eval'd already when needed by invar */
#define WRN_GLO_UNF (WRN_BASE+29) /* global label not defined */

#define WRN_TWO (WRN_BASE+30)
#define WRN_CXIN_UNS (WRN_TWO+1) /* some conxs in oelm not used */
#define WRN_CXIN_TYPE (WRN_TWO+2) /* destination conx of wrong type */
#define WRN_CXIN_OWRT (WRN_TWO+3) /* new conx would overwrite, so buffer it */
#define WRN_CXIN_TMIS (WRN_TWO+4) /* sor & dest conxs type mismatch */
#define WRN_CXIN_NEQU (WRN_TWO+5) /* conx to del at pos not equal to conx*/
#define WRN_CXIN_NTCX (WRN_TWO+6) /* tried to del a non conx */
#define WRN_CXIN_DIVR (WRN_TWO+7) /* cx diverted to bucket, dest wrng type */
#define WRN_CXIN_DIDE (WRN_TWO+8) /* cx dvrtd to bckt & not del'd frm dst*/
#define WRN_FRK_NUL (WRN_TWO+9) /* no conxs in fork path */
#define WRN_OEL_NUL (WRN_TWO+10) /* no conxs in oelm path */
#define WRN_PAR_EMP (WRN_TWO+11) /* no members found in partition */
#define WRN_CX_NEX (WRN_TWO+12) /* no conx exists at specified leaf */
#define WRN_VAR_UNK (WRN_TWO+13) /* unknown type...processed anyway */
#define WRN_VAR_TMS (WRN_TWO+14) /* node cast applied to different type */
#define WRN_VAR_STR (WRN_TWO+15) /* node cast applied to struct */
#define WRN_VAR_TYP (WRN_TWO+16) /* var type mismatch */
#define WRN_VAR_TST (WRN_TWO+17) /* struct type mismatch */
#define WRN_VAR_CNX (WRN_TWO+18) /* tried to initialise a conx */
#define WRN_NODE_NOIO (WRN_TWO+19) /* node has no i/o cxs */
#define WRN_NO_COMMS (WRN_TWO+20) /* (in/out) comms not allowed on id */
#define WRN_BIN_CX (WRN_TWO+22) /* can't get conxs from bin */
#define WRN_STK_EMPT (WRN_TWO+23) /* heap/pile empty, conx expected */
#define WRN_STK_NTEM (WRN_TWO+24) /* heap/pile not empty */
#define WRN_STK_IOOR (WRN_TWO+25) /* heap/pile index out of range */

#define WRN_THREE (WRN_TWO+30)
#define WRN_TIL_UNF (WRN_THREE+1) /* '~' or '~~' used before ref path known*/
#define WRN_QST_ZER (WRN_THREE+2) /* append LOP, nothing to append??*/
#define WRN_NET_PAR (WRN_THREE+3) /* $!x used on a net, returns fixed val */
#define WRN_NET_EXC (WRN_THREE+4) /* $!@,# used on a net, returns has val */
#define WRN_SEL_SER (WRN_THREE+5) /* sel unintentionally refs self list */
#define WRN_USR_NAM (WRN_THREE+6) /* user func number of args mismatch */
#define WRN_STIL_UNF (WRN_THREE+7) /* sel path '~' not def'd yet */

#define WRN_FOUR (WRN_THREE+20)
#define WRN_DS_NARB (WRN_FOUR+1) /* DS member ref'd wasn't an ary */
#define WRN_SEL_DAR (WRN_FOUR+2) /* ary '.' applied to wasn't dary */
#define WRN_LOP_NUL (WRN_FOUR+3) /* lop returned null list [] */
#define WRN_LOP_ZER (WRN_FOUR+4) /* LOP evals to nul lop[0..0] */
#define WRN_LRY_HTB (WRN_FOUR+5) /* had to bypass a lary */
#define WRN_SEL_UNL (WRN_FOUR+6) /* sel not eval'd yet when needed by lop */
#define WRN_UAL_NUL (WRN_FOUR+7) /* unused UAs automatically set to NULL pts*/
#define WRN_PAR_ZER (WRN_FOUR+8) /* no members in a [+] LOP partition */
#define WRN_PAR_EXD (WRN_FOUR+9) /* no members because part level too big */

#define WRN_INT_ERR (WRN_FOUR+18) /* Debug: possible internal error */

#define WRN_MAX (WRN_FOUR+20)

/* path error report types */
#define PER_REF 0 /* Ref-path */
#define PER_IN 1 /* In-path */
#define PER_OUT 2 /* Out-path */
#define PER_GEN 3 /* General-path */

#define max_ident 80 /* Size of identifiers */
#define namesize 40 /* max chrs per filename  */
#define bigbuffsize 2000 /* max chrs in an input line */

#define malmem(tt,nn) ((tt *)mem_mal((size_t)(nn)*sizeof(tt)))
#define relmem(tt,nn,pp,ss) ((tt *)mem_rel(pp,(size_t)(nn),(size_t)(ss)))
#define fremem(p) mem_free(p)
/* THESE MUST BE MACRO's */
/* free a malloced block if a is not null */
#define chk_free(p) if(p)fremem(p)
/* free a malloced block & zero pointer */
#define nul_free(p) if(p){fremem(p);p=NULL;}

/* return magnitude of item */
#define magy(val) (((val)>=0)?(val):(-(val)))

/* See if char is a valid identifier char */
#define isident(c) ((boolean)(isalnum(c) || c=='_'))

/* return T if type cast contains '[' */
#define cast_array(str) ((boolean)(strchr(str,'[')!=NULL))

#define fcat2(a,b,c) fcat(fcat(a,b),c)

extern void signal_error(),path_error(),in_error(),out_error(),mem_free();
extern int find_name(),cast_type();
extern boolean strideq(),match();
extern char *strsave(),*fcat(),*base_str(),*mem_mal(),*mem_rel();

/* Only these token-handler routines should be used externally */
extern token str_to_token(),data_to_token(),copy_token(),atoms_to_token();
extern token *token_to_data();
extern char *token_to_str(),*token_value();
extern void del_token(),more_tokens(),token_free();
extern swin token_refs();
