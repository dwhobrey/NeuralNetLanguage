/* Name: conxlo2.c   Type: low level connection routines  Date: 30 March 89
*  NeSeL Neural Net Specification Language Compiler
*/
#include "globals2.h"
#include "iolo2.h"
#include "iohi2.h"
#include "nial2.h"
#include "nsltwo2.h"
#include "pathlo2.h"
#include "pathhi2.h"
#include "nslyacc2.h"
#include "conxlo2.h"

/* returns T if struct is a read only compiler struct */
boolean is_read_struct(dpt)
  decl_elm *dpt;
{ char *tag=dpt->tag;
  return((boolean)(tag && strncmp(tag,"NSL",3)==0
    && strcmp(tag,"NSLNSLtime")!=0  && strcmp(tag,"NSLNSLio")!=0)
    );
}

node_elm *copy_adrs(npt)
  node_elm *npt;
{ char *xpt,*ypt; size_t nsize;
  if(!npt) return(NULL);
  nsize=node_size(npt);
  xpt=malmem(char,nsize);
  ypt=(char *)npt;
  npt=(node_elm *)xpt;
  while(nsize--) *xpt++ = *ypt++;
  return(npt);
}

/* returns true if the two conx/node addresses are equal */
boolean adrsequal(na,nb)
 node_elm *na,*nb;
{ di_elm *a,*b; int num;
  if(!na || !nb || (na->base!=nb->base) || (na->di_num!=nb->di_num)) return(F);
  a=node_di(na); b=node_di(nb); num=na->di_num;
  while(num)
    {if((a->decl==b->decl)&&(a->indx==b->indx)) {++a; ++b; --num;}
     else return(F);}
  return(T);
}

/* return a declaration's type string */
char *decl_type(dpt,bpt)
  decl_elm *dpt; char *bpt;
{ int dtyp,stars;
  if(!dpt) return(fcat(bpt,"?\0"));
  dtyp=dpt->decltype;
  switch(dtyp&STAR_TYPE)
    {case TYPE_NODE: case TYPE_STRUCT:
       stars=(dtyp&STAR_MASK)>>STAR_SHFT;
       while(stars--) {*bpt='*'; ++bpt;}
       return(fcat(bpt,dpt->tag));
     case TYPE_NET: return(fcat(bpt,"net\0"));
     default:;
    }
  return(fcat(bpt,base_str(dtyp)));
}

/* work out symbolic adrs of item at ppt in buffer at bpt
*  returns next free char in buffer */
char *symbol_adrs(ppt,apt,indx,typs)
  path_elm *ppt; char *apt; boolean indx,typs;
{ path_elm *tpt=ppt; char *npt,*cpt=NULL,sepchr='\\',dchr;
  if(ppt)
    {while(ppt->parent) ppt=ppt->parent;
     for(;;)
       {if(indx)
          {sprintf(apt,"%d:%d\0",dechd(ppt,ppt->decl_pt),ppt->dary.hasval);
           apt+=strlen(apt);
          }
        else
          {if(ppt->net->tag) sepchr='.';
           if(ppt->partflg) dchr='|'; else dchr=sepchr;
           *apt=dchr; ++apt;
           if(typs) apt=decl_type(ppt->decl_pt,apt);
           else
             {npt=ppt->pat;
              if(!npt) {if(ppt->decl_pt) npt=ppt->decl_pt->name; else npt="?";}
              if(*npt=='#' && ppt->decl_pt)
                {cpt=npt+2; npt=ppt->decl_pt->name;
                 while(*cpt=='0') ++cpt;
                 if(*cpt=='1' && *(cpt+1)=='\0') cpt=NULL;}
              apt=fcat(apt,npt);
              if(cpt) {*apt=':'; ++apt; apt=fcat(apt,cpt); cpt=NULL;}
              sprintf(apt,"[%d]\0",ppt->dary.hasval);
              apt+=strlen(apt);
             }
          }
        if(ppt==tpt) break;
        ppt=ppt->tail;
        if(indx) {*apt=','; ++apt;}
       }
    }
  *apt='\0';
  return(apt);
}

/* Work out a node's address: similar to a fly_adrs
*  must allow for aux nodes: consult aux list via aux_top->rootpt
*  result put in buff, returns length (even adjusted) */
word node_adrs(buff,ppt,indx,typs,pare)
  char *buff; path_elm *ppt; boolean indx,typs,pare;
{ path_elm *tpt; decl_elm *dpt; aux_elm *auxpt; char *apt=buff;
  yard nodenum; word len; boolean loop,addcom=F;
  if(indx) {*apt='"'; ++apt;}
  if(ppt)
    {if(aux_top)
       {auxpt=aux_top; tpt=ppt; while(tpt->tail) tpt=tpt->tail;
        while(tpt && !tpt->net->root_flag) tpt=tpt->parent;
        if(!tpt) tpt=ppt;
        nodenum=tpt->net_base;
        loop=(boolean)(auxpt && auxpt->aux_base<=nodenum);
        addcom=loop;
        while(loop)
          {apt=symbol_adrs(auxpt->par_state.org_pt,apt,indx,typs);
           *apt=';'; ++apt;
           dpt=auxpt->par_state.ilk_pt->aux;
           if(dpt)
             {if(indx)
                {sprintf(apt,"%d\0",(-dpt->declnum)); apt+=strlen(apt);}
              else
                {if(typs) apt=decl_type(dpt,apt);
                 else apt=fcat(apt,dpt->name);}
             }
           else apt=fcat(apt,"?");
           auxpt=auxpt->tail;
           loop=(boolean)(auxpt && auxpt->aux_base<=nodenum);
           if(loop) apt=fcat(apt,",");
          }
       }
     if(pare) ppt=ppt->parent;
     if(ppt)
       {if(addcom) apt=fcat(apt,","); apt=symbol_adrs(ppt,apt,indx,typs);}
    }
  if(indx) {*apt='"'; ++apt;}
  *apt='\0'; ++apt;
  len=(word)(apt-buff); if(len&1) {*apt='\0'; ++len;}
  return(len);
}

/* Work out a conx's address: similar to a fly_adrs */
word conx_adrs(buff,npt)
  char *buff; node_elm *npt;
{ stub_elm *spt; ds_sea_elm *rpt; di_elm *dpt;
  char *apt; word num;
  apt=buff; *apt='"'; ++apt; *apt='\0';
  if(!npt) signal_error(EXIT_INT_ERR,49);
  rpt=ds_claim(npt->base);
  if(!rpt) signal_error(EXIT_CONX); /* should always have a rpt ! */
  spt=ds_subpt(rpt->ds_base);
  while(spt && !(spt->conds==NISNODE)) spt=ds_tail(spt);
  if(spt) spt=ds_tail(ds_subpt(spt)); /* spt now pts at conx root path str */
  if(!spt) signal_error(EXIT_INT_ERR,50);
  num=ds_value(spt)->str.len;
  apt=(char *)&(ds_value(spt)->str.base);
  if(!num || *apt!='"') signal_error(EXIT_INT_ERR,51);
  apt=fcat(buff,apt);
  num=npt->di_num;
  if(num)
    {*(apt-1)=';';
     dpt=node_di(npt);
     while(num)
       {sprintf(apt,"%d:%d\0",dpt->decl,dpt->indx);
        apt+=strlen(apt); ++dpt; --num;
        if(num) {*apt=','; ++apt;}
       }
     *apt='"'; ++apt; *apt='\0';
    }
  ds_release(npt->base);
  num=(word)(apt-buff); if(num&1) {*apt='\0'; ++num;}
  return(num);
}

/* Work out the address of a node struct item pointed to by ppt.
*  On entry ppt->net_base & ppt->decl_pt must be set correctly.
*  A node adrs is a list [nodenum,limb1,...,limbN]
*  where nodenum is base node number. limbN are offsets into the node's ds.
*  The net_base at root_flag ppt is node's nodenum, inside DS it is 0.
*  There is at least 1 limb & 1 more for each sublevel.
*  A limb offset is a pair (declnum,index)
*  Each limb directs us down struct relative to last limb pos
*  A sub struct var should have a hasval of zero or its array index,
*  and by this time be a simple type (non node substruct)
*  Note if no parent to ppt then it's the head of an aux net
*/
node_elm *struct_adrs(ppt)
 path_elm *ppt;
{ di_elm *dpt; node_elm *npt; int num=0;
  while(ppt) /* look for parent node, this should be up a few levels */
    {++num;
     if(ppt->net && ppt->net->root_flag) /* got parent */
       {npt=(node_elm *)malmem(char,sizeof(node_elm)+num*sizeof(di_elm));
        npt->base=ppt->net_base; /* get its node base num */
        npt->di_num=(word)num; dpt=node_di(npt);
        while(num)
          {dpt->decl=ppt->decl_pt->declnum;
           if((dpt->indx=ppt->dary.hasval)<0) signal_error(EXIT_INT_ERR,52);
           ppt=ppt->tail; --num; ++dpt;
          }
        return(npt);
       }
     ppt=ppt->parent;
    }
  signal_error(EXIT_INT_ERR,53);
}

int inp_to_num(ppt,inp)
  path_elm *ppt; inp_elm *inp;
{ inp_elm *apt=ppt->net->inp; int num=1;
  while(inp!=apt) {apt=apt->tail; ++num;}
  return(num);
}

/* given a relative inp_elm num, return pt to inp_elm */
inp_elm *num_to_inp(num)
  int num;
{ inp_elm *inp;
  if(!org_pt) signal_error(EXIT_INT_ERR,54);
  if(num<0) /* a special aux decl */
    {if((num!=AUX_BFLY) || !aux_bot) signal_error(EXIT_INT_ERR,55);
     /* fetch special inp decl from aux stack */
     return((inp_elm *)&aux_bot->inp);
    }
  inp=org_pt->net->inp;
  if(!inp) signal_error(EXIT_INT_ERR,56);
  while(num>1) {inp=inp->tail; --num;}
  return(inp);
}

/* returns true if apt is a list containing $!1 operator */
boolean is_exclam(apt)
  char *apt;
{ if(!apt || *apt!='[') return(F);
  return((boolean)(*(apt+1)>=LIST_ATONE));
}

/* see if decl was determined using $!1 operator
*  chk it's aryparents when necessary
*  chk if UA via ppt->decl_pt->size=0
*  And then Only worry about UA if ary has exclam op */
boolean has_exclam(ppt)
  path_elm *ppt;
{ if(is_exclam(ppt->dary.ary)) return(T);
  /* DEBUGJAN91 commented out following line --test */
  /* if(!ppt->decl_pt->size) return(T); */ /* DEBUG affect on rest ??*/
  if(ppt->arychild==PARY_CHILD)
    {do {if(is_exclam(ppt->pary.ary)) return(T); ppt=ppt->aryparent;}
     while(ppt);
    }
  return(F);
}

/* check if there was a prior $!1 operator or UA in this part
*  don't test cur ppt since this is what we're evaling
*/
boolean check_exclam(ppt)
  path_elm *ppt;
{ for(;;)
    {if(ppt->partflg) break;
     if(!(ppt=ppt->parent)) break;
     if(has_exclam(ppt)) return(T);
    }
  return(F);
}

void blank_part(pt)
  path_elm *pt;
{ while(pt) {pt->partflg=F; pt=pt->tail;}
}

/* move to part pos from cur ppt down */
path_elm *part_path(ppt,partnum)
  path_elm *ppt; int partnum;
{ --partnum;
  while(ppt)
   {if(ppt->partflg) --partnum;
    if(partnum<1) break;
    ppt=ppt->tail;
   }
  if(partnum!=0) return(NULL);
  return(ppt);
}

/* return cur partnum: parts go from 1..x */
int cur_part(ppt)
  path_elm *ppt;
{ int pnum=1;
  if(!ppt) return(0);
  while(ppt)
    {if(ppt->partflg) ++pnum;
     ppt=ppt->parent;}
  return(pnum);
}

/* count num of parts in path */
int max_part(ppt)
  path_elm *ppt;
{ if(!ppt) return(0);
  while(ppt->tail) ppt=ppt->tail;
  return(cur_part(ppt));
}

/* adjusts sip tree connection count by +-cadd, or isdef flags */
void adj_conx(spt,cadd,inconx)
  stub_elm *spt; int cadd,inconx;
{ boolean snow; int tum; stub_elm *apt;
  if(cadd==0&&inconx>=0) return;
  apt=ds_subpt(spt);
  if(apt && apt->type==SIP_NODE) spt=apt;
  toptop: if(!spt) return;
  if((snow=(boolean)(spt->type==SIP_NODE))) /* move up leaves */
    while(spt && spt->type==SIP_NODE) spt=ds_parent(spt);
  else /* move to root of array */
   {if(spt->type!=TYPE_ARB&&spt->type!=TYPE_NODE&&spt->info.data.size==1)
      goto skip; /* a plain node/struct member, move to node/struct root */
    while(spt)
      {if(spt->type==TYPE_ARB || spt->type==TYPE_NODE) break;
       if(spt->type==TYPE_STRUCT && inconx<0) break;
       spt=ds_parent(spt);
      }
   }
  if(!spt) return;
  loop:
  if(inconx<0) spt->isdef=T;
  else
    {if(inconx>0) tum=(ds_value(spt)->conx.in_num+=(word)cadd);
     else tum=(ds_value(spt)->conx.out_num+=(word)cadd);
     if(tum<0) signal_error(EXIT_CONX);
    }
  if(spt->type==TYPE_NODE||(spt->type==TYPE_STRUCT&&inconx<0))
    {if(!spt->hasparent) return;
     while(spt)
       {if(!spt->hasparent) goto loop; /* move to ary root */
        apt=spt; spt=ds_parent(apt); if(apt!=ds_arypt(spt)) break;}
     if(spt)
       {if(spt->type==TYPE_ARB) goto loop; else goto skip;}
     return;
    }
  else if(spt->type==TYPE_ARB||snow)
    {/* spt=ds_parent(spt);*/ skip: /* skip to parent of this struct */
     while(spt)
       {apt=spt; if(!apt->hasparent) break;
        spt=ds_parent(apt); if(apt!=ds_tail(spt)) break;
       }
    }
  goto toptop;
}

void adj_tree(spt,cnum)
  stub_elm *spt; int cnum;
{ adj_conx(spt,cnum,T);
}
/* adjust quick output cx list by adding, incing or deling conx */
void adj_bk_lst(npt,cnum)
  node_elm *npt; int cnum;
{
}

/* adjust ds tree, given a leaf adrs, adjust it's DS tree out_num's by cnum
*  if chkgp, chk it has name and path node info in it's root
*  if(cnum==0) sets cnum=1 & type of ds tree leaf in ilk_pt */
void adj_cx_ds(npt,cnum,chkgp)
  node_elm *npt; int cnum; boolean chkgp;
{ stub_elm *lpt,*spt; ds_sea_elm *tmpool,*rpt; path_elm *ppt;
  word len; int slot1,slot2; char *apt,*bpt,buff[MAX_PATH_STR];
  if(!bookeep) {adj_bk_lst(npt,cnum); if(cnum) return;}
  if(!npt) signal_error(EXIT_INT_ERR,57);
  tmpool=ds_pool; if(ds_pool) slot1=push_stub(ds_stub);
  lpt=node_to_ds(npt,(ds_sea_elm **)&rpt);
  if(ds_pool) ds_stub=restore_stub(slot1);
  if(!cnum)
    {cnum=1; ilk_pt->type=lpt->type; ilk_pt->size=lpt->info.data.size;
     if(!bookeep) goto byebye;
    }
  if(!lpt) signal_error(EXIT_CONX); /* should always have a spt ! */
  adj_conx(lpt,cnum,F);
  fast_ds_mod(rpt); spt=(stub_elm *)rpt->ds_base;
  if(chkgp && !(spt->conds==NISNODE)) /* see if root needs node info */
    {spt->conds=NISNODE; /* find node struct */
     spt=ds_subpt(spt); while(spt && !(spt->conds==NISNODE)) spt=ds_tail(spt);
     if(spt) /* if null means no node info present */
       {ppt=path_pt;
        while(ppt && !ppt->net->root_flag) ppt=ppt->parent;
        if(spt->isdef || !ppt) signal_error(EXIT_INT_ERR,58);
        len=node_adrs(buff,ppt,T,F,T);
        spt->isdef=T; lpt=ds_subpt(spt);
        ds_pool=rpt;
        if(lpt)
          {spt=ds_tail(lpt);
           apt=(char *)&(ds_value(spt)->str.base);
           slot2=push_stub(spt);
           apt=ds_meminc(apt,len);
           spt=pop_stub(slot2); lpt=ds_prior(spt);
           set_stub_size(spt,(int)(SIZ_SIP_STR+len));
          }
        else /* set up a node struct */
          {lpt=new_stub(spt,(int)(SIZ_SIP_STR+len),(int)TYPE_LONG,DS_SUB,F);
           set_stub_size(lpt,(int)SIZ_SIP_LONG);
           lpt->hastail=T; lpt->info.data.size=1;
           spt=ds_next(lpt);
           zap_stub(spt,(int)(SIZ_SIP_STR+len),(int)(TYPE_CHAR+STAR_ONE),T);
           spt->info.data.size=1;
           apt=(char *)&(ds_value(spt)->str.base);
          }
        ds_value(lpt)->num=npt->base; lpt->isdef=T;
        ds_value(spt)->str.len=len; spt->isdef=T; spt->conds=NISSTR;
        bpt=buff;
        while(len--) *apt++ = *bpt++;
       }
    }
  byebye:
  ds_release(npt->base);
  ds_pool=tmpool; if(ds_pool) ds_stub=pop_stub(slot1);
}

void get_cx_type()
{ stub_elm *lpt; ds_sea_elm *tmpool,*rpt; int slot1;
  tmpool=ds_pool; if(ds_pool) slot1=push_stub(ds_stub);
  lpt=node_to_ds(ilk_pt->node,(ds_sea_elm **)&rpt);
  ilk_pt->type=lpt->type; ilk_pt->size=lpt->info.data.size;
  ds_release(ilk_pt->node->base);
  ds_pool=tmpool; if(ds_pool) ds_stub=pop_stub(slot1);
}

void state_init()
{ inpt=NULL; outpt=NULL; til_pt=NULL; path_pt=NULL;
  ds_pool=NULL; ds_stub=NULL; org_pt=NULL; ilk_pt=NULL;
  fly_top=NULL; fly_bot=NULL; fly_pt=NULL; fly_di=NULL; flo_pt=NULL;
  flags.inp=F; flags.build=F; flags.inout=F;
  flags.pamat=F; flags.last=F; flags.fork=F;
  flags.info=F; flags.ary=F; flags.state=F; flags.fly=F;
}

void state_save(apt)
  state_elm *apt;
{ apt->flags=flags;
  apt->path_pt=path_pt; apt->rootpt=rootpt; apt->inpt=inpt; apt->outpt=outpt;
  apt->til_pt=til_pt; apt->org_pt=org_pt; apt->flo_pt=flo_pt;
  apt->ds_pool=ds_pool;
  if(ds_pool) apt->dslot=push_stub(ds_stub);
  apt->ilk_pt=ilk_pt;
  apt->fly_top=fly_top; apt->fly_bot=fly_bot;
  apt->fly_pt=fly_pt; apt->fly_di=fly_di;
}

void state_update(apt)
  state_elm *apt;
{ apt->ds_pool=ds_pool;
  if(ds_pool) apt->dslot=push_stub(ds_stub);
}

void state_restore(apt)
  state_elm *apt;
{ flags=apt->flags;
  path_pt=apt->path_pt; rootpt=apt->rootpt; inpt=apt->inpt; outpt=apt->outpt;
  til_pt=apt->til_pt; org_pt=apt->org_pt; flo_pt=apt->flo_pt;
  ds_pool=apt->ds_pool;
  if(ds_pool) ds_stub=pop_stub(apt->dslot);
  else ds_stub=NULL;
  ilk_pt=apt->ilk_pt;
  fly_top=apt->fly_top; fly_bot=apt->fly_bot;
  fly_pt=apt->fly_pt; fly_di=apt->fly_di;
}

void aux_push()
{ aux_elm *apt=malmem(aux_elm,1);
  apt->parent=aux_bot; apt->tail=NULL;
  if(aux_bot) aux_bot->tail=apt; else aux_top=apt;
  aux_bot=apt;
}

void aux_pop()
{ aux_elm *apt=aux_bot;
  if(!apt) signal_error(EXIT_INT_ERR,59);
  aux_bot=apt->parent;
  if(aux_bot) aux_bot->tail=NULL; else aux_top=NULL;
  fremem(apt);
}

/* set up a special aux inp elm. Ip=input[*] or[0..0], Op= special aux Op */
void set_aux_inp(ap,ismerge)
  inp_elm *ap; boolean ismerge;
{ if(ismerge) ap->name=strsave(STR_AUXMER); /* input[*] */
  else ap->name=strsave(STR_AUXIN); /* input[0..0] */
  ap->out_list=lmore((out_rec *)NULL,2,TYPE_STRUCT,out_rec);
  ap->out_list->combop=COMB_MASK;
  ap->out_list->outok=AUX_AOTOK;
  ap->out_list->invar=NULL;
  ap->out_list[1].combop=0; ap->out_list[1].outok=0;
  ap->out_list[1].invar=NULL;
  ap->tail=NULL;
  ap->line_num=(word)err_line_num;
}

void clr_aux_inp(ap)
  inp_elm *ap;
{ nul_free(ap->name); lfree(ap->out_list);
  ap->out_list=NULL; ap->tail=NULL;
}

void zap_ilk(ipt)
  ilk_elm *ipt;
{ ipt->type=0; ipt->size=0; ipt->indx=0;
  ipt->abnum=0; ipt->cxnum=0; ipt->part_num=0;
  ipt->endofcx=F; ipt->startaux=F; ipt->self=F; ipt->discan=F; ipt->glob=F;
  ipt->part_list=NULL; ipt->node=NULL;
  ipt->aux=NULL; ipt->opt=NULL; ipt->orefpt=NULL;
  ipt->baspt=NULL; ipt->curpt=NULL;
  ipt->veed.tabpt=NULL; ipt->veed.curpt=NULL;
}

void set_aux_ilk(ap)
  ilk_elm *ap;
{ out_rec *opt=malmem(out_rec,2);
  zap_ilk(ap);
  ap->curpt=opt; ap->baspt=opt;
  opt->combop=COMB_MASK;
  opt->outok=TOK_AUXOUT;
  opt->invar=NULL;
  opt[1].combop=0; opt[1].outok=0; opt[1].invar=NULL;
}

void clr_aux_ilk(ap)
  ilk_elm *ap;
{ nul_free(ap->curpt);
  if(ap->veed.tabpt) signal_error(EXIT_INT_ERR,60);
}

void tidy_ilk(ipt)
  ilk_elm *ipt;
{ if(ipt->cxnum==0 && ipt->endofcx)
    {if(ipt->curpt->combop&COMB_FORK)
       in_error(WRN_FRK_NUL);
     else in_error(WRN_OEL_NUL);}
  if(ipt->node)
    {in_error(WRN_CXIN_UNS);
     nul_free(ipt->node);}
  ipt->cxnum=0; ipt->endofcx=F; ipt->glob=F;
  ipt->part_num=0; nul_free(ipt->part_list);
  free_path(ipt->opt); ipt->opt=NULL; ipt->orefpt=NULL;
  ipt->startaux=F; ipt->self=F;
  if(ipt->aux)
    {ipt->aux=NULL;
     tidy_ilk((ilk_elm *)&aux_bot->ilk);
     clr_aux_inp((inp_elm *)&aux_bot->inp);
     clr_aux_ilk((inp_elm *)&aux_bot->ilk);
     aux_pop();
    }
}
