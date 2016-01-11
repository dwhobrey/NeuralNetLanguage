/* Name: thirdpa2.c   Type: third pass routines   Date: 30 March 89
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

/* Runtime indiactors */
char flasher='*';
void mod_indicate() {if(mod_ison) {printf("\b%c",flasher); flasher^=0x1;}}

/* Simply fetch the next decl from net or struct, set ppt->decl_pt
*  and return decl number. Skip undefined decls
*/
mile next_decl(ppt)
  path_elm *ppt;
{ mile decl=0; decl_elm *dpt=ppt->decl_pt; boolean decok;
  if(flags.fly) /* if in fly mode get next decl from fly_di adrs */
    {dpt=ppt->net->decls; decl=fly_di->decl;
     /* Note decl=(-1) means a fly eval has finished & is returning */
     /*      decl=(0) means reached fly eval point */
     if(decl>0) while(dpt && dpt->declnum!=decl) dpt=dpt->tail;
     else dpt=NULL;
     if(!dpt) decl=0;
    }
  else
    do{if(dpt)
         {dpt=dpt->tail;
          if(flags.state && flags.build && dpt && dpt->declnum)
            {ds_stub=sip_inc(ds_stub);
             if(ds_stub->type==TYPE_NODE || ds_stub->type==TYPE_ARB)
               ds_stub->isdef=T;
            }
         }
       else
         {dpt=ppt->net->decls;
          if(flags.state && flags.build && dpt && dpt->declnum)
            {if(!ds_stub->hassubpt) ds_stub=sip_decl(ds_stub,dpt,F);
             ds_stub=ds_subpt(ds_stub);
             if(ds_stub->type==TYPE_NODE || ds_stub->type==TYPE_ARB)
               ds_stub->isdef=T;
            }
         }
       if(dpt && dpt->declnum)
         {if(dpt->nodes || dpt->sst_pos)
            {decok=(boolean)!(ppt->net->root_flag ||
                   (dpt->decltype!=TYPE_NODE && dpt->decltype!=TYPE_NET));
             if(flags.state || decok)
               {if(dpt->declnum>0) decl=dpt->declnum;
                break;}
            }
         }
      }while(dpt);
  ppt->decl_pt=dpt; ppt->decl=decl;
  return(decl);
}

/* Work out the next index */
mile next_indx(ppt)
  path_elm *ppt;
{ mile size=ppt->dary.fixval,indx=ppt->dary.atval; stub_elm *spt;
  if(flags.fly)
    {indx=fly_di->indx; /* fly stores indx+1 */
     if(!indx)  /* upon termintation & return indx=0 */
       {indx=(-1); goto byebye;}
     ++fly_di; --indx; /* re-adj indx */
    }
  else if(indx<0) indx=0; /* first time through, set to zero */
  else ++indx;
  if(flags.state && flags.build)
    {if(size<1)
       {if(indx==0 && !ppt->dary.ary_list) /* at an unused UA */
          {indx=(-1); goto byebye;}
        size=get_nth(ppt->dary.ary_list,indx);
        ppt->dary.hasval=size;
        if(size<0) ppt->dary.atval=size;
        else
          {ppt->dary.atval=indx; /* now position ds */
           if(indx==0) /* first time through */
             {spt=ds_subpt(ds_stub);
              if(ds_stub->type==TYPE_ARB || (spt && spt->type==SIP_NODE))
                {if(!spt) signal_error(EXIT_INT_ERR,2);
                 ds_stub=spt;}
             }
        /* else ds_stub=ds_arypt(ds_stub); */ /* DEBUG replaced this with: */
           else if(ds_stub->type==TYPE_ARB) /* added test for arb */
             ds_stub=ds_subpt(ds_stub);     /* and move to subpt */
           while(ds_stub && ds_stub->info.data.elm!=size)
              ds_stub=ds_arypt(ds_stub);
           if(!ds_stub)  signal_error(EXIT_INT_ERR,3);
           if(ds_stub->type==TYPE_NODE || ds_stub->type==TYPE_ARB)
             ds_stub->isdef=T;
          }
        return(size);
       }
     if(indx>=size) indx=(-1);
     else /* position ds in fixed ary */
       {if(indx==0) /* first time, make sure all elms of ary exist */
          {if(size>1 && ds_stub->type==TYPE_NODE) /*an unnobed ary of nodes*/
             {if(ds_stub->hasarypt||ds_stub->hassubpt)
                signal_error(EXIT_INT_ERR,4);
              ds_stub=sip_arnob(ds_stub);
             }
           spt=ds_subpt(ds_stub);
           if(ds_stub->type==TYPE_ARB || (spt && spt->type==SIP_NODE))
             ds_stub=spt;
           ds_stub=sip_ary(ds_stub,size);
          }
        else ds_stub=ds_arypt(ds_stub);
        if(!ds_stub || ds_stub->info.data.elm!=indx)
          signal_error(EXIT_INT_ERR,5);
       }
     goto byebye;
    }
  if(indx>=size) indx=(-1);
  if(size<1) /* we have a UA, signal an error */
    path_error(PER_GEN,ppt,ppt,ERR_UA_ENC);
  byebye:
  ppt->dary.atval=indx; ppt->dary.hasval=indx;
  return(indx);
}

void clear_fly(fpt)
  fly_elm *fpt;
{ chk_free(fpt->dipt); fremem(fpt);
}

/* move down fly stack by adding a new elm */
void push_fly()
{ fly_stk *fpt;
  if(fly_top && fly_top->fly_top && (fly_top->fly_top->num==AUX_AFLY))
    {/* found a aux special input decl, root fly list already setup... */
     fly_top->fly_top->num=AUX_BFLY; /* switch off future tests */
     return;}
  fpt=malmem(fly_stk,1);
  fpt->tail=NULL; fpt->parent=fly_bot;
  fpt->fly_top=NULL; fpt->fly_bot=NULL; /* no entries yet */
  fpt->pile_list=NULL;
  if(fly_top) fly_bot->tail=fpt; else fly_top=fpt;
  fly_bot=fpt;
}

/* pop up fly stack, free popped elm, err if not empty */
void pop_fly(ppt)
  path_elm *ppt;
{ fly_stk *fpt;
  if(fly_bot)
    {if(fly_bot->fly_bot) signal_error(EXIT_INT_ERR,6); /* wasn't empty! */
     pile_free(ppt,fly_bot->pile_list,T);
     fpt=fly_bot->parent; fremem(fly_bot); fly_bot=fpt;
     if(fpt) fpt->tail=NULL; else fly_top=NULL;
    }
  else signal_error(EXIT_INT_ERR,7);
}

/* fetch a fly's compressed relative address from float pos to refpt
*  Note doesn't include cur refpt(decl,indx) addres ==>(0,0)
*  numlvls=0,1.. (0=refpt lvl only) ==>(0,0)
*  Note fly addresses are similar to node addresses, but not quite the same
*/
di_elm *fly_adrs(refpt,numlvls)
  path_elm *refpt; int numlvls;
{ di_elm *dpt,*tpt;
  dpt=malmem(di_elm,1+numlvls); tpt=dpt+numlvls;
  tpt->decl=0; tpt->indx=0;
  while(tpt!=dpt)
    {--tpt; refpt=refpt->parent;
     tpt->decl=refpt->decl; tpt->indx=refpt->dary.hasval;
     if(tpt->indx>=0) tpt->indx++;
    }
  return(dpt);
}

/* store an inp_elm in fly_list on cur fly_stack
*  spt points to correct floated path stack
*  build compressed fly address.
*/
void fly_store(spt,adrs,innum,tilde)
  fly_stk *spt; di_elm *adrs; int innum; boolean tilde;
{ fly_elm *fpt=malmem(fly_elm,1);
  fpt->tail=NULL; fpt->dipt=adrs; fpt->num=innum; fpt->tilde=tilde;
  if(!spt) signal_error(EXIT_INT_ERR,8); /* no fly stack ?!! */
  if(spt->fly_bot) spt->fly_bot->tail=fpt; else spt->fly_top=fpt;
  spt->fly_bot=fpt;
}

/* chop off any postfix float or abs path designators */
char *fly_filter(spt)
  char *spt;
{ for(;;)
  {switch(*spt)
    {case '$': if(*(spt+1)=='$') ilk_pt->glob=T;
     case '/': case '.': case '~': case '@': ++spt; break;
     default: return(spt);
    }
  }
}

/* work out how many levels to float Inpath,
*  returns (0) for no float, (-1) float to root, (+x) up from cur pos
*  (-2) if '~'or'~~' to same net, ie stick In in fly list
*/
int fly_level(relvl,spt)
  int relvl; char *spt;
{ int lvl;
  if(*spt!='~') return(0);
  lvl=(-2);
  ++spt; if(*spt=='~') ++spt;
  switch(*spt)
    {case '/': return(-1);
     case '@': lvl=0; while(*spt=='@') {++spt; ++lvl;}
       if(lvl==relvl) return(-1);
       if(lvl>relvl) {in_error(WRN_FLO_OVR); return(-1);}
     default: ;
    }
  return(lvl);
}

/* put the special aux In decl in root fly list */
void fly_aux()
{ di_elm *dpt=malmem(di_elm,1);
  dpt->decl=0; dpt->indx=0;
  if(fly_top) signal_error(EXIT_INT_ERR,9);
  push_fly();
  fly_store(fly_top,dpt,(int)AUX_AFLY,T);
}

/* see if next inpath needs to be floated, return first non floated inpath
*/
inp_elm *fly_check(refpt,inp)
  path_elm *refpt; inp_elm *inp;
{ di_elm *adrs; fly_stk *skpt; int kk,relvl,curlvl;
  if(!inp) return(NULL);
  curlvl=rel_level(refpt);
  while(kk=fly_level(curlvl,inp->name))
    {if(kk<0) {relvl=curlvl; if(kk==(-2)) skpt=fly_bot; else skpt=fly_top;}
     else {relvl=kk; skpt=fly_bot; while(kk--) skpt=skpt->parent;}
     if(!skpt) signal_error(EXIT_INT_ERR,10);
     adrs=fly_adrs(refpt,relvl); kk=inp_to_num(refpt,inp);
     fly_store(skpt,adrs,kk,(boolean)(*(inp->name+1)!='~'));
     inp=inp->tail;
    }
  return(inp);
}

/* do an inp str & an oelm in ilk_pt, or var in vee_pt if flags.state
*  NOTE for do_inp calls we are outside ds, so ppt->decl_pt need not be set
*  HOWEVER, for do_varss calls from struct inits this is not so...
*/
void path_instr(ppt,pstr,clnilk)
  path_elm *ppt; char *pstr; boolean clnilk;
{ path_elm *tmppt=path_pt,*tmpinpt=inpt; byte lab_id; boolean dumflg;
  if(ppt->tail) signal_error(EXIT_INT_ERR,11);
  path_pt=ppt; inpt=rootpt;
  pstr=fly_filter(pstr); /* chk for floated paths etc, chop off */
  if(!*pstr) signal_error(EXIT_INT_ERR,12);
  lab_id=stack_str(path_pt,pstr);
  if(flags.state) vee_pt->lab_id=lab_id;
  if(ds_pool)
    {ds_stub=find_dspt(path_pt,(boolean *)&dumflg); flags.build=dumflg;}
  do_nxt_decl();
  if(clnilk) tidy_ilk(ilk_pt);
  yank_lab(path_pt,lab_id);
  clear_pelm(path_pt);
  path_pt=tmppt; inpt=tmpinpt;
}

/* Do the inpath--Olist by expanding inpath in inout mode */
void do_inp(rpt,inp)
  path_elm *rpt; inp_elm *inp;
{ ilk_elm *tmpilk=ilk_pt,curilk; vee_elm *veed; ds_sea_elm *tmprot=ds_pool;
  flag_elm tmpflag; int tmpef=err_file_num,tmpel=err_line_num,slot1;
  tmpflag=flags;
  if(rpt->tail) signal_error(EXIT_INT_ERR,13);
  if(ds_pool) slot1=push_stub(ds_stub);
  flags.inout=T; flags.inp=T; flags.info=F; flags.ary=F;
  flags.pamat=F; flags.last=F; flags.state=F; flags.fork=F; flags.wild=F;
  mod_indicate();
  ilk_pt=(ilk_elm *)&curilk;
  zap_ilk(ilk_pt);
  ilk_pt->baspt=inp->out_list; ilk_pt->curpt=inp->out_list;
  veed=(vee_elm *)&ilk_pt->veed;
  veed->curpt=NULL; veed->usedlvl=0; veed->tabpt=NULL;
  err_file_num=rpt->net->sor_num; err_line_num=inp->line_num;
  /*do each oelm in turn, note get_conx() may read nxt oelm if inp not empty*/
  while(ilk_pt->curpt->combop)
    {path_instr(rpt,inp->name,T);
     ++(ilk_pt->curpt); ilk_pt->discan=F;
     free_tab(veed->tabpt); veed->tabpt=NULL;
    }
  ilk_pt=tmpilk;
  flags=tmpflag; ds_pool=tmprot;
  if(ds_pool) ds_stub=pop_stub(slot1);
  err_file_num=tmpef; err_line_num=tmpel;
}

void do_relinp(refpt,inp)
  path_elm *refpt; inp_elm *inp;
{ path_elm *ppt=NULL,*tmproot=NULL; char *ostr=inp->name;
  switch(*ostr)
    {case '/': ppt=rootpt; break;
     case '$': ostr+=2; ppt=glob_ref(org_pt,ostr); break;
     case '.': ostr+=2; if(*ostr!='@') break;
     case '@':
       while(*ostr=='@')
         {++ostr;
          if(ppt->parent) ppt=ppt->parent;
          else path_error(PER_GEN,org_pt,org_pt,WRN_OUT_OFL);
         }
       break;
     default: ;
    }
  if(ppt)
    {tmproot=rootpt; /* move eval pt to higher net */
     rootpt=dup_path(ppt,F); /* new path pary info is zapped, apart from ary*/
     refpt=rootpt;
     while(refpt->tail) refpt=refpt->tail;
     blank_pelm(refpt); /*DEBUG 27/4/92 added blank_pelm */
    }
  flo_pt=refpt; /* DEBUG do not need to save flo_pt here */
  do_inp(refpt,inp);
  if(tmproot) {free_path(rootpt); rootpt=tmproot;}
}

void do_inpss(refpt)
  path_elm *refpt;
{ inp_elm *inp=refpt->net->inp; path_elm *tmporg=org_pt,*tmpflo=flo_pt;
  org_pt=refpt; flo_pt=refpt;
  if(refpt->tail) signal_error(EXIT_INT_ERR,14);
  while(inp=fly_check(refpt,inp)) 
  {	do_relinp(refpt,inp); 
	inp=inp->tail;}
  org_pt=tmporg; flo_pt=tmpflo;
}

/* eval a floated path eg ('~'or'~~') plus .\--- or ..\--- or \--- etc
*  on entry we're at original INS decl path node
*/
void do_a_fly(refpt)
  path_elm *refpt;
{ inp_elm *inp; path_elm *tmproot=NULL,*tmporg=org_pt,*tmpflo=flo_pt;
  /*check if reached fly's orginal decl path node (decl=0), */
  /* return if haven't (decl>0) or fly is returning (decl<0) */
  if(fly_di->decl) return;
  if(!fly_pt) signal_error(EXIT_INT_ERR,15);
  org_pt=refpt; /* set inpaths decl node pos, flo_pt was set by do_flys() */
  /* chk for '~~'or'~', cp path if '~~' else set flo_pt to org_pt if '~' */
  if(!fly_pt->tilde)/*must cp path & set up new one down to flo_pt as ref*/
    {tmproot=rootpt; /* flo_pt is higher than refpt */
     rootpt=dup_path(flo_pt,F);
       /* new path pary info is zapped, apart from ary*/
     refpt=rootpt;
     while(refpt->tail) refpt=refpt->tail;
    }
  flo_pt=refpt; inp=num_to_inp(fly_pt->num);
  do_inp(refpt,inp);
  if(tmproot) {free_path(rootpt); rootpt=tmproot;}
  org_pt=tmporg; flo_pt=tmpflo;  fly_di->decl=(-1); /* signal fly finished*/
}

void do_module();

/* eval list of floated Ins in fly list */
void do_flys(refpt,fpt)
  path_elm *refpt; foot fpt;
{ fly_elm *lpt; net_elm *tmpnpt=refpt->net; refpt->net=NULL;
  flags.fly=T; fly_pt=fly_bot->fly_top;
  flo_pt=refpt; /* set floated inpaths node pos for later */
  while(fly_pt)
     {fly_di=fly_pt->dipt;
      do_module(refpt,fpt,NULL);
      /* remove fly from list */
      lpt=fly_pt; fly_pt=fly_pt->tail; clear_fly(lpt);
     }
  /* adj fly_top, fly_bot etc */
  flags.fly=F; fly_bot->fly_top=NULL; fly_bot->fly_bot=NULL;
  refpt->net=tmpnpt;
}
/*------------------------------------------------------------------------*/
/* Var declaration routines */

byte var_pthlvl(ppt)
  path_elm *ppt;
{ byte pthlvl=1;
  while(ppt->parent)
    {if(ppt->partflg && ppt->pat_id==vee_pt->lab_id) ++pthlvl;
     ppt=ppt->parent;}
  return(pthlvl);
}

var_elm *var_parent(vpt)
  var_elm *vpt;
{ if(!vpt || !vpt->parent ||vpt->level<1) return(NULL);
  vpt=vpt->parent;
  while(vpt && !vpt->subase) vpt=vpt->parent;
  return(vpt);
}

#define HOL_EOL 1 /* bit set when at EOL */
#define HOL_VER 2 /* bit set when in vertical mode */
#define HOL_ISL 4 /* bit set when var value is a list of sorts */

var_elm *var_repeat(vpt,pthlvl,holbits)
  var_elm *vpt; byte pthlvl; mile holbits;
{ if(vpt->level==0) return(vpt);
  if(holbits&HOL_ISL) /* chk for list initialiser */
    {if(!(holbits&HOL_EOL)
        &&!(holbits&HOL_VER)) return(vpt);
    }
  if(vpt->tail) return(vpt->tail);
  while(vpt && !vpt->subase) vpt=vpt->parent;
  if(!vpt) signal_error(EXIT_INT_ERR,16);
  if(vpt->parent && vpt->parent->level>=pthlvl && vpt->parent->tail)
    return(vpt->parent->tail);
  return(vpt);
}

/* returns T if first member of ds struct is a struct */
boolean ds_has_substruc()
{ stub_elm *spt=ds_subpt(ds_stub);
  if(!spt) return(F);
  if(ds_stub->type==TYPE_ARB) {if(!(spt=ds_subpt(spt))) return(F);}
  return((boolean)(spt->type==TYPE_NODE || spt->type==TYPE_STRUCT));
}

/* chk value type matches atom var type */
boolean var_cast_check(vpt,spt,ppt,usecast,isstruc)
  var_elm *vpt; stub_elm *spt; path_elm *ppt; boolean *usecast,*isstruc;
{ char *str=vpt->cast,*tag; int valtyp,reltyp,atmtyp;
  tag=ppt->decl_pt->tag;
  atmtyp=ppt->decl_pt->decltype;
  *usecast=F; *isstruc=(boolean)(atmtyp==TYPE_NODE || atmtyp==TYPE_STRUCT);
  if(atmtyp!=spt->type)
    {path_error(PER_GEN,ppt,ppt,WRN_VAR_TYP,(mile)atmtyp,(mile)spt->type);
     return(F);}
  if(!str) return(T);
  reltyp=cast_type(*str); ++str;
  valtyp=(reltyp&STAR_TYPE);
  if(valtyp==TYPE_UNKNOWN) /* chk unknown type */
    {if(*str=='(') /* see if a weird sort of type cast */
       {path_error(PER_GEN,ppt,ppt,WRN_VAR_UNK,str); *usecast=T;}
     return(T);
    }
  if(valtyp==TYPE_NODE) /* a node cast, chk it matches spt type */
    {if(!tag || !*tag)
       {path_error(PER_GEN,ppt,ppt,WRN_VAR_TMS,str,(mile)atmtyp);
        return(F);}
     if((strcmp(tag,str)!=0) || (atmtyp!=TYPE_NODE))
       {path_error(PER_GEN,ppt,ppt,WRN_VAR_STR,tag,str,(mile)atmtyp);
        return(F);}
     return(T); /* node cast match, but skip cast str though */
    }
  /* chk normal type cast */
  if(((atmtyp&STAR_MASK)==STAR_ONE)&&(ppt->decl_pt->size==0)) atmtyp-=STAR_ONE;
  /*DEBUG added next line 24/10/92 */
  *isstruc=(boolean)(atmtyp==TYPE_NODE || atmtyp==TYPE_STRUCT);
  if(atmtyp!=reltyp)
    {path_error(PER_GEN,ppt,ppt,WRN_VAR_TYP,(mile)atmtyp,(mile)reltyp);
     return(F);}
  if(valtyp==TYPE_STRUCT) /* check type names */
    {str+=7; if(*str) ++str; /* step over '(struct tag)' to struct tag */
     if(*str)
       {if(*str=='_') ++str; /*DEBUG FEB91*/
        if(!strideq(tag,str))
          {path_error(PER_GEN,ppt,ppt,WRN_VAR_TST,tag,str,(mile)atmtyp);
           return(F);}
       }
     /* no struct tag found, treat like wild tag */
    }
  else *usecast=T; /* user wants var to be cast */
  return(T);
}

/* compare path struct/node tag with var tag
*  return T if types ok */
boolean var_struct_eq(vpt,ppt)
  var_elm *vpt; path_elm *ppt;
{ char *tag,*str=vpt->cast; int valtyp;
  if(is_read_struct(ppt->decl_pt)||!ppt->decl_pt->declnum) return(F);
  if(!str || !*str) return(T);
  valtyp=cast_type(*str); ++str;
  if(valtyp==TYPE_STRUCT) /* check type names */
    {str+=7; if(*str) ++str; /* step over '(struct tag)'*/}
  else if(valtyp!=TYPE_NODE) signal_error(EXIT_INT_ERR,17);
  if(*str)
    {tag=ppt->decl_pt->tag; if(*str=='_') ++str; /*DEBUG FEB91*/
     if(!strideq(tag,str))
       {path_error(PER_GEN,ppt,ppt,WRN_VAR_TST,tag,str,(mile)valtyp);
        return(F);}
    }
  return(T);
}

/* chk casts in parents of vpt & ppt
*  return T if ok */
boolean var_cast_path(vpt,ppt)
  var_elm *vpt; path_elm *ppt;
{ while(ppt && (vpt=var_parent(vpt)))
    {if(!var_struct_eq(vpt,ppt)) return(F);
     ppt=ppt->parent;}
  return(T);
}

extern void adj_conx();

void set_isdefs(rpt)
  stub_elm *rpt;
{ if(rpt) rpt->isdef=T;
  adj_conx(rpt,(int)0,(-1));
}

/* during do_module descent in flags.state mode when at an atom
*  chk if special compiler added struct/atom & initialise if so */
void do_com_atom(ppt)
  path_elm *ppt;
{ stub_elm *spt,*lpt,*npt,*rpt; word len;
  char *tag=ppt->net->tag,buff[MAX_PATH_STR];
  if(ds_stub->isdef || !tag || strncmp(tag,"NSLNSL",6)!=0) return;
  tag+=6;
  lpt=ds_parent(ds_stub); spt=ds_tail(ds_stub);
  ds_stub->isdef=T; lpt->isdef=T; /* set structs parent */
  if(strcmp(tag,"time")==0) /* T0 : time struct:set members to {1,0}*/
    {ds_value(ds_stub)->nat=(int)1;
     ds_value(spt)->nat=(int)0; spt->isdef=T;
    }
  else if(strcmp(tag,"io")==0) /* io driver arg struct: {1,0L}*/
    {ds_value(ds_stub)->nat=(int)1;
     ds_value(spt)->num=(long)0L; spt->isdef=T;
    }
  else if(strcmp(tag,"id")==0) /* T1 : node or group struct */
    {if(lpt->conds==NISNODE) /* root node struct, set name & path */
       {npt=dsroot(); npt->conds=NISNODE;
        ds_value(ds_stub)->num=npt->info.nodenum;
        ds_stub=spt;
        while(ppt && !ppt->net->root_flag) ppt=ppt->parent;
        if(!ppt) signal_error(EXIT_INT_ERR,18);
        len=node_adrs(buff,ppt,T,F,T);
        ds_stub=ds_put_str(ds_stub,buff,len);
       }
     else /* group struct to an input */
       {/* look for a conx in cur struct to get group details from */
        spt=ds_stub;
        do{spt=ds_parent(spt);} while(spt->type!=TYPE_STRUCT);
        spt=ds_parent(spt); lpt=spt; npt=NULL;
        for(;;)
          {rpt=ds_subpt(spt);
           if(rpt==lpt) break;
           if(rpt && rpt->type==SIP_NODE) {npt=rpt; break;}
           lpt=spt; spt=ds_parent(spt);
          }
        if(npt)
          {/* set groups name */
           ds_value(ds_stub)->num=ds_value(npt)->node.base;
           ds_stub=ds_tail(ds_stub);
           /* get conx's node adrs down to conx (dip data) */
           len=conx_adrs(buff,(node_elm *)&(ds_value(npt)->node));
           ds_stub=ds_put_str(ds_stub,buff,len);
          }
       }
    }
}
/* chks if str starts with a cast ($^Dc), returns cast code if so, else 0 */
char hascast(apt)
  char *apt;
{ if(*apt=='$'&&(*(apt+1)==SEL_XS)&&(*(apt+2)==SEL_XS_TYPE))
    return((char)cast_type(*(apt+3)));
  return((char)0);
}
/* returns T if val str is a list of sorts */
boolean haslist(apt)
  char *apt;
{ char castchr; ++apt; /* step on eval flag */
  if((castchr=hascast(apt))) apt+=4; /* step over any cast */
  return((boolean)(*apt=='[')); /* what about STAR_MODE flag?? */
}

/* get next var value from vpt, chk if vpt is a list. Returns HOL_... bits:
*  HOL_ISL if is list
*  HOL_VER if vertical cast was used with list
*  HOL_EOL if HOL_ISL && could not get another val from list
*/
mile var_value(vpt)
  var_elm *vpt;
{ tab_elm *tpt=vee_pt->tabpt,*parpt=NULL; ary_elm *arypt; double *elpt;
  char *str=vpt->val+1,*apt=str,castchr; mile holbits=0;
  boolean tmpvax=vax_flg; vax_flg=vpt->axvar;
  while(tpt) {if(tpt->vpt==vpt) break; parpt=tpt; tpt=tpt->tail;}
  if(tpt) /* chk if on list table already */
    {holbits=tpt->holbits; arypt=(ary_elm *)&tpt->pippa; goto do_list;}
  if((castchr=hascast(apt))) apt+=4; /* chk for cast */
  if(*apt=='[') /* fetch nxt list val */
    {tpt=malmem(tab_elm,1); if(parpt) parpt->tail=tpt; else vee_pt->tabpt=tpt;
     arypt=(ary_elm *)&tpt->pippa; tpt->tail=NULL; tpt->vpt=vpt;
     zap_ary(arypt); arypt->ary=strsave(str); reset_sels(arypt->ary);
     arypt->isdary=T; holbits|=HOL_ISL; if(castchr&STAR_MODE) holbits|=HOL_VER;
     do_list: elpt=var_fetch_elm(arypt);
     if(!elpt||!arypt->ary_list) /* eol.., remove from table */
       {if(tpt->tail) {if(parpt) parpt->tail=tpt->tail;} tpt->tail=NULL;
        if(!parpt) vee_pt->tabpt=NULL; free_tab(tpt);
        holbits|=HOL_EOL;
        exp_value.typ=TYPE_DOUBLE; exp_value.val.dd=(double)0.0;
       }
     else
       {exp_value.typ=lst_typ(arypt->ary_list)&STAR_TYPE;
        if(exp_value.typ==TYPE_PT) exp_value.val.pp=(char *)((long)(*elpt));
        else exp_value.val.dd=(*elpt);
        tpt->holbits=holbits;
       }
    }
  else eval_var(str); /* fetch atomic val */
  vax_flg=tmpvax;
  return(holbits);
}

/* initialise a simple atom: should not be a struct or array initialiser
*  Note, 'inlevel' refers to the path level a var initialiser appeared at.
*  When set, only initialisers at a higher inlevel can override prior value.
*/
mile do_atom(vpt,ppt,usecast,inlevel)
  var_elm *vpt; path_elm *ppt; boolean usecast; byte inlevel;
{ mile stype,holbits=0; word len; boolean evalit; char *bpt,buff[MAX_VAL_STR];
  if(!ds_stub) signal_error(EXIT_INT_ERR,19);
  if(ds_stub->isvar)
    {if(*ds_level(ds_stub)>inlevel) return(holbits);}
  if(vpt->subpt) /* must be a type mismatch, or struct init member missing */
    {path_error(PER_GEN,ppt,ppt,WRN_VAR_MIS); return(holbits);}
  stype=(mile)ds_stub->type;
  if(stype&STAR_MASK)
    {if(!ds_stub->info.data.size &&((stype&STAR_MASK)==STAR_ONE))
       stype&=STAR_TYPE;
     else stype=TYPE_PT;
    }
  if(ds_stub->conds==NISSTR) ds_stub=ds_rem_str(ds_stub);
  evalit=(boolean)(*vpt->val==VAR_EVAL);
  bpt=buff;
  if(usecast) bpt=fcat(bpt,(vpt->cast+1));
  if(evalit)
    {holbits=var_value(vpt);
     switch(stype)
       {case TYPE_VOID: case SIP_CONX:
          if(usecast) strcpy(bpt,"0\0");
          else ds_value(ds_stub)->num=0;
          break;
        case TYPE_SHORT: case TYPE_USSHORT: case TYPE_INT: case TYPE_USINT:
          if(exp_value.typ&STAR_MASK)
            {path_error(PER_GEN,ppt,ppt,WRN_VAR_MIS);
              exp_value.val.dd=(double)0.0;}
          if(usecast) sprintf(bpt,"%d\0",(int)exp_value.val.dd);
          else ds_value(ds_stub)->nat=(int)exp_value.val.dd;
          break;
        case TYPE_LONG: case TYPE_USLONG:
           if(exp_value.typ&STAR_MASK)
             {path_error(PER_GEN,ppt,ppt,WRN_VAR_MIS);
              exp_value.val.dd=(double)0.0;}
          if(usecast) sprintf(bpt,"%ld\0",(long)exp_value.val.dd);
          else ds_value(ds_stub)->num=(long)exp_value.val.dd;
          break;
        case TYPE_PT:
          if(!(exp_value.typ&STAR_MASK)&&!(exp_value.typ==TYPE_PT))
             {path_error(PER_GEN,ppt,ppt,WRN_VAR_MIS);
              exp_value.val.pp=(char *)NULL;}
          if(((ds_stub->type)&STAR_TYPE)==TYPE_CHAR)
            {usecast=T;
             if(!exp_value.val.pp) fcat(bpt,"0L");
             else fcat(bpt,exp_value.val.pp);
             break;}
          if(usecast) sprintf(bpt,"%ld\0",(long)exp_value.val.pp);
          else ds_value(ds_stub)->num=(long)exp_value.val.pp;
          break;
        case TYPE_FLOAT:
          if(exp_value.typ&STAR_MASK)
             {path_error(PER_GEN,ppt,ppt,WRN_VAR_MIS);
              exp_value.val.dd=(double)0.0;}
          if(usecast) sprintf(bpt,"%g\0",(float)exp_value.val.dd);
          else ds_value(ds_stub)->floa=(float)exp_value.val.dd;
          break;
        case TYPE_DOUBLE:
          if(exp_value.typ&STAR_MASK)
             {path_error(PER_GEN,ppt,ppt,WRN_VAR_MIS);
              exp_value.val.dd=(double)0.0;}
          if(usecast) sprintf(bpt,"%g\0",(double)exp_value.val.dd);
          else ds_value(ds_stub)->doub=(double)exp_value.val.dd;
          break;
        case TYPE_CHAR: case TYPE_USCHAR:
          if(exp_value.typ&STAR_MASK)
             {path_error(PER_GEN,ppt,ppt,WRN_VAR_MIS);
              exp_value.val.dd=(double)0.0;}
          sprintf(bpt,"%d\0",(int)exp_value.val.dd); evalit=F;
          break;
        case SIP_STR:
          signal_error(EXIT_INT_ERR,20); break;
        case SIP_NODE: path_error(PER_GEN,ppt,ppt,WRN_VAR_CNX); break;
        case TYPE_UNION: path_error(PER_GEN,ppt,ppt,WRN_VAR_UNI); break;
        case TYPE_NODE: case TYPE_STRUCT: case TYPE_ARB:
        case TYPE_NET:
        default: signal_error(EXIT_INT_ERR,21);
       }
    }
  else fcat(bpt,(vpt->val+1));
  if(!evalit || usecast)
    {len=(word)(1+strlen(buff)); if(len&1) {*(buff+len)='\0'; ++len;}
     ds_stub=ds_put_str(ds_stub,buff,len);
    }
  set_isdefs(ds_stub);
  if(inlevel) {ds_stub->isvar=T; *ds_level(ds_stub)=inlevel;}
  return(holbits);
}

/* given type of cur ds elm in elmtyp & isstruc & isarray,
*  position vpt at start of initialisers for ds elm
*  returns NULL if some sort of type/cast err
*/
var_elm *var_pos(vpt,elmtyp,isstruc,isarray)
  var_elm *vpt; int elmtyp; boolean isstruc,isarray;
{ int reltyp;
  if(isstruc) /* we're at a node or struct inside ds */
    {if(isarray) /* we're at an array of them */
       {if(vpt->subpt) /* cur vpt pts at an array or struct */
          {if(vpt->cast) /* see if cast used to force array or struct...*/
             {reltyp=cast_type(*vpt->cast);
              if(reltyp==TYPE_NODE || reltyp==TYPE_STRUCT)
                {if(cast_array(vpt->cast)) /* array of nodes/structs */
                   {if(vpt->subpt->subpt) /* move to subpt if possible */
                      {vpt=vpt->subpt;}
                    else /* no subpt->subpt so stop at vpt */
                      {;}
                   }
                 else /* cast to node/struct so stop at vpt */
                   {;}
                }
              else /* weird cast to substruct of sorts */
                {path_error(PER_GEN,path_pt,path_pt,WRN_VAR_NOD);
                 return(NULL);}
             }
           else /* no cast, so see if subpt */
             {if(vpt->subpt->subpt && !ds_has_substruc())
                {if(!vpt->subpt->ispath) vpt=vpt->subpt;}
              else
                {;} /* stick with cur subpt & repeat over it */
             }
          }
        else /* vpt pts at an atom, struct/node expected */
          {/* repeat atom over struct/node */
           path_error(PER_GEN,path_pt,path_pt,WRN_VAR_NOD);}
       }
     else /* node or struct expected, but not an array */
       {if(vpt->subpt) /* cur vpt pts at an array or struct */
          {if(vpt->cast) /* see if cast used to force array or struct...*/
             {reltyp=cast_type(*vpt->cast);
              if(reltyp==TYPE_NODE || reltyp==TYPE_STRUCT)
                {if(cast_array(vpt->cast)) /* array of nodes/structs */
                   {/* array not expected */
                    path_error(PER_GEN,path_pt,path_pt,WRN_VAR_ARY);
                    if(vpt->subpt->subpt && !ds_has_substruc())
                      {if(!vpt->subpt->ispath) vpt=vpt->subpt;}
                    else
                      {;} /* stick with cur vpt */
                   }
                 else /* cast to node/struct so stop at vpt */
                   {;}
                }
              else /* weird cast to substruct of sorts */
                {path_error(PER_GEN,path_pt,path_pt,WRN_VAR_NOD); return(NULL);}
             }
           else /* no cast, so stop at vpt */
             {;}
          }
        else /* vpt pts at an atom, struct/node expected */
          {/* repeat atom over struct/node */
           path_error(PER_GEN,path_pt,path_pt,WRN_VAR_NOD);}
       }
    }
  else /* we're now at an atom inside ds */
    {if(isarray) /* we're at an array of them */
       {if(vpt->subpt) /* cur vpt pts at an array or struct */
          {if(vpt->cast) /* see if cast used to force array or atom type...*/
             {reltyp=cast_type(*vpt->cast);
              if(reltyp==elmtyp)
                {if(cast_array(vpt->cast)) /* array of atoms */
                   {if(vpt->subpt)
                      {vpt=vpt->subpt;} /* move to subpt if possible */
                    else /* use this vpt */
                      {;}
                   }
                 else /* cast to atom so stop at vpt */
                   {;}
                }
              else /* type mismatch */
                {path_error(PER_GEN,path_pt,path_pt,WRN_VAR_TYP,
                            (mile)elmtyp,(mile)reltyp);
                 return(NULL);}
             }
           else /* no cast, so see if subpt */
             {if(vpt->subpt)
                {vpt=vpt->subpt;} /* more to it */
              else {;}
             }
          }
        else /* vpt pts at a single atom, use it for all of array */
          {;} /* signal warning ? */
       }
     else /* atom expected, but not an array */
       {if(vpt->subpt) /* cur vpt pts at an array or struct */
          {/* user must be initialising a path array of elms... */
           vpt=vpt->subpt;
          }
        else /* vpt pts at an atom */
          {;}
       }
    }
  return(vpt);
}

void do_struct();

/* move to a parent node struct to initialise */
void do_par_struct(vpt,refpt,inlevel)
  var_elm *vpt; path_elm *refpt; byte inlevel;
{ path_elm *tmptail; stub_elm *spt; char *ap=vpt->name; int slot1;
  spt=ds_parent(ds_stub);
  while(*ap=='@')
    {refpt=refpt->parent; spt=move_to_node(spt);
     if(!refpt||!spt) return;
     ++ap;
    }
  slot1=push_stub(ds_stub); ds_stub=spt;
  tmptail=refpt->tail; refpt->tail=NULL;
  do_struct(vpt,refpt,tmptail->net_base,inlevel);
  ds_stub=pop_stub(slot1);
  refpt->tail=tmptail;
}
/* initialise a struct or node
*  Note this is very similar to do_module! */
void do_struct(vpt,refpt,net_base,inlevel)
  var_elm *vpt; path_elm *refpt; yard net_base; byte inlevel;
{ path_elm *newpt; var_elm *tmpvpt,*newvpt; foot fpt=refpt->decl_pt->sst_pos;
  mile net_indx,err,holbits; int elmtyp,slot1,slot2,tmpel=err_line_num;
  boolean tmpwild,usecast,isstruc,isarray,istail,recur,resary;
  err_line_num=vpt->line_num;
  newpt=new_path(refpt); /* set up a path elm */
  claim_net((net_elm **)&newpt->net,fpt);
  slot1=push_stub(ds_stub);
  newpt->net_base=net_base;
  vpt=vpt->subpt; tmpwild=flags.wild;
  if(!vpt) signal_error(WRN_VAR_NOD);
  else while(vpt) /* loop over var initialisers for struct/node */
    {err_line_num=vpt->line_num;
     var_init(newpt,vpt->name);
     flags.wild|=newpt->curwldflg;
     newpt->decl_pt=NULL; newpt->lab_pt=NULL;
     if(*vpt->name=='@') do_par_struct(vpt,refpt,inlevel);
     else
     while(get_nxt_decl(newpt))
       {err=LEX_OK;
        if(is_read_struct(newpt->decl_pt)||!newpt->decl_pt->declnum) break;
        if(!newpt->dary.ary) err=make_path(newpt);
        else
          err=get_ary_info((ary_elm *)&newpt->dary,newpt,(boolean *)&resary);
        if(err==LEX_OK)
          {slot2=push_stub(ds_stub); tmpvpt=vpt;
           elmtyp=newpt->decl_pt->decltype;
           isarray=(boolean)(newpt->decl_pt->size!=1);
           isstruc=(boolean)(elmtyp==TYPE_NODE || elmtyp==TYPE_STRUCT);
           newvpt=vpt; vpt=var_pos(vpt,elmtyp,isstruc,isarray);
           istail=(boolean)!((vpt==newvpt)&&(isarray||isstruc));
           newvpt=vpt;
           err_line_num=vpt->line_num;
           if(vpt) while((net_indx=next_indx(newpt))>=0)
             {if(var_cast_check(vpt,ds_stub,newpt,(boolean *)&usecast,
                        (boolean *)&isstruc))
                {if(isstruc) /* recurse to do struct */
                   do_struct(vpt,newpt,(yard)0,inlevel);
                 else /* reached an atom */
                   {recur=F; loop:
                    holbits=do_atom(vpt,newpt,usecast,inlevel);
                    if(holbits&HOL_ISL) /* chk for list initialiser */
                      {if(holbits&HOL_EOL) /*eol, retry & chk for recursion*/
                         {if(recur) break; recur=T; goto loop;}
                       if(holbits&HOL_VER) break; /* stop if in vert mode */
                       continue;
                      }
                   }
                 if(istail) {vpt=vpt->tail; if(!vpt) vpt=newvpt;}
                }
              else break; /* if type mismatch, terminate this array */
             }
           vpt=tmpvpt; err_line_num=vpt->line_num;
           ds_stub=pop_stub(slot2);
          }
        clear_ary((ary_elm *)&newpt->dary);
       }
     ds_stub=restore_stub(slot1);
     flags.wild=tmpwild;
     vpt=vpt->tail; /* next struct member initialiser */
     nul_free(newpt->pat);
    }
  err_line_num=tmpel;
  ds_stub=pop_stub(slot1);
  release_net((net_elm **)&newpt->net);
  rem_path(newpt);
}

/* called frm go_dw_lvl when at a var terminal */
/* Note if doing an ary elm, chk isdef flag set in all prior ary elms */
void do_var(indx,isinl)
  mile indx; boolean isinl;
{ var_elm *vpt; ds_sea_elm *tmprot; stub_elm *rpt; yard net_base=0;
  int tmpel=err_line_num,elmtyp,slot1,big_recur=0; mile holbits;
  byte pthlvl,inlevel; boolean usecast,isstruc,isarray,recur;
  elmtyp=path_pt->decl_pt->decltype;
  isarray=(boolean)(path_pt->decl_pt->size!=1);
  isstruc=(boolean)(elmtyp==TYPE_NODE || elmtyp==TYPE_STRUCT);
  vpt=vee_pt->curpt; err_line_num=vpt->line_num;
  tmprot=ds_pool; if(ds_pool) slot1=push_stub(ds_stub);
  /* position ds_stub at cur var to be initialised */
  if(!flags.build || !ds_stub) /* chk var path reached a node */
    {if(elmtyp==TYPE_NODE) /* chk if path stopped just at node */
       {net_base=path_pt->net_base+path_pt->decl_pt->nodenum;
        net_base+=(yard)(path_pt->decl_pt->nodes*indx);
        flags.build=T; nial_get_ds(net_base,path_pt->decl_pt->sst_pos);
       }
     else {path_error(PER_GEN,path_pt,path_pt,WRN_VAR_FAIL); goto byebye;}
    }
  else
    {rpt=ds_subpt(ds_stub);
     if(rpt && rpt->type==SIP_NODE)
       {path_error(PER_GEN,path_pt,path_pt,WRN_VAR_CNX); goto byebye;}
     if(ds_stub->type==TYPE_ARB || ds_stub->type==TYPE_NODE)
       {if(ds_stub->type==TYPE_NODE && !ds_stub->hassubpt)
          {/* node not defined yet */
           ds_stub->isdef=F; goto byebye;
           signal_error(EXIT_INT_ERR,23);}
        if(ds_stub->type==TYPE_ARB) ds_stub=ds_subpt(ds_stub); /*DEBUGFEB91*/
       }
     rpt=ds_stub;
     ds_stub=sip_pos((stub_elm **)&rpt,ds_stub,indx,(int)TYPE_NODE,(int)0,T);
    }
  if(!ds_stub || !vpt) signal_error(EXIT_INT_ERR,24);
  /* now position vpt */
  /* base vpt pos depends on Vpath partitions */
  big_loop:
  pthlvl=var_pthlvl(path_pt);
  if(pthlvl>1)
    {if(vpt->level!=pthlvl)
       {while(vpt->subpt) {vpt=vpt->subpt; if(vpt->level==pthlvl) break;}
        if(vpt->level!=pthlvl)
          {path_error(PER_GEN,path_pt,path_pt,WRN_VAR_LVL); goto byebye;}
       }
    }
  /* subsequent vpt pos depends on ds & vpt */
  vpt=var_pos(vpt,elmtyp,isstruc,isarray);
  if(!vpt) goto byebye;
  /* check types (if any) of ds_stub & cur var pt */
  if(!var_cast_check(vpt,ds_stub,path_pt,(boolean *)&usecast,
                     (boolean *)&isstruc)) goto byebye;
  /* double check any prior casts in var */
  if(!var_cast_path(vpt,path_pt)) goto byebye;
  /* got a valid var, now set isdef flag */
  set_isdefs(ds_stub); fast_ds_mod(ds_pool);
  /* now initialise var */
  if(isinl) inlevel=(byte)(1+rel_level(flo_pt)); else inlevel=(byte)0;
  if(isstruc) /* initialise a struct or node */
    do_struct(vpt,path_pt,net_base,inlevel);
  else /* initialise a simple atom */
    {recur=F; loop:
     holbits=do_atom(vpt,path_pt,usecast,inlevel);
     if(holbits&HOL_ISL) /* chk for list initialiser */
       {if(holbits&HOL_EOL) /*eol, retry & chk for recursion*/
          {if(!recur) {recur=T; goto loop;}
           else if(big_recur) goto byebye;
           else ++big_recur;
          }
       }
    }
  /* finally get next vpt pos up to pthlvl barrier */
  vee_pt->curpt=var_repeat(vpt,pthlvl,holbits); vee_pt->usedlvl=pthlvl;
  if(big_recur==1) goto big_loop;
  byebye:
  if(net_base) {flags.build=F; nial_put_ds(net_base);}
  ds_pool=tmprot; if(ds_pool) ds_stub=pop_stub(slot1);
  err_line_num=tmpel;
}

/* called frm go_dw_lvl upon ascent when at a partition */
/* update vee_pt's curpt */
void update_var_pt()
{ var_elm *vpt; byte pthlvl,varlvl; int diff;
  varlvl=vee_pt->usedlvl;
  if(!varlvl) return;
  pthlvl=var_pthlvl(path_pt);
  if((varlvl-pthlvl)<1) return;
  vpt=vee_pt->curpt;
  varlvl=vpt->level;
  diff=pthlvl-varlvl;
  if(diff<0) /* path_pt is above curpt, move up curpt */
    {while(vpt->parent) {if(vpt->level==pthlvl) break; vpt=vpt->parent;}
     if(vpt->level!=pthlvl) signal_error(EXIT_INT_ERR,25);
     if(vpt->tail) vpt=vpt->tail;
     else
       {while(vpt && !vpt->subase) vpt=vpt->parent;
        if(!vpt) signal_error(EXIT_INT_ERR,26);
       }
    }
  else if(diff>0) return; /* path_pt is below curpt */
  else return; /* curpt & path_pt at same level */
  vee_pt->curpt=vpt; vee_pt->usedlvl=pthlvl;
}

void loop_vars(rpt,var)
  path_elm *rpt; var_elm *var;
{ int tmpel=err_line_num,tmpef=err_file_num;
  err_file_num=rpt->net->sor_num; vee_pt->tabpt=NULL;
  while(var)
    {mod_indicate();
     vee_pt->curpt=var; vee_pt->usedlvl=0;
     err_line_num=var->line_num;
     path_instr(rpt,var->name,F);
     free_tab(vee_pt->tabpt); vee_pt->tabpt=NULL;
     var=var->tail;}
  err_line_num=tmpel; err_file_num=tmpef;
}

/* Do var decls, similar to do_inp() */
void do_varss(rpt,vpt)
  path_elm *rpt; var_elm *vpt;
{ var_elm *var; path_elm *tmporg=org_pt,*tmpflo=flo_pt;
  vee_elm *tmpvee=vee_pt,curvee; ds_sea_elm *tmprot=ds_pool; int slot1;
  flag_elm tmpflag; tmpflag=flags;
  if(rpt->tail) signal_error(EXIT_INT_ERR,27);
  flags.inout=T; flags.inp=T; flags.state=T; flags.info=F; flags.ary=F;
  flags.pamat=F; flags.last=F; flags.fork=F; flags.wild=F;
  org_pt=rpt; flo_pt=rpt;
  vee_pt=(vee_elm *)&curvee;
  if(ds_pool) slot1=push_stub(ds_stub);
  var=rpt->net->var;
  if(var) loop_vars(rpt,var); /* do main vars */
  if(rpt->parent) /* now see if any arg vars */
    {var=rpt->parent->decl_pt->var;
     if(var) loop_vars(rpt,var);
    }
  if(vpt) loop_vars(rpt,vpt);
  org_pt=tmporg; flo_pt=tmpflo;
  flags=tmpflag; vee_pt=tmpvee;
  ds_pool=tmprot; if(ds_pool) ds_stub=pop_stub(slot1);
}

/* initialise the struct containing conx at spt with vals in vpt
*  called when doing an input path's oelm from put_conx() */
void do_inline_var(spt)
  stub_elm *spt;
{ vee_elm *tmpvee=vee_pt; ds_sea_elm *tmprot=ds_pool;
  path_elm *tmppath=path_pt; int slot1; mile indx; flag_elm tmpflag;
  tmpflag=flags;
  flags.inout=T; flags.inp=T; flags.state=T; flags.info=F; flags.ary=F;
  flags.pamat=F; flags.last=F; flags.fork=F; flags.wild=F;
  if(ds_pool) slot1=push_stub(ds_stub);
  /* setup vars node parent path_pt & spt */
  path_pt=path_pt->parent; path_pt->tail=NULL;
  spt=move_to_node(spt); ds_stub=spt;
  /* setup vee_pt */
  vee_pt=(vee_elm *)&ilk_pt->veed;
  vee_pt->curpt=ilk_pt->curpt->invar; vee_pt->usedlvl=0; vee_pt->lab_id=0xff;
  err_file_num=path_pt->net->sor_num; err_line_num=vee_pt->curpt->line_num;
  /* get index */
  indx=spt->info.data.elm;
  do_var(indx,T);
  flags=tmpflag; vee_pt=tmpvee;
  path_pt->tail=tmppath; path_pt=tmppath;
  ds_pool=tmprot; if(ds_pool) ds_stub=pop_stub(slot1);
}
/*------------------------------------------------------------------------*/
/* Recursively compile a subnet. Loop over all decls in subnet.
* Output goes to the nial routine. refpt is the reference path.
*/
void do_module(refpt,fpt,vpt)
  path_elm *refpt; foot fpt; var_elm *vpt;
{ path_elm *newpt; ds_sea_elm *tmprot; yard net_base=0;
  mile net_size,net_indx; int slot1,slot2,slot3;
  boolean dsnode,isnonterm,tmpbuild;
  mod_indicate();
  if(fpt<=FAKE_MAX) /* check fpt */
    {if(flags.state) /* at a var atom */
       {/*??*/ signal_error(EXIT_INT_ERR,28); return;}
     else signal_error(EXIT_INT_ERR,29);
    }
  claim_net((net_elm **)&refpt->net,fpt);
  if(is_new_net)
    {is_new_net=F;
     refpt->net_base=net_node_max+1; net_node_max+=(yard)refpt->net->ners;}
  dsnode=(boolean)(refpt->net->root_flag);
  if(!flags.fly && !flags.state) push_fly();
  if(!dsnode || flags.state)
    {if(dsnode)
       {tmpbuild=flags.build;  tmprot=ds_pool;
        if(ds_pool) slot1=push_stub(ds_stub);
        flags.build=T; nial_get_ds(refpt->net_base,refpt->net->sst_pos);
        fast_ds_mod(ds_pool);}
     newpt=new_path(refpt); /* set up a path elm incase we go down a level */
     refpt->decl_pt=NULL; refpt->tail=NULL; /* block sublevel for now */
     if(flags.build) {slot2=push_stub(ds_stub); newpt->net_base=0;}
     while(next_decl(refpt))
       {make_path(refpt);
        refpt->partflg=F; refpt->curwldflg=F; refpt->wldflg=F;
        refpt->pat=refpt->decl_pt->name; /* NOTE we make a ref, NOT a COPY */
        if(flags.build) slot3=push_stub(ds_stub);
        isnonterm=(boolean)(check_path(refpt)==PATH_NON);
        while((net_indx=next_indx(refpt))>=0)
          {if(isnonterm) /*recurse and resolve subnet ref*/
             {if(!flags.build)
                {net_base=refpt->decl_pt->nodenum+refpt->net_base;
                 net_size=refpt->decl_pt->nodes;
                 newpt->net_base=net_base+(yard)net_size*net_indx;
                }
              refpt->tail=newpt;
              do_module(newpt,refpt->decl_pt->sst_pos,NULL);
              refpt->tail=NULL;
             }
           else /* reached an atom */
             {if(flags.state) /* chk if special compiler added struct */
                {do_com_atom(refpt);}
              else signal_error(EXIT_INT_ERR,30); /*err should never get here*/
             }
          } /* finished this decl */
        if(flags.build) ds_stub=pop_stub(slot3);
        clear_ary((ary_elm *)&refpt->dary); /* free ary used for indexing */
       }
     if(flags.build) ds_stub=pop_stub(slot2);
     refpt->pat=NULL; /* we took a pt copy */
     free_pelm(newpt);
    }
  if(flags.fly) do_a_fly(refpt);
  else if(flags.state) do_varss(refpt,vpt);
  else
    {do_inpss(refpt);
     if(fly_bot->fly_top) do_flys(refpt,fpt);
     pop_fly(refpt);
    }
  if(dsnode && flags.state)
    {nial_put_ds(refpt->net_base);
     flags.build=tmpbuild;
     ds_pool=tmprot; if(ds_pool) ds_stub=pop_stub(slot1);}
  release_net((net_elm **)&refpt->net);
}

void do_network(fpt,vpt)
  foot *fpt; var_elm *vpt;
{ path_elm *tmproot=rootpt; boolean tmpstate=flags.state;
  rootpt=new_path((path_elm *)NULL); flags.state=F;
  do_module(rootpt,fpt,NULL); /* do In paths */
  ds_flush(F); /* flush this nets nodes & prune at same time ready for state */
  flags.state=T;
  do_module(rootpt,fpt,vpt); /* now do Vars */
  nul_free(rootpt);
  rootpt=tmproot; flags.state=tmpstate;
}

void third_setup()
{ aux_top=NULL; aux_bot=NULL; vee_pt=NULL; heap_list=NULL;
  net_node_max=0; is_new_net=T; vax_flg=F;
}
void thirdpass()
{ foot fil_base;
  third_setup();
  fil_base=nss_init();/* positions file to root net */
  nial_start();
  TOK_AUXOUT=str_to_token(STR_AUXOUT);
  state_init();
  do_network(fil_base,NULL);
  pile_free((path_elm *)NULL,heap_list,F); heap_list=NULL;
  nial_end();
  flush_nets();
  del_token(TOK_AUXOUT);
  token_free(); source_free(); cir_free();
}
