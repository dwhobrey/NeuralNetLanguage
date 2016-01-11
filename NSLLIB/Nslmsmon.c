/* Name: nslmsmon.c  Type: yacc routines file  Date: 30 March 89
*  NeSeL Network Specification Language
*/
#include "nslms.h"
#include "nslwsio.h"
#include "nslmsmon.h"
#include "ytab.h"

#include "excpt.h"

extern int var_adrs();
extern int NSLCtrl_C();
extern void w_sleep(int milli);

/* internal token values for assignment ops */
#define MULEQ 1
#define DIVEQ 2
#define MODEQ 3
#define PLUEQ 4
#define MINEQ 5
#define ANDEQ 6
#define XOREQ 7
#define OREQ 8
#define LSEQ 9
#define RSEQ 10

#define MAX_IO_STACK 40
#define MAX_PARMS 20
#define MAX_STR_CIR 30
#define MAX_TYPES 21
#define MAX_BASE 8
#define DEF_WY 10 /*default mon watch y dim*/
#define DEF_WX 40 /*default mon watch x dim*/
#define DEF_MY 20 /*default mon wnd y dim*/
#define DEF_MX 80 /*default mon wnd x dim*/

extern BOOL 
	cmd_order=TRUE,is_was_upd=FALSE,
	cmd_pat=FALSE,cmd_exe=FALSE,cmd_brk=FALSE,cmd_yacc=FALSE,
	parm_pat=FALSE,parm_exe=FALSE,parm_brk=FALSE,parm_yacc=FALSE;

extern int 
  str_cir_max=MAX_STR_CIR;

extern int 
  *par_ofs=NULL,
  *state_stk=NULL,
  cmd_exit=0,
  parm_exit=0,
  io_fnum=0,
  parm_iof=0,
  nsl_sig_int=0,
  mon_wnd,mon_wat=0,mon_fs=0,mon_fx=DEF_MX,mon_fy=DEF_MY,
  mon_wy=DEF_WY,mon_wx=DEF_WX,mon_my=DEF_MY,mon_mx=DEF_MX,
  mon_dump=TRUE;

extern int 
  str_cir_pt=0,
  cmd_exp=0,
  yacc_entry=0,
  tmp_cmd=0, 
  parm_entry=0,
  parm_type=0,
  parm_ofs=0, 
  parm_num=0,parm_max=0;

extern long 
	fs_line=0,fs_last=0,
	io_file_pos=0,
	io_tmp_pos=0,
	io_tmp_end=0,
	parm_pos=0;

extern double tmp_val=(double)0.0; 

extern NSLNSLfval 
  zerval={TYPE_INT,0},
  trueval={TYPE_INT,1},
  tmp_fval={TYPE_INT,0},
  parm_val={TYPE_INT,0}, 
  dumval={TYPE_INT,0}, 
  nsl_func_rr={TYPE_INT,0}; 

extern NSLNSLnet *tmp_net=NULL,*mon_net=NULL;
extern fly_elm *fly_list=NULL;
extern exp_elm *exp_stk=NULL;
extern uff_elm *uff_stk=NULL;
extern farg_elm *farg_stk=NULL;
extern fsk_elm *fsk_stk=NULL;
extern glo_elm *glo_list=NULL;
extern fun_elm *fun_list=NULL;
extern user_elm *user_list=NULL;

extern FILE 
	*infp=NULL,*logfp=NULL,*tmpfp=NULL,
	*cmdfp=NULL,*fsfp=NULL;

extern char 
  **was_list=NULL,
  tmp_chr=0,
  *tmp_pt=NULL, 
  *io_buff_pt=NULL, 
  *cmd_id=NULL,
  *cmd_const=NULL,
  *cmd_entry=NULL,
  *par_stk=NULL,*par_str=NULL,
  **parm_list=NULL, 
  *parm_name=NULL, 
  *parm_const=NULL,
  **str_cir_bs=NULL, 
  *yacc_io=NULL, 
  *yacc_lastok=NULL,
  *yacc_ioerr=NULL; 

extern char 
	parm_args[MAX_PARMS]={0},
	io_buff[buffsize]={0}, /* io buffer */
	tmp_buff[buffsize]={0}; /* parsing buffer */

char io_last_chr;
char dir_buff[buffsize];
char cur_ident[namesize];
char out_buff[buffsize];
char fs_buff[buffsize];
char fs_text[buffsize];

char *err_list[]={"ok","ok",
"array size error","bad file",
"access to file went wrong","ran out of memory space",
"declaration error","access to node object block went wrong",
"version number error","nodenum not in index",
"monitor struct info error","conx map reconstruct error",
"access to var failed","monitor parsing err",
"no network defined","name in use already"};
int err_num=sizeof(err_list)/sizeof(char *);
cmd_elm cmd_list[]={
{"break",CBREAK,TRUE},{"call",CCALL,TRUE},{"cd",CCD,TRUE},{"close",CCLOSE,TRUE},
{"cls",CCLS,TRUE},{"continue",CCONT,TRUE},{"debug",CDEBUG,TRUE},{"delete",CDELETE,TRUE},
{"dir",CDIR,TRUE},{"disable",CDISABLE,TRUE},{"do",CDO,FALSE},{"driver",CDRIVER,TRUE},
{"dump",CDUMP,TRUE},{"else",CELSE,FALSE},{"enable",CENABLE,TRUE},{"exit",CEXIT,TRUE},
{"for",CFOR,FALSE},{"go",CGO,TRUE},{"help",CHELP,TRUE},
{"if",CIF,FALSE},{"load",CLOAD,TRUE},
{"log",CLOG,TRUE},{"logcmd",CLOGCMD,TRUE},{"new",CNEW,TRUE},
{"print",CPRINT,TRUE},{"quit",CEXIT,TRUE},
{"return",CRETURN,TRUE},
{"save",CSAVE,TRUE},{"shell",CSHELL,TRUE},
{"show",CSHOW,TRUE},{"step",CSTEP,TRUE},{"stop",CSTOP,TRUE},
{"view",COPEN,TRUE},
{"watch",CWATCH,TRUE},{"while",CWHILE,FALSE}
};
int cmd_num=sizeof(cmd_list)/sizeof(cmd_elm);
char *base_list[]=
  {"char","double","float","int","long","short","unsigned","void"};
char *base_types[]={"?",
	"char","double","float","int","long",
	"void","node","struct","union","short",
	"unsigned char","unsigned int","unsigned long","unsigned short",
	"pointer","net","array","string","connection","connection"};

char *cmd_help[]={
"C functions and variables declared & used as normal, no arrays",
"C basic types only (char, int, long, float, double, *)",
"Terminate all ambiguous commands with <;>",
"<$> [NODEHANDLE\\]VARPATH[=EXP] :access node variables",
"    if NODEHANDLE is zero returns node handle for VARPATH",
"<$time.NAME> :returns age of net NAME",
"<:EXP> :goto line in view file",
"</STR> :search for pattern in view file",
"<]>,<[>,< > :page view file up or down, or by line",
"break; continue; return [(ARG)] :C flow control",
"call NAME [ARGS] :execute commands in batch file NAME.nat",
"cd,dir, NAME: :change or display net directory, or select net",
"delete/disable/enable NUM|NAME|all :change break-point,watch or net status",
"driver(\"{null|setup|init|close|redraw|pattern|pre|post|user}\"[,ARGS])",
"  :issue command to net io driver",
"dump [WID|WID FILENAME|WID close|auto|manual] :dumps windows to file",
"exit; quit :leave monitor",
"for([EXP];[EXP];[EXP]); do; while[(EXP)] :C flow control",
"go [(args)]; step [(NUM [,RATE [,DELAY [,ARGS]]])]",
"  :cycle networks NUM times at RATE with intercycle millisecond DELAY",
"if(EXP); else :C conditionals",
"load/save NAME FILENAME :loads or saves network from/to `FILENAME.nso'",
"log/logcmd NAME :log i/o `.nog', or cmds `.nat', NAME=`off' turn off",
"new :restarts monitor",
"print EXP; ?EXP :evaluate & display, or use printf() like normal",
"shell [ARGS] :issue command to shell",
"show [all|functions|global|local|nets|stop|system|variables|watch|Wnd]",
"stop/watch ARGS :set break-points or watch",
"view FILEPATH, close :view text file",
(char *)NULL
};

void mputs(char *str)
{ if(logfp) fputs(str,logfp);
  w_puts(mon_wnd,str);
}

int mprintf(char *fmt,...)
{ va_list args; va_start(args,fmt);
  vsprintf(out_buff,fmt,args);
  mputs(out_buff);
  va_end(args);
  return(strlen(out_buff));
}
int mvprintf(va_list args)
{ char *fmt=va_arg(args,char *);
  vsprintf(out_buff,fmt,args);
  mputs(out_buff);
  return(strlen(out_buff));
}
void merr(char *s)
{ int nn=(int)nsl_err; if(nn<0) nn=(-nn);
  mprintf("%s:",s);
  if(nn>=err_num) mprintf("err num %d\n",nn);
  mprintf("%s\n",err_list[nn]);
}
void reset_io(fp,nn) FILE *fp; int nn;
{infp=fp; io_fnum=nn; io_buff_pt=io_buff; io_file_pos=0;
 *io_buff='\0';
 rewind(fp);
}
void zap_file(nn) int nn;
{ fly_elm *apt; FILE *fp;
  apt=lst_head(fly_list,fly_elm)+nn;
  apt->users=0;
  if(apt->name) {lfree(apt->name); apt->name=NULL;}
  if(fp=apt->fp)
	{if(fp!=stdin) fclose(fp);
	 if(fp==tmpfp) tmpfp=NULL;
	 if(fp==logfp) logfp=NULL;
	 if(fp==cmdfp) cmdfp=NULL;
	 apt->fp=NULL;
	}
}
int new_file(fp,name) FILE *fp; char *name;
{ fly_elm *apt; int num;
  if(lst_ok(fly_list))
	{num=lst_num(fly_list); apt=lst_tail(fly_list,fly_elm);
	 while(num--) {if(!apt->name) goto byebye; --apt;}
	}
  fly_list=lmore(fly_list,1,TYPE_STRUCT,fly_elm);
  num=lst_num(fly_list)-1;
  apt=lst_head(fly_list,fly_elm)+num;
  byebye:
  apt->fp=fp; apt->name=strsave(name); apt->users=1;
  reset_io(fp,num);
  return(num);
}
void dec_file(nn) int nn;
{ fly_elm *apt;
  if(fly_list)
	{apt=lst_head(fly_list,fly_elm)+nn;
	 if(apt->users>1) --apt->users; else zap_file(nn);
	}
}
void inc_file(nn) int nn;
{ fly_elm *apt=lst_head(fly_list,fly_elm)+nn;
  ++apt->users;
}
/* add a file to access list, open if necessary, set infp & return fnum */
int add_file(name,isrdonly) char *name; BOOL isrdonly;
{ char *rwa; fly_elm *apt; int nn=0,num;
  if(fly_list) {num=lst_num(fly_list); apt=lst_head(fly_list,fly_elm);}
  else {num=0; apt=NULL;}
  if(name)
	{while(nn<num)
		{if(apt->name&&strcmp(name,apt->name)==0)
		  {++apt->users;
			if(apt->fp) {reset_io(apt->fp,nn); return(nn);}
			break;
		  }
		++apt; ++nn;
		}
	 if(isrdonly) rwa="r+"; else rwa="w+";
	 if(!(infp=fopen(name,rwa))) {reset_io(stdin,0); return(0);}
	}
  else {name="stdin"; infp=stdin;}
  if(nn>=num) return(new_file(infp,name));
  reset_io(infp,nn);
  apt->fp=infp;
  return(nn);
}
BOOL get_line(istell) BOOL istell;
{ loop:
  if(infp==stdin) io_file_pos=0;
  else {if(istell) io_file_pos=ftell(infp); fmoveto(infp,io_file_pos);}
  io_buff_pt=io_buff; *io_buff='\0'; io_last_chr='\0';
  if(cmd_entry)
	{fcat(fcat(io_buff,cmd_entry),";\n"); cmd_entry=NULL; goto shove;}
  else
	{if(infp==tmpfp&&io_file_pos==io_tmp_end)
		{infp=stdin; io_fnum=0; io_file_pos=0;}
	 if(infp==stdin)
		{if(mon_net) mputs(mon_net->ins_name); else mputs("?");
		mputs(">");
		w_gets(mon_wnd,io_buff); if(!*io_buff) return(FALSE);
		switch(*io_buff)
		  {case '\n': w_clear_line(mon_wnd);
			 w_getch(mon_wnd); goto loop;
			case '/': if(io_buff[1]=='\n')
			 {w_clear_line(mon_wnd); do_fssearch(); goto loop;} break;
			case ' ': if(io_buff[1]=='\n')
			 {w_clear_line(mon_wnd); if(fs_get()) do_fsdsp(); goto loop;} break;
			case '[': case ']': if(io_buff[1]=='\n'){
				w_clear_line(mon_wnd); do_fspage((BOOL)(*io_buff=='['));
				goto loop;}
			default:;
		  }
		mputs("\n");
		}
	 else if(fgets(io_buff,MAX_IO_CMD,infp)==NULL) return(FALSE);
	}
  if(infp==stdin)
	{shove:
	 if(tmpfp)
		{io_tmp_pos=ftell(tmpfp); fmoveto(tmpfp,io_tmp_pos);
		fputs(io_buff,tmpfp); fflush(tmpfp);
		io_tmp_end=ftell(tmpfp);
		}
	}
  if(cmdfp) fputs(io_buff,cmdfp);
  if(logfp) fputs(io_buff,logfp);
  return(TRUE);
}
void push_io()
{ fsk_elm *fpt;
  fsk_stk=lmore(fsk_stk,1,TYPE_STRUCT,fsk_elm);
  fpt=lst_tail(fsk_stk,fsk_elm);
  if(io_fnum==0) {fpt->fnum=1; fpt->pos=io_tmp_pos;}
  else {fpt->fnum=io_fnum; fpt->pos=io_file_pos;}
  fpt->lofs=(int)(io_buff_pt-io_buff);
  fpt->io_last_chr=io_last_chr;
  fpt->cmd_exe=cmd_exe;  fpt->cmd_pat=cmd_pat; fpt->cmd_yacc=cmd_yacc;
  fpt->cmd_brk=cmd_brk; fpt->cmd_const=cmd_const;
  fpt->cmd_exit=cmd_exit; fpt->entry=yacc_entry;
}
void set_io(fpt) fsk_elm *fpt;
{ io_fnum=fpt->fnum; io_file_pos=fpt->pos;
  infp=(lst_head(fly_list,fly_elm)+io_fnum)->fp;
  if(cmd_const=fpt->cmd_const) fcat(io_buff,cmd_const);
  else get_line(FALSE);
  io_last_chr=fpt->io_last_chr;
  cmd_pat=fpt->cmd_pat; cmd_exe=fpt->cmd_exe; cmd_brk=fpt->cmd_brk;
  cmd_exit=fpt->cmd_exit; yacc_entry=fpt->entry; cmd_yacc=fpt->cmd_yacc;
  if(fpt->lofs<(int)strlen(io_buff)) io_buff_pt=io_buff+fpt->lofs;
  else
	{io_buff_pt=io_buff+strlen(io_buff);
	 if(cmd_exe||
		(io_last_chr==';'&&(io_buff_pt==io_buff||*(io_buff_pt-1)!=';')))
		{*io_buff_pt=';'; *(io_buff_pt+1)='\0';}
	}
}
/* set io from a string */
void push_str(str) char *str;
{ push_io();
  io_buff_pt=io_buff; *io_buff='\0'; io_last_chr='\0';
  fcat(io_buff,str); cmd_const=str; /* signal io from a str const */
}
/* go to pos indicated by nnth elm from stack top, top= 0th elm */
void go_io(nn) int nn;
{ int kk=lst_ok(fsk_stk);
  if(kk>nn) {fsk_elm *fpt=lst_tail(fsk_stk,fsk_elm); set_io(fpt-nn);}
  else reset_io(stdin,0);
}
/* delete an elm from io stack without moving to pos */
void del_io(nn) int nn;
{ if(lst_num(fsk_stk)>nn) lst_num(fsk_stk)-=nn;
  if(lst_num(fsk_stk)<1) lst_num(fsk_stk)=1;
}
void pop_io()
{ go_io(0); del_io(1);
}
char input_char()
{ char chr;
  for(;;)
	{if((chr=(*io_buff_pt))) goto byebye;
	 if(cmd_const) return('\0');
	 if(cmd_exe) {cmd_exe=FALSE; if(io_last_chr!=';') goto byesemi;}
	 cmd_pat=FALSE;
	 if(!get_line(TRUE)) break;
	}
  if(io_last_chr&&io_last_chr!=';')
	{byesemi: chr=';'; *io_buff_pt=';'; *(io_buff_pt+1)='\0';
	 byebye: ++io_buff_pt; io_last_chr=chr; return(chr);
	}
  return('\0');
}
void unput_char(chr)
  char chr;
{ if(io_buff_pt>io_buff && chr) {--io_buff_pt; *io_buff_pt=chr;}
}
void skip_to_eol()
{ while(*io_buff_pt) ++io_buff_pt;
  io_last_chr=' '; cmd_exe=TRUE; cmd_pat=FALSE;
}
char skip_blanks(and_ret) BOOL and_ret;
{ char chr;
  for(;;) switch(chr=input_char())
	{case ' ': case '\t': break;
	 case '\n': if(and_ret) break;
	 default: return(chr);}
}

/* Skip anything in single or double quotes.
*  Returns first no quoted chr. Newline and EOF terminate it as well. */
char skip_quoted()
{ char chr='\n',laschr; BOOL dquote=FALSE,squote=FALSE,spaces=FALSE;
  do{spaces=FALSE;
	 switch(laschr=chr, chr=skip_blanks(FALSE)){
		case '\n': dquote=FALSE; squote=FALSE; spaces=TRUE; break;
		case '\0': return(chr);
		case '"': if(!squote && laschr!='\\') dquote=(BOOL)!dquote;
		 break;
		case '\'': if(!dquote && laschr!='\\') squote=(BOOL)!squote;
		 break;
		default: ;
	  }
	}while(spaces||dquote||squote);
  return(chr);
}

/* Skip over a matching pair.
*  Returns first chr if not lpair, or rpair if found. */
char skip_pair(lpair,rpair)
  char lpair,rpair;
{ int pair=0; char chr;
  do{chr=skip_quoted();
	 if(chr==lpair) ++pair; else if(chr==rpair) --pair;
	}while(pair&&chr);
  return(chr);
}

/* Skip anything inside quote, brackets etc.
*  Returns first chr if ok, or rpair. */
char skip_bodies()
{ char chr;
  for(;;) switch(chr=skip_quoted())
	{case '{': unput_char(chr); return(skip_pair('{','}'));
	 case '[': unput_char(chr); return(skip_pair('[',']'));
	 case '(': unput_char(chr); return(skip_pair('(',')'));
	 default: return(chr);
	}
}

char skip_unmatch()
{ char chr;
  for(;;) switch(chr=skip_quoted())
	{case '{': unput_char(chr); chr=skip_pair('{','}');
	  if(chr!='}') return(chr); break;
	 case '[': unput_char(chr); chr=skip_pair('[',']');
	  if(chr!=']') return(chr); break;
	 case '(': unput_char(chr); chr=skip_pair('(',')');
	  if(chr!=')') return(chr); break;
	 default: return(chr);
	}
}

/* skip to chr which isn't part of a matching body */
char skip_to_chr(chr,and_ret) char chr; BOOL and_ret;
{ char aa;
  do {aa=skip_unmatch(); if(and_ret&&aa=='\n') break;} while(aa!=chr && aa);
  return(aa);
}
/* skip the nxt statement which may be xxxx ';' or '{' xxx '}' */
void skip_statement(and_ret) BOOL and_ret;
{ char chr=skip_blanks(TRUE);
  if(chr=='{') {unput_char(chr); skip_bodies();}
  else skip_to_chr(';',and_ret);
}
void skip_comment()
{ char chr; input_char();
  do
	{do chr=input_char(); while(chr&&chr!='*');
	 if(chr) chr=input_char();
	}while(chr&&chr!='/');
}
char get_ident()
{ int n=namesize-2; char *id_pt=cur_ident,chr=skip_blanks(TRUE);
  if((isalpha(chr) || chr=='_'))
	{*id_pt=chr;
	 do{chr=input_char(); if(n) {--n; ++id_pt; *id_pt=chr;}
		}while(isident(chr));
	}
  unput_char(chr); *id_pt='\0';
  return(*cur_ident);
}

/* unpact cast char type code to int */
int cast_type(chr)
  char chr;
{ return((int)(((chr&0x30)<<1)|(chr&0x0f)));
}

void make_hole(at_pt,size)
  char *at_pt; int size;
{ char *from_pt=at_pt,*to_pt;
  if(size>0) /* move up */
	{from_pt+=strlen(at_pt);
	 to_pt=from_pt+size;
	 while(from_pt>=at_pt) {*to_pt=(*from_pt); --to_pt; --from_pt;}
	}
  else if(size<0) /* move down */
	{size=(-size);
	 to_pt=from_pt; from_pt+=size;
	 while(*from_pt) {*to_pt=(*from_pt); ++to_pt; ++from_pt;}
	 *to_pt='\0';
	}
}

void cir_free()
{ if(str_cir_bs)
	{str_cir_pt=(int)str_cir_max;
	 while(str_cir_pt--)
		{if(str_cir_bs[str_cir_pt]) lfree(str_cir_bs[str_cir_pt]);}
	 lfree(str_cir_bs);
	}
}

char *cir_save(ap)
  char *ap;
{ if(!str_cir_bs)
	{str_cir_bs=malmem(char *,str_cir_max); str_cir_pt=(int)str_cir_max;
	while(str_cir_pt--) str_cir_bs[str_cir_pt]=NULL;
	str_cir_pt=0;
	}
  if(str_cir_bs[str_cir_pt]) lfree(str_cir_bs[str_cir_pt]);
  str_cir_bs[str_cir_pt++]=(ap=strsave(ap));
  if(str_cir_pt>=(int)str_cir_max) str_cir_pt=0;
  return(ap);
}
/* rest of cur line contains file args separated by spaces, tabs or ';'
*  Note line must be terminated by a '\n' */
void get_args()
{ char *apt,*bpt,chr; farg_elm *fpt;
  if(!lst_ok(farg_stk)) return;
  fpt=lst_tail(farg_stk,farg_elm);
  while(chr=skip_blanks(FALSE))
	{if(chr=='\n') break;
	 apt=get_io_pos()-1;
	 do chr=input_char(); while(chr!=';'&&chr!=' '&&chr!='\t'&&chr!='\n');
	 bpt=get_io_pos()-1; if(chr==';') {++bpt; chr=(*bpt);}
	 *bpt='\0';
	 if(!*apt||*apt=='\n') {*bpt=chr; break;}
	 *fpt=lmore(*fpt,1,TYPE_PT,char *);
	 *lst_tail(*fpt,char *)=strsave(apt);
	 *bpt=chr;
	 if(chr=='\n') break;
	}
}

int get_base_code()
{ int pos,base_type=0,base_sign=0,first_time=TRUE; char *ap=get_io_pos();
  while((pos=find_name(cur_ident,base_list,MAX_BASE,sizeof(char *)))>0)
	{switch(pos)
		{case 2: /* double */
		case 3: /* float */ if(base_type==5) base_type=2;
		  base_sign=0;
		case 1: /* char */
		case 4: /* int */ if(base_type==5 || base_type==7) break;
		case 5: /* long */ base_type=pos; break;
		case 6: /* short */ base_type=10; break;
		case 7: /* unsigned */ base_sign=1; if(first_time) base_type=4; break;
		case 8: /* void */ base_type=6;
		default: ;
		}
	 first_time=FALSE;
	 ap=get_io_pos();
	 if(!get_ident()) break;
	}
  if(base_sign)
	switch(base_type)
	  {case 1: /* char */ base_type=11; break;
		case 4: /* int */ base_type=12; break;
		case 5: /* long */ base_type=13; break;
		case 10: /* short */ base_type=14; break;
		default:;
	  }
  if(*cur_ident) {set_io_pos(ap); *cur_ident='\0';}
  return(base_type);
}

double tx_to_d(num)
  NSLNSLfval num;
{ if(num.typ&STAR_MASK) return((double)((long)num.val.pp));
  switch(num.typ)
	{case TYPE_PT: return((double)((long)num.val.pp));
	 case TYPE_FLOAT: return((double)num.val.ff);
	 case TYPE_DOUBLE: return(num.val.dd);
	 case TYPE_LONG: return((double)num.val.ll);
	 case TYPE_USLONG: return((double)num.val.ull);
	 case TYPE_USINT: return((double)num.val.uii);
	 case TYPE_USSHORT: return((double)num.val.uss);
	 case TYPE_USCHAR: return((double)num.val.ucc);
	 case TYPE_SHORT: return((double)num.val.ss);
	 case TYPE_CHAR: return((double)num.val.cc);
	 case TYPE_INT:
	 default: ;
	}
  return((double)num.val.ii);
}

NSLNSLfval d_to_tx(ta,tb,dd)
  int ta,tb; double dd;
{ NSLNSLfval num;
  if(ta==TYPE_DOUBLE||tb==TYPE_DOUBLE)
	{num.typ=TYPE_DOUBLE; num.val.dd=dd;}
  else if(ta==TYPE_FLOAT||tb==TYPE_FLOAT)
	{num.typ=TYPE_FLOAT; num.val.ff=(float)dd;}
  else if(ta==TYPE_PT||tb==TYPE_PT||ta&STAR_MASK||tb&STAR_MASK)
	{num.typ=TYPE_PT; num.val.pp=(char *)((long)dd);}
  else if(ta==TYPE_USLONG||tb==TYPE_USLONG)
	{num.typ=TYPE_USLONG; num.val.ull=(unsigned long)dd;}
  else if(ta==TYPE_LONG||tb==TYPE_LONG)
	{num.typ=TYPE_LONG; num.val.ll=(long)dd;}
  else if(ta==TYPE_USINT||tb==TYPE_USINT)
	{num.typ=TYPE_USINT; num.val.uii=(unsigned int)dd;}
  else if(ta==TYPE_INT||tb==TYPE_INT)
	{num.typ=TYPE_INT; num.val.ii=(int)dd;}
  else if(ta==TYPE_USSHORT||tb==TYPE_USSHORT)
	{num.typ=TYPE_USSHORT; num.val.uss=(unsigned short)dd;}
  else if(ta==TYPE_SHORT||tb==TYPE_SHORT)
	{num.typ=TYPE_SHORT; num.val.ss=(short)dd;}
  else if(ta==TYPE_USCHAR||tb==TYPE_USCHAR)
	{num.typ=TYPE_USCHAR; num.val.ucc=(unsigned char)dd;}
  else if(ta==TYPE_CHAR||tb==TYPE_CHAR)
	{num.typ=TYPE_CHAR; num.val.cc=(char)dd;}
  else
	{num.typ=TYPE_INT; num.val.ii=(int)dd;}
  return(num);
}
NSLNSLfval d_to_t(ta,dd)
  int ta; double dd;
{ NSLNSLfval num;
  if(ta&STAR_MASK) ta=TYPE_PT;
  num.typ=ta;
  switch(ta)
	{case TYPE_DOUBLE: num.val.dd=dd; break;
	 case TYPE_FLOAT: num.val.ff=(float)dd; break;
	 case TYPE_PT: num.val.pp=(char *)((long)dd); break;
	 case TYPE_LONG: num.val.ll=(long)dd; break;
	 case TYPE_USLONG: num.val.ull=(unsigned long)dd; break;
	 case TYPE_SHORT: num.val.ss=(short)dd; break;
	 case TYPE_USSHORT: num.val.uss=(unsigned short)dd; break;
	 case TYPE_CHAR: num.val.cc=(char)dd; break;
	 case TYPE_USCHAR: num.val.ucc=(unsigned char)dd; break;
	 case TYPE_INT: num.val.ii=(int)dd; break;
	 case TYPE_USINT: num.val.uii=(unsigned int)dd; break;
	 default: num.typ=TYPE_INT; num.val.ii=(int)dd;
	}
  return(num);
}

void free_exp()
{ exp_elm *apt; int num;
  if(exp_stk)
	{apt=lst_head(exp_stk,exp_elm); num=lst_num(exp_stk);
	 while(num--) {lfree(apt->par_ofs); lfree(apt->par_stk); ++apt;}
	 lfree(exp_stk); exp_stk=NULL;
	}
}

/* save par_stk, set par to first arg */
void save_exp()
{ exp_elm *ept;
  exp_stk=lmore(exp_stk,1,TYPE_STRUCT,exp_elm);
  ept=lst_tail(exp_stk,exp_elm);
  ept->par_stk=par_stk; ept->par_str=par_str; ept->par_ofs=par_ofs;
  par_stk=NULL; par_str=NULL; par_ofs=NULL;
}

void restore_exp()
{ exp_elm *ept; int num=(lst_num(exp_stk)-=1);
  ept=lst_head(exp_stk,exp_elm)+num; lfree(par_stk); lfree(par_ofs);
  par_stk=ept->par_stk; par_str=ept->par_str; par_ofs=ept->par_ofs;
}

/* push an arg on arg stack */
void push_exp()
{ char *vpt,*ept; int cstype,top; size_t siz; 
  top=lst_ok(par_stk);
  if(par_str&&*par_str)
	{if(*par_str!=STAR_VARG)
		{nsl_func_rr=d_to_t((int)cast_type(*par_str),tx_to_d(nsl_func_rr));
		++par_str;}
	}
  cstype=nsl_func_rr.typ&STAR_TYPE;
  if(nsl_func_rr.typ&STAR_MASK) cstype=TYPE_PT;
  switch(cstype)
	{case TYPE_PT: siz=sizeof(char *); vpt=(char *)&nsl_func_rr.val.pp;
		break;
	 case TYPE_FLOAT: siz=sizeof(float); vpt=(char *)&nsl_func_rr.val.ff;
		break;
	 case TYPE_DOUBLE: siz=sizeof(double); vpt=(char *)&nsl_func_rr.val.dd;
		break;
	 case TYPE_LONG: siz=sizeof(long); vpt=(char *)&nsl_func_rr.val.ll; break;
	 case TYPE_USLONG: siz=sizeof(long); vpt=(char *)&nsl_func_rr.val.ull;
		break;
	 case TYPE_SHORT: siz=sizeof(short); vpt=(char *)&nsl_func_rr.val.ss;
		break;
	 case TYPE_USSHORT: siz=sizeof(short); vpt=(char *)&nsl_func_rr.val.uss;
		break;
	 case TYPE_CHAR: siz=sizeof(char); vpt=(char *)&nsl_func_rr.val.cc; break;
	 case TYPE_USCHAR: siz=sizeof(char); vpt=(char *)&nsl_func_rr.val.ucc;
		break;
	 case TYPE_USINT: siz=sizeof(int); vpt=(char *)&nsl_func_rr.val.uii;
	 case TYPE_INT:
	 default: siz=sizeof(int); vpt=(char *)&nsl_func_rr.val.ii;
	}
  par_stk=lmore(par_stk,(int)siz,TYPE_CHAR,char);
  ept=lst_head(par_stk,char); ept+=top;
  while(siz--) *ept++ = *vpt++;
  par_ofs=lmore(par_ofs,(int)2,TYPE_INT,int);
  *lst_tail(par_ofs,int)=(int)top;
  *(lst_tail(par_ofs,int)-1)=(int)cstype;
}
/* add a null to end of arg stack */
void push_null()
{ char *vpt,*ept,*npt=NULL; int top,siz=sizeof(char *);
  top=lst_ok(par_stk);
  par_stk=lmore(par_stk,siz,TYPE_CHAR,char);
  ept=lst_head(par_stk,char); ept+=top; vpt=(char *)&npt;
  while(siz--) *ept++ = *vpt++;
}
/* evaluate a user func which returns a value in nsl_func_rr */
typedef NSLNSLfval (*myusrtype)();

void user_func(num,indx) int num,indx;
{ NSLNSLnet *npt; NSLNSLfelm *fpt; char **apt; NSLNSLfval (*ept)();
  if(num) {npt=nsl_net_list[num-1]; fpt=npt->apt; ept=(myusrtype)(npt->ept);}
  else {fpt=nsl_mon_list; ept=nsl_mon_evaluate;}
  fpt+=indx;
  if(*par_str&&*par_str!=STAR_VARG)
	{if(num) apt=npt->upt; else apt=nsl_mon_names;
	 signal_error(WRN_USR_NAM,apt[indx]);
	}
  push_null();
  nsl_func_rr=(*ept)(lst_head(par_stk,char),fpt->fn_pt,fpt->swt);
  restore_exp();
}

/************************************************************************/
/* convert str integer to a binary long */
long tobinary(b) int b;
{ int c,t; long n=0;
  while(c=input_char())
	{switch(c)
	  {case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		 t = c-'0'; break;
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
		 t = c - 'a' + 10; if (b>10) break;
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
		 t = c - 'A' + 10; if (b>10) break;
		default: unput_char(c); t = -1;
	  }
	 if(t<0) break;
	 n=n*b+t;
	}
 return(n);
}

/* convert str reals to binary reals, on entry s pts to '.' of float */
double doubin(s)
  char *s;
{ char chr,*lex_str; double dvalue=0.0;
  do{++s; chr=(*s);
	 if(chr=='e' || chr=='E')
		{++s; chr=(*s); if(chr=='+' || chr=='-') {++s; chr=(*s);} }
	}while(chr<='9' && chr>='0');
  lex_str=get_io_pos();
  *s='\0'; sscanf(lex_str,"%lf",&dvalue); *s=chr; set_io_pos(s);
  return(dvalue);
}

/* Add name to list if it isn't on it, update size of list.
*  Returns pos+1 if on list, -pos if added to list.
*  NOTE: assumes name pt is 1st elm of struct (offset 0) !!!!!!!!!!
*/
int add_name(name,list,typ,siz)
  char *name,**list; int typ,siz;
{ char *apt,*bpt; int pos; int num;
  num=lst_ok(*list);
  if((pos=find_name(name,*list,num,siz))>0) return(pos);
  *list=(char *)nsl_lst_more(*list,1,typ,siz);
  apt=lst_end(*list)-1; bpt=apt-siz;
  pos=(-pos);
  num-=pos; num*=siz; while(num--) *apt-- = *bpt--;
  apt=(*list)+pos*siz; *((char **)apt)=name;
  return(-pos);
}

/* find jcl var in loc_list, fup_list & glo_list.
*  Returns adrs if found, or (NULL) if not */
char *find_local(name,typpt) char *name; int *typpt;
{ uff_elm *upt; glo_elm *llp; int num;
  if(lst_ok(uff_stk))
	{upt=lst_tail(uff_stk,uff_elm);
	 do_search: 
  	 num=lst_ok(upt->loc_list);
     if(num>0) llp=lst_head(upt->loc_list,glo_elm);
	 while(num--) /* check user func local vars */
		{if(strcmp(name,llp->name)==0)  /* return adrs */
		  {*typpt=llp->val.typ; return((char *)&llp->val.val);}
		++llp;
		}
	 num=lst_ok(upt->fup_list);
	 if(num>0) llp=lst_head(upt->fup_list,glo_elm);
	 while(num--) /* check user func parameters */
		{if(llp->name&&strcmp(name,llp->name)==0)
		  {/* fup_list elms are all pts to var */
		   *typpt=llp->val.typ; return((char *)llp->val.val.pp);}
		++llp;
		}
  if(is_was_upd&&upt!=lst_head(uff_stk,uff_elm)) {--upt; goto do_search;}
	}
  if(glo_list)
	if((num=find_name(name,&glo_list->name,
					  lst_num(glo_list),sizeof(glo_elm)))>0)
	  {*typpt=glo_list[num-1].val.typ;
		return((char *)&glo_list[num-1].val.val);
	  }
  return(NULL);
}
void yacc_cmds()
{ yacc_entry=S_CMDS; yyparse();
}
void yacc_exp(apt) char *apt;
{ push_str(apt);
  cmd_yacc=TRUE; cmd_exe=FALSE; cmd_brk=FALSE; cmd_exit=0; cmd_pat=FALSE;
  yacc_entry=S_EXP; yyparse();
  cmd_yacc=FALSE;
  pop_io();
}

BOOL update_was(forceclr) BOOL forceclr;
{ BOOL dobrk=FALSE,doclr=TRUE; int kk=0,jj; char **wp,*sp;
  if(forceclr) {doclr=FALSE; w_blank(mon_wat,FALSE);}
  if(!(jj=lst_ok(was_list))) return(FALSE);
  wp=was_list;
  while(jj--)
	{sp=(*wp);
	 if(sp&&(sp[1]=='e'||sp[1]=='d')&&((sp[2]=='w')||(sp[2]=='s')))
		{if(doclr) {doclr=FALSE; w_blank(mon_wat,FALSE);}
		if(kk>0) w_puts(-mon_wat,"\n"); ++kk;
		w_printf(-mon_wat,"%d:%s",*sp,sp+1);
		if(sp[1]=='e')
		  {is_was_upd=TRUE; yacc_exp(sp+TMP_OFS); is_was_upd=FALSE;
		   w_printf(-mon_wat,"=%g",tmp_val);
			  if((sp[2]=='s')&&tmp_val) dobrk=TRUE;}
		}
	 ++wp;
	}
  w_update(mon_wat);
  return(dobrk);
}

char *base_str(bpt,typ)
  char *bpt; int typ;
{ int stars;
  if(typ&STAR_CAST)
	{if(typ&STAR_PBIT) bpt=fcat(bpt,"pair ");
	 if(typ&STAR_MODE) bpt=fcat(bpt,"horz ");
	 stars=0;}
  else stars=(typ&STAR_MASK)>>STAR_SHFT;
  typ&=STAR_TYPE;
  if(typ>MAX_TYPES) typ=0;
  bpt=fcat(bpt,base_types[typ]);
  while(stars--) bpt=fcat(bpt,"*");
  return(bpt);
}

void func_pro(bpt,name,apt) char *bpt,*name,*apt;
{ bpt=base_str(bpt,cast_type(*apt)); ++apt;
  bpt=fcat(bpt," "); bpt=fcat(bpt,name); bpt=fcat(bpt,"(");
  while(*apt)
	{if(*apt==STAR_VARG) bpt=fcat(bpt,"va_args");
	 else bpt=base_str(bpt,cast_type(*apt));
	 ++apt;
	 if(*apt) bpt=fcat(bpt,",");
	}
  fcat(bpt,")\n");
}
void var_pro(bpt,name,typ) char *bpt,*name; int typ;
{ bpt=fcat(base_str(bpt,typ)," ");
  bpt=fcat(bpt,name); fcat(bpt,"\n");

}
void glo_pro(bpt,txt,gpt) char *bpt,*txt; glo_elm *gpt;
{ int jj;
  mputs(txt);
  if(gpt)
	{jj=lst_num(gpt);
	 while(jj--)
		{var_pro(bpt,gpt->name,gpt->val.typ); mputs(bpt); ++gpt;
		if(!w_more(mon_wnd,NIO_DEFAULT)) return;
		}
	}
  else mputs("  None\n");
}
void do_show_net(bpt,name,npt,fpt,vpt,nf,nv,isfun,isvar)
  char *bpt,*name,**npt; NSLNSLfelm *fpt; NSLNSLvar *vpt;
  int nf,nv; BOOL isfun,isvar;
{ if(isfun)
	{mprintf("%s functions:\n",name);
	 if(nf)
		{while(nf--)
			{func_pro(bpt,*npt,fpt->ats); mputs(bpt); ++npt; ++fpt;
			if(!w_more(mon_wnd,NIO_DEFAULT)) return;
			}
		}
	 else mputs("  None\n");
	}
 if(!w_more(mon_wnd,NIO_DEFAULT)) return;
 if(isvar)
	{mprintf("%s variables:\n",name);
	if(nv)
	  {while(nv--)
		 {var_pro(bpt,vpt->name,vpt->type_code); mputs(bpt); ++vpt;
		  if(!w_more(mon_wnd,NIO_DEFAULT)) return;
		 }
	  }
	else mputs("  None\n");
	}
}

void do_info_net(cp) NSLNSLnet *cp;
{ if(cp)
	{mputs("  ");
	 mprintf("%s:%s(%s)",cp->ins_name,cp->name,cp->sst_name);
	 if(cp->var_name) mprintf("<%s>",cp->var_name);
	 if(cp==mon_net) mputs("C,");
	 if(!(cp->flags&NET_CDECL)) mputs("L,");
	 if(cp->flags&NET_ACTIV) mputs("E,");
	 if(cp->flags&NET_STUB) mputs("S,");
	 mprintf("time=%ld,fns=%d,cxs=%d,nds=%d",
		cp->time,cp->num_funcs,cp->num_conxs,cp->num_nodes);
	 if(cp->num_ufs) mprintf(",ufn=%d",*(cp->num_ufs));
	 if(cp->num_sts) mprintf(",stu=%d",*(cp->num_sts));
	 if(cp->num_vas) mprintf(",vrs=%d",*(cp->num_vas));
	 mputs("\n");
	}
}
void do_show(key) char *key;
{ BOOL isall=FALSE,isloc=FALSE,isglo=FALSE,issys=FALSE,isnet=FALSE,iswnd=FALSE,
	isfun=FALSE,isvar=FALSE,isstp=FALSE,iswat=FALSE;
  int nn,jj,val; NSLNSLnet *cp,**netpt; 
  fun_elm *fun; uff_elm *upt; char tbuf[buffsize];
  w_more(mon_wnd,(int)TRUE);
  switch(*key)
	{case 'l': isloc=TRUE; break; case 'g': isglo=TRUE; break;
	 case 'c': issys=TRUE; break; case 'n': isnet=TRUE; break;
	 case 'f': isfun=TRUE; break; case 'v': isvar=TRUE; isglo=TRUE; isloc=TRUE; break;
	 case 'w': iswat=TRUE; break; case 's': isstp=TRUE; break;
	 case 'W': iswnd=TRUE; break;
	 default: isall=TRUE; isloc=TRUE; isglo=TRUE; issys=TRUE;
		isnet=TRUE; isfun=TRUE; isvar=TRUE; isstp=TRUE; iswat=TRUE; iswnd=TRUE;
	}
  if(iswnd) w_report(mon_wnd);
  if(isnet)
	{jj=lst_ok(nsl_net_list);
	 mputs("Installed nets:\n");
	 if(jj)
		{netpt=nsl_net_list;
		while(jj--)
		  {if(cp=(*netpt)) do_info_net(cp);
			++netpt;
		  }
		}
	 else mputs("  No nets installed\n");
	}
  if(!w_more(mon_wnd,NIO_DEFAULT)) return;
  if(issys)
	{mputs("Commands:\n"); jj=0;
	 for(nn=0;nn<cmd_num;++nn)
		{mprintf(" %s",cmd_list[nn].name); ++jj;
		 if(jj>10) {mputs("\n"); jj=0;}}
	 if(jj!=0) mputs("\n");
	 if(!w_more(mon_wnd,NIO_DEFAULT)) return;
	 mputs("User commands:\n");
	 val=lst_ok(user_list);
	 if(val)
		{for(nn=0;nn<val;++nn)
		{mprintf(" %s",user_list[nn].name); ++jj;
		 if(jj>10) {mputs("\n"); jj=0;}}
		if(jj!=0) mputs("\n");
		}
	 else mputs("  None\n");
	 if(!w_more(mon_wnd,NIO_DEFAULT)) return;
	 mputs("Types:\n");
	 for(nn=0;nn<MAX_BASE;++nn) mprintf(" %s",base_list[nn]);
	 mputs("\n");
	}
  if(!w_more(mon_wnd,NIO_DEFAULT)) return;
  jj=lst_ok(nsl_net_list);
  if(jj)
	{netpt=nsl_net_list;
	 while(jj--)
		{if(cp=(*netpt))
		  {do_show_net(tbuf,cp->ins_name,cp->upt,cp->apt,cp->vpt,
		  cp->num_ufs?*(cp->num_ufs):0,
		  cp->num_vas?*(cp->num_vas):0,isfun,isvar);
		  }
		 if(!w_more(mon_wnd,NIO_DEFAULT)) return;
		++netpt;
		}
	}
  if(isfun||isglo)
	 do_show_net(tbuf,"Core",nsl_mon_names,nsl_mon_list,nsl_mon_vars,
		nsl_num_mon_list,nsl_num_mon_vars,isfun,isglo);
  if(!w_more(mon_wnd,NIO_DEFAULT)) return;
  if(isfun)
	{jj=lst_ok(fun_list);
	 mputs("User functions:\n");
	 if(jj)
		{fun=fun_list;
		while(jj--)
		 {func_pro(tbuf,fun->name,fun->ats); mputs(tbuf); ++fun;
		  if(!w_more(mon_wnd,NIO_DEFAULT)) return;
		 }
		}
	 else mputs("  None\n");
	}
  if(isglo) glo_pro(tbuf,"Global variables:\n",glo_list);
  if(!w_more(mon_wnd,NIO_DEFAULT)) return;
  if(isloc&&lst_ok(uff_stk))
	{upt=lst_tail(uff_stk,uff_elm);
	 glo_pro(tbuf,"Local variables:\n",upt->loc_list);
	 if(!w_more(mon_wnd,NIO_DEFAULT)) return;
	 glo_pro(tbuf,"Functiona parameters:\n",upt->fup_list);
	 if(!w_more(mon_wnd,NIO_DEFAULT)) return;
	}
  if(isvar) {var_pro(tbuf,"$$",nsl_func_rr.typ); mputs(tbuf);}
  if(!w_more(mon_wnd,NIO_DEFAULT)) return;
  if(isstp||iswat) update_was(TRUE);
  w_more(mon_wnd,(int)FALSE);
}

/* some handy defs to save typing...*/
#define cs(nn,mm) case nn:yylval.sval=mm;return(nn)
#define cscc(nn,mm,pp,qq) case nn:if(*lex_str=='=')\
		{input_char();yylval.sval=pp;return(qq);}yylval.sval=mm;return(nn)
#define csop(nn,mm,rr) case nn:\
		if(*lex_str=='='){input_char();yylval.ival=rr;return(EQOP);}\
		yylval.sval=mm;return(nn)
#define cseq(nn,mm,pp,qq,rr) case nn:\
		if(*lex_str=='='){input_char();yylval.ival=rr;return(EQOP);}\
		if(*lex_str==nn){input_char();yylval.sval=pp;return(qq);}\
		yylval.sval=mm;return(nn)

int yylex()
{ char chr,*s,*lex_str; int val; user_elm *upt;
  if(yacc_entry)
	{val=yacc_entry;
	 if(cmd_brk) {yacc_entry=CEOF; return(CEOF);} else yacc_entry=0;
	 return(val);}
  for(;;)
	{chr=skip_blanks(TRUE);
	 lex_str=get_io_pos();
	 yacc_lastok=lex_str-1;
	 switch(chr) /* chk 2 chr ops & 1 chr ops */
		{case '\0':
		  if(cmd_const&&!cmd_yacc) {pop_io(); continue;}
		  return(CEOF);
		cscc('!',"!","!=",NE);
		case '"': /* a string */
		  unput_char(chr); chr=skip_quoted();
		  s=get_io_pos(); chr=(*s); *s='\0';
		  yylval.sval=cir_save(lex_str-1); *s=chr;
		  dqstr_to_mem(yylval.sval);
		  return(DQSTR);
		cs('#',"#");
		case '$': if(*lex_str=='$') {input_char(); return(DOLDOL);}
		  yylval.sval="$";return('$');
		case '%': val=(*lex_str);
		if(val=='='){input_char();yylval.ival=MODEQ;return(EQOP);}
		if(isdigit(val)&&!isdigit(*(lex_str+1))) /* chk for file arg */
		  {val-='0';
			if(lst_ok(farg_stk)) s=(char *)(*lst_tail(farg_stk,farg_elm)); else s=NULL;
			if(s && lst_num(s)>val)
			 {s= *(lst_head(s,char *)+val);
			  set_io_pos(lex_str+1);
			  push_str(s); cmd_yacc=FALSE;
			 }
			continue;
		  }
		yylval.sval="%";return('%');
		cseq('&',"&","&&",ANDAND,ANDEQ);
		case '\'':
		  unput_char(chr); chr=skip_quoted();
		  s=get_io_pos(); chr=(*s); *s='\0';
		  yylval.fval.val.ii=sqchrtobin(lex_str-1); *s=chr;
		  yylval.fval.typ=TYPE_INT;
		  return(NUMBER);
		cs('(',"("); cs(')',")");
		csop('*',"*",MULEQ);
		cseq('+',"+","++",PLUPLU,PLUEQ);
		cs(',',",");
		cseq('-',"-","--",MINMIN,MINEQ);
		case '.':
		  if(*lex_str=='.') {input_char(); yylval.sval="..";return(DOTDOT);}
		  yylval.sval=".";return('.');
		case '/':
		  if(*lex_str=='*'){skip_comment(); continue;}
		  if(lex_str-1==io_buff)
			{cmd_exe=TRUE; cmd_pat=FALSE;
			 if(*(io_buff+1)!='\n')
			  {s=fs_buff;
				do{chr=input_char(); *s=chr; ++s; }while(chr&&chr!='\n');
				*(s-1)='\0';}
			 return(CSEARCH);
			}
		  if(*lex_str=='='){input_char(); yylval.ival=DIVEQ;return(EQOP);}
		  yylval.sval="/";return('/');
		case ':':
			if(lex_str-1==io_buff) {cmd_exe=TRUE; cmd_pat=FALSE; return(CLINE);}
			yylval.sval=":";return(':');
		case ';': cmd_pat=FALSE; yylval.sval=";";return(';');
		case '<':
		  if(*lex_str=='=') {input_char(); yylval.sval="<=";return(LE);}
		  if(*lex_str=='<')
			{input_char();
			 if(*(lex_str+1)=='=')
				{input_char(); yylval.ival=LSEQ;return(EQOP);}
			 yylval.sval="<<";return(LS);}
		  yylval.sval="<"; return('<');

		cscc('=',"=","==",EQ);
		case '>':
		  if(*lex_str=='=') {input_char(); yylval.sval=">=";return(GE);}
		  if(*lex_str=='>')
			{input_char();
			 if(*(lex_str+1)=='=')
				{input_char();yylval.ival=RSEQ;return(EQOP);}
			 yylval.sval=">>";return(RS);}
		  yylval.sval=">"; return('>');
		case '?':
		  if(lex_str-1==io_buff)
			{cmd_exe=TRUE; cmd_pat=FALSE;
			 if(*(io_buff+1)=='\n') return(CHELP); return(CPRINT);}
		yylval.sval="?";return('?');
		cs('@',"@"); cs('[',"[");
		cs('\\',"\\"); cs(']',"]");
		csop('^',"^",XOREQ);
		case '_': goto doident;
		cs('`',"`"); cs('{',"{");
		cseq('|',"|","||",OROR,OREQ);
		cs('}',"}"); cs('~',"~");
		default: ; /* just leaves nums & alphas */
		}
	 if(isdigit(chr))
		{val=NUMBER; /* a number of some sort */
		yylval.fval.typ=TYPE_INT;
		s=lex_str; while(*s<='9' && *s>='0') ++s;
		if(*s=='.' && *(s+1)!='.')  /* a reals number, but not '..' */
		  {unput_char(chr);
			yylval.fval.val.dd=doubin(s); yylval.fval.typ=TYPE_DOUBLE;
			return(val);}
		else if(chr=='0')
		  {chr=input_char();
			if(chr=='x' || chr=='X') {yylval.fval.val.ll=tobinary(16);}
			else {unput_char(chr); yylval.fval.val.ll=tobinary(8);}
		  }
		else {unput_char(chr); yylval.fval.val.ll=tobinary(10);}
		chr=(*get_io_pos());
		if(chr=='l'||chr=='L'||chr=='I')
		  {input_char(); yylval.fval.typ=TYPE_LONG;}
		else yylval.fval.val.ii=(int)yylval.fval.val.ll;
		return(val);
		}
	 /* check idents */
	 doident:
	 unput_char(chr);
	 if(get_ident())
		{if(lex_str[-2]=='.'||*get_io_pos()=='.')
			goto plainid;
		 if(*get_io_pos()==':')
			{if(nsl_get_p(cur_ident))
			  {chr=input_char();
				yylval.sval=cur_ident;
				fcat(cur_ident+strlen(cur_ident),":");
				cmd_exe=TRUE; tmp_net=(*my_net_p);
				return(CNET);}
			else
		  	{input_char();
				yylval.sval=cur_ident; fcat(cur_ident+strlen(cur_ident),":");
				return(LABEL);}
			}
		 if((val=find_name(cur_ident,base_list,MAX_BASE,sizeof(char *)))>0)
		  {yylval.ival=get_base_code(); cmd_pat=FALSE; cmd_exe=FALSE; return(CTYPE);}
		if((val=find_name(cur_ident,&cmd_list->name,
						  cmd_num,sizeof(cmd_elm)))>0)
		  {cmd_exe=cmd_list[val-1].addsemi; cmd_pat=FALSE;
			val=(int)cmd_list[val-1].token;
			switch(val)
			 {case CDIR: case CCD: cmd_pat=TRUE; break;
			  case CSHELL: case CWATCH: case CSTOP:
	chr=skip_blanks(); unput_char(chr); lex_str=get_io_pos();
	skip_statement(TRUE); s=get_io_pos(); chr=(*s); *s='\0';
	fcat(tmp_buff+TMP_OFS,lex_str); *s=chr; s=tmp_buff+TMP_OFS;
	while(*s) {if(*s=='/'&&*(s+1)=='*') {*s='\0'; break;} ++s;}
	s=tmp_buff+TMP_OFS;
	if(*s)
	  {lex_str=s; s+=strlen(s)-1;
		while(*s=='\n'||*s==' '||*s==';')
		 {*s='\0'; if(s==lex_str) break; --s;}
	  }
			  default:;
			 }
			return(val);
		  }
		if(user_list)
		  {upt=user_list; val=lst_num(user_list);
			while(val--)
			 {if(strcmp(cur_ident,upt->name)==0)
				{cmd_pat=FALSE; cmd_exe=TRUE; yylval.fpval=upt->fpt; return(CUSER);}
			  ++upt;
			 }
		  }
		if(cmd_pat) goto plainid;
		chr=skip_blanks(TRUE);
		unput_char(chr);
		if(chr!='.')
		  {if(find_symadr(cur_ident,&val,&yylval.nival.indx))
			 {save_exp(); yylval.nival.net=val;
			  if(val)
				par_str=nsl_net_list[val-1]->apt[yylval.nival.indx].ats+1;
			  else par_str=nsl_mon_list[yylval.nival.indx].ats+1;
			  return(FUNKY);
			 }
			else if(fun_list&&(val=find_name(cur_ident,&fun_list->name,
								  lst_num(fun_list),sizeof(fun_elm)))>0)
			 {save_exp(); yylval.ival=val; --val;
			  par_str=fun_list[val].ats+1;
			  return(UFFKY);
			 }
			else if((yylval.gval.val.val.pp=find_local(cur_ident,&yylval.gval.val.typ)))
			 {yylval.gval.name=cir_save(cur_ident); return(VAR);}
			else if((val=find_name(cur_ident,&(nsl_mon_vars->name),
								  nsl_num_mon_vars,sizeof(NSLNSLvar)))>0)
			 {yylval.gval.val.val.pp=nsl_mon_vars[val-1].adrs;
			  yylval.gval.val.typ=nsl_mon_vars[val-1].type_code; netty:
			  yylval.gval.name=cir_save(cur_ident);
			  return(VAR);}
			else if((val=var_adrs(NULL,(long)0,cur_ident,&(yylval.gval.val.val.pp)))>0)
				{yylval.gval.val.typ=val; goto netty;
				}
			}
		/* must be an ident parameter to a cmd */ plainid:
		yylval.sval=cir_save(cur_ident);
		return(IDENT);
		}
	 return(input_char());
	}
}

void yyerror(s)
  char *s;
{ char *spt,*ept,chr; int num=0,rest=0,cols=w_x(w_size(mon_wnd,TRUE));
  cmd_exit=2;
  if(cmd_brk) return;
  yacc_ioerr=yacc_lastok;
  spt=yacc_ioerr;
  while(spt>yacc_io && (*spt=='\n'||*spt==' '||*spt=='\t')) --spt;
  ept=spt+1;
  while(spt>yacc_io && *spt!='\n')
	{++num; --spt; if(num>(cols-4)) break;}
  if(*spt=='\n') ++spt;
  rest=num; while(rest<cols && *ept!='\n' && *ept) {++rest; ++ept;}
  chr=(*ept); *ept='\0';
  mprintf("%s\n",spt);
  *ept=chr;
  while(num>1) {mputs(" "); --num;} mputs("^\n");
}


/* vid contains a var type & a pt to it
*  apply operation opr to it with val
*  if old set return val before adjusting to new val
*/
NSLNSLfval var_selector(vid,val,opr,old)
  NSLNSLfval vid,val; int opr; BOOL old;
{ char *apt=vid.val.pp; double oldval,curval,newval,factor=1.0;
  int typ=vid.typ;
  if(!apt) return(zerval);
  curval=tx_to_d(val);
  /* chk if accessing func return variable */
  if(apt==(char *)&nsl_func_rr.val&&opr=='=')
	{typ=val.typ; vid.typ=typ; oldval=(double)0.0;}
  else oldval=nsl_lst_val(&apt,(mile)typ);
  newval=oldval;
  if(typ&STAR_MASK)
	{if(typ&STAR_TWO) factor=(double)sizeof(char *);
	 else switch(opr)
		{case PLUEQ: case MINEQ: case PLUPLU: case MINMIN:
		  factor=(double)lsize(typ&STAR_TYPE,(char *)NULL);
		default: ;
		}
	}
  switch(opr)
	{case MULEQ: newval*=curval; break;
	 case DIVEQ: newval/=curval; break;
	 case MODEQ: ((long)newval)%=(long)curval; break;
	 case ANDEQ: ((long)newval)&=(long)curval; break;
	 case XOREQ: ((long)newval)^=(long)curval; break;
	 case OREQ: ((long)newval)|=(long)curval; break;
	 case LSEQ: ((long)newval)<<=(long)curval; break;
	 case RSEQ: ((long)newval)>>=(long)curval; break;
	 case PLUEQ: newval+=curval*factor; break;
	 case MINEQ: newval-=curval*factor; break;
	 case PLUPLU: newval+=factor; break;
	 case MINMIN: newval-=factor; break;
	 case '=': newval=curval; break;
	 default: goto skip; /* just return it's val */
	}
  apt=vid.val.pp; nsl_lst_put(apt,newval,(int)typ);
  skip: if(old) return(d_to_t(typ,oldval));
  return(d_to_t(typ,newval));
}
/* look back in state stk for CFOR_ONE, CWHILE, or func
*  if found pop to it & return cmd, or leave stk & return 0 */
int pop_to_loop(cfonly) BOOL cfonly;
{ int num=lst_ok(state_stk); int *cpt;
  if(num>0) cpt=lst_tail(state_stk,int);
  while(num>0)
	 {switch(*cpt)
		{case CFOR_ONE: case CWHILE: if(cfonly) break;
		 case CCALL: case UFFKY: lst_num(state_stk)=num; return(*cpt);
		 default:;
		}
	  --num; --cpt;
	 }
  return(0);
}
/* command stk */
void push_state(state) int state;
{ state_stk=lmore(state_stk,1,TYPE_INT,int);
  *lst_tail(state_stk,int)=state;
}
int pop_state()
{ int num=lst_ok(state_stk); if(num<1) return(0);
  lst_num(state_stk)=(--num);
  return(*(lst_head(state_stk,int)+num));
}
int last_state()
{ int num=lst_ok(state_stk); if(num<1) return(0);
  return(*(lst_head(state_stk,int)+num-1));
}

/* user file func evaluation stk */
void push_uff(fnum) int fnum;
{ uff_elm *upt; glo_elm *gpt; int *opt; char **npt,*apt; int num;
  uff_stk=lmore(uff_stk,1,TYPE_STRUCT,uff_elm);
  upt=lst_tail(uff_stk,uff_elm); upt->fnum=fnum;
  upt->fup_list=NULL; upt->loc_list=NULL; upt->arg_stk=NULL;
  if(par_ofs)
	{num=lst_num(par_ofs)/2; opt=lst_head(par_ofs,int);
	 gpt=lmore(NULL,num,TYPE_STRUCT,glo_elm);
	 upt->fup_list=gpt; gpt=lst_head(gpt,glo_elm); apt=lst_head(par_stk,char);
  if(fnum) npt=fun_list[fnum-1].parms;
	 while(num--)
		{if(fnum) {gpt->name=(*npt); ++npt;} else gpt->name=NULL;
		 gpt->val.typ=(*opt++); gpt->val.val.pp=apt+(*opt++); ++gpt;}
	}
}
void glo_free(gpt) glo_elm *gpt;
{ int num=lst_num(gpt); gpt=lst_head(gpt,glo_elm);
  while(num--) {lfree(gpt->name); ++gpt;}
}
void pop_uff()
{ uff_elm *upt; int num=lst_ok(uff_stk); glo_elm *apt;
  if(num<1) return;
  lst_num(uff_stk)=(--num);
  upt=lst_head(uff_stk,uff_elm)+num;
  if(apt=upt->fup_list) {lfree(apt); upt->fup_list=NULL;}
  if(apt=upt->loc_list) {glo_free(apt); lfree(apt); upt->loc_list=NULL;}
  lfree(upt->arg_stk); upt->arg_stk=NULL;
}
/* file arg stk */
void push_farg()
{ farg_elm *fpt;
  farg_stk=lmore(farg_stk,1,TYPE_PT,farg_elm);
  fpt=lst_tail(farg_stk,farg_elm);
  *fpt=NULL;
}
void pop_farg()
{ farg_elm *lpt,apt; int num=lst_ok(farg_stk);
  if(num<1) return;
  lpt=lst_tail(farg_stk,farg_elm);
  lst_num(farg_stk)=(--num);
  if(*lpt)
	{num=lst_num(*lpt); apt=lst_head(*lpt,char *);
	 while(num--) lfree(*apt++);
	 lfree(*lpt);
	}
}
void free_list(lst,num)
  char **lst; int num;
{ if(lst) {while(num--) lfree(lst[num]); lfree(lst);}
}
void free_func(fpt) fun_elm *fpt;
{ if(fpt->ats) {lfree(fpt->ats); free_list(fpt->parms,fpt->p_num);}
  chk_free(fpt->name);
}
BOOL is_set(netpt) NSLNSLnet *netpt;
{ if(netpt) return(TRUE);
  mputs("\nNo net! Select a net first!\n");
  return(FALSE);
}
void do_shell(cmd) char *cmd;
{ system(cmd); 
}

extern void debug();

void do_debug()
{ debug(); 
}
void do_help()
{ int nn=0; w_more(mon_wnd,(int)TRUE);
  while(cmd_help[nn])
	{mprintf("%s\n",cmd_help[nn++]);
	 if(!w_more(mon_wnd,NIO_DEFAULT)) return;
	}
  w_more(mon_wnd,(int)FALSE);
}
void do_log(name,fext,fpp) char *name,*fext; FILE **fpp;
{ char buff[buffsize];
  if(*fpp) {fclose(*fpp); *fpp=NULL;}
  if(strcmp(name,"off")!=0)
	{sprintf(buff,"%s.%s\0",name,fext); *fpp=fopen(buff,"w");}
}
/* push id details */
void do_id_func()
{ fun_elm *fpt; uff_elm *upt; glo_elm *gpt; int pos;
  if(parm_num||parm_pos>=0) /* a func prototype or body */
	{/*save func details, add to sorted fun_list:*/
	 pos=add_name(parm_name,&fun_list,(int)TYPE_STRUCT,(int)sizeof(fun_elm));
	 if(pos>0) /* clear/redefine prior func or prototype */
		{fpt=fun_list+pos-1; free_func(fpt); fpt->name=parm_name;}
	 else fpt=fun_list-pos;
	 parm_name=NULL;
	 if(parm_pos>=0) /* func body */
		{fpt->ats=strsave(parm_args);
		fpt->parms=parm_list; fpt->p_num=parm_num;
		fpt->iop.fnum=parm_iof; fpt->iop.lofs=parm_ofs;
		fpt->iop.pos=parm_pos; fpt->iop.cmd_const=parm_const;
		fpt->iop.cmd_brk=parm_brk; fpt->iop.cmd_exe=parm_exe;
		fpt->iop.cmd_pat=parm_pat; fpt->iop.cmd_yacc=parm_yacc;
		fpt->iop.cmd_exit=parm_exit; fpt->iop.entry=parm_entry;
		}
	 else  /* prototype, set *fpt to null */
		{fpt->ats=NULL; fpt->parms=NULL; fpt->p_num=0;
		free_list(parm_list,parm_num);
		}
	 parm_list=NULL; parm_num=0;
	}
  else /* save a local or global variable */
	{if(lst_ok(uff_stk)&&(lst_tail(uff_stk,uff_elm)->fnum))
		{/* treat as local var */
		upt=lst_tail(uff_stk,uff_elm);
		upt->loc_list=lmore(upt->loc_list,1,TYPE_STRUCT,glo_elm);
		gpt=lst_tail(upt->loc_list,glo_elm);
		gpt->name=parm_name;
		}
	 else
		{pos=add_name(parm_name,&glo_list,(int)TYPE_STRUCT,(int)sizeof(glo_elm));
		if(pos>0) {lfree(parm_name); gpt=glo_list+pos-1;}
		else gpt=glo_list-pos;
		}
	 parm_name=NULL;
	 gpt->val=d_to_t(*parm_args,tx_to_d(parm_val));
	}
}
void was_clear()
{  int num; char **apt;
  if(was_list)
	{num=lst_ok(was_list); apt=was_list;
	 while(num--) {if(*apt) lfree(*apt); ++apt;}
	 lfree(was_list); was_list=NULL;}
}
void do_was(int num, char was)
{ char **apt,*bp; int nn,jj;
  tmp_buff[0]='1'; tmp_buff[1]='e'; tmp_buff[2]=was; tmp_buff[3]=':';
  nn=lst_ok(was_list);
  switch(was)
	{case 'k':
		if(num<=0||nn==0) was_clear();
		else if(num<=nn) {apt=was_list+num-1; lfree(*apt); *apt=NULL;}
		break;
	 case 'e': case 'd': if(num>nn||nn<1) break;
		if(num!=0) {--num; nn=1;}
		apt=was_list+num;
		while(nn--) {if(*apt) (*apt)[1]=was; ++apt;}
		break;
	 default:
		 apt=was_list+num;
		 while(nn--)
			{if(*apt)
			  {if(strcmp((*apt)+2,tmp_buff+2)==0) {(*apt)[1]='e'; goto byebye;}}
			++apt;
			}
		 apt=new_slot(&was_list); bp=strsave(tmp_buff); *apt=bp; jj=1;
		 while(apt!=was_list) {++jj; --apt;}
		 *bp=(char)jj;
	} byebye:
  update_was(TRUE);
}

void CtrlC_intr()
{ nsl_stop=1; nsl_sig_int=1; yacc_entry=CEOF; cmd_brk=TRUE;
}

extern int 
	nsl_mon_min=1, /* min number of cycles */
	nsl_mon_max=10000, /* max number of cycles */
	nsl_mon_rate=1, /* cycles per loop */
	nsl_mon_delay=250; /* millisecond delay between cycles */

void do_step(isstep) BOOL isstep;
{ NSLNSLnet **npt; long *lpt,a,b,c,nn; int fret=0,jj,kk; BOOL active=FALSE;
  push_null(); push_null(); 
  lpt=lst_head(par_stk,long); nsl_stop=0; nsl_sig_int=0;
  a=(*lpt++); b=(*lpt++); c=(*lpt++);
  if(isstep) 
  {	if(a<1) a=nsl_mon_min; 
	if(b<1) b=nsl_mon_rate; 
	if(c<1) c=nsl_mon_delay;
  }
  else {a=nsl_mon_max; b=nsl_mon_rate; c=nsl_mon_delay;}
  if(nsl_net_list)
  	{kk=lst_num(nsl_net_list);
  	 for(nn=0;nn<a;++nn)
		{if(update_was(FALSE)) {active=TRUE; break;}
	  	 /* cycle each net */
		 jj=kk; npt=nsl_net_list;
		 while(jj--)
			{if(*npt&&((*npt)->flags&NET_ACTIV))
			  {active=TRUE;
				fret=nsl_driver(*npt,NSLIO_PRE,lpt);
				if(NSLCtrl_C()) CtrlC_intr();
				if(nsl_stop||nsl_abort||nsl_sig_int) goto byebye;
				fret=nsl_cycle(*npt,b);
				if(NSLCtrl_C()) CtrlC_intr();
				if(nsl_stop||nsl_abort||nsl_sig_int) goto byebye;
				fret=nsl_driver(*npt,NSLIO_POST,lpt);
				if(NSLCtrl_C()) CtrlC_intr();
				if(nsl_stop||nsl_abort||nsl_sig_int||w_active(mon_wnd)) goto byebye;
			  }
			++npt;
			}
	  	 if(!active) break;
			if(mon_dump) w_dump((int)-1,NIO_DP_DUMP);
		 if(c>1) w_sleep((int)c);
		}
	}
  byebye:
  cmd_brk=FALSE; yacc_entry=0;
  nsl_func_rr.typ=TYPE_INT; nsl_func_rr.val.ii=fret;
  restore_exp();
  if(!active) mputs("No nets enabled!\n");
}

void do_driver()
{ int cmd,fret=0; void **args; char *key;
  push_null(); 
  if(!is_set(mon_net)) goto byebye;
  args=(void **)lst_head(par_stk,char);
  if(!args||!*args) goto byebye;
  key=(char *)(*args); ++args;
  switch(*key)
  {	case 'n': cmd=NSLIO_NULL; break;
    case 's': cmd=NSLIO_SETUP; break;
	case 'i': cmd=NSLIO_INIT; break;
	case 'c': cmd=NSLIO_CLOSE; break;
	case 'r': cmd=NSLIO_REDRAW; break;
	case 'p': 
		if(key[1]=='a') cmd=NSLIO_PATTERN; 
		else if(key[1]=='r') cmd=NSLIO_PRE;
		else if(key[1]=='o') cmd=NSLIO_POST;
		else goto byebye;
		break;
	case 'u': cmd=NSLIO_USER; break;
	default: goto byebye;
  }
  fret=nsl_driver(mon_net,cmd,args);
byebye:
  nsl_func_rr.typ=TYPE_INT; nsl_func_rr.val.ii=fret;
  restore_exp();
}

void do_gen_func(fpt) int (*fpt)();
{ int fret;
  push_null(); 
  fret=(*fpt)(lst_head(par_stk,char));
  nsl_func_rr.typ=TYPE_INT; nsl_func_rr.val.ii=fret;
  restore_exp();
}
void do_mess()
{ merr("Net not installed! Network variable unchanged.");
}
/* remove net from net list & free it's mem if possible */
void do_del_one(name) char *name;
{ NSLNSLnet *npt;
  if(!(npt=nsl_get_p(name))) {do_mess(); return;}
  if(nsl_uninstall(name)==NSLER_OK)
	{if(npt==mon_net&&!(npt->flags&NET_CDECL)) mon_net=NULL;
	 nsl_driver(npt,NSLIO_CLOSE,(char *)NULL);
	 nsl_free(npt);
	}
}
void do_delete(name) char *name;
{ NSLNSLnet **npt; int jj;
  if(strcmp(name,"all")==0)
	{npt=nsl_net_list; jj=lst_ok(npt);
	 while(jj--) {if(*npt) do_del_one((*npt)->ins_name); ++npt;}
	 if(nsl_net_list) {lfree(nsl_net_list); nsl_net_list=NULL;}
	}
  else do_del_one(name);
}
void do_enadis(name,ena) char *name; BOOL ena;
{ NSLNSLnet *netpt=NULL,**npt; int jj;
  if(strcmp(name,"all")==0)
	{npt=nsl_net_list; jj=lst_ok(npt);
	 while(jj--)
		{if(*npt)
		  {netpt=(*npt);
			if(ena) {if(!(netpt->flags&NET_STUB)) netpt->flags|=NET_ACTIV;}
			else netpt->flags&=(0xff-NET_ACTIV);
		  }
		++npt;
		}
	 if(!netpt) return;
	}
  else
	{if(!(netpt=nsl_get_p(name))) {do_mess(); return;}
	 if(ena) {if(!(netpt->flags&NET_STUB)) netpt->flags|=NET_ACTIV;}
	 else netpt->flags&=(0xff-NET_ACTIV);
	}
}

void do_load(name,filn) char *name,*filn;
{ char *ap; NSLNSLnet *npt=(NSLNSLnet *)1L;
  ap=strsave(name);
  if(nsl_install(npt,ap)!=NSLER_OK)
	{merr("Bad net name for install"); goto byebye;}
  npt=nsl_get(filn); *my_net_p=npt;
  if(npt)
	{npt->ins_name=ap; npt->flags|=NET_ACTIV;
	 nsl_driver(npt,NSLIO_SETUP,(char *)NULL);
	}
  else {merr("Couldn't load net"); byebye: lfree(ap);}
}
void do_save(name,filn) char *name,*filn;
{ NSLNSLnet *npt;
  if(!(npt=nsl_get_p(name))) merr("Net not installed!?");
  else if(nsl_put(npt,filn)!=NSLER_OK) merr("Couldn't save net");
}
/* display data structure in dir format: indx:(-3)=dir,(-2)=all,(-1)=none,n */
void dsp_dir(ppt,ndbs,indx)
  path_elm *ppt; long ndbs; int indx;
{ path_elm dinod; BOOL dobrk=TRUE,isary=FALSE;
  int typ,sw,cc,kk,nn,num=1; 
  decl_elm *dpt; char *adrs;
  dinod.net=ppt->net; dinod.mempt=ppt->mempt; dpt=ppt->decl_pt;
  if(indx<0)
	{dinod.indx=0;
	 if(indx==(-2))
		{num=dpt->size;
		if(num<1) {num=(int)ua_size(ppt,dpt); if(num<1) num=1;}
		if(num>1) isary=TRUE;
		}
	 else
		{if(indx==(-3)) dobrk=FALSE;}
	}
  else dinod.indx=indx;
  sw=w_x(w_size(mon_wnd,TRUE))-2;
  while(dpt)
	{nn=dpt->size; if(nn<1) nn=(int)ua_size(ppt,dpt);
	 kk=mprintf("%d#%ld[%d:%d]%d:%s=",
			 dpt->declnum,ndbs+dpt->nodenum,
			 dinod.indx,nn,dpt->nodes,dpt->name);
	 typ=dpt->decltype;
  	 if(!typ) mputs("?\n");
  	 else if(typ==TYPE_STRUCT) mputs("struct\n");
  	 else if(typ==TYPE_NODE) mputs("node(struct)\n");
	 else if(typ==TYPE_NET) mputs("net\n");
	 else
		{dinod.decl_pt=dpt; dinod.isua=(BOOL)(dpt->size==0);
		base_str(tmp_buff,typ);	kk+=mprintf("(%s)",tmp_buff); nn=num;
		if(isary) {++kk; mputs("{");}
		if(dpt->size!=1) {if(typ&STAR_MASK) typ-=STAR_ONE;}
		do{adrs=indx_adrs((path_elm *)&dinod);
			if(typ&STAR_MASK||((typ&STAR_TYPE)==TYPE_PT)) /* a pt type */
		  	  {if((typ&STAR_MASK)==STAR_ONE&&((typ&STAR_TYPE)==TYPE_CHAR))
			 	 cc=sprintf(tmp_buff,"\"%s\"",*((char **)adrs));
					else cc=sprintf(tmp_buff,"%ld",*((char **)adrs));
		  	  }
			else
		  	  {cc=sprintf(tmp_buff,"%lg",nsl_lst_val((void **)&adrs,(mile)typ));}
			--nn; ++dinod.indx;
			if(kk+cc>sw) {mputs("\n"); kk=0;}
			kk+=cc+1; mputs(tmp_buff);
			if(isary) {if(nn) mputs(","); else mputs("}");}
		  }while(nn);
		dinod.indx=0;
		mputs("\n");
		if(dobrk) break;
		}
	 dpt=dpt->tail;
	}
}
BOOL is_sep(chr) char chr;
{ return((BOOL)(chr==':'||chr=='.'||chr=='/'||chr=='\\'));
}
BOOL is_val_sep(ap) char *ap;
{ return((BOOL)(is_sep(*ap)&&(isalpha(ap[1])||is_sep(ap[1]))));
}
/* build dir path based on cur path & change path dp
*  bf is buffer to put new path string in */
char *get_dir(bf,dp) char *bf,*dp;
{ char *ap=dp,*np;
  if(*dp) {np=ap+strlen(ap); while(np[-1]==';') --np; *np='\0';}
  while(*ap) {if(is_val_sep(ap)) break; ++ap;}
  if(*ap==':') {ap=fcat(bf,dp); return(ap);} /*net path from new net & root*/
  ap=fcat(bf,dir_buff);
  if(!*dir_buff) return(ap); /* no root set yet ! */
  np=bf; while(*np!=':') ++np;
  if(*dp=='\\'||*dp=='/') {ap=fcat(np+1,dp); return(ap);} /*from cur root*/
  loop:
  if(*dp=='.') /* chk for move to parent or cur dir */
	{if(dp[1]=='.')
		{while(ap>np) {if(is_val_sep(ap)) break; --ap;}
		if(*ap==':'||(*ap&&!is_sep(*ap))) ++ap;
		*ap='\0'; dp+=2; goto loop;
		} /* must be cur dir */
	 while(is_val_sep(dp)) ++dp; ap=fcat(ap,dp-1); return(ap);
	}
  while(is_val_sep(dp)) ++dp;
  if(*dp)
	{if(ap[-1]==':') ap=fcat(ap,"\\");
	 else if(!is_val_sep(ap-1)) ap=fcat(ap,"\\");}
  return(fcat(ap,dp));
}

/* display contents of net dir from path */
void do_dir(bp) char *bp;
{ NSLNSLnet *netpt; char *ap,buff[200]; long num;
  bp=get_dir(buff,bp);
  if(!*buff) {mputs("No net selected\n"); return;}
  ap=buff; while(!is_sep(*ap)) ++ap;
  *ap='\0';
  if(!(netpt=nsl_get_p(buff))) return; /* should never happen */
  *ap=':';
  mprintf("%s\n",buff);
  if(!is_sep(bp[-1])) bp=fcat(bp,"\\");
  fcat(bp,"*");
  w_more(mon_wnd,(int)TRUE);
  num=nsl_num_adrs(netpt,(long)(-1),ap+1,(char **)&bp,dsp_dir);
  w_more(mon_wnd,(int)FALSE);
  if(num==0&&sst_is_bad(netpt)) merr("Couldn't open net resource file");
}
void do_cd(ap) char *ap;
{ NSLNSLnet *netpt; char buff[buffsize];
  get_dir(buff,ap);
  if(!*buff) {merr("No net selected"); return;}
  fcat(dir_buff,buff);
  ap=buff; while(!is_sep(*ap)) ++ap;
  *ap='\0';
  if(!(netpt=nsl_get_p(buff))) return; /* should never happen */
  mon_net=netpt;
}
void do_cnet(dp) char *dp;
{ NSLNSLnet *netpt; char *ap; char buff[buffsize];
  ap=fcat(buff,dp); ap[1]='\0';
  while(ap[-1]==';'||ap[-1]==':') {--ap; *ap='\0';}
  netpt=nsl_get_p(buff);
  if(!netpt) {merr("Unknown net"); return;}
  *ap=':';
  fcat(dir_buff,buff);
  mon_net=netpt;
}

void do_dump(wnd,cmd) char *wnd,*cmd;
{ int w=w_handle(wnd);
  if(w<0)
	{if(strcmp(wnd,"all")==0) w=(-1);
	 else
		{if(strcmp(wnd,"auto")==0) mon_dump=TRUE;
		else if(strcmp(wnd,"manual")==0) mon_dump=FALSE;
		return;
		}
	}
  if(!cmd||!*cmd) w_dump(w,NIO_DP_DUMP);
  else if(strcmp(cmd,"close")==0) w_dump(w,NIO_DP_CLOSE);
  else w_dump(w,NIO_DP_OPEN,cmd);
}

void do_fsclose()
{ if(fsfp)
	{fclose(fsfp); fsfp=NULL; 
	 if(mon_fs!=mon_wnd) w_close(mon_fs);
	}
  mon_fs=mon_wnd; *fs_buff='\0'; *fs_text='\0'; fs_line=0; fs_last=0;
}

void do_fsdsp()
{ if(*fs_text) 
	{	fs_text[strlen(fs_text)-1]='\0';
		w_printf(mon_fs,"\n%ld:%s",fs_line,fs_text);
	}
}

BOOL fs_get()
{ BOOL isok=(BOOL)(fgets(fs_text,buffsize,fsfp)!=NULL);
  if(isok) ++fs_line;
  return(isok);
}

BOOL do_fsgo(long lnum)
{ long tmpl=fs_line,fp; *fs_text=(char)0;
  if(lnum<1) lnum=1;
  if(fs_line>lnum||lnum==1) {rewind(fsfp); fs_line=0;} 
  //--lnum;
  fp=ftell(fsfp);
  if(lnum) while(fs_get()) {if(fs_line==lnum) break;}
  if(lnum&&fs_line!=lnum)
	{	mprintf("Only %ld lines in file.\n",fs_line);
		fmoveto(fsfp,fp); fs_line=tmpl;
		return(FALSE);
	}
  do_fsdsp();
  return(TRUE);
}

void do_fssearch()
{ BOOL first=TRUE; long tmpl=fs_line,matl=0,fp; char *ap,*bp,*cp;
  if(!fsfp) return;
  fp=ftell(fsfp); goto jump;
  do{first=FALSE;
	 do{if(fs_line==matl) goto byebye;
		jump: bp=fs_text;
		while(*bp)
		 {ap=fs_buff; cp=bp;
		  while(*ap&&(*ap==(*cp))) {++ap; ++cp;}
		  if(!*ap)
			{if(fs_line!=fs_last) {do_fsdsp(); fs_last=fs_line; return;}}
		  ++bp;
		 }
		}while(fs_get());
	 matl=tmpl; rewind(fsfp); fs_line=0;
	}while(first);
  byebye: fmoveto(fsfp,fp); fs_line=tmpl;
}
void do_fspage(is_up) BOOL is_up;
{ int num=w_y(w_size(mon_fs,TRUE));
  if(!fsfp) return;
  if(is_up) 
  {	if(do_fsgo(1+fs_line-2*num)) --num; else return;}
  while(--num>=0)
	{if(fs_get()) do_fsdsp(); else break;}
}

void do_fsopen(name) char *name;
{	do_fsclose();
	if((fsfp=fopen(name,"r+"))==NULL) return;
	mon_fs=w_open("view",TRUE,TRUE,FALSE,TRUE,TRUE,mon_fx,mon_fy,wnd_black,wnd_white);
	if(mon_fs) {name=strsave(name); w_title(mon_fs,NIO_TC,NIO_DRAW,TRUE,name);}
	else mon_fs=mon_wnd;
	fs_line=0;
	do_fspage(FALSE);
}

/* check network installed */
void chk_ins()
{ NSLNSLnet **npt=nsl_net_list; int jj; char nbuf[10];
  *dir_buff='\0'; 
  jj=lst_ok(nsl_net_list);
  if(!mon_net)
	{while(jj--) {if(*npt) {mon_net=(*npt); break;} ++npt;}	}
  else if(nsl_net_list)
	{while(jj--) {if(*npt==mon_net) goto byebye; ++npt;}
	 if(nsl_install(mon_net,(char *)NULL)==NSLER_OK) goto byebye;
	 if(nsl_install(mon_net,"network")==NSLER_OK) goto byebye;
	 jj=0;
	 while(jj<100)
		{sprintf(nbuf,"N%d",jj++);
		if(nsl_install(mon_net,nbuf)==NSLER_OK) goto byebye;
		}
	}
  else nsl_install(mon_net,(char *)NULL);
  byebye: if(mon_net) do_cnet(mon_net->ins_name);
}

/* clears main mon stacks */
void clear_mon()
{ int num;
  if(glo_list) /* free global variables */
	{glo_free(glo_list); lfree(glo_list); glo_list=NULL;}
  if(fun_list) /* free user functions */
	{num=lst_num(fun_list); while(num--) free_func(fun_list+num);
	 lfree(fun_list); fun_list=NULL;}
  if(fly_list)
	{num=lst_num(fly_list); while(num--) zap_file(num);
	 lfree(fly_list); fly_list=NULL;}
  was_clear();
  if(user_list)
	{lfree(user_list); user_list=NULL;}
  if(tmpfp) {fclose(tmpfp); tmpfp=NULL;}
  if(logfp) {fclose(logfp); logfp=NULL;}
  if(cmdfp) {fclose(cmdfp); cmdfp=NULL;}
  if(fsfp) {fclose(fsfp); fsfp=NULL;}
}
/* clear io, cmd & arg stks upon a user interrupt etc */
void clear_stdio()
{ int num; 
  if(farg_stk) /* free file arg stack */
	{num=lst_ok(farg_stk); while(num--) pop_farg();
	 lfree(farg_stk); farg_stk=NULL;}
  if(uff_stk) /* user func eval stk */
	{num=lst_ok(uff_stk); while(num--) pop_uff();
	 lfree(uff_stk); uff_stk=NULL;}
  if(exp_stk) /* expression eval stack */
	{num=lst_ok(exp_stk); while(num--) restore_exp();
	 lfree(exp_stk); exp_stk=NULL;}
  lfree(fsk_stk); fsk_stk=NULL;
  lfree(state_stk); state_stk=NULL; /* cmd stack */
  lfree(par_stk); par_stk=NULL; /* parameter args stack */
  lfree(par_ofs); par_ofs=NULL;
}
/* set up some mon vars */
void init_stdio()
{ clear_stdio();
  nsl_err=NSLER_OK; nsl_stop=0; nsl_abort=0; nsl_verbatim=1;
  *fs_buff='\0'; *fs_text='\0'; fs_line=0; fs_last=0;
  cmd_pat=FALSE; cmd_exe=FALSE; cmd_exit=0; cmd_brk=FALSE; cmd_yacc=FALSE; cmd_const=NULL;
  w_more(mon_wnd,(int)FALSE);
  reset_io(stdin,0); push_state((int)0); push_farg();
  yacc_entry=0; yacc_io=get_io_pos(); yacc_lastok=yacc_io;
  rewind(tmpfp); io_tmp_pos=ftell(tmpfp); io_tmp_end=io_tmp_pos;
}
void init_mon()
{ clear_mon();
  add_file((char *)NULL,TRUE);
  add_file("nslmon.tmp",FALSE); tmpfp=infp;
  init_stdio();
  chk_ins();
  mprintf("NeSeL Monitor v1.0\n");
}

void free_mon()
{ do_delete("all");
  w_close(NIO_DEFAULT);
  cir_free();
  clear_mon(); 
  clear_stdio();
}

typedef char (*myusr2cast)();

int nsl_order(name,fpt) char *name; void (*fpt)();
{ user_elm *upt;
  if(name&&fpt)
	{user_list=lmore(user_list,1,TYPE_STRUCT,user_elm);
	 upt=lst_tail(user_list,user_elm);
	 upt->name=name; upt->fpt=(myusr2cast)fpt;
	}
  return(NSLER_OK);
}

void nsl_mon_wnds()
{ if(mon_wnd) return;
  mon_wnd=w_open("monitor",TRUE,TRUE,FALSE,TRUE,TRUE,mon_mx,mon_my,wnd_black,wnd_white);
  mon_wat=w_open("watch",TRUE,TRUE,FALSE,TRUE,FALSE,mon_wx,mon_wy,wnd_black,wnd_white);
  w_title(mon_wnd,NIO_TC,NIO_DRAW,FALSE,"NeSeL Monitor");
  w_title(mon_wat,NIO_TC,NIO_DRAW,FALSE,"Stops & Watches");
  /* reposition wnds & make mon_wnd active */
  if(wnd_std>=0) {w_origin(wnd_std,80,10); w_resize(wnd_std,480,280);}
  if(mon_wat>=0) {w_origin(mon_wat,280,120); w_resize(mon_wat,336,168);}
  if(mon_wnd>=0) {w_origin(mon_wnd,80,340); w_resize(mon_wnd,656,308);}
}

extern int my_exit();

int nsl_monitor(arg) char *arg;
{	int doloop,jmpret;
	nsl_mon_wnds();
	init_mon();
	if(arg&&*arg) cmd_entry=arg; else cmd_entry=NULL;
	if(cmd_order)
	{	cmd_order=FALSE; /* standard system order installs should go here e.g: */
		nsl_order("printf",mvprintf);
	}
	
	do 
	{	doloop=0;
		jmpret = w_setjmp();
		if( jmpret == 0 )
		{
     		do{if(cmd_exit>1) init_stdio(); yacc_cmds();} while(cmd_exit);

		}
		else /* exception jumps here */
		{	/* check if w_exit exception */
			if(w_exit_called) 
			{	mprintf("w_exit called by net. Return code(%d)\n",w_exit_code);
				w_exit_called=0; w_exit_code=0;
				doloop=1;
			}
			else {w_fpcheck(); doloop=1;}
		}
	}
	while(doloop);

  free_mon();

my_exit();

  return(nsl_err);
}


