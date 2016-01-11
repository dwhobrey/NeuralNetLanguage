/* Name: iohi2.c   Type: Higher level I/O based routines   Date: 30 March 89
*  NeSeL Network Specification Language Preprocessor
*/

#include "globals2.h"
#include "iolo2.h"
#include "iohi2.h"
#include "nial2.h"
#include "nsltwo2.h"
#include "pathlo2.h"

net_elm *net_top=NULL;

char vrname[4][5]={"heap","pile","bin\0","net\0"};
decl_elm virnet[3]={
{(siby)TYPE_NET,(byte)COMM_INOUT,(mile)0,(mile)0,(mile)1,(yard)0,
 (foot)VIR_PILE,vrname[1],(char *)NULL,(var_elm *)NULL,(decl_elm *)&virnet[1]},
{(siby)TYPE_NET,(byte)COMM_INOUT,(mile)0,(mile)0,(mile)1,(yard)0,
 (foot)VIR_HEAP,vrname[0],(char *)NULL,(var_elm *)NULL,(decl_elm *)&virnet[2]},
{(siby)TYPE_NET,(byte)COMM_INOUT,(mile)0,(mile)0,(mile)1,(yard)0,
 (foot)VIR_BIN,vrname[2],(char *)NULL,(var_elm *)NULL,(decl_elm *)NULL}
};

decl_elm fake=
{(siby)TYPE_NET,(byte)COMM_INOUT,(mile)10000,(mile)0,(mile)0,(yard)0,
 (foot)VIR_NET,vrname[3],(char *)NULL,(var_elm *)NULL,(decl_elm *)NULL};
decl_elm *fake_pt=(decl_elm *)&fake;

/* read source file names from nss file & return fpt to network */
foot nss_init()
{ int num=0; char vernum='0',verupd='0',buffer[bigbuffsize];
  fgets(buffer,bigbuffsize,sstfp); /* check version */
  sscanf(buffer,"%*[^(](a)v%c.%c",&vernum,&verupd);
  if(vernum!=version_num) signal_error(EXIT_VERSION);
  fgets(buffer,bigbuffsize,sstfp); /* get nss_ok flg after function list pt */
  sscanf(buffer,"%*ld,%d",&num);
  if(!num) signal_error(EXIT_BADNSS,input_name);
  fgets(buffer,bigbuffsize,sstfp); /* get number of source files */
  sscanf(buffer,"%d",&num);
  source_list=malmem(char *,num);  /* mem not initialised yet */
  sour_num=num; num=0;
  while(num<sour_num && fgets(buffer,bigbuffsize,sstfp))
   {source_list[num]=NULL;
    if(!*buffer) break;
    *(buffer+strlen(buffer)-1)='\0'; /* step on \n */
    source_list[num++]=strsave(buffer);
   }
  return(ftell(sstfp));
}
void source_free()
{ if(source_list)
    {while(sour_num--) chk_free(source_list[sour_num]);
     fremem(source_list);
    }
}

/* Add output data to input list */
void add_inp_list(c_ary,invar,combop,tok)
  out_rec **c_ary; var_elm *invar; short combop; token tok;
{ int num=lst_ok(*c_ary);
  *c_ary=lmore(*c_ary,2,TYPE_STRUCT,out_rec);
  (*c_ary)[num].combop=combop; (*c_ary)[num].outok=tok;
  (*c_ary)[num++].invar=invar;
  (*c_ary)[num].combop=0; --lst_num(*c_ary);
}

/* A var intialiser may contain substruct initialisers,
*  these should be freed as well */
void clear_subvar(xpt)
  var_elm *xpt;
{ var_elm *ypt;
  while(xpt)
    {/* xpt->name and xpt->val */
     /* These are pts to root var val str, so don't free */
     chk_free(xpt->cast);
     if(xpt->subpt) clear_subvar(xpt->subpt);
     ypt=xpt; xpt=xpt->tail; fremem(ypt);}
}

void clear_var(xpt)
  var_elm *xpt;
{var_elm *ypt;
  while(xpt)
    {chk_free(xpt->name); chk_free(xpt->val); chk_free(xpt->cast);
     if(xpt->subpt) clear_subvar(xpt->subpt);
     ypt=xpt; xpt=xpt->tail; fremem(ypt);}
}

/* free all memory used by net */
void clear_net(netpt)
  net_elm *netpt;
{ decl_elm *dpt,*ept; lab_elm *lpt,*mpt; comm_elm *vpt,*wpt;
  inp_elm *spt,*tpt; out_elm *fpt;
  dpt=netpt->decls; lpt=netpt->lab; vpt=netpt->comm;
  spt=netpt->inp; fpt=netpt->out;
  if(fpt)
    {while(fpt->name) {del_token(fpt->name); clear_var(fpt->invar); ++fpt;}
     fremem(netpt->out);}
  while(dpt)
    {if(dpt==virnet) break;
     chk_free(dpt->name); chk_free(dpt->tag); clear_var(dpt->var);
     ept=dpt; dpt=dpt->tail; fremem(ept);}
  while(lpt)
    {chk_free(lpt->name); chk_free(lpt->val); chk_free(lpt->ary);
     mpt=lpt; lpt=lpt->tail; fremem(mpt);}
  while(vpt)
    {chk_free(vpt->name);
     wpt=vpt; vpt=vpt->tail; fremem(wpt);}
  clear_var(netpt->var);
  while(spt)
    {chk_free(spt->name); lfree(spt->out_list);
     tpt=spt; spt=spt->tail; fremem(tpt);}
  netpt->decls=NULL; netpt->lab=NULL; netpt->comm=NULL;
  netpt->var=NULL; netpt->inp=NULL; netpt->out=NULL;
  nul_free(netpt->tag);  nul_free(netpt->name);
}

var_elm *new_varelm(vpt)
  var_elm *vpt;
{ var_elm *new_var;
  new_var=malmem(var_elm,1);
  new_var->tail=NULL; new_var->subpt=NULL; new_var->parent=vpt;
  new_var->axvar=F; new_var->ispath=F;
  return(new_var);
}

char *get_cast(vpt,spt)
  var_elm *vpt; char *spt;
{ char *cpt;
  if(*spt=='<')
    {cpt=spt+1; while(*spt!='>') ++spt; *spt='\0';
     vpt->cast=strsave(cpt); *spt='>'; ++spt;}
  else vpt->cast=NULL;
  return(spt);
}

/* Break a struct initialiser string into var_elm sublist format
*  apt->"{...,#Dx:...,id:...,id:{...},...}"   */
void save_subvar(apt,vpt,level)
  char **apt; var_elm *vpt; byte level;
{ var_elm **ppt; char chr,*bpt,*spt=(*apt); word el;
  boolean subase=T,brkcomma;
  ppt=(var_elm **)&vpt->subpt;
  brkcomma=
    (boolean)(isalpha(*spt)||*spt=='*'||*spt=='?'||*spt=='@'||*spt=='_');
  do{if(!*spt||*spt=='{'||*spt==',') ++spt; /* step over first '{' or ',' */
     if(*spt=='}') {chr=(*spt); break;} /* a null initialiser {} */
     vpt=new_varelm(vpt);
     vpt->level=level; vpt->subase=subase; subase=F;
     *ppt=vpt; ppt=(var_elm **)&vpt->tail;
     vpt->name=spt;
     while(*spt&&(*spt!=':'))
       {if(*spt=='\\'||*spt=='.'||*spt=='/')
          {*spt=='\0'; ++spt; bpt=spt;
           while(*spt!=':') ++spt;
           goto subname;
          }
        ++spt;
       }
     bpt=NULL; *spt='\0';
     subname: ++spt; el=0;
     while(*spt && *spt!=':') {el=(word)((el*10)+((*spt)-'0')); ++spt;}
     vpt->line_num=el; if(*spt) ++spt;
     if(!bpt) spt=get_cast(vpt,spt);
     else {spt=bpt; vpt->cast=NULL; vpt->ispath=T;}
     vpt->val=spt;
     if(bpt||*spt=='{') save_subvar((char **)&spt,vpt,(byte)(level+1));
     else /* skip to comma or rcrul */
       {set_io_str(spt); chr=skip_to_var(); unput_char(chr);
        spt=get_io_pos();}
     chr=(*spt);
     if(brkcomma&&chr==',') break;
     *spt='\0';  /* terminate val string */
    }while(chr==',');
   if(chr=='}') ++spt; /* step over terminating '}' if present */
  *apt=spt;
}

var_elm *get_a_var(var_tail,buffer)
  var_elm *var_tail; char *buffer;
{ char *bpt; var_elm *new_var; word el=0;
  new_var=new_varelm(var_tail);
  new_var->level=(byte)0; new_var->subase=T;
  set_io_str(buffer);
  skip_to_colon(); bpt=get_io_pos(); *(bpt-1)='\0';
  new_var->name=strsave(buffer);
  while(*bpt && *bpt!=':') {el=(word)((el*10)+((*bpt)-'0')); ++bpt;}
  new_var->line_num=el; if(*bpt) ++bpt;
  bpt=get_cast(new_var,bpt);/* process: <type_cast>... */
  bpt=strsave(bpt); new_var->val=bpt;
  if(*bpt=='{') save_subvar((char **)&bpt,new_var,(byte)1);
  return(new_var);
}


void get_vars(net_pt,buffer)
  net_elm *net_pt; char *buffer;
{ var_elm *new_var,*var_tail=NULL,*var_old=net_pt->var;
  while(fgets(buffer,bigbuffsize,sstfp))
    {if(*buffer=='%') break;
     new_var=get_a_var(var_tail,buffer);
     if(var_tail) var_tail->tail=new_var; else net_pt->var=new_var;
     var_tail=new_var;
    }
  if(var_tail) var_tail->tail=var_old; /* incase there were some old defs */
  if(var_old) var_old->parent=var_tail;
}

void get_dec_vars(decl_pt,apt,isax)
  decl_elm *decl_pt; char *apt; boolean isax;
{ var_elm *new_var,*var_tail=NULL; char *bpt,chr;
  do{set_io_str(apt); bpt=apt;
     chr=skip_to_dec(); apt=get_io_pos(); *(apt-1)='\0';
     new_var=get_a_var(var_tail,bpt); new_var->axvar=isax;
     if(var_tail) var_tail->tail=new_var; else decl_pt->var=new_var;
     var_tail=new_var;
    }while(chr==',');
  if(var_tail) var_tail->tail=NULL;
}

/* note only nodes have a seperate comm list,
*  comms for nets coded on labels & decl ids directly */
void get_comms(net_pt,buffer)
  net_elm *net_pt; char *buffer;
{ char *bpt; comm_elm *new_comm,*comm_tail=NULL;
   while(fgets(buffer,bigbuffsize,sstfp))
     {if(*buffer=='%') break;
      new_comm=malmem(comm_elm,1);
      bpt=buffer; while(*bpt!='\n') ++bpt; *bpt='\0';
      new_comm->name=strsave(buffer);
      new_comm->tail=NULL;
      if(comm_tail) comm_tail->tail=new_comm; else net_pt->comm=new_comm;
      comm_tail=new_comm;
     }
}

var_elm *get_invar(bpt)
  char *bpt;
{ var_elm *new_var=new_varelm(NULL);
  new_var->level=(byte)0; new_var->subase=T;
  new_var->cast=NULL; new_var->name=strsave("#D0");
  bpt=strsave(bpt); new_var->val=bpt;
  save_subvar((char **)&bpt,new_var,(byte)1);
  if(new_var->subpt) new_var->line_num=new_var->subpt->line_num;
  return(new_var);
}

void get_outs(net_pt,buffer)
  net_elm *net_pt; char *buffer;
{ char *bpt; out_elm *oupt; int num=0; 
  fgets(buffer,bigbuffsize,sstfp);
  sscanf(buffer,"%d\n",&num);
  oupt=malmem(out_elm,1+num);
  net_pt->out=oupt;
  while(fgets(buffer,bigbuffsize,sstfp))
     {if(*buffer=='%') break;
      if(*buffer=='{')
        {oupt->invar=get_invar(buffer); fgets(buffer,bigbuffsize,sstfp);}
      else oupt->invar=NULL;
      bpt=buffer; while(*bpt!='\n') ++bpt; *bpt='\0';
      oupt->name=str_to_token(buffer);
      ++oupt;
     }
  oupt->name=0;
}

void get_inps(net_pt,buffer)
  net_elm *net_pt; char *buffer;
{ token tok; short el,combop,cop;
  char *bpt; var_elm *invar; inp_elm *new_inp,*inp_tail=NULL;
  while(fgets(buffer,bigbuffsize,sstfp))
    {if(*buffer=='%') break;
     new_inp=malmem(inp_elm,1);
     new_inp->name=NULL; new_inp->out_list=NULL;
     bpt=buffer; while(*bpt && *bpt!=':') ++bpt;
     if(*bpt==':')
       {*bpt='\0'; new_inp->name=strsave(buffer);
        *bpt=':'; el=0; ++bpt;
        while(*bpt && *bpt!=':') {el=(short)((el*10)+((*bpt)-'0')); ++bpt;}
        new_inp->line_num=(word)el;
        while(*bpt)
          {++bpt; combop=(short)*bpt;
           ++bpt; cop=(short)*bpt; cop^='@'; cop<<=8; combop|=cop;
           ++bpt; el=(short)get_num(&bpt,',');
           tok=net_pt->out[el-1].name;
           invar=net_pt->out[el-1].invar;
           add_inp_list((out_rec **)&new_inp->out_list,invar,combop,tok);
          }
       }
     new_inp->tail=NULL;
     if(inp_tail) inp_tail->tail=new_inp; else net_pt->inp=new_inp;
     inp_tail=new_inp;
    }
}

void get_struct(net_pt,buffer)
  net_elm *net_pt; char *buffer;
{ int args,dtyp; mile declnum=1;
  decl_elm *new_decl,*decl_tail=NULL; char *bpt,*apt;
  fgets(buffer,bigbuffsize,sstfp);
  do{/* read in struct */
     if(*buffer=='%')
       {if(*(buffer+1)=='V') get_vars(net_pt,buffer);
        break;}
     new_decl=malmem(decl_elm,1);
     bpt=buffer; while(*bpt!=':') ++bpt; *bpt='\0'; ++bpt;
     new_decl->name=strsave(buffer); new_decl->var=NULL;
     args=sscanf(bpt,"%d,%d,%ld,%d",&new_decl->size,&new_decl->nodes,
                 &new_decl->sst_pos,&dtyp);
/*     new_decl->decltype=(siby)( ((dtyp&0x30)<<1)|(dtyp&0x0f) ); */
     new_decl->decltype=(siby)dtyp;
     if(args!=4) signal_error(EXIT_FILE,input_name);
     new_decl->comcode=COMM_INOUT;
     new_decl->declnum=declnum; ++declnum;
     apt=bpt; args=4; while(args && *apt) {if((*apt++)==',') --args;}
     if(*apt!='\n')
       {bpt=apt; while(*apt!='\n') ++apt; *apt='\0';
        if(*bpt=='_') ++bpt; /*DEBUG FEB91 step on first underscore */
        new_decl->tag=strsave(bpt);}
     else new_decl->tag=NULL;
     new_decl->nodenum=0; new_decl->tail=NULL;
     if(decl_tail) decl_tail->tail=new_decl; else net_pt->decls=new_decl;
     decl_tail=new_decl;
    }while(fgets(buffer,bigbuffsize,sstfp));
    net_pt->dec_num=declnum-1;
}

void get_decls(net_pt,buffer)
  net_elm *net_pt; char *buffer;
{ decl_elm *new_decl,*decl_tail=NULL; yard nodenum=0; int num,maxdec=0,curdec;
  char *apt,*bpt,dchr;
  do{/* read in decls and auxs defs */
     if(*buffer!='#') break;
     new_decl=malmem(decl_elm,1);
     sscanf(buffer+2,"%d:%c%c%d,%d,%ld",&curdec,
            &new_decl->comcode,&dchr,
            &new_decl->size,&new_decl->nodes,&new_decl->sst_pos);
     apt=buffer+4; num=3; while(num && *apt) {if(*apt++ ==',') --num;}
     bpt=apt; while(*apt!='(') ++apt; *apt='\0';
     new_decl->name=strsave(bpt);
     new_decl->tag=NULL; new_decl->var=NULL;
     if(dchr=='N') new_decl->decltype=TYPE_NET;
     else
       {new_decl->decltype=TYPE_NODE; ++apt;
        if(*apt!=')') /* get nodes arg var initialisers */
          get_dec_vars(new_decl,apt,(boolean)(*(buffer+1)=='X'));
       }
     if(*(buffer+1)=='X' || *(buffer+1)=='?') /*DEBUG should abort on '?'*/
       {new_decl->nodenum=0; new_decl->declnum=(-curdec);}
     else
       {new_decl->nodenum=nodenum; new_decl->declnum=curdec;
        if(curdec>maxdec) maxdec=curdec;
        if(new_decl->size==0) ++nodenum; /* should never be zero */
        else nodenum+=new_decl->size*new_decl->nodes;}
     new_decl->tail=NULL;
     if(decl_tail) decl_tail->tail=new_decl; else net_pt->decls=new_decl;
     decl_tail=new_decl;
    }while(fgets(buffer,bigbuffsize,sstfp));
  net_pt->dec_num=maxdec;
}

/* get types of atom decls, a fudge really, nsl1 should do this...*/
void get_atm_typs(net_pt,bpt)
  net_elm *net_pt; char *bpt;
{ decl_elm *tpt=NULL,*dpt=net_pt->decls; char *apt,*cpt; foot fpt; 
  while(dpt) /* test each decl, if atomic get its type */
    {if(dpt->decltype==TYPE_NODE)
       {fmoveto(sstfp,dpt->sst_pos);
        fgets(bpt,bigbuffsize,sstfp);
        fgets(bpt,bigbuffsize,sstfp);
        fgets(bpt,bigbuffsize,sstfp);
        sscanf(bpt,"%ld\n",&fpt);
        fmoveto(sstfp,fpt);
        fgets(bpt,bigbuffsize,sstfp);
        apt=bpt; while(*apt && *apt!=',') ++apt;
        if(*apt!=',') signal_error(EXIT_FILE,input_name);
        ++apt; cpt=apt; while(*cpt!='\n') ++cpt;
        *cpt='\0'; if(!*apt) signal_error(EXIT_FILE,input_name);
        dpt->tag=strsave(apt);
       }
     tpt=dpt;
     dpt=dpt->tail;
    }
  if(tpt) tpt->tail=virnet; /* now add virtual nets to this net */
}

void get_lab(net_pt,buffer)
  net_elm *net_pt; char *buffer;
{ decl_elm *dpt; lab_elm *new_lab,*lab_tail=NULL; mile decl_num; int args;
  char chr,*apt,*bpt;
  do{/* read in label defs */
     if(*buffer=='%') break;
     new_lab=malmem(lab_elm,1);
     bpt=buffer; while(*bpt && *bpt!=':' && *bpt!='[') ++bpt;
     chr=(*bpt); *bpt='\0';
     new_lab->name=strsave(buffer); *bpt=chr;
     if(chr=='[')
       {apt=bpt; set_io_str(bpt); skip_to_colon();
        bpt=get_io_pos(); chr= *(bpt-1); *(bpt-1)='\0';
        new_lab->ary=strsave(apt); --bpt; *bpt=chr; }
     else new_lab->ary=NULL;
     ++bpt; new_lab->comcode=(*bpt); ++bpt;
     if(*bpt=='#') /* chk for a relative label */
       {args=sscanf(bpt,"#D%d%\n",&decl_num);
        if(args<1) signal_error(EXIT_FILE,input_name);
        dpt=net_pt->decls;
        while(dpt) {if(dpt->declnum==decl_num) break; dpt=dpt->tail;}
        new_lab->decl_pt=dpt;
        bpt+=2; while(isdigit(*bpt)) ++bpt;
       }
     else new_lab->decl_pt=fake_pt; /* label is relative */
     /* get label value, if none default to wild ary */
     if(*bpt && *bpt!='\n')
       {apt=bpt; while(*bpt!='\n') ++bpt; *bpt='\0';
        new_lab->val=strsave(apt);}
     else {if(dpt->size==1) new_lab->val=strsave(STR_ATLS);
           else new_lab->val=strsave(STR_SARY);}
     new_lab->tail=NULL;
     if(lab_tail) lab_tail->tail=new_lab; else net_pt->lab=new_lab;
     lab_tail=new_lab;
    }while(fgets(buffer,bigbuffsize,sstfp));
}

void get_line_info(net_pt,ismodule)
  net_elm *net_pt; int ismodule;
{ int args,el,ef;
  args=fscanf(sstfp,"%d,%d\n",&el,&ef);
  if(args!=2) signal_error(EXIT_FILE,input_name);
  if(ismodule) {net_pt->line_num=el; net_pt->sor_num=ef;}
  else {net_pt->line_sec=el; net_pt->sor_sec=ef;}
}

/* Get module info.
   Read in decl info and lab info.
   If it can't get module it terminates compilation.
*/
void get_new_net(net_ppt,sst_pos)
  net_elm **net_ppt; foot sst_pos;
{ net_elm *net_pt; foot alpt; int ismodule,args;
  char *bpt,*apt,buffer[bigbuffsize];
  if(!*net_ppt) *net_ppt=malmem(net_elm,1);
  net_pt= *net_ppt; net_pt->refs=1;
  net_pt->sor_num=0; net_pt->sor_sec=0; net_pt->dec_num=0;
  net_pt->line_num=(-1); net_pt->line_sec=(-1);
  net_pt->decls=NULL; net_pt->lab=NULL; net_pt->comm=NULL; net_pt->out=NULL;
  net_pt->root_flag=F; net_pt->name=NULL; net_pt->var=NULL; net_pt->inp=NULL;
  net_pt->sst_pos=sst_pos; net_pt->tag=NULL; net_pt->subofs=0; net_pt->ners=0;
  if(sst_pos<=FAKE_MAX) signal_error(EXIT_FILE,input_name);
  fmoveto(sstfp,sst_pos); fgets(buffer,bigbuffsize,sstfp);
  ismodule=sscanf(buffer,"%*d,%d",&net_pt->ners);
  ismodule=(ismodule==1);
  if(!ismodule) goto zippy; /* It's a struct def: doesn't have a ner# field */
  get_line_info(net_pt,T);
  bpt=buffer; args=2; while(args && *bpt) {if((*bpt++)==',') --args;}
  if(args) signal_error(EXIT_FILE,input_name);
  apt=bpt; while(*apt && *apt!='(') ++apt;
  if(*apt!='(') signal_error(EXIT_FILE,input_name);
  *apt='\0'; net_pt->name=strsave(bpt);
  fgets(buffer,bigbuffsize,sstfp);
  args=sscanf(buffer,"%ld\n",&alpt);
  if(args==1) /* module is a node */
    {fgets(buffer,bigbuffsize,sstfp);
     if(*(buffer+1)=='C') get_comms(net_pt,buffer);
     if(*(buffer+1)=='O') get_outs(net_pt,buffer);
     if(*(buffer+1)=='I') get_inps(net_pt,buffer);
     if(*(buffer+1)=='V') get_vars(net_pt,buffer);
     /* Once vars read in, move to nodes struct def & read in */
     fmoveto(sstfp,alpt); net_pt->root_flag=T;
     fgets(buffer,bigbuffsize,sstfp);
     zippy: /* a plain struct def will jump here from above */
     args=sscanf(buffer,"%d,",&net_pt->subofs);
     if(args!=1) signal_error(EXIT_FILE,input_name);
     bpt=buffer; while(*bpt && *bpt!=',') ++bpt;
     ++bpt; apt=bpt; while(*apt!='\n') ++apt; *apt='\0';
     if(*bpt)
       {if(*bpt=='_') ++bpt; /* step on first underscore if present */
        net_pt->tag=strsave(bpt);}  /* THIS = NULL IF A MODULE */
     get_line_info(net_pt,F);
     get_struct(net_pt,buffer);
    }
  else
    {get_decls(net_pt,buffer);
     get_lab(net_pt,buffer);
     while(*buffer=='%') /* read in output paths, input paths & vars */
       {if(*(buffer+1)=='%') break;
        *buffer='\0';
        switch(*(buffer+1))
          {case 'C':
             get_comms(net_pt,buffer); break;
           case 'I':
             get_inps(net_pt,buffer); break;
           case 'V':
             get_vars(net_pt,buffer); break;
           case 'O':
             get_outs(net_pt,buffer); break;
           case 'S': break; /* if we are reading in a node struct */
           default: return; /* shouldn't get here */
          }
       }
     get_atm_typs(net_pt,buffer);
    }
}

/* find a net in pool, if found make it the top net */
net_elm *find_net(sst_pos)
  foot sst_pos;
{ net_elm *npt=net_top,*lpt=NULL;
  while(npt)
    {if(npt->sst_pos==sst_pos)
       {if(lpt) {lpt->tail=npt->tail; npt->tail=net_top; net_top=npt;}
        return(npt);}
     lpt=npt; npt=npt->tail;}
  return(NULL);
}

/* relase a net def to pool */
void release_net(netpt)
  net_elm **netpt;
{ net_elm *apt=(net_elm *)(*netpt),*npt,*lpt,*hpt; int num;
  if(apt)
    {*netpt=(net_elm *)NULL;
     if(apt->refs<2)
       {if(apt->refs<=0) signal_error(ERR_NET_CHK);
        apt->refs=0; num=0; npt=net_top; lpt=NULL; hpt=NULL;
        while(npt) /* free the net lowest on the net stack */
          {++num; if(npt->refs==0) {apt=npt; hpt=lpt;} lpt=npt; npt=npt->tail;}
        if(num<10) return; /* exit if < 10 nets in memory */
        if(hpt) {hpt->tail=apt->tail;}
        if(apt==net_top) net_top=apt->tail;
        clear_net(apt); fremem(apt);
       }
     else --(apt->refs);
    }
}
void claim_net(net_ppt,sst_pos)
  net_elm **net_ppt; foot sst_pos;
{ net_elm *net_pt;
  if(*net_ppt) signal_error(ERR_NET_CHK);
  if((net_pt=find_net(sst_pos)))
    {*net_ppt=net_pt; if(net_pt->refs>=0) ++net_pt->refs;
    else {signal_error(ERR_NET_CHK); net_pt->refs=1;}
    }
  else
    {push_io(); get_new_net(net_ppt,sst_pos); pop_io();
     net_pt=(net_elm *)*net_ppt; net_pt->tail=net_top; net_top=net_pt;
    }
}

/* Eventually this will just flush net queue at end of compilation */
void flush_nets()
{ net_elm *npt=net_top,*apt;
  while(npt)
    {if(npt->refs!=0) printf("Net:%s:%d\n",npt->name,npt->refs);
     npt->refs=0; apt=npt; npt=npt->tail;
     clear_net(apt); fremem(apt); }
}
