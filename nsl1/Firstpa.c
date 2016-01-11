/* Name: firstpa.c   Type: first pass routines   Date: 30 March 89
*  NeSeL Network Specification Language Preprocessor
*/

#include "globals.h"
#include "iolo.h"
#include "nslone.h"
#include "general.h"
#include "nslyacc.h"

/* sorted list of identifiers to ignore in input stream */
char *cpp_def_kludge_list[]={
"__mb_cur_max","_CDECL","_FAR","_HUGE","_NEAR",
"_amblksiz","_alloca","_cdecl","_far","_huge","_near",
"_pctype","_pwctype",
"HUGE","cdecl","far","huge","near"
};
char **cpp_kludge_list=(char **)cpp_def_kludge_list;
int cpp_kludge_num=sizeof(cpp_def_kludge_list)/sizeof(char *);
int cpp_kludge_max=0;

long def_file_pos=0; /* position of cur definition in source file */
int real_last=0; /* actual last line number of object */
stack_elm  *new_stack_elm=NULL;

/* copy default kludge list to dynamic list */
void copy_kludge()
{ int num,k;   
  k=cpp_kludge_num; cpp_kludge_num=0; cpp_kludge_max=0;
  cpp_kludge_list=NULL;
  for(num=0;num<k;++num)
	{add_list_name(&cpp_kludge_list,cpp_def_kludge_list[num],
				&cpp_kludge_num,&cpp_kludge_max,10);
	}
}

void add_kludge(char *name)
{	if(!cpp_kludge_max) copy_kludge();
	add_list_name(&cpp_kludge_list,name,&cpp_kludge_num,&cpp_kludge_max,10);
}

/* Set current defintion type.
*/
void set_type(def_type)
  int def_type;
{ cur_def_type=def_type;
  def_file_pos=cur_file_pos; new_stack_elm=NULL;
}

/* Save current definition information on mem stack.
*/
void push(name,xlen,bsline,abline)
  char *name; int xlen,bsline,abline;
{ char *xpt;
  int space=sizeof(char)*(1+strlen(name)+xlen);
  unsigned int some_bytes=sizeof(stack_elm)+sizeof(char)+space;
  if((new_stack_elm=(stack_elm *)malloc(some_bytes))==(stack_elm *)NULL)
    signal_error(EXIT_OSPACE,"pushing name");
  new_stack_elm->isroot=FALSE;
  new_stack_elm->def_type = cur_def_type;
  new_stack_elm->file_num = cur_file_num;
  new_stack_elm->line_num = bsline;
  new_stack_elm->abs_line = abline;
  new_stack_elm->num_lines = 0;
  new_stack_elm->file_pos = def_file_pos;
  new_stack_elm->sst_pos = 0;
  new_stack_elm->tail = stack_top;
  stack_top = new_stack_elm;
  xpt=(char *)&new_stack_elm->name;
  while(space--) *xpt++ = *name++; *xpt='\0';
}

/* A stack of redefed names for current struct/node def is maintained
*  for later num_lines updating in scan_neuron()
*/
#define MAX_REDEF 20
stack_elm *redef_list[MAX_REDEF]; int num_redef=0;

/* Push a new neuron type onto stack and add to neuron def list.
*  If neuron type already defined, overwrite it.
*/
void push_new_type(name)
  char *name;
{ stack_elm *stack; int pos;
  push(name,0,cur_line_num,abs_line_num);
  stack=stack_top; /* Assume name not predefined for speed */
  pos=add_list_name(&module_list,(char *)&stack->name,
                    &module_num,&max_module,inc_mod);
  if(pos>0) /* Name on list already, step on old def */
    {new_stack_elm=stkrec(module_list[pos-1]);
     new_stack_elm->file_num = cur_file_num; /* update def to new def */
     new_stack_elm->line_num = cur_line_num;
     new_stack_elm->abs_line = abs_line_num;
     new_stack_elm->num_lines = 0;
     new_stack_elm->file_pos = def_file_pos;
     redef_list[num_redef++]=new_stack_elm;
     stack=stack_top; stack_top=stack_top->tail;
     free(stack);
    }
}
void add_node_tdef(name) char *name;
{ char *npt,*dpt,tmp_str[100];
  sprintf(tmp_str,"%s,%d,%s,%%s\0",name,TYPE_NODE,name);
  npt=strsave(tmp_str); dpt=npt; *(npt+strlen(name))='\0';
  while(*dpt!=',') ++dpt; *dpt='\0'; while(*dpt!=',') ++dpt; *dpt='\0';
  if(add_list_name(&tdefs_list,npt,&tdefs_num,&max_tdefs,inc_tdefs)>0)
    signal_error(WRN_TYP_MUL,name);
}
/* Scan neuron def structure for neuron declaration identifiers
*/
void scan_neuron()
{ char chr='\0';
  int tmp_line, tmp_cur_line=cur_line_num, tmp_abs=abs_line_num;
  long tmp_file_pos=def_file_pos;
  stack_elm *last_top=stack_top,*tmppt; num_redef=0;
  if(skip_braces())
    do
      {error_mark();
       if(!(chr=get_ident())) break; /* syntax error */
       push_new_type(cur_ident); add_node_tdef(cur_ident);
       if((chr=skip_blanks())=='=') chr=skip_to_sep();
      } while(chr==',');
  tmppt=stack_top; tmp_line=1+cur_line_num - tmp_cur_line;
  while(num_redef--) /* take care of any redef names */
     {redef_list[num_redef]->line_num=(int)tmp_cur_line;
      redef_list[num_redef]->abs_line=(int)tmp_abs;
      redef_list[num_redef]->file_pos=tmp_file_pos;
      redef_list[num_redef]->num_lines=(int)tmp_line;
     }
  while(tmppt!=last_top)
     {tmppt->line_num=(int)tmp_cur_line;
      tmppt->abs_line=(int)tmp_abs;
      tmppt->file_pos=tmp_file_pos;
      tmppt->num_lines=(int)tmp_line;
      tmppt=tmppt->tail;}
  new_stack_elm=NULL;
  real_last=cur_line_num;
  if(chr!=';') unput_char(chr);
}

/* Handle a struct or union def in a typedef statement.
*  Push a tag for struct/union for later reference.
*/
BOOL do_struct(wrkbuff,type_num,bsline,abline)
  char *wrkbuff; int type_num,bsline,abline;
{ char chr,*tpt,*bpt,*apt=wrkbuff;
  int qchrs; int tmp_def_type=cur_def_type;
  cur_def_type=DEF_STRUCT;
  if(type_num==TYPE_UNION) cur_def_type=DEF_UNION;
  if(!*cur_ident) /* generate a tag and write to file */
    {sprintf(cur_ident," T%d\0",num_structs++);
     qchrs=strlen(cur_ident); bpt=get_io_pos(); tpt=bpt+strlen(bpt)+qchrs+4;
     make_hole(bpt,qchrs,tpt);
     tpt=cur_ident; while(*tpt) {*bpt= *tpt; ++bpt; ++tpt;}
     get_ident();
    }
  apt=fcat2(apt," ",cur_ident);
  push(cur_ident,0,bsline,abline);
  cur_def_type=tmp_def_type;
  real_last=cur_line_num;
  chr=skip_blanks(); unput_char(chr);
  if(chr=='{')
    {chr=skip_braces();real_last=cur_line_num; return((BOOL)(chr=='}'));}
  return(TRUE);
}

/* Scan typedef statement, build list of defs */
void scan_typedef()
{ char type_spec[bigbuffsize], type_name[100], type_decl[100], tmp_str[100];
  char *spec_pt=type_spec, *name_pt=type_name, *decl_pt=type_decl;
  char chr, *decl_us, *spec_us,*buff_beg,*buff_end; 
  int pos,type_code=0,bsline=cur_line_num,abline=abs_line_num;
  BOOL valid;
  *type_spec='\0'; *decl_pt='\0';
  if(get_ident())
    {
#if 0
	/*DEBUG, node types are now added directly to tdefs_list */
     if((pos=find_name(cur_ident,module_list,module_num))>0)
       {stack_elm *stack;=stkrec(module_list[pos-1]);
        if(stack->def_type==DEF_NEURON)
          {strcpy(spec_pt,cur_ident); spec_pt+=strlen(spec_pt);
           spec_us=get_io_pos(); valid=TRUE; type_code=TYPE_NODE;
           get_ident();
           goto skip_struct;
          }
       }
     
#endif
     type_code=get_type_code((char **)&spec_pt,(char **)&spec_us,
                             (BOOL *)&valid);
     if(type_code==TYPE_STRUCT || type_code==TYPE_UNION)
       {if(!do_struct(spec_pt,type_code,bsline,abline))
          {signal_error(ERR_TYP_SYN); goto byebye;}
        *cur_ident='\0';
       } 
#if 0
	 skip_struct:;
#endif
     /* on exit type_spec contains base type specs if any */
     /* next input = start of decls, */
     /* cur_ident =  user type id if valid, or first member of decl */
     if(*cur_ident)
       {if(*type_spec&&type_code) goto typname; /* must be type name */
        if((pos=find_name(cur_ident,tdefs_list,tdefs_num))>0)
          {spec_us=tdefs_list[pos-1]+strlen(cur_ident)+1;
           sscanf(spec_us,"%d",&type_code); spec_us+=strlen(spec_us)+1;
           decl_us=spec_us+strlen(spec_us)+1; *type_spec='\0';
          }
        else /* undeclared type error */
          {signal_error(ERR_TYP_UDF,cur_ident); goto byebye;}
       }
     do
       {*type_decl='\0'; decl_pt=type_decl;
        *type_name='\0'; name_pt=type_name;
        for(;;)
          {chr=skip_blanks();
           if(chr=='*' || chr=='(') { *decl_pt=chr; ++decl_pt; }
           else break;
          }
        *decl_pt='\0'; unput_char(chr);
        if(get_ident())
          {typname: pos=find_name(cur_ident,module_list,module_num);
           if(pos>0)
             {signal_error(ERR_RED_WORD,cur_ident); goto byebye;}
             /* Attempt to redefine reserved tokens or neuron */
           strcpy(name_pt,cur_ident); *cur_ident='\0';
          }
        else
          {signal_error(ERR_TYP_ID); goto byebye;}
          /* syntax error, type id missing */
        /* start collecting all chrs read */
        buff_beg=buffer_on(); chr=skip_to_sep(); buff_end=buffer_off();
        if(!chr) {signal_error(EXIT_TYP_EOF); goto byebye;}
          /* EOF error, seperator expected */
        if(buff_end) *(buff_end-1)='\0';
        if(*type_spec) {spec_pt=type_spec; decl_pt= "%s";}
        else {spec_pt=spec_us; decl_pt=decl_us;}
        sprintf(tmp_str,"%s,%d,%s,%s%s%s\0",
                type_name,type_code,spec_pt,type_decl,decl_pt,buff_beg);
        name_pt=strsave(tmp_str); decl_pt=name_pt;
        *(name_pt+strlen(type_name))='\0';
        while(*decl_pt!=',') ++decl_pt; *decl_pt='\0';
        while(*decl_pt!=',') ++decl_pt; *decl_pt='\0';
        if(add_list_name(&tdefs_list,name_pt,
                         &tdefs_num,&max_tdefs,inc_tdefs)>0)
          {signal_error(WRN_TYP_MUL,name_pt); goto byebye;}
          /* multiply defined type  */
       } while(chr==',');
     /* all ok typedefs get to here, */
     /* fall through to skip to semicolon */
    }
  byebye: real_last=cur_line_num;
}

/* Scan struct or union declaration for id and save it.
*/
void scan_struct()
{ char chr; int bsline=cur_line_num,abline=abs_line_num;
  if(get_ident())
    {if(on_stack(cur_ident))
       {chr=skip_blanks(); unput_char(chr);
        if(chr=='{')
          signal_error(ERR_RED_WORD,cur_ident);
          /* Error, attempt to redefine reserved int or previous id */
       }
     else push(cur_ident,0,bsline,abline);
    }
  if(!skip_to_scolon()) signal_error(ERR_STU_SEM);
  real_last=cur_line_num;
}

/* Scan a macro defintion and skip main body.
*/
void scan_macro()
{ stack_elm *stack; char chr; int bsline=cur_line_num,abline=abs_line_num;
  if(get_ident())
    {if(stack=on_stack(cur_ident))
       if(!stack->line_num)
         {signal_error(ERR_RED_WORD,cur_ident); goto goodbye;}
         /* Error, attempt to redefine reserved int or previous id */
     push(cur_ident,0,bsline,abline);
    }
  else signal_error(ERR_MAC_SYN);
  chr=skip_braces();
  if(chr!='}') signal_error(ERR_MAC_RBR);
  real_last=cur_line_num;
  chr=skip_blanks();
  if(chr!=';') unput_char(chr); else {goodbye: real_last=cur_line_num;}
}

BOOL prag_var[MAX_PRAG],prag_hed[MAX_PRAG];
int prag_vv=0,prag_hh=0;
void scan_pragma()
{ char chr; int expval=1;
  if(get_ident())
    {
     if(strcmp(cur_ident,"variable")==0)
       {yacc_parse_exp(); expval=(int)yacc_exp_value;
        if(expval<0) {if(prag_vv>0) var_info=prag_var[--prag_vv];}
        else {prag_var[prag_vv++]=var_info; var_info=(BOOL)(expval!=0);}}
     else if(strcmp(cur_ident,"header")==0)
       {yacc_parse_exp(); expval=(int)yacc_exp_value;
        if(expval<0) {if(prag_hh>0) hd_info=prag_hed[--prag_hh];}
        else {prag_hed[prag_hh++]=hd_info; hd_info=(BOOL)(expval!=0);}}
     expval=0;
    }
  real_last=cur_line_num;
  if(expval) do{chr=skip_blanks();}while(chr&&chr!='\n');
}

/* Scan a neuron function defintion, or net, and skip main body.
*/
void scan_nerdef(sp) stack_elm *sp;
{ int bsline=cur_line_num,abline=abs_line_num; BOOL iserr=FALSE;
  stack_elm *stack; char *apt,*cpt,chr,buff[buffsize];
  if(get_ident())
    {if(stack=on_stack(cur_ident))
       if(!stack->line_num)
         {signal_error(ERR_RED_WORD,cur_ident); iserr=TRUE; goto byebye;}
         /* Error, attempt to redefine reserved int or previous id */
     /* get args to neuron func def & place after name */
     apt=fcat(buff,cur_ident); ++apt; cpt=apt;
     chr=skip_blanks();
     if(chr=='(') /* scan a struct path */
       {unput_char(chr);
        yacc_tmp_pt=apt; /* yacc result put in buff at yacc_tmp_pt */
        *apt='\0';
        if(!yacc_parse_parms()) {iserr=TRUE; goto byebye;}
        apt=yacc_tmp_pt; ++apt;
       }
     else unput_char(chr);
     *apt='\0';
     push(buff,(int)(apt-cpt),bsline,abline);
    }
  else {iserr=TRUE; goto misfit;} /* might be a plain node var */
  byebye:
  skip_braces();
  misfit:
  real_last=cur_line_num;
  chr=skip_blanks();
  if(chr==';') {real_last=cur_line_num; goto chk_err;}
  if(chr=='=')
    {if((chr=skip_braces())=='}')
       {real_last=cur_line_num;
        chr=skip_blanks();
        if(chr==';') {real_last=cur_line_num; goto chk_err;}
       }
    }
  unput_char(chr);
  chk_err: if(iserr) blank_off(); else sp->isroot=TRUE;
}

int star_to_code(chkfunc,basecode)
  BOOL chkfunc; int basecode;
{ char chr; int stars=0;
  while((chr=skip_blanks())=='*') stars+=STAR_ONE;
  unput_char(chr);
  if(chkfunc==1&&chr=='(')
    {chr=skip_bodies(); unput_char(chr); basecode=TYPE_PT;}
  if(basecode!=TYPE_PT) basecode+=stars;
  return('@'|basecode);
}

/* convert a base type or typdef type to it's type code
*  Assumes first ident is in cur_ident
*  positions io at next chr */
int type_to_code(chkfunc,valid,baspt)
  int chkfunc; BOOL *valid; int *baspt;
{ char *type_pt; int pos,basecode=0,stars=0; *valid=TRUE; *baspt=0;
  /*look for base type keyword, or typedef'd type int */
  if(chkfunc && strcmp(cur_ident,"va_list")==0)
    {*valid=TRUE; *baspt=STAR_VARG; *cur_ident='\0'; return(STAR_VARG);}
  while(strcmp(cur_ident,"const")==0 || strcmp(cur_ident,"volatile")==0)
     get_ident();
  if(!*cur_ident) return(0);
  if(basecode=get_base_code((char **)&type_pt)) goto checkpt;
  if((pos=find_name(cur_ident,tdefs_list,tdefs_num))<1) return(0);
  *cur_ident=0; /* clear cur_ident ready for return */
  /* convert func typedef to base type */
  if((basecode=functypedef(pos,(char **)&type_pt))<1) {*valid=FALSE; return(0);}
  if(strcmp(type_pt,"*%s")==0) stars=STAR_ONE;
  else if(strcmp(type_pt,"**%s")==0) stars=STAR_TWO;
  else if(chkfunc && strcmp(type_pt,"(*%s)()")==0) basecode=TYPE_PT;
  else if(strcmp(type_pt,"%s")!=0)
    {*valid=FALSE; return(0);} /* not a permitted user func type*/
  /* got base type, check for pointer to */
  checkpt:
  /*DEBUG3/6/93, allow this type at users own risk...*/
  /*if(basecode==TYPE_STRUCT) {*valid=FALSE; return(0);}*/
  if(basecode!=TYPE_PT) basecode|=stars;
  *baspt=basecode;
  return(star_to_code(chkfunc,basecode));
}
/* handle a variable decl, add to globals list; ignore array decls */
void do_glo_var(funchr) char funchr;
{ char *apt,*bpt;
  apt=malloc((size_t)(strlen(cur_ident)+3));
  bpt=fcat(apt,cur_ident); ++bpt;
  *bpt=funchr; ++bpt; *bpt='\0';
  update_list_name(&glo_list,apt,&glo_num,&max_glo,inc_glo);
}
/* kludge: some cpp's don't deal with #directives inset from line */
BOOL cpp_kludge()
{ 
	return(find_name(cur_ident,cpp_kludge_list,cpp_kludge_num)>0);
}
/* scan & test for a user function declaration or prototype */
void scan_user_func()
{ char funchr,chr='\0',ptchr,*apt,*fpt,*bpt,*parmstk[MAX_PARMS],buff[buffsize];
  int pos,parms,codes,basecode=0,rootcode,stars; BOOL hasvarg=FALSE,valid;
  /*look for base type keyword, or typedef'd type int */
  funchr=(char)type_to_code(0,(BOOL *)&valid,(int *)&rootcode);
  if(!valid) goto byebye; /* cur_ident has been cleared */
  if(!funchr) /* check for default type (int) functions */
    {chr=skip_blanks(); unput_char(chr);
     if(chr!='(') {*cur_ident='\0'; goto byebye;} /* NB no chk for glo vars */
     funchr='@'|TYPE_INT; rootcode=TYPE_INT;
    }
  /* should now be either at func name, or non permissible types '**' etc */
  do{hasvarg=FALSE; /* flag for variable args */
  if(chr==',')
    {*cur_ident='\0'; valid=TRUE;
     funchr=(char)star_to_code(FALSE,rootcode);
    }
  if(!*cur_ident) get_ident();
  if(*cur_ident)
    {while(cpp_kludge()) {if(!get_ident()) goto byebye;}
     chr=skip_blanks();
     if(chr!='(')
       {if(var_info&&chr!='[') do_glo_var(funchr);
        if(chr=='=') /* skip var initialiser to comma or semi */
           chr=skip_to_sep();
        if(chr==',') continue; else break;/* ignore this ident */
       }
     /* now at a valid func decl / prototype */
     fpt=buff; fpt=fcat(buff,cur_ident); ++fpt;
     parms=0; codes=0;
     /*DEBUG3/6/93, ignore arg scaning: now we always scan inorder to
     * prevent a func parm being picked up by do_glo_var by mistake */
     /* if(!(func_info||monitor||mx_info))
       {unput_char(chr); goto save_func_name;}
     */
     *fpt=funchr; ++fpt;
     do{if(get_ident())
          {hasvarg=FALSE; /* reset var arg flag */
           funchr=(char)type_to_code(1,(BOOL *)&valid,(int *)&pos);
           if(!funchr) /* parm id or a disallowed user func type*/
             {if(!valid) /* weird user type, ignore this user func */
                {chr=skip_to_rbrk(); break;}
              /* must be a parm ident */
              if(parms>=MAX_PARMS)
                {signal_error(ERR_USR_PARMS,buff);
                 chr=skip_to_rbrk(); break;}
              parmstk[parms++]=strsave(cur_ident); valid=TRUE;
             }
           else /* at an arg type specifier */
             {*fpt=funchr; ++fpt; ++codes;}
           chr=skip_to_cast(',');
          }
        else chr=input_char();
        if(chr==',') hasvarg=TRUE; /* set var arg flag */
       }while(chr && chr!=')');
     if(chr!=')') {valid=FALSE; signal_error(ERR_USR_BRK,buff);}
     else if(parms) /* need to get parm types */
       {if(codes) {valid=FALSE; signal_error(ERR_USR_ARG,buff);}
        else
          {chr=skip_blanks(); unput_char(chr);
           if(!isident(chr)) /* no types given for args, ignore func */
             {for(pos=0;pos<parms;++pos)/* default to ints */
                {*fpt=(char)('@'|TYPE_INT); ++fpt;}
              codes=parms;
             }
           else  /* get arg types */
             {do{chr='\0';
                 if(get_ident())
                   {funchr=(char)type_to_code(2,(BOOL *)&valid,(int *)&pos);
                    if(!funchr) /* weird arg type, so ignore this func */
                      {valid=FALSE; break;}
                    else /* got a type specifier, now scan its args */
                      {do{ptchr='\0';
                          if(!*cur_ident) get_ident();
                          if(!*cur_ident)
                            {chr=skip_blanks();
                             if(chr=='*')
                               {stars=1;
                                while((chr=skip_blanks())=='*') ++stars;
                                unput_char(chr);
                                ptchr=(char)(stars<<=STAR_SHFT);; get_ident();
                               }
                             else if(chr=='(')
                               {chr=skip_blanks();
                                if(chr!='*')
                                  {unput_char(chr); valid=FALSE; break;}
                                stars=1;
                                while((chr=skip_blanks())=='*') ++stars;
                                unput_char(chr);
                                if(!get_ident()) {valid=FALSE; break;}
                                chr=skip_blanks();
                                if(chr!=')')
                                  {unput_char(chr); valid=FALSE; break;}
                                ptchr=(char)(stars<<=STAR_SHFT);
                               }
                             else /* ignore this func */
                               {unput_char(chr);valid=FALSE; break;}
                            } /* must now be at an ident */
                          for(pos=0;pos<parms;++pos)
                             {if(strcmp(cur_ident,parmstk[pos])==0) break;}
                          if(pos==parms) /* undeclared id */
                            {valid=FALSE;
                             signal_error(ERR_USR_UID,buff,cur_ident); break;}
                          else
                            {apt=parmstk[pos];
                             if(*apt=='$') /* multiply defined id */
                               {valid=FALSE;
                                signal_error(ERR_USR_MID,buff,cur_ident);
                                *cur_ident='\0'; /* clr id */
                                break;}
                             *cur_ident='\0'; /* clr id */
                             *apt='$';
                             *(apt+1)=funchr|ptchr; ++codes;
                            }
                          chr=skip_blanks();
                          if(chr=='(')
                            {unput_char(chr);
                             if(ptchr) chr=skip_to_cast(',');
                             else {valid=FALSE; break;}
                            }
                          if(chr!=',' && chr!=';')
                            {unput_char(chr); valid=FALSE; break;}
                         }while(chr && chr==',');
                      }
                   }
                }while(valid && chr && chr==';');
              if(valid)
                {if(codes!=parms) {valid=FALSE; signal_error(ERR_USR_ARG,buff);}
                 for(pos=0;pos<parms;++pos)
                   {apt=parmstk[pos]; *fpt=(*(apt+1)); ++fpt;}
                }
             }
          }
       }
     else {;} /* just codes, or no codes at all */
     if(hasvarg) {*fpt=STAR_VARG; ++fpt;}
     *fpt='\0'; ++fpt; *fpt='\0'; ++fpt;
     if(!(func_info||monitor||mx_info))  /* ignore arg scaning */
       goto skip_save_func;
     if(valid) /* now add valid func to func list */
       {pos=(int)(1+fpt-buff);
        apt=(char *)malloc((size_t)pos); fpt=buff; bpt=apt;
        while(pos--) *apt++ = *fpt++;
        update_list_name(&func_list,bpt,&func_num,&max_func,inc_func);
       }
     else /* func no good, clear arg stack & return */
       {skip_save_func:
        while(parms) free(parmstk[--parms]);}
     *cur_ident='\0';
    }
  chr=skip_blanks();
    }while(chr==',');
  unput_char(chr);
  byebye: real_last=cur_line_num;
}

/* Setup vars etc for first pass.
*/
void first_setup()
{ cur_line_num=0; cur_file_num=0; cur_file_pos=0; hline=0; abs_line_num=0;
  raise_error=ERR_OK; num_errors=0; num_structs=0; glo_num=0;
  max_module=0; module_num=0; tdefs_num=0; max_tdefs=0; max_glo=0;
  func_num=0; max_func=0; func_list=NULL; glo_list=NULL;
  stack_top=(stack_elm *)NULL; module_list=NULL;
  tdefs_list=NULL;
  cur_def_type=DEF_PRAGMA; push_new_type(PRAGMA_TOKEN);
  cur_def_type=DEF_MACRO; push_new_type(MACRO_TOKEN);
  cur_def_type=DEF_NEURON; push_new_type(NEURON_TOKEN);
  cur_def_type=DEF_NET; push_new_type(NET_TOKEN);
  cur_def_type=DEF_TYPEDEF; push_new_type(TYPEDEF_TOKEN);
  cur_def_type=DEF_STRUCT; push_new_type(STRUCT_TOKEN);
  cur_def_type=DEF_UNION; push_new_type(UNION_TOKEN);
  set_type((int)DEF_NONE); error_mark();
  if(raise_error) signal_error(EXIT_INITIAL);
}
/* Parse source, first pass.
*/
void firstpass()
{ char chr; int pos; stack_elm *stack; int tin;
  first_setup();
  while(chr=skip_bodies())
    {unput_char(chr); blank_mark(); hline=0;
     if(get_ident())
       {if((pos=find_name(cur_ident,module_list,module_num))>0)
          {stack=stkrec(module_list[pos-1]);
           if(stack->line_num) set_type((int)DEF_NFUNC);
           else set_type((int)stack->def_type);
           switch(cur_def_type)
             {case DEF_NEURON: blank_on(); scan_neuron(); break;
              case DEF_TYPEDEF:
                scan_typedef();
                if(raise_error)
                  if(!skip_to_scolon()) signal_error(ERR_SEMIC);
                break;
              case DEF_UNION:
              case DEF_STRUCT: scan_struct(); break;
              case DEF_MACRO: blank_on(); scan_macro(); break;
              case DEF_PRAGMA: blank_on(); scan_pragma(); break;
              default: blank_on(); scan_nerdef(stack);
             }
           if(new_stack_elm)
             {
			   /*if(hline) tin=(int)(1+hline-new_stack_elm->line_num);
              else
			  */
				  tin=(int)(1+real_last-new_stack_elm->line_num);
              if(tin<1) {signal_error(WRN_HASH); tin=1;}
              new_stack_elm->num_lines=tin;
             }
           set_type((int)DEF_NONE);
          }
        else scan_user_func();
       } else input_char();
     blank_do();
     blank_off();
     raise_error=FALSE;
    }
}

#ifdef NSLDEBUG
int first_dump()
{ int i; stack_elm *stack=stack_top; char *str, *apt,*bpt,*cpt;
printf("firstpass\n"); /* DEBUG */
  for(i=0;i<source_num;i++) printf("[%d,%s]\n",i,source_list[i]);
  printf("Symbol table\n");
  while(stack!=NULL){
    printf("<T=%d,F=%d,L=%d,n=%d,N=%s>\n",
         stack->def_type,stack->file_num,
         stack->line_num,stack->num_lines,(char *)&stack->name);
    stack=stack->tail;
  }
  printf("Neuron list\n");
  for(i=0;i<module_num;i++) printf("(%d,%s)\n",i,module_list[i]);
  printf("Typedef list\n");
  for(i=0;i<tdefs_num;i++)
    { str=tdefs_list[i];
      apt=str+strlen(str); *apt=','; /* Dangerous */
      bpt=str+strlen(str); *bpt=','; /* Dangerous */
      cpt=str+strlen(str); *cpt=','; /* Dangerous */
      printf("(%d,%s)\n",i,str);
      *apt='\0'; *bpt='\0'; *cpt='\0'; }
  printf("Ntab=%d,ftab=%d,Ttab=%d\n",module_num,source_num,tdefs_num);
  return(1);
}
#endif
