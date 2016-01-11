/* Name: general.c   Type: general npp dependent c routines   Date: 30 March 89
*  NeSeL Network Specification Language Preprocessor
*  Update: Nov 1996.
*/

#include "globals.h"
#include "iolo.h"
#include "nslone.h"
#include "nslyacc.h"

extern reals yacc_exp_value;

#define MAX_BASE 11	/* number of base types */
extern char *base_list[]={
     "char","double","float","int","long","short",
     "signed","struct","union","unsigned","void"};

/* Searches stack for name str, returns pos if found
*/
stack_elm *on_stack(name)
  char *name;
{ stack_elm *stk_pt=stack_top;
  while(stk_pt!=NULL)
    {if(strcmp(name,(char *)&stk_pt->name)==0) return(stk_pt);
     stk_pt=stk_pt->tail;
    }
  return(NULL);
}

/* Base type string.
*  Returns base type string in buffer.
*/
void base_str(base_type,buffer)
  int base_type; char *buffer;
{ switch(base_type)
   {case 6: base_type=11; loop:
    case 1: case 2: case 3: case 4: case 5: case 8:
    case 9: strcpy(buffer,base_list[base_type-1]); break;
    case 7: strcpy(buffer,"struct"); break;
    case 10: strcpy(buffer,"short int"); break;
    case 11: strcpy(buffer,"unsigned char"); break;
    case 12: strcpy(buffer,"unsigned int"); break;
    case 13: strcpy(buffer,"unsigned long"); break;
    case 14: strcpy(buffer,"unsigned short int"); break;
    default: /* type unknown, treat as int */ /*DEBUG12/91*/
      base_type=TYPE_INT; goto loop;
     /* *buffer='\0'; */
   }
}

/* Parse a type specifier. & return it's base type code */
int get_base_code(io_store)
  char **io_store;
{ int pos,base_type=0,base_sign=0,first_time=TRUE;
  *io_store=get_io_pos();
  while((pos=find_name(cur_ident,base_list,MAX_BASE))>0)
    {switch(pos)
       {case 2: /* double */
        case 3: /* float */ if(base_type==5) base_type=2;
        case 8: /* struct */ 
        case 9: /* union */ base_sign=0;
        case 1: /* char */
        case 4: /* int */ if(base_type==5 || base_type==10) break;
        case 5: /* long */ base_type=pos; break;
        case 6: /* short */ base_type=10; break;
        case 7: /* signed */ base_sign=0; if(first_time) base_type=4; break;
        case 10: /* unsigned */ base_sign=1; if(first_time) base_type=4; break;
        case 11: /* void */ base_type=6; break;
        default: ;
       }
     first_time=FALSE;
     *io_store=get_io_pos();
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
  return(base_type);
}

int get_type_code(bf_pt,io_store,valid)
  char **bf_pt,**io_store; BOOL *valid;
{ int base_type;
  base_type=get_base_code(io_store);
  *valid=(BOOL)(base_type!=0);
  base_str(base_type,*bf_pt);
  *bf_pt+=strlen(*bf_pt);
  return(base_type);
}

cond_elm *new_cond(label,type_code,ary_size,tag,nodes)
  char *label,*tag; int type_code,ary_size; long nodes;
{ unsigned int some_bytes=sizeof(cond_elm); cond_elm *new_elm;
  if( ( new_elm=(cond_elm *)malloc(some_bytes) ) == (cond_elm *)NULL)
    signal_error(EXIT_OSPACE,"pushing struct details");
  strcpy(new_elm->label,label);
  strcpy(new_elm->tag,tag);
  new_elm->type_code = type_code;
  new_elm->ary_size = ary_size;
  new_elm->nodes = nodes;
  new_elm->tail = NULL;
  return(new_elm);
}

/* Save information about struct members on a stack for later.
*/
void push_cond(label,type_code,ary_size,tag,nodes)
  char *label,*tag; int type_code,ary_size; long nodes;
{ cond_elm *npt;
  npt=new_cond(label,type_code,ary_size,tag,nodes);
  npt->tail = cond_top;
  cond_top = npt;
}

void push_bot_cond(label,type_code,ary_size,tag,nodes)
  char *label,*tag; int type_code,ary_size; long nodes;
{ cond_elm *npt,*apt;
  npt=new_cond(label,type_code,ary_size,tag,nodes);
  if(!cond_top) cond_top=npt;
  else
    {apt=cond_top; while(apt->tail) apt=apt->tail;
     apt->tail=npt;}
  npt->tail=NULL;
}

/* Construct a condensed rep of variable.
*  Returns buffer pos for next member.
*  apt == "*label[exp]" or "**label[exp]" or "label:exp"
*     terminated by , or ;
*/
char *cond_append(apt,type_code,tag_pt)
  char *apt; int type_code; char *tag_pt;
{ char chr,*tpt,*ipt,*upt=NULL,lab_buf[max_ident];
  int ident=0,stars=0,bracks=0,uarys=0,farys=0,bits=0,ary_size=0;
  BOOL loop=TRUE,idsbrk=FALSE;
  push_io(); set_io_str(apt); tpt=apt;
  do{switch(chr=skip_blanks())
       {case '\0': case ';': case ',':
          *apt=chr; ++apt; loop=FALSE; break;
        case '*': ++stars; *apt=chr; ++apt;  break;
        case '(': ++bracks; *apt=chr; ++apt;  break;
        case ')': *apt=chr; ++apt; break;
        case '[': *apt=chr; ++apt; idsbrk=(BOOL)(ident!=0);
          chr=skip_blanks();
          if(chr==']')
            {upt=apt; *apt=chr; ++apt; ++uarys;}
          else
            {unput_char(chr); ++farys;
             if(!yacc_parse_exp())
               {yacc_exp_value=(float)1; signal_error(ERR_VAR_FIX,tpt);}
             else
               {chr=skip_blanks(); if(chr!=']') signal_error(ERR_VAR_RSB,tpt);}
             chr=input_char();
             ary_size=(int)yacc_exp_value;
             sprintf(apt,"%d]\0",ary_size);
             apt+=strlen(apt);
             unput_char(chr);
            }
          break;
        case ':': *apt=chr; ++apt; ++bits; *apt='\0';
          if(!yacc_parse_exp())
            {yacc_exp_value=(float)1; signal_error(ERR_VAR_BIT,tpt);}
          chr=input_char();
          sprintf(apt,"%d\0",(int)yacc_exp_value);
          apt+=strlen(apt);
          unput_char(chr);
          break;
        default:
          if(isalpha(chr) || chr=='_')
            {idsbrk=(BOOL)((uarys+farys)==0);
             ipt=apt; unput_char(chr); get_ident(); ++ident;
             chr=input_char();
             strcpy(lab_buf,cur_ident); apt=fcat(apt,cur_ident);
             unput_char(chr);
            }
          else
            {*apt=chr; ++apt; *apt='\0'; signal_error(WRN_VAR_SYN,tpt);}
       }
    }while(loop);
  *apt='\0';
  if(!ident) {signal_error(WRN_VAR_NAME,tpt); strcpy(lab_buf,"?\0");}
  if(ident==1 && uarys==1 && idsbrk && !farys && !bracks && !bits)
    {*ipt='*'; ++ipt; strcpy(ipt,lab_buf); *upt=' '; ary_size=(-1);
     if(type_code!=TYPE_NODE) ++stars;}
  if(stars)
    {if(type_code==TYPE_NODE) signal_error(WRN_NOD_STAR,tpt);
     stars<<=STAR_SHFT;
     type_code|=stars;}
  push_cond(lab_buf,type_code,ary_size,tag_pt,(long)0);
  pop_io();
  return(apt);
}

/* Expand a typedef'ed declaration.
* cur_ident = start of decl. Returns typedef terminator or null and expanded
* str in buffer. Next input= next chr after decl.
*/
char expand_typedef(buffer)
  char *buffer;
{ char chr,*buff_beg,*buff_end,*str_pt=buffer,*type_pt="%s",*apt,*io_store,*s;
  char tag_id[max_ident]; int type_num,pos; BOOL valid;
  *buffer='\0'; type_num=0; *tag_id='\0';
  if((pos=find_name(cur_ident,tdefs_list,tdefs_num))>0)
    {/* a type defed decl */
     type_pt=tdefs_list[--pos];
     type_pt+=strlen(type_pt)+1; sscanf(type_pt,"%d",&type_num);
     type_pt+=strlen(type_pt)+1;
     str_pt=fcat(str_pt,type_pt);
     type_pt+=strlen(type_pt)+1;
     if(type_num==TYPE_STRUCT || type_num==TYPE_NODE || type_num==TYPE_UNION)
       {apt=str_pt-1; while(*apt!=' '&&apt>=buffer) --apt;
        strcpy(tag_id,apt+1);}
    }
  else
    {
     if((pos=find_name(cur_ident,neuron_list,neuron_num))>0)
       {if((stkrec(neuron_list[pos-1])->def_type)==DEF_NEURON)
          {/*node substruct */
           type_num=TYPE_NODE; str_pt=fcat(str_pt,cur_ident);
           strcpy(tag_id,cur_ident);
          }
        else  /* Undefined type */
          {signal_error(ERR_TYP_UDF,cur_ident);
           return('\0');}
       }
     else
       {type_num=get_type_code((char **)&str_pt,(char **)&io_store,
                               (BOOL *)&valid);
        if(type_num==TYPE_STRUCT || type_num==TYPE_UNION)
          {if(!*cur_ident) {signal_error(ERR_TAG); strcpy(cur_ident,"?");}
           str_pt=fcat2(str_pt," ",cur_ident);
           strcpy(tag_id,cur_ident);
          }
        else
          {if(!valid) /* add undefined type to undef_list */
             {s=strsave(cur_ident);
              if(add_list_name(&undef_list,s,&undef_num,
                               &max_undef,inc_undef)>0)
                free(s);
              else signal_error(WRN_TYP_UNK,s);
              type_num=TYPE_INT;
             }
           set_io_pos(io_store);
          }
       }
    }
  str_pt=fcat(str_pt," ");
/* By this point start of Buffer is base types, str_pt is nxt buffer pos,
*  type_num indicates type code of base type.
*  If typedefed type_pt is set to template.
*/
  do
    {chr=skip_blanks(); unput_char(chr);
     buff_beg=buffer_on();  chr=skip_to_cast(','); buff_end=buffer_off();
     if(!chr)
       {signal_error(EXIT_TYP_EOF); return(FALSE);}
        /* separator or terminator expected */
     if(buff_end) *(buff_end-1)='\0';
     sprintf(str_pt,type_pt,buff_beg); apt=str_pt;
     str_pt+=strlen(str_pt);
     *str_pt=chr; ++str_pt; *str_pt='\0';
/* At this point apt pts to start of a decl member,
*  Next io is after type member separator.
*  Need to convert NODE types to *Node & append to cond_str.
*/
     str_pt=cond_append(apt,type_num,tag_id);
     if(raise_error) return(FALSE);
    } while(chr==',');
  return(chr);
}

/* convert a typedef in a function decl to base code given typedef list pos
*  & return type template pt
*  returns 0 if not a user func type */
int functypedef(num,tppt)
  int num; char **tppt;
{ char *type_pt;
  type_pt=tdefs_list[--num];
  type_pt+=strlen(type_pt)+1; sscanf(type_pt,"%d",&num);
  /*DEBUG3/6/93 allow these types at users own risk...*/
  /*if(num==TYPE_STRUCT || num==TYPE_NODE || num==TYPE_UNION) return(0);*/
  type_pt+=strlen(type_pt)+1;
  type_pt+=strlen(type_pt)+1;
  *tppt=type_pt;
  return(num);
}
/* convert chr type code to int */
int cast_type(chr)
  char chr;
{ return((int)(chr&0x3f));
}

/* given a typedef name and additional spec arg (* & []'s) convert to base
*  If it's a normal type cast stick it in round brackets as well
*  finally put angular brackets round type spec */
char *type_cast(name,spec)
  char *name,*spec;
{ int pos,typcode=0,stars=0; BOOL valid;
  char *str_pt,*apt,*cod_pt,*rig_pt,buff[buffsize],bufftwo[buffsize];
  /* convert any typedef'd type to base type */
  sal_lis&=(SIS_VMK); tmp_int=0; /* clear prior cast flags */
  if((pos=find_name(name,tdefs_list,tdefs_num))>0)
    {name=tdefs_list[--pos];
     name+=strlen(name)+1;
     name+=strlen(name)+1;
     str_pt=name+strlen(name)+1;
     if(!spec) spec="\0";
     sprintf(buff,str_pt,spec);
     spec=buff;
    }
  /* now fetch typecode */
  str_pt=bufftwo;
  push_io(); set_io_str(name);
  if(!get_ident()) signal_error(EXIT_INT_ERR);
  typcode=get_type_code((char **)&str_pt,(char **)&apt,(BOOL *)&valid);
  pop_io();
  if(valid) /* @ is code for unknown type */
    {cod_pt="<@("; *(cod_pt+1)=(char)('@'|typcode); rig_pt=")>";
     if(typcode!=TYPE_STRUCT && typcode!=TYPE_UNION) name=bufftwo;
    }
  else /* node type, or undefined type if so just leave it unchanged */
    {if(find_name(name,neuron_list,neuron_num)>0)
       cod_pt="<G"; /* G is code for TYPE_NODE */
     else
       cod_pt="<@"; /* @ is code for unknown type */
     rig_pt=">";
    }
  /* chk if pointer type */
  apt=spec;
  while(apt && *apt)
    {if(*apt=='*') ++stars;
     else if(*apt=='[') {sal_lis|=SIS_VER; ++stars;}
     ++apt;}
  if(stars) {stars<<=STAR_SHFT; *(cod_pt+1)|=stars;}
  if(spec && *spec) return(fsav5(cod_pt,name," ",spec,rig_pt));
  else return(fsav3(cod_pt,name,rig_pt));
}

/* workout true cast & type for the elements of list etc and workout the
* evaluation cast for list i.e. vert/horz, elm siz, elm typ, LST_PBIT etc
* Note if var or list is used without a cast then this routine is not called
*  and the default list type or var/func type is taken.
* (type)[...] (type *)[...]
* (type [])[...] (type *[])[...]
* if [...] contains a range (a..b) then sets STAR_PBIT
* converts (type *)[...] to TYPE_PT
* (type [])[...] set vertical mode flag
*/
char *conv_cast(apt,bpt,isarg)
  char *apt,*bpt; BOOL isarg;
{ char *ept=")>",*dpt="A$^Dc",*cpt,*fpt=bpt,trutyp=(*(apt+1)),evatyp,stars;
  BOOL islist;
  evatyp=trutyp&STAR_RES; stars=trutyp&STAR_MASK;
  if(isarg) {islist=(BOOL)(*bpt=='[');}
  else
    {*dpt=(*bpt); if(*bpt!='{') ++bpt;
     islist=(BOOL)(*(dpt)=='A'&&*(bpt)=='[');}
  if(islist) /* if list, workout internal list type code */
    {if(stars)
       {if(sal_lis&SIS_VER)
          {trutyp=evatyp;
           if(stars&STAR_TWO) {trutyp|=STAR_ONE; evatyp=(char)('@'|TYPE_PT);}
           evatyp|=STAR_ONE; *(apt+1)=trutyp;
          }
        else goto fred;
       }
     if(tmp_int&SIS_RNG) evatyp|=STAR_TWO;
    }
  else if(stars) /* on non-lists, convert pt casts to TYPE_PT */
    {fred: evatyp=(char)('@'|TYPE_PT);}
  tmp_int=0; sal_lis&=SIS_RMK; *(dpt+4)=evatyp;
  if(isarg) return(fsav2(dpt+1,bpt)); /*if arg cast, strip off cast str*/
  if(islist) /* if list, workout new list cast string */
    {cpt=apt+strlen(apt)-3;
     if(*(apt+2)!='(') {++ept; ++cpt;}
     while(*cpt==' '||*cpt=='*'||*cpt=='['||*cpt==']') --cpt;
     ++cpt;
     if(trutyp&STAR_MASK) {if(*cpt!=' ') *cpt=' '; ++cpt; *cpt='*'; ++cpt;}
     *cpt='\0';
     return(fsav4(apt,ept,dpt,bpt));
    }
  return(fsav2(apt,fpt)); /* plain var cast, leave as cast string + value */
}

/* Make a hole in string ready for string insertion.
*/
BOOL make_hole(at_pt,size,top_pt)
  char *at_pt,*top_pt; int size;
{ char *from_pt=at_pt,*to_pt;
  if(size>0) /* move up */
    {from_pt+=strlen(at_pt);
     to_pt=from_pt+size;
     if(to_pt>top_pt) return(FALSE); /* not enough space for expansion */
     while(from_pt>=at_pt) {*to_pt=(*from_pt); --to_pt; --from_pt;}
    }
  else if(size<0) /* move down */
    {to_pt=from_pt+size;
     while(*from_pt) {*to_pt=(*from_pt); ++to_pt; ++from_pt;}
     *to_pt='\0';
    }
  return(TRUE);
}

/* Expand a structure declaration. Resolve typedefs and collect
*  condense string information.
*/
char *expand_struct(wrkbuff,buff_size)
  char *wrkbuff; int buff_size;
{ char *apt,*at_pt, *buff_beg, chr; int qchrs;
  char big_buff[bigbuffsize];
  chr=skip_to_brace();
  if(!chr) {signal_error(EXIT_TYP_LBR); return(NULL);}
  unput_char(chr);
  buff_beg=buffer_on(); chr=skip_braces(); buffer_off();
  if(!chr) {signal_error(EXIT_TYP_RBR); return(NULL);}
  apt=fcat(wrkbuff,"typedef struct {\n"); fcat(apt,buff_beg+1);
  push_io(); set_io_str(apt);
  do{apt=get_io_pos();
     if(get_ident())
       {chr=expand_typedef(big_buff);
        if(!chr || !*big_buff)
          chr=skip_to_scolon();
        else
          {strcat(big_buff,"\n");
           at_pt=get_io_pos();
           qchrs=strlen(big_buff)-(at_pt-apt);
           if(make_hole(at_pt,qchrs,wrkbuff+buff_size))
             {set_io_pos(at_pt+qchrs);
              at_pt=big_buff; while(*at_pt) {*apt= *at_pt; ++apt; ++at_pt;}
              chr=';'; }
          }
       } else chr=skip_blanks();
    } while(chr==';');
  pop_io();
  if(chr=='}' && !raise_error) {*apt++ =chr; *apt='\0'; return(apt);}
  return(NULL);
}

char *buffer_module(mod_pt,sizept)
  stack_elm *mod_pt; int *sizept;
{ char *mod_buff_pt=get_module(mod_pt,sizept); /* get original module def */
  if(!mod_buff_pt) return(NULL); /* Error: couldn't get module */
  *sizept=1000 + 2 * (*sizept);
  mod_buff_pt=(char *)realloc(mod_buff_pt,(unsigned int)*sizept);
  if(!mod_buff_pt) signal_error(EXIT_OSPACE,"buffering module");
  return(mod_buff_pt);
}

/* blank to end of line */
char *blankit(pt)
  char *pt;
{ while(pt && *pt!='\n') {*pt=' '; ++pt;}
  return(pt);
}

/* blank to next endif, else or elif, blank first line regardless */
char blanktoe(pt)
  char *pt;
{ char *lpt,chr; int lvl=0;
  pt=blankit(pt);
  set_io_str(pt);
  while(chr=skip_quoln())
    {if(chr=='$')
       {lpt=get_io_pos()-1;
        if(get_ident())
          {if(strcmp(cur_ident,"if")==0) ++lvl;
           else if(strcmp(cur_ident,"endif")==0)
              {if(!lvl) goto wipe; --lvl;}
           else if((strcmp(cur_ident,"else")==0||strcmp(cur_ident,"elif")==0)
                   &&!lvl)
             {wipe:
              while(pt!=lpt)
                {if(*pt!='\n') *pt=' '; ++pt;}
              set_io_str(lpt); chr='\n';
              break;
             }
          }
       }
    }
  return(chr);
}
char *do_recmac();
typedef struct _mac_elm
{int hi,lo,arg;} mac_elm;
mac_elm  mac_sub[MAX_EXPANDS];
int mac_rec[MAX_MACREC],num_rec=0,num_sub=0;

#define IsHash(p) (((p)[0]=='#')&&((p)[-1]!='\\'))

/* Expand a macro. Substitue args occurring in str for their defs.
*  Args expanded in line. So str must be big enough.
*  If overflows returns FALSE and str may be altered.
*  sets raise_error accordingly.
*  arg_list[n]="arg_name\0arg_def"; arg_list[end]=NULL;
*  If if_flag is set then conditional expansion takes place
*  i.e. $if exp, $elif exp, $else, $endif
*/
BOOL expand_macro(str,max_len,arg_list,if_flag)
  char *str,*arg_list[]; int max_len; BOOL if_flag;
{ int arg_num=0,num_hashes=0,old_sub=num_sub,expands=0,cur_arg;
  int qchrs,beg_ofs,end_ofs,lo,hi,nn;
  BOOL hash=FALSE,if_cond=TRUE,exitflag=FALSE;
  char *tmp_pt, *beg_pt, *end_pt, *arg_pt,*las_pt,*if_pt=NULL,*if_exp=NULL;
  char *bas_pt=NULL,chr=' ',laschr,priorchr;
  if((int)strlen(str)>max_len) {signal_error(ERR_MACRO); return(FALSE);}
  while(arg_list[arg_num]) ++arg_num;
/* DEBUG even though no args, macro could contain internal mac calls...
  if(arg_num<1) return(TRUE);
*/
  sort_list(arg_list,arg_num); /* this was commented out */
  push_io();
  do
    {expands=0; set_io_str(str);
  do
    {laschr=chr; las_pt=get_io_pos()-1;
     chr=skip_quoln();
     if(chr=='\n' && if_pt) /* a conditional line has now been expanded */
       {las_pt=get_io_pos()-1; *las_pt='\0'; set_io_str(if_exp);
        if(yacc_parse_exp()) if_cond=(BOOL)(yacc_exp_value!=(reals)0.0);
        else if_cond=FALSE;
        *las_pt=chr;
        las_pt=blankit(if_pt); if_pt=NULL;
        if(!if_cond) chr=blanktoe(las_pt);
        continue;
       }
     if((chr=='#')&&(laschr!='\\')) /* chk not an escaped hash */
       ++num_hashes;
     else
       {unput_char(chr);
        if(get_ident()) /* got ident */
          {if(laschr=='$' && if_flag)  /* check expansion conditionals */
             {if(chr=='i')
                {if(strcmp(cur_ident,"if")==0) /* if cond */
                   {if(!if_pt) {if_exp=get_io_pos(); if_pt=las_pt;}
                    continue; }
                }
              else if(chr=='e')
                {if(strcmp(cur_ident,"endif")==0) /* endif */
                   {if(if_pt) blankit(if_pt); else blankit(las_pt);
                    if_cond=TRUE; if_pt=NULL;
                    continue; }
                 else if(strcmp(cur_ident,"else")==0)
                   {if(if_pt) blankit(if_pt); else blankit(las_pt);
                    if(if_cond)
                      chr=blanktoe(las_pt); /* blank to endif or else */
                    else if_cond=TRUE;
                    if_pt=NULL;
                    continue; }
                 else if(strcmp(cur_ident,"elif")==0)
                   {if(if_cond)
                      chr=blanktoe(las_pt); /* treat like an invalid else */
                    else if(!if_pt) {if_pt=las_pt; if_exp=get_io_pos();}
                    continue; }
                }
             }
           if((cur_arg=find_name(cur_ident,arg_list,arg_num))>0)
             {--cur_arg; arg_pt=arg_list[cur_arg]; end_pt=get_io_pos();
              arg_pt+=(lo=strlen(arg_pt))+1;
              beg_pt=end_pt-lo;
jumpin:
			  chr=skip_blanks(); priorchr=(*(get_io_pos()-2));
              if(chr=='#' && input_char()=='#' && (priorchr!='\\'))
                {chr=skip_blanks(); unput_char(chr); end_pt=get_io_pos();}
              else set_io_pos(end_pt);
              --end_pt; qchrs=0;
              if(num_hashes>0)
                {hash=TRUE; while(!IsHash(beg_pt)) --beg_pt;
                 if(num_hashes>1 && IsHash(beg_pt-1))
                   {beg_pt-=2; num_hashes-=2;
                    while(beg_pt>str
                          && (*beg_pt==' ' || *beg_pt=='\t' || *beg_pt=='\n'))
                      --beg_pt;
                    ++beg_pt; hash=FALSE;
                   }
                 else
                   {tmp_pt=arg_pt; qchrs=2; --num_hashes;
                    while(*tmp_pt)
                      {if(*tmp_pt=='\'' || *tmp_pt=='"') ++qchrs; ++tmp_pt; }
                   }
                }
              qchrs+=strlen(arg_pt);
              qchrs-=(1+end_pt-beg_pt);
              beg_ofs=beg_pt-str; end_ofs=end_pt-str;
              for(nn=old_sub;nn<num_sub;++nn)
                {if((beg_ofs>=mac_sub[nn].lo) && (beg_ofs<=mac_sub[nn].hi)
                    && (cur_arg==mac_sub[nn].arg))
                   {num_hashes=0;
                    if(bas_pt) {free(bas_pt); bas_pt=NULL;}
                    goto skipmac;} /* recursive macro, so skip */
                }
              if(!make_hole(end_pt+1,qchrs,(str+max_len)))
                {signal_error(ERR_MACRO); exitflag=FALSE;
                 if(bas_pt) {free(bas_pt); bas_pt=NULL;}
                 goto byebye;}
              set_io_pos(end_pt+qchrs+1); /* DEBUG io_str > end_pt ???? */
              if(qchrs)
                {for(nn=old_sub;nn<num_sub;++nn)
                   {hi=mac_sub[nn].hi; lo=mac_sub[nn].lo;
                    if(hi<beg_ofs) ;
                    else if(lo>end_ofs) {lo+=qchrs; hi+=qchrs; }
                    else if(lo<beg_ofs) {if(hi<=end_ofs) hi=end_ofs; hi+=qchrs;}
                    else if(hi>end_ofs) {lo=beg_ofs; hi+=qchrs; }
                    else {lo=beg_ofs; hi=end_ofs+qchrs; }
                    mac_sub[nn].hi=hi; mac_sub[nn].lo=lo;
                   }
                }
              mac_sub[num_sub].lo=beg_ofs; mac_sub[num_sub].hi=end_ofs+qchrs;
              mac_sub[num_sub].arg=cur_arg; ++expands;
              if(num_sub<MAX_EXPANDS) ++num_sub;
              if(hash) {*beg_pt='"'; ++beg_pt;}
              while(*arg_pt)
                {chr=(*arg_pt);
                 if(hash && (chr=='"' || chr=='\'')) {*beg_pt='\\'; ++beg_pt;}
                 *beg_pt=chr; ++beg_pt; ++arg_pt;
                }
              if(hash) *beg_pt='"';
              if(bas_pt) {free(bas_pt); bas_pt=NULL;}
             }
           else if(((nn=find_name(cur_ident,macro_list,mac_num))>0)
                   &&(num_rec<MAX_MACREC))
             {lo=0; for(hi=0;hi<num_rec;++hi) if(mac_rec[hi]==nn) ++lo;
              if(lo>=MAX_MACREC) goto skiprec;
              mac_rec[num_rec++]=nn; cur_arg=(-nn);
              beg_pt=get_io_pos(); end_pt=beg_pt; beg_pt-=strlen(cur_ident);
              arg_pt=do_recmac(&end_pt,&bas_pt,nn);
              --num_rec;
              if(!arg_pt) goto skiprec;
              goto jumpin;
             }
           else {skiprec: num_hashes=0;} /* DEBUG NEW */
          }
        else {num_hashes=0; chr=input_char();}
        skipmac: hash=FALSE;
       }
    } while(chr);
  } while(expands);
  exitflag=TRUE;
  byebye: pop_io(); num_sub=old_sub;
  return(exitflag);
}

/* Scan module def and pick out macro args.
*  Handles white space and quoted args etc.
*  mod_pt="name(a,b,c,..) .. {..}.."  arg_pt="(1,2,3,..)"
*  Note, if more args than actual parms, the excess args are assigned to
*  the last parm.
*  Then expands macro.
*/
BOOL do_macro(mod_pt,arg_pt,max_mac_len,if_flag)
 char *mod_pt,*arg_pt; int max_mac_len; BOOL if_flag;
{ char *apt,*arg_beg,*mac_pt,*arg_list[MAX_MAC_ARGS],
     nxchr,chr,wrk_buff[argbuffsize];
  BOOL exit_flag=TRUE; int arg_num=0,nn=0,len;
  arg_list[0]=NULL; push_io();
  if(*arg_pt=='(') ++arg_pt; else arg_pt="\0\0";
  set_io_str(mod_pt); error_mark();
  do chr=input_char(); while(chr!='(' && chr);
  if(!chr) goto brkerr;
  mac_pt=get_io_pos(); max_mac_len-=(mac_pt-mod_pt);
  do{if(get_ident())
       {len=strlen(cur_ident);
        nxchr=skip_blanks(); unput_char(nxchr);
        mod_pt=get_io_pos(); set_io_str(arg_pt);
        arg_beg=get_arg((BOOL)(nxchr==')'));
        /* *arg_beg is '\0' if no arg val */
        apt=fcat2(wrk_buff,cur_ident,"=");
        fcat(apt,arg_beg);
        if(chr=='#') dqstr_to_mem(apt); /* unpack a quoted arg */
        else if(chr=='%') /* eval arg */
          {push_io(); set_io_str(apt);
           yacc_parse_exp(); sprintf(apt,"%g\0",yacc_exp_value);
           pop_io();
          }
        arg_list[arg_num++]=(arg_pt=strsave(wrk_buff));
        *(arg_pt+len)='\0'; input_char(); /* skip comma after arg */
        arg_pt=get_io_pos(); set_io_str(mod_pt);
       }
     chr=input_char(); /* ',' or '#' or '%' before parameter */
    }while(chr && chr!=')');
  arg_list[arg_num]=NULL; mac_pt=get_io_pos();
  /* Note convert_module wants mac args expanded as well */
  if(chr==')') exit_flag=expand_macro(mod_pt,max_mac_len,arg_list,if_flag);
  else {brkerr: signal_error(ERR_MOD_BRK); exit_flag=FALSE;}
       /* Syntax error: module brackets missing in def */
  nn=0; while(arg_list[nn]) free(arg_list[nn++]);
  pop_io();
  return(exit_flag);
}
/* On entry: *nxpt pts after macro name in source text, mid is macro stack id.
*  skips to first non-blank chr after name & looks for macro parms.
*  expands macro.
*  Upon return base of mem buff for this expanded macro is in *bppt,
*  and *nxpt is next pos in original source text,
*  and return value is start of macro replacement text (null chr terminated).
*/
char *do_recmac(nxpt,bppt,mid)
  char **nxpt,**bppt; int mid;
{ char chr,*mac_buff_pt,*mac_arg_pt=(*nxpt); int msize;
  push_io(); set_io_str(mac_arg_pt); chr=skip_blanks(); unput_char(chr);
  mac_arg_pt=get_io_pos();
  if(chr=='(')
    {chr=skip_bodies(); if(chr!=')') goto byeerrone;  *nxpt=get_io_pos();}
  mac_buff_pt=buffer_module(stkrec(macro_list[mid-1]),(int *)&msize);
  if(!mac_buff_pt) goto byeerrone; /* Error: couldn't get macro */
  *bppt=mac_buff_pt;
  if(!do_macro(mac_buff_pt,mac_arg_pt,msize,TRUE)) goto byeerrtwo;
  set_io_str(mac_buff_pt); chr=skip_to_brace();
  if(chr!='{') goto byeerrtwo;
  mac_arg_pt=get_io_pos();
  unput_char(chr); chr=skip_braces(); unput_char(chr);
  mac_buff_pt=get_io_pos(); *mac_buff_pt='\0';
  pop_io(); /*DEBUG: added this pop ?? */
  return(mac_arg_pt);
  byeerrtwo: free(mac_buff_pt);
  byeerrone: pop_io(); *nxpt=NULL; *bppt=NULL;
  return(NULL);
}
