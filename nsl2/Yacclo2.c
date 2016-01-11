/* Name: yacclo2.c   Type: yacc routines file   Date: 30 March 89
*   NeSeL Neural Net Specification Language Preprocessor
*/
#include "globals2.h"
#include "iolo2.h"
#include "iohi2.h"
#include "nial2.h"
#include "nsltwo2.h"
#include "pathlo2.h"
#include "pathhi2.h"
#include "nslyacc2.h"
#include "ytab2.h"

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
#define ASIEQ 11

mile str_cir_max=10, /* default max size of strings buffer */
  str_cir_pt;

int yacc_calls=0, /* recursion semiphore for yacc_parse */
  yacc_entry, /* tells yacc what rule to start from if set */
  yacc_error; /* if error occurs report it as this type */

char
  **str_cir_bs=NULL, /* base of strings buffer */
  *yacc_io, /* base of cur str being parsed */
  *yacc_lastok,
  *yacc_ioerr; /* pos in str at which err occurred */

NSLNSLfval zerval={TYPE_INT,0};

void cir_free()
{ str_cir_pt=lst_ok(str_cir_bs);
  while(str_cir_pt--) chk_free(str_cir_bs[str_cir_pt]);
  lfree(str_cir_bs);
}

char *cir_save(ap)
  char *ap;
{ if(!str_cir_bs)
    {str_cir_bs=lmore(str_cir_bs,str_cir_max,TYPE_PT,char *);
     str_cir_pt=lst_num(str_cir_bs);
     while(str_cir_pt--) str_cir_bs[str_cir_pt]=NULL;
     str_cir_pt=0;
    }
  if(str_cir_bs[str_cir_pt]) fremem(str_cir_bs[str_cir_pt]);
  str_cir_bs[str_cir_pt++]=(ap=strsave(ap));
  if(str_cir_pt>=lst_num(str_cir_bs)) str_cir_pt=0;
  return(ap);
}

/* get user defined function internal code number */
/* returns 0 in *pospt if unknown user func */
int funcky_token_num(pospt,typpt)
  int *pospt,*typpt;
{ int pos,ftype;
  ++infunc; save_exp();
  if((pos=find_name(cur_ident,nsl_mon_names,nsl_num_mon_list))>0)
    {par_str=nsl_mon_list[pos-1].ats;
     ftype=cast_type(*par_str); ++par_str; *pospt=pos; *typpt=ftype;
     if(ftype&STAR_MASK || ftype==TYPE_PT) return(PUNKY);
    }
  else {*pospt=0; *typpt=TYPE_DOUBLE; par_str=NULL;}
  return(FUNKY);
}

void *my_dumy()
{return((void *)0L);
}

char *funcky_pt()
{ int pos;
  if((pos=find_name(cur_ident,nsl_mon_names,nsl_num_mon_list))>0)
    return((char *)(nsl_mon_list[pos-1].fn_pt));
  return((char *)(my_dumy));
}

void push_null()
{ char *vpt,*ept,*npt=NULL; int top,siz=sizeof(char *);
  top=lst_ok(par_stk);
  par_stk=lmore(par_stk,siz,TYPE_CHAR,char);
  ept=lst_head(par_stk,char)+top; vpt=(char *)&npt;
  while(siz--) *ept++ = *vpt++;
}

/* evaluate a user func which returns a value in func_rr */
void user_func(fnum)
  int fnum;
{ NSLNSLfelm *fpt;
  --infunc;
  if(fnum>0 && fnum<nsl_num_mon_list)
    {fpt=nsl_mon_list+fnum-1;
     if(*par_str&&*par_str!=STAR_VARG)
       path_error(PER_GEN,path_pt,path_pt,WRN_USR_NAM,nsl_mon_names[fnum-1]);
     push_null();
     nsl_func_rr=nsl_mon_evaluate(lst_head(par_stk,char),fpt->fn_pt,fpt->swt);
    }
  else signal_error(EXIT_INT_ERR,143);
  restore_exp();
}
/************************************************************************/
/* convert str integer to a binary long */
long tobinary(b)
  int b;
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

/* convert str reals to binary reals
*  on entry s pts to '.' of float
*/
double doubin(s)
  char *s;
{ char chr,*lex_str; double dvalue=0.0;
  do{++s; chr=(*s);
     if(chr=='e' || chr=='E')
       { ++s; chr=(*s); if(chr=='+' || chr=='-') {++s; chr=(*s);} }
    } while(chr<='9' && chr>='0');
  lex_str=get_io_pos();
  *s='\0'; sscanf(lex_str,"%lf",&dvalue);
  *s=chr; set_io_pos(s);
  return(dvalue);
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
/* converts inline */
void dqstr_to_mem(apt)
  char *apt;
{ char *cpt=apt;
  ++apt;
  while(*apt && (*apt!='"')) {*cpt=codetoasc(&apt); ++cpt;}
  *cpt='\0';
}

/* some handy defs to save typing...*/
#define cs(nn,mm) case nn:yylval.sval=mm; return(nn)
#define cscc(nn,mm,pp,qq) case nn:if(*lex_str==nn)\
        {input_char();yylval.sval=pp;return(qq);}yylval.sval=mm;return(nn)
#define cseq(nn,mm,pp,qq,rr) case nn:\
        if(*lex_str=='='){input_char();yylval.ival=rr;return(EQOP);}\
        if(*lex_str==nn){input_char();yylval.sval=pp;return(qq);}\
        yylval.sval=mm;return(nn)

int yylex()
{ char chr,*s,*lex_str; int val='\0';
  if(yacc_entry) {val=yacc_entry; yacc_entry=F; return(val);}
  for(;;)
    {if(!(chr=skip_blanks())) return('\0'); /* end of expression */
     lex_str=get_io_pos();
     yacc_lastok=lex_str-1;
     switch(chr) /* chk sels, lops, 2 chr ops & 1 chr ops */
       {case '!':
          if(*lex_str=='=') {input_char(); yylval.sval="!="; return(NE);}
          yylval.sval="!"; return('!');
        case '"': /* a string */
          unput_char(chr); chr=skip_quoted();
          s=get_io_pos(); chr=(*s); *s='\0';
          yylval.sval=cir_save(lex_str-1); *s=chr;
          if(infunc) dqstr_to_mem(yylval.sval);
          return(DQSTR);
        cs('#',"#");
        case '$': /* chk selectors */
          chr=(*lex_str); selec=NULL;
          if(chr>=LOOP_FOR && chr<=LOOP_EXIT)
            {yylval.ival=(int)chr; input_char(); return(LOOPOPR);}
          if(chr==SEL_FIXPTH)
            {input_char(); input_char();
             if(*(lex_str+1)=='I') val=FIXPATH; else val=STRPATH;
             yylval.sval=(lex_str+2); skip_pair('(',')');
             return(val);}
          chr&=0xdf; /* mask selector dirty bit in type code*/
          if(chr==SEL_FIDENT)
            {input_char(); get_ident(); lex_str=get_io_pos();
             if(*lex_str!='(')
               {yylval.fval.val.pp=funcky_pt(); yylval.fval.typ=TYPE_PT;
                return(NUMBER);}
             return(funcky_token_num((int *)&yylval.sival.indx,
                     (int *)&yylval.sival.sel));
            }
          if(chr==SEL_VIDENT)
            {input_char(); get_ident(); yylval.sval=lex_str+1; return(DOLVAR);}
          if(chr>=SEL_EXCEXC && chr<=SEL_EXCAT)
            {yylval.ival=(int)chr; input_char();
             if(flags.pamat) selec=lex_str; /*save sel pt for bit twiddling*/
             return(DOLEXC);}
          if(chr>=SEL_FIDENT && chr<=SEL_HASH)
            {basic_sel: /*note SEL_EXC etc trapped above*/
             yylval.sival.sel=(int)chr; /* fetch sel code & coor type*/
             if(chr==SEL_EXC) val=DOLPAR; else val=DOLSEL;
             chr=(*(lex_str+1));
             extsel: input_char();
             if(chr>=SIN_DEF) {input_char(); yylval.sival.indx=(int)chr;}
             else yylval.sival.indx=SIN_DEF;
             if(flags.pamat) selec=lex_str;
             return(val);
            }
          if(chr==SEL_XS) /* handle extended selectors */
            {input_char(); chr=(*(lex_str+1));
             if(chr==SEL_XS_TYPE) /* Note special elm type cast follows: */
               {/* nsl1 converts all pt casts to TYPE_PT and then: */
                /* STAR_ONE to vert/horiz mode, STAR_TWO to list pair bit */
                yylval.ival=cast_type(*(lex_str+2))|STAR_CAST;
                input_char(); input_char();
                return(ELMCAST);
               }
             else if(chr==SEL_XS_NODE) {++lex_str; goto basic_sel;}
             val=DOLXS; yylval.sival.sel=(int)(*(lex_str+2)); goto extsel;
            }
          yylval.sval="$"; return('$');
        case '%':
          if(*lex_str=='=') {input_char(); yylval.ival=MODEQ;return(EQOP);}
          yylval.sval="%"; return('%');
        cseq('&',"&","&&",ANDAND,ANDEQ);
        case '\'':
          unput_char(chr); chr=skip_quoted();
          s=get_io_pos(); chr=(*s); *s='\0';
          yylval.fval.val.ii=(int)sqchrtobin(lex_str); *s=chr;
          yylval.fval.typ=TYPE_INT;
          return(NUMBER);
        cs('(',"("); cs(')',")");
        case '*':
          if(*lex_str=='='){input_char();yylval.ival=MULEQ;return(EQOP);}
          yylval.sval="*";return('*');
        cseq('+',"+","++",PLUPLU,PLUEQ);
        cs(',',",");
        cseq('-',"-","--",MINMIN,MINEQ);
        case '.':
          if(*lex_str=='.' && *(lex_str+1)!='.')
            {input_char(); yylval.sval=".."; return(DOTDOT);}
          yylval.sval="."; return('.');
        case '/':
          if(*lex_str=='=') {input_char(); yylval.ival=DIVEQ;return(EQOP);}
          yylval.sval="/"; return('/');
        cs(':',":"); cs(';',";");
        case '<':
          if(*lex_str=='=') {input_char(); yylval.sval="<=";return(LE);}
          if(*lex_str=='<')
            {input_char();
             if(*(lex_str+1)=='=')
               {input_char(); yylval.ival=LSEQ;return(EQOP);}
             yylval.sval="<<";return(LS);}
          yylval.sval="<"; return('<');
        case '=':
          if(*lex_str=='=') {input_char(); yylval.sval="==";return(EQ);}
          yylval.ival=ASIEQ; return(EQOP);
        case '>':
          if(*lex_str=='=') {input_char(); yylval.sval=">=";return(GE);}
          if(*lex_str=='>')
            {input_char();
             if(*(lex_str+1)=='=')
               {input_char(); yylval.ival=RSEQ;return(EQOP);}
             yylval.sval=">>";return(RS);}
          yylval.sval=">"; return('>');
        cs('?',"?"); cs('@',"@");
        case '[': /* check lops */
          input_char(); /* step over list code x */
          if(*(lex_str+1)=='$') /* chk for lops [x$Y...] */
            {val=0; chr=(char)((*(lex_str+2))&0xdf);
             if(chr==SEL_XS) /* handle extended lops */
               {chr=(char)((*(lex_str+3))&0xdf);
                if(chr<LOP_QST||chr>LOP_STAR) goto not_xs_lop;
                input_char(); ++lex_str; val=LOP_XS;
               }
             if(chr>=LOP_QST && chr<=LOP_STAR)
               {input_char(); yylval.sival.sel=val+(int)input_char();
                chr=(*(lex_str+3));
                if(chr>=SIN_DEF) {input_char(); yylval.sival.indx=(int)chr;}
                else yylval.sival.indx=SIN_DEF;
                if(flags.pamat) selec=lex_str+2;
                return(LSTOPR);
               }
            } not_xs_lop:
          yylval.sval="["; return('[');
        cs('\\',"\\"); cs(']',"]");
        case '^':
          if(*lex_str=='=') {input_char(); yylval.ival=XOREQ;return(EQOP);}
          yylval.sval="^"; return('^');
        case '_': break;
        cs('`',"`"); cs('{',"{");
        cseq('|',"|","||",OROR,OREQ);
        cs('}',"}"); cs('~',"~");
        default: ; /* just leaves nums & alphas */
       }
     if(chr<='9' && chr>='0')
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
       }
     else val=chr;
     return(val);
    }
}

void free_a_lst(lpt)
  mile **lpt;
{ mile **apt,num;
  if(lpt)
    {num=lst_num(lpt); apt=lpt; while(num--) {lfree(*lpt); ++lpt;}
     lfree(apt);
    }
}

void free_exp(ept)
  exp_elm *ept;
{ exp_elm *apt; mile num;
  if(ept)
    {apt=ept; num=lst_num(ept);
     while(num--)
       {lfree(ept->par_stk); free_a_lst(ept->lst_stk); ++ept;}
     lfree(apt);
    }
}

/* Call yacc parser to parse current input (should be from a str).
*  stype denotes start symbol, serr default error.
*  Results placed in global variables...
*  yacc_calls is a recursion semiphore.
*  returns T if error
*/
boolean yacc_parse(stype,serr,state,ladd,respos,restate)
  int stype,serr,*state; boolean *ladd; char **respos; int *restate;
{ char *tmpyio=yacc_io,*tmpyerr=yacc_ioerr,*tmpsep=seppt,
    *tmpparstk=par_stk,*tmpparstr=par_str;
  exp_elm *tmpexpstk=exp_stk; mile **tmplststk=lst_stk,tmpelm=cur_cast;
  int tmpye=yacc_entry,tmper=yacc_error,tmpra=raise_error,
    tmpinf=infunc,tmpmiss=list_miss,tmpsepstate=sepstate,tmpstate=list_type;
  boolean tmpladd=list_add;
  yacc_entry=stype; yacc_error=serr; raise_error=ERR_OK;
  exp_value.val.dd=0.0; exp_value.typ=TYPE_DOUBLE;
  yacc_io=get_io_pos(); yacc_lastok=yacc_io;
  list_type=LOOP_LST; list_add=F; seppt=yacc_io; sepstate=LOOP_LST;
  selec=NULL; infunc=0; list_miss=0; cur_cast=STAR_PAIR;
  lst_stk=NULL; par_stk=NULL; par_str=NULL; exp_stk=NULL;
  ++yacc_calls;
  if(yyparse()) signal_error(ERR_PAR);
  --yacc_calls;
  yacc_entry=tmpye; yacc_error=tmper; yacc_io=tmpyio; yacc_ioerr=tmpyerr;
  infunc=tmpinf; list_miss=tmpmiss; cur_cast=tmpelm;
  lfree(par_stk); free_a_lst(lst_stk); free_exp(exp_stk);
  exp_stk=tmpexpstk; par_stk=tmpparstk; par_str=tmpparstr; lst_stk=tmplststk;
  *state=list_type; *ladd=list_add; *respos=seppt; *restate=sepstate;
  list_type=tmpstate; list_add=tmpladd; seppt=tmpsep; sepstate=tmpsepstate;
  if(raise_error) return(T);
  raise_error=tmpra;
  return(F);
}

/* evaluate an expression, which must be in brackets '(' exp ')'
*  this is only called by ary_eval() to eval loop exps
*  assumes yyary_pt is set up */
void eval_exp()
{ char *respos; int state,restate; boolean err,ladd;
  if(*get_io_pos()!='(') signal_error(EXIT_INT_ERR,144);
  err=yacc_parse(SEXP,ERR_ILL,(int *)&state,(boolean *)&ladd,
                 (char **)&respos,(int *)&restate);
  if(err) {exp_value.val.dd=0.0; exp_value.typ=TYPE_DOUBLE;}
}

int cond_eval_exp()
{ eval_exp(); return((int)tx_to_d(exp_value));
}
/* evaluate a var expression
*  result in exp_value:exp_type */
void eval_var(str)
  char *str;
{ char *respos; int state,restate; boolean err,ladd;
  ary_elm apt,*tmpary=yyary_pt;
  zap_ary((ary_elm *)&apt); yyary_pt=(ary_elm *)&apt;
  push_io(); set_io_str(str);
  err=yacc_parse(SVAR,ERR_ILL,(int *)&state,(boolean *)&ladd,
                 (char **)&respos,(int *)&restate);
  if(err) {exp_value.val.dd=0.0; exp_value.typ=TYPE_DOUBLE;}
  clear_ary(yyary_pt); yyary_pt=tmpary;
  pop_io();
}

void yyerror(s)
  char *s;
{ char *spt,*ept,chr; int num=0,rest=0;
  yacc_ioerr=yacc_lastok;
  if(flags.wild) return;
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

/* returns T if list types mismatch
*  All list types have the STAR_CAST bit set !! */
boolean neq_list(ltyp,cas)
  mile ltyp,cas;
{ if(!(ltyp&STAR_CAST)||!(cas&STAR_CAST)) signal_error(EXIT_INT_ERR,145);
  return((boolean)((ltyp&STAR_LIST)!=(cas&STAR_LIST))); /* mask out mode bit */
}
/* user list, store each elm, no limit checks */
void lst_ary_user(val,cstype)
  reals val; mile cstype;
{ char *vpt,*ept,*pp,cc,ucc; mile **lpt,mm; int siz;
  short ss,uss; int ii,uii; long ll,ull; float ff; double dd;
  mm=lst_ok(lst_stk);
  if(!mm) lpt=(mile **)&yyary_pt->ary_list; else lpt=lst_stk+mm-1;
  cstype|=STAR_CAST; /*DEBUG1/6/93, force bit*/
  if(*lpt && neq_list(lst_typ(*lpt),cstype))
    {path_error(PER_GEN,path_pt,path_pt,WRN_ARY_TYP,
                (mile)cstype,lst_typ(*lpt));
     cstype=lst_typ(*lpt);
    }
  switch(cstype&STAR_TYPE)
    {case TYPE_PT:
       siz=(int)sizeof(char *); pp=(char *)((long)val); vpt=(char *)&pp; break;
     case TYPE_FLOAT:
       siz=(int)sizeof(float); ff=(float)val; vpt=(char *)&ff; break;
     case TYPE_DOUBLE:
       siz=(int)sizeof(double); dd=(double)val; vpt=(char *)&dd; break;
     case TYPE_LONG:
       siz=(int)sizeof(long); ll=(long)val; vpt=(char *)&ll; break;
    case TYPE_USLONG:
       siz=(int)sizeof(long); ull=(unsigned long)val; vpt=(char *)&ull; break;
     case TYPE_INT:
       siz=(int)sizeof(int); ii=(int)val; vpt=(char *)&ii; break;
     case TYPE_USINT:
       siz=(int)sizeof(int); uii=(unsigned int)val; vpt=(char *)&uii; break;
     case TYPE_SHORT:
       siz=(int)sizeof(short); ss=(short)val; vpt=(char *)&ss; break;
     case TYPE_USSHORT:
       siz=(int)sizeof(short); uss=(unsigned short)val; vpt=(char *)&uss; break;
     case TYPE_CHAR:
       siz=(int)sizeof(char); cc=(char)val; vpt=(char *)&cc; break;
     case TYPE_USCHAR:
       siz=(int)sizeof(char); ucc=(unsigned char)val; vpt=(char *)&ucc; break;
     default:
       siz=(int)sizeof(mile); mm=(mile)val; vpt=(char *)&mm; break;
    }
  if(*lpt && lst_siz(*lpt)!=(mile)siz)
    path_error(PER_GEN,path_pt,path_pt,ERR_ARY_MIS,(mile)siz,lst_siz(*lpt));
  else
    {*lpt=(mile *)nsl_lst_more(*lpt,(int)1,(int)cstype,siz);
     ept=lst_tail(*lpt,char);
     while(siz--) *ept++ = *vpt++;
    }
}

void lst_ary_pair(ma,mb,isone,per)
  mile ma,mb; boolean isone,*per;
{ mile **lpt,ary_hi,diff; ary_hi=lst_ok(lst_stk);
  if(!ary_hi) {lpt=(mile **)&yyary_pt->ary_list; ary_hi=yyary_pt->fixval;}
  else {lpt=lst_stk+ary_hi-1; ary_hi=0;}
  if(isone) mb=ma; diff=ma-mb;
  if(ma<0)
    {ma=0; if(!*per) path_error(PER_GEN,path_pt,path_pt,ERR_ILL_SEL);
     *per=T;}
  if(mb<0)
    {mb=0; if(diff && !*per) path_error(PER_GEN,path_pt,path_pt,ERR_ILL_SEL);
     *per=T;}
  /* ary_hi=yyary_pt->fixval; */  /*DEBUG DJRW 12/91, cur size limit */
  if(ary_hi>0 && !infunc) /* a zero fixval means ary elms are unbounded */
    {--ary_hi;
     if(ma>ary_hi)
       {if(!*per) path_error(PER_GEN,path_pt,path_pt,ERR_ARY_ORG,ma,ary_hi);
        *per=T; ma=ary_hi;}
     if(mb>ary_hi)
       {if(diff && !*per)
          path_error(PER_GEN,path_pt,path_pt,ERR_ARY_ORG,mb,ary_hi);
        *per=T; mb=ary_hi;}
    }
  add_map_res(lpt,ma,mb);
}

/* Add list elm to list ary. */
void lst_ary_list(a,b,cstype,isone)
  reals a,b; mile cstype; boolean isone;
{ boolean per=F; if(list_miss) return;
  if(cstype==STAR_PAIR) lst_ary_pair((mile)a,(mile)b,isone,&per);
  else {lst_ary_user(a,cstype); if(!isone) lst_ary_user(b,cstype);}
}

/* Add a list to list ary. If scrap is set, it free's a_list mem space */
void lst_ary_ary(a_list,scrap,cstype)
  mile *a_list; boolean scrap; mile cstype;
{ mile **lpt,*apt,a_num,b_num,atyp,al,bl;
  boolean per=F;
  if(a_list && !list_miss)
    {a_num=lst_num(a_list);
     atyp=lst_typ(a_list); atyp|=STAR_CAST;/*DEBUG1/6/93*/
     if(a_num)
       {/* check list types match here */
        b_num=lst_ok(lst_stk);
        if(!b_num) lpt=(mile **)&yyary_pt->ary_list; else lpt=lst_stk+b_num-1;
        if(*lpt) cstype=lst_typ(*lpt);
        cstype|=STAR_CAST; /*DEBUG1/6/93, force bit */
        if(cstype==STAR_PAIR) /* add to a pair list with checks */
          {apt=lst_head(a_list,mile);
           if(atyp&LST_PBIT)
             while(a_num>0)
               {al=(mile)nsl_lst_val(&apt,atyp);
                bl=(mile)nsl_lst_val(&apt,atyp);
                lst_ary_pair(al,bl,F,&per); a_num-=2;}
           else
             while(a_num>0)
               {al=(mile)nsl_lst_val(&apt,atyp);
                lst_ary_pair(al,(mile)0,T,&per); --a_num;}
          }
        else /* add to user list, no checks */
          {
#if 0 /* old ver */
		   char *ept,*vpt;int siz;  
		   siz=(int)lst_siz(a_list);
           if(*lpt)
             {if(neq_list(cstype,atyp))
                {path_error(PER_GEN,path_pt,path_pt,WRN_ARY_TYP,atyp,cstype);
                 siz=(int)lst_siz(*lpt);
                }
              b_num=lst_num(*lpt);
             }
           else b_num=0;
           *lpt=(mile *)nsl_lst_more(*lpt,(int)a_num,(int)cstype,siz);
           ept=lst_head(*lpt,char); ept+=b_num*siz;
           vpt=lst_head(a_list,char); siz*=a_num;
           while(siz--) *ept++ = *vpt++;
#endif
		   /* new: converts list to same type as user list */
		   reals val;
		   apt=lst_head(a_list,mile);
		   while(a_num>0)
		     {val=(reals)nsl_lst_val(&apt,atyp);
		      lst_ary_user(val,cstype); --a_num;
		     }
		   /* new end */
          }
       }
    }
  if(scrap && a_list) lfree(a_list);
}

void eval_push(apt,state,sofs,bnum,val,id)
  ary_elm *apt; int state,sofs,bnum; NSLNSLfval val; char *id;
{ eval_elm *ept;
  apt->eval_list=lmore(apt->eval_list,1,TYPE_STRUCT,eval_elm);
  ept=lst_tail(apt->eval_list,eval_elm);
  ept->state=state; ept->str_ofs=sofs; ept->bsary_num=bnum;
  ept->val=val; ept->discan=F; ept->priscan=F;
  if(id) ept->id=1+(int)(id-apt->ary); else ept->id=0;
}

/* remove eval state info from eval list */
boolean eval_pop(apt)
  ary_elm *apt;
{ eval_elm *ept;
  if(!lst_ok(apt->eval_list)) return(F); /* eol ?? */
  loop:
  ept=lst_tail(apt->eval_list,eval_elm); --lst_num(apt->eval_list);
  if(ept->discan && ilk_pt) ilk_pt->discan=ept->priscan;
  if(!lst_ok(apt->eval_list)) tidy_eval(apt); /* eol */
  else if(ept->state==LOOP_TNEDI) goto loop; /* skip vars */
  return(T);
}

void eval_scan(apt)
  ary_elm *apt;
{ eval_elm *ept=lst_tail(apt->eval_list,eval_elm);
  if(ilk_pt && !ept->discan)
    {ept->discan=T; ept->priscan=ilk_pt->discan; ilk_pt->discan=T;}
}

/* get the current eval state */
boolean eval_state(apt,state,sofs,bnum,valp)
  ary_elm *apt; int *state,*sofs,*bnum; NSLNSLfval *valp;
{ eval_elm *ept;
  if(!lst_ok(apt->eval_list)) return(F); /* eol ?? */
  ept=lst_tail(apt->eval_list,eval_elm);
  *state=ept->state; *sofs=ept->str_ofs; *bnum=ept->bsary_num;
  *valp=ept->val;
  return(T);
}

#define eval_topstat(aa)  (lst_tail(aa->eval_list,eval_elm)->state)
#define eval_updstat(aa,ss) (lst_tail(aa->eval_list,eval_elm)->state=ss)
#define eval_updsofs(aa,ss) (lst_tail(aa->eval_list,eval_elm)->str_ofs=ss)

void eval_updcid(apt,state,sofs,val,id)
  ary_elm *apt; int state,sofs; NSLNSLfval val; char *id;
{ eval_elm *ept=lst_tail(apt->eval_list,eval_elm);
  ept->state=state; ept->str_ofs=sofs; ept->val=val;
  if(id) ept->id=1+(int)(id-apt->ary); else ept->id=0;
}

boolean pop_to_loop(apt)
  ary_elm *apt;
{ int state;
  while(eval_pop(apt))
    {state=eval_topstat(apt);
     if(state==LOOP_ROF||state==LOOP_ELIHW) return(T);
    }
  return(F);
}

/* skip apnd byte, CBREAK cmd, matching [] & stop at single ] ',' or '\0'*/
char skip_list()
{ char chr=input_char();
  switch(chr)
    {case '$': input_char(); break; /* skip CBREAK cmds */
     case '[': unput_char(chr);
     case '0': case '1': case '2': /* apnd code */
       chr=skip_pair('[',']');
       if(chr==']') chr=input_char();
     default:;
    }
  return(chr);
}

/* eval a list,
* return (LEX_OK) if ok, (LEX_ER) err,
* (LEX_SU) suspend, (LEX_AD) suspend but elms added
* apt may already be partially eval'd
* set apt->eval_list to null (& ary_list!=null) etc if eol
* sets resary if ary was reset to ary_num=0 at some point
* note eval relative to path_pt
*/
int ary_eval(apt,resary)
  ary_elm *apt; boolean *resary;
{ boolean err=F,ladd=F,cond; int state,restate,sofs,bnum;
  mile *lpt,anum; NSLNSLfval val;
  char chr,lapnd,*id,*bpt=apt->ary,*spt,*respos; ary_elm *tmpyary=yyary_pt;
  if(apt->suspend) return(2);
  yyary_pt=apt; selec=NULL;
  push_io();
  if(!apt->ary_list) /* first time */
    {if(!bpt || lst_ok(apt->eval_list)) signal_error(EXIT_INT_ERR,148);
     eval_push(apt,LOOP_LST,(int)0,(int)0,zerval,(char *)NULL);
    }
  do{dostate:
     if(eval_state(apt,(int *)&state,(int *)&sofs,(int *)&bnum,
                   (NSLNSLfval *)&val))
       {do{if(!*(bpt+sofs)) goto eolbye; /* eol ? */
           set_io_str(bpt+sofs); lapnd=APND_BEG;
           switch(state)
             {case LOOP_BRK: /* pop to a do, while or for & pop it */
                if(pop_to_loop(apt)) goto poppop;
                tidy_eval(apt); goto byebye;
              case LOOP_CONT: /* pop to a do, while or for */
                if(pop_to_loop(apt)) goto dostate;
              case LOOP_RET:
              case LOOP_EXIT: /* force end of list */
                tidy_eval(apt); goto byebye;
              case LOOP_IDENT: /* stack ident & val, treat floats like ints */
                val.typ=(mile)cast_type(input_char()); /*get ident type code*/
                id=get_io_pos();
                do{chr=input_char();}while(chr!='('); unput_char(chr);
                eval_exp(); val=d_to_t(val.typ,tx_to_d(exp_value));
                if(apt->suspend) goto byebye;
                eval_updcid(apt,LOOP_TNEDI,sofs,val,id);
                goto do_int;
              case LOOP_TNEDI: goto poppop;
              case LOOP_FOR: /* process (e1;e2;e3) first time round */
                chr=input_char(); /* skip '(' */
                eval_exp(); if(apt->suspend) goto byebye;
                chr=input_char(); /* skip ';' */
                spt=get_io_pos(); sofs=(int)(spt-bpt); /* save e2 cond exp pt*/
                cond=(boolean)(cond_eval_exp()>0);
                if(apt->suspend) goto byebye;
                chr=input_char(); /* skip ';' */
                spt=get_io_pos(); /* save e3 inc exp pt */
                val.typ=TYPE_INT; val.val.ii=(int)(spt-bpt);
                eval_updcid(apt,LOOP_ROF,sofs,val,(char *)NULL);
                chr=skip_pair('(',')'); /*skip e3 */
                chr=input_char(); /* skip closing ')'*/
                goto doloop;
              case LOOP_ROF: /* process ...e2;e3)... */
                push_io();
                set_io_str(bpt+val.val.ii);
                eval_exp();  /* eval e3 */
                spt=get_io_pos(); /* pts to closing ')' */
                pop_io();
                if(apt->suspend) goto byebye;
                cond=(boolean)(cond_eval_exp()>0);
                set_io_pos(spt+1); /*skip to closing ')'*/
                goto doloop;
              case LOOP_DO: eval_updstat(apt,LOOP_ELIHW);
                cond=T; goto doloop;
              case LOOP_WHILE: eval_updstat(apt,LOOP_ELIHW);
              case LOOP_ELIHW:
                cond=(boolean)(cond_eval_exp()!=0);
                if(apt->suspend) goto byebye;
                goto doloop;
              case LOOP_FI: /* skip else loop */
                cond=F; goto doloop;
              case LOOP_ELSE:
                 cond=(boolean)(cond_eval_exp()!=0);
                 if(apt->suspend) goto byebye;
                 eval_updstat(apt,LOOP_FI);
                 goto do_else;
              case LOOP_IF:
                cond=(boolean)(cond_eval_exp()!=0);
                if(apt->suspend) goto byebye;
                do_else:
                spt=get_io_pos(); sofs=(int)(spt-bpt);
                eval_updsofs(apt,sofs);
              doloop:
                if(cond) /* do loop list */
                  {chr=input_char(); /* check for CBREAK cmds */
                   if(chr=='$') unput_char(chr);
                   else lapnd=chr; /* get new list apnd type */
                  }
                else /* skip loop list */
                  {eval_pop(apt);
                   chr=skip_list();  /* skip [] & stop at ] */
                   if(chr==']') goto poppop; /* pop state ?? */
                  }
                break;
              default: /* LOOP_LST :  eval list */
                chr=input_char();
                if(chr==']') goto poppop; else unput_char(chr);
             }
           /* get bs ary_num to add at via lapnd */
           lpt=apt->ary_list; anum=lst_ok(lpt);
           switch(lapnd)
             {case APND_BEG:
                if(anum) *resary=T; /*DEBUGFEB91*/
                if(lpt) lst_num(lpt)=0; break;
              case APND_CUR:
                if(bnum<=(int)anum)
                  {if(lpt) lst_num(lpt)=(mile)bnum; if(bnum==0) *resary=T;}
                else /* list under flow */
                  path_error(PER_GEN,path_pt,path_pt,WRN_LST_UNF);
              default: ; /* APND_END leave ary_num as is, append to end */
             }
           do_int:
           if(err=yacc_parse(SARY,ERR_ARY,(int *)&state,(boolean *)&ladd,
                             (char **)&respos,(int *)&restate))
             goto byebye;
           if(apt->suspend)
             {spt=respos; state=restate;} /* state always == LOOP_LST ??*/
           else
             spt=get_io_pos();
           sofs=(int)(spt-bpt);
           if(state==LOOP_LST) /* reached a ']' or eol */
             {if(!*spt) /* eol encountered */
                {eolbye: /* should be one state left*/
                 if(!eval_pop(apt)) signal_error(EXIT_INT_ERR,149);
                 if(eval_pop(apt)) signal_error(EXIT_INT_ERR,150);
                 goto byebye;
                }
              if(!ladd && !apt->suspend)
                goto poppop; /* pop state if didn't add to list */
              /* else fall thru, save state & exit */
             }
           /* push new LOOP state */
           lpt=apt->ary_list; anum=lst_ok(lpt);
           eval_push(apt,state,sofs,(int)anum,zerval,(char *)NULL);
           if(ladd || apt->suspend)
             goto byebye; /* we added some, so return for now */
          }while(state!=LOOP_LST); /* loops if we hit a LOOPOPR */
        poppop:
        if(!eval_pop(apt)) signal_error(EXIT_INT_ERR,151);
       } else err=T;
    }while(!err);
  byebye: pop_io(); yyary_pt=tmpyary;
  if(err) return(LEX_ER);
  if(apt->suspend) {if(ladd) return(LEX_AD); return(LEX_SU);}
  if(!lst_ok(apt->ary_list)) return(LEX_NO);
  return(LEX_OK);
}
/* return storage size of type, if unknown returns size of a pt */
int elm_size(typ)
  int typ;
{ switch(typ)
    {case TYPE_CHAR: case TYPE_USCHAR: return(sizeof(char));
     case TYPE_SHORT: case TYPE_USSHORT: return(sizeof(short int));
     case TYPE_INT: case TYPE_USINT: return(sizeof(int));
     case TYPE_LONG: case TYPE_USLONG: return(sizeof(long));
     case TYPE_FLOAT: return(sizeof(float));
     case TYPE_DOUBLE: return(sizeof(double));
     case SIP_NODE:
     case TYPE_PT: case SIP_STR:
     case TYPE_UNION: case TYPE_NODE: case TYPE_STRUCT:
     case TYPE_VOID: case TYPE_UNKNOWN:
     case TYPE_ARB: case TYPE_NET:
     default: ;
    }
  return(sizeof(char *));
}

/* fetch value of var
*  apply operation opr to it with val
*  if old set return val before adjusting to new val
*  return (-1) if undefined */
NSLNSLfval var_selector(vid,opr,val,old)
  char *vid; int opr; NSLNSLfval val; boolean old;
{ eval_elm *ept; char *ss; NSLNSLfval tmpv,*npt;
  double factor=(double)1.0,oldval=(double)(-1),curval,newval;
  mile *mpt=NULL; int num,typ;
  if(!yyary_pt) signal_error(EXIT_INT_ERR,152);
  if(!yyary_pt->eval_list ||!vid|| !*vid) goto byebye; /*undefined so far?*/
  curval=tx_to_d(val);
  if(*vid=='(') /* it's a selector assignment...*/
    {/* chk sel is one of $!@ or $!# */
     if(flags.state && !(ilk_pt && ilk_pt->curpt->invar))
       {path_error(PER_GEN,path_pt,path_pt,WRN_VAR_SEL); goto byebye;}
     npt= &tmpv; npt->typ=TYPE_INT;
     switch(*(vid+2))
       {case SEL_EXCAT: mpt=(mile *)&ilk_pt->abnum;
           oldval=(double)(*mpt); goto apply;
        case SEL_EXCNUM: mpt=(mile *)&ilk_pt->cxnum;
           oldval=(double)(*mpt); goto apply;
        default:;
       }
     /* assignment not allowed on specified sel */
     path_error(PER_GEN,path_pt,path_pt,WRN_SEL_ASG);
     return(zerval);
    }
  num=lst_ok(yyary_pt->eval_list);
  if(num>0)
    {ept=yyary_pt->eval_list+num-1;
     while(num)
       {if(ept->id) ss=yyary_pt->ary+(ept->id)-1; else ss=NULL;
        if(strideq(vid,ss)) /* got var, now apply op & update val etc */
          {npt=(NSLNSLfval *)&ept->val; oldval=tx_to_d(*npt);
           typ=npt->typ;
           if(typ&STAR_MASK)
             {if(typ&STAR_TWO) factor=(double)sizeof(char *);
              else switch(opr)
                {case PLUEQ: case MINEQ: case PLUPLU: case MINMIN:
                   factor=(double)elm_size(typ&STAR_TYPE);
                 default: ;
                }
             }
           apply:
           newval=oldval;
           switch(opr)
             {case MULEQ: newval*=curval; break;
              case DIVEQ: newval/=curval; break;
              case MODEQ: ((long)newval)%=(long)curval; break;
              case PLUEQ: newval+=curval*factor; break;
              case MINEQ: newval-=curval*factor; break;
              case ANDEQ: ((long)newval)&=(long)curval; break;
              case XOREQ: ((long)newval)^=(long)curval; break;
              case OREQ: ((long)newval)|=(long)curval; break;
              case LSEQ: ((long)newval)<<=(long)curval; break;
              case RSEQ: ((long)newval)>>=(long)curval; break;
              case PLUPLU: newval+=factor; break;
              case MINMIN: newval-=factor; break;
              case ASIEQ: newval=curval; break;
              default: goto skip; /* DOLVAR just return it's val */
             }
           if(mpt) *mpt=(mile)newval;
           else *npt=d_to_t(npt->typ,newval);
           skip:
           if(old) return(d_to_t(npt->typ,oldval));
           return(d_to_t(npt->typ,newval));
          }
        --ept; --num;
       }
    }
  byebye:
  tmpv.typ=TYPE_INT; tmpv.val.ii=(-1);
  return(tmpv);
}

/*eval ary str & add to cur array: note works relative to path_pt & yyary_pt*/
void lst_ary_str(apt,cstype)
  char *apt; mile cstype;
{ ary_elm aelm,*arypt= &aelm; int excode; boolean resary;
  if(apt && *apt)
    {if(is_wild(apt)) return;
     zap_ary(arypt); arypt->ary=strsave(apt); reset_sels(arypt->ary);
     arypt->isdary=yyary_pt->isdary;
     excode=ary_eval(arypt,(boolean *)&resary);
     arypt->pipval=nsl_lst_num(arypt->ary_list);
     if(excode==LEX_OK || excode==LEX_AD)
       lst_ary_ary(arypt->ary_list,F,cstype);
     free_ary(arypt);
    }
}

void add_lst_stk()
{ lst_stk=lmore(lst_stk,1,TYPE_PT,mile *);
  *lst_tail(lst_stk,mile *)=NULL;
}

/* save par_stk & lst_stk
*  set par to first arg, set lst to a new ary */
void save_exp()
{ exp_elm *ept;
  exp_stk=lmore(exp_stk,1,TYPE_STRUCT,exp_elm);
  ept=lst_tail(exp_stk,exp_elm);
  ept->par_stk=par_stk; ept->par_str=par_str; ept->lst_stk=lst_stk;
  ept->list_miss=list_miss; ept->cur_cast=cur_cast;
  par_stk=NULL; par_str=NULL; lst_stk=NULL; list_miss=0; cur_cast=STAR_PAIR;
  add_lst_stk();
}

/* free any lst's; restore old par & lst */
void restore_exp()
{ exp_elm *ept=lst_tail(exp_stk,exp_elm);
  lfree(par_stk); free_a_lst(lst_stk);
  par_stk=ept->par_stk; par_str=ept->par_str; lst_stk=ept->lst_stk;
  list_miss=ept->list_miss; cur_cast=ept->cur_cast;
  --lst_num(exp_stk);
}

/* given a pt to a user list, work out it's size & save in cur lst_stk
*  then inc lst_stk */
void push_lst(mpt)
  mile *mpt;
{ if(!lst_stk) signal_error(EXIT_INT_ERR,153);
  *lst_tail(lst_stk,mile *)=mpt;
  add_lst_stk(); /* now in lst_stk in case more lists added */
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
  mile ta,tb; double dd;
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
  mile ta; double dd;
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

/* push an arg on arg stack */
void push_exp(cstype)
  mile cstype;
{ char *vpt,*ept; size_t siz; mile top=lst_ok(par_stk);
  if(cstype && cstype!=STAR_PAIR)
    nsl_func_rr=d_to_t((mile)(cstype&STAR_TYPE),tx_to_d(nsl_func_rr));
  if(par_str&&*par_str)
    {if(*par_str!=STAR_VARG)
       {nsl_func_rr=d_to_t((mile)cast_type(*par_str),tx_to_d(nsl_func_rr));
        ++par_str;}
    }
  if(nsl_func_rr.typ&STAR_MASK) goto ispt;
  switch(nsl_func_rr.typ&STAR_TYPE)
    {case TYPE_PT: ispt:
       siz=sizeof(char *); vpt=(char *)&nsl_func_rr.val.pp; break;
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
  par_stk=lmore(par_stk,siz,TYPE_CHAR,char);
  ept=lst_head(par_stk,char); ept+=top;
  while(siz--) *ept++ = *vpt++;
}
