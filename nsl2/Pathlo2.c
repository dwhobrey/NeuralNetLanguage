/* Name: pathlo2.c   Type:  low level path routines   Date: 30 March 89
*  NeSeL Neural Net Specification Language Compiler
*/
#include "globals2.h"
#include "iolo2.h"
#include "iohi2.h"
#include "nial2.h"
#include "nsltwo2.h"
#include "pathlo2.h"
#include "pathhi2.h"
#include "conxlo2.h"
#include "conxhi2.h"
#include "nslyacc2.h"

char path_str_buff[MAX_PATH_STR];

/* returns a declarations handle (i.e. index number)
*  handle of virtual nets (pile, heap, bin) is:
*     (max index of normal decl + virtual net number (in dpt->sst_pos) )
*/
mile decl_hand(ppt,dpt)
  path_elm *ppt; decl_elm *dpt;
{ if(dpt->declnum) return(dpt->declnum);
  return((mile)(ppt->net->dec_num+(mile)dpt->sst_pos));
}

int rel_level(ppt)
  path_elm *ppt;
{ int lvl=0;
  if(ppt) while(ppt->parent) {++lvl; ppt=ppt->parent;}
  return(lvl);
}

path_elm *rel_path(ppt,lvl)
  path_elm *ppt; int lvl;
{ while(lvl && ppt) {ppt=ppt->tail; --lvl;}
  return(ppt);
}

/* return level path has been eval'd to, zero if not eval'd */
int eval_level(ppt)
  path_elm *ppt;
{ int lvl=0;
  while(ppt) {if(ppt->dary.atval<0) break; ++lvl; ppt=ppt->tail;}
  return(lvl);
}

int lab_mx_pos(ppt,id)
  path_elm *ppt; byte id;
{ int pos=0;
  while(ppt) {if(ppt->pat_id==id||ppt->dec_id==id) ++pos; ppt=ppt->parent;}
  return(pos);
}

/* Set the wild flag in remainder of path accordingly
*/
void set_wild_flag(ppt,val)
  path_elm *ppt; boolean val;
{  while(ppt)
     {ppt->curwldflg=(boolean)(val || ppt->wldflg);
      if(ppt->curwldflg) val=T; /* QST is this correct? */
      ppt=ppt->tail;}
}

void set_rebuild(ppt)
  path_elm *ppt;
{ while(ppt) {ppt->rebflg=T; ppt=ppt->parent;}
}

/* reset selector dirty bits in ary str */
void reset_sels(apt)
  char *apt;
{ if(apt) while(*apt)
    {switch(*apt)
       {case '$': ++apt; if(*apt=='^') ++apt;
          if(*apt>'@') (*apt)&=0xdf; ++apt; break;
        case '\'': case '"':
          push_io(); set_io_str(apt); skip_quoted();
          apt=get_io_pos(); pop_io(); break;
        default: ++apt;
       }
    }
}
/* reset all sel dirty bits in path */
void sels_path(ppt) path_elm *ppt;
{ while(ppt)
    {reset_sels(ppt->pary.ary); reset_sels(ppt->dary.ary);
     ppt->pary.suspend=F; ppt->dary.suspend=F;
     ppt=ppt->tail;}
}

void tidy_eval(arypt) ary_elm *arypt;
{ lfree(arypt->eval_list); arypt->eval_list=NULL;
}

/* clear an ary */
void free_ary(arypt)
  ary_elm *arypt;
{ lfree(arypt->eval_list); chk_free(arypt->ary); lfree(arypt->ary_list);
}

void zap_ary(apt)
  ary_elm *apt;
{ apt->ary=NULL; apt->ary_list=NULL; apt->eval_list=NULL;
  apt->atval=(-1); apt->hasval=(-1); apt->fixval=0; apt->pipval=0;
  apt->isdary=F; apt->suspend=F;
}

/* free up list table */
void free_tab(tpt)
  tab_elm *tpt;
{ if(tpt)
    {free_ary((ary_elm *)&tpt->pippa);
     if(tpt->tail) free_tab(tpt->tail);
     fremem(tpt);
    }
}

tab_elm *new_tab()
{ tab_elm *apt=malmem(tab_elm,1);
  zap_ary(&apt->pippa); apt->tail=NULL;
  return(apt);
}

/* clr ary ready for next user but don't touch arypt->ary */
void tidy_ary(arypt)
  ary_elm *arypt;
{ tidy_eval(arypt);
  arypt->atval=(-1); arypt->hasval=(-1); arypt->suspend=F;
  arypt->fixval=0; arypt->pipval=0;
  lfree(arypt->ary_list); arypt->ary_list=NULL;
}

/* clear all ary for next user */
void clear_ary(arypt)
  ary_elm *arypt;
{ tidy_ary(arypt); nul_free(arypt->ary);
}

/* tidy dary ary, pary & it's parents if any ready for next user*/
void tidy_allarys(opt)
  path_elm *opt;
{ path_elm *mypt=opt;
	boolean paryparent=(boolean)(opt->arychild==PARY_CHILD);
  tidy_ary((ary_elm *)&opt->dary);
  tidy_ary((ary_elm *)&opt->pary);
  if(paryparent)
    {opt=opt->aryparent;
     while(opt) /* don't clear parent ary strs */
      {if(!opt->laryflg) tidy_ary((ary_elm *)&opt->pary);
       opt=opt->aryparent;}
    }
}

void free_allarys(opt)
  path_elm *opt;
{ tidy_allarys(opt);
  nul_free(opt->pary.ary); nul_free(opt->dary.ary);
}

void free_reb(ppt)
  path_elm *ppt;
{ if(ppt->wldpth) free_pelm(ppt->wldpth);
  if(ppt->lary)
    {if(ppt->lary->stktop) free_sak(ppt->lary->stktop);
     clear_ary((ary_elm *)&ppt->lary->sdary);
     clear_ary((ary_elm *)&ppt->lary->spary);
     fremem(ppt->lary);
    }
}

/* clr & then del path elm */
void free_pelm(ppt)
  path_elm *ppt;
{ if(!ppt) return;
  release_net((net_elm **)&ppt->net);
  free_allarys(ppt);
  chk_free(ppt->pat);
  free_reb(ppt);
  fremem(ppt);
}

/* remove a path_elm from path */
void rem_path(wpt)
  path_elm *wpt;
{ path_elm *ppt,*tpt;
  if(wpt)
    {tpt=wpt->tail; ppt=wpt->parent;
     if(ppt) ppt->tail=tpt; if(tpt) tpt->parent=ppt;
     free_pelm(wpt);
    }
}

/* Free this path entirely */
void free_path(ppt)
  path_elm *ppt;
{ path_elm *lpt;
  if(!ppt) return;
  /* clear from top down:
  while(ppt->parent) ppt=ppt->parent;
  while(ppt) {lpt=ppt->tail; free_pelm(ppt); ppt=lpt;}
  */
  /* clear from bottom up: (in case of PARY_CHILD's etc) */
  lpt=ppt; while(ppt->tail) ppt=ppt->tail;
  while(ppt) {lpt=ppt->parent; free_pelm(ppt); ppt=lpt; if(lpt) lpt->tail=NULL;}
}

void zap_path(apt)
  path_elm *apt;
{ apt->laryflg=F; apt->partflg=F;
  apt->pat_id=LAB_NULL; apt->dec_id=LAB_NULL;
  apt->reb_id=LAB_NULL; apt->tac_id=LAB_NULL;
  apt->larypos=(-1); apt->laryat=(-1);
  apt->curwldflg=F; apt->wldflg=F; apt->rebflg=F; apt->rebwldflg=F;
  apt->tail=NULL; apt->aryparent=NULL; apt->arychild=NULL;
  apt->decl_pt=NULL; apt->lab_pt=NULL; apt->pat=NULL;
  apt->lary=NULL; apt->wldpth=NULL;
  zap_ary((ary_elm *)&apt->pary); zap_ary((ary_elm *)&apt->dary);
  apt->dary.isdary=T;
}

void tidy_reb(ppt)
  path_elm *ppt;
{ if(ppt->rebflg)
    {free_reb(ppt); ppt->wldpth=NULL; ppt->lary=NULL;
     ppt->reb_id=LAB_NULL; ppt->tac_id=LAB_NULL;
     ppt->rebflg=F; ppt->rebwldflg=F; ppt->rebflg=F;
    }
  ppt->larypos=(-1); ppt->laryat=(-1); ppt->laryflg=F;
  ppt->decl_pt=NULL; ppt->lab_pt=NULL;
}

/* tidy path elm ready for next user,
*  but don't touch opt->partflg, opt->pat & opt->lab_id */
void tidy_pelm(opt)
  path_elm *opt;
{ tidy_allarys(opt);
  nul_free(opt->dary.ary);
  opt->laryflg=F;  opt->curwldflg=F; opt->wldflg=F;
  opt->larypos=(-1); opt->laryat=(-1); opt->dec_id=LAB_NULL;
  opt->decl_pt=NULL; opt->lab_pt=NULL;
  if(opt->lary || opt->wldpth) signal_error(EXIT_INT_ERR,111);
  opt->lary=NULL; opt->wldpth=NULL;
  opt->rebflg=F; opt->rebwldflg=F;
  opt->reb_id=LAB_NULL; opt->tac_id=LAB_NULL;
}

/* clr ready for next user */
void clear_pelm(opt)
  path_elm *opt;
{ tidy_pelm(opt); nul_free(opt->pary.ary);
  opt->partflg=F; opt->pat_id=LAB_NULL; nul_free(opt->pat);
}

void blank_pelm(ppt)
  path_elm *ppt;
{ clear_ary((ary_elm *)&ppt->pary); clear_ary((ary_elm *)&ppt->dary);
  nul_free(ppt->pat); ppt->dec_id=LAB_NULL; ppt->pat_id=LAB_NULL;
  ppt->decl_pt=NULL; ppt->lab_pt=NULL;
}
/* Fetch a new path storage element from memory */
path_elm *new_path(ppt)
  path_elm *ppt;
{ path_elm *apt;
  apt=malmem(path_elm,1);
  zap_path(apt); apt->net=NULL;
  apt->parent=ppt; if(ppt) ppt->tail=apt;
  return(apt);
}

/* Note apt is a copy of ary, to be overwritten by new eval_list */
void copy_eval(apt)
  ary_elm *apt;
{ apt->eval_list=lcopy(apt->eval_list,(eval_elm *)NULL);
}

/* Copy ary info: ary_list and name string into apt.
*  copy eval_list if cpeval set
*  Note apt is a straight copy of ary, we now have to fixup pts etc
*/
void copy_ary(apt,cpeval)
  ary_elm *apt; boolean cpeval;
{ boolean reset=T; /* force ary sel dirty bits to be cleared for now...*/
  if(apt->ary_list) apt->ary_list=lcopy(apt->ary_list,NULL);
  else {apt->suspend=F; reset=T;}
  if(cpeval) copy_eval(apt);
  else
    {apt->eval_list=NULL; apt->suspend=F; reset=T;}
  if(apt->ary) apt->ary=strsave(apt->ary);
  if(reset) reset_sels(apt->ary);
}

sak_elm *copy_sak(spt,cpary)
  sak_elm *spt; boolean cpary;
{ sak_elm *rpt=NULL,*lpt;
  while(spt)
    {lpt=malmem(sak_elm,1); lpt->ary=spt->ary;
     copy_ary((ary_elm *)&lpt->ary,cpary);
     lpt->tail=NULL;
     if(!rpt) rpt=lpt; else rpt->tail=lpt;
     spt=spt->tail;
    }
  return(rpt);
}

lary_elm *copy_lary(lary)
  lary_elm *lary;
{ lary_elm *lpt=NULL; sak_elm *spt;
  if(lary)
    {lpt=malmem(lary_elm,1);
     *lpt=(*lary);
     /* lpt->tmplcpt (ppt->arychild) is set later */
     spt=copy_sak(lpt->stktop,T);
     lpt->stktop=spt; if(spt) while(spt->tail) spt=spt->tail;
     lpt->stail=spt;
     copy_ary((ary_elm *)&lpt->sdary,T);
     copy_ary((ary_elm *)&lpt->spary,T);
    }
  return(lpt);
}

/* Copy a path state. Yuck. Have to create new pointers etc.
*  sets lary, rebwldflg, wldpth & rebflg to F/null if flag cpary is F */
path_elm *copy_path(ppt,cpary)
  path_elm *ppt; boolean cpary;
{ path_elm *cpt=ppt,*npt=NULL,*opt=NULL,
           *spt,*old_top,*new_top=NULL,*xpt,*ypt;
  while(ppt->parent) ppt=ppt->parent; /*find root*/
  old_top=ppt;
  while(ppt)
    {npt=malmem(path_elm,1);
     if(!new_top) new_top=npt; /* this points to root of new path */
     *npt=(*ppt); /* this copies simple vars etc */
     if(!cpary) {npt->wldpth=NULL; npt->lary=NULL; npt->rebflg=F;}
     else npt->lary=copy_lary(ppt->lary);
     if(npt->net && npt->net->refs>0)
       ++(npt->net->refs); /* we have made a copy of this net def */
     if(ppt->pat) npt->pat=strsave(ppt->pat);
     copy_ary((ary_elm *)&npt->pary,cpary); /* this points to old ary info */
     copy_ary((ary_elm *)&npt->dary,cpary);
     spt=npt->aryparent; xpt=old_top; ypt=new_top;
     npt->parent=opt; npt->tail=NULL;
     if(spt)
       {do{if(xpt==spt)
             {npt->aryparent=ypt;
              if(cpary && ypt->lary && ypt->lary->tmplcpt!=PARY_CHILD)
                 ypt->lary->tmplcpt=npt;
              if(ypt->arychild!=PARY_CHILD) ypt->arychild=npt;}
           if(xpt==ppt) break;
           xpt=xpt->tail; ypt=ypt->tail;
          }while(xpt);
       }
     if(opt) opt->tail=npt;
     opt=npt; ppt=ppt->tail;
    }
  return(new_top);
}

/* copy path, but only down to ppt
*  copy ary eval stk info if cpary=T */
path_elm *dup_path(ppt,cpary)
  path_elm *ppt; boolean cpary;
{ path_elm *ptail,*pchild,*cpt;
  ptail=ppt->tail; ppt->tail=NULL; pchild=ppt->arychild; ppt->arychild=NULL;
  cpt=copy_path(ppt,cpary); ppt->tail=ptail; ppt->arychild=pchild;
  return(cpt);
}

/* returns T if str is a wild ary, undefined (==null), or not an ary (!='[') */
boolean is_wild(apt)
  char *apt;
{ if(!apt) return(T); if(*apt!='[') return(T);
  return((boolean)(*(apt+1)==LIST_WILD));
}

/* returns T if list is a constant i.e. yields same list regardless of
*  context
*  called primarily by eval_nth: some lops remain const as well....
*/
boolean is_const(apt)
  char *apt;
{ if(!apt) return(T); if(*apt!='[') return(T); ++apt;
  return((boolean)(*apt==LIST_ATCON || *apt==LIST_LSCON));
}

/* Looks at ary str list code & determines if ary is a list expression.
* if [*] or [...a,b...] or [...a..b...] or [...usrlistfunc...] etc
*/
boolean is_list(apt)
  char *apt;
{ char chr; if(!apt) return(F); if(*apt!='[') return(F);
  chr= *(apt+1);
  return((boolean)!(chr==LIST_ATCON || chr==LIST_ATSEL
          || chr==LIST_ATPAR || chr==LIST_ATEXC || chr==LIST_ATONE));
}

/* See if str ends with name, if so chop it off and return true
*/
boolean check_grp(str,name)
  char *str,*name;
{ int nlen=strlen(name),slen=strlen(str); char *member;
  member=str+slen-nlen;
  if(strcmp(member,name)==0)
    {--member;
     if(!(isalnum(*member) || *member=='_')) {*member='\0'; return(T); }
    }
  return(F);
}

/* find first left non [*]
*  returns NULL if none found
*  Note: bypassed arrays (in conx_to_node etc) have ary set, but ary_list=NULL
*/
path_elm *byleft(ppt,isterm)
  path_elm *ppt; boolean isterm;
{ if(isterm && !((ppt->arychild==PARY_CHILD) || !(ppt->arychild)))
    return(NULL); /* at right terminal [*] */
  /* check if non [*] to left */
  do{if(!is_wild(ppt->pary.ary)) break;
     ppt=ppt->aryparent;
    }while(ppt);
  return(ppt);
}

/* bypass to right untill non [*] or hit spt
*  returns NULL if spt & [*] */
path_elm *byright(ppt,spt,istermp)
  path_elm *ppt,*spt; boolean *istermp;
{ *istermp=F;
  if(ppt)
    do{if(!is_wild(ppt->pary.ary)) break;            /* added !ppt->arychild*/
       if(!ppt->arychild || ppt->arychild==PARY_CHILD) /*DEBUG DJRW 6/12/91*/
         {*istermp=T; break;}
       if(ppt==spt) return(NULL);
       ppt=ppt->arychild;
      }while(ppt);
  return(ppt);
}

/* bypass a wild array if possible
*  On entry istermp is set if at dary, else at pary
*  goleft indicates initial direction to bypass in for pary's
*  On exit istermp is set according to ary returned
*  returns null if couldn't bypass & posts a warning
*/
path_elm *bypass(ppt,istermp,goleft)
  path_elm *ppt; boolean *istermp,goleft;
{ path_elm *tpt;
  if(*istermp)
    {if(is_wild(ppt->dary.ary))
       {tpt=byleft(ppt,T);
        if(tpt) {*istermp=F; return(tpt);}
        else {path_error(PER_GEN,ppt,ppt,WRN_SEL_BUN); return(NULL);}
       }
    }
  else if(is_wild(ppt->pary.ary))
    {if(goleft) {tpt=byleft(ppt,F); if(tpt) return(tpt);}
     tpt=byright(ppt,(path_elm *)NULL,istermp);
     if(!tpt || (*istermp && is_wild(tpt->dary.ary)))
       {*istermp=F; if(goleft) tpt=NULL; else tpt=byleft(ppt,F);}
     if(tpt) ppt=tpt;
     else {path_error(PER_GEN,ppt,ppt,WRN_SEL_BUN); return(NULL);}
    }
  else *istermp=F;
  return(ppt);
}

/* Fetch value of nth elm from a mile pair list: returns val or -1 */
mile get_nth(ary,nth)
  mile *ary,nth;
{ mile num_elms,asize,aelm,bl,al;
  if(!ary) signal_error(EXIT_INT_ARY); /*DEBUG trap */
  if(nth>=0)
    {asize=lst_num(ary);
     ary=lst_head(ary,mile); num_elms=0;
     while(asize)
          {al= *ary++; bl= *ary++; asize-=2;
           aelm=num_elms; ++num_elms;
           bl=al-bl; if(bl<0) num_elms-=bl; else num_elms+=bl;
           if(nth<num_elms)
             {nth-=aelm;
              if(bl<0) return(al+nth); else return(al-nth);
             }
          }
    }
  return(-1);
}

/* Add elm to list_ary in pair format
*/
void add_map_res(c_ary,clo,chi)
  mile **c_ary,clo,chi;
{ mile *mpt;
  *c_ary=lmore(*c_ary,(int)2,(int)STAR_PAIR,mile);
  mpt=lst_tail(*c_ary,mile);
  *mpt-- =chi; *mpt=clo;
}

/* find the node path-node belonging to current IN path */
path_elm *node_path(tpt)
  path_elm *tpt;
{ path_elm *rpt=tpt;
  while(tpt)
   {if(tpt->net->root_flag) return(tpt); /*DEBUG was tpt->parent*/
    tpt=tpt->parent;
   }
  return(rpt); /* none found, return cur pt */
}
/*----------------------------------------------------------------------*/
/* label routines */

/* Chop off the array part which starts the str and return result in spt.
* On exit str points to pos after original array.
*/
char *get_array(str,spt)
  char *str,**spt;
{ char chr,*apt;
  push_io(); set_io_str(str); skip_bodies(); apt=get_io_pos(); pop_io();
  if(*spt) signal_error(EXIT_INT_ERR,112);
  chr= *apt; *apt='\0'; *spt=strsave(str); *apt=chr;
  return(apt);
}

/* return the highest lab_id in path + 1 */
byte new_lab_id(ppt)
  path_elm *ppt;
{ path_elm *tpt=ppt; byte id=LAB_NULL;
  while(tpt)
    {if(tpt->pat_id>id) id=tpt->pat_id;
     if(tpt->dec_id>id) id=tpt->dec_id;
     tpt=tpt->parent;}
  if(ppt)
    {tpt=ppt->tail;
     while(tpt)
       {if(tpt->pat_id>id) id=tpt->pat_id;
        if(tpt->dec_id>id) id=tpt->dec_id;
        tpt=tpt->tail;}
    }
  return((byte)(id+1));
}

/* copy path pat and ary to path elm
*/
char *store_lab(ppt,str,id,hastail)
  path_elm *ppt; char *str; byte id; boolean hastail;
{ char **arypt=(char **)&ppt->pary.ary,*apt,chr=(*str); boolean dumwld;
  if(*arypt || !str) signal_error(EXIT_INT_ERR,113);
  ppt->partflg=F;
  switch(chr) /* process part & path seps */
    {case '\\': /* soft part */ if(hastail) goto incstr;
     case '|': /* hard part */ ppt->partflg=T;
     case '/': case '.': incstr: ++str;
     default:;}
  apt=get_wild_id(str,(boolean *)&dumwld);
  ppt->wldflg=dumwld; ppt->curwldflg=dumwld;
  if(apt==str) {path_error(PER_GEN,ppt,ppt,ERR_OPTH_NAM); return(NULL); }
  chr= *apt; *apt='\0'; ppt->pat=strsave(str); *apt=chr; str=apt;
  ppt->dec_id=LAB_NULL;
  if(chr=='[')
    {if(*ppt->pat=='#') {arypt=(char **)&ppt->dary.ary; ppt->dec_id=id;}
     str=get_array(str,arypt);}
  if(ppt->pat_id) signal_error(EXIT_INT_ERR,114);
  ppt->pat_id=id;
  return(str);
}

/* Break up a path into members(name:ary) and store as a path list
*  sets path->pat & path->pary.ary when appropriate
*  Returns tail : Note assumes caller has taken care of abs paths & parents
*  i.e. /abspath or ../../restofpath etc
*/
path_elm *stack_lab(parpt,str,id,hastail)
  path_elm *parpt; char *str; byte id; boolean hastail;
{ path_elm *ppt=parpt; /* parent path pointer */
  if(!str || !*str) signal_error(EXIT_INT_ERR,115);
  while(*str)
    {ppt=new_path(ppt);
     if(!(str=store_lab(ppt,str,id,hastail))) return(parpt); }
  return(ppt);
}

byte stack_str(ppt,str)
  path_elm *ppt; char *str;
{ byte id=new_lab_id(ppt);
  if(!str || !*str) signal_error(EXIT_INT_ERR,116);
  str=store_lab(ppt,str,id,F);
  if(str)
    {if(*str)
       {if(stack_lab(ppt,str,id,F)==ppt) id=0;}
    }
  else id=0;
  return(id);
}

char *tack_pat(ppt)
  path_elm *ppt;
{ byte id=new_lab_id(ppt);
  if(flags.inp) return(store_lab(ppt,STR_INARY,id,F));
  return(store_lab(ppt,STR_OUTARY,id,F));
}

path_elm *tack_lvl(ppt)
  path_elm *ppt;
{ byte id=new_lab_id(ppt);
  if(flags.inp) return(stack_lab(ppt,STR_INARY,id,F));
  return(stack_lab(ppt,STR_OUTARY,id,F));
}

/* Link in label path to main output path, set relative path pointers etc.
*  Match label ary with first path ary if necessary
* On entry: path_pt->pat = pattern caller matched on
*   path_pt->dary is to be set by label value
*   path_pt->pary was set by caller
* Note label val comes from nets label list "!Dx[..]/path" or "!Dx/path"
* The !Dx part of label is chopped off when net is read in
* thus, either "[..]/path" or "/path"
* Returns lab_id of installed lab if ok, 0 otherwise
*/
byte install_lab(normal)
  boolean normal;
{ path_elm *newtailpt,*oldnextpt,*lpt; char *apt=path_pt->lab_pt->val;
  byte id=0; boolean darlist=F;
  if(!apt) return(0);
  while(*apt=='@' || *apt=='.' || *apt=='/') ++apt;
  if(!*apt) return(0);
  newtailpt=path_pt; oldnextpt=path_pt->tail; path_pt->tail=NULL;
  if(path_pt->dary.ary) signal_error(EXIT_INT_ERR,117);
  id=new_lab_id(path_pt);
  if(*apt=='[') /* stack label decl ary */
    {apt=get_array(apt,(char **)&path_pt->dary.ary);
     if(path_pt->dec_id) signal_error(EXIT_INT_ERR,118);
     path_pt->dec_id=id;}
  if(*apt) /* stack rest of label value (first chr may be a sep) */
    {newtailpt=stack_lab(path_pt,apt,id,(boolean)(oldnextpt!=NULL));
     if((path_pt->dec_id!=id) && (newtailpt==path_pt)) return(0);
     /* no label to install !! */}
  if(!path_pt->dary.ary) /* make sure dary has ary */
    {darlist=F; path_pt->dec_id=id;
     /*DEBUG2/6/93, was just set to STR_SARY, but now:*/
     if(path_pt->pary.ary) /* caller has a pary label, so add STR_AND */
        path_pt->dary.ary=strsave(STR_AND);
     else path_pt->dary.ary=strsave(STR_SARY);
    }
  else darlist=is_list(path_pt->dary.ary);
  /* If this label was used with an ary postfix, */
  /* pair it with first child ary appearing in label value */
  if(path_pt->pary.ary)
    {lpt=path_pt; /* chk decl ary of path_pt */
     if(!darlist)
       {lpt=lpt->tail; /* chk pary's in rest of label value */
        while(lpt) {if(is_list(lpt->pary.ary)) break; lpt=lpt->tail; }
        if(!lpt) /* no child ary was found, so create one */
          {/* if label value ends with an ary add a new member to value */
           /* otherwise tack a wild ary on to tail of label value */
           lpt=newtailpt;
           if(newtailpt->pary.ary)
             {if(oldnextpt) /* if no list in lab val pair with dary.ary */
                {lpt=path_pt;}
              else
                {if(!path_pt->dary.ary && path_pt->decl_pt->size>1)
                   {path_pt->dary.ary=strsave(STR_SARY);
                    if(path_pt->dec_id) signal_error(EXIT_INT_ERR,119);
                    path_pt->dec_id=path_pt->pat_id;
                    lpt=path_pt; }
                 else
                   {if(normal || is_list(path_pt->pary.ary))
                      {newtailpt=tack_lvl(newtailpt); lpt=newtailpt;}
                    else /* a lary ary with 1 elm was given */
                      lpt=newtailpt; /* Match lary with last label ary */
                   }
                }
             }
           else newtailpt->pary.ary=strsave(STR_SARY);
       }  }
     /* Note if dary was selected as child ary, these will be self-refering */
     /* i.e. pary is aryparent to dary, but aryparent may be in use already */
     /* so.. */
     if(lpt)
       {if(lpt==path_pt) /* indicate self-ref with special dummy child */
          path_pt->arychild=PARY_CHILD;
        else /* a normal label parent and child */
          {lpt->aryparent=path_pt; path_pt->arychild=lpt;}
       }
    }
  /* reroute original path through new label value */
  if(oldnextpt) {newtailpt->tail=oldnextpt; oldnextpt->parent=newtailpt;}
  return(id);
}

/* Yank label value from out path (denoted by lab_id)
*  ppt pts at point in lab to yank from, but don't del entry ppt
*/
void yank_lab(ppt,lab_id)
  path_elm *ppt; byte lab_id;
{ path_elm *tpt=ppt->arychild,*npt=ppt->tail;
  if(tpt)
    {if(tpt==PARY_CHILD) {if(ppt->dec_id>=lab_id) ppt->arychild=NULL;}
     else {tpt->aryparent=NULL; ppt->arychild=NULL;}
    }
  if(ppt->pat_id>=lab_id) {nul_free(ppt->pat); ppt->pat_id=LAB_NULL;}
  if(ppt->pary.ary) tidy_ary(&ppt->pary); /* DEBUG added */
  if(ppt->dec_id>=lab_id) {clear_ary(&ppt->dary); ppt->dec_id=LAB_NULL;}
  while(npt && (npt->pat_id>=lab_id))
    {tpt=npt; npt=npt->tail; free_pelm(tpt);}
  ppt->tail=npt;
  if(npt) npt->parent=ppt;
}
/*-----------------------*/
/* set up cur ref path elm according to decl & indx */
int make_path(ppt)
  path_elm *ppt;
{ path_elm *tmppt;
  ary_elm *apt=(ary_elm *)&ppt->dary; mile size=ppt->decl_pt->size;
  if(apt->ary_list || ppt->pary.ary_list) signal_error(EXIT_INT_ERR,120);
  zap_ary((ary_elm *)&ppt->pary); zap_ary(apt); apt->isdary=T;
  if(flags.state && flags.build && size<1)
    {apt->ary=strsave(STR_DARY);
     tmppt=path_pt; path_pt=ppt;
     apt->ary_list=sip_list(ds_stub);
     path_pt=tmppt;
     apt->pipval=nsl_lst_num(apt->ary_list);
     apt->fixval=0;
    }
  else
    {if(size<1) signal_error(EXIT_INT_ERR,121);
     apt->ary=strsave(STR_AND);
     apt->fixval=size; apt->pipval=size; --size;
     add_map_res(&apt->ary_list,(mile)0,size);
    }
  if(apt->ary_list) return(LEX_OK);
  return(LEX_ER);
}

/* set up path when doing a var initialiser */
void var_init(ppt,str)
  path_elm *ppt; char *str;
{ char *apt,chr=(*str); boolean dumwld;
  byte id=new_lab_id(ppt);
  apt=get_wild_id(str,(boolean *)&dumwld);
  if(apt==str) signal_error(EXIT_INT_ERR,122);
  ppt->wldflg=dumwld;
  ppt->curwldflg=(boolean)(dumwld || ppt->parent->curwldflg);
  chr= *apt; *apt='\0'; ppt->pat=strsave(str); *apt=chr;
  if(!is_wild(apt)) get_array(apt,(char **)&ppt->dary.ary);
  ppt->dec_id=id; ppt->pat_id=id; ppt->partflg=F;
}


/*------------------------------------------------------------------------*/
/* Routines to evaluate selectors */

/* count num members to label head from cur pos (ppt is not null)
*  isterm reflects is dary */
int lab_hd(ppt,isterm)
  path_elm *ppt; boolean isterm;
{ int lvl=0; byte id;
  if(isterm) return(0);
  id=ppt->pat_id;
  while(ppt->parent)
    {ppt=ppt->parent; if((ppt->pat_id==id)||(ppt->dec_id==id)) ++lvl;}
  return(lvl);
}

/* count num members to label tail from cur pos
*  isterm reflects isdary */
int lab_tl(ppt,isterm)
  path_elm *ppt; boolean isterm;
{ int lvl=0; byte id;
  if(isterm) id=ppt->dec_id; else id=ppt->pat_id;
  while(ppt->tail) {ppt=ppt->tail; if(ppt->pat_id==id) ++lvl;}
  return(lvl);
}

/* convert cardinal coors to relative for path in label mode
*  isterm reflects isdary
*/
int lab_car_to_rel(ppt,isterm,carlvl)
  path_elm *ppt; boolean isterm; int carlvl;
{ if(carlvl<1) return(1+carlvl+lab_tl(ppt,isterm));
  else return(carlvl-lab_hd(ppt,isterm));
}

/* ascend/descend path in lab units, set rfdarypt if ref a dary
*  lvl<1 --> parent    lvl>1 --> tail
*  returns null if lvl out of range
*/
path_elm *asde_lab(ppt,lvl,rfdarypt)
  path_elm *ppt; int lvl; boolean *rfdarypt;
{ byte id; int idinc; path_elm *tpt=ppt;
  if(lvl!=1)
    {if(*rfdarypt) id=ppt->dec_id; else id=ppt->pat_id;
     if(lvl>1) idinc=(-1); else idinc=1;
     while(ppt && lvl!=1)
        {if(idinc>0) ppt=ppt->parent; else ppt=ppt->tail;
         if(!ppt) break;
         if((ppt->pat_id==id)||(ppt->dec_id==id)) lvl+=idinc;
        }
     if(lvl==1) {if(ppt->dec_id==id) *rfdarypt=T; else *rfdarypt=F;}
     else path_error(PER_GEN,tpt,tpt,WRN_SEL_OOR);
    }
  return(ppt);
}

int abs_hd(ppt)
  path_elm *ppt;
{ int lvl=0;
  while(ppt->parent) {ppt=ppt->parent; ++lvl;}
  return(lvl);
}

/* count num members to path tail from cur pos */
int abs_tl(ppt)
  path_elm *ppt;
{ int lvl=0;
  while(ppt->tail) {ppt=ppt->tail; ++lvl;}
  return(lvl);
}

/* convert cardinal coors to relative for path in absolute mode */
int abs_car_to_rel(ppt,carlvl)
  path_elm *ppt; int carlvl;
{ if(carlvl<1) return(1+carlvl+abs_tl(ppt));
  else return(carlvl-abs_hd(ppt));
}

/* ascend/descend path in abs units, set rfdarypt if ref a dary
* note abs ref to dary's
* returns null if lvl out of range
*/
path_elm *asde_abs(ppt,lvl,rfdarypt)
  path_elm *ppt; int lvl; boolean *rfdarypt;
{ boolean goup; path_elm *tpt=ppt;
  *rfdarypt=T; /* in abs mode always ref a dary */
  if(lvl!=1)
    {goup=(boolean)(lvl<1);
     while(ppt && lvl!=1)
        {if(goup) {ppt=ppt->parent; ++lvl;}
         else {ppt=ppt->tail; --lvl;}
        }
     if(lvl!=1) path_error(PER_GEN,tpt,tpt,WRN_SEL_OOR);
    }
  return(ppt);
}

/* Return path elm that is sub_num levels from cur pos.
*  called indirectly from yacc_parse
*    rfdarypt set if final ary is dary
*    if pary & inds, *sppt is rel to base mary
*  On entry coors indicates type of indexing e.g: (@,^,$ ?,!)
*  absolute or label, cardinal or relative, in or out, cur ary or dary
*  stype is sel type, sub_num=(+|-)n, isdary set if cur ppt ary is dary.
*  Note if sub_num goes beyond end of path returns null
*  If ref a wild array & bypassing results in cur ary then sigs warning
*/
path_elm *get_level(ppt,stype,coors,sub_num,sppt,inds,rfdarypt)
  path_elm *ppt; int stype,coors,sub_num; stub_elm **sppt;
  boolean *inds, *rfdarypt;
{ boolean self=F,arydary=(boolean)((coors&SIN_ARY)!=0),tdary;
  path_elm *opt=ppt;
  tdary=yyary_pt->isdary; *rfdarypt=tdary;
  *inds=F; *sppt=NULL; coors&=SIN_MASK;
  if(!ppt) return(ppt);
/* Note extended seletor codes overlap SEL_EXC's, so don't chk any more
  if(stype==(int)SEL_EXCAT || stype==(int)SEL_EXCNUM
     || stype==(int)SEL_EXC || stype==(int)SEL_EXCEXC)
    signal_error(EXIT_INT_ERR,123);
*/
  switch(coors)
    {          /*  PATH     CAR     ABS  */
     case 'F': /* SIN_INP SIN_CAR  label */
       if(org_pt) ppt=org_pt;
       else path_error(PER_GEN,ppt,ppt,WRN_STIL_UNF,"?");
       goto carlab;
     case 'J': /* SIN_OUT SIN_CAR  label */
       if(!(ppt=sel_out_val(ppt,F,sub_num))) return(NULL);
     case 'B': /* curpath SIN_CAR  label */
       carlab: self=T; sub_num=lab_car_to_rel(ppt,tdary,sub_num);
     case '@': /* curpath relative label */
       ppt=asde_lab(ppt,sub_num,rfdarypt);
       break; /* go set sppt & inds */
     case 'G': /* SIN_INP SIN_CAR  SIN_ABS */
       if(!inpt) signal_error(EXIT_INT_ERR,124); else ppt=inpt;
       goto carabs;
     case 'K': /* SIN_OUT SIN_CAR  SIN_ABS */
       if(!(ppt=sel_out_val(ppt,T,sub_num))) return(NULL);
     case 'C': /* curpath SIN_CAR  SIN_ABS */
       carabs: self=T; sub_num=abs_car_to_rel(ppt,sub_num);
     case 'A': /* curpath relative SIN_ABS */
       ppt=asde_abs(ppt,sub_num,rfdarypt);
       break; /* now set sppt etc */
     default: signal_error(EXIT_INT_ERR,125);
    }
  if(!ppt) return(NULL);
  ppt=bypass(ppt,rfdarypt,(boolean)(sub_num<1));
  if(ppt && arydary && !*rfdarypt) /*rqst for dary to ppt, move to base Mary*/
    {while(ppt->arychild)
       {if(ppt->arychild==PARY_CHILD) break; ppt=ppt->arychild; }
     *rfdarypt=T;
     if((stype<SEL_AND || stype>SEL_COL) && stype!=SEL_LOP_DOT)
       ppt=bypass(ppt,rfdarypt,F);
    }
  *sppt=find_dspt(ppt,inds); /* find ds pos if possible */
  /* see if an unintentional self ref has occurred */
  if((ppt==opt) && (*rfdarypt==tdary))
    {if(self || (!self && (sub_num!=1)))
       path_error(PER_GEN,ppt,ppt,WRN_SEL_SER);}
  return(ppt);
}

mile virt_size(ppt)
  path_elm *ppt;
{ switch((int)ppt->decl_pt->sst_pos)
   {case VIR_HEAP: return(lst_ok(heap_list));
    case VIR_PILE: if(fly_bot) return(lst_ok(fly_bot->pile_list));
    default: return(0); /* bin size always 0 */
   }
}

/* compute node number from path */
long calc_node(ppt) path_elm *ppt;
{ long indx,num;
  if(!ppt) return(0);
  while(ppt->parent) ppt=ppt->parent;
  num=ppt->net_base;
  while(ppt)
    {if(ppt->net&&ppt->net->root_flag) break;
     if(ppt->decl_pt)
       {num+=ppt->decl_pt->nodenum;
        indx=(long)ppt->dary.hasval;
        if(indx>0) num+=(indx*ppt->decl_pt->nodes);
       }
     ppt=ppt->tail;
    }
  return(num);
}
/* Meaning of selectors:  (note may be postfixed by fix_path)
* Note when refdary/ds op '.' is used after certain sels, their meaning is
* modified slightly
* $&x = fixed size of array, after '.': size of DS if inds
*       Note UA's return size of zero, fixed arys return max_elm+1
* $|x = cur num elms in list
*       after '.': num elms in DS ary if inds or 0 if applied to net
* $^x = largest value in list
*       after '.': largest ds ary val if inds or (-1) if applied to net
* $_x = lowest value in list
*       after '.': lowest ds ary val if inds or 0 if applied to net
* $%x = number of net or struct subcomponents associated with decl
*       after '.': number of input conxs to branch so far if inds
* $:x = number of net or struct subcomponents associated with decl
*       after '.': number of conx outputs from branch so far if inds
* $@x = position of cur elm in it's list (-1 if not ref'd yet)
* $#x = value of cur elm (-1 if not ref'd yet)
* $!@ $!# = returns conx absolute or relative number to be read next
* $!! = returns non zero if not end of oelm
* $!p = returns number of leafs in next opath lists at specified part lvl
* $*x = returns path dip string, note caller should copy before it is reused
* $+x = returns path string, copy before reuse
* $-x = returns path type string, copy before reuse
*/
reals selector_val(sel_type,coors,sub_num,cstype)
  int sel_type,coors,sub_num; mile cstype;
{ path_elm *ppt; stub_elm *spt; ary_elm *arypt; mile *a_list;
  reals value=(reals)(-SEX_UF); state_elm aux_state; decl_elm *tmpaux;
  boolean isvirt,inds,rfdary,arydary,dsok=F,tmpvax=vax_flg;
  if(yyary_pt->suspend) return((reals)-SEX_CL);
  if(selec && ((*selec)&SEL_DIRTY))
    {yyary_pt->suspend=T; return((reals)-SEX_CL);}
  if(selec) (*selec)|=SEL_DIRTY;
  arydary=(boolean)((coors&SIN_ARY)!=0);
  if(vax_flg)
    {state_save((state_elm *)&aux_state);
     state_restore((state_elm *)&aux_bot->par_state);
     tmpaux=ilk_pt->aux; ilk_pt->aux=NULL; vax_flg=F;
    }
  if(ppt=get_level(path_pt,sel_type,coors,sub_num,
                   (stub_elm **)&spt,(boolean *)&inds,(boolean *)&rfdary))
   {isvirt=(boolean)(!ppt->decl_pt->declnum);
    if(rfdary) arypt=(ary_elm *)&ppt->dary; else arypt=(ary_elm *)&ppt->pary;
    if(arydary) /* user wants dary, chk got dary or it's been bypassed */
      {if((sel_type>=SEL_AND && sel_type<=SEL_COL) || sel_type==SEL_LOP_DOT)
         dsok=(boolean)(inds && spt); /* DS ary, or decl ref*/
       else if((!rfdary) && (!(is_wild(ppt->dary.ary) && byleft(ppt,T))))
         path_error(PER_GEN,ppt,ppt,WRN_SEL_DAR);
      }
    switch(sel_type)
      {case SEL_XS_NODE: /* node number */
         value=(reals)calc_node(ppt); break;
       case SEL_AT:   /* cur pos of elm: (-1) not refd */
         value=(reals)arypt->atval; break;
       case SEL_HASH: /* cur val of elm: (-1) not refd */
         value=(reals)arypt->hasval; break;
       case SEL_AND: fixsize: /* fixed size of arys */
         value=(reals)arypt->fixval; break;
       case SEL_PERC: /* input conx's associated with decl */
         if(dsok) /* fetch # conxs in ary so far from ds */
           value=(reals)ds_value(spt)->conx.in_num;
         else if(arydary && inds) /* ds not def'd yet: no conxs */
           value=(reals)0.0;
         else /* return number of subnets/substructs on decl */
           value=(reals)ppt->decl_pt->nodes;
         break;
       case SEL_COL: /* output conx's associated with decl */
         if(dsok) /* fetch # conxs in ary so far from ds */
           value=(reals)ds_value(spt)->conx.out_num;
         else if(arydary && inds) /* ds not def'd yet: no conxs */
           value=(reals)0.0;
         else /* return number of subnets/substructs on decl */
           value=(reals)ppt->decl_pt->nodes;
         break;
       case SEL_PIP:  /* cur num elms in list */
         if(dsok) /* fetch num elms in ds ary */
           {if(spt->type==TYPE_NODE) /* ds not def'd yet*/
              value=(reals)0.0;
            else value=(reals)spt->info.data.elm;}
         else if(arydary && inds) /* ds not def'd yet: size 0 */
           value=(reals)0.0;
         else if(isvirt) value=(reals)virt_size(ppt);
         else if(arydary) /* cur num elms in a net always 0 */
           value=(reals)0.0;
         else value=(reals)arypt->pipval;
         break;
       case SEL_HAT:  /* cur largest val in ary */
         if(dsok) /* fetch largest elm in ds ary */
           {if(ppt->decl_pt->size>0) value=(reals)(ppt->decl_pt->size-1);
            value=(reals)sip_largest(spt);}
         else if(arydary && inds) /* ds not def'd yet: size 0 */
           value=(reals)0.0;
         else if(isvirt) value=(reals)virt_size(ppt)-1;
         else if(arydary) /*cur lar of net always (-1) on arydary request*/
           value=(reals)(-1.0);
         else value=(reals)nsl_lst_hilo(arypt->ary_list,T);
         break;
       case SEL_SUB:  /* cur lowest val in ary */
         if(dsok) /* fetch lowest elm in ds ary */
           {if(ppt->decl_pt->size>0) value=(reals)0.0;
            else value=(reals)sip_lowest(spt);}
         else if(arydary && inds) /* ds not def'd yet: size 0 */
           value=(reals)(0.0);
         else if(isvirt) value=(reals)virt_size(ppt)-1;
         else if(arydary) /* cur lowest of a net always 0 on arydary request*/
           value=(reals)(0.0);
         else value=(reals)nsl_lst_hilo(arypt->ary_list,F);
         break;
       case SEL_LOP_EQU: /* special internal sel for [=] lop evaluation*/
         lst_ary_str(arypt->ary,cstype);
         value=(reals)(-SEX_NL); break;
       case SEL_LOP_EXC: /* special internal sel for [!] lop evaluation*/
         if(arypt->ary_list)
           {a_list=arypt->ary_list;
            dsok=(boolean)((coors&SIN_REV)!=0);
            if(dsok) a_list=(mile *)lrev(lcopy(a_list,(void *)NULL));
            lst_ary_ary(a_list,dsok,cstype); /* add to cur ary ds list */
            value=(reals)(-SEX_DS);
            break; /* indicate ds ary was found to caller */
           } /* otherwise null list [] added */
         unbemp:
         if(flags.state) path_error(PER_GEN,ppt,ppt,WRN_UAL_NUL);
         else path_error(PER_GEN,ppt,ppt,WRN_LOP_NUL);
         value=(reals)(-SEX_NL); break;
       case SEL_LOP_DOT: /* special internal selector for [.] lop evaluation*/
         if(inds && spt) /* fetch ds ary */
           {if(ppt->decl_pt->size<1)
              {a_list=sip_list(spt);
               if(a_list)
                 {if((coors&SIN_REV)!=0) a_list=(mile *)lrev(a_list);
                  lst_ary_ary(a_list,T,cstype); /* add to cur ary ds list */
                  value=(reals)(-SEX_DS);
                 }
               else goto unbemp; /* unbound ary is empty */
               break; /*indicate ds ary was found to caller*/
              }
           } /* otherwise fall through & return fixsize to caller */
         else if(isvirt) {value=(reals)virt_size(ppt); break;}
         goto fixsize;
       default: signal_error(EXIT_INT_ERR,126);
      }
   }
  if(tmpvax)
    {ilk_pt->aux=tmpaux;
     state_update((state_elm *)&aux_bot->par_state);
     state_restore((state_elm *)&aux_state);
     vax_flg=T;
    }
  return(value);
}

mile exc_selector(sel_type)
  int sel_type;
{ if(flags.state && !(ilk_pt && ilk_pt->curpt->invar))
    {path_error(PER_GEN,path_pt,path_pt,WRN_VAR_SEL); return(-SEX_UF);}
  if(yyary_pt->suspend) return(-SEX_CL);
  if(selec && ((*selec)&SEL_DIRTY)) {yyary_pt->suspend=T; return(-SEX_CL);}
  if(selec) (*selec)|=SEL_DIRTY;
  if(sel_type==(int)SEL_EXCEXC)
    {if(ilk_pt)
       {eval_scan(yyary_pt);
        if(ilk_pt->aux) /* Op has an aux path */
          {if(ilk_pt->startaux) return(F); /* chk aux_bot->ilk.endofcx */
           if(aux_bot->ilk.endofcx) return(T);
           if(!aux_bot->ilk.opt) return(F);
           if(get_conx(1)) return(F);
           return(T);
          }
        if(ilk_pt->endofcx) return(T);
        if(!ilk_pt->opt) return(F);
        if(get_conx(2)) return(F);
        return(T);
       }
     return(T); /* force end of conx if oelm undefined */
    }
  else if(sel_type==(int)SEL_EXCAT)
    {if(ilk_pt) return(ilk_pt->abnum);}
  else if(sel_type==(int)SEL_EXCNUM)
    {if(ilk_pt) return(ilk_pt->cxnum);}
  path_error(PER_GEN,path_pt,path_pt,WRN_NET_EXC);
  if(selec) (*selec)&=0xdf; /* mask out dirty bit */
  return((mile)selector_val((int)SEL_HASH,(int)SIN_DEF,(int)1),(mile)0);
}

/* push state, setup parent, do part_val, restore states */
mile parent_part(coors,sub_num)
  int coors,sub_num;
{ state_elm aux_state; path_elm *curpt=path_pt; decl_elm *tmpaux; mile parval;
  boolean tmpvax=vax_flg; vax_flg=F;
  state_save((state_elm *)&aux_state);
  state_restore((state_elm *)&aux_bot->par_state);
  tmpaux=ilk_pt->aux; ilk_pt->aux=NULL; /* disable par aux */
  parval=partition_val(curpt,coors,sub_num);
  ilk_pt->aux=tmpaux;
  state_update((state_elm *)&aux_bot->par_state);
  state_restore((state_elm *)&aux_state);
  vax_flg=tmpvax;
  return(parval);
}

mile part_selector(coors,sub_num,reperr)
  int coors,sub_num; boolean reperr;
{ mile parval;
  if(flags.state)
    {path_error(PER_GEN,path_pt,path_pt,WRN_VAR_SEL); return(-SEX_UF);}
  if(yyary_pt->suspend) return(-SEX_CL);
  if(selec && ((*selec)&SEL_DIRTY)) {yyary_pt->suspend=T; return(-SEX_CL);}
  if(selec) (*selec)|=SEL_DIRTY;
  if(ilk_pt)
    {if(ilk_pt->curpt->outok==AUX_AOTOK) /* special aux Op */
       parval=parent_part(coors,sub_num);
     else
       parval=partition_val(path_pt,coors,sub_num);
    }
  else
    {path_error(PER_GEN,path_pt,path_pt,WRN_NET_PAR);
     if(selec) (*selec)&=0xdf; /* mask out dirty bit */
     parval=(mile)selector_val((int)SEL_AND,(int)SIN_DEF,(int)1,(mile)0);
    }
  if(parval==0&&reperr) path_error(PER_GEN,path_pt,path_pt,WRN_PAR_EMP);
  return(parval);
}

/* Meaning of List operators [c] ([-c] reverses list):
*  Note lops can have coors & subnum where indicated by x,
*  these are combined with defaults
* [*] = wild array, dummy
* [&x] = [0..($&x-1)] provided ($&x-1)>0, else [0..0]
* [|x] = [0..($|x-1)] provided ($|x-1)>0, else [0..0]
* [^x] = [$^x..$_x]
* [@x] = [(1+$^.x)..(2+$^.x)] add one to top
* [_x] = [$_x..$^x]
* [%x] = [0..($%x-1)] provided ($%x-1)>0, else [0..0]
* [:x] = [0..($:x-1)] provided ($:x-1)>0, else [0..0]
* [=x] = takes corresponding ary str, evals & retunrs list as result
* [.x] = returns DS ary if in Ds, otherwise returns [&x]
* [!x] = takes corresponding ary as result
* [+x] = [(1+$^.x)..(1+$^.x+$!-1)] append
* [?x] = [+x] if +oelm or var, or [.x] if -oelm
*/
void list_operator(ltype,coors,sub_num,cstype)
  int ltype,coors,sub_num; mile cstype;
{ mile ia=0,ib=0,a=0,b=0,c=0; int err=WRN_LOP_ZER;
  boolean isbin,addab=F,reverse=(boolean)((coors&SIN_REV)!=0);
  if(yyary_pt->suspend) return;
  if(selec && ((*selec)&SEL_DIRTY)) {yyary_pt->suspend=T; return;}
  switch(ltype)
    {case LOP_AND: ib=(-1);
       b=(mile)selector_val((int)SEL_AND,coors,sub_num,(mile)0); break;
     case LOP_PIP: ib=(-1);
       b=(mile)selector_val((int)SEL_PIP,coors,sub_num,(mile)0); break;
     case LOP_PERC: ib=(-1);
       b=(mile)selector_val((int)SEL_PERC,coors,sub_num,(mile)0); break;
     case LOP_COL: ib=(-1);
       b=(mile)selector_val((int)SEL_COL,coors,sub_num,(mile)0); break;
     case LOP_HAT: case LOP_SUB:
       a=(mile)selector_val((int)SEL_SUB,coors,sub_num,(mile)0); selec=NULL;
       b=(mile)selector_val((int)SEL_HAT,coors,sub_num,(mile)0);
       if(ltype==LOP_SUB) reverse=(boolean)!reverse;
       break;
     case LOP_QST: /* if +oelm --> [+] else [.] */
       isbin=(boolean)((!path_pt->decl_pt->declnum)
              && (path_pt->decl_pt->sst_pos==VIR_BIN));
       if(!flags.state && ilk_pt
          && (isbin||((ilk_pt->curpt->combop&COMB_PLMI)==0))) goto append;
     case LOP_DOT: declary: /* fetch cur ds ary if inds or return [&] */
       b=(mile)selector_val((int)SEL_LOP_DOT,coors,sub_num,cstype);
       if(b==(-SEX_DS)) return; /* lop was dealt with by selector_val */
       if(b==(-SEX_NL)) return; /* unbound ary was empty */
       ib=(-1);
       break;
     case LOP_XS_HAT_ADD:
     case LOP_PLUS: append:
       if(flags.state) goto declary;
       addab=T; ia=1; ib=(-1); err=WRN_QST_ZER;
       a=(mile)selector_val((int)SEL_HAT,(int)(coors|SIN_ARY),sub_num,(mile)0);
       selec=NULL;
       if(ltype==LOP_XS_HAT_ADD) b=1; /* just adding one element */
       else b=part_selector((int)SIN_DEF,(int)1,F);
       if(b<1)
         {ib=0;
          if(b==(-3)) ; /* opath eval'd already, treat as single leaf */
          else if(b==(-2)) /* part level empty, but not exceeded */
            path_error(PER_GEN,path_pt,path_pt,WRN_PAR_EXD);
          else path_error(PER_GEN,path_pt,path_pt,WRN_PAR_ZER);
          b=0;
         }
       if(a<0) {a=0; ia=0;}
       break;
     case LOP_EXC:
       selector_val((int)SEL_LOP_EXC,coors,sub_num,cstype); return;
     case LOP_EQU:
       selector_val((int)SEL_LOP_EQU,coors,sub_num,cstype); return;
     default: /* LOP_STAR etc */ signal_error(EXIT_INT_ERR,127);
    }
  if(yyary_pt->suspend) return;
  if(a<0) {path_error(PER_GEN,path_pt,path_pt,WRN_SEL_UNL); a=0; ia=0;}
  if(b<0) {path_error(PER_GEN,path_pt,path_pt,WRN_SEL_UNL); b=0; ib=0;}
  b+=ib; a+=ia;
  if(a<0) {path_error(PER_GEN,path_pt,path_pt,err); a=0;}
  if(b<0) {path_error(PER_GEN,path_pt,path_pt,err); b=0;}
  if(addab) b+=a;
  if(reverse) {c=a; a=b; b=c;}
  lst_ary_list((reals)a,(reals)b,cstype,F);
}

/* returns path dip, symbol or type string */
char *path_selector(sel_type,coors,sub_num)
  int sel_type,coors,sub_num;
{ path_elm *ppt; stub_elm *spt; char *apt;
  boolean inds,rfdary,getypes,getindx;
  if(yyary_pt->suspend) return(NULL);
  if(selec && ((*selec)&SEL_DIRTY)) {yyary_pt->suspend=T; return(NULL);}
  if(selec) (*selec)|=SEL_DIRTY;
  coors|=SIN_ARY; coors^=SIN_ARY; /* mask out SIN_ARY op */
  getypes=(boolean)(sel_type==SEL_XS_MINUS);
  getindx=(boolean)(sel_type==SEL_XS_STAR);
  sel_type=SEL_XS;
  *path_str_buff='\0';
  if(ppt=get_level(path_pt,sel_type,coors,sub_num,
                   (stub_elm **)&spt,(boolean *)&inds,(boolean *)&rfdary))
    {apt=path_str_buff; if(!getindx) {*apt='"'; ++apt;}
     node_adrs(apt,ppt,getindx,getypes,F);
     apt+=strlen(apt);
     if(!getindx) {*apt='"'; ++apt; *apt='\0';}
    }
  return(cir_save(path_str_buff));
}

/* select ref path to use when doing a path selector
* chk for abs,rel & ~~ paths etc: ~---|~~---|.\---|..\---|\---|c|*|?
* path selector str of form "($[!|?][$$...]path[sel])..."
* returns start of path to add to ppt in strpt
*/
path_elm *selpath_ref(strpt)
  char **strpt;
{ path_elm *cpt=NULL,*ppt=NULL; char *ostr=(*strpt);
  boolean firsterr=T;
  if(!ostr || !*ostr) signal_error(EXIT_INT_ERR,128);
  ostr+=2;
  switch(*ostr)
    {case '?': ++ostr;
       if(!inpt) path_error(PER_GEN,path_pt,path_pt,WRN_STIL_UNF,"?");
       else ppt=inpt;
       break;
     case '!': ++ostr;
       if(!outpt) path_error(PER_GEN,path_pt,path_pt,WRN_STIL_UNF,"!");
       else ppt=outpt;
       break;
     default:;
    }
  if(!ppt)
    {cpt=path_pt; ppt=cpt; if(cpt->tail) cpt=cpt->tail;
     while(ppt->parent) ppt=ppt->parent;}
  if(!cpt) {cpt=ppt; while(cpt->tail) cpt=cpt->tail;}
  if(*ostr=='$') /* move to parent label */
    {++ostr; if(*ostr=='$') ++ostr;
     *strpt=ostr;
     return(glob_ref(cpt,ostr));
    }
  if(*ostr=='~')
    {++ostr;
     if((*ostr && (*ostr!='~') && (check_path(cpt)!=PATH_TER)))
       path_error(PER_GEN,path_pt,path_pt,WRN_STIL_UNF,"~");
     else if(*ostr=='~') {++ostr; cpt=node_path(cpt);}
    }
  switch(*ostr)
    {case '/': ++ostr; cpt=ppt; break;
     case '#': break;
     case '.': ostr+=2; if(*ostr!='@') break;
     case '@':
       while(*ostr=='@')
         {++ostr;
          if(cpt->parent) cpt=cpt->parent;
          else if(firsterr)
            {path_error(PER_GEN,path_pt,path_pt,WRN_SFLO_OFL); firsterr=F;}
         }
       ++ostr; break;
     default: /* path member to tack on cur path pos */
      /*
       if(cpt->tail) cpt=cpt->tail;
       else path_error(PER_GEN,cpt,path_pt,path_pt,WRN_SNO_TAIL);
      */ ;
    }
  *strpt=ostr;
  if(!cpt) signal_error(EXIT_INT_ERR,129);
  return(cpt);
}

/* evaluate selector val at path
*  pstr="($path[sel])..." : result in exp_value:exp_type*/
void pathsel_val(pstr)
  char *pstr;
{ path_elm *ppt=path_pt,*tmppt=path_pt,*tmpinpt=inpt; ds_sea_elm *tmpool;
  char *apt,chr; int lvl,slot1; byte lab_id; boolean dumflg;
  flag_elm tmpflg; tmpflg=flags;
  flags.inp=T; flags.info=T; flags.ary=F; flags.inout=F;
  flags.pamat=F; flags.last=F; flags.state=F; flags.fork=F; flags.wild=F;
  tmpool=ds_pool;
  if(ds_pool) slot1=push_stub(ds_stub);
  push_io(); set_io_str(pstr);
  if(skip_pair('(',')')!=')') signal_error(EXIT_INT_ERR,130);
  apt=get_io_pos()-1; pop_io();
  chr=(*apt); *apt='\0';
  ppt=selpath_ref(&pstr); lvl=rel_level(ppt); inpt=dup_path(ppt,T);
  path_pt=rel_path(inpt,lvl); blank_pelm(path_pt);
  if(path_pt->tail||!*pstr) signal_error(EXIT_INT_ERR,131);
  lab_id=stack_str(path_pt,pstr); *apt=chr;
  if(ds_pool)
    {ds_stub=find_dspt(path_pt,(boolean *)&dumflg); flags.build=dumflg;}
  exp_value.val.dd=(double)0.0; exp_value.typ=TYPE_UNKNOWN;
  do_nxt_decl();
  ds_pool=tmpool; if(ds_pool) ds_stub=pop_stub(slot1);
  yank_lab(path_pt,lab_id); free_path(path_pt);
  path_pt=tmppt; inpt=tmpinpt; flags=tmpflg;
}
