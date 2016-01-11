%{
/* Name: nslyacc2.y   Type: yacc file   Date: 18 May 89
*  NeSeL Network Specification Language
*/
#include "globals2.h"
#include "iolo2.h"
#include "nial2.h"
#include "nsltwo2.h"
#include "pathlo2.h"
#include "nslyacc2.h"
%}
%start START
%union
{int ival;
 long lval;
 sielm sival;
 NSLNSLfval fval;
 char *sval;
}
%type <fval> e term select
%right <ival> SEXP SVAR SARY
%left <sval> ',' ';'
%right <fval> NUMBER
%right <sival> FUNKY PUNKY
%right <ival> ELMCAST
%right <sval> DOLVAR FIXPATH
%right <sval> DQSTR STRPATH
%right <ival> EQOP
%binary <sval> DOTDOT
%right <sval> '?' ':'
%left <sval> OROR
%left <sval> ANDAND
%left <sval> '|'
%left <sval> '^'
%left <sval> '&'
%left <sval> EQ NE
%left <sval> '<' '>' LE GE
%left <sval> LS RS
%left <sval> '+' '-'
%left <sval> '*' '/' '%' '\\'
%right <ival> PLUPLU MINMIN
%right <sval> UMINUS '!' '~' '$' '@' ')' ']' '}'
%right <ival> DOLEXC
%right <sival> DOLSEL DOLPAR LSTOPR DOLXS
%right <ival> LOOPOPR
%left <sval> '(' '[' '{' '.'
%%

START:
	  SVAR var
	{YYACCEPT;}
	| SEXP '(' e ')'
	{exp_value.val.dd=tx_to_d($3); exp_value.typ=TYPE_DOUBLE; YYACCEPT;}
	| SARY array
	{YYACCEPT;}
;
e:
	  e '*' e
	{$$=d_to_tx($1.typ,$3.typ,tx_to_d($1)*tx_to_d($3));}
	| e '/' e
	{$$=d_to_tx($1.typ,$3.typ,tx_to_d($1)/tx_to_d($3));}
	| e '%' e
	{$$=d_to_tx($1.typ,$3.typ,(double)((long)tx_to_d($1) % (long)tx_to_d($3)));}
	| e '+' e
	{$$=d_to_tx($1.typ,$3.typ,tx_to_d($1)+tx_to_d($3));}
	| e '-' e
	{if(($1.typ==TYPE_PT||$1.typ&STAR_MASK)&&($3.typ==TYPE_PT||$3.typ&STAR_MASK))
	   $$=d_to_t((mile)TYPE_LONG,tx_to_d($1)-tx_to_d($3));
	 else  $$=d_to_tx($1.typ,$3.typ,tx_to_d($1)-tx_to_d($3));}
	| e LS e
	{$$=d_to_tx($1.typ,$3.typ,(double)((long)tx_to_d($1) << (long)tx_to_d($3)));}
	| e RS e
	{$$=d_to_tx($1.typ,$3.typ,(double)((long)tx_to_d($1) >> (long)tx_to_d($3)));}
	| e '<' e
	{$$=d_to_tx($1.typ,$3.typ,(double)(tx_to_d($1)<tx_to_d($3)));}
	| e '>' e
	{$$=d_to_tx($1.typ,$3.typ,(double)(tx_to_d($1)>tx_to_d($3)));}
	| e LE e
	{$$=d_to_tx($1.typ,$3.typ,(double)(tx_to_d($1)<=tx_to_d($3)));}
	| e GE e
	{$$=d_to_tx($1.typ,$3.typ,(double)(tx_to_d($1)>=tx_to_d($3)));}
	| e EQ e
	{$$=d_to_tx($1.typ,$3.typ,(double)(tx_to_d($1)==tx_to_d($3)));}
	| e NE e
	{$$=d_to_tx($1.typ,$3.typ,(double)(tx_to_d($1)!=tx_to_d($3)));}
	| e '&' e
	{$$=d_to_tx($1.typ,$3.typ,(double)((long)tx_to_d($1) & (long)tx_to_d($3)));}
	| e '^' e
	{$$=d_to_tx($1.typ,$3.typ,(double)((long)tx_to_d($1) ^ (long)tx_to_d($3)));}
	| e '|' e
	{$$=d_to_tx($1.typ,$3.typ,(double)((long)tx_to_d($1) | (long)tx_to_d($3)));}
	| e ANDAND e
	{$$=d_to_tx($1.typ,$3.typ,(double)((long)tx_to_d($1) && (long)tx_to_d($3)));}
	| e OROR e
	{$$=d_to_tx($1.typ,$3.typ,(double)((long)tx_to_d($1) || (long)tx_to_d($3)));}
	| e '?' e ':' e
	{if((long)tx_to_d($1)) $$=$3; else $$=$5;}
	| term
	{$$=$1;}
	| FIXPATH
	{pathsel_val($1); $$=exp_value;}
	| FUNKY args
	{user_func($1.indx); $$=nsl_func_rr;}
;
term:
	  '-' term %prec UMINUS
	{$$=d_to_t($2.typ,-(tx_to_d($2)));}
	| '!' term
	{$$=d_to_t($2.typ,(double)(!(long)tx_to_d($2)));}
	| '~' term
	{$$=d_to_t($2.typ,(double)(~(long)tx_to_d($2)));}
	| select
	| '(' e ')'
	{$$=$2;}
	| NUMBER
	{$$=$1;}
	| DQSTR
	{$$.val.pp=(char *)$1; $$.typ=(STAR_ONE+TYPE_CHAR);}
	| DOLVAR
	{$$=var_selector($1,(int)DOLVAR,zerval,T);}
	| DOLVAR PLUPLU
	{$$=var_selector($1,(int)PLUPLU,zerval,T);}
	| DOLVAR MINMIN
	{$$=var_selector($1,(int)MINMIN,zerval,T);}
	| DOLVAR EQOP e
	{$$=var_selector($1,$2,$3,F);}
	| PLUPLU DOLVAR
	{$$=var_selector($2,(int)PLUPLU,zerval,F);}
	| MINMIN DOLVAR
	{$$=var_selector($2,(int)MINMIN,zerval,F);}
;
args:
	  '(' ')'
	| '(' arg_list ')'
;
arg_list:
	  arg
	| arg_list ',' arg
;
arg:
	  {cur_cast=STAR_PAIR;} arg_exp
	| ELMCAST {cur_cast=$1;} arg_exp
;
arg_exp:
	  e
	{nsl_func_rr=$1; push_exp(cur_cast);}
	| sslist
	{/* a lst_elm should be on stack already to take new list */
	 if(!lst_ok(lst_stk)) signal_error(EXIT_INT_ERR,154);
	 nsl_func_rr.val.pp=(char *)(*lst_tail(lst_stk,char *));
	 nsl_func_rr.typ=TYPE_PT;
	 push_exp((mile)0);
	 add_lst_stk(); /* create a new hole in case elist contains another list */
	}
	| PUNKY args
	{user_func($1.indx); push_exp(cur_cast); /* assume lst func, unless cast */
	 if(cur_cast!=STAR_ISPT) /* a list rather than a simple pt */
	   {if(!lst_ok(lst_stk)) signal_error(EXIT_INT_ERR,155);
	    /*push lst details on lst_stk for deling later*/
	    push_lst(nsl_func_rr.val.pp);
	   }
	}
	| DOLXS term
	{nsl_func_rr.val.pp=path_selector((int)$1.sel,(int)$1.indx,(int)tx_to_d($2));
	 nsl_func_rr.typ=TYPE_PT; push_exp((mile)0);}
	| STRPATH
	{pathsel_val($1); nsl_func_rr=exp_value; push_exp((mile)0);}
;
select:
	  DOLSEL term
	{$$.val.ii=(int)selector_val((int)$1.sel,
	    (int)$1.indx,(int)tx_to_d($2),cur_cast);
	 $$.typ=TYPE_INT;
	 if(yyary_pt->suspend) YYACCEPT;}
	| DOLPAR term
	{$$.val.ii=(int)part_selector((int)$1.indx,(int)tx_to_d($2),T);
	 $$.typ=TYPE_INT;
	 if(yyary_pt->suspend) YYACCEPT;}
	| DOLEXC
	{$$.val.ii=(int)exc_selector((int)$1);
	 $$.typ=TYPE_INT;
	 if(yyary_pt->suspend) YYACCEPT;}
;
array:
	  sslist
	| ELMCAST {cur_cast=$1;} sslist
	| ',' {if(yyary_pt->ary_list) cur_cast=lst_typ(yyary_pt->ary_list);} aelms ']'
;
sslist:
	  '[' aelms ']'
	| LSTOPR term ']'
	{list_operator((int)$1.sel,(int)$1.indx,(int)tx_to_d($2),cur_cast);
	 if(!infunc && !list_miss && yyary_pt->ary_list) list_add=T;
	 if(yyary_pt->suspend) YYACCEPT;
	}
;
aelms:
	  anum
	| aelms ',' {if(!infunc) {seppt=get_io_pos()-1; sepstate=list_type;}} anum
;
anum:
	 ':' {++list_miss;} amem {--list_miss;}
	| amem
;
amem:
	  sslist
	| e
	{lst_ary_list((reals)tx_to_d($1),(reals)0.0,cur_cast,T);
	 if(!infunc && !list_miss && yyary_pt->ary_list) list_add=T;}
	| e DOTDOT e
	{lst_ary_list((reals)tx_to_d($1),(reals)tx_to_d($3),cur_cast,F);
	 if(!infunc && !list_miss && yyary_pt->ary_list) list_add=T;}
	| PUNKY args
	{user_func($1.indx);
	 /* add list in nsl_func_rr.pp to cur list */
	 lst_ary_ary(nsl_func_rr.val.pp,F,cur_cast);
	 lfree(nsl_func_rr.val.pp);
	 if(!infunc && !list_miss && yyary_pt->ary_list) list_add=T;
	}
	| LOOPOPR
	{list_type=$1; YYACCEPT;}
;
var:
	 var_exp
	| ELMCAST {cur_cast=$1;} var_exp
;
var_exp:
	  e
	{if($1.typ==TYPE_PT||($1.typ&STAR_MASK)) exp_value=$1;
	 else {exp_value.val.dd=tx_to_d($1); exp_value.typ=TYPE_DOUBLE;}}
	| PUNKY args
	{user_func($1.indx); exp_value=nsl_func_rr;}
	| DOLXS term
	{exp_value.val.pp=path_selector((int)$1.sel,(int)$1.indx,(int)tx_to_d($2));
	 exp_value.typ=(STAR_ONE+TYPE_CHAR);}
	| STRPATH
	{pathsel_val($1);}
