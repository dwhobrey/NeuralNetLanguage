%{
/* Name: nslyacc.y   Type: yacc file   Date: 18 May 89
*  NeSeL Network Specification Language Preprocessor
*/
#include "globals.h"
#include "nslone.h"
#include "general.h"
#include "nslyacc.h"
#include "iolo.h"
%}

%start START
%union
{int ival;
 long lval;
 reals dval;
 char *sval;
 selm seval;
 xelm xval;
}

%type <ival> obj_ary
%type <dval> e term
%type <xval> sterm sexp natnum
%type <sval> my_sterm my_sexp
%type <sval> eqops pmops lelm range selnum
%type <sval> stelm stelist str_var arg arg_exp arg_cast_exp arg_val arg_list lelms
%type <sval> salist sslist
%type <sval> select selrel selcoor parnum parexp list_sel outvar_decl
%type <sval> loop_list loop_statement loop_apnd loop_ary for_exp frexp if_list
%type <sval> star_list star_elm var_symbol type_cast type_id type_list
%type <sval> var_decls var_decl var_val var_exp var_name var_cast_exp str_name str_mem
%type <sval> bas_path in_path flo_path obj_path add_path out_path var_path
%type <sval> gen_path dodo_path dada_path app_stem node_prefix ref_dot_path
%type <sval> abs_path ref_path wild_path app_path dec_path dol_path
%type <sval> sel_path vir_innet vir_outnet abssep gensep pathsep tildes
%type <sval> app_id app_typ app_member app_abs out_decl add_lab_path
%type <sval> dol_id path_id wild_id wild_name wild_elms wild_elm first_elm
%type <sval> obj_lab_path net_or_node dint_list dint_star dint_elm dint_id

%right error
%right <ival> DECL EXP STRU FUNCDECL COMMDECL COMMTYPE NODEPARMS
%right PR1
%left <sval> ',' ';'
%right <ival> LSBRK
%right <lval> LNUMBER INUMBER
%right <dval> DNUMBER
%right <sval> IDENT FIDENT VIDENT CBIN CPILE CHEAP
%right <sval> SQCHR DQSTR
%right <sval> '=' MULEQ DIVEQ MODEQ PLUEQ MINEQ ANDEQ XOREQ OREQ LSEQ RSEQ
%binary <sval> DOTDOT DOTSEP DOTDOTSEP DOTDOTID
%right <sval> '?' ':'
%left <sval> OROR
%left <sval> ANDAND
%left <sval> '|'
%left <sval> '^'
%left <sval> '&'
%left <sval> EQ NE
%left <sval> '<' '>' LE GE
%left <sval> LS RS
%left <sval> '+' '-' COPBRK
%left <sval> '*' '/' '%' '\\' '#'
%right <sval> PLUPLU MINMIN UMINUS '!' '~' '$' '@' ')' ']' '}' ATPIP ATQST ATEXC
%right <sval> STARSTAR DOLQUEST DOLDOL
%right <sval> DOLSEL DOLEXC DOLEXCEXC LSTSTAR
%right <seval> LSTOPR
%right <sval> LABEL LABELARY NET NETARGS NODE
%right <sval> CDO CFOR CWHILE CIF CELSE CBREAK CIDENT
%left <sval> '(' '[' '{' '.'
%%

START:
	  DECL
	{comm_mode=TRUE; tmp_aux_num=1; tmp_decl_num=1; tmp_var_num=1; clr_vare();}
	  net_decl
	{YYACCEPT;}
	| FUNCDECL {tmp_aux_num=1; tmp_decl_num=1; tmp_var_num=1; clr_vare();}
	 '{' var_decls '}'
	{YYACCEPT;}
	| EXP {yacc_exp_value=(reals)0.0;} e
	{yacc_exp_value=$3; unput_char((char)yychar); YYACCEPT;}
	| STRU {tmp_var_num=1; clr_vare();} '{' str_decls '}'
	{YYACCEPT;}
	| COMMDECL {comm_mode=TRUE;} comm_decls '{'
	{YYACCEPT;}
	| NODEPARMS '(' node_parms ')'
	{YYACCEPT;}
	| error
	{ resynch(yacc_error,(char *)NULL,(char *)NULL,"\0"); YYACCEPT;}
;
e:
	  e '*' e
	{$$ = $1 * $3;}
	| e '/' e
	{$$ = $1 / $3;}
	| e '%' e
	{$$ = (reals)((long)$1 % (long)$3);}
	| e '+' e
	{$$ = $1 + $3;}
	| e '-' e
	{$$ = $1 - $3;}
	| e LS e
	{$$ = (reals)((long)$1 << (long)$3);}
	| e RS e
	{$$ = (reals)((long)$1 >> (long)$3);}
	| e '<' e
	{$$ = (reals)($1 < $3);}
	| e '>' e
	{$$ = (reals)($1 > $3);}
	| e LE e
	{$$ = (reals)($1 <= $3);}
	| e GE e
	{$$ = (reals)($1 >= $3);}
	| e EQ e
	{$$ = (reals)($1 == $3);}
	| e NE e
	{$$ = (reals)($1 != $3);}
	| e '&' e
	{$$ = (reals)((long)$1 & (long)$3);}
	| e '^' e
	{$$ = (reals)((long)$1 ^ (long)$3);}
	| e '|' e
	{$$ = (reals)((long)$1 | (long)$3);}
	| e ANDAND e
	{$$ = (reals)((long)$1 && (long)$3);}
	| e OROR e
	{$$ = (reals)((long)$1 || (long)$3);}
	| e '?' e ':' e
	{$$ = (reals)((long)$1 ? $3 : $5);}
	| term
	{$$ = $1;}
;
term:
	  '-' term %prec UMINUS
	{$$ = -$2;}
	| '!' term
	{$$ = (reals)!$2;}
	| '~' term
	{$$ = (reals)~(long)$2;}
	| '(' e ')'
	{$$ = $2;}
	| DNUMBER
	{$$= $1;}
	| LNUMBER
	{$$=(reals)$1;}
	| INUMBER
	{$$= (reals)$1;}
	| SQCHR
	{$$= (reals)sqchrtobin($1);}
;
my_sexp:
	  sexp %prec PR1
	{if($1.bad) $$=$1.str; else $$=num_asc_str(&$1);}
;
my_sterm:
	  sterm {if($1.bad) $$=$1.str; else $$=num_asc_str(&$1);}
;
sexp:
	  sexp '*' sexp
	{goto do_sexp;}
	| sexp '/' sexp
	{goto do_sexp;}
	| sexp '%' sexp
	{goto do_sexp;}
	| sexp '+' sexp
	{goto do_sexp;}
	| sexp '-' sexp
	{goto do_sexp;}
	| sexp LS sexp
	{goto do_sexp;}
	| sexp RS sexp
	{goto do_sexp;}
	| sexp '<' sexp
	{goto do_sexp;}
	| sexp '>' sexp
	{goto do_sexp;}
	| sexp LE sexp
	{goto do_sexp;}
	| sexp GE sexp
	{goto do_sexp;}
	| sexp EQ sexp
	{goto do_sexp;}
	| sexp NE sexp
	{goto do_sexp;}
	| sexp '&' sexp
	{goto do_sexp;}
	| sexp '^' sexp
	{goto do_sexp;}
	| sexp '|' sexp
	{goto do_sexp;}
	| sexp ANDAND sexp
	{goto do_sexp;}
	| sexp OROR sexp
	{do_sexp: $$.str=fsav3($1.str,$2,$3.str);
	 $$.val=do_exp_op(&($1),&($3),$2,&($$.bad),&($$.typ));}
	| sexp '?' sexp ':' sexp
	{$$.str=fsav5($1.str,$2,$3.str,$4,$5.str);
	 if((long)$1.val) {$$.val=$3.val; $$.bad=$3.bad; $$.typ=$3.typ;}
	 else {$$.val=$5.val; $$.bad=$5.bad; $$.typ=$5.typ;}
	}
	| sterm
;
sterm:
	  '-' sterm %prec UMINUS
	{$$.str=fsav2($1,$2.str); $$.val=(-$2.val); $$.bad=$2.bad; $$.typ=$2.typ;}
	| '!' sterm
	{$$.str=fsav2($1,$2.str); $$.val=(reals)!$2.val; $$.bad=$2.bad; $$.typ=$2.typ;}
	| '~' sterm
	{$$.str=fsav2($1,$2.str); $$.val=(reals)~(long)$2.val;
	 $$.bad=$2.bad; $$.typ=$2.typ;}
	| '&' var_symbol %prec PLUPLU
	{hasand=TRUE; $$.str=fsav2($1,$2); $$.val=(reals)1.0; $$.bad=TRUE; $$.typ=0;}
	| '(' sexp ')'
	{if($2.bad) $$.str=fsav3($1,$2.str,$3); else $$.str=num_asc_str(&$2);
	  $$.val=$2.val; $$.bad=$2.bad; $$.typ=$2.typ;
	}
	| DNUMBER
	{char *apt; sprintf(yacc_buff,"%g",$1); apt=yacc_buff;
	 while(*apt) {if(*apt=='.') break; ++apt;}
	 if(*apt!='.') fcat(apt,".0");
	 $$.str=yacsave(yacc_buff); $$.val=(reals)$1; $$.bad=FALSE; $$.typ=2;}
	| natnum
	| SQCHR
	{$$.str=$1; $$.val=(reals)sqchrtobin($1); $$.bad=FALSE; $$.typ=0;}
	| DQSTR
	{$$.str=$1; $$.val=(reals)1.0; $$.bad=TRUE; $$.typ=0;}
	| FIDENT  '(' ')'
	{$$.str=fsav3("$@",$1,"()"); $$.val=(reals)1.0; $$.bad=TRUE; $$.typ=0;}
	| FIDENT  '(' arg_list ')'
	{$$.str=fsav5("$@",$1,$2,$3,$4); $$.val=(reals)1.0; $$.bad=TRUE; $$.typ=0;}
	| VIDENT pmops
	{$$.str=fsav3("$A",$1,$2); $$.val=(reals)1.0; $$.bad=TRUE; $$.typ=0;}
	| VIDENT eqops sexp
	{$$.str=fsav4("$A",$1,$2,$3.str); $$.val=(reals)1.0; $$.bad=TRUE; $$.typ=0;}
	| pmops VIDENT
	{$$.str=fsav3($1,"$A",$2); $$.val=(reals)1.0; $$.bad=TRUE; $$.typ=0;}
	| VIDENT
	{$$.str=fsav2("$A",$1); $$.val=(reals)1.0; $$.bad=TRUE;$$.typ=0;}
	| select app_path %prec PR1
	{char *apt=($2); apt+=strlen(apt)-1;
	 if(*apt==']') resynch(ERR_APP_ARY,NULL,NULL,NULL);
	 if(*(($1)+1)=='^') $$.str=fsav5("$BC(",$2,"[",$1,"])");
	 else $$.str=fsav5("$BI(",$2,"[",$1,"])");
	 $$.val=(reals)1.0; $$.bad=TRUE; $$.typ=0;}
	| select pmops
	{$$.str=fsav4("$A(",$1,")",$2); $$.val=(reals)1.0; $$.bad=TRUE; $$.typ=0;}
	| select eqops sexp %prec PR1
	{$$.str=fsav5("$A(",$1,")",$2,$3.str); $$.val=(reals)1.0; $$.bad=TRUE; $$.typ=0;}
	| pmops select %prec PR1
	{$$.str=fsav4($1,"$A(",$2,")"); $$.val=(reals)1.0; $$.bad=TRUE; $$.typ=0;}
	| select %prec PR1
	{$$.str=$1; $$.val=(reals)1.0; $$.bad=TRUE; $$.typ=0;}
;
eqops:
	  '='
	| MULEQ | DIVEQ | MINEQ | MODEQ | PLUEQ
	| ANDEQ | XOREQ | OREQ | LSEQ | RSEQ
;
pmops:
	  PLUPLU | MINMIN
;
arg_list:
	  arg
	| arg_list ',' arg
	{$$=fsav3($1,",",$3);}
;
arg:
	{sal_lis=(char)0; sal_typ=LIST_ATCON;} arg_val
	{$$=$2;}
;
arg_val:
	  arg_exp {sal_lis&=SIS_RMK;}
	| arg_cast_exp
;
arg_cast_exp:
	'(' type_cast ')' arg_exp
	{$$=conv_cast($2,$4,TRUE);}
	| '(' arg_cast_exp ')'
	{$$=$2;}
;
arg_exp:
	  my_sexp
	| FIDENT
	{$$=fsav2("$@",$1);}
	| sslist
;
select:
	  DOLSEL selnum
	{hassel=TRUE; $$=fsav2($1,$2);}
	| DOLEXC parnum
	{hassel=TRUE; $$=fsav2($1,$2); sal_lop(LIST_ATPAR);}
	| DOLEXCEXC
	{hassel=TRUE; $$=$1; sal_lop(LIST_ATEXC);}
;
parnum:
	 parexp
	{$$=fsav3("@(",$1,")");}
	| '|' parexp
	{$$=fsav3("B(",$2,")");}
;
parexp:
	  e %prec PR1
	{int pp=(int)$1; if(pp==1) sal_lop(LIST_ATONE);
	 sprintf(yacc_buff,"%d",pp); $$=yacsave(yacc_buff);}
;
natnum:
	  LNUMBER
	{haslong=TRUE; sprintf(yacc_buff,"%dL",$1); $$.str=yacsave(yacc_buff);
	 $$.val=(reals)$1; $$.bad=FALSE; $$.typ=1;}
	| INUMBER
	{sprintf(yacc_buff,"%d",$1); $$.str=yacsave(yacc_buff);
	 $$.val=(reals)$1; $$.bad=FALSE; $$.typ=0;}
;
selnum:
	  selrel my_sterm %prec PR1
	{$$=fsav4($1,"(",$2,")");}
	| selrel %prec PR1
	{$$=fsav2($1,"(1)");}
	| my_sterm %prec PR1
	{$$=fsav3("@(",$1,")");}
	| %prec PR1
	{$$=yacsave("@(1)");}
;
selrel:
	  '.' selcoor %prec PR1
	{$$=yacsave($2); *$$|='P';}
	| '.' %prec PR1
	{$$="P";}
	| selcoor %prec PR1
;
selcoor:
	  '@'
	{$$="A";}
	| '|'
	{$$="B";}
	| '?'
	{$$="F";}
	| '!'
	{$$="J";}
	| ATPIP
	{$$="C";}
	| ATQST
	{$$="G";}
	| ATEXC
	{$$="K";}
;
salist:
	  list_sel
	| '[' {hasand=FALSE; sal_lis=(char)0; sal_typ=LIST_ATCON; sal_start();}
	  lelms lssep ']'
	{$$=sal_end($3);
	 if(hasand) {hasand=FALSE; signal_error(ERR_ARY_AND); $$=STR_SARY;} }
	| LSTSTAR
;
sslist:
	  list_sel {tmp_int=0;}
	| '[' {sal_start();} lelms lssep ']'
	{tmp_int=(int)sal_lis; $$=sal_end($3);}
;
loop_statement:
	  CFOR {sal_start();} '(' for_exp ')' loop_list
	{$$=loop_end("$0(",$6,$4);}
	| CWHILE {sal_start();} '(' my_sexp ')' loop_list
	{$$=loop_end("$1(",$6,$4);}
	| CDO {sal_start();} loop_list CWHILE '(' my_sexp ')' lssep
	{$$=loop_end("$2(",$3,$6);}
	| CIF {sal_start();} '(' my_sexp ')' if_list
	{if(*($6)=='e') $$=loop_end("$4(",($6)+1,$4);
	 else $$=loop_end("$3(",$6,$4);}
;
for_exp:
	  frexp ';' frexp ';' frexp
	{$$=fsav5($1,";",$3,";",$5);}
;
frexp:
	  %prec PR1
	{$$="(1)";}
	| my_sexp
	{$$=fsav3("(",$1,")");}
;
if_list:
	  loop_list %prec PR1
	| loop_list CELSE loop_list %prec PR1
	{$$=fsav3("e",$1,$3);}
;
loop_list:
	  loop_apnd loop_ary
	{$$=fsav2($1,$2);}
	| CBREAK
;
loop_apnd:
	  %prec PR1
	{$$="0";}
	| '+' %prec PR1
	{$$="1";}
	| PLUPLU %prec PR1
	{$$="2";}
;
loop_ary:
	  sslist lssep
	| loop_statement
	| {sal_start();} range lssep
	{$$=sal_end($2);}
	| '{' {sal_start();} lelms lssep '}'
	{$$=sal_end($3);}
;
lelms:
	  lelm
	| lelms lssep lelm
	{$$=fsav3($1,",",$3); sal_lis|=SIS_LIS;}
;
lssep:
	  %prec PR1
	| ','
	| ';'
;
lelm:
	  list_sel
	| range
	| loop_statement
	| CBREAK
	| CIDENT dint_list ';'
	{$$=$2;}
;
dint_list:
	  dint_star
	| dint_list ',' dint_star
	{$$=fsav3($1,$2,$3);}
;
dint_star:
	  dint_elm
	| '*' dint_elm
	{$$=$2; *($$+2)|=(char)STAR_ONE;}
	| STARSTAR dint_elm
	{$$=$2; *($$+2)|=(char)STAR_TWO;}
;
dint_elm:
	  dint_id
	{save_id($1); $$=fsav3(cint_str,$1,"(0)");}
	| dint_id '=' {$2=yacsave(cint_str);} my_sexp
	{cint_str=$2; save_id($1); $$=fsav5(cint_str,$1,"(",$4,")");}
;
dint_id:
	  IDENT
	| VIDENT
;
range:
	  my_sexp %prec PR1
	| skips my_sexp %prec PR1
	{$$=fsav2(":",$2);}
	| my_sexp DOTDOT my_sexp %prec PR1
	{$$=fsav3($1,$2,$3); sal_lis|=(SIS_LIS+SIS_RNG);}
;
skips:
	 ':'
	| skips ':'
;
list_sel:
	  LSTOPR selnum ']'
	{if($1.rev) *$2|=0x20; $$=fsav3($1.str,$2,$3); sal_lis|=SIS_LIS;}
;
comm_decls:
	  %prec PR1
	| comm_do
;
comm_do:
	  comm_elm
	| comm_do comm_elm
;
comm_elm:
	  COMMTYPE {tmp_int=(int)$1;} comm_list ';'
;
comm_list:
	  comm_mem
	| comm_list ',' comm_mem
;
comm_mem:
	  wild_name
	{comm_push($1,tmp_int);}
;
net_decl:
	  comm_decls net_body net_vars
;
net_body:
	  '{' {comm_mode=FALSE;} net_decls '}'
;
net_vars:
	| '=' '{' var_decls '}'
;
net_decls:
	  %prec PR1
	| net_list
;
net_list:
	  net_elm
	| net_list net_elm
;
net_elm:
	  obj_decl ';'
	| add_decl ';'
	| ';'
	| error
	{if(resynch(0,"object/additional declaration",";","}")) yyerrok; yyclearin;}
;
obj_decl:
	  net_or_node {chklsbrk=TRUE;} obj_ary {do_obj_spec($1,$3);} obj_elms
;
add_decl:
	{isnewd=TRUE; decl_line=cur_line_num;} add_elm
;
add_elm:
	  add_lp '=' {decl_dollar="~";} conx_list
	| add_lp
;
add_lp:
	{decl_label=(char *)NULL; decl_dollar="$"; decl_line=cur_line_num; isnewd=TRUE;}
	  add_lab_path
	{decl_path=$2; do_con_member(TRUE);}
;
add_lab_path:
	  label add_path
	{$$=$2;}
	| label
	{$$=yacsave(decl_name);}
	| add_path
	| vir_innet
;
net_or_node:
	 NET NETARGS
	{$$=fsav3($1,":",$2);}
	| NODE {isnode=TRUE; yacc_tmp_pt=(($1)+strlen($1)+1);} '(' var_decls ')'
	{isnode=FALSE; $$=fsav4($1,":(",$4,")");}
;
obj_ary:
	{$$=(int)1; chklsbrk=FALSE;}
	| LSBRK {chklsbrk=FALSE;} e ']'
	{$$=(int)$3;}
;
obj_elms:
	| obj_list
;
obj_list:
	  obj_elm
	| obj_list comma obj_elm
;
obj_elm:
	  obj_lp '=' {decl_dollar=decl_name;} conx_list
	| obj_lp
;
obj_lp:
	{decl_label=(char *)NULL; decl_dollar="$"; decl_line=cur_line_num; isnewd=TRUE;}
	  obj_lab_path
	{decl_path=$2; do_con_member(FALSE);}
	| error
	{if(resynch(0,"object member",NULL,"=,;}")) yyerrok; yyclearin;}
;
obj_lab_path:
	  label obj_path
	{$$=$2;}
	| label
	{$$=yacsave(decl_name);}
	| obj_path
	| vir_innet
;
obj_path:
	  bas_path
	{$$=add_name($1);}
	|  tildes in_path
	{$$=do_con_path($1,$2);}
;
bas_path:
	  gen_path
	| dodo_path
	| dol_path
	| dec_path
	{$$=fsav2(decl_dollar,$1);}
;
in_path:
	  flo_path %prec PR1
	| ref_dot_path flo_path %prec PR1
	{$$=fsav2($1,$2);}
	| ref_path %prec PR1
	| abssep flo_path %prec PR1
	{$$=fsav2($1,$2);}
;
flo_path:
	  wild_path
	| dol_path
	| dec_path
	{$$=fsav2(decl_dollar,$1);}
;
dodo_path:
	 DOLDOL LABEL wild_path %prec PR1
	{$$=fsav4($1,$2,"/",$3);}
	| DOLDOL wild_path %prec PR1
	{$$=fsav2("$$global/",$2);}
;
out_path:
	  bas_path
	| tildes sel_path %prec PR1
	{$$=fsav2($1,$2);}
	| tildes %prec PR1
	| vir_outnet salist %prec PR1
	{$$=fsav2($1,$2);}
	| vir_outnet %prec PR1
;
vir_innet:
	 CBIN
	| CPILE
	| CHEAP
;
vir_outnet:
	  CPILE
	| CHEAP
;
tildes:
	  '~'
	| '~' '~'
	{$$="~~";}
;
sel_path:
	  gen_path
	| dec_path
;
dec_path:
	  salist gensep wild_path %prec PR1
	{$$=fsav3($1,$2,$3);}
	| salist %prec PR1
;
dol_path:
	  dol_id gensep wild_path %prec PR1
	{$$=fsav3($1,$2,$3);}
	| dol_id %prec PR1
;
dol_id:
	  '$' salist
	{$$=fsav2(decl_dollar,$2);}
	| '$'
	{$$=yacsave(decl_dollar);}
;
label:
	  '$' ':'
	 {decl_label=decl_id;}
	| LABEL
	{decl_label=$1; }
	| LABELARY salist ':'
	{if(*(($2)+1)==LIST_WILD) decl_label=$1;
	 else decl_label=fsav2($1,$2);}
;
add_path:
	  gen_path
	| dodo_path
	| tildes gen_path
	{$$=fsav2($1,$2);}
;
gen_path:
	  ref_dot_path abs_path %prec PR1
	{$$=fsav2($1,$2);}
	| ref_path %prec PR1
	| wild_path %prec PR1
	| abssep wild_path %prec PR1
	{$$=fsav2($1,$2);}
;
abs_path:
	  ref_path %prec PR1
	| wild_path %prec PR1
;
app_path:
	  dada_path
	| '$' app_abs %prec PR1
	{$$=fsav2($1,$2);}
	| '$' tildes app_abs %prec PR1
	{$$=fsav3($1,$2,$3);}
	| app_typ app_stem %prec PR1
	{$$=fsav2($1,$2);}
;
app_typ:
	  DOLEXC %prec PR1
	{$$="$!";}
	| DOLQUEST %prec PR1
;
app_stem:
	  app_abs %prec PR1
	| tildes app_abs %prec PR1
	{$$=fsav2($1,$2);}
	| dada_path %prec PR1
;
dada_path:
	 DOLDOL LABEL app_member %prec PR1
	{$$=fsav4($1,$2,"/",$3);}
	| DOLDOL app_member %prec PR1
	{$$=fsav2("$$global/",$2);}
;
app_abs:
	  ref_dot_path app_member %prec PR1
	{$$=fsav2($1,$2);}
	| abssep app_member %prec PR1
	{$$=fsav2($1,$2);}
	| app_member %prec PR1
;
app_member:
	  app_member pathsep app_id %prec PR1
	{$$=fsav3($1,"/",$3);}
	| app_id %prec PR1
;
app_id:
	  IDENT salist
	{$$=fsav2($1,$2);}
	| IDENT
;
ref_path:
	  '.' %prec PR1
	| DOTDOT %prec PR1
	{$$=PATHDOT;}
;
ref_dot_path:
	 DOTSEP %prec PR1
	| DOTDOTSEP %prec PR1
	| DOTDOTID %prec PR1
;
wild_path:
	  wild_path gensep path_id %prec PR1
	{$$=fsav3($1,$2,$3);}
	| path_id %prec PR1
;
path_id:
	  wild_id
	| STARSTAR
;
wild_id:
	  wild_name salist %prec PR1
	{$$=fsav2($1,$2);}
	| wild_name %prec PR1
;
wild_name:
	  '*' wild_elms %prec PR1
	{$$=fsav2($1,$2);}
	| '*' %prec PR1
	| first_elm '*' %prec PR1
	{$$=fsav2($1,$2);}
	| first_elm %prec PR1
;
first_elm:
	 '?' wild_elms %prec PR1
	{$$=fsav2($1,$2);}
	| '?' %prec PR1
	| IDENT wild_elms %prec PR1
	{$$=fsav2($1,$2);}
	| IDENT %prec PR1
;
wild_elms:
	  wild_elm wild_elms %prec PR1
	{$$=fsav2($1,$2);}
	| wild_elm %prec PR1
;
wild_elm:
	  '?'
	| IDENT
	| INUMBER
	{sprintf(yacc_buff,"%d",$1); $$=yacsave(yacc_buff);}
;
conx_list:
	  conx_elm %prec PR1
	| conx_elm conx_list %prec PR1
;
conx_elm:
	  outvar_decl %prec PR1
	{syn_decl_do((int)'@',$1);}
	|{optok=(int)'@';} cop_list outvar_decl %prec PR1
	{syn_decl_do(optok,$3);}
	| error
	{if(resynch(0,"connection declaration",NULL,"+-<>^!,;}")) yyerrok; yyclearin;}
;
outvar_decl:
	  out_decl str_var
	{$$=fsav3($2,"\n",$1);}
	| out_decl
;
out_decl:
	  '(' net_or_node ')' out_path
	{$$=fsav4($1,do_aux_spec($2),$3,$4);}
	| out_path
;
cop_list:
	  cop cop_list
	| cop
;
cop:
	  one_cop
	| COPBRK one_cop
	{optok|=0x100;}
;
one_cop:
	  '+'
	{optok|=(int)'@';}
	| '-'
	{optok|=(int)'A';}
	| PLUPLU
	{optok|=(int)'B';}
	| MINMIN
	{optok|=(int)'C';}
	| '<'
	{optok|=(int)'H';}
	| '>'
	{optok|=(int)'P';}
	| '^'
	{optok|=(int)'p';}
	| '!'
	{optok|=(int)'D';}
;
var_decls:
	  var_decl
	| var_decls semi var_decl
	{if(isnode)
	   {if($1 && $3) $$=fsav3($1,",",$3); else if($1) $$=$1; else $$=$3;}
	 else $$=(char *)NULL;
	}
;
var_decl:
	{var_mode=TRUE;} var_name
	{hassel=FALSE; hasand=FALSE; haslong=FALSE; decl_line=cur_line_num;
	 sal_lis=(char)0; sal_typ=LIST_ATCON;} var_val
	{char *yacc_apt;
	 if($4)
	   {sprintf(yacc_buff,":%d:\0",decl_line); yacc_apt=yacsave(yacc_buff);
	    if(isnode)
	      {if($2) $$=fsav3($2,yacc_apt,$4);
	       else {signal_error(WRN_OBJ_ARG,($4)+1); $$=(char *)NULL;}
	      }
	    else
	      {$$=(char *)NULL; fcat3(yacc_buff,$2,yacc_apt,$4);
	       yacc_apt=strsave(yacc_buff);
	       append_list_name(&vars_list,yacc_apt,&vars_num,&max_vars,inc_vars);
	      }
	   }
	 else $$=(char *)NULL;
	}
;
var_name:
	  %prec PR1
	{var_mode=FALSE;
	 if(isnode)
	   {if(*yacc_tmp_pt)
	      {$$=yacsave(yacc_tmp_pt); yacc_tmp_pt+=strlen(yacc_tmp_pt)+1;}
	    else /* arg must have a name since none found in object parms */
	      $$=(char *)NULL;
	   }
	 else
	   {sprintf(yacc_buff,"#D%02d\0",tmp_var_num++); $$=yacsave(yacc_buff);}
	}
	| '=' var_path
	{$$=$2;}
;
var_path:
	  gen_path '='
	| error
	{if(resynch(0,"var path","=",",}")) yyerrok; yyclearin; $$="?";}
;
str_decls:
	  str_decl
	| str_decls ',' str_decl
;
str_decl:
	  stelm
	{if($1)
	 append_list_name(&strs_list,strsave($1),&strs_num,&max_strs,inc_strs);}
;
stelist:
	  stelm
	| stelist ',' stelm
	{if($1 && $3) $$=fsav3($1,",",$3); else if($1) $$=$1; else $$=$3;}
;
stelm:
	{var_mode=TRUE;} str_name
	{decl_line=cur_line_num; sal_lis=(char)0; sal_typ=LIST_ATCON;} var_val
	{if($4)
	   {char *apt; sprintf(yacc_buff,":%d:\0",decl_line); apt=yacsave(yacc_buff);
	    $$=fsav3($2,apt,$4);}
	 else $$=(char *)NULL;}
;
str_name:
	  %prec PR1
	{var_mode=FALSE; hassel=FALSE; haslong=FALSE; hasand=FALSE;
	 sprintf(yacc_buff,"#D%02d\0",tmp_var_num++); $$=yacsave(yacc_buff);}
	| '=' str_mem
	{hassel=FALSE; haslong=FALSE; hasand=FALSE; $$=$2;}
;
str_mem:
	  gen_path '='
	| error
	{if(resynch(0,"struc id","=",",}")) yyerrok; yyclearin; $$="?";}
;
var_val:
	  %prec PR1
	{$$=(char *)NULL;}
	| var_exp
	| var_cast_exp
	| error
	{if(resynch(0,"var exp",NULL,",}")) yyerrok; yyclearin; $$="B?";}
;
var_cast_exp:
	'(' type_cast ')' var_exp
	{$$=conv_cast($2,$4,FALSE);}
	| '(' var_cast_exp ')'
	{$$=$2;}
;
var_exp:
	 my_sexp
	{if(hasand || haslong)
	   {if(hassel) signal_error(WRN_SEL_AND);
	    hasand=FALSE; haslong=FALSE; $$=fsav2("B",$1);}
	 else
	   $$=fsav2("A",$1);
	 hassel=FALSE;
	}
	| IDENT
	{$$=fsav2("B",$1);}
	| FIDENT
	{$$=fsav2("B",$1);}
	| sslist
	{$$=fsav2("A",$1);}
	| str_var
;
str_var:
	 '{' {push_vare(&tmp_var_num);} stelist '}'
	{pop_vare(&tmp_var_num); $$=fsav3("{",$3,"}");
	 if(!$3||!*($3)) signal_error(WRN_NUL_STRUC);}
;
node_parms:
	 %prec PR1
	| node_list
;
node_list:
	  node_elm
	| node_list ',' node_elm
;
node_elm:
	 node_prefix wild_path
	{yacc_tmp_pt=fcat2(yacc_tmp_pt,$1,$2); ++yacc_tmp_pt; *yacc_tmp_pt='\0';}
	| wild_path
	{yacc_tmp_pt=fcat(yacc_tmp_pt,$1); ++yacc_tmp_pt; *yacc_tmp_pt='\0';}
	| error
	{if(resynch(0,"node parm",NULL,",)")) yyerrok; yyclearin;}
;
node_prefix:
	 '#'
	| '%'
;
var_symbol:
	  IDENT
	| var_symbol '.' IDENT
	{$$=fsav3($1,$2,$3);}
;
type_cast:
	  type_list
	{$$=type_cast($1,(char *)NULL);}
	| type_list star_list
	{$$=type_cast($1,$2);}
;
type_list:
	  type_id
	| type_id type_id
	{$$=fsav3($1," ",$2);}
;
type_id:
	  IDENT
	| CIDENT
;
star_list:
	  star_elm
	| star_list star_elm
	{$$=fsav2($1,$2);}
;
star_elm:
	  '*'
	| STARSTAR
	| '[' ']'
	{$$="[]";}
	| '(' ')'
	{$$="()";}
	| '(' star_list ')'
	{$$=fsav3($1,$2,$3);}
;
gensep:
	  pathsep
	| '\\' '\\'
	{$$=PATHPAR;}
	| '/' '/'
	{$$=PATHPAR;}
	| '|'
	| OROR
	{$$="|";}
;
pathsep:
	  '.' %prec PR1
	| abssep
;
abssep:
	 '/' %prec PR1
	| '\\' %prec PR1
	{$$="/";}
;
comma:
	 ','
	| error
	{if(resynch(0,"',' expected",",",";)}")) yyerrok; yyclearin;}
;
semi:
	 ','
	| ';'
	| error
	{if(resynch(0,"',' expected",",","}")) yyerrok; yyclearin;}
;
