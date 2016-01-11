/* Name: yacclo.c   Type: yacc routines file   Date: 30 March 89
*  NeSeL Network  Specification Language Preprocessor
*/

#include "globals.h"
#include "iolo.h"
#include "nslone.h"
#include "general.h"
#include "nslyacc.h"
#include "ytab.h"

extern int recursive_sst();

#define max_yacc_fast 10 /* number of yacc buffers */
#define yacc_fast_size ((unsigned int) 32000) /* size of yacc buff */
#define VAR_MAXS 20 /* max number of substruct levels */
#define MAX_LIDS 100 /* number of sub lists */

extern BOOL isnewd=FALSE,isnode=FALSE,chklsbrk=FALSE,var_mode=FALSE,
	hasand=FALSE,haslong=FALSE,hassel=FALSE;

extern int 
   tmp_int=0,sal_skpt=0,
   yacc_entry=0, /* tells yacc what rule to start from if set */
   yacc_error=0, /* if error occurs report it as this type */
   comm_mode=0, /* tells yylex to tokenise 'in' 'out' & 'inout' */
   optok=0; /* coded oelm comb ops */

extern long tmp_decl_num=0,tmp_aux_num=0,tmp_var_num=0;

extern char 
     *yacc_tmp_pt=NULL,  /* pts to macro arg string during object parsing */
     *yacc_io=NULL, /* pts to start of str to parse */
     *yacc_ioerr=NULL, /* pts to point at which an err was first detected */
     *cint_str=NULL, /* code str for var type (int or double)*/
     sal_lis=0,sal_typ=0,sal_stk[SAL_MAX]={0}, /*stk for lst states during eval*/
     yacc_buff[argbuffsize]={0};

extern reals yacc_exp_value=(reals)0.0;

char *yacc_fast_pt=NULL,*yacc_fast[max_yacc_fast],*yacc_fast_top=NULL;
char *yacc_lastok=NULL,*resy_pt=NULL;
int yacc_fast_num=(-1),
  var_stkpt=0, /* stack pt for decl num when var parsing */
  id_stk_pt=0, /* marks last push pos in id stack */
  id_stk[MAX_LIDS+2]={0};
long var_stack[VAR_MAXS]={0};  /* stack for tmp var decl num */

void sal_push(slis,styp)
  char slis,styp;
{ if(sal_skpt>(SAL_MAX-2)) signal_error(EXIT_SALOVR);
 /* if(!sal_skpt) {slis=(char)0; styp=LIST_ATCON;} */ /*DEBUG 25/5/93*/
  sal_stk[sal_skpt++]=slis;
  sal_stk[sal_skpt++]=styp;
}

void sal_pop(slispt,styppt)
  char *slispt,*styppt;
{ char oldval;
  if(sal_skpt==0) return;
  oldval=sal_stk[--sal_skpt];
  if(oldval>(*styppt)) *styppt=oldval;
  oldval=sal_stk[--sal_skpt];
  (*slispt)|=oldval;
}
/*save a new id in id list */
void save_id(id)
  char *id;
{ if(!id) return;
  id=strsave(id);
  append_list_name(&id_list,id,&id_num,&max_id,inc_id);
}

/* remove some id's from id_list */
void pop_id()
{ int oldtop=0;
  if(id_stk_pt<1 || id_num<0) signal_error(EXIT_INT_ERR);
  oldtop=id_stk[--id_stk_pt];
  while(id_num>oldtop) {--id_num; free(id_list[id_num]);}
}

/* mark id stack pos */
void push_id()
{ if(id_stk_pt>MAX_LIDS) signal_error(EXIT_INT_ERR);
  id_stk[id_stk_pt++]=id_num;
}

void sal_start()
{ sal_push(sal_lis,sal_typ);
  sal_lis&=SIS_LMK; sal_typ=LIST_ATCON; push_id();
}

char *sal_end(spt)
  char *spt;
{ spt=fsav3("[b",spt,"]"); *(spt+1)=sal_typ;
  if(sal_lis&SIS_LIS) *(spt+1)+=LIST_LOFS;
  sal_pop((char *)&sal_lis,(char *)&sal_typ);
  pop_id();
  return(spt);
}

char *loop_end(wpt,spt,ept)
  char *wpt,*spt,*ept;
{ char *apt="[b"; *(apt+1)=sal_typ;
  if(sal_lis&SIS_LIS) *(apt+1)+=LIST_LOFS;
  sal_pop((char *)&sal_lis,(char *)&sal_typ);
  pop_id();
  return(fsav6(apt,wpt,ept,")",spt,"]"));
}

void push_vare(val)
  long *val;
{ if(var_stkpt>VAR_MAXS) /* sub struct level overflow */
    signal_error(EXIT_SUBLVL);
   var_stack[var_stkpt++]= *val; *val=1;
}
void pop_vare(val)
  long *val;
{ *val=1;
  if(var_stkpt>0) *val=var_stack[--var_stkpt];
}
void clr_vare()
{ var_stkpt=0; }

/* convert str integer to a binary long */
long tobinary(b)
  int b;
{ int c,t; long n=0;
  while(c=input_char())
    {switch(c)
       {case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
          t=c-'0'; break;
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
          t=c-'a'+10; if(b>10) break;
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
          t=c-'A'+10; if(b>10) break;
        default: unput_char(c); t=(-1);
       }
     if(t<0) break;
     n=n*b+t;
    }
  return(n);
}

char codetoasc(sppt)
  char **sppt;
{ char *s=(*sppt),chr;
  chr=(*s); ++s;
  if(chr=='\\')
    {chr=(*s); ++s; *sppt=s;
     switch(chr)
       {case '0': return('\0');
        case 'a': return('\a'); case 'b': return('\b');
        case 'f': return('\f'); case 'n': return('\n');
        case 'r': return('\r'); case 't': return('\t');
        case 'v': return('\v'); /* '\xhh' not implemented */
        case '"': return('"');  case '\'': return('\'');
        case '\\': return('\\'); default: return(chr);
       }
    }
  *sppt=s;
  return(chr);
}
/* convert a quoted chr to it's integer val */
int sqchrtobin(s)
  char *s;
{ if(*s=='\'') ++s;
  return((int)codetoasc(&s));
}
/* convert string (with escape codes) to mem string value */
/* converts inline, chops out first & last chrs regardless */
void dqstr_to_mem(apt)
  char *apt;
{ char *cpt=apt,*bpt=apt;
  ++apt;
  while(*apt) {*cpt=codetoasc(&apt); ++cpt;}
  *cpt='\0';
  while(cpt!=bpt) {--cpt; if(*cpt=='"'||*cpt=='\'') break;}
  *cpt='\0';
}

/* convert str double to binary double
*  on entry s pts to '.' of float
*/
reals doubin(s)
  char *s;
{ char chr, *lex_str; reals dvalue=(reals)0.0;
  do{++s; chr=(*s);
     if(chr=='e' || chr=='E')
       { ++s; chr=(*s); if(chr=='+' || chr=='-') {++s; chr=(*s);} }
    } while(chr<='9' && chr>='0');
  lex_str=get_io_pos();
  *s='\0'; sscanf(lex_str,"%lf",&dvalue);
  *s=chr; set_io_pos(s);
  return(dvalue);
}

BOOL chk_equals()
{ BOOL gotequal;
  push_io();
  gotequal=(BOOL)(skip_to_cast('=')=='=');
  pop_io();
  return(gotequal);
}

/* some handy defs to save typing...*/
#define cs(nn,mm) case nn:yylval.sval=mm; return(nn)
#define cscc(nn,mm,pp,qq) case nn:if(*lex_str==nn)\
        {input_char();yylval.sval=pp;return(qq);}yylval.sval=mm;return(nn)

#define cseq(nn,mm,pp,qq,ee,rr) case nn:\
        if(*lex_str=='='){input_char();yylval.sval=ee;return(rr);}\
        if(*lex_str==nn){input_char();yylval.sval=pp;return(qq);}\
        yylval.sval=mm;return(nn)

/* parse a ".id" or "../id" or "../../id" or "..x" or "../..x" string
* On entry ap is io_pos after first dot
* Returns str in yylval.sval & '.' or DOTSEP or DOTDOTSEP or DOTDOTID
*/
int do_dot(ap) char *ap;
{ int val; char chr,sep_buff[32],*sep;
  chr=(*ap);
  if(chr!='.')
    {if(chr=='/'||chr=='\\') {input_char(); yylval.sval="./"; return(DOTSEP);}
     just_dot: yylval.sval="."; return('.');
    }
  if(ap[1]=='.') goto just_dot;
  input_char(); chr=skip_blanks();
  if(chr!='/'&&chr!='\\') /* chk if just a ".." */
    {unput_char(chr); yylval.sval=".."; return(DOTDOT);}
  sep=sep_buff; sep=fcat(sep,"@/");
  for(;;)
    {chr=skip_blanks(); ap=get_io_pos();
     if(chr!='.'||*ap!='.') {val=DOTDOTSEP; break;}
     input_char();
     sep=fcat(sep,"@");
     chr=skip_blanks();
     if(chr!='/'&&chr!='\\') {val=DOTDOTID; break;}
     sep=fcat(sep,"/");
    }
  unput_char(chr); yylval.sval=yacsave(sep_buff);
  return(val);
}

char skip_escape()
{	char chr;	
	loop:
	chr=skip_blanks(); 
	if(chr=='\\') /* some chrs need to be 'escaped' to get past cpp */
	 {	char *s=get_io_pos();
		switch(*s)
		{	case '#': case '%': case '&': case '+': case '@':
            case ';': case '\'': case '"':
			s[-1]=' '; /* valid escaped char. replace '\\' by a space */
			goto loop;
            default:;
		}
	 }
	return(chr);
}

int yylex()
{ char chr,*s,*lex_str; int val;
  if(yacc_entry)
    {val=yacc_entry;
     if(val==NETARGS) /* get a nets macro args */
       {s=yacc_buff; chr=input_char();
        do{*s=chr; ++s; s=fcat(s,get_arg(FALSE)); chr=input_char();
          }while(chr && chr==',');
        fcat(s,")");
        yylval.sval=yacsave(yacc_buff);
       }
     yacc_entry=0; return(val);}
  if(var_mode) {var_mode=FALSE; if(chk_equals()){yylval.sval="="; return('=');}}
  for(;;)
    {lexstart:
     if(!(chr=skip_escape())) return('\0'); /* end of expression */
     lex_str=get_io_pos();
     yacc_lastok=lex_str-1;
     switch(chr) /* chk sels, lops, 2 chr ops & 1 chr ops */
       {case '!':
          if(*lex_str=='=') {input_char(); yylval.sval="!="; return(NE);}
          yylval.sval="!"; return('!');
        case '"': val=DQSTR; doquote: /* a chr def, or string */
          unput_char(chr); s=buffer_on(); chr=skip_quoted(); buffer_off();
          yylval.sval=yacsave(s); return(val);
        cs('#',"#");
        case '$': /* chk selectors */
		  s=get_io_pos(); chr=skip_escape(); lex_str=get_io_pos();
          switch(chr)
            {case '&': s="$G"; goto simon; case '|': s="$H"; goto simon;
             case '^': s="$I"; goto simon; case '_': s="$J"; goto simon;
             case '%': s="$K"; goto simon; case ':': s="$L"; goto simon;
             case '@': s="$M"; goto simon; case '#': s="$N"; goto simon;
             case '*': s="$^A"; goto simon; case '+': s="$^B"; goto simon;
             case '-': s="$^C"; goto simon;
             case '.': if(*lex_str=='.') goto just_dol; s="$^E";
             simon: sal_lop(LIST_ATSEL); break;
             case '!': s=get_io_pos(); chr=skip_escape();
               if(chr=='@') {yylval.sval="$E"; return(DOLEXCEXC);}
               if(chr=='#') {yylval.sval="$D"; return(DOLEXCEXC);}
               if(chr=='!') {yylval.sval="$C"; return(DOLEXCEXC);}
               set_io_pos(s); yylval.sval="$F"; return(DOLEXC);
             case '?': yylval.sval="$?"; return(DOLQUEST);
             case '$': yylval.sval="$$"; return(DOLDOL);
             default: just_dol: set_io_pos(s); yylval.sval="$"; return('$');
            }
          yylval.sval=s;
          return(DOLSEL);
        case '%':
          if(*lex_str=='=') {input_char(); yylval.sval="%=";return(MODEQ);}
          yylval.sval="%"; return('%');
        cseq('&',"&","&&",ANDAND,"&=",ANDEQ);
        case '\'':
          val=SQCHR; goto doquote;
        cs('(',"("); cs(')',")");
        cseq('*',"*","**",STARSTAR,"*=",MULEQ);
        cseq('+',"+","++",PLUPLU,"+=",PLUEQ);
        cs(',',",");
        cseq('-',"-","--",MINMIN,"-=",MINEQ);
        case '.': return(do_dot(lex_str));
        case '/':
          if(*lex_str=='=') {input_char(); yylval.sval="/=";return(DIVEQ);}
          yylval.sval="/"; return('/');
        cs(':',":"); cs(';',";");
        case '<':
          if(*lex_str=='=') {input_char(); yylval.sval="<=";return(LE);}
          if(*lex_str=='<')
            {input_char();
             if(*(lex_str+1)=='=')
               {input_char(); yylval.sval="<<=";return(LSEQ);}
             yylval.sval="<<";return(LS);}
          yylval.sval="<"; return('<');
        cscc('=',"=","==",EQ);
        case '>':
          if(*lex_str=='=') {input_char(); yylval.sval=">=";return(GE);}
          if(*lex_str=='>')
            {input_char();
             if(*(lex_str+1)=='=')
               {input_char(); yylval.sval=">>=";return(RSEQ);}
             yylval.sval=">>";return(RS);}
          yylval.sval=">"; return('>');
        cs('?',"?");
        case '@':
          if(*lex_str=='|') {input_char(); yylval.sval="@|";return(ATPIP);}
          if(*lex_str=='!') {input_char(); yylval.sval="@!";return(ATEXC);}
          if(*lex_str=='?') {input_char(); yylval.sval="@?";return(ATQST);}
          yylval.sval="@"; return('@');
        case '[': /* check lops */
          if(chklsbrk && *(lex_str-2)==')') return(LSBRK);
          s=get_io_pos(); chr=skip_escape();
          if(chr=='*')
            {chr=skip_escape();
             if(chr==']') {yylval.sval="[A$Z]"; return(LSTSTAR);}
             set_io_pos(s); chr='*';
            }
          if((yylval.seval.rev=(BOOL)(chr=='-'))) chr=skip_escape();
          switch(chr)
            {case '&': s="[F$Y"; goto sara; case '|': s="[F$X"; goto sara;
             case '^': s="[F$W"; goto sara; case '@': s="[F$^W"; goto sara;
             case '_': s="[F$V"; goto sara; case '%': s="[F$U"; goto sara;
             case ':': if(*get_io_pos()==':') goto john; s="[F$T"; goto sara;
             case '=': s="[F$S"; goto sara; case '.': s="[F$R"; goto sara;
             case '!': s="[F$Q"; sara: sal_lop(LIST_ATLOP); break;
             case '+': s="[Q$P"; goto cassandra;
             case '?': s="[Q$O"; cassandra: sal_lop(LIST_ATQST); break;
             default: john: set_io_pos(s); yylval.sval="["; return('[');
            }
          yylval.seval.str=s; return(LSTOPR);
        case '\\': /* some chrs need to be 'escaped' to get past cpp */
          switch(*lex_str)
            {case '#': case '%': case '&': case '+': case '@':
             case ';': case '\'': case '"':
             goto lexstart;
             default:;}
          yylval.sval="\\"; return('\\');
        cs(']',"]");
        case '^':
          if(*lex_str=='=') {input_char(); yylval.sval="^=";return(XOREQ);}
          yylval.sval="^"; return('^');
        case '_':
          goto doident;
        cs('`',"`"); cs('{',"{");
        case '|':
          if(*lex_str=='=') {input_char();yylval.sval="|=";return(OREQ);}
          if(*lex_str=='|') {input_char();yylval.sval="||";return(OROR);}
          yylval.sval="|"; s=get_io_pos(); chr=skip_blanks(); set_io_pos(s);
          if(chr=='+'||chr=='-'||chr=='!'||chr=='^'||chr=='<'||chr=='>')
             return(COPBRK);
          return('|');
        cs('}',"}"); cs('~',"~");
        default: ; /* just leaves nums & alphas */
       }
     if(chr<='9' && chr>='0')
       {val=INUMBER; /* a number of some sort */
        s=lex_str; while(*s<='9' && *s>='0') ++s;
        if(*s=='.' && *(s+1)!='.')  /* a double number, but not '..' */
          {unput_char(chr); yylval.dval=doubin(s); return(DNUMBER);}
        else if(chr=='0')
          {chr=input_char();
           if(chr=='x' || chr=='X') {yylval.lval=tobinary(16);}
           else {unput_char(chr); yylval.lval=tobinary(8);}
          }
        else {unput_char(chr); yylval.lval=tobinary(10);}
        chr=(*get_io_pos());
        if(chr=='l'||chr=='L'||chr=='I') {input_char(); val=LNUMBER;}
       }
     else /* see if function like 'sin' 'exp' etc */
      {doident:
       unput_char(chr);
       if(get_ident())
         {if((val=find_name(cur_ident,macro_list,mac_num))>0)
            signal_error(WRN_MAC_UNX,cur_ident); /* didn't expand it..*/
          if(comm_mode)
            {if(strcmp(cur_ident,"in")==0)
               {yylval.ival=COMM_IN; return(COMMTYPE);}
             if(strcmp(cur_ident,"out")==0)
               {yylval.ival=COMM_OUT; return(COMMTYPE);}
             if(strcmp(cur_ident,"inout")==0)
               {yylval.ival=COMM_INOUT; return(COMMTYPE);}
            }
          if(strcmp(cur_ident,"if")==0) return(CIF);
          if(strcmp(cur_ident,"do")==0) return(CDO);
          if(strcmp(cur_ident,"for")==0) return(CFOR);
          if(strcmp(cur_ident,"else")==0) return(CELSE);
          if(strcmp(cur_ident,"while")==0) return(CWHILE);
          if(strcmp(cur_ident,"bin")==0) {yylval.sval="bin"; return(CBIN);}
          if(strcmp(cur_ident,"heap")==0) {yylval.sval="heap"; return(CHEAP);}
          if(strcmp(cur_ident,"pile")==0) {yylval.sval="pile"; return(CPILE);}
          if(strcmp(cur_ident,"break")==0) {yylval.sval="$6"; return(CBREAK);}
          if(strcmp(cur_ident,"return")==0) {yylval.sval="$8"; return(CBREAK);}
          if(strcmp(cur_ident,"continue")==0)
            {yylval.sval="$7"; return(CBREAK);}
          if(strcmp(cur_ident,"exit")==0) {yylval.sval="$9"; return(CBREAK);}
          if((val=find_name(cur_ident,base_list,5))>0)
            {--val; cint_str="$5@"; *(cint_str+2)|=(char)val;
             yylval.sval=base_list[val];
             return(CIDENT);
            }
          chr=skip_blanks();
          if(chr==':') {yylval.sval=yacsave(cur_ident); return(LABEL);}
          unput_char(chr);
          if(chr=='[')
            {push_io(); skip_bodies();
             if(skip_blanks()==':')
               {yylval.sval=yacsave(cur_ident); pop_io(); return(LABELARY);}
             pop_io();
            }
          if((val=find_name(cur_ident,func_list,func_num))>0)
            {s=func_list[val-1]; yylval.sval=s;
             s+=strlen(s)+1;
             if((*s)&STAR_MASK) sal_lis|=SIS_LIS;
             return(FIDENT);}
          else if(search_name(cur_ident,id_list,id_num)>0)
            {yylval.sval=yacsave(cur_ident); return(VIDENT);}
          else if(chr=='(') /* see if net or node */
            {if((val=find_name(cur_ident,module_list,module_num))>0)
               {yylval.sval=module_list[val-1];
                if(stkrec(module_list[val-1])->def_type==DEF_NFUNC)
                  return(NODE);
               }
             else /* must be an undefined func or module */
               {s=strsave(cur_ident); yylval.sval=s;
                add_list_name(&undef_list,s,&undef_num,&max_undef,inc_undef);
                signal_error(WRN_PAR_UDS,s);
               }
             yacc_entry=NETARGS;
             return(NET);
            }
          yylval.sval=yacsave(cur_ident);
          return(IDENT);
         }
       else
         val=input_char();
      }
     return(val);
    }
}


/* This routine saves a string quickly in static mem, rather than calling
*  malloc -- which would lead to mem fragmenting in this application
*/
char *yacsave(str)
  char *str;
{char *a_pt=str,*b_pt=yacc_fast_pt;
  if(!str) return((char *)NULL);
  while(*a_pt)
   {if(yacc_fast_pt>=yacc_fast_top)
      {
       if(yacc_fast_num>=max_yacc_fast)
          signal_error(EXIT_OSPACE,"yacc buffer space");
       if(yacc_fast_pt=yacc_fast[yacc_fast_num]) ++yacc_fast_num;
       else
         {yacc_fast_pt=
            (yacc_fast[yacc_fast_num++]=
              (char *)malloc((unsigned int)yacc_fast_size));
          if(!yacc_fast_pt)
            signal_error(EXIT_OSPACE,"yacsave");
         }
       yacc_fast_top=yacc_fast_pt+yacc_fast_size-4;
       b_pt=yacc_fast_pt; a_pt=str;
      }
    *(yacc_fast_pt++)=(*(a_pt++));
   };
  *(yacc_fast_pt++)='\0';
 return(b_pt);
}

void yacc_clear()
{ int n;
 if(yacc_fast_num<0)
    {for(n=0;n<max_yacc_fast;++n) yacc_fast[n]=(char *)NULL; }
  yacc_fast_top=(char *)NULL; yacc_fast_num=0; yacc_fast_pt=(char *)NULL;
}

/* Call yacc parser to parse current input (should be from a str).
*  Parse a net declaration.
*  Results placed in global variables...
*  returns TRUE if ok
*/
BOOL yacc_parse()
{ int exit_flag;
  comm_mode=FALSE; var_mode=FALSE; isnode=FALSE; chklsbrk=FALSE;
  sal_skpt=0; id_num=0; id_stk_pt=0; sal_lis=(char)0; sal_typ=LIST_ATCON;
  hasand=FALSE; haslong=FALSE;
  yacc_clear();
  yacc_io=get_io_pos(); yacc_lastok=yacc_io; resy_pt=NULL;
  if((exit_flag=yyparse())) if(!raise_error) signal_error(ERR_PAR);
  while(id_num>0) {--id_num; free(id_list[id_num]);}
  yacc_fast_num=0;
  return((BOOL)!exit_flag);
}
BOOL yacc_parse_decl()
{ yacc_entry=DECL; yacc_error=ERR_PAR_NET;
  return(yacc_parse());
}
extern BOOL eolon;
BOOL yacc_parse_exp()
{ BOOL exval; yacc_entry=EXP; yacc_error=ERR_PAR_ARY;
  eolon=TRUE;
  exval=yacc_parse();
  eolon=FALSE;
  return(exval);
}
BOOL yacc_parse_struct()
{ yacc_entry=STRU; yacc_error=ERR_PAR_STU;
  strs_num=0; strs_list=(char **)NULL; max_strs=0;
  return(yacc_parse());
}
BOOL yacc_parse_finit()
{ yacc_entry=FUNCDECL; yacc_error=ERR_PAR_FUN;
  return(yacc_parse());
}
BOOL yacc_parse_comms()
{ yacc_entry=COMMDECL; yacc_error=ERR_PAR_COM;
  return(yacc_parse());
}
BOOL yacc_parse_parms()
{ yacc_entry=NODEPARMS; yacc_error=ERR_PAR_NOP;
  return(yacc_parse());
}

/* Save an in-out decl str.
*  Returns its rec handle.
*/
inouts_elm *save_inouts(s,isnew)
  char *s; BOOL isnew;
{ unsigned int somebytes=4+strlen(s)+sizeof(inouts_elm);
  int len=0; long *listpt; inouts_elm *elm;
  if(!isnew)
   {if(ins_num<1 || strcmp(s,ins_list[ins_num-1])!=0)
      signal_error(EXIT_INT_ERR);
    return(inoutrec( *(ins_list+ins_num-1) ));
   }
  if((elm=(inouts_elm *)malloc(somebytes))==(inouts_elm *)NULL)
    signal_error(EXIT_OSPACE,"saving declaration");
  elm->num=ins_num+1;
  elm->list=(long *)NULL;
  elm->line_num=decl_line;
  enlarge_array(&elm->list,sizeof(long),&len,4);
  listpt= elm->list;
  *listpt=(long)2;
  *(listpt+1)=(long)len;
  *(listpt+2)=(long)0;
  strcpy((char *)&(elm->pat),s);
  append_list_name(&ins_list,(char *)&(elm->pat),&ins_num,&max_ins,inc_outs);
  return(elm);
}

/* Save an input str, if necessary, and update its def with out handle
*/
void save_ins(s,sohand,sotoken,isnew)
  char *s; int sohand,sotoken; BOOL isnew;
{ long *listpt,cur_top,max_top; int val;
  inouts_elm *ihand;
  ihand=save_inouts(s,isnew);
  listpt= ihand->list;
  cur_top= *listpt;
  max_top= *(listpt+1);
  if(cur_top<2) cur_top=2;
  if((cur_top+2)>=max_top)
    {val=enlarge_array(&ihand->list,sizeof(long),&max_top,inc_conxs);
     if(!val)
       signal_error(EXIT_OSPACE,"saving Ins");
     listpt= ihand->list;
     *(listpt+1)=max_top;
    }
  *(listpt+cur_top++)=sotoken;
  *(listpt+cur_top++)=sohand;
  *(listpt+cur_top)=(long)0;
  *listpt=cur_top;
}

/* Save and return an int handle for out decl str
*/
int save_outs(s)
  char *s;
{ unsigned int somebytes=4+strlen(s)+sizeof(outs_elm);
  int pos=(-1); outs_elm *elm;
  pos=find_name(s,outs_list,outs_num);
  if(pos>0)
    {elm=outsrec(outs_list[pos-1]);
     return(elm->num);}
  if((elm=(outs_elm *)malloc(somebytes))==(outs_elm *)NULL)
    signal_error(EXIT_OSPACE,"saving declaration");
  elm->num=outs_num;
  strcpy((char *)&(elm->pat),s);
  add_list_name(&outs_list,(char *)&(elm->pat),&outs_num,&max_outs,inc_outs);
  return(elm->num);
}

void initialise_yacc()
{
  yacc_clear();
}

/* store comms info on func/nety */
void comm_push(sname,comtype)
  char *sname; int comtype;
{ char *apt,*tpt="@";
  (*tpt)=(char)comtype;
  fcat2(yacc_buff,tpt,sname);
  apt=strsave(yacc_buff);
  add_list_name(&comms_list,apt,&comms_num,&max_comms,inc_comms);
}

void parserr()
{ char *spt,*ept,chr; int num=0,rest=0;
  spt=yacc_ioerr;
  while(spt>yacc_io && (*spt=='\n'||*spt==' '||*spt=='\t')) --spt;
  ept=spt+1;
  while(spt>yacc_io && *spt!='\n') {++num; --spt; if(num>(SCR_WD-5)) break;}
  if(*spt=='\n') ++spt;
  rest=num; while(rest<(SCR_WD-1) && *ept!='\n' && *ept) {++rest; ++ept;}
  chr=(*ept); *ept='\0';
  fprintf(logfp,"%s\n",spt);
  *ept=chr;
  while(num>1) {fputc(' ',logfp); --num;} fputs("^\n",logfp);
}

void yyerror(s)
  char *s;
{ yacc_ioerr=yacc_lastok;
}

int resynch(yerr,mess,skip,keep)
  int yerr; char *mess,*skip,*keep;
{ char *apt,chr;
  if(!yacc_ioerr) yacc_ioerr=yacc_lastok;
  if(yerr) signal_error(yerr,mess);
  else if(mess) signal_error(ERR_MESS,mess);
  else signal_error(ERR_MESS,"?");
  parserr();
  if(skip||keep)
    {set_io_str(yacc_ioerr);
     chr=skip_blanks();/* skip chr causing err */
     apt=get_io_pos(); /* force skip if last io err pos was the same */
     if(apt==resy_pt) {chr=skip_blanks(); apt=get_io_pos();}
     resy_pt=apt;
     apt=keep;/* check if need to keep offending char */
     if(apt) while(*apt)
       {if(*apt==chr) {unput_char(chr); return(2);}
        ++apt;
       }
     if(chr=='{'||chr=='('||chr=='[') unput_char(chr);
     while(chr=skip_over())
       {apt=skip;
        if(apt) while(*apt)
          {if(*apt==chr) return(1);
           ++apt;
          }
        apt=keep;
        if(apt) while(*apt)
          {if(*apt==chr) {unput_char(chr); return(2);}
           ++apt;
          }
       }
     signal_error(WRN_RESYN);
    }
  return(0);
}

/* add decl_name to path if required */
char *add_name(dpt)
  char *dpt;
{ char chr=(*dpt);
  if(isident(chr)||chr=='*'||chr=='?')
    {if(!strcmp(dpt,"pile")||!strcmp(dpt,"heap")||!strcmp(dpt,"bin"))
       return(dpt);
     dpt=fsav3(decl_name,"/",dpt);
    }
  else if(chr=='$') dpt=fsav2(decl_name,(dpt+1));
  return(dpt);
}

/* chop off any prefix float or abs path designators */
char *fly_filter(spt)
  char *spt;
{ for(;;)
  {switch(*spt)
    {case '/': case '.': case '~': case '@': ++spt; break;
     default: return(spt);
    }
  }
}
/* put name in list even if already in it */
void mul_list_name(list,name,numpt,amaxpt,ainc)
  char ***list,*name; int *numpt,*amaxpt,ainc;
{ int pos=find_name(name,*list,*numpt);
  if(pos>0)
    {while((pos<*numpt)&&strcmp(name,*((*list)+pos))==0) ++pos;}
  else pos=(-pos);
  insert_hole(pos,list,*numpt,amaxpt,ainc);
  *((*list)+pos)=name; ++(*numpt);
}

void do_con_member(isadd) BOOL isadd;
{ char *apt; int pos;
  if(decl_label)
    {pos=find_name(decl_label,labs_list,labs_num);
     if(pos>0)
       {goto mul_ok; /* DEBUG muldef labels are ok, each scanned in turn */
        add_list_name(&muldef_list,strsave(decl_label),
                      &muldef_num,&max_muldef,inc_muldef);
        signal_error(WRN_PAR_MUL,decl_label);
       }
     else
       {mul_ok:
        if(!isadd) apt=fly_filter(decl_path); else apt=decl_path;
        sprintf(yacc_buff,"%s:%c%s\0",
                decl_label,comm_code(decl_label),apt);
        apt=strsave(yacc_buff);
        *(apt+strlen(decl_label))='\0';
        mul_list_name(&labs_list,apt,&labs_num,&max_labs,inc_labs);
       }
    }
}

char *do_con_path(sor,apt)
  char *sor,*apt;
{ char chr,*wpt,*dpt;
  dpt=add_name(apt);
  if(*dpt=='#')
    apt=fsav2(sor,dpt); /* this is actually a null float */
  else /* insert decl_name between abs tokens & rest of path */
    {wpt=dpt; chr=(*wpt);
     while(chr=='/'||chr=='@'||chr=='.') {++wpt; chr=(*wpt);}
     *wpt='\0'; dpt=fsav2(sor,dpt); *wpt=chr;
     if(chr=='$') {++wpt; if(*wpt=='/') ++wpt;} /* chk for embedded '$' */
     if(*wpt)
       {if(*wpt=='[')  apt=fsav3(dpt,decl_name,wpt);
        else apt=fsav4(dpt,decl_name,"/",wpt); }
     else apt=fsav2(dpt,decl_name);
    }
  return(apt);
}

void syn_decl_do(sotoken,bpt)
  int sotoken; char *bpt;
{ save_ins(decl_path,(int)save_outs(bpt),sotoken,isnewd);
  isnewd=FALSE;
}

char *get_brackets(apt)
  char *apt;
{ while(*apt && *apt!=':') ++apt;
  if(*apt!=':') signal_error(EXIT_INT_ERR);
  *apt='\0';
  return(apt+1);
}

void do_obj_spec(apt,asiz)
  char *apt; int asiz;
{ char ccode,*bpt=get_brackets(apt);
  strcpy(decl_id,apt);
  ccode=comm_code(apt);
  if(asiz<1) {signal_error(WRN_OBJ_SIZ,decl_id); asiz=1;}
  sprintf(decl_name,"#D%02d\0",tmp_decl_num++);
  sprintf(yacc_buff,"%s:%cN%d,%09d,%010d,%s%s\0",
          decl_name,ccode,asiz,0,0,decl_id,bpt);
  add_list_name(&labs_list,strsave(yacc_buff),&labs_num,&max_labs,inc_labs);
}

char *do_aux_spec(apt)
  char *apt;
{ char *cpt,*bpt,ccode,tchr='X'; int pos;
  pos=find_name(apt,aux_list,aux_num);
  if(pos>0) {cpt=aux_list[pos-1]; return(cpt+strlen(cpt)+1);}
  bpt=get_brackets(apt);
  if(recursive_sst(stkrec(apt))) tchr='?';
  sprintf(yacc_buff,"#%c%02d\0",tchr,tmp_aux_num++);
  cpt=yacsave(yacc_buff);
  ccode=comm_code(apt);
  sprintf(yacc_buff,"%s:%cN%d,%09d,%010d,%s%s\0",cpt,ccode,1,0,0,apt,bpt);
  add_list_name(&labs_list,strsave(yacc_buff),&labs_num,&max_labs,inc_labs);
  bpt=fcat3(yacc_buff,apt,":",bpt); pos=strlen(yacc_buff); fcat2(bpt,":",cpt);
  apt=strsave(yacc_buff);
  add_list_name(&aux_list,apt,&aux_num,&max_aux,inc_labs);
  *(apt+pos)='\0';
  return(cpt);
}

reals do_exp_op(xa,xb,ops,bpt,tp)
  xelm *xa,*xb; char *ops; BOOL *bpt; int *tp;
{reals a,b,val; *bpt=FALSE; a=xa->val; b=xb->val;
 *tp=(xa->typ|xb->typ);
 if(xa->bad||xb->bad) {*bpt=TRUE; *tp=0; val=(reals)1.0;}
 else switch(*ops)
  {case '*': val = a * b; break;
   case '/': if(b==(reals)0.0) {val=(reals)1.0; *bpt=TRUE; *tp=0;}
   else val = a / b; break;
   case '%': val = (reals)((long)a % (long)b); break;
   case '-': val = a - b; break;
   case '<': if(ops[1]=='<') val = (reals)((long)a << (long)b);
     else if(ops[1]=='=') val = (reals)(a <= b);
     else val = (reals)(a < b); break;
   case '>': if(ops[1]=='>') val = (reals)((long)a >> (long)b);
     else if(ops[1]=='=') val = (reals)(a >= b);
     else val = (reals)(a > b); break;
   case '=': val = (reals)(a == b); break;
   case '!': val = (reals)(a != b); break;
   case '&': if(ops[1]=='&') val = (reals)((long)a && (long)b);
     else val = (reals)((long)a & (long)b); break;
   case '^': val = (reals)((long)a ^ (long)b); break;
   case '|': if(ops[1]=='|') val = (reals)((long)a || (long)b);
     else val = (reals)((long)a | (long)b); break;
   default:  val = a + b;
  }
 return(val);
}

char *num_asc_str(np) xelm *np;
{ char *apt;
  if(np->typ==0) sprintf(yacc_buff,"%d",(int)np->val);
  else if(np->typ<2) sprintf(yacc_buff,"%dL",(long)np->val);
  else
  {sprintf(yacc_buff,"%g",(double)np->val); apt=yacc_buff;
   while(*apt) {if(*apt=='.') break; ++apt;}
   if(*apt!='.') fcat(apt,".0");
  }
 return(yacsave(yacc_buff));
}