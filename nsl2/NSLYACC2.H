/* Name: yaccety2.h   Type: header file   Date: 18 May 89
*   NeSeL Network Specification Language
*/

#define SCR_WD 80 /* width of screen in chrs for yacc error reporting */
#define inc_lst 8
#define inc_exp 8

typedef struct _sielm  /* holds selector type & index type for yacc */
{ int sel; int indx;
} sielm;

/* during eval of lists & user funcs occurring in a list or exp,
*  a stack of sublist & arg numbers--positions is maintained */
typedef struct _exp_elm
{ char *par_stk,*par_str;
  mile **lst_stk,cur_cast;
  int list_miss;
} exp_elm;

/* User function access variables */
/* these following five vars/funcs are defined in nslusrfn.c */
extern void lfree(); /* free a list */
extern int nsl_num_mon_list;  /* number of user functions */
extern NSLNSLfval nsl_mon_evaluate(); /* func to eval user func */
extern char *nsl_mon_names[]; /* list of user func names */
extern NSLNSLfelm nsl_mon_list[]; /* user func addresses & arg details */

NSLNSLfval
  zerval, /* a zero int val */
  exp_value, /* cur expression value */
  nsl_func_rr; /* result of cur user func */

boolean list_add; /* indicates if ary elms were added to ary_pt */

int
  infunc, /* indicates when parsing part of a func arg */
  sepstate, /* state when last list sep was seen */
  list_miss, /* when positive, do not add elms to list: i.e. eval but ignore*/
  cur_cast, /* indicates type of cur elm cast */
  list_type; /* the type of list component just parsed e.g. loop list etc*/

char
  *seppt, /* io pos of last list separator, or start of exp */
  *selec; /* io pos of last lop or selector type code */

ary_elm *yyary_pt; /* cur array limit information */
mile **lst_stk; /* user func's intermediate arrays */
char *par_stk,*par_str; /* cur user func arg list */
exp_elm *exp_stk;

extern void lst_ary_list(),lst_ary_ary(),lst_ary_str(),cir_free(),
  yyerror(),eval_var(),eval_scan(),
  save_exp(),restore_exp(),push_exp(),push_lst(),add_lst_stk(),user_func();
extern char *cir_save();
extern int ary_eval(),yylex(),yyparse();
extern double tx_to_d();
extern NSLNSLfval d_to_tx(),d_to_t(),var_selector();
