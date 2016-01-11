/* Name: pathhi2.c   Type:  output nodes nsl c routines   Date: 30 March 89
*   NeSeL Neural Net Specification Language Compiler
*/
#include "globals2.h"
#include "iolo2.h"
#include "iohi2.h"
#include "nial2.h"
#include "nsltwo2.h"
#include "nslyacc2.h"
#include "pathlo2.h"
#include "pathhi2.h"
#include "conxlo2.h"
#include "conxhi2.h"

extern void do_var(),update_var_pt();

void set_dary_ary(arypt,ppt)
  ary_elm *arypt; path_elm *ppt;
{ arypt->ary=strsave(STR_SARY); /*DEBUG2/6/93, should this be STR_AND*/
  if(!arypt->isdary || ppt->dec_id) signal_error(EXIT_INT_ERR,132);
  ppt->dec_id=ppt->pat_id;
}

/* check array has a legitimate str to eval, bypass otherwise
*  returns F if ary was bypassed */
boolean check_arystr(arypt,ppt)
  ary_elm *arypt; path_elm *ppt;
{ if(!arypt->ary) set_dary_ary(arypt,ppt);
  if(is_wild(arypt->ary)) /* bypass [*] etc */
    {if(arypt->isdary) /*a term [*], bypass to left, or default to [%] [$]*/
       {if(byleft(ppt,arypt->isdary)) return(F);
        else  /* no non wild found, so default */
          {fremem(arypt->ary);
           if(flags.inp && !flags.fork && !arypt->fixval)
             arypt->ary=strsave(STR_AARY); /*[?]*/
           else
             arypt->ary=strsave(STR_DARY); /*[.]*/
          }
       }
     else return(F); /* bypass pary wilds */
    }
  return(T);
}

/* eval an ary str & get its stats
*  returns LEX_* codes */
int get_ary_info(arypt,ppt,resary)
  ary_elm *arypt; path_elm *ppt; boolean *resary;
{ int excode=LEX_OK; path_elm *tmppt=path_pt; path_pt=ppt;
  if(check_arystr(arypt,ppt))
    {excode=ary_eval(arypt,resary); arypt->pipval=nsl_lst_num(arypt->ary_list);}
  path_pt=tmppt;
  return(excode);
}

/* Check a nodes child parys have been eval'd & thus set fixval correctly.
*  only need to see if immediate arychild has been eval'd
*  eval from bottom up, only reeval if eol
*  paryparent indicates if at tail of a label chain
*  Two sorts: -->[...]-->[...] ==> size is set to current declpt->size
*  and ..<--[mary2]<--[mary1] ==> i.e. LLLARY, size = size previous MARY
*  returns LEX_OK, LEX_ER or LEX_SU
*/
int check_parys(ppt)
  path_elm *ppt;
{ path_elm *labpt=ppt; ary_elm *arypt; int excode;
  boolean resary,paryparent=(boolean)(ppt->arychild==PARY_CHILD);
  if(!ppt->arychild) signal_error(EXIT_INT_ERR,133);
  if(paryparent) /* chck dary */
    {arypt=(ary_elm *)&labpt->dary;
     if(is_wild(arypt->ary))
       {if(!arypt->ary) set_dary_ary(arypt,ppt);
        if(!labpt->laryflg) labpt->pary.fixval=arypt->fixval;
       }
     else
       {if(!arypt->ary_list)
          {if(excode=get_ary_info(arypt,labpt,(boolean *)&resary))
             {if(excode==LEX_ER || excode==LEX_SU) return(excode);
              if(excode==LEX_NO) return(LEX_ER);}
          }
        if(!labpt->laryflg) labpt->pary.fixval=arypt->pipval;
       }
    }
  else
    {labpt=ppt->arychild; arypt=(ary_elm *)&labpt->pary;
     if(!arypt->ary_list && !ppt->laryflg)
       {if(excode=check_parys(labpt)) return(excode);
        if(is_wild(arypt->ary))
          ppt->pary.fixval=arypt->fixval;
        else ppt->pary.fixval=arypt->pipval;
        if(excode=get_ary_info(arypt,labpt,(boolean *)&resary))
          {if(excode==LEX_ER || excode==LEX_SU) return(excode);
           if(excode==LEX_NO) return(LEX_ER);}
       }
    }
  return(LEX_OK);
}

double *var_fetch_elm(arypt)
  ary_elm *arypt;
{ double *elpt=NULL; mile nth=arypt->atval+1,*last_lst=NULL,last_eval=(-1);
  int excode,count; boolean first=T,resary=F;
  loop: count=REVAL_NUM;
  do{if(arypt->ary_list) /* see if cur ary has nth elm */
       {elpt=nsl_lst_nth(arypt->ary_list,nth);
        if(elpt) {excode=LEX_OK; break;} /* found elm */
        if(arypt->suspend) {excode=LEX_ER; break;}
        if(!arypt->eval_list) /* if list totally evald */
          {if(nth>=arypt->pipval) break; /* nth>ary size: eol */
           if(is_const(arypt)) break;} /* it's a constant list */
       }
     excode=ary_eval(arypt,(boolean *)&resary);
     arypt->pipval=nsl_lst_num(arypt->ary_list);
     if(excode==LEX_ER || excode==LEX_SU | excode==LEX_NO || resary) break;
     excode=LEX_ER;
     if(lst_ok(arypt->eval_list)==last_eval
        && lequal(arypt->ary_list,last_lst)) break;
     last_lst=(mile *)lcopy(arypt->ary_list,last_lst);
     last_eval=lst_ok(arypt->eval_list);
    }while(count--);
  lfree(last_lst);
  if(excode==LEX_ER||excode==LEX_NO) return(NULL);
  if(resary) {if(first) {first=F; nth=0; goto loop;} return(NULL);}
  if(excode==LEX_OK) {arypt->atval=nth; arypt->hasval=nth; return(elpt);}
  return(NULL);
}

/* get the value of the nth elm from list (itsval=T) or
*  get the pos of the elm with val nth (itsval=F).
*  Note, when itsval=T, if nth<pipval & list is not a const,
*  it may be reval'd REVAL_NUM times
*  In order to chk if nth in list: is_const could be improved
*  Checks if list needs evaling
*  If pary, also chks childs are eval'd inorder to det size (base MARY)
*  returns, in order of priority:
*  (-LEX_ER) if err,
*  (-LEX_CL) not in list but list was reeval & reset to ary_num=0
*  (-LEX_SU) not in list & list was suspended
*  (-LEX_ZZ) not in list & eol,
*  (-LEX_NE) not in list but !eol
*  eol = eval_list==NULL && ary_list!=NULL
*/
mile eval_nth(ppt,nth,dary,itsval)
  path_elm *ppt; mile nth; boolean dary,itsval;
{ ary_elm *arypt; mile *last_lst=NULL,last_eval=(-1),elm_val;
  int excode=LEX_OK,count=REVAL_NUM; boolean resary=F;
  if(dary) arypt=(ary_elm *)&ppt->dary; else arypt=(ary_elm *)&ppt->pary;
  do{if(arypt->ary_list) /* see if cur ary has nth elm */
       {if(itsval) /* we want the val of nth elm */
          {elm_val=get_nth(arypt->ary_list,nth);
           if(elm_val>=0) goto byeone; /* found elm */
           if(arypt->suspend) goto byetwo;
           if(arypt->eval_list) goto reval; /* can try reval list */
           if(nth>=arypt->pipval) goto byebye; /* nth>ary size: eol */
          }
        else /* we want the pos of elm with val nth */
          {elm_val=nsl_lst_pos(arypt->ary_list,(double)nth);
           if(elm_val>=0) goto byeone;
           if(arypt->suspend) goto byetwo;
           if(arypt->eval_list) goto reval;
          }
        if(is_const(arypt)) goto byebye; /* it's a constant list */
       }
     else /* first time ary eval'd */
       {if(!arypt->isdary) /*chk parychild arys eval'd so that fixval is ok*/
          {if(excode=check_parys(ppt)) goto byebye;}
        if(!check_arystr(arypt,ppt)) /* chk got an ary to eval */
          signal_error(EXIT_INT_ERR,134); /* should have been bypassed!! */
       }
     reval: if(excode=get_ary_info(arypt,ppt,(boolean *)&resary))
       {if(excode==LEX_ER || excode==LEX_SU) goto byebye;
        if(excode==LEX_NO) {elm_val=(-LEX_ZZ); goto byeone;}
       }
     /* crude test for infinite loops... */
     if(resary) goto byebye; /* if we reset arypos ex to caller for new pos*/
     if(lst_ok(arypt->eval_list)==last_eval
        && lequal(arypt->ary_list,last_lst)) break;
     last_lst=(mile *)lcopy(arypt->ary_list,last_lst);
     last_eval=lst_ok(arypt->eval_list);
    }while(count--);
  resary=F;
  byebye:
  lfree(last_lst);
  if(excode==LEX_ER) return(-LEX_ER);
  if(resary) return(-LEX_CL);
  if(excode==LEX_SU) return(-LEX_SU);
  if(arypt->eval_list) return(-LEX_NE);
  return(-LEX_ZZ);
  byetwo: elm_val=(-LEX_SU);
  byeone: lfree(last_lst);
  return(elm_val);
}

/* Using the path_pt list pos, fetch the next elm and update pos
* If a labelled list, then maps elm accordingly.
* Could be a multi level lab!!
* Note a dary can only be the arychild to its pary, (arychild=PARY_CHILD)
* and is never a aryparent. Labparents are always pary's.
* i.e. The basis ary to fetch_elm() is always a dary.
* AND therefore there must be a dary in path_pt when fetch_elm is called
*/
mile fetch_elm()
{ path_elm *labpt=path_pt,*tpt; mile elm_val,pos,oldpos=0;
  boolean tdary,headary=T,paryparent=(boolean)(path_pt->arychild==PARY_CHILD);
  pos=path_pt->dary.atval+1; /* take base dary pos as default */
  /* The tail of a label chain is always a paryparent */
  /* If we are at a label tail then map up accordingly */
  if(flags.inp && flags.inout) til_pt=path_pt;
  if(paryparent)
    {/* we have at least one label parent to map to, see if any more */
     /* Do not go thorough any Lary's, these are dealt with in do_an_ary() */
     while(labpt->aryparent && !labpt->laryflg) labpt=labpt->aryparent;
     /* we're now at top label parent */
     oldpos=labpt->pary.atval+1; /* this = 0 on first time round */
     if(labpt->laryflg) /* the top parent could be a lary */
       {/* parent pary's will have been eval'd, so just look at larypos */
        /* chk pary isn't wild, bypass to right if so */
        tpt=labpt;
        labpt=byright(labpt,(path_elm *)NULL,(boolean *)&tdary);
        if(!labpt)
          {path_error(PER_GEN,path_pt,path_pt,WRN_LRY_BYP);
           tpt->pary.hasval=(-1); tpt->pary.atval=(-1); return(-1);}
        if(labpt!=tpt || tdary)
          path_error(PER_GEN,path_pt,path_pt,WRN_LRY_HTB);
        if(labpt->larypos>=0)
          {/* If it's being reduced get the next root elm value from larypos */
           if(oldpos>0) return(-1); /* exit if we've already done larypos */
           largo:
           oldpos=eval_nth(labpt,labpt->larypos,tdary,F);
           if(oldpos<0)
             {if(!headary || oldpos!=(-LEX_CL)) return(-1);
              headary=F; goto largo; /* if ary num reset try pary once more*/
             }
           headary=F;
           pos=oldpos;
           labpt->pary.atval=labpt->laryat; labpt->pary.hasval=labpt->larypos;
           if(tdary)
             {labpt->dary.atval=labpt->laryat;
              labpt->dary.hasval=labpt->larypos;}
          }
        else
          {/* otherwise get next elm by mapping elms by pos */
           /* e.g. lary[1,3..5,8] -> path[0..4,6] => 1->0,3->1 etc*/
           larmap:
           pos=eval_nth(labpt,oldpos,tdary,T);
           if(pos<0)
             {if(!headary || pos!=(-LEX_CL)) return(-1);
              headary=F; goto larmap;
             }
           headary=F;
           labpt->pary.atval=oldpos; labpt->pary.hasval=pos; /* save oldpos */
           if(tdary) {labpt->dary.atval=oldpos; labpt->dary.hasval=pos;}
          }
        labpt=labpt->arychild;
        /* we have now dealt with lary & can map remaining pary's directly */
       }
     /* now descend pary's mapping next elm as we go */
     /* note labpt=PARY_CHILD if only a paryparent at path_pt level */
     while(labpt && labpt!=PARY_CHILD)
       {if(is_wild(labpt->pary.ary)) /*chk if [*]*/
          labpt->pary.atval=oldpos;
        else
          {tripp:
           pos=eval_nth(labpt,oldpos,F,T);  /* get the elm at index oldpos */
           if(pos<0)
             {if(pos==(-LEX_ER)) return(-1); /* fault error in list */
              if(headary)
                {if(pos!=(-LEX_CL)) return(-1);
                  /*elm didn't map to term, or lst suspended */
                 oldpos=0; headary=F;
                 goto tripp; /* try pary again with new pos */
                }
              else return(-1); /*elm didn't map to terminal dary, or suspend*/
             }
           labpt->pary.atval=oldpos; oldpos=pos; headary=F;
          }
        labpt->pary.hasval=pos; labpt=labpt->arychild;
       }
    }
  /* chk base dary isn't wild [*], bypass if so to last pary */
  if(!check_arystr((ary_elm *)&path_pt->dary,path_pt))
    {elm_val=pos; pos=oldpos;}
  else /* finally, use computed pos to get next elm value from dary */
     {tridd:
      elm_val=eval_nth(path_pt,pos,T,T);
      if(elm_val<0)
        {if(elm_val==(-LEX_ER)) return(-1); /* fault error in list */
         if(headary)
           {if(elm_val!=(-LEX_CL)) return(-1); /*non term or suspended */
            pos=0; headary=F;
            goto tridd; /* try dary again with new pos */
           }
         else return(-1); /*err: elm didn't map to terminal dary*/
        }
     }
  if(elm_val>=0) /* if valid value, update position */
    {path_pt->dary.atval=pos; path_pt->dary.hasval=elm_val;}
  return(elm_val);
}

/* get the next elm, chk for escape & rebuild */
mile get_nxt_elm(rebpt)
  boolean *rebpt;
{ mile elm,size,count;
  if(flags.escape) return(-1);
  if(path_pt->rebflg)
    {path_pt->rebflg=F;
     if(path_pt->dary.atval>=0) {*rebpt=T; return(path_pt->dary.hasval);}
    }
  *rebpt=F;
  for(count=10;count>0;--count)
    {elm=fetch_elm();
     size=path_pt->decl_pt->size;
     if(elm<size || size==0) break;
     path_error(PER_GEN,path_pt,path_pt,WRN_ELM_OOR,elm,(size-1));
    }
  if(count==0) path_error(PER_GEN,path_pt,path_pt,WRN_ELM_ABR);
  return(elm);
}

/* search comms to see if id is permitted */
boolean chk_comm(rpt,id,delacode)
  path_elm *rpt; char *id; byte delacode;
{ comm_elm *cpt; char *apt,ccode; boolean idok;
  if(rpt->parent&&rpt->parent->decl_pt)
    if(rpt->parent->decl_pt->sst_pos==VIR_NET) return(T);/* chk aux labels */
  if(flags.state || flags.pamat || flags.info || flags.ary) return(T);
  cpt=rpt->net->comm;
  if(cpt) /* a node with comms */
    {ccode=COMM_NONE;
     while(cpt)
      {if(match(cpt->name+1,id)) ccode|=(*cpt->name);
       cpt=cpt->tail;
      }
    }
  else  /* either a net or struct, or a node with no comms */
    ccode=delacode;
  idok=((boolean)( (flags.inp && (ccode&0x1))
                    || (!flags.inp && (ccode&0x2)) ));
  if(!idok) /* now allow for floated & local labels etc*/
    {if(!org_pt || ((org_pt->net_base==rpt->net_base)
         && (org_pt->net==rpt->net))) return(T);
     if(!flags.wild)
       {if(ilk_pt && ilk_pt->glob) return(T);
        if(flags.inp) apt="in"; else apt="out";
          path_error(PER_GEN,rpt,rpt,WRN_NO_COMMS,apt,id);
       }
    }
  return(idok);
}

/* Fetch nxt decl whch mtchs p_id, Returns T if match */
boolean get_nxt_decl(ppt)
  path_elm *ppt;
{ char chr,*apt=ppt->pat; mile decl_num;
  decl_elm *dpt,*decl_pt=ppt->decl_pt;
  if(!apt) {tack_pat(ppt); apt=ppt->pat;}
  if(!*apt) signal_error(EXIT_INT_ERR,135);
  if(flags.escape) return(F);
  if(ppt->rebflg)
    {if(ppt->lab_pt || !decl_pt) return(F);
     if(flags.build)
       {if(!ds_stub->hassubpt) return(T); /*DEBUG Nov 92 */
        ds_stub=ds_subpt(ds_stub); dpt=ppt->net->decls;
        if(!decl_pt->declnum) {ds_stub=NULL; return(T);}
        while(dpt!=decl_pt) {dpt=dpt->tail; ds_stub=sip_inc(ds_stub);}
       }
     return(T);
    }
  if(!decl_pt) /* first time round */
    {decl_pt=ppt->net->decls;
     if(flags.build && decl_pt && decl_pt->declnum)
       {if(!ds_stub->hassubpt)
          {if(!flags.state) ds_stub->type=TYPE_NODE;
           ds_stub=sip_decl(ds_stub,decl_pt,F);
          }
        ds_stub=ds_subpt(ds_stub);
       }
     if(*apt=='#') /* if pat == "#Dxx" then fnd Dxx only */
       {apt+=2; decl_num=0;
        while(isdigit(chr= *apt))
          {decl_num*=10; decl_num+=(chr&0xf); ++apt;}
        while(decl_pt)
          {if(dechd(ppt,decl_pt)==decl_num) break;
           decl_pt=decl_pt->tail;
           if(flags.build && decl_pt && decl_pt->declnum)
             ds_stub=sip_inc(ds_stub);
          }
        goto byebye;
       }
    }
  else
    {/* restart: Sometimes only a goto will do.. :( */
     if(*apt=='#') {decl_pt=NULL; goto byebye;}
     decl_pt=decl_pt->tail;
     if(flags.build && decl_pt && decl_pt->declnum) ds_stub=sip_inc(ds_stub);
    }
  while(decl_pt)
    {if(decl_pt->declnum<0) {decl_pt=NULL; break;}
     if(match(apt,decl_pt->name))
       {if(chk_comm(ppt,decl_pt->name,decl_pt->comcode)) break;}
     decl_pt=decl_pt->tail;
     if(flags.build && decl_pt && decl_pt->declnum)
       {ds_stub=ds_tail(ds_stub); if(!ds_stub) decl_pt=NULL;}
    }
  byebye: ppt->decl_pt=decl_pt;
  return((boolean)(decl_pt!=NULL));
}

/* Fetch nxt lab whch mtchs p_id, Returns T if match
*  Note cannot be at a node or struct, so no ds_stub adjustment necessary */
boolean get_nxt_lab()
{ lab_elm *lab_pt=path_pt->lab_pt;
  if(flags.escape) return(F);
  if(path_pt->rebflg) return((boolean)(lab_pt!=NULL));
  if(lab_pt) lab_pt=lab_pt->tail; else lab_pt=path_pt->net->lab;
  while(lab_pt)
    {if(match(path_pt->pat,lab_pt->name))
       {if(chk_comm(path_pt,lab_pt->name,lab_pt->comcode)) break;}
     lab_pt=lab_pt->tail; }
  if(lab_pt) path_pt->decl_pt=lab_pt->decl_pt;
  else path_pt->decl_pt=NULL;
  path_pt->lab_pt=lab_pt;
  return((boolean)(lab_pt!=NULL));
}

/* Returns  0 if path error or invalid
*  PATH_NON if path valid nonterminal
*  PATH_TER if path valid terminal
* A terminal path elm will have a type (net->tag)
* but not be of type TYPE_NODE or have a tail
*/
int check_path(ppt)
  path_elm *ppt;
{ if(ppt->net)
    {if(!ppt->decl_pt) return(PATH_NON); /* not eval'd fully yet */
     if(!ppt->decl_pt->declnum) /* trap virtual nets */
       {if(ppt->tail) goto byebye;
        return(PATH_TER);
       }
     if(ppt->net->tag) /* struct etc have a tag */
       {if(ppt->tail) /* non-terminal expected */
          {if((ppt->decl_pt->decltype==TYPE_NODE)
              || (flags.state && ppt->decl_pt->decltype==TYPE_STRUCT))
             return(PATH_NON); /* non-terminal */
           /* an error, so fall through */
          }
        else /* no tail, so terminal expected */
          {if((ppt->decl_pt->decltype!=TYPE_NODE)
              && !(flags.state && ppt->decl_pt->decltype==TYPE_STRUCT))
             return(PATH_TER); /* terminal */
           else /* can have a non-term without a tail */
             return(PATH_NON); /* non-terminal path */
          }
       }
     else return(PATH_NON); /* non-terminal module */
    }
  else return(PATH_NON); /* net not rd yet, so treat as a non term */
  byebye: /* a path error, path terminated prematurely */
  path_error(PER_GEN,ppt,NULL,ERR_TER_OPTH);
  return(PATH_ERR); /* skip to next decl */
}

/* returns T if tail of path is at a terminal node, or path err */
boolean term_path(ppt)
  path_elm *ppt;
{ while(ppt->tail) ppt=ppt->tail;
  return((boolean)(check_path(ppt)!=PATH_NON));
}

/* See if '**' selector is next path elm.
*  chop it out & signal if so.
*/
path_elm *check_sstar()
{ path_elm *wpt,*chdpt,*babpt; char *tmp;
  if(!path_pt) return(NULL);
  tmp=path_pt->pat;
  if(strcmp(tmp,"**")!=0) return(NULL);
  wpt=path_pt; chdpt=path_pt->tail;
  if(!chdpt)
    {chdpt=tack_lvl(path_pt); /*make sure "**" followed by something*/
     if(!chdpt) signal_error(EXIT_INT_ERR,136);}
  babpt=chdpt->tail; path_pt->tail=babpt;
  path_pt->pat=chdpt->pat; chdpt->pat=tmp; path_pt->pary.ary=chdpt->pary.ary;
  if(babpt) babpt->parent=path_pt;
  return(chdpt);
}

/* Insert a star && sstar operator into path: '*\**\'.
* On entry wpt is definitely not null! */
void insert_sstar(wpt)
  path_elm *wpt;
{ path_elm *chdpt,*newpt; char *tmp;
  /* reinsert ** into path */
  chdpt=path_pt->tail; path_pt->tail=wpt;
  tmp=wpt->pat; wpt->pat=path_pt->pat; path_pt->pat=tmp;
  path_pt->pary.ary=NULL;
  if(chdpt) chdpt->parent=path_pt;
  chdpt=wpt;
  /*now add a * node after it. Then swap pats */
  newpt=new_path(path_pt); newpt->wldflg=T; path_pt->wldflg=T;
  newpt->pat=path_pt->pat; path_pt->pat=strsave("*");
  newpt->tail=chdpt;
  chdpt->parent=newpt;
  set_wild_flag(path_pt,T);
}

/* remove '*' if added by '**' selector */
void rem_star(wpt)
  path_elm *wpt;
{ path_pt=wpt; wpt=wpt->tail;
  fremem(path_pt->pat); path_pt->pat=wpt->pat; wpt->pat=NULL;
  rem_path(wpt);
}

/* get the file pos of additional label declaration */
foot get_add_fpt(ppt) path_elm **ppt;
{ path_elm *p=(*ppt); char *apt;
  apt=p->lab_pt->val;
  if(*apt=='@') {while(*apt=='@') {p=p->parent; ++apt;}}
  else if(*apt=='/') {while(p->parent) p=p->parent;}
  *ppt=p;
  if(!p->net) signal_error(EXIT_INT_ERR,157);
  return(p->net->sst_pos);
}

void do_nxt_decl();

/* Go down a level, or call conx_to_node if isterm
*  Upon each call path_pt == current lvl
*           path_pt->tail == level to move to...
*/
void go_dw_lvl(isterm)
  boolean isterm;
{ path_elm *tmppt=path_pt,*subpt,*refpt; stub_elm *rpt,*spt; foot fpt;
  yard net_base; mile net_size,net_indx; int curlvl,slot1,slot2;
  byte lab_id=0; boolean firstime=T,rebble,party,daryadd=F;

  if(isterm) subpt=path_pt;
  else
    {subpt=path_pt->tail;
     if(!subpt)
       {if(flags.state) {subpt=path_pt; isterm=T; goto do_term;}
        else subpt=tack_lvl(path_pt); /*tac a default lvl if none found*/
        if(!subpt) signal_error(EXIT_INT_ERR,137);
        lab_id=subpt->pat_id;
	   }
     refpt=path_pt;
     if(subpt->rebflg) lab_id=subpt->tac_id;
     else
       {subpt->decl_pt=NULL; subpt->lab_pt=NULL; /* DEBUG added to be sure */
        fpt=path_pt->decl_pt->sst_pos;
        if(fpt==VIR_NET) fpt=get_add_fpt(&refpt);/* handle additional label */
        claim_net((net_elm **)&subpt->net,fpt);
       }
     if(flags.build)
       {if(ds_stub->type!=TYPE_NODE && ds_stub->type!=TYPE_ARB
           && !(flags.state && ds_stub->type==TYPE_STRUCT)) goto skippy;
        slot1=push_stub(ds_stub);
        subpt->net_base=0;}
     else
       {net_size=path_pt->decl_pt->nodes;
        net_base=refpt->net_base+path_pt->decl_pt->nodenum;}
    } do_term:
  if(flags.pamat)
    {if(part_leafs<0) /* handle sel path eval */
       {if(part_leafs==SOV_END) goto hoppy;
        party=F;
        if(part_leafs==SOV_ABS)
          {curlvl=abs_hd(path_pt);
           if(curlvl>part_level) goto parend;
          }
        else if(part_leafs==SOV_LAB)
          {curlvl=lab_mx_pos(path_pt,ilk_pt->orefpt->pat_id);
           if(curlvl>part_level)
             {parend: flags.escape=T; part_leafs=SOV_END;
              set_rebuild(path_pt); goto hoppy;}
          }
       }
     else /* a normal part seek */
       {curlvl=cur_part(path_pt);
        if(!flags.last && curlvl>part_level)
          {++part_leafs; part_exceed=T; goto hoppy;} /*DEBUG 20/5/93*/
        party=(boolean)(flags.last && curlvl>=part_level);
        part_exceed|=party;
       }
    }
  set_wild_flag(path_pt->tail,path_pt->curwldflg);
  if(path_pt->decl_pt->sst_pos==VIR_NET) path_pt->dary.fixval=1;
  else path_pt->dary.fixval=path_pt->decl_pt->size;
  if(ds_pool) slot2=push_stub(ds_stub); /* rpt will be saved here */
  daryadd=(boolean)(!path_pt->dary.ary);
  while((net_indx=get_nxt_elm(&rebble))>=0)  /* loop over dary's */
    {if(isterm) /* we have a terminal path elm */
       {if(flags.state) do_var(net_indx,F);
        else if(flags.pamat)
          {if(part_leafs<0)
             {part_leafs=SOV_END; flags.escape=T;
              set_rebuild(subpt);
              if(!rebble)
                {conx_to_node(net_indx);
                 if(ilk_pt->node)
                   {++ilk_pt->cxnum; ++ilk_pt->abnum; ilk_pt->endofcx=F;}
                }
              break;
             }
           if(party && !rebble) ++part_leafs;}
        else if(!rebble) conx_to_node(net_indx);
       }
     else /* non-terminal */
       {path_pt=subpt;
        if(flags.build)
          {rpt=restore_stub(slot2);
           if(firstime) /* first time through */
             {spt=ds_subpt(ds_stub);
              if(spt && spt->type==SIP_NODE) signal_error(EXIT_INT_ERR,138);
              firstime=F;
              if(ds_stub->type==TYPE_ARB || ds_stub->type==TYPE_NODE)
                {if(ds_stub->type==TYPE_NODE)
                   {if(subpt->parent->decl_pt->size!=1)
                      {ds_stub=sip_decl(ds_stub,subpt->net->decls,T);
                       rpt=ds_prior(ds_stub);}
                    else
                      {ds_stub=sip_decl(ds_stub,subpt->net->decls,F);
                       rpt=ds_stub;}
                   }
                 else {rpt=ds_stub; ds_stub=spt;}
                }
              else rpt=ds_stub;
             }
           ds_stub=sip_pos((stub_elm **)&rpt,ds_stub,
                           net_indx,(int)TYPE_NODE,(int)0,T);
           update_stub(rpt,slot2);
          }
        else if(!subpt->rebflg)
          path_pt->net_base=net_base+(yard)net_size*net_indx;
        do_nxt_decl();
        if(!isterm && !path_pt->rebflg) /*need to tidy for nxt loop/user*/
          {tidy_allarys(path_pt); path_pt->dec_id=LAB_NULL;
           nul_free(path_pt->dary.ary);}
        if(flags.build && !flags.state)/*prune ds if no conxs added or del'd*/
          {rpt=restore_stub(slot2);
           if(ds_value(rpt)->conx.in_num==0 && ds_value(rpt)->conx.out_num==0)
             {/* no conxs on this branch so del its subpt*/
              rem_sip(ds_subpt(rpt));
              rpt->info.data.elm=0;
              rpt->hassubpt=F; rpt->type=TYPE_NODE; ds_stub=rpt; firstime=T;
             }
          }
        path_pt=tmppt;
       }
    }
  if(ds_pool) rpt=pop_stub(slot2);
  hoppy:
  if(daryadd && !subpt->rebflg) /* clear added ary */
    {tidy_allarys(path_pt); path_pt->dec_id=LAB_NULL;
     nul_free(path_pt->dary.ary);}
  if(!isterm) /* not a terminal node so tidy subpt */
    {if(flags.build) ds_stub=pop_stub(slot1);
     skippy:
     if(!subpt->rebflg)
       {release_net((net_elm **)&subpt->net);
	    if(lab_id) 
		{	yank_lab(path_pt,lab_id); /*DEBUG: this could del subpt elm, so */
			if(path_pt->tail!=subpt) return; /* do not set tac_id */
		}
        else tidy_pelm(subpt); /* should clr eval_lists & parys */
       }
    }
  if(subpt->rebflg) subpt->tac_id=lab_id; else subpt->tac_id=LAB_NULL;
}

/* Given a particular decl, find (recursively) the nodes associated
*  with it. The flags indicate if a label needs installing &
*  if it should be yanked before returning.
*  path_pt->pat & ->pary.ary have been set by caller
*  ->dary will be set by install_lab if necessary
*  Return value: lab_id>0 if a label was installed & needs yanking, 0 otherwise
*/
byte do_a_decl(insflg,yankflg)
  boolean insflg,yankflg;
{ path_elm *ppt; int chk=0; byte lab_id=0;
  boolean tmpwild=flags.wild;
  if(ppt=path_pt->parent) chk=ppt->curwldflg;
  path_pt->curwldflg=(boolean)(path_pt->wldflg || chk);
  flags.wild|=path_pt->curwldflg;
  if(chk=check_path(path_pt)) /* see if valid path */
    {if(path_pt->rebflg) lab_id=path_pt->reb_id;
     else
       {if(flags.info && !path_pt->tail) /* return selector val */
          {if(path_pt->decl_pt->sst_pos==VIR_NET) path_pt->dary.fixval=1;
           else path_pt->dary.fixval=path_pt->decl_pt->size;
           path_pt->arychild=PARY_CHILD;
           if(!path_pt->pary.ary) signal_error(EXIT_INT_ERR,139);
           eval_var(path_pt->pary.ary+1); /* result in exp_value */
           flags.escape=T;
           goto byebye;
          }
        if(insflg) lab_id=install_lab(yankflg);
       }
     path_pt->laryflg=(boolean)(insflg && !yankflg);
     /* If reducing struct refs, or pary was a direct decl match */
     /* then force arychild to a PARY */
     if(!insflg && !path_pt->arychild
        && (path_pt->net->tag || !path_pt->dec_id) )
       path_pt->arychild=PARY_CHILD;
     go_dw_lvl((boolean)((chk&PATH_TER)!=0));
     if(path_pt->rebflg) path_pt->reb_id=lab_id;
     else if(yankflg && lab_id) {yank_lab(path_pt,lab_id); lab_id=0;}
    }
  byebye:
  flags.wild=tmpwild;
  return(lab_id);
}

/* save ary info for later, Note arypt->ary has been set to null */
sak_elm *push_ary(stail,arypt)
  sak_elm *stail; ary_elm *arypt;
{ sak_elm *spt; boolean tmpdary=arypt->isdary;
  spt=malmem(sak_elm,1); spt->ary=(*arypt); spt->tail=NULL;
  zap_ary(arypt);
  if(stail) stail->tail=spt;
  arypt->isdary=tmpdary;
  return(spt);
}

void free_sak(spt)
  sak_elm *spt;
{ sak_elm *lpt;
  while(spt)
    {lpt=spt; spt=spt->tail; clear_ary((ary_elm *)&lpt->ary); fremem(lpt);}
}

/* This is quite a tricky routine. It handles LARY's.
* On entry: "label_name[labexp]"
*  lary_id1[laryexp1] = !Dx[..]\path1
*  lary_idn[laryexpn] = !Dy[..]\pathn
* path_pt->pat = label_name, ->pary = labexp
* path_pt->lab_pt = start of larys
* Note larys cannot occur in ds's, so dsroot & ds_stub should be null
*/
void do_an_ary()
{ boolean rebble,resary; byte lab_id,dec_id; int tmpras; mile lelm=0,size=0;
  flag_elm tmpflg; ary_elm store_pary,store_dary;
  path_elm *tmplcpt; sak_elm *stktop=NULL,*stail=NULL;
  lab_elm *bs_pt,*new_lab,*lab_pt=path_pt->lab_pt; lary_elm *lpt;
  char *labpat,*labexp=path_pt->pary.ary; /* This could be null */
  if(path_pt->rebflg)
    {tmplcpt=path_pt->lary->tmplcpt; new_lab=path_pt->lary->new_lab;
     store_dary=path_pt->lary->sdary; store_pary=path_pt->lary->spary;
     dec_id=path_pt->lary->dec_id; stail=path_pt->lary->stail;
     stktop=path_pt->lary->stktop; bs_pt=path_pt->lary->bs_pt;
     labpat=bs_pt->name; size=store_dary.fixval;
     lab_pt=path_pt->lary->lab_pt; nul_free(path_pt->lary);
     goto rebuild;
    }
  tmplcpt=path_pt->arychild; tmpflg=flags;
  labpat=lab_pt->name;     /* get name of lary to work on */
  bs_pt=lab_pt;            /* save a base pointer to the first lary */
  path_pt->laryat=(-1); path_pt->larypos=(-1); /* reset lary vars */
  if(flags.build || ds_stub || path_pt->dary.ary) signal_error(EXIT_INT_ERR,140);
  flags.ary=T; /* this forces escape mode when conx_to_node is hit */
  flags.fork=F; flags.state=F; flags.info=F;
  flags.build=F; flags.inout=F; flags.pamat=F; flags.last=F;
  /* loop over larys and find maximum list element value */
  /* Note aryparent should be disabled via larypos<0 & laryflg=T */
  /* so do_a_decl doesn't map lary ary from a parent ary */
  new_lab=lab_pt; tmpras=raise_error;
  while(lab_pt && strcmp(labpat,lab_pt->name)==0)
    {path_pt->pary.ary=strsave(lab_pt->ary);
     path_pt->decl_pt=lab_pt->decl_pt; path_pt->lab_pt=lab_pt;
     /* path_pt->laryflg is set in do_a_decl */
     raise_error=ERR_OK;
     if(is_const(path_pt->pary.ary)) /* eval const arys right away */
       {get_ary_info((ary_elm *)&path_pt->pary,path_pt,(boolean *)&resary);
        lab_id=0;}
     else /* have to eval path to eval context dependent arys...*/
       {lab_id=do_a_decl(T,F); flags.escape=F;}
     if(!raise_error)
       {lelm=(mile)nsl_lst_hilo(path_pt->pary.ary_list,T);
        if(lelm>size) size=lelm;}
     nul_free(path_pt->pary.ary); /* just in case it wasn't freed */
     /* stack evaled lab's ary's for later */
     stail=push_ary(stail,(ary_elm *)&path_pt->pary); /* zaps pary as well */
     if(!stktop) stktop=stail;
     clear_ary((ary_elm *)&path_pt->dary); /* should clr dary completely */
     if(lab_id) yank_lab(path_pt,lab_id);
     new_lab=lab_pt; /* upon exit this is the next but one label to test */
     lab_pt=lab_pt->tail;
    }
  ++size; flags=tmpflg; raise_error=tmpras;
  /* reload label ary and evaluate it using calc'd size */
  path_pt->arychild=PARY_CHILD; path_pt->pary.ary=labexp;
  path_pt->dary.ary=strsave(STR_AND); /* force dary to [&] */
  path_pt->laryat=(-1); path_pt->larypos=(-1); /* reset lary vars */
  path_pt->laryflg=F; /* enables lab mapping */
  set_wild_flag(path_pt->tail,path_pt->curwldflg);
  path_pt->dary.fixval=size; /* the highest elm in LARY's */
  /* This outer while loop fetches next elm from parent pary's to lary */
  /* NOTE get_nxt_elm goes over all elms of LARY, a LARY member will be */
  /* descended even if it doesn't contain cur larypos. A more efficient */
  /* way is to check if larypos is in lary member first: see stktop */
  /* now do lary elms one by one */
  while((path_pt->larypos=get_nxt_elm(&rebble))>=0)
    {path_pt->laryat=path_pt->dary.atval; dec_id=path_pt->dec_id;
     store_pary=path_pt->pary; zap_ary((ary_elm *)&path_pt->pary);
     store_dary=path_pt->dary; zap_ary((ary_elm *)&path_pt->dary);
     path_pt->dary.isdary=T; path_pt->dec_id=LAB_NULL;
     stail=stktop; lab_pt=bs_pt; /* loop over larys with same name */
     while(lab_pt && strcmp(labpat,lab_pt->name)==0)
       {/* set up pary as though this lary was a regular label */
        /* larypos will cause do_a_decl to only eval 1 elm */
        /* now see if val is in ary */
        if(nsl_lst_pos(stail->ary.ary_list,(double)path_pt->larypos)>=0)
          {path_pt->decl_pt=lab_pt->decl_pt; path_pt->lab_pt=lab_pt;
           path_pt->pary.ary=strsave(lab_pt->ary);
           path_pt->pary.fixval=size;
           rebuild: /* on a rebuild jump in here */
           lab_id=do_a_decl(T,F);
           if(path_pt->rebflg)  /* save state for future rebuild */
             {if(path_pt->lary) signal_error(EXIT_INT_ERR,141);
              lpt=malmem(lary_elm,1); path_pt->lary=lpt;
              lpt->tmplcpt=tmplcpt; lpt->new_lab=new_lab;
              lpt->bs_pt=bs_pt; lpt->lab_pt=lab_pt;
              lpt->sdary=store_dary; lpt->spary=store_pary;
              lpt->stail=stail; lpt->stktop=stktop; lpt->dec_id=dec_id;
              return;
             }
           clear_ary((ary_elm *)&path_pt->pary);
           clear_ary((ary_elm *)&path_pt->dary);
           if(lab_id) yank_lab(path_pt,lab_id);
           if(flags.escape) break;
          }
        lab_pt=lab_pt->tail; stail=stail->tail;
       }
     path_pt->dary=store_dary; path_pt->pary=store_pary;
     path_pt->arychild=PARY_CHILD; path_pt->laryflg=F; path_pt->dec_id=dec_id;
     path_pt->laryat=(-1); path_pt->larypos=(-1); /* reset lary vars */
    }
  nul_free(path_pt->dary.ary);
  free_sak(stktop);
  path_pt->lab_pt=new_lab; path_pt->arychild=tmplcpt;
}

/* Fetch and move to the next item in path
*  On entry: path_pt->pat = pattern at this level to match on
*/
void do_nxt_decl()
{ path_elm *wldpth=NULL; ds_sea_elm *tmprot; int slot1,slot2,curlvl;
  boolean tmpbuild,dsnode,matches=F,tmpwild=flags.wild,rebwldflg=F;
  if(!path_pt) signal_error(EXIT_INT_ERR,142);
  dsnode=(boolean)(path_pt->net->root_flag&&(bookeep||flags.inout));
  if(dsnode)
    {tmpbuild=flags.build;
     tmprot=ds_pool; if(ds_pool) slot1=push_stub(ds_stub);
     flags.build=T; nial_get_ds(path_pt->net_base,path_pt->net->sst_pos);}
  do{/* The '**' op is handled by firstly treating path as though it */
     /* wasn't there (chop it out), and then by inserting '*\**' */
     if(path_pt->rebflg)
       {wldpth=path_pt->wldpth; rebwldflg=path_pt->rebwldflg;}
     else
       {if(wldpth) {rebwldflg=T; insert_sstar(wldpth);} /* if '**' mode */
        wldpth=check_sstar(); /* see if pat is '**' */
       }
     flags.wild=(boolean)(tmpwild || path_pt->curwldflg);
     if(!path_pt->rebflg)
       {path_pt->decl_pt=NULL; path_pt->lab_pt=NULL;} /* DEBUG to be sure */
     if(flags.build) slot2=push_stub(ds_stub);
     while(get_nxt_decl(path_pt)) /* scan decls '#Dx...name' for a match */
       {matches=T;
        do_a_decl(F,F);
       } /* end of decls, try label(ary) match */
     if(flags.build) ds_stub=pop_stub(slot2);
     else if(*path_pt->pat>')') /* chk pat isn't a decl ref '#Dx' */
       {while(get_nxt_lab())
          {matches=T;
           if(path_pt->lab_pt->ary)
             do_an_ary(); /* try array_label */
           else /* normal label match, or sary */
             do_a_decl(T,T);
          } /* no more matches at this lvl */
       }
     if(flags.escape)
       {if(path_pt->rebflg)
          {path_pt->wldpth=wldpth; path_pt->rebwldflg=rebwldflg; rebwldflg=F;}
        matches=T;
        break;
       }
     if(wldpth) /* DEBUG 25/6/92 */
       {tidy_allarys(path_pt); path_pt->dec_id=LAB_NULL;
        nul_free(path_pt->dary.ary);}
    }while(wldpth);
  if(!(matches || flags.info || flags.ary)) /* no matching path found */
    path_error(PER_GEN,path_pt,path_pt,ERR_NON_OPTH);
  if(rebwldflg) rem_star(path_pt);
  if(dsnode)
    {nial_put_ds(path_pt->net_base);
     flags.build=tmpbuild; ds_pool=tmprot;
     if(ds_pool) ds_stub=pop_stub(slot1);}
  if(flags.state && path_pt->partflg
     && path_pt->pat_id==vee_pt->lab_id) update_var_pt();
  if(!flags.escape && flags.pamat)
    {if(part_leafs<0) /* check sel eval mode */
       {if(part_leafs==SOV_ABS)
          {curlvl=abs_hd(path_pt);
           if(curlvl>=part_level) goto parend;
          }
        else if(part_leafs==SOV_LAB)
          {curlvl=lab_mx_pos(path_pt,ilk_pt->orefpt->pat_id);
           if(curlvl>=part_level)
             {parend: part_leafs=SOV_END; 
              flags.escape=T; set_rebuild(path_pt);}
          }
        /* else if(part_leafs==SOV_END||SOV_ALL) */
       }
     else if(path_pt->partflg)
       {if(part_leafs>0 && (part_level==cur_part(path_pt)))
          {flags.escape=T; set_rebuild(path_pt->parent);}
       }
    }
  flags.wild=tmpwild;
}
