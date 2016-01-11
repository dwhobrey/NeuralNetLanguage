/* Name: secondpa.c   Type: second pass routines   Date: 30 March 89
*  NeSeL Network Specification Language Preprocessor
*/
#include "globals.h"
#include "iolo.h"
#include "nslone.h"
#include "general.h"
#include "nslyacc.h"

extern void push();

typedef struct sst_elm
{
  long run_num;
  long sst_decl_pos;
  long sst_mod_pos;
  stack_elm *mod_pos;
} sst_elm;

sst_elm *sst_list=NULL;

#define inc_struct 5
int sst_num,max_sst,struct_num=0,max_struct=0,struct_pos=0,mx_sum=0;
int recurse_num=0; 
sst_elm *recurse_first=NULL;
long decl_ary=0,decl_neurons=0,decl_num=0;

long run_num_neurons=0,mod_num_neurons=0;
long decl_file_pos=0,sst_bs=0;
long mod_file_pos=0,ner_type_pos=0;
long fun_name_pt=0; /* spot where to store function name list in sstfp */
extern int nss_ok=1; /* flag to indicate if nss file is ok */

char decl_code=0,decl_type=0,
	decl_arg_buff[argbuffsize],mod_name[max_ident];
char **struct_list=NULL;
extern char *muddle=NULL;
extern char output_name[];

stack_elm *decl_mod_pt=NULL,*stack_bot=NULL;
#define MAX_FPT 40 /* max temporarily unresolved structs */
#define MAX_ADJ 40 /* max cond_str adjustments */
stack_elm *fpt_stk[MAX_FPT],*cond_adj_stk[MAX_ADJ];
int fpt_num=0,adj_num=0;

BOOL mx_netnullok=FALSE; /* set to TRUE when a null network is allowed */ 

#define xltyp(tt) (((tt&0x30)<<1)|(tt&0x0f))

void push_sst()
{ sst_elm *top_pt;
  if(sst_num>=max_sst)
    if(!enlarge_array(&sst_list,sizeof(sst_elm),&max_sst,inc_sst))
    signal_error(EXIT_OSPACE,"stacking module");
  top_pt= sst_list+sst_num++;
  top_pt->sst_decl_pos=decl_file_pos;
  top_pt->sst_mod_pos=mod_file_pos;
  top_pt->mod_pos=decl_mod_pt;
  top_pt->run_num=run_num_neurons;
}

BOOL pop_sst()
{ sst_elm *top_pt;
  if(!sst_num) return(FALSE);
  top_pt= sst_list+ --sst_num;
  if(top_pt==recurse_first) {recurse_first=NULL; recurse_num=0;}
  decl_file_pos=top_pt->sst_decl_pos;
  mod_file_pos=top_pt->sst_mod_pos;
  decl_mod_pt=top_pt->mod_pos;
  run_num_neurons=top_pt->run_num;
  if(mod_file_pos>=0) return(TRUE);
  mod_file_pos=0; return(FALSE);
}


/* Search for a sst'ed module in sstfp.
*/
long search_sst(decl_pt)
  stack_elm *decl_pt;
{ char sst_mod_id[argbuffsize],mod_id[argbuffsize];
  long next_mod_pos,decl_mod_pos;
  if(decl_mod_pos=decl_pt->sst_pos)
    {if(decl_pt->def_type!=DEF_NET) {mod_num_neurons=1; return(decl_mod_pos);}
     strcpy(mod_id,(char *)&decl_pt->name); strcat(mod_id,decl_arg_buff);
     while(decl_mod_pos)
       {fmoveto(sstfp,decl_mod_pos);
        fscanf(sstfp,"%ld,%ld,%[^\n]",
        &next_mod_pos,&mod_num_neurons,sst_mod_id);
        if(strcmp(mod_id,sst_mod_id)==0)
          return(decl_mod_pos); /* got a sst def */
        decl_mod_pos=next_mod_pos;
    }  }
  mod_num_neurons=0;
  return(0); /* didn't find a matching sst module */
}

void update_head()
{ long next_mod_pos;
  fmoveto(sstfp,mod_file_pos);
  fscanf(sstfp,"%ld", &next_mod_pos);
  fmoveto(sstfp,mod_file_pos);
  fprintf(sstfp,"%ld,%09ld",next_mod_pos,run_num_neurons);
  fflush(sstfp);
}

/* Reverse stack */
void reverse_stack()
{ stack_elm *b_elm,*c_elm=NULL;
  do{b_elm=stack_top->tail;
     stack_top->tail=c_elm;
     c_elm=stack_top; stack_top=b_elm;
    }while(stack_top);
  stack_top=c_elm;
}

int write_slist(fil1,fil2)
  FILE *fil1,*fil2;
{ char *apt,*bpt,*ept,buffer[bigbuffsize]; stack_elm *st_pt;
  int num=1,commas,pos;
  fposeof(fil1); fmoveto(fil2,0);
  while(fgets(buffer,bigbuffsize,fil2))
    {apt=buffer;
     if(*(apt+1)=='0')
       {bpt=buffer; ++apt; commas=5; /* NOTE step over 5 commas */
        while(commas && *apt) {if(*apt==',') --commas; ++apt;}
        if(*apt=='"')
          {++apt; ept=apt; while(*ept!='"') ++ept; *ept='\0';
           if((pos=find_name(apt,neuron_list,neuron_num))>0)
             {st_pt=stkrec(neuron_list[pos-1]);
              if(st_pt->sst_pos) {fprintf(fil1,"{%ld",st_pt->nodes); bpt+=2;}
             }
           *ept='"';
          }
        apt=bpt;
       }
     fputs(apt,fil1); ++num;
    }
  return(num);
}
/* write names of user "c" functions to file */
void write_funcs(fp,f_list,f_num)
  FILE *fp; char **f_list; int f_num;
{ char *apt,*bpt,*cpt; int *funswt,*ipt,numswt=0,nn,jj,kk,gg,ee,pp;
  fposeof(fp);
  funswt=(int *)malloc((size_t)sizeof(int)*(f_num+1));
  if(core_def)
    {fprintf(fp,"char *nsl_mon_names[]={(char *)0};\n");
     fprintf(fp,
       "NSLNSLfelm nsl_mon_list[]={{0,(char (*)())0L,(char *)0L}};\n");
     fprintf(fp,"int nsl_num_mon_list=0;\n");
     fprintf(fp,"int nsl_mon_evaluate(cp)char *cp;{;}\n");
    }
  fprintf(fp,"char *nsl_%s_names[]={\n",muddle);
  ipt=funswt; pp=0;
   /* work out eventual switch case numbers & write func names */
  for(nn=0;nn<f_num;++nn)
    {apt=f_list[nn];
     fprintf(fp,"\"%s\",",apt);
     ++pp; if(pp>3) {fprintf(fp,"\n"); pp=0;}
     for(jj=0;jj<=nn;++jj)
        {bpt=f_list[jj];
         if(strcmp((apt+strlen(apt)+1),(bpt+strlen(bpt)+1))==0) break;
        }
     if(jj<nn) *ipt=funswt[jj]; else {*ipt=numswt; ++numswt;}
     ++ipt;
    }
  fprintf(fp,"(char *)0};\n");
  /* now write details on funcs */
  fprintf(fp,"NSLNSLfelm nsl_%s_list[]={\n",muddle);
  for(nn=0;nn<f_num;++nn)
    {apt=f_list[nn]; bpt=apt+strlen(apt)+1;
     if(!*bpt) bpt="FX"; /* must be a node func */
     fprintf(fp,"{%d,(char (*)())%s,\"%s\"},\n",funswt[nn],apt,bpt);
    }
  fprintf(fp,"{0,(char (*)())0L,(char *)0L}};\n");
  fprintf(fp,"int nsl_num_%s_list=%d;\n\n",muddle,f_num);
  /* now write function evaluation routine */
  fprintf(fp,
"NSLNSLfval nsl_%s_evaluate(cp,fp,sw)char *cp;NSLNSLfuni fp;int sw;\n\
{NSLNSLfval rv;switch(sw){\n",muddle);
  for(nn=0;nn<numswt;++nn)
    {fprintf(fp,"case %d:\n",nn);
     for(jj=0;jj<f_num;++jj) {if(nn==funswt[jj]) break;}
     apt=f_list[jj]; apt+=strlen(apt)+1;
     if(!*apt) apt="FX"; /* must be a node func */
     jj=cast_type(*apt); ++apt;
     if(jj==TYPE_VOID)
       fprintf(fp,"rv.typ=%d;rv.val.ii=0;(*fp.vv)(\n",TYPE_INT);
     else
       {if(jj&STAR_MASK || jj==TYPE_PT)
          {gg=TYPE_PT; bpt="pp";}/* func returns a pt */
        else
          {gg=jj;
           switch(jj)
             {case TYPE_CHAR: bpt="cc"; break;
              case TYPE_USCHAR: bpt="ucc"; break;
              case TYPE_SHORT: bpt="ss"; break;
              case TYPE_USSHORT: bpt="uss"; break;
              case TYPE_LONG: bpt="ll"; break;
              case TYPE_USLONG: bpt="ull"; break;
              case TYPE_FLOAT: bpt="ff"; break;
              case TYPE_DOUBLE: bpt="dd"; break;
              case TYPE_USINT: bpt="uii"; break;
              default: gg=TYPE_INT; bpt="ii";
             }
          }
        fprintf(fp,"rv.typ=%d;rv.val.%s=(*fp.%s)(\n",gg,bpt,bpt);
       }
     kk=0; ee=0;
     while(*apt)
       {jj=cast_type(*apt);
        if(jj==STAR_VARG)
          {*(apt+1)='\0'; cpt=""; bpt="char *"; gg=sizeof(char *);}
        else if(jj&STAR_MASK || jj==TYPE_PT)
          {cpt="*"; bpt="char **"; gg=sizeof(char *);} /* arg is a pt */
        else
          {cpt="*";
           switch(jj)
             {case TYPE_DOUBLE: bpt="double *"; gg=sizeof(double); break;
              case TYPE_FLOAT: bpt="float *"; gg=sizeof(float); break;
              case TYPE_LONG: bpt="long *"; gg=sizeof(long); break;
              case TYPE_USLONG: bpt="unsigned long *"; gg=sizeof(long); break;
              case TYPE_SHORT: bpt="short *"; gg=sizeof(short); break;
              case TYPE_USSHORT: bpt="unsigned short *"; gg=sizeof(short); break;
              case TYPE_CHAR: bpt="char *"; gg=sizeof(char); break;
              case TYPE_USCHAR: bpt="unsigned char *"; gg=sizeof(char); break;
              case TYPE_USINT: bpt="unsigned int *"; gg=sizeof(int); break;
              default: bpt="int *"; gg=sizeof(int);
             }
          }
        fprintf(fp,"%s((%s)(cp+%d))",cpt,bpt,kk);
        ++apt; kk+=gg; ++ee;
        if(*apt) fprintf(fp,",");
        if(ee==3) {fprintf(fp,"\n"); ee=0;}
       }
     fprintf(fp,"); break;\n");
    }
  fprintf(fp,"default:rv.typ=4;rv.val.ii=0;}\nreturn(rv);}\n");
}

BOOL dup_line(bp,lp) char *bp; int *lp;
{ BOOL gotline=(BOOL)(fgets(bp,bigbuffsize,garfp)!=NULL);
  if(gotline)
    {++(*lp);
     fputs(bp,hoofp);
     if(func_info) fputs(bp,funfp);
    }
  return(gotline);
}
/* merge header & struct defs to hoofp file */
void write_defs()
{ stack_elm *s_elm; int lines,styp,line_num=0,xx; BOOL gotline;
  char buff[bigbuffsize],tag[max_ident];
  reverse_stack(); s_elm=stack_top;
  /* Write out struct, union & node defs to hoo file */
  fflush(garfp); fmoveto(garfp,0);
  do{gotline=dup_line(buff,&line_num);
     while(s_elm&&(!gotline||(s_elm->abs_line<=line_num)))
       {styp=s_elm->def_type;
        if(s_elm->sst_pos>0)
         {if(styp==DEF_NEURON || styp==DEF_STRUCT || styp==DEF_UNION)
            {/*skip over def*/
             if((styp==DEF_STRUCT||styp==DEF_UNION)&&s_elm->abs_line>0)
               {xx=s_elm->num_lines;
                while(xx>1&&gotline) {gotline=dup_line(buff,&line_num); --xx;}
               } /*DEBUG ^ 1 or 2, 2 doesn't work for _time etc.. ??? */
             fmoveto(sstfp,s_elm->sst_pos);
             fgets(buff,bigbuffsize,sstfp);
             *tag='\0'; sscanf(buff,"%*ld,%s",tag);
             while(*buff!='%')
               {fgets(buff,bigbuffsize,sstfp);
                if(*buff=='%' &&
                   (*(buff+1)=='%' || *(buff+1)=='S' || *(buff+1)=='F')) break;
                *buff='\0';
               }
             lines=0;
             if(*(buff+1)!='%')
               for(;;)
                 {fgets(buff,bigbuffsize,sstfp);
                  if(*buff=='%') break;
                  if(lines==0&&*buff=='t') styp=DEF_NEURON;
                  fputs(buff,hoofp); ++lines;
                 }
             if(monitor)
               {if(styp==DEF_UNION) /*DEBUG was ...NSL%s NSL_%s... FEB91*/
                  fprintf(hoofp,"union %s NSL_%s;\n",tag,tag);
                else if(styp==DEF_STRUCT)
                  fprintf(hoofp,"struct %s NSL_%s;\n",tag,tag);
                else
                  fprintf(hoofp,"%s NSL_%s;\n",tag,tag);
               }
            }
          }
        s_elm=s_elm->tail;
       }
    }while(gotline||s_elm);
}

#ifdef NOT_FINISHED
/* NOTE: not finished. At one time this was going to write an aux net struct
* for monitor installation, but nsl1 does not know driver node numbers yet...
* Instead nsltox produces one when neither -t or -d options are used.
*/
void write_aux_net(fp,muddle) FILE *fp; char *muddle;
{ fprintf(fp,"NSLNSLnet aux_%s={%d,%d,%d,%ld,0,\n",
       net_name,num,func_num,cx_num,(nodenum-1));
  fprintf(fp,
    "  nsl_%s_funcs,%ld,nsl_%s_sst,nsl_%s_conxs,nsl_%s_nodes,\n",
    net_name,sst_bs,net_name,net_name,net_name);
  fprintf(fp,"  \"%s\",\"%s\",\"%s\",(char *)0L,\n",
     output_name,base_name,npt);
  if(aux_name)
    {fprintf(fp,
      "  &nsl_num_%s_list,&nsl_num_%s_structs,&nsl_num_%s_stindx,\n",
      aux_name,aux_name,aux_name);
    fprintf(fp,"  &nsl_num_%s_vars,nsl_%s_stindx,\n",aux_name,aux_name);
    fprintf(fp,"  nsl_%s_evaluate,nsl_%s_names,nsl_%s_list,\n",
      aux_name,aux_name,aux_name);
    fprintf(fp,"  nsl_%s_structs,nsl_%s_vars,\n",aux_name,aux_name);
    }
  else
    {fprintf(fp,"(int *)0L,(int *)0L,(int *)0L,(int *)0L,(int *)0L,\n");
    fprintf(fp,"(void (*)())0L,(char **)0L,(NSLNSLfelm *)0L,\n");
    fprintf(fp,"(NSLNSLstruct *)0L,(NSLNSLvar *)0L,");
    }
  if(drv_num) fprintf(fp,"nsl_%s_drvs,%d",net_name,drv_num);
  else fprintf(fp,"(NSLNSLdrv *)0L,0");
  fprintf(fp,"};\n");
}
#endif

/* write func defs to hoofp file, write struct info to monfp */
void write_rest()
{ stack_elm *s_elm; long next_pos;
  int ii,jj,lines,line_num,file_num,f_num=0,s_num,f_max=0;
  char chr,*apt,**f_list=NULL,buff[bigbuffsize];
  if(line_info)
    {if(pretty_C) fputs("#line",hoofp); else fputs("#",hoofp);
     fputs(" 1 \"Internally_generated\"\n",hoofp); }
  /* Write out node func defs to mon file */
  s_elm=stack_top;
  while(s_elm)
    {if(s_elm->def_type==DEF_NFUNC)
       {if(next_pos=s_elm->sst_pos)
          {/* build a sorted nodes name list */
           /* terminate name by two null chrs */
           apt=(char *)&s_elm->name; *(apt+strlen(apt)+1)='\0';
           add_list_name(&f_list,apt,&f_num,&f_max,20);
           fmoveto(sstfp,next_pos);
           fgets(buff,bigbuffsize,sstfp);
           fgets(buff,bigbuffsize,sstfp);
           sscanf(buff,"%d,%d",&line_num,&file_num);
           if(line_info)
             {if(pretty_C) fputs("#line",monfp); else fputs("#",monfp);
              fprintf(monfp," %d %s\n",line_num,source_list[file_num]);}
           lines=0; fgets(buff,bigbuffsize,sstfp); /* step over stru fpt */
           fgets(buff,bigbuffsize,sstfp);
           if(*buff=='%')
             {while(*buff=='%' && *(buff+1)!='%')
                {if(*(buff+1)=='F') break;
                 do {fgets(buff,bigbuffsize,sstfp);} while(*buff!='%');
                }
              fgets(buff,bigbuffsize,sstfp);
             }
           do{fputs(buff,monfp); ++lines;
              fgets(buff,bigbuffsize,sstfp);
             }while(*buff!='%' && *(buff+1)!='%');
       }  }
     s_elm=s_elm->tail;
    }
  /* add sorted node names list to sstfp */
  fflush(sstfp); fposeof(sstfp); next_pos=fcurpos(sstfp);
  fprintf(sstfp,"%d\n",f_num);
  for(ii=0;ii<f_num;++ii)
     fprintf(sstfp,"%s\n%ld\n",f_list[ii],stkrec(f_list[ii])->sst_pos);
  fprintf(sstfp,"%%%%\n");
  fflush(sstfp); fmoveto(sstfp,fun_name_pt);
  fprintf(sstfp,"%09ld,%02d",next_pos,nss_ok);
  if(monitor){
  if(line_info)
    {if(pretty_C) fputs("#line",monfp); else fputs("#",monfp);
     fputs(" 1 \"Monitor_information\"\n",monfp);}
  /* Write out struct list */
  fprintf(monfp,"NSLNSLstruct nsl_%s_structs[]={\n",muddle);
  s_num=write_slist(monfp,tmpfp);
  fprintf(monfp,"{0}};\nint nsl_num_%s_structs=%d;\n",muddle,s_num);
  jj=0; fprintf(monfp,"int nsl_%s_stindx[]={\n",muddle);
  for(ii=0;ii<struct_num;++ii)
    {apt=struct_list[ii]; apt+=strlen(apt)+1;
     fprintf(monfp,"%d,",*((int *)apt));
     free(struct_list[ii]);
     ++jj; if(jj>10) {fprintf(monfp,"\n"); jj=0;}
    }
  fprintf(monfp,"(-1)};\nint nsl_num_%s_stindx=%d;\n",muddle,struct_num);
  free(struct_list);
  if(glo_list) /* write var info */
    {fprintf(monfp,"NSLNSLvar nsl_%s_vars[]={\n",muddle);
     for(ii=0;ii<glo_num;++ii)
        {apt=glo_list[ii]; chr= *(apt+strlen(apt)+1);
         fprintf(monfp,"{%d,(char *)&%s,\"%s\"},\n",(int)xltyp(chr),apt,apt);
        }
     fprintf(monfp,
       "{0,(char *)0L,(char *)0L}};\nint nsl_num_%s_vars=%d;\n\n",
       muddle,glo_num);
     free(glo_list);
    }
  if(!fsym_info) write_funcs(monfp,f_list,f_num);
  }
  if(core_def)
    {fprintf(monfp,"NSLNSLstruct nsl_mon_structs[]={{0}};\n");
     fprintf(monfp,"int nsl_num_mon_structs=0;\n");
     fprintf(monfp,"int nsl_mon_stindx[]={(-1)};\n");
     fprintf(monfp,"int nsl_num_mon_stindx=0;\n");
     fprintf(monfp,"NSLNSLvar nsl_mon_vars[]={{0,(char *)0L,(char *)0L}};\n");
     fprintf(monfp,"int nsl_num_mon_vars=0;\n");
    }
 if(f_num) free(f_list);
}
/* write function name & address info to monitor file */
void write_fsyms()
{ char *apt; stack_elm *s_elm;
  /* Add node funcs to func_list */
  s_elm=stack_top;
  while(s_elm)
    {if(s_elm->def_type==DEF_NFUNC && s_elm->sst_pos)
       {/* terminate name by two null chrs */
        apt=(char *)&s_elm->name; *(apt+strlen(apt)+1)='\0';
        add_list_name(&func_list,apt,&func_num,&max_func,inc_func);
       }
     s_elm=s_elm->tail;
    }
  write_funcs(monfp,func_list,func_num);
}

void write_head(dab)
  char *dab;
{ push_sst();
  fposeof(sstfp); /* move to end of file */
  mod_file_pos=fcurpos(sstfp);
  fprintf(sstfp,"%ld,%09ld,%s%s\n",decl_mod_pt->sst_pos,(long)0,mod_name,dab);
  fprintf(sstfp,"%d,%d\n",decl_mod_pt->line_num,decl_mod_pt->file_num);
  decl_mod_pt->sst_pos=mod_file_pos;
  decl_file_pos=fcurpos(sstfp);
}

/* Write a new struct def to sst file & update neuron_list.
  studef="memtype memlabel,..."; condef="label;type;;,..."
*/
void write_new_struct(stype,newtag,studef,condef,nodes,nlines)
  stack_elm *stype; char *newtag,*studef,*condef; long nodes; int nlines;
{ push(newtag,(int)0,cur_line_num,0);
    /* later on elm is moved to bottom of stack */
  add_list_name(&neuron_list,(char *)&stack_top->name,
                &neuron_num,&max_neuron,inc_structs);
  stack_top->def_type=DEF_STRUCT;
  stack_top->file_num=stype->file_num;
  stack_top->line_num=stype->line_num;
  stack_top->num_lines=nlines+2;
  stack_top->file_pos=stype->file_pos;
  stack_top->nodes=nodes;
  fposeof(sstfp);
  stack_top->sst_pos=fcurpos(sstfp);
  stack_bot->tail=stack_top; stack_bot=stack_top;
  stack_top=stack_top->tail; stack_bot->tail=NULL;
  fprintf(sstfp,"%09ld,%s\n",nodes,newtag);
  fprintf(sstfp,"%d,%d\n",
          stype->line_num,stype->file_num);
  fprintf(sstfp,"%s",condef);
  if(studef)
    fprintf(sstfp,"%%S\ntypedef struct {\n%s}%s;\n%%%%\n",studef,newtag);
  else fprintf(sstfp,"%%%%\n");
  fflush(sstfp);
}

char *do_datinfo(laspt,name,tagid,size)
  char *laspt,*name,*tagid; long size;
{ laspt=fcat3(laspt,"\nstruct ",tagid," ");
  /* add to tail of cond_top list */
  push_bot_cond(name,(int)TYPE_STRUCT,(int)0,tagid,size);
  return(fcat2(laspt,name,";"));
}

void add_struct(name,pos)
  char *name; int pos;
{ char *apt=(char *)malloc(sizeof(char)*(strlen(name)+2)+sizeof(int));
  strcpy(apt,name);
  add_list_name(&struct_list,apt,&struct_num,&max_struct,inc_struct);
  apt+=strlen(apt)+1;
  *((int *)apt)=pos;
}

/* Add additional information to struct in buffer.
*  buffer="struct tag { ... }" laspt pts to terminal null
*  def_type = node.  cond_top pts to struct member type info.
*  Returns end of buffer.
*/
char *info_struct(stype,buffer,laspt)
  stack_elm *stype; char *buffer, *laspt;
{ cond_elm *c_elm=cond_top; int snodes=1,ctyp,val;
  BOOL groupflag=FALSE,rootflag=FALSE;
  char tagbuff[max_ident],wrkbuff[buffsize],cwrkbuf[buffsize],
    *apt=wrkbuff,*cpt=cwrkbuf,*tag;
  if(*(laspt-1)!='}') {signal_error(ERR_STU_ERR); return(laspt);}
  *apt='\0';
  while(c_elm) /* compute 'ins', 'outs' & 'size' struct details */
    {ctyp=c_elm->type_code; val=ctyp; ctyp&=STAR_TYPE;
     if(ctyp==TYPE_NODE || c_elm->ary_size==(-1))
       {if(conx_info)
          {if(!*wrkbuff)
             {tag=(char *)&stype->name; fcat2(tagbuff,"NSL",tag);
              if(monitor)
                {fprintf(tmpfp,
                 "{0,1,sizeof(%s),(char *)&NSL_%s,(char *)0,\"%s\"},\n",
                 tagbuff,tagbuff,tagbuff);
                 add_struct(tagbuff,struct_pos++);
                }
             }
           apt=fcat3(apt,"int ",c_elm->label,";\n");
           cpt=fcat2(cpt,c_elm->label,":1,1,0,4,\n"); /*NOTE 4=TYPE_INT*/
           ++snodes; /* this doubles as a line count */
           if(monitor)
             {fprintf(tmpfp,"{1,1,4,(char *)&NSL_%s.%s,\"%s\",(char *)0},\n",
                     tagbuff,c_elm->label,c_elm->label);
              ++struct_pos;
             }
       }  }
     else if(val<0 && ctyp!=TYPE_NODE)
       {if(c_elm->ary_size==0 && strcmp("input",c_elm->label)==0) groupflag=TRUE;}
     /* DEBUG change: add to known node types, not nodes with output mems */
     /* if(strcmp(c_elm->label,"output")==0) rootflag=TRUE; */
     c_elm=c_elm->tail;
    }
  --laspt; if(*(laspt-1)=='\n') --laspt;
  if(stype->isroot && (node_info || time_info)) /* add time,node structs */
    {if(time_info) laspt=do_datinfo(laspt,"time","_NSLNSLtime",(long)2);
     if(node_info) laspt=do_datinfo(laspt,"node","_NSLNSLid",(long)2);
    }
  if(groupflag && group_info) /* add group info */
    laspt=do_datinfo(laspt,"group","_NSLNSLid",(long)1);
  if(*wrkbuff && conx_info) /* add size and node structs to struct */
    {if(!raise_error)
       {apt=fcat(apt,"int total;\n");
        cpt=fcat(cpt,"total:1,1,0,4,\n");  /* NOTE this 4=TYPE_INT !!!*/
        if(monitor)
          {fprintf(tmpfp,
           "{1,1,4,(char *)&NSL_%s.total,\"total\",(char *)0},\n",tagbuff);
           ++struct_pos;
          }
        laspt=fcat3(laspt,"\n",tagbuff," ins,outs,size;");
        push_bot_cond("ins",(int)TYPE_STRUCT,(int)0,tagbuff,(long)snodes);
        push_bot_cond("outs",(int)TYPE_STRUCT,(int)0,tagbuff,(long)snodes);
        push_bot_cond("size",(int)TYPE_STRUCT,(int)0,tagbuff,(long)snodes);
        write_new_struct(stype,tagbuff,wrkbuff,cwrkbuf,(long)snodes,snodes);
    }  }
  laspt=fcat(laspt,"\n}");
  return(laspt);
}

/* Write condense string for struct to sst file.
* if a ref to struct/union/node is made see if already defined.
* if it is write fpt to it, else push stype_pt to stack_elm
* for later, and write padded zeros.
* cond_str member="label;type_code;ary_size;tag;sstpos"
*/
void write_cond_str(ner_type,force_pt)
  stack_elm *ner_type; BOOL *force_pt;
{ stack_elm *st_pt; cond_elm *c_elm=cond_top; char *apt;
  int fld_width,pos,ctyp,rec_top=fpt_num;
  long nodes=0,ary_size,sub_nodes; long fpt,tpos;
  char *tag,*tag_pt,andchr;
  tag=(char *)&ner_type->name;
  fprintf(sstfp,"%09ld,%s\n",(long)0,tag);
  fprintf(sstfp,"%d,%d\n",ner_type->line_num,ner_type->file_num);
  if(cond_top && !raise_error)
    {if(monitor)
      {if(ner_type->def_type==DEF_NEURON) apt=""; else apt="struct ";
       fprintf(tmpfp,
         "{0,1,sizeof(%s%s),(char *)&NSL_%s,(char *)0,\"%s\"},\n",
         apt,tag,tag,tag);
       add_struct(tag,struct_pos++);
      }
     while(c_elm)
      {fpt=0; fld_width=1; ctyp=c_elm->type_code; ctyp&=STAR_TYPE;
       ary_size=c_elm->ary_size; sub_nodes=c_elm->nodes;
       if(ary_size==0) ary_size=1; else if(ary_size<0) ary_size=0;
       if(ctyp==TYPE_NODE || ctyp==TYPE_STRUCT || ctyp==TYPE_UNION)
         {if((pos=find_name(c_elm->tag,neuron_list,neuron_num))>0)
            {st_pt=stkrec(neuron_list[pos-1]);
             for(pos=0;pos<rec_top;++pos)
               {if(fpt_stk[pos]==st_pt)
                 /*DEBUG not correct chk, could have a prior struct waiting
                  *to be resolved from a prior struct, not necessarily this
                  * one, so...
                  {signal_error(ERR_STU_REC,(char *)&st_pt->name); break;}
                  */
                 {if(!st_pt->sst_pos) {*force_pt=TRUE; goto skipit;}
                 }
               }
             if(raise_error) break;
             if(fpt=st_pt->sst_pos)
               {sub_nodes=st_pt->nodes; c_elm->nodes=sub_nodes;
                if(ary_size==0) ++nodes; /* DEBUG, make UA's have a 1 slot */
                else nodes+=ary_size*sub_nodes;}
             else
               {fpt_stk[fpt_num++]=st_pt; skipit: fld_width=9; fpt=(-1);}
            }
          else
            { /* Undefined symbol error */
             signal_error(ERR_STU_UDS,c_elm->tag); break;
            }
         }
       else
         {sub_nodes=1; c_elm->nodes=1;
          if(ary_size==0) ++nodes; else nodes+=ary_size;
         }
       fprintf(sstfp,"%s:%ld,%*ld,%*ld,%d,%s", c_elm->label,ary_size,
               fld_width, sub_nodes, fld_width,fpt,
               xltyp(c_elm->type_code), c_elm->tag);
       if(monitor)
         {if(ary_size>1) andchr=' '; else andchr='&';
          fprintf(tmpfp,"{%ld,%ld,%d,(char *)%cNSL_%s.%s,\"%s\",",
                  sub_nodes,ary_size,xltyp(c_elm->type_code),andchr,
                  tag,c_elm->label,c_elm->label);
          tag_pt=c_elm->tag;
          if(tag_pt && (*tag_pt)) fprintf(tmpfp,"\"%s\"},\n",tag_pt);
          else fprintf(tmpfp,"(char *)0},\n");
          ++struct_pos;
         }
       if(c_elm->tail) fputc('\n',sstfp);
       c_elm=c_elm->tail;
      }
    }
  fputc('\n',sstfp);
  ner_type->nodes=nodes; fflush(sstfp);
  tpos=fcurpos(sstfp); fmoveto(sstfp,ner_type->sst_pos);
  fprintf(sstfp,"%09ld",nodes); fflush(sstfp);
  fmoveto(sstfp,tpos);
}

/* Free cond stk.
*/
void clear_cond()
{ cond_elm *apt;
  while(cond_top) {apt=cond_top->tail; free(cond_top); cond_top=apt;}
}

/* Write neuron type structure to sst file.
*/
BOOL resolve_struct(ner_type,force_pt)
  stack_elm *ner_type; BOOL *force_pt;
{ char chr,*ner_buff_pt,*new_buff,*apt,*beg_pt=NULL;
  cond_elm *a_elm,*b_elm,*c_elm=NULL;
  unsigned int buff_len; int nn; *force_pt=FALSE;
  if(ner_type->sst_pos) return(TRUE); /*DEBUG FEB91*/
  push_io();  /* get original neuron struct def */
  ner_buff_pt=get_module(ner_type,(int *)&cur_mod_size);
  if(!ner_buff_pt) {pop_io(); return(FALSE);}
  buff_len=1000+2*cur_mod_size; new_buff=(char *)malloc(buff_len);
  if(!new_buff) signal_error(EXIT_OSPACE,"resolving struct");
  set_io_str(ner_buff_pt);
  chr=skip_to_brace(); unput_char(chr);
  if(!(apt=expand_struct(new_buff,(int)buff_len))) goto byebye;
  if(cond_top)
    {a_elm=cond_top;
     do{/* reverse cond stack */
        b_elm=a_elm->tail; a_elm->tail=c_elm; c_elm=a_elm; a_elm=b_elm;
       }while(a_elm); cond_top=c_elm;
    }
  if(add_info && ner_type->def_type==DEF_NEURON)
    apt=info_struct(ner_type,new_buff,apt);
  do{if(get_ident())
       {if(strcmp(cur_ident,(char *)&ner_type->name)==0)
          {chr=skip_blanks();
           if(chr=='=') /* get struct initialising part */
             {beg_pt=buffer_on(); skip_braces(); buffer_off();
             }
           break;
          }
       }
     else chr=skip_bodies();
    } while(chr && chr!=';');
  fcat2(apt,(char *)&ner_type->name,";\n");
  fposeof(sstfp); /* move to end of file */
  ner_type->sst_pos=fcurpos(sstfp);
  write_cond_str(ner_type,force_pt);
  if(ner_type->def_type==DEF_NEURON)
    {if(beg_pt)
       {set_io_str(beg_pt); yacc_parse_struct();
        fprintf(sstfp,"%%V\n");
        for(nn=0;nn<strs_num;++nn)
          {fprintf(sstfp,"%s\n",strs_list[nn]); free(strs_list[nn]);}
       }
     fprintf(sstfp,"%%S\n%s",new_buff);
    }
  fputs("%%\n",sstfp);
  byebye: free(new_buff);
  pop_io(); free(ner_buff_pt);
  clear_cond();
  fflush(sstfp);
  return((BOOL)!raise_error);
}

/* Adjust a sst cond string, fill out zeroed fpts.
*/
BOOL cond_adj_str(styp)
  stack_elm *styp;
{ int pos,size; long laspos,sumnodes=0,oldnodes=0,nodes=0,fpt,tpos;
  stack_elm *st_pt; char *bpt,wrkbuff[buffsize];
  fmoveto(sstfp,styp->sst_pos);
  if(!fgets(wrkbuff,buffsize,sstfp))
    {signal_error(ERR_FILE,"nsl object"); return(FALSE);}
  sscanf(wrkbuff,"%ld",&sumnodes); oldnodes=sumnodes;
  fgets(wrkbuff,buffsize,sstfp); /* step over line num info */
  while(laspos=fcurpos(sstfp), fgets(wrkbuff,buffsize,sstfp))
    {if(*wrkbuff=='%') break;
     bpt=wrkbuff; while(*bpt && (*bpt!=':')) ++bpt; *bpt='\0';
     sscanf(bpt+1,"%d,%ld,%ld,%*d,%[^\n]",&size,&nodes,&fpt,cur_ident);
     if(*cur_ident && fpt==(-1))
       {if((pos=find_name(cur_ident,neuron_list,neuron_num))>0)
          {st_pt=stkrec(neuron_list[pos-1]);
           if(!(fpt=st_pt->sst_pos) || st_pt->nodes<0)
             {signal_error(ERR_STU_ERR); return(FALSE);}
           nodes=st_pt->nodes;
           if(size==0) ++sumnodes; else sumnodes+=size*nodes; /* DEBUG */
           tpos=fcurpos(sstfp);
           fmoveto(sstfp,laspos);
           fprintf(sstfp,"%s:%d,%09ld,%09ld",wrkbuff,size,nodes,fpt);
           fflush(sstfp); fmoveto(sstfp,tpos);
          }
        else {signal_error(ERR_STU_DIS,cur_ident); return(FALSE);}
       }
    }
  if(oldnodes!=sumnodes)
    {fmoveto(sstfp,styp->sst_pos); fprintf(sstfp,"%09ld",sumnodes);
     fflush(sstfp);}
  styp->nodes=sumnodes;
  return(TRUE);
}

/* Write neuron type structure to sst file.
*/
void write_ner_type(ner_type)
  stack_elm *ner_type;
{ stack_elm *styp; int last_fpt; BOOL force;
  cond_top=NULL; adj_num=0; fpt_num=0; fpt_stk[fpt_num++]=ner_type;
  while(fpt_num && !raise_error)
    {styp=fpt_stk[--fpt_num]; last_fpt=fpt_num;
     if(!resolve_struct(styp,(BOOL *)&force)) break;
     if(last_fpt!=fpt_num||force) cond_adj_stk[adj_num++]=styp;
    }
  while(adj_num && !raise_error)
    {styp=cond_adj_stk[--adj_num];
     if(!cond_adj_str(styp)) break;
    }
  ner_type_pos=ner_type->sst_pos;
}

/* write list of strings to sst */
void write_strs(list,num,tchr)
  char **list; int num; char tchr;
{int nn;
  if(num>0)
    {fprintf(sstfp,"%%%c\n",tchr);
     for(nn=0;nn<num;++nn)
       {fputs(list[nn],sstfp);
        fprintf(sstfp,"\n");
       }
    }
}

/* Write out any var intialisers to sst */
void write_vars()
{ write_strs(vars_list,vars_num,'V');
}
void write_comms()
{ write_strs(comms_list,comms_num,'C');
}

void clear_vars()
{ int nn; outs_elm *oelm; inouts_elm *elm;
  for(nn=0;nn<labs_num;++nn) free(labs_list[nn]);
  for(nn=0;nn<aux_num;++nn) free(aux_list[nn]);
  for(nn=0;nn<outs_num;++nn) {oelm=outsrec(outs_list[nn]); free(oelm);}
  for(nn=0;nn<ins_num;++nn)
     {elm=inoutrec(ins_list[nn]); free(elm->list); free(elm);}
  for(nn=0;nn<vars_num;++nn) free(vars_list[nn]);
  for(nn=0;nn<comms_num;++nn) free(comms_list[nn]);
 vars_num=0; labs_num=0; aux_num=0; ins_num=0; outs_num=0; comms_num=0;
}

void write_ins_out()
{ inouts_elm *elm; outs_elm *oelm; long *listpt;
  int *outpt=NULL,ihand,nn,cops; unsigned int some_bytes;
  char achr,bchr; BOOL isbin;
  if(outs_num>0)
    {some_bytes= (unsigned int)(outs_num+1)*sizeof(int);
     if(!(outpt=(int *)malloc(some_bytes)))
        signal_error(EXIT_OSPACE,"writing Out paths");
     fprintf(sstfp,"%%O\n");
     fprintf(sstfp,"%d\n",outs_num);
     for(nn=0;nn<outs_num;++nn) outpt[nn]=(-1);
     for(nn=0;nn<outs_num;++nn)
       {oelm=outsrec(outs_list[nn]); ihand=oelm->num;
        if(ihand>=0) outpt[ihand]=nn;
        fputs((char *)&oelm->pat,sstfp);
        fprintf(sstfp,"\n");
    }  }
  if(ins_num>0)
    {fprintf(sstfp,"%%I\n");
     for(nn=0;nn<ins_num;++nn)
       {elm=inoutrec(ins_list[nn]); fputs((char *)&elm->pat,sstfp);
        isbin=(BOOL)!strcmp(&elm->pat,"bin");
        fprintf(sstfp,":%d:",elm->line_num);
        listpt=elm->list; listpt+=2;
        while(*listpt)
          {cops=(int)(*listpt); ihand=(int)*(listpt+1);
           if(ihand>=0) ihand=1+outpt[ihand];
           else signal_error(EXIT_INT_ERR);
           achr=(char)(cops&0xff); bchr=(char)(cops>>8); bchr|='@';
           if(isbin) achr=0x71; /* force conx ops to bin to be '-^' */
           if(achr&1) bchr|=1; /* force all '-' ops to single groups */
           fprintf(sstfp,"%c%c%d",achr,bchr,ihand); listpt+=2;
           if(*listpt) fprintf(sstfp,",");}
        fprintf(sstfp,"\n");
    }  }
  if(outpt) free(outpt);
  write_vars();
}


/* Write out sst'ed net module to sst file.*/
void write_net_sst()
{ int nn; char *spt;
  for(nn=0;nn<labs_num;++nn)
     {spt=labs_list[nn];
      if(*spt!='#') *(spt+strlen(spt))=':';
      fprintf(sstfp,"%s\n",spt);
     }
  write_ins_out();
  fputs("%%\n",sstfp);
  fflush(sstfp);
}

/* Write out sst'ed neuron module to sst file.
*/
void write_neuron_sst(str)
  char *str;
{ fprintf(sstfp,"%ld\n",ner_type_pos);
  write_comms(); write_ins_out();
  fprintf(sstfp,"%%F\n");
  fputs(str,sstfp);
  fflush(sstfp);
}

/* Parse a net module.
*  Expand and add labels, Ins and Outs to lists.
*  On entry str_pt is start of module.
*  return TRUE if ok
*/
BOOL parse_net(str_pt)
  char *str_pt;
{ char chr; set_io_str(str_pt);
  do {chr=skip_bodies();} while(isalnum(chr) || chr=='_');
  if(chr!=')')
    {signal_error(ERR_MOD_COMM,(char *)&decl_mod_pt->name); return(TRUE);}
  return(yacc_parse_decl());
}

/* Parse neuron module.
*  type name(args) type_list; comms_list; { .$.. }
*  => void name(NSLNSLnet *net,struct type *node)
*     { (node->..) }
*  returns TRUE if ok
*/
BOOL parse_neuron(str_pt,size)
  char *str_pt; int size;
{ int curly=1,pos,gnum=0,gmax=0; stack_elm *stype_pt;
  char *bspt=str_pt,**glist=NULL,*apt,*bpt,*cpt,*beg_pt,*exp_pt,
    chr,wrkbuff[buffsize];
  set_io_str(str_pt); ner_type_pos=0;
  if(!get_ident()) signal_error(ERR_MOD_TYP,(char *)&decl_mod_pt->name);
  else /* get neuron struct type template to save */
    {if( (pos=find_name(cur_ident,neuron_list,neuron_num))<1 )
       {signal_error(ERR_COM_DIS,cur_ident);
        return(TRUE);} /* ner type disappeared?*/
     stype_pt=stkrec(neuron_list[pos-1]);
     if(stype_pt->def_type!=DEF_NEURON)
       signal_error(ERR_MOD_TYP,(char *)&decl_mod_pt->name); /*type mismatch*/
     else
       {if(!(ner_type_pos=stype_pt->sst_pos)) write_ner_type(stype_pt);}
    }
  error_mark();
  do chr=skip_bodies(); while(chr && chr!=')');
  if(chr!=')')
    {signal_error(ERR_MOD_COMM,(char *)&decl_mod_pt->name); return(TRUE);}
  if(yacc_parse_comms())
    {sprintf(wrkbuff,"void %s(NSLNSLnet *net,%s *node)\n{",
             (char *)&decl_mod_pt->name,(char *)&stype_pt->name);
     if(replace_str(str_pt,wrkbuff,(str_pt+size)))
       {do{if(get_ident())
             {if(strcmp(cur_ident,"return")==0)
                {beg_pt=get_io_pos()-6; chr=skip_blanks();
                 if(chr!=';')
                   {unput_char(chr); exp_pt=buffer_on(); chr=skip_to_scolon();
                    buffer_off();
                    fcat3(wrkbuff," {node->output= ",exp_pt, "return;} ");
                    if(!replace_str(beg_pt,wrkbuff,(str_pt+size)))
                       {signal_error(ERR_BUF_OVR,(char *)&decl_mod_pt->name);
                        break;}
                    set_io_pos(beg_pt);
                   }
                } /* replace node with struct only in var defs */
              continue;
             }
           chr=skip_quoted();
           if(chr=='{') ++curly;
           else if(chr=='}') --curly;
           else if(chr=='$')
             {beg_pt=get_io_pos()-1;
              chr=skip_blanks();
              if(chr=='$') /* chk for global inputs */
                {apt=get_io_pos(); bpt=fcat(wrkbuff,"$$");
                 do {chr=input_char();} while(isident(chr));
                 if(chr==':')
                   {cpt=get_io_pos()-1; *cpt='\0';
                    bpt=fcat(bpt,apt); *cpt=chr;}
                 else
                   {bpt=fcat(bpt,"global"); set_io_str(apt);}
                 chr=skip_blanks(); if(chr=='.') chr=skip_blanks();
                 apt=get_io_pos()-1;
                 while(isident(chr)||chr=='.') chr=input_char();
                 cpt=get_io_pos()-1; *cpt='\0';
                 if(*apt) {bpt=fcat2(bpt,"/",apt); ++bpt; fcat(bpt,apt);}
                 else {++bpt; fcat(bpt,"input");}
                 *cpt=chr;
                 if(*bpt) /* check we have a valid in path */
                   if((pos=add_list_name(&glist,wrkbuff,&gnum,&gmax,10))<1)
                     {save_ins(bpt,(int)save_outs(wrkbuff),(int)'@',TRUE);
                      glist[-pos]=strsave(wrkbuff);
                     }
                 set_io_str(apt); chr=input_char();
                }
              apt="node->";
              if(chr!='.')
                {unput_char(chr);
                 if(!isalpha(chr)) apt="node";
                }
              if(!replace_str(beg_pt,apt,(str_pt+size)))
                {signal_error(ERR_BUF_OVR,(char *)&decl_mod_pt->name); break;}
             }
          }while(chr && curly);
        if(chr=='}') /* now skip over initialising part */
          {beg_pt=get_io_pos(); chr=skip_blanks();
           if(chr=='=')
             {/* parse initialising part, build list in vars_list */
              error_mark();
              yacc_parse_finit();
             }
           else chr=';'; /* just in case nerdef at eof */
           fcat(beg_pt,"\n%%\n");
           /* semi colon expected now...*/
          }
        if(!chr) signal_error(ERR_NER_SYN);
       }
     else
      {signal_error(ERR_BUF_OVR,(char *)&decl_mod_pt->name);}/*Buff Overflow!*/
    }
  if(glist)
    {while(gnum) free(glist[--gnum]);
     free(glist);
    }
  return(TRUE);
}

char *extract_nodes(sip)
  int *sip;
{ char *bpt=NULL,*apt,**mpt=module_list; stack_elm *spt;
  int num=module_num,kk=0;
  mx_info=FALSE; mx_sum+=512; /* enough space for macro expanding */
  bpt=(char *)malloc((size_t)mx_sum);
  if(!bpt) return(NULL);
  apt=fcat(bpt,"network(){\n");
  while(num--)
    {spt=stkrec(*mpt); ++mpt;
     if(spt->def_type==DEF_NFUNC)
       {apt=fcat2(apt,(char *)&spt->name,"();\n"); ++kk;}
    }
  fcat(apt,"}\n\0");
  *sip=mx_sum;
  mx_netnullok=(BOOL)(kk==0);
  return(bpt);
}

/* Convert to SST format, save in sst file.
*  returns TRUE if ok
*/
BOOL convert_module()
{ char *mod_buff_pt;
  BOOL exok=TRUE,exit_flag=FALSE,net_flag; /* get original module def */
  if(mx_info) mod_buff_pt=extract_nodes((int *)&cur_mod_size);
  else mod_buff_pt=buffer_module(decl_mod_pt,(int *)&cur_mod_size);
  if(!mod_buff_pt) return(FALSE); /* Error: couldn't get module */
  net_flag=(BOOL)(decl_mod_pt->def_type==DEF_NET);
  if(net_flag) /* handle a net decl */
    {if(do_macro(mod_buff_pt,decl_arg_buff,(int)cur_mod_size,TRUE))
       {exit_flag=parse_net(mod_buff_pt);
        if(exit_flag)
          {if(labs_num||outs_num||ins_num||vars_num)
             {write_head(decl_arg_buff); write_net_sst();
              run_num_neurons=0;
             }
           else exok=FALSE;
          }
       }
    }
  else /* handle a neuron decl */
    {exit_flag=parse_neuron(mod_buff_pt,(int)cur_mod_size);
     if(exit_flag)
       {write_head("()"); write_neuron_sst(mod_buff_pt);
        run_num_neurons=1;
       }
    }
  clear_vars();
  free(mod_buff_pt);
  return((BOOL)(exok && exit_flag));
}

BOOL get_decl()
{ int val; long dummy_pos;
  decl_file_pos=fcurpos(sstfp);
  val=fscanf(sstfp,"#%c%ld:%c%*c%ld,%ld,%ld,%[^(]%[^\n]\n",&decl_type,
          &decl_num,&decl_code,&decl_ary,&decl_neurons,
          &dummy_pos,mod_name,decl_arg_buff);
  if(decl_type!='X' && decl_type!='D') return(FALSE);
  return((BOOL)(val==8));
}

void put_decl(decl_mod_pos,deftype)
  long decl_mod_pos; int deftype;
{ char dchr; decl_neurons=mod_num_neurons;
  if(decl_type=='D') run_num_neurons+=decl_neurons*decl_ary;
  if(deftype==DEF_NET) dchr='N'; else dchr='A';
  fmoveto(sstfp,decl_file_pos);
  fprintf(sstfp,"#%c%02ld:%c%c%ld,%09ld,%010ld,%s%s\n",
          decl_type,decl_num,decl_code,dchr,
          decl_ary,decl_neurons,decl_mod_pos,mod_name,decl_arg_buff);
  fflush(sstfp);
  decl_file_pos=fcurpos(sstfp); /* this fudge is needed due to quirk with r/w*/
}

/* See if next module to reduce is already being reduced.
* Recursion error if so.
*/
BOOL recursive_sst(mod_pt)
  stack_elm *mod_pt;
{ int nn=sst_num; sst_elm *top_pt=sst_list;
  while(nn--)
    {if(top_pt->mod_pos==mod_pt)
       {if(!recurse_first) recurse_first=top_pt;
        return(TRUE); }
     ++top_pt;
    }
  return(FALSE);
}

void second_errors()
{ int nn;
  if(undef_num>0)
    {fprintf(logfp,"Undefined Symbols:\n");
     for(nn=0;nn<undef_num;++nn) fprintf(logfp,"%s\n",undef_list[nn]);}
  if(muldef_num>0)
    {fprintf(logfp,"Multiply defined labels:\n");
     for(nn=0;nn<muldef_num;++nn) fprintf(logfp,"%s\n",muldef_list[nn]);}
  if(undef_num>0) signal_error(EXIT_UNDEF);
}

/* Setup for secondpass. Adjust symbol table etc.
* i.e. make sure all names on module_list and sorted.
*/
void second_setup()
{ stack_elm *stack=stack_top; int stk_type,nn;
  sst_list=(sst_elm *)NULL; neuron_list=NULL; muldef_list=NULL;
  undef_list=NULL; raise_error=FALSE; strs_list=NULL; aux_list=NULL;
  labs_list=NULL; ins_list=NULL; vars_list=NULL; outs_list=NULL;
  comms_list=NULL; macro_list=NULL; id_list=NULL;
  neuron_num=0; max_neuron=0; max_undef=0; max_muldef=0; max_mac=0; max_id=0;
  module_num=0; max_strs=0; strs_num=0; mac_num=0; aux_num=0; max_aux=0;
  sst_num=0; labs_num=0; ins_num=0; vars_num=0; outs_num=0; muldef_num=0;
  max_labs=0; max_ins=0; max_vars=0; max_outs=0; max_sst=0; undef_num=0;
  comms_num=0; id_num=0; max_comms=0;
  run_num_neurons=0;
  stack_bot=stack_top;
  while(stack)
    {stk_type=stack->def_type;
     if(stk_type==DEF_NET)
        {modadd: add_list_name(&module_list,(char *)&stack->name,
                               &module_num,&max_module,inc_mod); }
     else if(stk_type==DEF_NFUNC)
       {if(mx_info) mx_sum+=strlen((char *)&stack->name)+5;  goto modadd;}
     else if(stk_type==DEF_MACRO)
        {add_list_name(&macro_list,(char *)&stack->name,
                      &mac_num,&max_mac,inc_mac); }
     else if(stack->line_num>0 && (stk_type==DEF_NEURON
             || stk_type==DEF_STRUCT || stk_type==DEF_UNION))
        add_list_name(&neuron_list,(char *)&stack->name,
                      &neuron_num,&max_neuron,inc_structs);
     stack_bot=stack; stack=stack->tail;
    }
  fprintf(sstfp,version,version_num,version_upd);
  fun_name_pt=fcurpos(sstfp);
  fprintf(sstfp,"%09ld,00\n%d\n",0L,source_num);
  for(nn=0;nn<source_num;++nn)
     fprintf(sstfp,"%s\n",source_list[nn]);
  decl_file_pos=fcurpos(sstfp); sst_bs=decl_file_pos;
}

void secondpass()
{ int pos; long decl_mod_pos;
  second_setup();
  mod_file_pos=(-1); decl_mod_pt=NULL;
  strcpy(mod_name,"network"); strcpy(decl_arg_buff,"()");
  if((pos=find_name(mod_name,module_list,module_num))<1)
    {if(func_info)
       {appendfile(hoofp,garfp);
        if(func_info) appendfile(funfp,garfp);
        goto skipnet;}
     signal_error(EXIT_ROOT);
    }
  goto exmod; /* jump right in... */
  for(;;)
    {raise_error=FALSE;
     if(get_decl())
       {if((pos=find_name(mod_name,module_list,module_num))<1) continue;
          /*yacc parse will have stacked name */
        exmod:  /* entry point for main net */
        decl_mod_pt=stkrec(module_list[pos-1]);
        if(recursive_sst(decl_mod_pt))
          {++recurse_num;
           if(recurse_num>recurse_max)
             {signal_error(ERR_SEC_REC,(char *)&decl_mod_pt->name); continue;
              /* recursively defined module, skip this decl */
             }
          }
        if(decl_mod_pos=search_sst(decl_mod_pt)) /* module has been sst'ed */
          {put_decl(decl_mod_pos,decl_mod_pt->def_type);}
        else
          {if(!convert_module()) /* treat as undefined if err*/
             {if(decl_type!='X'&&!mx_netnullok) /* only aux defs can be null nets */
                signal_error(ERR_NUL_NET,(char *)&decl_mod_pt->name);
              continue;}
           /*DEBUG keep going to trap multiple syntax errs etc*/
          }
       }
     else
       {update_head();          /* save total num ners & pop a decl */
        if(!pop_sst()) break;   /* we are done */
       }
     fmoveto(sstfp,decl_file_pos);
    }
  write_defs();
  write_rest();
  skipnet:
  if(func_info) write_funcs(funfp,func_list,func_num);
  if(fsym_info) write_fsyms();
  second_errors();
}
