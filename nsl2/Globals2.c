/* Name: globals2.c   Type: general c routines   Date: 30 March 89
*   NeSeL Neural Net Specification Language Preprocessor
*/
#include <varargs.h>
#include "globals2.h"
#include "iolo2.h"
#include "iohi2.h"
#include "nial2.h"
#include "nsltwo2.h"
#include "pathlo2.h"
boolean err_skip=F;
int firsterr=1,err_ef=0,err_el=0,err_sel=0;
path_elm *err_pt; boolean err_usein;
char *err_aa=NULL,*err_bb=NULL,*err_cc,err_b1[300],err_b2[300];
struct error_elm {int error_num; char *error_str;};
struct error_elm error_list[]={
{WRN_NET_PAR,"$!x used on a constant declaration or net, taking fixed size"},
{WRN_NET_EXC,"$!@,$!# or $!! used on constant decl or net, taking fixed size"},
{WRN_CXIN_TYPE,"dest conx type not a pointer:%s"},
{WRN_CXIN_TMIS,"conx type mismatch: source{c}(%s)-->dest{c*[*]}(%s)"},
{WRN_CXIN_OWRT,"new conx would overwrite present one at[%d], buffered it ok?"},
{WRN_CXIN_NEQU,"conx at dest not same as specified conx to delete"},
{WRN_CXIN_NTCX,"dest not a conx, type(%s)"},
{WRN_CXIN_DIVR,"conx diverted to bucket, dest wrong type"},
{WRN_CXIN_DIDE,"conx diverted to bucket & not deleted from dest"},
{WRN_CXIN_UNS,"some conxs in conx-elm not used"},
{WRN_CX_NEX,"no conx exists at specified leaf, ignored"},
{WRN_VAR_UNK,"unknown type cast '%s' processed anyway ok?"},
{WRN_VAR_TMS,"node cast '%s' applied to different type '%s'"},
{WRN_VAR_STR,"node cast '%s' applied to struct (%s)'%s'"},
{WRN_VAR_TYP,"var type mismatch '%s' '%s'"},
{WRN_VAR_TST,"struct type mismatch '%s' (%s)'%s'"},
{WRN_VAR_LVL,"data-struc and it's initialiser level mismatch"},
{WRN_VAR_UNI,"unions currently not supported"},
{WRN_VAR_CNX,"tried to initialise a conx, ignored"},
{WRN_VAR_NOD,"null struct {}, struct expected next in var decl"},
{WRN_VAR_ARY,"array cast not expected in var decl"},
{WRN_VAR_ATM,"atomic value expected in var decl"},
{WRN_VAR_FAIL,"var-path failed to reach node, decl ignored ok?"},
{WRN_VAR_MIS,"atomic value expected in var decl, initialiser member missing?"},
{WRN_AUX_MIN,"tried to subtract an aux-path, ignored ok?"},
{WRN_AUX_UND,"aux net '%s' undefined, ignored in out-path ok?"},
{WRN_FLO_OVR,"floated in-path above root, taking root"},
{WRN_OUT_OFL,"floated out-path above root, taking root"},
{WRN_QST_ZER,"[?] LOP, nothing to append, taking [0..0] ok?"},
{WRN_LOP_ZER,"LOP evals to nothing, taking [0..0] ok?"},
{WRN_LOP_NUL,"LOP returned null list [], check UA usage"},
{WRN_UAL_NUL,"access to unused UA, automatically initialised to null ok?"},
{WRN_FRK_NUL,"no conxs found in fork-path"},
{WRN_OEL_NUL,"no conxs found in out-path"},
{WRN_PAR_EMP,"no leaves found in partition, taking 0 ok?"},
{WRN_PAR_ZER,"[+] LOP, no leaves in a partition, treating as single leaf ok?"},
{WRN_PAR_EXD,"[+] LOP, partition level not exceeded, taking one leaf ok?"},
{WRN_LST_UNF,"prior list level underflow, elms added to end ok?"},
{WRN_DS_NARB,"data-struc member referenced wasn't an array, taking 0 ok?"},
{WRN_VAR_SEL,"$! selectors not allowed in var decls, taking -1 ok?"},
{WRN_VAR_OUT,"no out-path exists for var decls, taking cur in-path ok?"},
{WRN_INL_VAR,"inline var: out-path eval'd already, taking null-path ok?"},
{WRN_NO_COMMS,"%sput communications not specified for '%s', ignored ok?"},
{WRN_SEL_UNL,"selector not eval'd yet when needed by lop, taking 0 ok?"},
{WRN_SEL_OOR,"selector level out of range, taking -1 ok?"},
{WRN_SEL_BUN,"couldn't bypass [*], taking null-path ok?"},
{WRN_SEL_SER,"selector references its own list ok?"},
{WRN_SEL_REC,"selector eval leads to infinite regress, taking -1 ok?"},
{WRN_SEL_DAR,"selector ds_op '.' failed to find base-ary, taking lab-ary ok?"},
{WRN_SEL_ASG,"only $!# and $!@ selectors can be assigned to, taking 0 ok?"},
{WRN_BAD_OPTH,"conx-elm was bad, ignored it ok?"},
{WRN_USR_NAM,"different number of arguments to user function '%s'"},
{WRN_USR_FUN,"user function '%s' evaluation failed, check 'nslusrfn.c'"},
{WRN_TIL_UNF,"'~' or '~~' used on conx-elm before in-path known, ignored ok?"},
{WRN_PRT_OOR,"selector partition number out of range, taking -1 ok?"},
{WRN_ELM_OOR,"decl index out of range %d>%d, skipping ok?"},
{WRN_ELM_ABR,"decl index array aborted, too many elms out of range"},
{WRN_LRY_BYP,"couldn't bypass a labdef-ary, ignoring ok?"},
{WRN_LRY_HTB,"had to bypass a labdef-ary"},
{WRN_NODE_NOIO,"node(s) with no input or output conxs found:"},
{WRN_SFLO_OFL,"tried to float selector path above root, taking root ok?"},
{WRN_STIL_UNF,"selector %s-path not eval'd yet, taking cur path ok?"},
{WRN_SNO_TAIL,"selector path request beyond tail, taking tail ok?"},
{WRN_BIN_CX,"can't get conx from bin, ignored request ok?"},
{WRN_STK_NTEM,"%s not empty(%d), deleting conxs ok?"},
{WRN_STK_EMPT,"%s empty, conx expected, ignored request ok?"},
{WRN_STK_IOOR,"%s index(%d) out of range, taking highest ok?"},
{WRN_ARY_TYP,"mixing types in list: '%s' [%s], converting to base type ok?"},
{WRN_GLO_UNF,"global conx label '%s' not defined, taking original net ok?"},
{WRN_INT_ERR,"possible internal err?"},
{ERR_ILL ,"badly formed expression, returns 0.0"},
{ERR_PAR ,"yacc couldn't parse it correctly"},
{ERR_YER ,"yacc error occurred"},
{ERR_ARY ,"error evaluating array"},
{ERR_TOO ,"too many user functions defined"},
{ERR_DEF ,"function already defined"},
{ERR_ARG ,"too many args to function"},
{ERR_MEM ,"not enough memory to install function"},
{ERR_OPTH_NAM,"name expected in path"},
{ERR_LAR_BAD,"lab-ary elm (at %d) not in map list"},
{ERR_TER_OPTH,"path terminated prematurely"},
{ERR_NON_OPTH,"no matching path found"},
{ERR_ARY_ORG,"list elm out of range:%d[%d]"},
{ERR_ARY_MIS,"list elms size mismatch:%d[%d]"},
{ERR_ILL_SEL,"negative values (e.g. undef selectors) illegal in path lists"},
{ERR_NO_MAT,"no matching path found"},
{ERR_ATM_CON,"atom initialised and receives an input"},
{ERR_UA_ENC,"unbound array encountered unexpectantly!"},
{ERR_NET_CHK,"internal consistancy of net is suspect"},
{ERR_CXIN_INDX,"source conx index out of range:%d[%d]"},
{EXIT_INT,"exit on interrupt"},
{EXIT_BADFILE,"unable to open input/output file '%s'"},
{EXIT_OSPACE,"ran out of memory space: %s"},
{EXIT_EOF,"EOF encountered prematurely"},
{EXIT_PARSE,"non recoverable parsing error occurred"},
{EXIT_ROOT,"root network not found"},
{EXIT_FILE,"access to file '%s.nss' went wrong"},
{EXIT_DSP_FUL,"data structure queue overflow"},
{EXIT_INT_ERR,"internal error(%d)?!"},
{EXIT_INT_ARY,"internal errror?!: tried to access null array"},
{EXIT_CONX,"internal error: node DS conx account inconsistent"},
{EXIT_REC_MOD,"module was illegally recursively defined"},
{EXIT_VERSION,"nsl1/nsl2 version number mismatch"},
{EXIT_NOGO,"feature not implemented"},
{EXIT_BADNSS,"bad nss file:%s"},
{ERR_OK	,"@ok"},  /* same code as EXIT_OK */
{0,NULL}};

#define MAX_TYPES 21
char *base_list[]={"?",
  "char","double","float","int","long",
  "void","node","struct","union","short",
  "unsigned char","unsigned int","unsigned long","unsigned short",
  "pointer","net","array","string","connection","connection"};
char base_one[32],base_two[32];
boolean base_flag=0;

void mem_free(a) char *a;
{ if(a)
   {*a=(char)0; free(a);}
}
/* routines to alloc mem & try claiming mem from ds pool if necessary */
char *mem_mal(nn) size_t nn;
{ char *pp;
  do{if(pp=malloc(nn)) return(pp);
    }while(ds_space());
  signal_error(EXIT_OSPACE,"malloc");
  return(NULL); /* never actually gets here */
}
char *mem_rel(rr,nn,kk) char *rr; size_t nn,kk;
{ char *pp,*ss,*tt;
/* Note: some versions of realloc corrupt block if realloc fails, so...*/
/*  do{if(pp=realloc(rr,nn)) break;}while(ds_space()); */
  if(pp=mem_mal(nn))
    {if(rr) {ss=pp; tt=rr; if(kk>nn) kk=nn; while(kk--) *ss++ =(*tt++);}}
  if(rr) fremem(rr);
  return(pp);
}

/* Save string, if enough mem */
char *strsave(s) char *s;
{ char *p;
  if((p=malmem(char,1+strlen(s)))!=NULL) strcpy(p,s);
  return(p);
}

char *fcat(a,b) register char *a,*b;
{ if(!a) signal_error(EXIT_INT_ERR,1);
  if(b) while(*b) *(a)++= *(b)++; *(a)='\0';
  return(a);
}

/* Base type string. Returns base type string pt */
char *base_str(base_type) int base_type;
{ char *bpt,*apt; int stars;
  if(base_flag) apt=base_one; else apt=base_two; bpt=apt;
  base_flag=(boolean)!base_flag;
  if(base_type&STAR_CAST)
    {if(base_type&LST_PBIT) bpt=fcat(bpt,"pair ");
     if(base_type&STAR_MODE) bpt=fcat(bpt,"horz ");
     stars=0;}
  else stars=(base_type&STAR_MASK)>>STAR_SHFT;
  base_type&=STAR_TYPE;
  if(base_type>MAX_TYPES) base_type=0;
  bpt=fcat(bpt,base_list[base_type]);
  while(stars--) bpt=fcat(bpt,"*");
  return(apt);
}

void sayline(ismodule) boolean ismodule;
{ int ef=(-1),el,sel=0;
  if(err_usein) {el=err_line_num; ef=err_file_num;}
  else if(err_pt)
    {if(ismodule)
       {el=err_pt->net->line_num; ef=err_pt->net->sor_num;
        if(flags.state) sel=err_line_num;
       }
     else {el=err_pt->net->line_sec; ef=err_pt->net->sor_sec;}
    }
  if(ef<0 || sour_num==0) err_cc=fcat(err_cc,"?(?): ");
  else
    {char *a=source_list[ef],*b=a+strlen(a)-1,chr;
	 chr=(*b); *b='\0'; 
	 if(sel>0) sprintf(err_cc,"%s(%d,%d) : ",a+1,el,sel);
     else sprintf(err_cc,"%s(%d) : ",a+1,el);
	 *b=chr;
     while(*err_cc) ++err_cc;
    }
 err_skip=(boolean)(err_ef==ef&&err_el==el&&err_sel==sel);
 if(!err_skip) {err_ef=ef; err_el=el; err_sel=sel;}
}
/* Main parser syntax error handling routine */
void raw_error(args)
  va_list args;
{ int error_no,nn=0,cur_error=1; mile ma,mb; boolean skip=F;
  char *ept,*ear=err_cc,*spt,*tpt,*sa,*sb;
  error_no=va_arg(args, int);
  do{if((cur_error=error_list[nn].error_num)==error_no) break;
     ++nn;
    }while(cur_error);
  if(firsterr && !flags.wild)
    {firsterr=0; fprintf(logfp,version,version_num,version_upd,input_name);}
  if(!flags.wild || (cur_error!=error_no) || (cur_error<ERR_BASE))
    {if(cur_error==error_no)
       {if(cur_error>=warn_limit) return;
        ept=error_list[nn].error_str;
        if(ept)
          {if(*ept=='@') ++ept; else sayline(T);
           if(cur_error>=WRN_BASE) err_cc=fcat(err_cc,"warning ");
		   else {++num_errors; err_cc=fcat(err_cc,"error ");}
           sprintf(err_cc,"N2%d: ",error_no);  while(*err_cc) ++err_cc;
           switch(error_no)
             {case EXIT_OSPACE: case EXIT_BADFILE: case EXIT_FILE:
              case WRN_AUX_UND: case WRN_VAR_UNK: case EXIT_BADNSS:
              case WRN_USR_NAM: case WRN_USR_FUN: case WRN_STIL_UNF:
              case WRN_STK_EMPT: case WRN_GLO_UNF:
                spt=va_arg(args, char *);
                sprintf(err_cc,ept,spt);
                break;
              case WRN_NO_COMMS:
                spt=va_arg(args, char *); tpt=va_arg(args, char *);
                sprintf(err_cc,ept,spt,tpt);
                break;
              case EXIT_INT_ERR: ma=(mile)(va_arg(args, int)); goto jimbob;
              case ERR_LAR_BAD: case WRN_CXIN_OWRT:
                ma=va_arg(args, mile); jimbob:
                sprintf(err_cc,ept,ma);
                break;
              case ERR_ARY_ORG: case ERR_CXIN_INDX: case ERR_ARY_MIS:
              case WRN_ELM_OOR:
                ma=va_arg(args, mile); mb=va_arg(args, mile);
                sprintf(err_cc,ept,ma,mb);
                break;
              case WRN_CXIN_TYPE: case WRN_CXIN_NTCX:
                ma=va_arg(args, mile); sa=base_str((int)ma);
                sprintf(err_cc,ept,sa);
                break;
              case WRN_STK_IOOR: case WRN_STK_NTEM:
                spt=va_arg(args, char *);
                ma=va_arg(args, mile);
                sprintf(err_cc,ept,spt,ma);
                break;
              case WRN_VAR_TMS:
                spt=va_arg(args, char *);
                ma=va_arg(args, mile); sa=base_str((int)ma);
                sprintf(err_cc,ept,spt,sa);
                break;
              case WRN_VAR_STR: case WRN_VAR_TST:
                spt=va_arg(args, char *); tpt=va_arg(args, char *);
                ma=va_arg(args, mile); sa=base_str((int)ma);
                sprintf(err_cc,ept,spt,tpt,sa);
                break;
              case WRN_CXIN_TMIS: case WRN_VAR_TYP: case WRN_ARY_TYP:
                ma=va_arg(args, mile); mb=va_arg(args, mile);
                sa=base_str((int)ma); sb=base_str((int)mb);
                sprintf(err_cc,ept,sa,sb);
                break;
              case ERR_OK: break;
              default: sprintf(err_cc,ept);
             } while(*err_cc) ++err_cc; err_cc=fcat(err_cc,"\n");
          }
       }
     else
       {err_pt=NULL; err_usein=F; sayline(F); cur_error=EXIT_ERRERR;
	    ++num_errors;
        sprintf(err_cc,"Unknown error number:%d?\n",error_no);
        while(*err_cc) ++err_cc;
       }
    }
  if(cur_error<ERR_BASE)
    {fputs(ear,logfp);
     if(cur_error>EXIT_OK) fputs("Can't continue. Bye!\n",logfp);
#ifdef MEM_TEST
if(cur_error==EXIT_OSPACE) my_status(); /*DEBUG*/
#endif
     exit(cur_error);}
  if(cur_error<=RAISE_TOP) raise_error=cur_error;
}

void err_ptype(ptype)
  int ptype;
{ char *apt;
  switch(ptype)
    {case PER_IN: apt="In-path:"; break;
     case PER_OUT: apt="Out-path:"; break;
     case PER_GEN: apt="Path:"; break;
     default: apt="Ref-path:";
    }
  err_cc=fcat(err_cc,apt);
}

void err_path(ept,bpt)
  path_elm *ept,*bpt;
{ char *npt,*tpt=NULL,sepchr='\\',dchr;
  if(!ept) {err_cc=fcat(err_cc,"?"); return;}
  while(ept->parent) ept=ept->parent;
  while(ept)
    {if(ept->net->tag) sepchr='.';
     if(ept->partflg) dchr='|'; else dchr=sepchr;
     npt=ept->pat;
     if(!npt) {if(ept->decl_pt) npt=ept->decl_pt->name; else npt="?";}
     if(*npt=='#' && ept->decl_pt)
       {tpt=npt+2; npt=ept->decl_pt->name;
        while(*tpt=='0') ++tpt; if(*tpt=='1' && *(tpt+1)=='\0') tpt=NULL;}
     sprintf(err_cc,"%c%s",dchr,npt); while(*err_cc) ++err_cc;
     if(tpt) err_cc=fcat2(err_cc,":",tpt); tpt=NULL;
     if(ept==err_pt)
       {sprintf(err_cc,"[%d:%d;%d:%d]",
                ept->pary.hasval,(mile)nsl_lst_hilo(ept->pary.ary_list,T),
                ept->dary.hasval,(mile)nsl_lst_hilo(ept->dary.ary_list,T));
        while(*err_cc) ++err_cc;
        break;}
     sprintf(err_cc,"[%d]",ept->dary.hasval); while(*err_cc) ++err_cc;
     if(ept==bpt) break;
     ept=ept->tail;
    }
}

void err_aux(ppt)
  path_elm *ppt;
{ decl_elm *dpt; aux_elm *auxpt; path_elm *tpt; yard nodenum;
  if(ppt)
    {if(aux_top)
       {auxpt=aux_top; tpt=ppt; while(tpt->tail) tpt=tpt->tail;
        while(tpt && !tpt->net->root_flag) tpt=tpt->parent;
        if(!tpt) tpt=ppt;
        nodenum=tpt->net_base;
        while(auxpt && auxpt->aux_base<=nodenum)
          {err_path(auxpt->par_state.org_pt,auxpt->par_state.org_pt);
           dpt=auxpt->par_state.ilk_pt->aux;
           if(dpt) err_cc=fcat2(err_cc,";",dpt->name);
           else err_cc=fcat(err_cc,";?");
           auxpt=auxpt->tail;
          }
       }
     err_path(ppt,NULL);
    }
  err_cc=fcat(err_cc,"\n");
}

char *skip_to(p,chr) char *p,chr;
{ while(*p&&*p!=chr) ++p; return(p);}

void err_disp()
{ char *apt,*bpt;
  if(err_aa&&*err_aa&&err_bb&&*err_bb)
    {apt=err_aa; bpt=err_bb;
     if(!err_filter) {err_aa=bpt; err_bb=apt;}
     else
       {while(*apt)
         {if(*apt=='[') {apt=skip_to(apt,']'); bpt=skip_to(bpt,']');}
          if(*apt=='(') {apt=skip_to(apt,')'); bpt=skip_to(bpt,')');}
          if(*apt!= *bpt)
            {if(isdigit(*apt)&&isdigit(*bpt)) /* skip numbers */
               {do ++apt; while(isdigit(*apt));
                do ++bpt; while(isdigit(*bpt));
               }
             else {apt=err_aa; err_aa=err_bb; err_bb=apt; goto disp;}
            }
          else {++apt; ++bpt;}
         }
       return;
      }
    }
  else {err_aa=err_b1; err_bb=err_b2;}
  disp: if(*err_aa) fputs(err_aa,logfp);
}
void signal_error(va_alist)
  va_dcl
{ va_list args; va_start(args); err_pt=path_pt; err_usein=F;
  if(!err_aa||!*err_aa) err_cc=err_b1; else err_cc=err_bb; *err_cc='\0';
  raw_error(args); va_end(args);
  err_disp();
}

void gen_error(ptype,usein,ppt,ept,args)
  int ptype; boolean usein; path_elm *ppt,*ept; va_list args;
{ va_list tar=args; int error_no; err_pt=ept; err_usein=usein;
  error_no=va_arg(tar,int);
  if(!err_aa||!*err_aa) err_cc=err_b1; else err_cc=err_bb; *err_cc='\0';
  if(error_no!=ERR_OK) raw_error(args);
  if(!flags.wild && error_no<warn_limit)
    {err_ptype(ptype); err_aux(ppt);}
  err_disp();
}

void in_error(va_alist)
  va_dcl
{ va_list args; va_start(args);
  gen_error(PER_IN,T,inpt,path_pt,args); va_end(args);
}

void out_error(va_alist)
  va_dcl
{ va_list args; va_start(args);
  gen_error(PER_OUT,T,outpt,path_pt,args); va_end(args);
}

void path_error(ptype,ppt,ept,va_alist)
  int ptype; path_elm *ppt,*ept; va_dcl
{ va_list args; va_start(args);
  gen_error(ptype,T,ppt,ept,args); va_end(args);
}
/*-----------------------------------------------------------------*/
/* unpact cast char type code to int */
int cast_type(chr)
  char chr;
{ return((int)(((chr&0x30)<<1)|(chr&0x0f)));
}

/* compare two id strings which are terminated by non ident chrs
* return T if equal */
boolean strideq(a,b)
  char *a,*b;
{ if(!a || !b) {if(a==b) return(T); else return(F);}
  while(isident(*a) && (*a==(*b))) {++a; ++b;}
  return((boolean)(!isident(*a) && !isident(*b)));
}

/* Find name str in sorted list of names.
*  Returns pos+1, or -(closest pos) if not found */
int find_name(name,list,hi)
  char *name, **list; int hi;
{ int lo=0,test=0,oldtest,temp;
  if(hi<1) return(0); /* user could have garbage in list even if no elms */
  do {oldtest=test; test=(hi+lo)/2;
    if((temp=strcmp(name,list[test]))==0) return(test+1);
    if(temp<0) hi=test;  else lo=test;
  } while(test!=oldtest);
  return(-hi);
}

/* Look for mask in string.
*  Returns mask position in string, 0 if not found */
int maskinstr(mask,str)
  char *mask,*str;
{ int j,i=0; char chr;
  while(str[i]) /* test "(l|?)*" = "str" */
   {j=0;
    while(chr=mask[j])
     {if(chr=='?' || chr==str[i+j]) j++;
      else break; }
    i++;
    if(!chr) return(i);
   }
  return(0);
}

/* See if string matches pattern.
*  Returns T or F.
* mask --> (l | ?)*
* pattern --> *mask | mask* | mask | *
* template --> pattern | pattern.template
*/
boolean match(pattern,str)
  char pattern[],str[];
{ int j,i=(int)strlen(pattern); char chr;
  if(pattern[0]=='*')		/* test "*mask" == "str" */
    { if(!pattern[1] || pattern[1]=='*') return(T); /* "*" */
      if(j=maskinstr((char *)pattern+1,str))
        return((boolean)(str[j+i-2]=='\0'));
      return(F);
    }
  else if(i==0) return(F);
  j=i; chr='\0';		/* test "mask*" == "str" */
  if(pattern[j-1]=='*') {j--; pattern[j]=chr; chr='*';}
  else if(j!=(int)strlen(str)) return(F);
  i=maskinstr(pattern,str);
  pattern[j]=chr;
  return((boolean)(i==1));
}

/*---------------------------------------------------------------*/
/* Token Manager
* token=0 is a free list which uses refs to point to next free token
*/
typedef union _token_val
{ char *spt;
  token *tpt;
} token_val;

typedef struct _token_elm
{ token refs; /* number of references to token, doubles as free pt */
  swin size; /* size of token(positive) or data(negative) list */
  token_val pt; /* token value: string or list of tokens */
} token_elm;

#define tofs 256 /* token value offset, first 256 are ascii */

token_elm  *token_table=NULL;
char token_chrbuff[]={'a','\0'};
char token_buff[MAX_TOKEN_STR],*token_pt=NULL;

void token_free()
{ lfree(token_table); token_table=NULL;
}

/* add a freed token to token free list. tok must be >0 */
void add_free_token(abstok,next)
  token abstok; token next;
{ token prev;
  do {prev=next; next=token_table[prev].refs; }
  while(next && next<abstok);
  token_table[prev].refs=abstok;
  token_table[abstok].refs=next;
  token_table[abstok].size=0;
  token_table[abstok].pt.spt=NULL; /* freed tokens have this set to NULL */
}

boolean asciitok(tok)
  token tok;
{ if(tok<=tofs) return(T); /* ascii */
  return(F);
}

/* check token is valid, returns its absvalue if valid, else 0 */
token valid_token(tok)
  token tok;
{ token abstok;
  if(tok<=tofs) return(0); /* ascii */
  abstok=(token)(tok-tofs);
  if(abstok>lst_ok(token_table)||tok<1) /* bad token, out of range */
    return(0);
  if(!token_table[abstok].pt.spt) /* reference to a freed token */
    return(0);
  if(!token_table[abstok].refs) /* check outstanding refs */
    return(0);
  return(abstok);
}

swin token_refs(tok)
  token tok;
{ token abstok;
  if((abstok=valid_token(tok))<1) return(0);
  return(token_table[abstok].refs);
}

void del_token(tok)
  token tok;
{ token abstok,*dpt; token_elm *tpt; swin size;
  if((abstok=valid_token(tok))<1) return;
  tpt=(token_elm *)&token_table[abstok];
  if(tpt->refs) /* check outstanding refs */
     {--(tpt->refs); if(tpt->refs) return;}
  size=tpt->size;
  if(size==0) fremem(tpt->pt.spt);
  else
    {if(size>0)
       {dpt=tpt->pt.tpt;
        while(size) {del_token(*dpt); ++dpt; --size;}
       }
     fremem(tpt->pt.tpt);
    }
  add_free_token(abstok,(token)0);
}

/* copy token, returns 0 if not copied */
token copy_token(tok)
  token tok;
{ token abstok;
  if(asciitok(tok)) return(tok);
  if(!(abstok=valid_token(tok))) return(0);
  ++token_table[abstok].refs;
  return(tok);
 }

/* increase token table &update free list */
void more_tokens()
{ token prev=(token)0,max_old=(token)lst_ok(token_table);
  token_table=lmore(token_table,4,TYPE_STRUCT,token_elm);
  if(max_old==0) token_table[0].refs=0;
  while(max_old<lst_num(token_table))
    {add_free_token(max_old,prev); prev=max_old; ++max_old;}
}

/* fetch a new token */
token get_token()
{ token abstok;
  if(!lst_ok(token_table)||!token_table[0].refs) more_tokens();
  abstok=token_table[0].refs;
  token_table[0].refs=token_table[abstok].refs;
  token_table[abstok].refs=1;
  return(abstok);
}

char *move_token_str(tpt,fpt,size)
  char *tpt,*fpt; swin size;
{ while(size) {*tpt= (*fpt); ++tpt; ++fpt; --size;}
  *tpt='\0';
  return(tpt);
}

/* returns token value, size & kind of token */
char *token_value(tok,size,kind)
  token tok; swin *size,*kind;
{ token_elm *apt; token abstok;
  *kind=0; *size=0;
  if(asciitok(tok))
     {*size=1; *token_chrbuff=(char)tok; return(token_chrbuff);}
  if(!(abstok=valid_token(tok))) return(NULL);
  apt=(token_elm *)&token_table[abstok];
  *kind=apt->size;
  if(*kind==(token)0) *size=(token)strlen(apt->pt.spt);
  else if(*kind<(token)0) *size=(token)(-apt->size);
  else *size=apt->size;
  return(apt->pt.spt);
}

/* for atomic tokens simply fetch token string
*  for list tokens, detokenise token list
*  build string in token_buff if necessary
*/
char *token_to_str(tok,size)
  token tok; swin *size;
{ token *tpt; char *spt,*basept,*prevpt;
  swin sum=0,atom,count;
  spt=token_value(tok,(swin *)size,(swin *)&count);
  if(count<1 || !spt) return(spt);
  tpt=(token *)spt;
  basept=token_pt;
  if(!token_pt) {token_pt=token_buff; *token_pt='\0';}
  while(count)
    {prevpt=token_pt; spt=token_to_str(*tpt,(swin *)&atom);
     ++tpt; --count;
     if(spt)
       {sum+=(swin)atom;
        if(spt!=prevpt) token_pt=move_token_str(token_pt,spt,atom);
       }
     else break;
    }
  if(!basept) {token_pt=NULL; basept=token_buff;}
  *size=sum;
  return(basept);
}

token *token_to_data(tok,size)
  token tok; swin *size;
{ return((token *)token_to_str(tok,size));
}

/* convert atomic string to token, return token value */
token atom_to_token(apt)
  char *apt;
{ token pos=1; token_elm *tpt=token_table; ++tpt;
  if(*(apt+1)=='\0') return(*apt); /* ascii chr token */
  if(!lst_ok(token_table)) more_tokens();
  else while(pos<lst_num(token_table)) /* search for atom first */
    {if(tpt->refs>0 && tpt->pt.spt && tpt->size==0)
       if(strcmp(apt,tpt->pt.spt)==0) {++tpt->refs; return((token)(pos+tofs));}
     ++pos; ++tpt;}
  pos=get_token();
  token_table[pos].pt.spt=strsave(apt);
  return((token)(pos+tofs));
}

token *toksave(bpt,size)
  token *bpt; swin size;
{ token *tpt,*spt; swin abssize;
  if(size<0) abssize=(swin)(-size); else abssize=(swin)size;
  tpt=malmem(token,1+abssize); spt=tpt;
  while(abssize)
    {*tpt=(*bpt);
     ++tpt; ++bpt; --abssize;}
  *tpt=0;
  return(spt);
}

/* compare two token lists. return number of matching atoms
*  apt may be longer than bpt, nulls terminate match */
swin tokcmp(apt,bpt,size)
  token *apt,*bpt; swin size;
{ swin mats=0;
  while(size)
    {if(!*apt)
       {if(!*bpt) break;
        return(0); /* in case b is longer than a */
       }
     if(*apt!=(*bpt)) break;
     ++mats; ++apt; ++bpt; --size;
    }
  return(mats);
}

/* test if token data strings are equal */
boolean tokseq(apt,bpt,size)
  token *apt,*bpt; swin size;
{ if(size<0) size=(swin)(-size);
  while(size) {if(*apt!=(*bpt)) return(F); ++apt; ++bpt; --size;}
  return(T);
}

/* convert a list of tokens to a single token: assumed list is unique
*  if save, then saves a copy of string
*/
token list_to_token(bpt,size,save)
  token *bpt; swin size; boolean save;
{ token pos;
  if(size==0) return(0);
  if(size==1) return(*bpt);
  pos=get_token();
  token_table[pos].size=size;
  token_table[pos].pt.tpt=(save)?toksave(bpt,size):bpt;
  return((token)(pos+tofs));
}

/* convert a list of token sized data to a token
*  if save is F then can use apt memory or free it
*  if save is T then must malloc space for apt if necessary
*/
token data_to_token(apt,size,save)
  token *apt; swin size; boolean save;
{ token pos=1; token_elm *tpt=token_table; ++tpt;
  if(size>0) size=(swin)(-size);
  if(!lst_ok(token_table)) more_tokens();
  else while(pos<lst_num(token_table)) /* search for tokens first */
    {if(tpt->refs>0 && tpt->pt.tpt && tpt->size==size)
       if(tokseq(apt,tpt->pt.spt,size))
         {++tpt->refs; if(!save) fremem(apt); return((token)(pos+tofs));}
     ++pos; ++tpt;}
  return(list_to_token(apt,size,save));
}

/* Tokenising process:
*  1) convert ascii string to list of atomic tokens,
*  2) reduce list of atoms to single token.
*/

/* Convert string to a list of atoms. May have to contruct a token list
*  treat anything inside [], {}, () as a single token
*  takes a copy of tokens where necessary
*  returns start of token buff
*/
token *str_to_atoms(spt,size)
  char *spt; swin *size;
{ char chr,*pt; swin count=0; boolean iswild;
  token tok=1,*tpt=(token *)token_buff;
  *tpt=0;
  while(*spt && tok)
    {switch(*spt)
       {case '[': case '(': case '{': case'<': case '"': case '\'':
          push_io(); set_io_str(spt); skip_bodies();
          pt=get_io_pos(); pop_io();
          chr=(*pt); *pt='\0';
          tok=atom_to_token(spt); *pt=chr; spt=pt;
          break;
        case '|': case '\\': case '/': case '.': case '\0':
        case '#': case '?': case '!': case '$': case ',':
          tok=(token)*spt; ++spt;
          break;
        default:
          pt=get_wild_id(spt,(boolean *)&iswild);
          if(pt==spt) tok=0;
          else
            {chr=(*pt); *pt='\0';
             tok=atom_to_token(spt); *pt=chr; spt=pt;}
       }
       *tpt=tok; ++tpt;
       if(tok) ++count;
    }
  *tpt=0;
  *size=count;
  return((token *)token_buff);
}

/* search token table for longest matching list of tokens
* appt pts to a list of tokens */
token find_longtok(appt,sizpt)
  token **appt; swin *sizpt;
{ token *lspt,pos=1,laspos=0; token_elm *tpt=token_table;
  swin mats=0,lasmat=0,tsize;
  ++tpt;  *sizpt=1; lspt= (*appt); if(!*lspt) return(0);
  while(pos<lst_num(token_table))
    {tsize=tpt->size;
     if(tpt->refs>0 && tpt->pt.tpt && tsize>0)
       {mats=tokcmp(lspt,tpt->pt.tpt,tsize);
        if(mats==tsize && mats>lasmat) {lasmat=mats; laspos=pos;} }
     ++pos; ++tpt;}
  if(!lasmat) {lasmat=1; laspos=(token)((*lspt)-tofs);}
  *appt=lspt+lasmat; *sizpt=lasmat;
  return((token)(laspos+tofs));
}

/* Convert list of atoms to a single token
*  Uses atom list as a tmp stack: so input list is destroyed
*/
token atoms_to_token(apt)
  token *apt;
{ swin matlen,stklen=0; boolean matches=F;
  token *bspt,*stkpt,*opt,tok;
  bspt=(token *)token_buff; stkpt=bspt;
  do{opt=apt;
     tok=find_longtok((token *)&apt,(swin *)&matlen);
     if(matlen>1)
       {matches=T; copy_token(tok);
        while(matlen) {del_token(*opt); ++opt; --matlen;}
        if(stklen>1)
          {*bspt=list_to_token(bspt,stklen,T); stkpt=bspt+1; stklen=1;}
       }
     *stkpt=tok;
     if(tok)
       {++stkpt; ++stklen;}
     else if(stklen>1 && matches)
       {matches=F; tok=1; stklen=0; apt=bspt; stkpt=bspt;}
    }while(tok);
  if(stklen>1) *bspt=list_to_token(bspt,stklen,T);
  return(*bspt);
}

token str_to_token(str)
  char *str;
{ swin number; token *tpt;
  tpt=str_to_atoms(str,(swin *)&number);
  if(number>1) return(atoms_to_token(tpt));
  return(*tpt);
}
