/* Name: conxhi2.c   Type: third pass routines   Date: 30 March 89
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
#include "conxhi2.h"

extern void fly_aux(),do_inline_var(),do_network();
extern long nsl_aseed,nsl_bseed,nsl_cseed;

decl_elm *aux_decl(refpt,appt)
  path_elm *refpt; char **appt;
{ decl_elm *dpt=refpt->net->decls; int auxnum;
  auxnum=(-(int)get_num(appt,(char)')')); ++(*appt);
  while(dpt)
    {if(dpt->declnum==auxnum)
       {if(dpt->nodes && dpt->sst_pos>VIR_MAX) return(dpt);
        if(dpt->nodes||dpt->sst_pos)
          path_error(PER_GEN,refpt,refpt,WRN_AUX_UND,dpt->name);
        break;
       }
     dpt=dpt->tail;
    }
  return(NULL); /* ignore aux if it's a null net */
}

/* update part list */
void update_part()
{ int lvl,num,*npt;
  lvl=max_part(ilk_pt->opt); num=ilk_pt->part_num;
  if(lvl<1)
    {num=0; lvl=0; nul_free(ilk_pt->part_list);}
  else if(num==0)
    {npt=malmem(int,1+lvl); ilk_pt->part_list=npt; num=lvl;}
  else if(num<lvl)
    {npt=relmem(int,sizeof(int)*(1+lvl),ilk_pt->part_list,sizeof(int)*(1+num));
     ilk_pt->part_list=npt;
     npt+=num; num=lvl-num;}
  else num=0;
  while(num) {*npt=(-1); ++npt; --num;}
  ilk_pt->part_num=lvl;
}

path_elm *glob_search(ppt,apt)
  path_elm *ppt; char *apt;
{ lab_elm *lpt;
  while(ppt)
    {if(ppt->net)
       {lpt=ppt->net->lab;
        while(lpt)
          {if(strcmp(apt,lpt->name)==0) return(ppt);
           lpt=lpt->tail;
          }
       }
     ppt=ppt->parent;
    }
  return(NULL);
}

/* move to parent node containing label */
path_elm *glob_ref(opt,apt)
  path_elm *opt; char *apt;
{ aux_elm *auxpt; path_elm *tpt,*gpt; yard nodenum; char chr,*bpt=apt;
  while(isident(*bpt)) ++bpt;
  chr= *bpt; *bpt='\0';
  gpt=glob_search(opt,apt);
  if(!gpt && aux_top)
    {auxpt=aux_top; tpt=opt; while(tpt->tail) tpt=tpt->tail;
     while(tpt && !tpt->net->root_flag) tpt=tpt->parent;
     if(!tpt) tpt=opt;
     nodenum=tpt->net_base;
     while(auxpt && auxpt->aux_base<=nodenum)
       {tpt=glob_search(auxpt->par_state.org_pt,apt); auxpt=auxpt->tail;
        if(tpt) gpt=tpt;
       }
    }
  if(!gpt) {path_error(PER_GEN,opt,opt,WRN_GLO_UNF,apt); gpt=opt;}
  *bpt=chr;
  return(gpt);
}

/* select ref path to use & adj opath str
* chk for floated paths etc: ~---|~~---|.\---|..\---|\---|c|*|?
*/
path_elm *opath_ref(strpt,selfp)
  char **strpt; boolean *selfp;
{ boolean firsterr=T; path_elm *ppt=NULL; char *ostr=(*strpt);
  if(!ostr || !*ostr) signal_error(EXIT_INT_ERR,31);
  *selfp=F;
  switch(*ostr)
    {case '~': ++ostr;
       if(!til_pt || (*ostr && (*ostr!='~') && (check_path(til_pt)!=PATH_TER)))
         {path_error(PER_GEN,org_pt,org_pt,WRN_TIL_UNF); return(NULL);}
       else if(*ostr=='~') {++ostr; ppt=node_path(til_pt);}
       else ppt=til_pt;
       *selfp=T;
       break;
     case '/': ++ostr; ppt=rootpt; break;
     case '.': ostr+=2; ppt=org_pt; break;
     case '#': ppt=org_pt; break; /* DEBUG may be a decl to diff net?? */
     case '@': ppt=flo_pt; break;
     case '$': ilk_pt->glob=T; ostr+=2; ppt=glob_ref(org_pt,ostr); break;
     default: ppt=flo_pt;
    }
  if(*ostr=='@')
    {while(*ostr=='@')
       {++ostr;
        if(ppt->parent) ppt=ppt->parent;
        else if(firsterr)
          {path_error(PER_GEN,org_pt,org_pt,WRN_OUT_OFL); firsterr=F;}
       }
     ++ostr;
    }
  *strpt=ostr;
  if(!ppt) signal_error(EXIT_INT_ERR,32);
  return(ppt);
}

boolean grp_brk(cop) short cop;
{ return((boolean)(cop&COMB_BREAK||cop&COMB_PLMI));
}

/* sets most of flags (except flags.pamat & .last) etc for doing an opath
*  outpt & path_pt must be setup beforehand */
void do_opath()
{ boolean dumflg; int slot1;
  flags.fork=(boolean)((ilk_pt->curpt->combop&COMB_FORK)!=0);
  flags.inp=flags.fork;
  flags.info=F; flags.ary=F; flags.inout=F; flags.state=F;
  if(ds_pool)
    {slot1=push_stub(ds_stub);
     ds_stub=find_dspt(path_pt,(boolean *)&dumflg); flags.build=dumflg;
    }
  do_nxt_decl();
  if(ds_pool) ds_stub=pop_stub(slot1);
  if(!path_pt->rebflg) tidy_allarys(path_pt);
}


/* set up a new oelm & its path pts in ilk_pt
*  return T if ok */
boolean oelm_init()
{ char *ostr; decl_elm *auxpt=NULL; path_elm *ppt,*opt; swin lvl; boolean delf;
  ostr=token_to_str(ilk_pt->curpt->outok,(swin *)&lvl);
  if(!ostr || !*ostr)
    {path_error(PER_GEN,org_pt,org_pt,WRN_BAD_OPTH);
     return(F);}
  if(*ostr=='(')
    {if(ilk_pt->curpt->combop&COMB_PLMI)
       {path_error(PER_GEN,org_pt,org_pt,WRN_AUX_MIN);
        return(F);}
     ostr+=3;
     if(!org_pt) signal_error(EXIT_INT_ERR,33);
     auxpt=aux_decl(org_pt,(char **)&ostr);
    }
  /* select ref path to use */
  ppt=opath_ref((char **)&ostr,(boolean *)&delf); ilk_pt->self=delf;
  if(!ppt) return(F);
  opt=dup_path(ppt,F); /* zaps new paths pary's & eval stks */
  blank_part(opt);
  ppt=opt;
  if(ppt) while(ppt->tail) ppt=ppt->tail;
  else signal_error(EXIT_INT_ERR,34);
  /* Chk if ary_list etc of copied node (at ppt) are needed DEBUG */
  /* would be better if dup_path() zapped ppt */
  blank_pelm(ppt);
  if(!*ostr) ostr=STR_OUTARY;
  stack_str(ppt,ostr);
  ilk_pt->opt=opt; ilk_pt->orefpt=ppt; ilk_pt->endofcx=F;
  if(auxpt) {ilk_pt->aux=auxpt; ilk_pt->startaux=T;}
  else {ilk_pt->aux=NULL; ilk_pt->startaux=F;}
  return(T);
}

/* initialise an aux net root path for resolving */
void oaux_init(ipt,fpt,base)
  ilk_elm *ipt; foot fpt; yard base;
{ path_elm *opt;
  opt=new_path((path_elm *)NULL);
  opt->net_base=base;
  claim_net((net_elm **)&opt->net,fpt);
  stack_str(opt,STR_AUXOUT);
  ipt->opt=opt; ipt->orefpt=opt; ipt->endofcx=F;
}

/* process (Ax)Op, get cx for Ax/input[*] from Op in aux's parent net
  ilk_pt->curpt->outok==AUX_AOTOK
  if not merge only get 1 cx per aux, --> set ilk_pt->endofcx
  cp par ilk info to ilk_pt except ilk_pt->cxnum, use for merge test>1 cx
*/
ilk_elm *aux_spec_op(probe) short probe;
{ state_elm aux_state; decl_elm *tmpaux; ilk_elm *cx_pt;
  short cop=aux_bot->par_state.ilk_pt->curpt->combop;
  boolean conxok=F,notmerge=(boolean)((cop&COMB_MERGE)==0),first=F;
  if(notmerge && ilk_pt->cxnum>0) {ilk_pt->endofcx=T; return(NULL);}
  state_save((state_elm *)&aux_state);
  state_restore((state_elm *)&aux_bot->par_state); /* get par ilk_pt etc */
  tmpaux=ilk_pt->aux; ilk_pt->aux=NULL; /* disable par aux */
  if(ilk_pt->node) {conxok=T; goto byebye;} /* a conx is ready already..*/
  if(ilk_pt->endofcx) goto byebye; tryagain:
  if(!ilk_pt->opt)
    {if(probe||!oelm_init()) goto byebye; first=T;
     ilk_pt->startaux=F; tmpaux=ilk_pt->aux; ilk_pt->aux=NULL;}/*DEBUG 4/1/93*/
  outpt=ilk_pt->opt; path_pt=ilk_pt->orefpt;
  flags.pamat=F; flags.last=F;
  do_opath();
  if(ilk_pt->node) /* found a conx */
    {conxok=T; ++ilk_pt->abnum; ++ilk_pt->cxnum;/*DEBUG 25/5/93*/}
  else {if(!first) {++ilk_pt->cxnum; tidy_ilk(ilk_pt); goto tryagain;}}
  ilk_pt->endofcx=(boolean)!conxok;
  update_part();
  byebye:
  ilk_pt->aux=tmpaux; cx_pt=ilk_pt;
  state_update((state_elm *)&aux_bot->par_state);
  state_restore((state_elm *)&aux_state);
  ilk_pt->endofcx=cx_pt->endofcx;
  ilk_pt->abnum=cx_pt->abnum;
  if(conxok) {++ilk_pt->cxnum; return(cx_pt);}
  return(NULL);
}

/* process Ip=[<](Ax)Op path, get cx for Ip from Ax */
ilk_elm *get_aux(probe) short probe;
{ ilk_elm *cx_pt,*tmpilk; var_elm *vpt=ilk_pt->aux->var;
  foot fpt; boolean conxok=F,ismerge;
  ismerge=(boolean)((ilk_pt->curpt->combop&COMB_MERGE)!=0);
  fpt=ilk_pt->aux->sst_pos;
  if(ilk_pt->startaux) /* aux hasn't been eval'd yet */
    {if(probe) return(NULL);
     /* put special decl in aux root fly lst: input[*]=Op or input[*]= 1cxOp*/
     ilk_pt->startaux=F;
     aux_push(); /* add a slot to state stack */
     state_save((state_elm *)&aux_bot->par_state);
     set_aux_ilk((ilk_elm *)&aux_bot->ilk);
     set_aux_inp((inp_elm *)&aux_bot->inp,ismerge);
     do_an_aux: /* when doing another aux jumps here from below */
     state_init();
     fly_aux(); /* add special In decl to root aux fly list */
     is_new_net=T; aux_bot->aux_base=net_node_max+1;
     do_network(fpt,vpt); /* eval a new aux net */
     state_restore((state_elm *)&aux_bot->par_state);
     state_update((state_elm *)&aux_bot->par_state);
    }
  else state_save((state_elm *)&aux_bot->par_state);
  /* aux has been eval, see if conx ready */
  cx_pt=(ilk_elm *)&aux_bot->ilk;
  if(cx_pt->node) {conxok=T; goto byebye;}
  if(cx_pt->endofcx) goto tryagain; /* chk if need to loop over Op */
  /* some conxs left in aux */
  /* set up aux path etc to get nxt conx from aux Ip=Ax/output[.] */
  if(!cx_pt->opt)
    {if(probe) goto byebye; oaux_init(cx_pt,fpt,aux_bot->aux_base);}
  outpt=cx_pt->opt; path_pt=cx_pt->orefpt;
  rootpt=path_pt; /* new root is aux root, but inpt is still parent */
  ds_pool=NULL; ds_stub=NULL; flags.build=F;
  flags.pamat=F; flags.last=F;
  tmpilk=ilk_pt; ilk_pt=cx_pt; do_opath(); ilk_pt=tmpilk;
  if(cx_pt->node) /* found a conx */
    {conxok=T; ++cx_pt->cxnum; ++cx_pt->abnum;}
  else /* chk if not merge: try another aux */
    {tryagain: if(probe) goto byebye;
     if(!ismerge && !ilk_pt->endofcx)
       {tidy_ilk(cx_pt); goto do_an_aux;}
     /* end of this aux opath: clr aux & stack */
     /* tidy_ilk(ilk_pt); */
     ilk_pt->aux=NULL; aux_bot->ilk.aux=NULL;
     tidy_ilk((ilk_elm *)&aux_bot->ilk);
     clr_aux_inp((inp_elm *)&aux_bot->inp);
     clr_aux_ilk((ilk_elm *)&aux_bot->ilk);
     state_restore((state_elm *)&aux_bot->par_state);
     aux_pop();
     return(NULL);
    }
  /* can't clear aux stk & ilk till all Ax Ip conxs read... */
  byebye:
  state_restore((state_elm *)&aux_bot->par_state);
  /* note, aux stuff still on stack.. */
  cx_pt->endofcx=(boolean)!conxok;
  if(conxok) return(cx_pt);
  return(NULL);
}

/* get next connection & store in ilk_pt, or aux_pt if aux
*  eval ilk_pt oelm & get nxt conx if necessary
*  returns cx_pt if a conx is available else null
*  If reaches end of cur oelm tries next oelm
*  When probe is set it only looks in the current active oelm for a conx,
*  if no oelm mounted or past end of current oelm, it returns null.
*  Probe is used primarily by part_eval & endofcx determining.
*  probe=1 --> oelm or aux oelm, probe=2 --> oelm only
*/
ilk_elm *get_conx(probe) short probe;
{ path_elm *tmppt=path_pt,*tmpout=outpt; out_rec *opt,*recopt,*old=NULL;
  ilk_elm *cx_pt; int tmpras=raise_error,count=0;
  flag_elm tmpflg; boolean conxok=F,firstime=T; tmpflg=flags;
  if(ilk_pt->curpt->outok==AUX_AOTOK) /* get cx from aux parent net */
    return(aux_spec_op(probe));
  if(ilk_pt->aux&&probe!=2) goto tryaux; /* get cx from aux def */
  if(ilk_pt->node) return(ilk_pt); /* a conx is ready already..*/
  raise_error=ERR_OK;
  if(ilk_pt->endofcx) {if(probe) goto goodbye; goto trynxt;}
  for(;;)
    {if(!ilk_pt->opt) {if(probe || !oelm_init()) goto byebye;}
     if(ilk_pt->aux&&probe!=2)
       {tryaux: raise_error=ERR_OK;
        if((cx_pt=get_aux(probe))) {raise_error=tmpras; return(cx_pt);}
        goto trynxt;
       }
     outpt=ilk_pt->opt; path_pt=ilk_pt->orefpt;
     flags.pamat=F; flags.last=F; raise_error=ERR_OK;
     do_opath();
     if(ilk_pt->node) /* found a conx */
       {conxok=T; ++ilk_pt->cxnum; ++ilk_pt->abnum; break;}
     else /* no connection found, try next oelm */
       {trynxt: if(probe) goto goodbye;
        if((opt=ilk_pt->curpt))
          {old=opt;
           if(opt->combop)
             {if(count==0) /* work out recursion trap count */
                {recopt=opt;
                 while(recopt->combop && !grp_brk(recopt->combop))
                   {++recopt; ++count;}
                 if(count) --recopt;
                }
              if(!grp_brk(opt->combop))
                {++opt;
                 if(opt->combop && !grp_brk(opt->combop)) goto tryagain;
                 --opt;
                 if(!ilk_pt->self && !ilk_pt->discan)
                   {if(opt!=ilk_pt->baspt) /* chk if rescanning a single oelm*/
                      {/* opt is at a plus op on entry, so... */
                       --opt;
                       while(!grp_brk(opt->combop)&&(opt!=ilk_pt->baspt))
                         --opt;
                       if(grp_brk(opt->combop)) ++opt;
                      }
                    goto tryagain;
                   }
                }
              else if(!ilk_pt->self && !ilk_pt->discan && (opt!=ilk_pt->baspt))
                goto tryagain;
              if(ilk_pt->self && firstime) {firstime=F; goto tryagain;}
             }
          }
        break;
        tryagain:
        tidy_ilk(ilk_pt);
        ilk_pt->curpt=opt;
        if(raise_error && opt==old) break; /* stops recursion err */
        if(count)
          {if(firstime) {firstime=F; --count;}
           else if(count<2) {ilk_pt->curpt=recopt; break;}
           else --count; /* stops recursion */
          }
       }
    }
  byebye: update_part();
  goodbye: ilk_pt->endofcx=(boolean)!conxok;
  path_pt=tmppt; outpt=tmpout; flags=tmpflg; raise_error=tmpras;
  if(conxok) return(ilk_pt);
  return(NULL);
}

/* finds indx's stub_elm in ds pool pt to by ds_stub
*  if docre=T creates a new stub_elm (& adds extra size bytes to it as well)
*  with index if not found (type=SIP_CONX) else returns NULL
*  ds_stub->info.data.elm=cur size & ds_stub->info.data.size=fixed size of ARB
*/
stub_elm *fnd_conx(indx,size,docre)
  mile indx; int size; boolean docre;
{ stub_elm *rpt;
  if(!ds_stub->hassubpt)  /* ds_stub pts to TYPE_ARB or atomic type node */
    {if(!docre) return(NULL);
     rpt=new_stub(ds_stub,size,(int)SIP_CONX,DS_SUB,F);
     ds_stub=ds_prior(rpt); ds_stub->isdef=T;
     ds_stub->info.data.elm=1;
     rpt->info.data.elm=indx;
     rpt->info.data.size=ds_stub->info.data.size;
     return(rpt);
    }
  return(sip_pos((stub_elm **)&ds_stub,ds_subpt(ds_stub),
                 indx,(int)SIP_CONX,size,docre));
}

/* find conx with adrs npt in ds ary,
*  return null not found, or spt->type!=SIP_NODE */
stub_elm *fnd_adrs(spt,npt)
  stub_elm *spt; node_elm *npt;
{ while(spt)
    {if((spt->type!=SIP_NODE)
        || adrsequal((node_elm *)&(ds_value(spt)->node),npt)) break;
     spt=ds_arypt(spt);
    }
  return(spt);
}

/* put connection in cx_pt into data structure at index
*  Note ilk_pt pts to Op conx details. If an aux Op, cx_pt pts to aux_pt
*  adjust cur_size of indx if necessary
*  returns 1 if conx added to a new slot, otherwise 0
*/
int put_conx(cx_pt,indx)
  ilk_elm *cx_pt; mile indx;
{ stub_elm *apt; char *xpt,*ypt; int numadd=0,nsize,oldsize,slot1,slot2;
  if(!ds_stub) signal_error(EXIT_INT_ERR,35);
  nsize=node_size(cx_pt->node);
  apt=fnd_conx(indx,nsize,T); /* allow for size of new node elm */
  if(apt)
    {if(apt->type==SIP_NODE) /* old conx exists at indx */
       {if(ilk_pt->curpt->combop&COMB_OMNI) /*only overwrite if '++'op*/
          {/* adj overwritten cx's DS conx account */
           slot1=push_stub(apt);
           adj_cx_ds((node_elm *)&(ds_value(apt)->node),(int)-1,F);
           apt=pop_stub(slot1);
           oldsize=node_size((node_elm *)&(ds_value(apt)->node));
           if(oldsize!=nsize)
             {slot2=push_stub(ds_stub);
              if(oldsize>nsize)
                ds_memdec((char *)&(ds_value(apt)->node.base),
                          (int)(oldsize-nsize));
              else
                {slot1=push_stub(apt);
                 ds_meminc((char *)&(ds_value(apt)->node.base),
                          (int)(nsize-oldsize));
                 apt=pop_stub(slot1);
                }
              ds_stub=pop_stub(ds_stub);
              set_stub_size(apt,(int)(SIZ_SIP+nsize));
             }
          }
        else
          {path_error(PER_IN,inpt,NULL,WRN_CXIN_OWRT,(mile)indx);
           path_error(PER_OUT,org_pt,org_pt,ERR_OK);
           return(0); /* don't throw unused conx away ?? */
          }
       }
     else /* new conx */
       {if(apt->type!=SIP_CONX) signal_error(EXIT_INT_ERR,36);
        apt->type=SIP_NODE; numadd=1; apt->isdef=T;
       }
     xpt=(char *)&(ds_value(apt)->node); ypt=(char *)cx_pt->node;
     while(nsize--) *xpt++ = *ypt++;
     nul_free(cx_pt->node);
     fast_ds_mod(ds_pool);
     if(ilk_pt->curpt->invar) /* chk for inline var here */
       do_inline_var(apt);
    }
  else signal_error(EXIT_INT_ERR,37);
  return(numadd);
}

/* delete conx from ds_stub: caller must update ary size etc (numadd) */
int del_conx(apt)
  stub_elm *apt;
{ stub_elm *spt; int slot1;
  if(!apt) return(0);
  /* a conx to del from ds_stub has been found. dec deleted cx's DS */
  slot1=push_stub(apt);
  adj_cx_ds((node_elm *)&(ds_value(apt)->node),(int)-1,F);
  apt=restore_stub(slot1);
  adj_uasize(apt,(int)(-1));
  spt=ds_parent(apt);
  /* now remove connection stub from ds: parent->subpt or ->arypt */
  if(spt==ds_stub) spt->hassubpt=apt->hasarypt;
  else spt->hasarypt=apt->hasarypt;
  apt->hasarypt=F;
  update_stub(ds_stub,slot1);
  if(apt->hassubpt || apt->hastail) signal_error(EXIT_INT_ERR,38);
  rem_sip(apt);
  ds_stub=pop_stub(slot1);
  fast_ds_mod(ds_pool);
  return(-1);
}

/* add conx in ilk_pt->node to ds_stub at indx elm */
void add_connect(indx)
  mile indx;
{ ilk_elm *cx_pt; mile dsiz=ds_stub->info.data.size;
  int dtyp=ds_stub->type,ctyp,cnum=0,tofs; boolean skip;
  if(!(dtyp&STAR_MASK) && dsiz) /*conx destination of wrong type, pt expected*/
   in_error(WRN_CXIN_TYPE,(mile)dtyp);
  else
    {if(dsiz && indx>=dsiz) /* conx index out of range */
       in_error(ERR_CXIN_INDX,(mile)indx,(mile)dsiz);
     else
       {if((cx_pt=get_conx(0))) /* got a conx from Op */
          {if(dsiz) tofs=STAR_ONE; else tofs=STAR_TWO;
           ctyp=cx_pt->type;
           if(!cx_pt->size) /* check if outputting from a UA, adjust type */
             ctyp-=STAR_ONE;
           skip=(boolean)( ((ctyp&STAR_TYPE)!=(dtyp&STAR_TYPE))
                || (((ctyp&STAR_MASK)+tofs)!=(dtyp&STAR_MASK)) );
           if(skip && !flags.wild)
             in_error(WRN_CXIN_TMIS,(mile)ctyp,(mile)dtyp);
           if(skip && flags.wild)
             {/*chk if diverted conx was deleted and not added*/
              if(cx_pt->curpt->combop&COMB_DIVERT)
                in_error(WRN_CXIN_DIVR);
              else  /* dec Op's DS of conx */
                adj_cx_ds(cx_pt->node,(int)-1,F);
              nul_free(cx_pt->node); /* throw cx away */
             }
           else cnum+=put_conx(cx_pt,indx);
          }
       }
    }
  adj_tree(ds_stub,cnum);
}

/* subtract connection from ds_stub elm at indx
* Note conx always from ilk_pt for subtracts, unlike add (cx_pt)  */
void sub_connect(indx)
  mile indx;
{ stub_elm *apt,*spt; mile dsiz; int cnum=0,slot1; boolean loop;
  if(!ds_stub) signal_error(EXIT_INT_ERR,39);
  dsiz=ds_stub->info.data.size;
  if(dsiz && indx>=dsiz && !flags.wild) /* conx index out of range*/
    in_error(ERR_CXIN_INDX,(mile)indx,(mile)dsiz);
  else if(get_conx(0)) /*fnd cx in ds_stub & del*/
    {loop=(boolean)((ilk_pt->curpt->combop&COMB_OMNI)!=0);
     if(loop) /* look for conx in all of ds ary */
       apt=fnd_adrs(ds_subpt(ds_stub),ilk_pt->node);
     else apt=fnd_conx(indx,(int)0,F);
     if(apt) /* a conx to del has been found */
       {if(apt->type==SIP_NODE)  /* del conx */
          {/* chk conxs are equal */
           if(adrsequal((node_elm *)&(ds_value(apt)->node),ilk_pt->node))
             {do{spt=ds_arypt(apt);
                 if(spt) slot1=push_stub(spt);
                 cnum+=del_conx(apt);
                 if(spt) spt=pop_stub(slot1); else break;
                 if(loop)
                   {apt=fnd_adrs(spt,ilk_pt->node);
                    loop=(boolean)(apt && (apt->type==SIP_NODE));
                   }
                }while(loop);
             }
           else
             {if(!flags.wild) in_error(WRN_CXIN_NEQU);
              goto byebye;
             }
          }
        else
          {if(!flags.wild) /* tried to del a non conx */
             in_error(WRN_CXIN_NTCX,(mile)apt->type);
           byebye: /*chk if diverted conx wasn't deleted from dest*/
           if(ilk_pt->curpt->combop&COMB_DIVERT) in_error(WRN_CXIN_DIDE);
          }
       }
     nul_free(ilk_pt->node);
    }
  adj_tree(ds_stub,cnum);
}

/* get a fork conx from ds_stub and return as out conx (get its adrs)
*  return F if none found */
boolean do_fork()
{ stub_elm *apt; int numadd;
  if(!ds_stub) signal_error(EXIT_INT_ERR,40);
  apt=fnd_conx(ilk_pt->indx,(int)0,F);
  if(apt) /* found a conx in ds_stub to put in ilk_pt->node etc */
    {if(apt->type==SIP_NODE) /* copy conx to ilk_pt */
       {if(ilk_pt->curpt->combop&COMB_DIVERT) /* divert conx */
          {ilk_pt->node=copy_adrs((node_elm *)&(ds_value(apt)->node));
           get_cx_type(); numadd=del_conx(apt); adj_tree(ds_stub,numadd);
          }
        else /* fork conx */
          {ilk_pt->node=copy_adrs((node_elm *)&(ds_value(apt)->node));
           if(!(ilk_pt->curpt->combop&COMB_PLMI)) /* inc conx's DS if +Op */
             adj_cx_ds(ilk_pt->node,(int)0,F); /* a zero val sets type etc */
           else get_cx_type();
          }
        return(T);
       }
     else /* not a conx, signal type error */
       if(!flags.wild) in_error(WRN_CXIN_NTCX,(mile)apt->type);
    }
  return(F);
}

/* check pile or heap is empty & then free mem space */
void pile_free(ppt,plist,ispile)
  path_elm *ppt; pile_elm *plist; boolean ispile;
{ pile_elm *pilpt; mile pnum=lst_ok(plist);
  if(pnum>0) /* if not empty signal so & then delete conxs */
    {if(ispile) path_error(PER_GEN,ppt,ppt,WRN_STK_NTEM,vrname[ispile],pnum);
     else signal_error(WRN_STK_NTEM,vrname[ispile],pnum);
     pilpt=plist;
     while(pnum--)
       {adj_cx_ds(pilpt->node,(int)-1,F); fremem(pilpt->node); ++pilpt;}
    }
  lfree(plist);
}

pile_elm *get_plist(plist,ppos,isp)
  pile_elm *plist; int *ppos; boolean isp;
{ mile pos,num=lst_ok(plist);
  if(num==0)
    {path_error(PER_GEN,path_pt,path_pt,WRN_STK_EMPT,vrname[isp]);
     return(NULL);}
  pos=(*ppos);
  if(pos>=num)
    {path_error(PER_GEN,path_pt,path_pt,WRN_STK_IOOR,vrname[isp],(mile)pos);
     pos=num-1; *ppos=pos;}
  return(plist+(num-pos-1));
}

pile_elm *fnd_pile(plist,ppos,npt)
  pile_elm *plist; mile *ppos; node_elm *npt;
{ mile num=lst_ok(plist),pos=num;
  plist+=num;
  while(num--)
    {--plist;
     if(adrsequal(plist->node,npt)) {*ppos=(pos-num-1); return(plist);}
    }
  *ppos=0;
  return(NULL);
}

/* fetch a conx from virtual net */
boolean get_virtual(indx)
  mile indx;
{ pile_elm **plist,*pelm; foot fpos; mile pos; short cop; boolean ispile,keep;
  fpos=path_pt->decl_pt->sst_pos;
  if(fpos<1||fpos>VIR_MAX) signal_error(EXIT_INT_ERR,41);
  if(fpos==VIR_BIN) /* can't get conxs from bin: nsl1 trapped this! */
    {path_error(PER_GEN,path_pt,path_pt,WRN_BIN_CX); return(F);}
  ispile=(boolean)(fpos==VIR_PILE);
  if(ispile)
    {if(!fly_bot) signal_error(EXIT_INT_ERR,42);
     plist=(pile_elm **)&fly_bot->pile_list;}
  else plist=(pile_elm **)&heap_list;
  pos=indx;
  pelm=get_plist((pile_elm *)*plist,(mile *)&pos,ispile);
  if(!pelm) return(F);
  /* now treat conx like normal */
  ilk_pt->indx=pelm->indx; ilk_pt->size=pelm->size; ilk_pt->type=pelm->type;
  cop=ilk_pt->curpt->combop;
  keep=(boolean)(cop&COMB_COPY || (flags.fork && !(cop&COMB_DIVERT)));
  if(keep)
    {ilk_pt->node=copy_adrs(pelm->node);
     if(!(cop&COMB_PLMI)) /* inc conx's DS if +Op */
       adj_cx_ds(ilk_pt->node,(int)1,F);
    }
  else
    {/* remove conx from stk by shifting down over hole */
     ilk_pt->node=pelm->node;
     if(cop&COMB_PLMI) /* dec conx's DS if -Op */
       adj_cx_ds(ilk_pt->node,(int)-1,F);
     --lst_num(*plist); while(pos--) {*pelm=(*(pelm+1)); ++pelm;}
    }
  return(T);
}

/* add conx to virtual net */
void put_virtual(indx)
  mile indx;
{ ilk_elm *cx_pt; pile_elm **plist,*pelm; foot fpos;
  mile num,pos; short cop; boolean loop,ispile;
  fpos=path_pt->decl_pt->sst_pos;
  if(fpos<1||fpos>VIR_MAX) signal_error(EXIT_INT_ERR,43);
  if(!(cx_pt=get_conx(0))) return; /* no conx found */
  if(fpos==VIR_BIN) /* throw conx away, conx account should be in order*/
    {nul_free(cx_pt->node); return;}
  /* add conx to stk */
  ispile=(boolean)(num==VIR_PILE);
  if(ispile)
    {if(!fly_bot) signal_error(EXIT_INT_ERR,44);
     plist=(pile_elm **)&fly_bot->pile_list;}
  else
    plist=(pile_elm **)&heap_list;
  cop=ilk_pt->curpt->combop;
  if(!(cop&COMB_PLMI)) /* add connect to stk */
    {num=lst_ok(*plist);
     *plist=lmore(*plist,1,TYPE_STRUCT,pile_elm);
     pelm=(pile_elm *)(*plist)+num;
     pelm->indx=cx_pt->indx; pelm->size=cx_pt->size; pelm->type=cx_pt->type;
     pelm->node=cx_pt->node; cx_pt->node=NULL;
    }
  else /* sub connect from stk */
    {loop=(boolean)((cop&COMB_OMNI)!=0); pos=indx;
     if(loop) /* look for conx in all of stk */
       pelm=fnd_pile((pile_elm *)*plist,(mile *)&pos,cx_pt->node);
     else
       pelm=get_plist((pile_elm *)*plist,(mile *)&pos,ispile);
     if(pelm) /* a conx to del has been found */
       {/* chk conxs are equal */
        if(adrsequal(pelm->node,ilk_pt->node))
          {do{adj_cx_ds(pelm->node,(int)-1,F);
              --lst_num(*plist); while(pos--) {*pelm=(*(pelm+1)); ++pelm;}
              if(loop)
                {pelm=fnd_pile((pile_elm *)*plist,(mile *)&pos,cx_pt->node);
                 loop=(boolean)(pelm!=NULL);
                }
             }while(loop);
          }
        else
          {if(!flags.wild) in_error(WRN_CXIN_NEQU);
           /*chk if diverted conx wasn't deleted from dest*/
           if(cop&COMB_DIVERT) in_error(WRN_CXIN_DIDE);
          }
       }
    }
}

/* process(i.e. add, sub, divert, fork) a conx to IN node at indx.
*  By this time path_pt is at a terminal node (no tail)
*  indx is always >=0
*/
void conx_to_node(indx)
  mile indx;
{ stub_elm *spt; boolean paryparent=(boolean)(path_pt->arychild==PARY_CHILD);
  /* must be at a paryparent by this point, its an error if this is true:- */
  if(path_pt->aryparent && !paryparent) signal_error(EXIT_INT_ERR,45);
  if(flags.ary||flags.info) /* calcing path_info or do_an_ary lary size det */
    {flags.escape=T; return;}
  if(flags.inp && flags.inout) til_pt=path_pt;
  if(flags.inout) /* doing an inpath in process conx mode */
    {if(path_pt->decl_pt->declnum)
       {if(ds_stub->type==TYPE_ARB) spt=ds_subpt(ds_stub); else spt=ds_stub;
        if(!spt || (path_pt->decl_pt->decltype!=spt->type))
          signal_error(EXIT_INT_ERR,46);
        if(ilk_pt->curpt->combop&COMB_PLMI) sub_connect(indx);
        else add_connect(indx);
       }
     else  put_virtual(indx); /* trap virtual nets */
    }
  else /* fetching a conx from oelm path */
    {if(!ilk_pt || ilk_pt->node || ilk_pt->endofcx) signal_error(EXIT_INT_ERR,47);
     if(path_pt->decl_pt->declnum)
       {ilk_pt->indx=indx; /* this should be an elm value from ds_stub */
        ilk_pt->size=path_pt->decl_pt->size;
        ilk_pt->type=path_pt->decl_pt->decltype;
        if(flags.fork) /* conx comes from ds_stub */
          {if(!do_fork()) return;} /* no conx found */
        else /* conx comes from opath */
          {ilk_pt->node=struct_adrs(path_pt);  /* adrs includes indx */
           if(!(ilk_pt->curpt->combop&COMB_PLMI)) /* inc conx's DS if +Op */
             adj_cx_ds(ilk_pt->node,(int)1,T);
          }
       }
     else /* get conx from pile or heap */
       {if(!get_virtual(indx)) return;}
     flags.escape=T; set_rebuild(path_pt); /* enter escape mode */
    }
}

/* reduce path to it's original short form from npt down
*  keeps npt node regardless */
void strip_path(npt)
  path_elm *npt;
{ path_elm *ppt=npt,*tpt=npt; byte low_id; boolean keepelm;
   while(tpt) /* clear all arrays & rebuilds first */
    {tidy_allarys(tpt); tidy_reb(tpt); tpt=tpt->tail;}
  low_id=(byte)(npt->pat_id+1); /* lowest label id to del from */
  while(npt)
    {if(npt->pat_id>=low_id||npt->dec_id>=low_id)
       {keepelm=(boolean)(npt->pat_id<low_id||npt->dec_id<low_id);
        yank_lab(npt,low_id); tpt=npt; npt=npt->tail;
        if(!keepelm) rem_path(tpt);
       }
     else npt=npt->tail;
    }
  if(ppt)
    {ppt=ppt->tail;
     while(ppt) {release_net((net_elm **)&ppt->net); ppt=ppt->tail;}}
}

void set_flg_last(partnum) int partnum;
{ flags.last=(boolean)
             ((partnum==max_part(inpt))&&(partnum>=max_part(ilk_pt->opt)));
}

/* Called from selector_val() to eval oelm to part level
*  and return number of leaves.
*  Note special aux decls will have been trapped elsewhere (AUX_AOTOK).
*  Note may be called by an opath to return previous part oelm size
*  if so, chk not trying to get future $! --> recursion err
*  curpt is node containing $!p (may be ipth or opth)
*/
mile partition_val(curpt,coors,partnum)
  path_elm *curpt; int coors,partnum;
{ path_elm *tmppt=path_pt,*tmpout=outpt,*ppt,*npt; flag_elm tmpflg;
  mile size=(-1); int tmplvl=part_level,tmpleafs=part_leafs,lvl;
  long tmpa=nsl_aseed,tmpb=nsl_bseed,tmpc=nsl_cseed;
  boolean tmpexd=part_exceed,cardcoor=(boolean)((coors&SIN_CAR)!=0);
  if(ilk_pt->endofcx) return(-3);
  if(!ilk_pt->curpt->combop) return(-1);
  if(partnum==1 && !cardcoor && check_exclam(curpt)) return(1);
  tmpflg=flags;
  if(cardcoor) partnum=magy(partnum);
  else partnum+=cur_part(curpt)-1; /* convert rel part to abs, 1..x */
  if(!flags.inp && flags.pamat && partnum>=part_level)
    {path_error(PER_GEN,curpt,curpt,WRN_SEL_REC); return(-1);}
  /* now eval path to partnum */
  flags.pamat=T; part_level=partnum; part_leafs=0; part_exceed=F;
  if(ilk_pt->opt && ilk_pt->opt->decl_pt) /* chk for endofcx */
    {set_flg_last(partnum);
     if(!get_conx(2))
       {lvl=max_part(ilk_pt->opt);
        if(lvl<partnum&&ilk_pt->part_num<partnum) goto skiprior;
       }
    }
  if(ilk_pt->part_num>=partnum) /* chk if already eval'd */
    {size=ilk_pt->part_list[partnum-1];
     if(size<0) /* opath eval'd past partnum, have to back track */
       {lvl=rel_level(ilk_pt->orefpt);
        outpt=copy_path(ilk_pt->opt,T); /* copy rebuilds & ary's etc */
        ppt=rel_path(outpt,lvl);
        path_pt=part_path(ppt,partnum);
        if(!path_pt->net) /*DEBUG 20/5/93*/
          {if(path_pt->parent&&path_pt->parent->net) path_pt=path_pt->parent;
           else signal_error(EXIT_INT_ERR,200);
          }
        strip_path(path_pt);
        sels_path(outpt); set_flg_last(partnum);
        do_opath(); free_path(path_pt);
        ilk_pt->part_list[partnum-1]=part_leafs; size=part_leafs;
       }
    }
  else /* opath not eval'd past desired partnum, or partnum>max */
    {skiprior:
     if(!ilk_pt->opt) /* opth not eval'd yet, so set up */
       {if(!oelm_init()) goto byebye;}
     if(term_path(ilk_pt->opt))
       {lvl=max_part(ilk_pt->opt);
        if(partnum>=lvl)
          path_error(PER_GEN,curpt,curpt,WRN_PRT_OOR,lvl,partnum);
        else signal_error(EXIT_INT_ERR,48);}
     else /* opth not fully eval'd yet */
       {/* eval opath beyond part_level to det number of leafs */
        /* AND keep base path down to part_level, via rebflg, for later */
        /* note at this point ilk_pt->part_num is less than partnum */
        outpt=ilk_pt->opt; path_pt=ilk_pt->orefpt;
        if(path_pt->rebflg) signal_error(WRN_INT_ERR);
        set_flg_last(partnum);
        do_opath();
        /* now try & moveto part_level in opath */
        /* if opath not eval'd beyond part_level restore to eval root */
        if(part_leafs==0) ppt=path_pt;
        else /* otherwise move to (part_level)->parent in opath & keep */
          /* provided it's below path_pt */
          {ppt=part_path(outpt,partnum);
           if(ppt)
             {if(ppt->parent) ppt=ppt->parent;
              npt=ppt;
              while(npt) /* chk ppt is below path_pt */
                {if(npt==path_pt) {ppt=path_pt; break;}
                 npt=npt->tail;
                }
             }
           else ppt=path_pt;
          }
        /* ppt is now at part_level or opath eval root */
        strip_path(ppt); /* reduce path to original form, yank labels */
        sels_path(path_pt); /* reset sel dirty bits from path_pt down */
        update_part();
        size=part_leafs;
        if(ilk_pt->part_num>=partnum)
          {ilk_pt->part_list[partnum-1]=part_leafs;}
       }
    }
  byebye:
  if(size==0&&!part_exceed) size=(-2);
  flags=tmpflg; part_leafs=tmpleafs; part_level=tmplvl; part_exceed=tmpexd;
  path_pt=tmppt; outpt=tmpout;
  nsl_aseed=tmpa; nsl_bseed=tmpb; nsl_cseed=tmpc;
  return(size);
}

path_elm *sel_out_val(curpt,isabs,sub_num)
  path_elm *curpt; boolean isabs; int sub_num;
{ path_elm *tmppt=path_pt,*tmpout=outpt,*ppt; flag_elm tmpflg;
  int tmplvl=part_level,tmpleafs=part_leafs,evalvl,lvl,hd,tl;
  boolean tmpexd=part_exceed,isterm;
  if(!ilk_pt || !ilk_pt->curpt->combop) return(NULL);
  if(flags.state && !ilk_pt->curpt->invar)
    {path_error(PER_GEN,curpt,curpt,WRN_VAR_OUT); return(curpt);}
  tmpflg=flags;
  if(!flags.inp && flags.pamat)
    {path_error(PER_GEN,curpt,curpt,WRN_SEL_REC); return(NULL);}
  /* now eval path to list pos */
  flags.pamat=T; flags.last=F;
  if(!ilk_pt->opt) /* chk if outpath mounted */
    {if(flags.state)
       {path_error(PER_GEN,curpt,curpt,WRN_INL_VAR); return(NULL);}
     if(!oelm_init()) {curpt=NULL; goto byebye;}
    }
  ppt=ilk_pt->opt; curpt=ilk_pt->orefpt;
  evalvl=eval_level(ppt);
  isterm=term_path(ppt); /* set if outpt has been eval'd & is a terminal p*/
  tl=1+abs_tl(curpt); hd=abs_hd(curpt);
  if(isabs) /* absolute path */
    {part_leafs=SOV_ABS; lvl=tl+hd;
     if(!isterm && sub_num<1)
       {sub_num=(-sub_num); part_leafs=SOV_ALL; flags.last=T; goto chkskip;}
    }
  else  /* label path */
    {part_leafs=SOV_LAB; lvl=tl; evalvl-=hd;}
  if(sub_num<(-lvl)||sub_num>lvl)
    {path_error(PER_GEN,ppt,ppt,WRN_PRT_OOR,lvl,sub_num);
     curpt=NULL; goto byebye;}
  if(sub_num<1) sub_num+=lvl;
  chkskip:
  if(part_leafs==SOV_ALL || sub_num>=evalvl) /* opth not fully eval'd yet */
    {/* eval opath up to list */
     /* AND keep base path down to list, via rebflg, for later */
     part_level=sub_num; outpt=ppt; path_pt=curpt;
     /* if(path_pt->rebflg) signal_error(WRN_INT_ERR); */
     do_opath();
     sels_path(path_pt); /* reset sel dirty bits from path_pt down */
     update_part();
     part_level=tmplvl;
    }
  byebye:
  flags=tmpflg; part_leafs=tmpleafs; part_exceed=tmpexd;
  path_pt=tmppt; outpt=tmpout;
  return(curpt);
}
