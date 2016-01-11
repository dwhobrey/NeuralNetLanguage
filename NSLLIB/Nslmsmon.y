%{
/* Name: nslmsmon.y	Type: yacc file	Date: 18 May 89
*  NeSeL Network Specification Language
*/
#include "nslms.h"
#include "nslmsmon.h"
#include "nslwsio.h"
extern int mon_wnd;
%}
%start START
%union
{int ival;
 ni_elm nival;
 glo_elm gval;
 NSLNSLfval fval;
 char *sval;
 char (*fpval)();
}
%type <fval> e term opt_exp opt_T_exp variable npath var_exp func_exp
%type <ival> stars castle
%type <sval> id_name

%right PR1
%left <sval> ',' ';' CEOF
%right <fval> NUMBER
%right <sval> IDENT LABEL
%right <nival> FUNKY
%right <ival> UFFKY UFFKY_ONE
%right <ival> CBREAK CCALL CCALL_ONE CCONT CCLS CCD 
%right <ival> CDELETE CDO CDO_ONE CDIR CDISABLE CENABLE CELSE CEXIT CDRIVER
%right <ival> CFOR CFOR_ONE CFOR_TWO
%right <ival> CGO CHELP CDEBUG CIF CLOAD CLOG CLOGCMD CNEW
%right <ival> CPRINT CRETURN CCLOSE COPEN CSEARCH CLINE CDUMP
%right <ival> CSAVE CSHELL CSHOW CSTEP CSTOP CWATCH CWHILE
%right <ival> CTYPE
%right <fpval> S_EXP S_CMDS CUSER
%right <gval> VAR
%right <sval> DQSTR CNET
%right <sval> '='
%right <ival> EQOP
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
%right <ival> PLUPLU MINMIN DOLDOL
%right <sval> UMINUS '!' '~' '$' '@' ')' ']' '}' DOTDOT
%left <sval> '(' '[' '{' '.'
%%

START:
	  S_CMDS cmds {cmd_exit=2; YYACCEPT;}
	| S_EXP e CEOF {cmd_exit=2; tmp_val=tx_to_d($2); yyclearin; YYACCEPT;}
	| %prec PR1 {cmd_exit=2; tmp_val=(double)0.0; yyclearin; YYACCEPT;}
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
		$$=d_to_t((int)TYPE_LONG,tx_to_d($1)-tx_to_d($3));
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
;
term:
	  '-' term %prec UMINUS
	{$$=d_to_t($2.typ,-(tx_to_d($2)));}
	| '!' term
	{$$=d_to_t($2.typ,(double)(!(long)tx_to_d($2)));}
	| '~' term
	{$$=d_to_t($2.typ,(double)(~(long)tx_to_d($2)));}
	| castle term
	{$$=d_to_t($1,tx_to_d($2));}
	| '(' e ')'
	{$$=$2;}
	| NUMBER
	{$$=$1;}
	| DQSTR
	{$$.val.pp=(char *)$1; $$.typ=(STAR_ONE+TYPE_CHAR);}
	| '&' VAR %prec PR1
	{$$=$2.val; $$.typ=TYPE_PT+STAR_ONE;}
	| func_exp
	| var_exp
;
func_exp:
	  FUNKY args
	{user_func($1.net,$1.indx); $$=nsl_func_rr;}
	| UFFKY args
	{push_state(UFFKY); push_uff($1); push_io();
	 set_io(&fun_list[($1)-1].iop); inc_file(io_fnum); yyclearin;}
	  opt_semi cmds UFFKY_ONE
	{restore_exp(); pop_state(); pop_uff(); dec_file(io_fnum);
	 pop_io(); yyclearin; $$=nsl_func_rr;}
;
var_exp:
	  variable %prec PR1
	{$$=var_selector($1,zerval,(int)VAR,TRUE);}
	| variable PLUPLU
	{$$=var_selector($1,zerval,(int)PLUPLU,TRUE);}
	| variable MINMIN
	{$$=var_selector($1,zerval,(int)MINMIN,TRUE);}
	| variable EQOP e
	{$$=var_selector($1,$3,$2,FALSE);}
	| variable '=' e
	{$$=var_selector($1,$3,(int)'=',FALSE);}
	| PLUPLU variable
	{$$=var_selector($2,zerval,(int)PLUPLU,FALSE);}
	| MINMIN variable
	{$$=var_selector($2,zerval,(int)MINMIN,FALSE);}
;
variable:
	  VAR
	{$$=$1.val;}
	| '$' {tmp_pt=tmp_buff; *tmp_pt='\0'; tmp_net=mon_net;} npath
	{$$=$3;}
	| DOLDOL
	{$$.typ=nsl_func_rr.typ; $$.val.pp=(char *)&nsl_func_rr.val;}
;
npath:
	  e path_sep path_list
	{long hstval,nd=(long)tx_to_d($1); if(nd<1) nd=0L;
	 if(is_set(mon_net))
		{hstval=nsl_num_adrs(mon_net,nd,tmp_buff,&$$.val.pp,NULL);
		if(nd<1) {$$.typ=TYPE_LONG; $$.val.pp=(char *)&hstval;}
		else $$.typ=(int)hstval;
		}
	 else $$=zerval;
	}
	| gen_path
	{long timval; NSLNSLnet *npt;
	 if(strncmp(tmp_buff,"time.",5)==0)
		{npt=nsl_get_p(tmp_buff+5); if(npt) timval=npt->time; else timval=0L;
		$$.typ=TYPE_LONG; $$.val.pp=(char *)&timval;}
	 else if(is_set(tmp_net))
		$$.typ=(int)nsl_num_adrs(tmp_net,(long)(-1),
			tmp_buff,&$$.val.pp,NULL);
	 else $$=zerval;
	}
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
	  e {nsl_func_rr=$1; push_exp();}
;
castle:
	'(' CTYPE stars ')' {$$=($2|$3);}
;
id_list:
	  id_list ',' id_elm
	| id_elm
;
id_elm:
	  stars id_name
	{parm_args[0]=(char)($1|parm_type); parm_name=strsave($2);
	 parm_pos=(long)-1; parm_num=0; parm_val=zerval;
	}
	  opt_eqfunc
	{do_id_func();}
;
id_name:
	  IDENT
	| VAR
	{$$=$1.name;}
;
opt_eqfunc:
	 %prec PR1
	| '=' e
	{parm_val=$2;}
	| '(' opt_parms ')' opt_lbrace
;
opt_lbrace:
	 %prec PR1
	| '{'
	{parm_ofs=get_io_ofs()-1; parm_pos=io_file_pos; parm_const=cmd_const;
	 parm_pat=cmd_pat; parm_exe=cmd_exe; parm_brk=cmd_brk; parm_exit=cmd_exit;
	 parm_entry=yacc_entry; parm_yacc=cmd_yacc;
	 parm_iof=io_fnum; inc_file(parm_iof); skip_to_chr('}',FALSE);
	 yyclearin; yacc_entry=';';}
;
opt_parms:
	 %prec PR1
	| parm_list
;
parm_list:
	  parm_elm
	| parm_list ';' parm_elm
;
parm_elm:
	 CTYPE {parm_type=$1;} parm_ids
;
parm_ids:
	  parm_id
	| parm_ids ',' parm_id
;
parm_id:
	 stars IDENT
	{enlarge_array(&parm_list,&parm_max,&parm_num,1,8,sizeof(char *));
	 parm_list[parm_num-1]=strsave($2);
	 parm_args[parm_num]=(char)($1|parm_type);
	}
stars:
	{$$=0;/*nothing*/}
	| '*'
	{$$=STAR_ONE;}
	| '*' '*'
	{$$=STAR_TWO;}
;
opt_T_exp:
	{$$=trueval;}
	| e
;
opt_args:
	%prec PR1
	| args
;
opt_exp:
	{$$=zerval;}
	| '(' e ')'
	{$$=$2;}
;
net_path:
	  DOTDOT {tmp_pt=fcat(tmp_pt,$1);} dot_path
	| path_list
	| dot_path
;
dot_path:
	%prec PR1
	| path_sep path_stem
	| CNET {tmp_pt=fcat(tmp_pt,$1);} path_sep path_list
;
gen_path:
	  path_list
	| CNET {tmp_pt=fcat(tmp_pt,$1);} path_sep path_list
;
path_stem:
	%prec PR1
	| path_list
;
path_list:
	  path_elm path_sep path_list %prec PR1
	| path_elm %prec PR1
;
path_elm:
	  path_mem opt_ary
	| e ':' e
	{sprintf(tmp_pt,"%d:%d\0",(int)tx_to_d($1),(int)tx_to_d($3));
	 tmp_pt+=strlen(tmp_pt);
	}
;
path_mem:
	  IDENT
	{tmp_pt=fcat(tmp_pt,$1);}
	| LABEL {tmp_pt=fcat(tmp_pt,$1);} e
	{sprintf(tmp_pt,":%d\0",(int)tx_to_d($3)); tmp_pt+=strlen(tmp_pt);}
;
opt_ary:
 %prec PR1
	| '[' {cmd_pat=FALSE;} e ']'
	{sprintf(tmp_pt,"[%d]\0",(int)tx_to_d($3)); cmd_pat=TRUE;
	 tmp_pt+=strlen(tmp_pt);
	}
	| '[' '*' ']'
	{tmp_pt=fcat(tmp_pt,"[*]\0");}
;
e_or_n:
	  e {cmd_id=NULL; cmd_exp=(int)tx_to_d($1);}
	| IDENT {cmd_id=$1;}
path_sep:
	  '.'
	{do_path_sep: cmd_pat=TRUE; tmp_pt=fcat(tmp_pt,$1);}
	| '/'
	{goto do_path_sep;}
	| '\\'
	{goto do_path_sep;}
;
cmds:
	  cmds cmd
	| cmd
;
cmd:
	  stat_semi opt_sep_list
	| stat opt_sep_list
;
opt_sep_list:
	%prec PR1
	| sep_list
;
opt_semi:
	%prec PR1
	| ';' %prec PR1
;
sep_list:
	  sep_list sep_elm
	| sep_elm
;
sep_elm:
	  ';'
	{tmp_chr=';'; do_send:
	 switch(tmp_cmd=last_state())
		{case '{': break;
		case CFOR_ONE: do_for_one: pop_state();
		  go_io(0); yacc_entry=CFOR_ONE; yyclearin; break;
		case CWHILE: do_while: go_io(0); yacc_entry=CWHILE; yyclearin; break;
		case CIF: pop_state(); break;
		case UFFKY: if(tmp_chr=='}')
			{nsl_func_rr=zerval; yacc_entry=UFFKY_ONE; yyclearin; break;}
		default: ;
		}
	}
;
stat:
	  '{' {push_state('{');}
	| '}' {pop_state(); tmp_chr='}'; goto do_send;}
	| CDO
	{push_io(); push_state(CDO_ONE);}
	| CWHILE {push_io();} '(' e ')'
	{tmp_val=tx_to_d($4);
	 if(last_state()==CDO_ONE)
		{del_io(1);
		if(tmp_val!=(double)0.0) {go_io(0); yyclearin;}
		else {del_io(1); pop_state();}
		}
	 else
		{if(last_state()!=CWHILE) push_state(CWHILE); else del_io(1);
		if(tmp_val==(double)0.0)
		  {do_while_one: del_io(1); pop_state(); skip_statement(FALSE); yyclearin;}
		}
	}
	| CFOR '(' opt_T_exp ';' {push_io();} opt_T_exp ';'
	{push_io(); tmp_val=tx_to_d($6); do_for: skip_to_chr(')',FALSE);
	 if(tmp_val==(double)0.0) {del_io(2); skip_statement(FALSE); yyclearin;}
	 else push_state(CFOR_ONE);
	}
	| CFOR_ONE opt_T_exp ')'
	{go_io(1); yacc_entry=CFOR_TWO; yyclearin;}
	| CFOR_TWO opt_T_exp ';' {tmp_val=tx_to_d($2); goto do_for;}
	| CIF '(' e ')'
	{if(tx_to_d($3)==(double)0.0) {skip_statement(FALSE); yyclearin;}
	 else push_state(CIF);
	}
	| CELSE
	{if(tmp_cmd==CIF) {skip_statement(FALSE); yyclearin;}}
	| CEOF
	{tmp_fval=zerval; do_eof: yyclearin;
	 switch(tmp_cmd=pop_to_loop(TRUE))
		{case CCALL: yacc_entry=CCALL_ONE; break;
		case UFFKY: yacc_entry=UFFKY_ONE; break;
		default:
		  if(cmd_brk) cmd_exit=2; /* User Intrpt, reset to stdio */
		  else {init_mon(); cmd_exit=1;}/* runaway error: reset stacks */
		  YYACCEPT;
		}
	 nsl_func_rr=tmp_fval;
	}
;
dumpy:
	  net_path
	| CCLOSE {fcat(tmp_buff,"close");}
;
stat_semi:
	  CBREAK
	{tmp_cmd=pop_to_loop(FALSE);
	 if(tmp_cmd==CFOR_ONE)
		{pop_state(); go_io(0); tmp_val=(double)0.0; goto do_for;}
	 else if(tmp_cmd==CWHILE) {go_io(0); goto do_while_one;}
	}
	| CCONT
	{tmp_cmd=pop_to_loop(FALSE);/*look for loop cmd, or func end & clr to if found */
	 if(tmp_cmd==CFOR_ONE) goto do_for_one;
	 else if(tmp_cmd==CWHILE) goto do_while;
	}
	| CRETURN opt_exp {tmp_fval=$2; goto do_eof;}
	| CCALL IDENT
	{sprintf(tmp_buff,"%s.nat\0",$2);
	 push_farg();
	 get_args(); /* reads in rest of line */
	 save_exp(); push_state(CCALL); push_uff((int)0); push_io();
	 if(add_file(tmp_buff,TRUE)<1)
		{mprintf("unable to open `%s'\n",tmp_buff); goto do_call_one;}
	 yyclearin;
	}
	  opt_semi cmds CCALL_ONE
	{do_call_one: pop_farg();
	 restore_exp(); pop_state(); pop_uff(); dec_file(io_fnum); pop_io();
	 yyclearin;
	}
	| CSTEP {save_exp(); par_str="EEE0";} opt_args {do_step(TRUE);}
	| CGO {save_exp(); par_str="0";} opt_args {do_step(FALSE);}
	| CHELP {do_help();}
	| CDEBUG {do_debug();}
	| CCLS {w_clear(mon_wnd);}
	| CDIR {tmp_pt=tmp_buff; *tmp_pt='\0';} net_path {do_dir(tmp_buff);}
	| CCD {tmp_pt=tmp_buff; *tmp_pt='\0';} net_path  {do_cd(tmp_buff);}
	| CNET {do_cnet($1);}
	| CNEW {init_mon(); cmd_exit=1; YYACCEPT;}
	| CEXIT {cmd_exit=0; YYACCEPT;}
	| CLOG IDENT {do_log($2,"nog",&logfp);}
	| CLOGCMD IDENT {do_log($2,"nat",&cmdfp);}
	| CPRINT e {mprintf("%g\n",tx_to_d($2));}
	| CLINE e {do_fsgo((long)(tx_to_d($2)));}
	| CSEARCH {do_fssearch();}
	| CCLOSE {do_fsclose();}
	| COPEN {tmp_pt=tmp_buff; *tmp_pt='\0';} net_path {do_fsopen(tmp_buff);}
	| CDUMP IDENT {tmp_pt=tmp_buff;*tmp_pt='\0';} dumpy {do_dump($2,tmp_buff);}
	| CUSER {save_exp(); par_str="0";} opt_args {do_gen_func($1);}
	| CDRIVER {save_exp(); par_str="0";} opt_args {do_driver();}
	| CLOAD IDENT IDENT {do_load($2,$3);}
	| CSAVE IDENT IDENT {do_save($2,$3);}
	| CDELETE e_or_n
	{if(cmd_id) do_delete(cmd_id); else do_was(cmd_exp,'k');}
	| CDISABLE e_or_n
	{if(cmd_id) do_enadis(cmd_id,FALSE); else do_was(cmd_exp,'d');}
	| CENABLE e_or_n
	{if(cmd_id) do_enadis(cmd_id,TRUE); else do_was(cmd_exp,'e');}
	| CSHOW IDENT {do_show($2);}
	| CSHELL /* string */ {do_shell(tmp_buff+TMP_OFS);}
	| CSTOP /* string */ {do_was(0,'s');}
	| CWATCH /* string */{do_was(0,'w');}
	| CTYPE {parm_type=$1;} id_list
	| var_exp
	| func_exp
;
