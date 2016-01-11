/* Name: nial2.c   Type: nial output routine  Date: 30 March 89
*   NeSeL Network Specification Language Compiler
*/
#include "globals2.h"
#include "iolo2.h"
#include "iohi2.h"
#include "nial2.h"
#include "nsltwo2.h"
#include "pathlo2.h"
#include "conxlo2.h"

/* Routines to handle partial net node data-structures stored on disk */
#define nal_seg_max 1000 /* size of nal free block list */
#define nal_page_size 64 /* nal disk blocks allocated in multiples of this */
#define syssize (sizeof(foot)+sizeof(door)+sizeof(yard))

#define ds_page_size (40*(sizeof(stub_elm)+2*sizeof(long)))

#define ofs_inc 10 /* default inc for stub stack offsets */

typedef word door; /* size of a nal block in multiples of nal_page_size */

/* nalfp free list element structure */
typedef struct _nal_elm
{ door size;
  foot dadrs;
} nal_elm;

word ds_sea_max=10; /* default size of ds buffer */
/* a ds_sea size of 20 to 50 would be better mem space permitting */
ds_sea_elm *ds_sea; /* Node data structure queue/buffer details */
word *ds_lilo; /* lilo queue of ds_sea_elm handles */

/* list of nalfp free blocks, not including eof */
nal_elm nal_free[nal_seg_max];
int nal_seg_num; /* cur num of free blks in list */
yard nal_node_max; /* largest node number written to dsifp index */
foot dsindex=0; /* base adrs of dsindex in nalfp */

/* The partial net node list (dsik_list) is stored on disk (dsifp).
* It is a list of foot's into the partial net file (nalfp)
* A nodes nalfp foot adrs is held in dsikfp at nodenum*sizeof(foot)
*/

/* write data bytes to binary file */
#define fput_data(fpt,adrs,len) fwrite(adrs,len,(size_t)1,fpt)

/* read data bytes from binary file */
#define fget_data(fpt,adrs,len) fread(adrs,len,(size_t)1,fpt)

void ofs_increase(dpt)
  ds_sea_elm *dpt;
{ word *wpt,slot; int num,oldtop;
  if(dpt->ofs_list) num=lst_num(dpt->ofs_list);
  else {num=0; dpt->ofs_free=0;}
  oldtop=num; num+=(2*ofs_inc);
  dpt->ofs_list=lmore(dpt->ofs_list,2*ofs_inc,TYPE_USSHORT,word);
  wpt=dpt->ofs_list+oldtop;
  for(slot=(word)(oldtop+2);slot<=(word)num;slot+=(word)2) 
     {*wpt=slot; ++wpt; *wpt=(word)0; ++wpt;}
}
void ofs_clear(dpt) ds_sea_elm *dpt;
{ dpt->ofs_free=0; if(dpt->ofs_list) lst_num(dpt->ofs_list)=0;
  ofs_increase(dpt);
}
void nal_initialise()
{ nal_elm *npt=nal_free+nal_seg_max; ds_sea_elm *dpt;
  word *lpt,num=ds_sea_max;
  ds_sea=malmem(ds_sea_elm,ds_sea_max); dpt=ds_sea+ds_sea_max;
  ds_lilo=malmem(word,ds_sea_max); lpt=ds_lilo+ds_sea_max;
  while(npt>nal_free) {--npt; npt->dadrs=(foot)0; npt->size=(door)0;}
  while(dpt>ds_sea)
    {--dpt; dpt->refs=0; dpt->nodenum=0; dpt->modified=F;
            dpt->pool_handle=num;
            dpt->ds_base=NULL; dpt->ds_end=NULL; dpt->ds_size=0;
            dpt->ofs_list=NULL; dpt->ofs_free=0;
     --lpt; *lpt=num; --num;}
  nal_seg_num=0; nal_node_max=0;
}

void nal_delfre(npt)
  nal_elm *npt;
{ nal_elm *tpt=nal_free+nal_seg_num;
  while(npt<tpt) {*npt=(*(npt+1)); ++npt;}
  --nal_seg_num;
}
void nal_incfre(npt)
  nal_elm *npt;
{ nal_elm *tpt=nal_free+nal_seg_num;
  while(tpt>npt) {*tpt=(*(tpt-1)); --tpt;}
  ++nal_seg_num;
}
/* find a free nal block >= size */
nal_elm *nal_fnd_blk(size)
  door size;
{ door cc,dd=0; nal_elm *npt,*tpt,*lpt=NULL;
  npt=nal_free; tpt=npt+nal_seg_num;
  while(npt<tpt) /* this could be a binary search */
    {cc=npt->size;
     if(cc==size) return(npt); /* ??? npt ??? */
     if(cc>size)  /* find smallest blk in list that is bigger than size */
       {if(!dd) dd=(door)(cc+1);
        if(cc<dd) {lpt=npt; dd=cc;}
       }
     ++npt;
    }
  return(lpt);
}

/* free a nal block given its true base adrs & seg size
*  Its up to caller to update dsik indx
*/
void nal_frblk(base,size)
  foot base; door size;
{ foot dadrs; nal_elm *mpt,*npt,*lpt,*tpt; 
  if(base==(foot)0 || size==(door)0) return;
  dadrs=base+(foot)size*nal_page_size;
  npt=nal_free; tpt=nal_free+nal_seg_num; lpt=NULL; mpt=NULL;
  while(npt<tpt)
    {if(npt->dadrs==dadrs) mpt=npt;
     else if((npt->dadrs+(foot)npt->size*nal_page_size)==base) lpt=npt;
     ++npt; }
  if(lpt && mpt)
    {if(lpt>mpt) {mpt->dadrs=lpt->dadrs; tpt=lpt; lpt=mpt; mpt=tpt;}
     lpt->size+=(door)(size+mpt->size); nal_delfre(mpt);}
  else if(lpt) lpt->size+=(door)size;
  else if(mpt) {mpt->dadrs=base; mpt->size+=(door)size;}
  else
    {lpt=nal_fnd_blk(size);
     if(lpt) nal_incfre(lpt);
     else {lpt=nal_free+nal_seg_num; ++nal_seg_num;}
     if(nal_seg_num>=nal_seg_max)
       {signal_error(EXIT_OSPACE,"nal free list overflow");
        /* should compress then continue... */
       }
     lpt->dadrs=base; lpt->size=size;
    }
}

/* pos dsifp at nodes index entry & return adrs */
foot ds_node_indx(nodenum)
  yard nodenum;
{ foot dindx=(foot)nodenum*sizeof(foot);
  if(nodenum>nal_node_max) return(0);
  fposeof(dsifp); if(dindx>ftell(dsifp)) return(0);
  fmoveto(dsifp,dindx);
  return(dindx);
}

/* put a nodenum & it's nal block pt to dsik index, update nalfp */
void ds_put_indx(nodenum,dadrs,space,size)
  yard nodenum; foot dadrs,space; door size;
{ foot sum,fpt,dindx=(foot)nodenum*sizeof(foot),zerofoot=0;
  if(nodenum>nal_node_max)
    {++nal_node_max;
     fpt=(foot)(nal_node_max)*sizeof(foot);
     fmoveto(dsifp,fpt); sum=0;
     while(nal_node_max<nodenum)
       {sum+=sizeof(foot); if(sum>(BUFSIZ/2)) {fflush(dsifp); sum=0;}
        fput_data(dsifp,(foot *)&zerofoot,sizeof(foot)); ++nal_node_max;
       }
     fflush(dsifp);
    }
  else fmoveto(dsifp,dindx);
  fput_data(dsifp,(foot *)&dadrs,sizeof(foot));
  fflush(dsifp);
  /* write sys info to nodes nalfp block */
  fmoveto(nalfp,dadrs);
  fput_data(nalfp,(foot *)&space,sizeof(foot));
  fput_data(nalfp,(door *)&size,sizeof(door));
  fput_data(nalfp,(yard *)&nodenum,sizeof(yard));
  fflush(nalfp);
}

/* get a nodes nal block pt from dsik index: positions nalfp at block */
foot ds_get_indx(nodenum,spacept,sizept)
  yard nodenum; foot *spacept; door *sizept;
{ foot dindx; yard node; *sizept=0;*spacept=0;
  if(!(dindx=ds_node_indx(nodenum))) return(0);
  fget_data(dsifp,(foot *)&dindx,sizeof(foot));
  /* read sys info from nodes nalfp block */
  if(!dindx) return(0);
  fmoveto(nalfp,dindx);
  fget_data(nalfp,(foot *)spacept,sizeof(foot));
  fget_data(nalfp,(door *)sizept,sizeof(door));
  fget_data(nalfp,(yard *)&node,sizeof(yard));
  if(node!=nodenum) signal_error(EXIT_INT_ERR,61);
  return(dindx);
}

/* allocate a block of disfp disk space, add sys inf
* update dsik indx. Note nalfp is positioned ready for caller
*/
void nal_alloc(nodenum,space)
  yard nodenum; foot space;
{ foot sum,dadrs,newspace; door size,cc; nal_elm *lpt; char quest='?';
  size=(door)(1+(door)((2+space+syssize)/nal_page_size));
  dadrs=ds_get_indx(nodenum,(foot *)&newspace,(door *)&cc);
  if(dadrs) {if(cc==size) goto gotblock; else nal_frblk(dadrs,cc);}
  newspace=(foot)size*nal_page_size;
  if(lpt=nal_fnd_blk(size))
    {cc=lpt->size; dadrs=lpt->dadrs;
     if(lpt->size==size) nal_delfre(lpt); /*remove blk entry from free list*/
     else
       {lpt->size-=(door)size;
        lpt->dadrs+=(size*nal_page_size);} /* make free block smaller */
    }
  else /* make a block at eof */
    {fposeof(nalfp); dadrs=ftell(nalfp); sum=0;
     while(newspace--)
       {++sum; if(sum>(BUFSIZ/2)) {fflush(nalfp); sum=0;}
        fput_data(nalfp,(char *)&quest,(size_t)1);
       }
     fflush(nalfp);
    }
  gotblock:
  ds_put_indx(nodenum,dadrs,space,size);
}
#ifdef NSLDEBUG
void bad_pt(pt) stub_elm *pt;
{ printf("Bad pt:%ld\n",pt);
}
void chk_pt(pt) stub_elm *pt;
{ if(pt<(stub_elm *)0xffffL)
  bad_pt(pt);
}
#endif
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* returns the size of stub elm & its value & backtracking offset word */
size_t stub_size(spt)
  stub_elm *spt;
{ siby styp;
#ifdef NSLDEBUG
chk_pt(spt);/*DEBUG*/
#endif
  if(spt->conds==NISSTR) goto do_str;
  styp=spt->type; loop:
  switch(styp)
    {case TYPE_SHORT: case TYPE_USSHORT: case TYPE_INT: case TYPE_USINT:
       return(SIZ_SIP_INT);
     case TYPE_LONG: case TYPE_USLONG: return(SIZ_SIP_LONG);
     case TYPE_FLOAT: return(SIZ_SIP_FLOAT);
     case TYPE_DOUBLE: return(SIZ_SIP_DOUB);
     case TYPE_CHAR: case TYPE_USCHAR: case SIP_STR: do_str:
       if(spt->isdef) return(SIZ_SIP_STR+(ds_value(spt)->str.len));
       return(SIZ_SIP_STR);
     case SIP_NODE:
       return(SIZ_SIP_NODE+(ds_value(spt)->node.di_num)*sizeof(di_elm));
     case TYPE_PT: return(SIZ_SIP_PT);
     case TYPE_NODE: case TYPE_STRUCT: case TYPE_ARB: return(SIZ_SIP_ION);
     case TYPE_VOID: case TYPE_UNKNOWN: case SIP_CONX:
     case TYPE_UNION: case TYPE_NET: return(SIZ_SIP);
     default: if(!(styp&STAR_MASK)) signal_error(EXIT_INT_ERR,62);
       if(!spt->info.data.size && (styp&STAR_MASK)==STAR_ONE)
         {styp&=STAR_TYPE; goto loop;}
    }
  return(SIZ_SIP_PT);
}

size_t stub_base(stype,isua)
  int stype; boolean isua;
{ loop:
  switch(stype)
    {case TYPE_SHORT: case TYPE_USSHORT: case TYPE_INT: case TYPE_USINT:
       return(SIZ_SIP_INT);
     case TYPE_LONG: case TYPE_USLONG: return(SIZ_SIP_LONG);
     case TYPE_FLOAT: return(SIZ_SIP_FLOAT);
     case TYPE_DOUBLE: return(SIZ_SIP_DOUB);
     case TYPE_CHAR: case TYPE_USCHAR: case SIP_STR: return(SIZ_SIP_STR);
     case SIP_NODE: return(SIZ_SIP_NODE);
     case TYPE_PT: return(SIZ_SIP_PT);
     case TYPE_NODE: case TYPE_STRUCT: case TYPE_ARB: return(SIZ_SIP_ION);
     case TYPE_VOID: case TYPE_UNKNOWN: case SIP_CONX:
     case TYPE_UNION: case TYPE_NET: return(SIZ_SIP);
     default: if(!(stype&STAR_MASK)) signal_error(EXIT_INT_ERR,63);
       if(isua && (stype&STAR_MASK)==STAR_ONE)
         {stype&=STAR_TYPE; goto loop;}
    }
  return(SIZ_SIP_PT);
}

stub_elm *ds_skp_to_tail(spt)
  register stub_elm *spt;
{
#ifdef NSLDEBUG
chk_pt(spt);/*DEBUG*/
#endif
  if(spt->hassubpt || spt->hasarypt)
    {word num=(word)(spt->hassubpt+spt->hasarypt);
     do{--num;
        spt=ds_next(spt); if(!spt) signal_error(EXIT_INT_ERR,64);
        num+=(word)(spt->hassubpt+spt->hasarypt+spt->hastail);
       }while(num);
    }
  return(ds_next(spt));
}

stub_elm *ds_skp_to_arypt(spt)
  register stub_elm *spt;
{
#ifdef NSLDEBUG
chk_pt(spt);/*DEBUG*/
#endif
  if(spt->hassubpt)
    {word num=1;
     do{--num;
        spt=ds_next(spt); if(!spt) signal_error(EXIT_INT_ERR,65);
        num+=(word)(spt->hassubpt+spt->hasarypt+spt->hastail);
       }while(num);
    }
  return(ds_next(spt));
}

/* get a stubs parent stub by moving up ds & counting pt bits */
stub_elm *ds_parent(spt)
  register stub_elm *spt;
{
#ifdef NSLDEBUG
chk_pt(spt);/*DEBUG*/
#endif
  if(spt->hasparent)
    {int num=0;
      do{((char *)spt)-=(*(((word *)spt)-1)); /* move to prior stub */
         num+=(1-spt->hassubpt - spt->hasarypt - spt->hastail);
         if(num>0 && !spt->hasparent) signal_error(EXIT_INT_ERR,66);
        }while(num>0);
      return(spt);
    }
  return(NULL);
}

/* get a stubs arypt stub by moving down ds & counting pt bits */
stub_elm *ds_arypt(spt)
  register stub_elm *spt;
{
#ifdef NSLDEBUG
chk_pt(spt);/*DEBUG*/
#endif
  if(spt->hasarypt)
    {if(spt->hassubpt)
       {word num=1;
        do{--num;
           spt=ds_next(spt); if(!spt) signal_error(EXIT_INT_ERR,67);
           num+=(word)(spt->hassubpt+spt->hasarypt+spt->hastail);
          }while(num);
       }
     return(ds_next(spt));
    }
  return(NULL);
}

stub_elm *ds_tail(spt)
  register stub_elm *spt;
{
#ifdef NSLDEBUG
chk_pt(spt);/*DEBUG*/
#endif
  if(spt->hastail)
    {if(spt->hassubpt || spt->hasarypt)
       {word num=(word)(spt->hassubpt+spt->hasarypt);
        do{--num;
           spt=ds_next(spt); if(!spt) signal_error(EXIT_INT_ERR,68);
           num+=(word)(spt->hassubpt+spt->hasarypt+spt->hastail);
          }while(num);
       }
     return(ds_next(spt));
    }
  return(NULL);
}

byte *ds_level(spt)
  register stub_elm *spt;
{
#ifdef NSLDEBUG
chk_pt(spt);/*DEBUG*/
#endif
  if(spt->conds==NISSTR) goto do_str;
#define dsx(pp,ii) (byte *)(((char *)pp)+ii-3)
  switch(spt->type)
    {case TYPE_SHORT: case TYPE_USSHORT: case TYPE_INT: case TYPE_USINT:
       return(dsx(spt,SIZ_SIP_INT));
     case TYPE_LONG: case TYPE_USLONG: return(dsx(spt,SIZ_SIP_LONG));
     case TYPE_FLOAT: return(dsx(spt,SIZ_SIP_FLOAT));
     case TYPE_DOUBLE: return(dsx(spt,SIZ_SIP_DOUB));
     case TYPE_CHAR: case TYPE_USCHAR: case SIP_STR: do_str:
       if(spt->isdef) return(dsx(spt,SIZ_SIP_STR+(ds_value(spt)->str.len)));
       return(dsx(spt,SIZ_SIP_STR));
     case TYPE_PT: break;
     case TYPE_VOID: case SIP_CONX: case TYPE_UNION: case TYPE_NET:
     case TYPE_NODE: case TYPE_STRUCT: case TYPE_ARB: case SIP_NODE:
       signal_error(EXIT_INT_ERR,69);
     default: if(!(spt->type&STAR_MASK)) signal_error(EXIT_INT_ERR,70);
    }
  return(dsx(spt,SIZ_SIP_PT));
}

/* adj pool's offsets if any in use */
void ds_adj_ofs(basofs,adjofs)
  word basofs; int adjofs;
{ word num,*opt=ds_pool->ofs_list;
 if(opt)
   {num=(word)lst_num(opt);
    while(num-=2)
      {if(!*opt) /* only adjust offset if code word is zero */
         {++opt; if(*opt>=basofs) (*opt)=(word)((int)*opt+adjofs);
          ++opt;
         }
       else opt+=2;
      }
   }
}

/* save stub pt on ds_pool ofs list: return a handle to it */
/* NOTE the error chks on stub stack access only needed for debugging */
int push_stub(spt)
  char *spt;
{ word *opt,code; int num;
  if(!ds_pool) signal_error(EXIT_INT_ERR,71);
  if(!spt) code=1; /* code indicates if a null pt was pushed */
  else
    {code=0;
     if(spt<ds_pool->ds_base || spt>ds_pool->ds_end)
       {signal_error(EXIT_INT_ERR,72);}
    }
  opt=ds_pool->ofs_list; num=(int)ds_pool->ofs_free;
  if(!opt||(num&&(num==lst_num(opt))))
    {ofs_increase(ds_pool); opt=ds_pool->ofs_list; num=(int)ds_pool->ofs_free;}
  opt+=num;
  ds_pool->ofs_free=(word)(*opt); *opt=code; ++opt;
  if(code) *opt=0;
  else *opt=(word)(spt-ds_pool->ds_base);
  return(num); /* num is slot index of saved offset */
}
/* calls to this func only needed during check for internal errors */
void ofs_chk(num) int num;
{ word *opt;
  if(!ds_pool || num<0 || num&1) signal_error(EXIT_INT_ERR,73);
  opt=ds_pool->ofs_list;
  if(!opt||num>lst_num(opt)) signal_error(EXIT_INT_ERR,74);
}

stub_elm *pop_stub(num)
  int num;
{ word code,*opt; stub_elm *spt;
  ofs_chk(num);
  opt=ds_pool->ofs_list+num;
  code=(*opt); *opt=(word)ds_pool->ofs_free; ++opt;
  ds_pool->ofs_free=(word)num;
  if(code) spt=NULL; else spt=(stub_elm *)(ds_pool->ds_base+(*opt));
  *opt=0;
  return(spt);
}

/* save stub at indicated slot */
void update_stub(spt,num)
  char *spt; int num;
{ word *opt,code;
  ofs_chk(num);
  opt=ds_pool->ofs_list+num;
  if(!spt) code=1; /* code indicates if a null pt was pushed */
  else
    {code=0;
     if(spt<ds_pool->ds_base || spt>ds_pool->ds_end)
       signal_error(EXIT_INT_ERR,76);
    }
  if(*opt && *opt!=1) signal_error(EXIT_INT_ERR,77);
  *opt=code; ++opt;
  if(code) *opt=0;
  else *opt=(word)(spt-ds_pool->ds_base);
}

/* get stub val from stack, but leave on stack */
stub_elm *restore_stub(num)
  int num;
{ word code,*opt;
  ofs_chk(num);
  opt=ds_pool->ofs_list+num;
  code=(*opt);
  if(code && code!=1) signal_error(EXIT_INT_ERR,79);
  if(code) return(NULL);
  ++opt;
  return((stub_elm *)(ds_pool->ds_base+(*opt)));
}

/* move from s2 to s1 space bytes
*  Note string.h memmove() func would do aswell if available */
void memdown(s1,s2,num)
  char *s1,*s2; size_t num;
{ while(num--) *s1++ = *s2++;
}
/* move from s2 to s1 space bytes
*  Note string.h memmove() func would do aswell if available */
void memup(s1,s2,num)
  char *s1,*s2; size_t num;
{ s2+=num; s1+=num;
  while(num--) *(--s1) = *(--s2);
}

/* check ds has enough room for size more bytes
*  realloc if needed, adjust ds_pool & user pointer upt
*/
char *ds_check(upt,size)
  char *upt; int size;
{ char *bpt=ds_pool->ds_base; size_t ds_size=ds_pool->ds_size,dd;
  word uofs,dofs;
  if((ds_pool->ds_end+size+10)>=(bpt+ds_size))
    {if(upt<bpt) signal_error(EXIT_INT_ERR,80);
     uofs=(word)(upt-bpt); dofs=(word)(ds_pool->ds_end-bpt);
     dd=ds_size; ds_size+=(size+ds_page_size);
     if(bpt) upt=relmem(char,sizeof(char)*(4+ds_size),bpt,sizeof(char)*(4+dd));
     else upt=malmem(char,4+ds_size);
     ds_pool->ds_size=ds_size;
     ds_pool->ds_base=upt;
     ds_pool->ds_end=upt+dofs;
     upt+=uofs;
    }
 return(upt);
}

/* NOTE for speed two seperate routines used for ds_mem moving:- */

/* create more space, for a var say, in ds cur_poll at bpt
*  returns bpt. Note may move pool via realloc, so bpt may be different... */
char *ds_meminc(bpt,adjofs)
  char *bpt; int adjofs;
{ char *tpt; size_t space;
  bpt=ds_check(bpt,adjofs);
  space=(size_t)(1+ds_pool->ds_end-bpt);  /* bytes to move */
  tpt=bpt+adjofs; /* get new base adrs of stubs to move from spt */
  memup(tpt,bpt,space); /* move from bpt to tpt space bytes */
  ds_pool->ds_end+=adjofs;
  ds_adj_ofs((word)(bpt-ds_pool->ds_base),adjofs);
  return(bpt);
}

/* decrease space, when freeing a str or node var say
*  bpt is base of area to free from, adjofs is number bytes to free */
void ds_memdec(bpt,adjofs)
  char *bpt; int adjofs;
{ char *tpt; size_t space;
  tpt=bpt+adjofs; /* get adrs of stubs to move from */
  space=(size_t)(1+ds_pool->ds_end-tpt);  /* bytes to move */
  memdown(bpt,tpt,space); /* move from rpt to bpt space bytes */
  ds_pool->ds_end-=adjofs;
  ds_adj_ofs((word)(bpt-ds_pool->ds_base),(-adjofs));
}

void set_stub_size(cpt,size)
  char *cpt; word size;
{ cpt+=size-2;
  *((word *)cpt)=(word)size;
}

/* initialise a new stub: sets backtracking word as well */
void zap_stub(spt,size,stype,hasparent)
  stub_elm *spt; int size,stype; boolean hasparent;
{ char *cpt=(char *)spt;
  spt->hasparent=hasparent; spt->hassubpt=F; spt->hasarypt=F; spt->hastail=F;
  spt->isdef=F; spt->isvar=F; spt->conds=NISNUL;
  spt->type=(siby)stype;
  spt->info.data.size=0; spt->info.data.elm=0;
  if(size>=SIZ_SIP_ION)
    {ds_value(spt)->conx.in_num=0; ds_value(spt)->conx.out_num=0;}
  else if(size>=SIZ_SIP_STR) /* Note, this sets initial string lengths to 0*/
     ds_value(spt)->str.len=0;
  cpt+=size-2; *((word *)cpt)=(word)size; /* set backtracking offset */
}

/* create space for a new stub in ds_pool,
*  on entry spt is parent stub, child indicates relation of new stub to parent
*  stype is child type, adjofs is extra bytes required for var str etc
*  returns base of new stub. Note realloc may have changed other fast_hands
*/
stub_elm *new_stub(spt,adjofs,stype,child,isua)
  stub_elm *spt; int adjofs,stype,child; boolean isua;
{ stub_elm *ppt; char *bpt,*tpt; size_t space; int phd; boolean hasparent;
  if(spt) /* get new stubs eventual adrs */
    {hasparent=T; phd=push_stub(spt);
     switch(child)
       {case DS_SUB: spt=ds_next(spt); break;
        case DS_ARY: spt=ds_skp_to_arypt(spt); break;
        default: /* DS_TAIL */ spt=ds_skp_to_tail(spt);
       }
    }
  else
    {spt=(stub_elm *)ds_pool->ds_base; hasparent=F;
     if(!spt) spt=(stub_elm *)ds_check(spt,(int)0); /* first stub */
    }
  adjofs+=stub_base(stype,isua);
  spt=(stub_elm *)ds_check(spt,adjofs); /* Note this may move pool */
  bpt=(char *)spt;
  tpt=bpt+adjofs; /* get new base adrs of stubs to move from spt */
  space=(size_t)(1+ds_pool->ds_end-bpt);  /* bytes to move */
  memup(tpt,bpt,space); /* move from bpt to tpt space bytes */
  ds_pool->ds_end+=adjofs;
  ds_adj_ofs((word)(bpt-ds_pool->ds_base),adjofs);
  /* now initialise stub */
  zap_stub(spt,adjofs,stype,hasparent);
  /* set parent pointer to stub, & stubs pointers to it's childs */
  if(hasparent)
    {ppt=pop_stub(phd);
     switch(child)
       {case DS_SUB:
          if(ppt->hassubpt) spt->hassubpt=T; else ppt->hassubpt=T; break;
        case DS_ARY:
          if(ppt->hasarypt) spt->hasarypt=T; else ppt->hasarypt=T; break;
        default: /* DS_TAIL */
          if(ppt->hastail) spt->hastail=T; else ppt->hastail=T;
       }
    }
  return(spt);
}

/* remove a spt & all its child nodes from ds,
*  adjust any pool offsets. Note ds_pool must be set... */
void rem_sip(spt)
  stub_elm *spt;
{ char *bpt=(char *)spt,*tpt; int num=0,space;
  if(!spt) return;
  for(;;) /* fetch first stub elm after those being removed */
    {num+=(spt->hassubpt+spt->hasarypt+spt->hastail);
     if(!num) break;
     --num; spt=ds_next(spt); if(!spt) signal_error(EXIT_INT_ERR,81);
    }
  tpt=(char *)ds_next(spt); /* adrs of first stub to keep */
  space=(int)(1+ds_pool->ds_end-tpt); /* num bytes to move */
  memdown(bpt,tpt,(size_t)space); /* move from tpt to bpt space bytes */
  space=(int)(bpt-tpt); /* offset to add to ds_end & pool offsets */
  ds_pool->ds_end+=space; /* NOTE space holds a negative number */
  ds_adj_ofs((word)(bpt-ds_pool->ds_base),space);
}

void ds_free(dpt)
  ds_sea_elm *dpt;
{ if(dpt->nodenum)
    {dpt->nodenum=0;
     if(dpt->ds_size)
       {nul_free(dpt->ds_base);
        dpt->ds_end=NULL;
        dpt->ds_size=0;}
    }
  dpt->refs=0; dpt->modified=F;
}

/* adjust the size of a stub to nn. Returns new pt to it.
* On entry, siz = current stub size */
stub_elm *adj_stub(spt,nn,siz)
  stub_elm *spt; int nn,siz;
{ int slot1,slot2; char *apt=(char *)&(ds_value(spt)->str);
  siz=nn-siz;
  slot1=push_stub(ds_stub);
  if(siz>0)
    {slot2=push_stub(spt); apt=ds_meminc(apt,siz); spt=pop_stub(slot2);}
  else if(siz<0) {siz=(-siz); ds_memdec(apt,siz);}
  ds_stub=pop_stub(slot1);
  set_stub_size(spt,nn);
  return(spt);
}

stub_elm *ds_put_str(spt,bpt,len)
  stub_elm *spt; char *bpt; word len;
{ char *apt; int size;
  size=(int)stub_base((int)spt->type,(boolean)(spt->info.data.size==0));
  spt=adj_stub(spt,(int)(SIZ_SIP_STR+len),size),
  spt->conds=NISSTR; spt->isdef=T;
  ds_value(spt)->str.len=len;
  apt=(char *)&ds_value(spt)->str.base;
  while(len--) *apt++ = *bpt++;
  return(spt);
}
/* remove string from stub & make it it's base length */
stub_elm *ds_rem_str(spt)
  stub_elm *spt;
{ int size=stub_base((int)spt->type,(boolean)(spt->info.data.size==0));
  spt=adj_stub(spt,size,(int)(SIZ_SIP_STR+ds_value(spt)->str.len));
  spt->conds=NISNUL;
  return(spt);
}
/* move to parent node */
stub_elm *move_to_node(spt) stub_elm *spt;
{ stub_elm *apt=spt;
  while(spt)
    {apt=spt; if(!(spt=ds_parent(apt))) break;
     if(apt==ds_subpt(spt)&&spt->type==TYPE_NODE) return(spt);
    }
  return(apt);
}

/*--------------------------------------------------------------------------*/
/* adjust count of UA size by +-aadd */
void adj_uasize(spt,aadd)
  stub_elm *spt; int aadd;
{ stub_elm *apt; boolean isconx;
  if(aadd==0 || !spt || !spt->hasparent) return;
  isconx=(boolean)(spt->type==SIP_NODE||spt->type==SIP_CONX);
  if(isconx)
    {do spt=ds_parent(spt); while(spt && spt->type==SIP_NODE);}
  else if(spt->type!=TYPE_ARB)
    {while(spt) /* move to parent ary root */
      {apt=spt; if(!(spt=ds_parent(apt))) return;
       if(apt==ds_subpt(spt))
         {if(spt->type==TYPE_ARB) break;
          /* if(spt->type!=apt->type) {spt=apt; break;} */
          spt=apt; break; /* DEBUG could only be this */
         }
       else if(apt==ds_tail(spt)) {spt=apt; break;}
      }
    }
  /* gets here if TYPE_ARB or SIP_NODE base or null */
  if(spt && spt->hasparent && (isconx||(spt->type==TYPE_ARB)))
    {spt->info.data.elm+=aadd;
     if(spt->info.data.elm<0) signal_error(EXIT_INT_ERR,82);
    }
}

/* prune a ds node node
*  returns T if spt node was pruned as well & didn't have an arypt */
boolean prune_node(spt)
  stub_elm *spt;
{ stub_elm *ppt; boolean haschild,iszerelm;
  if(!spt->hassubpt) spt->isdef=F;
  loop:
  if(!flags.state && !spt->isdef && (ds_value(spt)->conx.in_num==0)
     && (ds_value(spt)->conx.out_num==0))
    {if(spt->hassubpt) {rem_sip(ds_subpt(spt)); spt->hassubpt=F;}
     if(!spt->hasparent) signal_error(EXIT_INT_ERR,83);
     ppt=ds_parent(spt); iszerelm=(boolean)(ppt->type==TYPE_ARB);
     if((ds_tail(ppt)!=spt)&&(iszerelm||(ds_subpt(ppt)!=spt)))
       {/* spt is a node in an array of nodes, remove it*/
        /*Note arb has at least 2 elms in it, otherwise prune_arb gets it*/
        adj_uasize(spt,(-1));
        haschild=spt->hasarypt;
        if(ds_subpt(ppt)==spt) ppt->hassubpt=haschild;
        else ppt->hasarypt=haschild;
        spt->hasarypt=F;
        if(spt->hassubpt || spt->hastail) signal_error(EXIT_INT_ERR,84);
        rem_sip(spt);
        if(haschild) goto loop;
        return(T);
       }
    }
  return(F);
}

/* prune a ds array node if possible
*  returns spt */
void prune_arb(spt)
  stub_elm *spt;
{ if(!spt->hassubpt) spt->isdef=F;
  if(!flags.state && !spt->isdef && (ds_value(spt)->conx.in_num==0)
     && (ds_value(spt)->conx.out_num==0))
    {if(spt->hassubpt) {rem_sip(ds_subpt(spt)); spt->hassubpt=F;}
     spt->type=TYPE_NODE; /* convert back to node */
     spt->info.data.elm=0;
     if(spt->hasarypt) signal_error(EXIT_INT_ERR,85);
    }
}

/* initialise the compiler added structs: ins, outs & size
*  ppt is start of struct containing conx info to process
*  inspt is first info struct in which to store conx info
*  NOTE due to aux defs & conx book keekping we have to do this every time
*  ds is modified & written to disk
*/
void set_com_struct(ppt,inspt)
  stub_elm *ppt,*inspt;
{ stub_elm *outspt,*sizept; int value,ins_num=0,outs_num,size_num=0;
  /* total number of outputs for a node/struct is held in its parent */
  outspt=ds_parent(ppt); outs_num=ds_value(outspt)->conx.out_num;
  outspt=ds_tail(inspt); sizept=ds_tail(outspt);
  inspt->isdef=T; outspt->isdef=T; sizept->isdef=T;
  inspt=ds_subpt(inspt); outspt=ds_subpt(outspt); sizept=ds_subpt(sizept);
  do /* do all decls in ins,outs,size struct */
    {if(ppt->conds==NISINFO) /* reached end of struct */
       {ds_value(inspt)->nat=ins_num; inspt->isdef=T;
        ds_value(outspt)->nat=outs_num; outspt->isdef=T;
        ds_value(sizept)->nat=size_num; sizept->isdef=T;
        return;}
     else if((ppt->info.data.size==0
              || ppt->type==TYPE_NODE || ppt->type==TYPE_ARB)
             && ppt->hasparent && ppt->type!=SIP_NODE)
       {/* at a UA or node, get info */
        sizept->isdef=T; inspt->isdef=T; outspt->isdef=T;
        value=ppt->info.data.elm; size_num+=value;
        ds_value(sizept)->nat=value;
        if(ppt->type==TYPE_NODE || ppt->type==TYPE_ARB) /* chk at io */
          {value=ds_value(ppt)->conx.out_num;
           ds_value(outspt)->nat=value;
           value=ds_value(ppt)->conx.in_num;
          }
        else /* at a input only struct member, set num outs to 0 */
          ds_value(outspt)->nat=0;  /*fall thru & set input num to size of UA*/
        ins_num+=value;
        ds_value(inspt)->nat=value;
        inspt=ds_tail(inspt); outspt=ds_tail(outspt); sizept=ds_tail(sizept);
       }
     ppt=ds_tail(ppt);
    }while(ppt);
  signal_error(EXIT_INT_ERR,86); /*should never get here */
}

/* given a ds, prune & work out ins,outs,size info if necessary
*  rpt is cpt's tail */
char *ds_prune(cpt)
  char *cpt;
{ stub_elm *spt,*ppt; siby stype; boolean notinit=T;
  ppt=(stub_elm *)cpt;
  do{spt=(stub_elm *)cpt; stype=spt->type;
     if(stype==TYPE_ARB) {prune_arb(spt); cpt+=SIZ_SIP_ION;}
     else if(stype==TYPE_NODE)
       {if(prune_node(spt)) return(cpt); cpt+=SIZ_SIP_ION;}
     else cpt+=stub_size(spt);
     /* cpt now points at next ds stub, and spt is present stub */
     if(spt->conds==NISINFO && notinit) /*initialise ins,outs & size structs*/
       {notinit=F; /* only initialise if info structs have been setup */
        if(spt->hassubpt) set_com_struct(ppt,spt);}
     if(spt->hassubpt) cpt=ds_prune(cpt);
     if(spt->hasarypt) cpt=ds_prune(cpt);
    }while(spt->hastail);
  return(cpt);
}

/* fetch the (partial) node ds from disk via dsik_indx
*  returns T if ok (ds in dpt) */
boolean ds_get_node(dpt,nodenum)
  ds_sea_elm *dpt; yard nodenum;
{ char *mpt; foot base,dsmem,space; size_t ds_size; door size;
  dpt->refs=1; dpt->modified=F; dpt->sst_pos=0;
  base=ds_get_indx(nodenum,(foot *)&space,(door *)&size);
  if(!base) return(F);
  dsmem=space-sizeof(foot);
  ds_size=(size_t)dsmem+ds_page_size;
  mpt=malmem(char,ds_size);
  fget_data(nalfp,(foot *)&dpt->sst_pos,sizeof(foot));
  fget_data(nalfp,mpt,(size_t)dsmem); /* read ds into memory */
  dpt->ds_base=mpt;
  dpt->ds_end=mpt+(size_t)dsmem;
  dpt->ds_size=ds_size;
  dpt->nodenum=nodenum;
  return(T);
}

/* store the (partial) node ds on disk. Update dsik index */
void ds_put_node(dpt)
  ds_sea_elm *dpt;
{ stub_elm *spt; ds_sea_elm *tmpool;
  foot space=0,dsmem; int slot1;
  if(!dpt) signal_error(EXIT_INT_ERR,87);
  if(dpt->modified)
    {if(dpt->nodenum<1) signal_error(EXIT_INT_ERR,88);
     tmpool=ds_pool; if(ds_pool) slot1=push_stub(ds_stub);
     ds_pool=dpt;
     /* check incase a node with no i/o */
     spt=(stub_elm *)dpt->ds_base;
     if(spt->hassubpt) /* don't bother to prune if only root node */
       {spt=ds_next(spt);
        ds_prune(spt); /* this may affect dpt->ds_end */
       }
     ds_pool=tmpool; if(ds_pool) ds_stub=pop_stub(slot1); else ds_stub=NULL;
     dsmem=(foot)(dpt->ds_end-dpt->ds_base);
     space=dsmem+sizeof(foot);
     nal_alloc(dpt->nodenum,space); /*alloc a blk & position nalfp*/
     fput_data(nalfp,(foot *)&dpt->sst_pos,sizeof(foot));
     fput_data(nalfp,dpt->ds_base,(size_t)dsmem); /* write ds to disk */
     fflush(nalfp);
    }
  ds_free(dpt); ofs_clear(dpt);
}

ds_sea_elm *find_pool(nodenum)
  yard nodenum;
{ ds_sea_elm *dpt=ds_sea,*lpt=ds_sea+ds_sea_max;
  while(dpt<lpt) {if(nodenum==dpt->nodenum) return(dpt); ++dpt;}
  return(NULL);
}

/* release a nodes ds */
void ds_release(nodenum)
  yard nodenum;
{ ds_sea_elm *dpt;
  if(nodenum)
    {dpt=find_pool(nodenum);
     if(!dpt) signal_error(EXIT_INT_ERR,89);
     if(dpt->refs==0) signal_error(ERR_NET_CHK);
     else --(dpt->refs);
    }
}

/* move a ds_sea_elm's handle to front of lilo */
void ds_move(dpt)
  ds_sea_elm *dpt;
{ word hd=dpt->pool_handle,*apt=ds_lilo,*lpt=ds_lilo+ds_sea_max;
  while(apt<lpt && *apt!=hd) ++apt;
  if(apt==ds_lilo) return;
  if(apt==lpt) signal_error(EXIT_INT_ERR,90); /* hd not in lilo!!! */
  while(apt>ds_lilo) {*apt=(*(apt-1)); --apt;}
  *apt=hd;
}

/* free up some ds space, returns T if some more space freed */
/* DEBUG it would be better to free consecutive ds blks, see mem_mal() */
boolean ds_space()
{ ds_sea_elm *dpt,*bpt=ds_sea-1; word *lpt=ds_lilo+ds_sea_max;
  foot dadrs; boolean wasmod;
  while(lpt>ds_lilo)
    {--lpt; dpt=bpt+(*lpt);
     if(!dpt->refs&&dpt->ds_size&&dpt->nodenum)
       {/* save cur nal pos in case we are interrupting a nal file access*/
        if(wasmod=dpt->modified) dadrs=ftell(nalfp);
        ds_put_node(dpt);
        if(wasmod) fmoveto(nalfp,dadrs);
        return(T);
       }
    }
  return(F);
}

/* maintain ds_sea as a LILO */
ds_sea_elm *ds_claim(nodenum)
   yard nodenum;
{ ds_sea_elm *dpt,*bpt=ds_sea-1;
  stub_elm *spt; word *lpt=ds_lilo+ds_sea_max; int slot1;
  if(nodenum)
    {dpt=find_pool(nodenum);
     if(dpt) /* if found move to front of lilo */
       {++(dpt->refs); ds_move(dpt);}
     else /* find a free ds_sea_elm starting from end of lilo */
       {while(lpt>ds_lilo)
          {--lpt; dpt=bpt+(*lpt); if(!dpt->refs) break; else dpt=NULL;}
        if(!dpt) signal_error(EXIT_DSP_FUL);
        ds_put_node(dpt);
        ds_move(dpt); /* move dpt handle to start of pool */
        if(!ds_get_node(dpt,nodenum))
          {dpt->nodenum=nodenum;
           bpt=ds_pool; if(ds_pool) slot1=push_stub(ds_stub);
           ds_pool=dpt;
           spt=new_stub((stub_elm *)NULL,(int)0,(int)TYPE_NODE,DS_SUB,F);
           ds_pool=bpt; if(ds_pool) ds_stub=pop_stub(slot1);
           spt->info.nodenum=nodenum;
          }
       }
     return(dpt);
    }
  return(NULL);
}

void ds_flush(refchk)
  boolean refchk;
{ ds_sea_elm *dpt=ds_sea,*lpt=ds_sea+ds_sea_max;
  while(dpt<lpt)
    {if(dpt->refs>0) {if(refchk) signal_error(EXIT_INT_ERR,91);}
     else
       {ds_put_node(dpt);
        if(refchk)
          {dpt->ofs_free=0;
           if(dpt->ofs_list) {lfree(dpt->ofs_list); dpt->ofs_list=NULL;}
          }
       }
     ++dpt;
    }
}

/*------------------------------------------------------------------------*/
void nial_get_ds(nodenum,sst_pos)
  yard nodenum; foot sst_pos;
{ ds_pool=ds_claim(nodenum);
  if(!ds_pool) signal_error(EXIT_INT_ERR,92);
  ds_pool->sst_pos=sst_pos;
  ds_stub=dsroot(); /* root should be set up by now */
}

void nial_put_ds(nodenum)
  yard nodenum;
{ ds_release(nodenum); ds_pool=NULL; ds_stub=NULL;
}

/* write initial banner to nal file */
void nial_start()
{ foot aa=0; yard bb=0;
  fprintf(nalfp,version,version_num,version_upd,input_name);
  dsindex=ftell(nalfp);
  fput_data(nalfp,(foot *)&aa,sizeof(foot));
  fput_data(nalfp,(yard *)&bb,sizeof(yard));
  fflush(nalfp);
}

/* append ds index to nal file */
void nial_end()
{ yard sum=0,num; int jj; boolean noiflg=T; foot fpt,dumy;
  ds_sea_elm dse; stub_elm *spt;
  ds_flush(T); num=nal_node_max; /* note ds_flush may alter nal_node_max */
  flags.wild=F;
  fposeof(nalfp); fmoveto(dsifp,(foot)sizeof(foot)); fpt=ftell(nalfp);
  while(num)
    {fget_data(dsifp,(foot *)&dumy,sizeof(foot));
     sum+=sizeof(foot); if(sum>(BUFSIZ/2)) {fflush(nalfp); sum=0;}
     fput_data(nalfp,(foot *)&dumy,sizeof(foot));
     --num;
    }
  fmoveto(nalfp,dsindex);
  fput_data(nalfp,(foot *)&fpt,sizeof(foot));
  fput_data(nalfp,(yard *)&nal_node_max,sizeof(yard));
  fflush(nalfp);
  if(WRN_NODE_NOIO>=warn_limit) return;
  jj=0;
  for(num=1;num<=nal_node_max;++num)
    {ds_get_node((ds_sea_elm *)&dse,num);
     spt=(stub_elm *)dse.ds_base;
     if(!spt) signal_error(EXIT_INT_ERR,93);
     if((ds_value(spt)->conx.in_num==0) && (ds_value(spt)->conx.out_num==0))
       {if(noiflg) {signal_error(WRN_NODE_NOIO); noiflg=F;}
        fprintf(logfp,"%ld ",(long)spt->info.nodenum); ++jj;
        if(jj>5) {jj=0; fprintf(logfp,"\n");}
       }
     ds_free((ds_sea_elm *)&dse);
    }
  if(jj>0) fprintf(logfp,"\n");
  fremem(ds_lilo); fremem(ds_sea);
}
/*-----------------------------------------------------------------------*/
/* check ds points to an array of sorts,
*  return adjusted spt if ok, else null */
stub_elm *check_sip(spt)
  stub_elm *spt;
{ if(!spt) return(NULL);
  if(spt->type!=TYPE_ARB)
    {if(spt->hassubpt)
       {spt=ds_subpt(spt);
        if(spt->type==SIP_NODE) return(spt);
        path_error(PER_GEN,path_pt,path_pt,WRN_DS_NARB);
       }
     else if(spt->hasarypt||(spt->info.data.size!=1)) return(spt);
     return(NULL);
    }
  return(ds_subpt(spt));
}

/* return lowest elm in cur ds ary
*  spt pts to arys parent node (TYPE_ARB) */
mile sip_lowest(spt)
  stub_elm *spt;
{ spt=check_sip(spt);
  if(spt)
    {if(spt->type!=TYPE_NODE) return(spt->info.data.elm);
     while(spt)
       {if(ds_value(spt)->conx.in_num!=0
           || ds_value(spt)->conx.out_num!=0)
          return(spt->info.data.elm);
        spt=ds_arypt(spt);
       }
    }
  return(0);
}

/* find largest elm in DS ary, returns (-1) if not set yet */
mile sip_largest(spt)
  stub_elm *spt;
{ boolean empty; stub_elm *lpt;
  spt=check_sip(spt);
  if(spt)
    {empty=(boolean)(spt->type==TYPE_NODE); lpt=spt;
     while(spt->hasarypt) /* sip ary elms are always in ascending order */
       {if(spt->type==TYPE_ARB) signal_error(EXIT_INT_ERR,94);
        spt=ds_arypt(spt);
        if(!empty || (ds_value(spt)->conx.in_num!=0)
           || (ds_value(spt)->conx.out_num!=0)) lpt=spt;
       }
     if(!lpt->isdef&&lpt->info.data.elm==0
        &&(ds_value(lpt)->conx.in_num==0)
        &&(ds_value(lpt)->conx.out_num==0)
       ) return(-1);
     return(lpt->info.data.elm);
    }
  return(-1);
}

/* return a list of the elms in the cur ds ary
*  spt pts to parent
*  Returns NULL if not set yet
*/
mile *sip_list(spt)
  stub_elm *spt;
{ mile *ary_list=NULL,aaelm=(-1),bbelm,curelm; boolean empty;
  spt=check_sip(spt);
  empty=(boolean)(spt && spt->type==TYPE_NODE);
  while(spt) /* sip ary elms are always in ascending order */
    {if(spt->type==TYPE_ARB) signal_error(EXIT_INT_ERR,95);
     if(!empty || (ds_value(spt)->conx.in_num!=0)
        || (ds_value(spt)->conx.out_num!=0))
       {curelm=spt->info.data.elm;
        if(curelm>=0)
          {if(aaelm<0) {aaelm=curelm; bbelm=curelm;}
           else if(curelm==(bbelm+1)) ++bbelm;
           else
             {add_map_res((mile **)&ary_list,aaelm,bbelm);
              aaelm=curelm; bbelm=curelm;}
          }
        else signal_error(EXIT_INT_ERR,96);
       }
     spt=ds_arypt(spt);
    }
  if(aaelm>=0)
    add_map_res((mile **)&ary_list,aaelm,bbelm);
  return(ary_list);
}

/* return ds position of index at current ds_stub level
*  creates a new elm if not found & docre=T else returns NULL*/
stub_elm *sip_pos(apt,spt,indx,stype,size,docre)
  stub_elm **apt,*spt; mile indx; int stype,size; boolean docre;
{ stub_elm *tpt; mile curelm; int slot1;
  while(spt)
    {if(spt->type==TYPE_ARB) signal_error(EXIT_INT_ERR,97);
     curelm=spt->info.data.elm;
     if(curelm==indx) return(spt);
     if(curelm<indx)
       {tpt=ds_arypt(spt);
        if(!tpt || tpt->info.data.elm>indx)
          {if(!docre) return(NULL);
           slot1=push_stub(*apt);
           spt=new_stub(spt,size,stype,DS_ARY,
                    (boolean)(spt->info.data.size==0));
           *apt=pop_stub(slot1);
           fudge:
           spt->info.data.size=(*apt)->info.data.size;
           mush:
           spt->info.data.elm=indx;
           adj_uasize(spt,(int)1);
           return(spt);
          }
        spt=tpt;
       }
     else
       {if(spt==(*apt))
          {if(spt->type==TYPE_ARB) signal_error(EXIT_INT_ERR,98);
           if(docre)
             {tpt=ds_parent(spt);
              if(ds_tail(tpt)==spt)
                {spt=new_stub(tpt,size,stype,DS_TAIL,
                        (boolean)(spt->info.data.size==0));
                 tpt=ds_tail(spt);
                }
              else if(ds_subpt(tpt)==spt)
                {spt=new_stub(tpt,size,stype,DS_SUB,
                          (boolean)(spt->info.data.size==0));
                 tpt=ds_subpt(spt); spt->hassubpt=F; spt->hastail=tpt->hastail;
                }
              else signal_error(EXIT_INT_ERR,99);
              tpt->hastail=F; spt->hasarypt=T; *apt=spt;
              spt->info.data.size=tpt->info.data.size;
              goto mush;
             }
           else return(NULL);
          }
        spt=ds_parent(spt);
        if(spt==(*apt) && spt->type==TYPE_ARB)
          {if(docre)
             {slot1=push_stub(*apt);
              spt=new_stub(*apt,size,stype,DS_SUB,
                      (boolean)(spt->info.data.size==0));
              *apt=pop_stub(slot1);
              spt->hasarypt=spt->hassubpt; spt->hassubpt=F;
              goto fudge;}
           else return(NULL);
          }
       }
    }
  signal_error(EXIT_INT_ERR,100); return(NULL);
}

stub_elm *sip_inc(spt)
  stub_elm *spt;
{ if(!spt->hastail) signal_error(EXIT_INT_ERR,101);
  return(ds_tail(spt));
}

/* form sip list for new decl, called by get_nxt_decl
*  ppt is parent, first new decl is subpt to ppt, rest are tails
*  if crenob is set, initialises a nob node as well
*  returns ppt (realloc may have moved pool) or nob node pt if crenob
*/
stub_elm *sip_decl(ppt,decl_pt,crenob)
  stub_elm *ppt; decl_elm *decl_pt; boolean crenob;
{ stub_elm *spt; char *cpt; decl_elm *dpt=decl_pt; int stype,space;
  if(!dpt || !ppt || !dpt->declnum) signal_error(EXIT_INT_ERR,102);
  if(ppt->hassubpt) /*already defined*/
    {if(crenob) signal_error(EXIT_INT_ERR,103);
     return(ppt);
    }
  /* work out total size of decl stubs */
  if(crenob)
    {ppt->type=TYPE_ARB; ppt->info.data.elm=1;
     /* cur size=1, and ppt->info.data.size should be set already */
     space=SIZ_SIP_ION;}
  else space=0;
  while(dpt && dpt->declnum)
    {space+=stub_base((int)dpt->decltype,(boolean)(dpt->size==0));
     dpt=dpt->tail;}
  spt=ds_next(ppt);
  cpt=ds_meminc(spt,space); /* create a chunk of space for all decl stubs */
  spt=(stub_elm *)cpt;
  ppt=ds_prior(spt); ppt->hassubpt=T;
  if(crenob)
    {zap_stub(spt,(int)SIZ_SIP_ION,(int)TYPE_NODE,T);
     spt->info.data.size=ppt->info.data.size;
     spt->hassubpt=T; ppt=spt;
     cpt+=SIZ_SIP_ION; spt=(stub_elm *)cpt;
    }
  crenob=T;
  for(;;) /* now initialise decl stubs in ds */
    {if(!decl_pt->declnum) break;
     stype=(int)decl_pt->decltype;
     space=stub_base(stype,(boolean)(decl_pt->size==0));
     zap_stub(spt,space,stype,T);
     spt->info.data.size=decl_pt->size;
     if(is_read_struct(decl_pt))
       {if(strcmp(decl_pt->name,"node")==0) spt->conds=NISNODE;
        else if(crenob && strcmp(decl_pt->name,"group")!=0)
          {if(crenob) spt->conds=NISINFO; else spt->conds=NISNUL; crenob=F;}
       }
     decl_pt=decl_pt->tail;
     if(decl_pt) spt->hastail=T; else break;
     cpt+=space; spt=(stub_elm *)cpt;
    }
  return(ppt);
}

/* convert cur node to a TYPE_ARB & then add a child TYPE_NODE
*  returns pt to TYPE_ARB node */
stub_elm *sip_arnob(ppt)
  stub_elm *ppt;
{ stub_elm *spt;
  ppt->type=TYPE_ARB; ppt->info.data.elm=1;
  spt=new_stub(ppt,(int)0,(int)TYPE_NODE,DS_SUB,F);
  ppt=ds_prior(spt);
  spt->info.data.size=ppt->info.data.size;
  return(ppt);
}

/* create an array of sips to hold a ds ary */
stub_elm *sip_ary(rpt,size)
  stub_elm *rpt; mile size;
{ stub_elm *spt=rpt; mile indx; int stype=(int)rpt->type;
  if(rpt->info.data.size!=size) signal_error(EXIT_INT_ERR,104);
  for(indx=0;indx<size;++indx)
     {spt=sip_pos((stub_elm **)&rpt,spt,indx,stype,(int)0,T);
      if(stype==TYPE_NODE && spt) spt->isdef=T;
      if(!spt) signal_error(EXIT_INT_ERR,105);
     }
  return(rpt);
}

/* find ds_stub corresponding to ppt
*  return null sip pos if not found, sets inds if in ds.
*/
stub_elm *find_dspt(ppt,inds)
  path_elm *ppt; boolean *inds;
{ mile decl,indx; stub_elm *spt; path_elm *tpt=ppt;
  *inds=F; /* look for net/node boundary */
  while(tpt) {if(tpt->net->root_flag) {*inds=T; break;} tpt=tpt->parent;}
  if(!tpt || !ppt || !inds) return(NULL);
  spt=dsroot();
  if(!spt) signal_error(EXIT_INT_ERR,106);
  while(tpt && spt)
    {if(spt->type!=TYPE_NODE && spt->type!=TYPE_ARB && spt->type!=TYPE_STRUCT)
       signal_error(EXIT_INT_ERR,107);
     if(!tpt->decl_pt)
       {if(/*flags.state && */ tpt==ppt) break; /*DEBUG-4/11/91 DJRW*/
        signal_error(EXIT_INT_ERR,108);}
     spt=ds_subpt(spt);
     decl=tpt->decl_pt->declnum; --decl; /* find decl */
     if(decl<0) return(NULL);
     while(decl && spt) {spt=ds_tail(spt); --decl;}
     if(decl || tpt==ppt) break; /* hasval is unset on last node */
     indx=tpt->dary.hasval; /* find index if array */
     if(spt->type==TYPE_ARB||spt->hasarypt) /*DEBUG12/91 added spt->hasarypt*/
       {if(spt->type==TYPE_ARB) spt=ds_subpt(spt); /* index into this ary */
        while(spt) {if(spt->info.data.elm==indx) break; spt=ds_arypt(spt);}
       }
     else {if(indx>0) signal_error(EXIT_INT_ERR,109);} /* was >1 DEBUG */
     tpt=tpt->tail;
    }
  return(spt);
}

/* find a ds index elm, create one if none */
stub_elm *sip_indx(rp,indx)
  stub_elm *rp; mile indx;
{ stub_elm *spt=rp; int stype=(int)rp->type;
  if(stype==TYPE_ARB)
    {if(!rp->hassubpt) /*DEBUG was return(NULL);  now goes to new_stub below*/
       goto addindx;
     spt=ds_subpt(rp); stype=(int)spt->type;}
  else if(rp->info.data.size==0)
    {/* need to adj old UA stub size to ARB size */
     rp=adj_stub(rp,(int)SIZ_SIP_ION,(int)stub_base(stype,T));
     ds_value(rp)->conx.in_num=0; ds_value(rp)->conx.out_num=0;
     rp->type=TYPE_ARB; 
     addindx:
     rp->info.data.elm=1;
     spt=new_stub(rp,(int)0,(int)stype,DS_SUB,T);
     rp=ds_prior(spt);
     spt->info.data.size=rp->info.data.size;
     spt->info.data.elm=indx;
    }
  spt=sip_pos((stub_elm **)&rp,spt,indx,stype,(int)0,T);
  if(stype==TYPE_NODE && spt) spt->isdef=T;
  return(spt);
}
/* given a leaf address, find it's DS pos, similar to find_dspt
*  if pos found caller must release ds when done */
stub_elm *node_to_ds(npt,rppt)
  node_elm *npt; ds_sea_elm **rppt;
{ mile decl,indx; int num=npt->di_num;
  ds_sea_elm *pool; stub_elm *spt; di_elm *dpt=node_di(npt);
  pool=ds_claim(npt->base); spt=(stub_elm *)pool->ds_base;
  *rppt=pool;
  while(spt && num)
    {if(spt->type!=TYPE_NODE && spt->type!=TYPE_ARB)
       signal_error(EXIT_INT_ERR,110);
     spt=ds_subpt(spt);
     decl=dpt->decl; --decl; /* find decl */
     if(decl<0) return(NULL);
     while(decl && spt) {spt=ds_tail(spt); --decl;}
     if(decl) break;
     indx=dpt->indx; /* find index if array */
     spt=sip_indx(spt,indx);
     ++dpt; --num;
    }
  if(!spt) ds_release(npt->base);
  return(spt);
}

#ifdef NSLDEBUG
int ds(spt)
  stub_elm *spt;
{ long ll=(long)spt; word ww=(word)ll&0xffff;
  printf("%x:(p%ds%da%dt%d,d%dv%dc%d,T%d,S%dE%d,V%lx)\n",ww,
    spt->hasparent,spt->hassubpt,spt->hasarypt,spt->hastail,
    spt->isdef,spt->isvar,spt->conds,
    spt->type,
    spt->info.data.size,spt->info.data.elm,
    ds_value(spt)->num);
  return(1);
}

char *dl(apt,bpt,space)
  char *apt,*bpt; int space;
{ stub_elm *spt; int size; boolean hassubpt,hasarypt,hastail=T;
  while(apt<bpt && hastail)
    {spt=(stub_elm *)apt;
     size=space; while(size>0) {printf("|"); --size;}
     hassubpt=spt->hassubpt; hasarypt=spt->hasarypt; hastail=spt->hastail;
     ds(spt);
     apt+=stub_size(apt);
     if(hassubpt) apt=dl(apt,bpt,space+1+hasarypt);
     if(hasarypt) apt=dl(apt,bpt,space+1);
    }
  return(apt);
}
int dd(dpt)
  ds_sea_elm *dpt;
{ if(!dpt) return(0);
  printf("<%d>\n",dpt->pool_handle);
  dl(dpt->ds_base,dpt->ds_end,0);
  return(1);
}
int dp()
{ dd(ds_pool);
  return(1);
}
int da(dpt)
  ds_sea_elm *dpt;
{ char *apt=dpt->ds_base,*bpt=dpt->ds_end; int num=0;
  printf("<%d>\n",dpt->pool_handle);
  while(apt<bpt)
    {++num;
     ds(apt);
     apt+=stub_size(apt);
    }
  return(num);
}
#endif
