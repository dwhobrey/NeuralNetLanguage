/* Name: yaccety.h   Type: header file   Date: 18 May 89
*   NeSeL Network Specification Language Preprocessor
*/

#define PATHDOT "@" /* internal symbol used to represent pathsep ".." */
#define PATHPAR "\\" /* internally '\' used as a partition op (\\,//) */
                      /* internally we use '/','.' as a separator */

#define STR_SARY "[A$Z]"

#define SAL_MAX 40 /*  2 * max number of lists within lists */
#define SCR_WD 80  /* screen width for err reporting */

/* some macros to save time typeing */
#define fsav2(a,b) (fcat2(yacc_buff,a,b), yacsave(yacc_buff))
#define fsav3(a,b,c) (fcat3(yacc_buff,a,b,c), yacsave(yacc_buff))
#define fsav4(a,b,c,d) (fcat4(yacc_buff,a,b,c,d), yacsave(yacc_buff))
#define fsav5(a,b,c,d,e) (fcat5(yacc_buff,a,b,c,d,e), yacsave(yacc_buff))
#define fsav6(a,b,c,d,e,f) (fcat6(yacc_buff,a,b,c,d,e,f), yacsave(yacc_buff))

/* Type codes for lists */
#define LIST_WILD 'A'  /* list is wild map operator [*] */
#define LIST_ATCON 'B' /* atom constant */
#define LIST_ATSEL 'C' /* atom containing selectors */
#define LIST_ATLOP 'D' /* single list operator, we add 2*/
#define LIST_LSCON 'D' /* list of constants */
#define LIST_LSSEL 'E' /* list containing selectors */
#define LIST_LSLOP 'F' /* list containing list operator */
#define LIST_ATPAR 'G' /*H atom, but contained partition op $!x */
#define LIST_LSPAR 'I' /* list, but contained $!x */
#define LIST_ATEXC 'J' /*K atom, but contained $!(!@#) */
#define LIST_LSEXC 'L' /* list, but contained $!(!@#) */
#define LIST_ATONE 'M' /*N atom, but contained $!1 */
#define LIST_LSONE 'O' /*P list, but contained $!1 */
#define LIST_ATQST 'O' /*P single list operator [?] [+], we add 2*/
#define LIST_LSQST 'Q' /* list containing list-operator [?] [-?] [+] [-+]*/

#define LIST_LOFS (char)2 /* valued added to code to signal it's a list */

/* bits set in sal_lis indicating various conditions: */
#define SIS_LIS 1 /* list contains more than one element */
#define SIS_RNG 2 /* list contains a range */
#define SIS_VER 4 /* list was used with a vertical cast specifier */
#define SIS_LMK 6 /* for masking out SIS_LIS bit */
#define SIS_RMK 5 /* for masking out SIS_RNG bit */
#define SIS_VMK 3 /* for masking out SIS_VER bit */

#define sal_lop(nn) if(sal_typ<nn)sal_typ=nn

typedef struct _selm
{ BOOL rev; char *str;} selm;
typedef struct _xelm
{ BOOL bad; int typ; reals val; char *str;} xelm;

extern BOOL isnewd,isnode,chklsbrk,var_mode,hasand,haslong,hassel;

extern int 
   tmp_int,sal_skpt,
   yacc_entry, /* tells yacc what rule to start from if set */
   yacc_error, /* if error occurs report it as this type */
   comm_mode, /* tells yylex to tokenise 'in' 'out' & 'inout' */
   optok; /* coded oelm comb ops */

extern long tmp_decl_num,tmp_aux_num,tmp_var_num;

extern char 
     *yacc_tmp_pt,  /* pts to macro arg string during object parsing */
     *yacc_io, /* pts to start of str to parse */
     *yacc_ioerr, /* pts to point at which an err was first detected */
     *cint_str, /* code str for var type (int or double)*/
     sal_lis,sal_typ,sal_stk[SAL_MAX], /*stk for lst states during eval*/
     yacc_buff[argbuffsize];

extern reals yacc_exp_value;

extern void yyerror(),initialise_yacc(),store_func_id(),save_ins(),
   push_vare(),pop_vare(),clr_vare(),comm_push(),sal_push(),sal_pop(),
   sal_start(),do_con_member(),do_obj_spec(),syn_decl_do(),save_id(),
   dqstr_to_mem();
extern BOOL yacc_parse_decl(),yacc_parse_exp(),yacc_parse_struct(),
   yacc_parse_finit(),yacc_parse_comms(),yacc_parse_parms();
extern int yylex(),yyparse(),save_outs(),sqchrtobin(),resynch();
extern reals do_exp_op();
extern char *yacsave(),*sal_end(),*loop_end(),*num_asc_str(),
   *do_con_path(),*do_aux_spec(),*add_name();