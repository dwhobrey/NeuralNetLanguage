/* Name: nslstd.c - Model Independent Function Library
*  NeSeL v2.0 April 1989  Neural Network Specification Language
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <stdarg.h>
#include <math.h>

#include "nsltype.h"
#include "nslstd.h"

#if !defined(NSL1)||!defined(NSL2)
#include "memtrack.h"
#endif

long nsl_aseed=123,nsl_bseed=7131,nsl_cseed=30013;
/* reseed p.r.n.g. */
void srnd(seed) int seed;
{ nsl_aseed=171*(seed%177); nsl_bseed=172*(seed%176); nsl_cseed=170*(seed%178);
}
/* return random number, 0.0 <= rndf() < 1.0 */
float rndf()
{float temp; double t1;
 nsl_aseed=171*(nsl_aseed % 177)-2*(nsl_aseed/177);
 if(nsl_aseed<0) nsl_aseed+=30269;
 nsl_bseed=172*(nsl_bseed % 176)-35*(nsl_bseed/176);
 if(nsl_bseed<0) nsl_bseed+=30307;
 nsl_cseed=170*(nsl_cseed % 178)-63*(nsl_cseed/178);
 if(nsl_cseed<0) nsl_cseed+=30323;
 temp=(float)((float)nsl_aseed/(float)30269.0
      +(float)nsl_bseed/(float)30307.0+(float)nsl_cseed/(float)30323.0);
 return((float)modf((double)temp,&t1));
}
/* return random number, 1 <= rnd() <= randmax */
int rnd(randmax)
  int randmax;
{ if(randmax==0) return(0);
 return((int)(1+randmax*rndf()));
}
#ifdef NSL2
extern char *mem_rel(),*mem_mal();
extern void mem_free();
#endif
/* free mem used by list */
void lfree(a) void *a;
{ if(a)
    {if(!lst_valid(a)) fprintf(stderr,"Bad list\n");
     else
       {lst_code(a)=0;
#ifdef NSL2
       mem_free(lst_base(a));
#else
       free(lst_base(a));
#endif
       }
    }
}
/* enlarge a list in [#elms:max#elms:sizofelm:lsttype:...elms...] format */
void *nsl_lst_more(mlp,need,lstype,elm_size)
  void *mlp; int need,lstype,elm_size;
{ int max; char *lp=(char *)mlp;
  if(!lp)
    {max=LST_MORE+need;
#ifdef NSL2
     lp=(char *)mem_mal(LST_MEM+elm_size*max)+LST_MEM;
#else
     lp=(char *)malloc(LST_MEM+elm_size*max)+LST_MEM;
#endif
     lst_max(lp)=(mile)max; lst_siz(lp)=(mile)elm_size;
     lst_num(lp)=(mile)need; lst_typ(lp)=(mile)lstype;
     lst_code(lp)=LST_CODE;
    }
  else
    {max=(int)lst_max(lp);
     if((lst_num(lp)+need+2)>=max)
       {
#ifdef NSL2
        int mm=LST_MEM+lst_siz(lp)*max;
        max+=need+LST_MORE;
        lp=(char *)mem_rel(lst_base(lp),LST_MEM+lst_siz(lp)*max,mm)+LST_MEM;
#else
        max+=need+LST_MORE;
        lp=(char *)realloc(lst_base(lp),LST_MEM+lst_siz(lp)*max)+LST_MEM;
#endif
        lst_max(lp)=(mile)max;
       }
     lst_num(lp)+=(mile)need;
    }
  return((void *)lp);
}

/* returns space for a list of elms & sets list details */
void *lnew(num,lstype) int num,lstype;
{ size_t siz;
  if(lstype>0) switch(lstype&LST_TYPE)
    {case TYPE_CHAR: case TYPE_USCHAR: siz=sizeof(char); break;
     case TYPE_DOUBLE: siz=sizeof(double); break;
     case TYPE_FLOAT: siz=sizeof(float); break;
     case TYPE_INT: case TYPE_USINT: siz=sizeof(int); break;
     case TYPE_LONG: case TYPE_USLONG: siz=sizeof(long); break;
     case TYPE_SHORT: case TYPE_USSHORT: siz=sizeof(short); break;
     case TYPE_PT: siz=sizeof(char *); break;
     default: return((void *)0L);
    }
  else {siz=(-lstype); lstype=TYPE_UNKNOWN;}
  return(nsl_lst_more((char *)0L,(int)num,(int)lstype,(int)siz));
}
boolean lequal(ma,mb)
  void *ma,*mb;
{ int num; char *a=(char *)ma,*b=(char *)mb;
  if(!a&&!b) return(T); if(!a||!b) return(F);
  num=lst_num(a);
  if(num!=lst_num(b)||lst_typ(a)!=lst_typ(b)||lst_siz(a)!=lst_siz(b))
    return(F);
  num*=lst_siz(a); a=lst_head(a,char); b=lst_head(b,char);
  while(num--) {if(*a!= *b) return(F); ++a; ++b;}
  return(T);
}
/* copy list a to b, create space if necessary */
void *lcopy(ma,mb) void *ma,*mb;
{ mile num,siz,*c=(mile *)0L; char *a=(char *)ma,*b=(char *)mb;
  if(ma)
    {siz=lst_siz(ma); num=lst_num(ma);
     c=nsl_lst_more(mb,(int)num,(int)lst_typ(ma),(int)siz);
     num*=siz; a=lst_head(ma,char); b=lst_head(c,char);
     while(num--) *b++ = *a++;
    }
  return((void *)c);
}
/* reverse list inline */
void *lrev(lp)
  void *lp;
{ char chr,*a=(char *)lp,*b; mile num,siz,val;
  if(a)
    {num=(lst_num(a)+1)/2; siz=lst_siz(a);
     b=lst_end(a); a=lst_head(a,char);
     while(num--)
       {b-=siz; val=siz;
        while(val--) {chr=(*a); *a++ =(*b); *b++ =chr;}
        a+=siz;
       }
    }
  return(lp);
}
/* put value in list after casting */
void nsl_lst_put(lp,val,typ)
  void *lp; double val; mile typ;
{ switch(typ&LST_TYPE)
    {case TYPE_PT: *((char **)lp)=(char *)((long)val); break;
     case TYPE_FLOAT: *((float *)lp)=(float)val; break;
     case TYPE_DOUBLE: *((double *)lp)=val; break;
     case TYPE_LONG: case TYPE_USLONG: *((long *)lp)=(long)val; break;
     case TYPE_INT: case TYPE_USINT:  *((int *)lp)=(int)val; break;
     case TYPE_CHAR: case TYPE_USCHAR: *((char *)lp)=(char)val; break;
     default: *((mile *)lp)=(mile)val;
    }
}
/* get value from list and return as a double */
double nsl_lst_val(mlpt,lstype)
  void **mlpt; mile lstype;
{ char *apt=(char *)*mlpt; double val;
  switch(lstype&LST_TYPE)
    {case TYPE_PT: break;
       val=(double)((long)(*((char **)apt)));
       apt+=sizeof(char *); break;
     case TYPE_FLOAT:
       val=(double)(*((float *)apt)); apt+=sizeof(float); break;
     case TYPE_DOUBLE:
       val=(*((double *)apt)); apt+=sizeof(double); break;
     case TYPE_LONG: case TYPE_USLONG:
       val=(double)(*((long *)apt)); apt+=sizeof(long); break;
     case TYPE_INT: case TYPE_USINT:
       val=(double)(*((int *)apt)); apt+=sizeof(int); break;
     case TYPE_CHAR: case TYPE_USCHAR:
       val=(double)(*apt); apt+=sizeof(char); break;
     default:
       val=(double)(*((mile *)apt)); apt+=sizeof(mile);
    }
  *mlpt=(void *)apt;
  return(val);
}
/* returns smallest / largest value in list */
double nsl_lst_hilo(lp,islar)
  void *lp; boolean islar;
{ mile num,lstype; double val,lar;
  if(islar) lar=(double)(-1.0); else lar=(double)0.0; val=lar;
  if(lp)
    {num=lst_num(lp); lstype=lst_typ(lp);
     lp=lst_head(lp,void);
     while(num--)
       {val=nsl_lst_val((void **)&lp,lstype);
        if(islar) {if(val>lar) lar=val;}
        else if(val<lar) lar=val;
       }
    }
  return(lar);
}
/* returns number of elms in list */
mile nsl_lst_num(lp) void *lp;
{ mile num,lstype,pip=0; double val; char *apt=(char *)lp;
  if(apt)
    {if((num=lst_num(apt)))
       {lstype=lst_typ(apt);
        if(lstype&LST_PBIT)
          {apt=lst_head(apt,char);
           while(num>0)
             {++pip;
              val=nsl_lst_val((void **)&apt,lstype);
              val-=nsl_lst_val((void **)&apt,lstype); num-=2;
              if(val<(double)0.0) val=(-val);
              pip+=(mile)val;}
           if(pip==0) return(1);
          }
        else return(num);
       }
    }
  return(pip);
}
/* return pt to nth elm of list cast as a double, returns null if not found */
double *nsl_lst_nth(lp,nth) void *lp; mile nth;
{ mile lstype,num_elms,asize,aelm; double bl; static double al;
  char *ary=(char *)lp;
  if(!ary) return((double *)0L);
  if(nth>=0)
    {asize=lst_num(ary); lstype=lst_typ(ary);
     if(lstype&LST_PBIT)
       {ary=lst_head(ary,char); num_elms=0;
        while(asize)
          {al=nsl_lst_val((void **)&ary,lstype);
           bl=nsl_lst_val((void **)&ary,lstype);
           asize-=2; aelm=num_elms; ++num_elms; bl=al-bl;
           if(bl<(double)0.0) num_elms-=(mile)bl; else num_elms+=(mile)bl;
           if(nth<num_elms)
             {nth-=aelm;
              if(bl<(double)0.0) al+=(double)nth; else al-=(double)nth;
              return((double *)&al);
             }
          }
       }
     else
       {if(nth>=asize) return((double *)0L);
        ary=(lst_head(ary,char)+nth*lst_siz(ary));
        al=nsl_lst_val((void **)&ary,lstype);
        return((double *)&al);
       }
    }
  return((double *)0L);
}

/* return nth elm of list cast as a double, returns 0.0 if not found */
double nsl_lst_ndd(a,nth) void *a; mile nth;
{ double *dp=nsl_lst_nth(a,nth);
  if(dp) return((double)*dp);
  return((double)0.0);
}

/* Find pos of elm with value in list.  return(-1) if not found */
mile nsl_lst_pos(a,val) void *a; double val;
{ mile xx,nn; double *dp;
  if(!a) return(-1);
  nn=nsl_lst_num(a);
  for(xx=0;xx<nn;++xx)
    {if(!(dp=nsl_lst_nth(a,xx))) break;
     if(*dp==val) return(xx);
    }
  return(-1);
}

/* if is_mi, subtract list b from list a & returns new list */
/* else returns interset of lists */
void *lmi_in(a,b,is_mi) void *a,*b; boolean is_mi;
{ void *d; mile lena,lenb,aa,bb,typ; double xx,val; boolean is_ok;
  if(!a) return(b); if(!b) return(a);
  lena=nsl_lst_num(a); lenb=nsl_lst_num(b);
  typ=lst_typ(a); if(typ&LST_PBIT) typ^=LST_PBIT;
  aa=lena; if(!is_mi) aa+=lenb;
  d=lnew((int)aa,(int)typ); lst_num(d)=0;
  for(aa=0;aa<lena;++aa)
    {xx=nsl_lst_ndd(a,aa); is_ok=is_mi;
     for(bb=0;bb<lenb;++bb)
       {val=nsl_lst_ndd(b,bb);
        if(val==xx) {is_ok=(boolean)!is_mi; if(is_mi) break;}
       }
     if(is_ok) {nsl_lst_put(lst_end(d),xx,typ); ++lst_num(d);}
    }
  return(d);
}
void *lminus(a,b) void *a,*b;
{ return(lmi_in(a,b,T));
}
void *lintersect(a,b) void *a,*b;
{ return(lmi_in(a,b,F));
}
/* returns union/catenation of lists */
void *lcatuni(a,b,iscat) void *a,*b; boolean iscat;
{ mile lena,lenb,aa,typ,*d; double val;
  if(!a) return(lcopy(b,(mile *)0L)); if(!b) return(lcopy(a,(mile *)0L));
  lena=nsl_lst_num(a); lenb=nsl_lst_num(b);
  typ=lst_typ(a); if(typ&LST_PBIT) typ^=LST_PBIT;
  d=(mile *)lnew((int)(lena+lenb),(int)typ); lst_num(d)=0;
  for(aa=0;aa<lena;++aa)
    {val=nsl_lst_ndd(a,aa);
     nsl_lst_put(lst_end(d),val,typ); ++lst_num(d);
    }
  for(aa=0;aa<lenb;++aa)
    {val=nsl_lst_ndd(b,aa);
     if(iscat||(nsl_lst_pos(a,val)<0))
       {nsl_lst_put(lst_end(d),val,typ); ++lst_num(d);}
    }
  return((void *)d);
}

/* returns union of lists */
void *lunion(a,b) void *a,*b;
{ return(lcatuni(a,b,F));
}
/* returns list of catenation of two lists */
void *lcat(a,b) void *a,*b;
{ return(lcatuni(a,b,T));
}

/* takes a list of lists as args and maps to single list
*  eg [A,B,C,D] --> [a1,b1,c1,d1,a2,b2,c2...] */
void *llcat(args) va_list args;
{ mile typ,jj,kk,xx,yy=0; double val; char **apt=(char **)args; void *lst;
  while(*apt++) ++yy; apt=(char **)args;
  if(yy<2) return((void *)*apt);
  xx=nsl_lst_num(*apt); typ=lst_typ(*apt); if(typ&LST_PBIT) typ^=LST_PBIT;
  for(kk=1;kk<yy;++kk) {if(xx!=nsl_lst_num(*(apt+kk))) return((void *)0L);}
  lst=lnew(xx*yy,(int)typ); lst_num(lst)=0;
  for(jj=0;jj<xx;++jj)
    {for(kk=0;kk<yy;++kk)
       {val=nsl_lst_ndd(*(apt+kk),jj);
        nsl_lst_put(lst_end(lst),val,typ); ++lst_num(lst);
       }
    }
  return(lst);
}

/* Apply func to list of lists & returns a new list as result:
*  map op (a:as) (b:bs) = (op a b):(map op as bs)
*  Must be lists of type mile, return list can be any type.
*  If islist, the func fnpt returns a list, else atomic val is assumed.
*/
void *lmap(islist,fnpt,args) boolean islist; void *(*fnpt)(); va_list args;
{ mile typ,jj,kk,xx,yy=0,*lp,*lst=(mile *)0L,(*fp)(); double val;
  char **apt=(char **)args; void *cst,*nst,*rst=(void *)0L;
  while(*apt++) ++yy; apt=(char **)args;
  if(yy<1||yy>4) return((void *)*apt);
  xx=nsl_lst_num(*apt); typ=lst_typ(*apt); if(typ&LST_PBIT) typ^=LST_PBIT;
  for(kk=0;kk<yy;++kk)
     {if(lst_siz(*(apt+kk))!=(mile)sizeof(mile)) return((void *)0L);
      jj=nsl_lst_num(*(apt+kk)); if(jj!=xx) return((void *)0L);}
  if(!islist) {fp=(mile(*)())fnpt; lst=(mile *)lnew((int)1,(int)typ);}
  lp=(mile *)lnew((int)yy,(int)typ);
  for(jj=0;jj<xx;++jj)
    {for(kk=0;kk<yy;++kk) {val=nsl_lst_ndd(*(apt+kk),jj); lp[kk]=(mile)val;}
     cst=rst;
     if(islist)
       {switch(yy)
         {case 4: nst=(*fnpt)(lp[0],lp[1],lp[2],lp[3]); break;
          case 3: nst=(*fnpt)(lp[0],lp[1],lp[2]); break;
          case 2: nst=(*fnpt)(lp[0],lp[1]); break;
          default: nst=(*fnpt)(lp[0]);
         }
        rst=lcat(cst,nst); lfree(nst);
       }
     else
       {switch(yy)
         {case 4: xx=(*fp)(lp[0],lp[1],lp[2],lp[3]); break;
          case 3: xx=(*fp)(lp[0],lp[1],lp[2]); break;
          case 2: xx=(*fp)(lp[0],lp[1]); break;
          default: xx=(*fp)(lp[0]);
         }
        lst[0]=xx;
        rst=lcat(cst,lst);
       }
     lfree(cst);
    }
  lfree(lst); lfree(lp);
  return(rst);
}

/* maps absolute index values to relative ones for stack referencing */
mile *lsmap(a) mile *a;
{ mile *b,lena,j,k,y,n;
  if(!a||lst_siz(a)!=sizeof(mile)) return((mile *)0L);
  lena=lst_num(a); b=(mile *)lnew((int)lena,(int)lst_typ(a));
  for(j=0;j<lena;++j)
    {y=a[j]; n=0;
     for(k=0;k<(j-1);++k) {if(a[k]<y) ++n;}
     y-=n; if(y>0) b[j]=y; else b[j]=0;
    }
  return(b);
}
/* returns a list of between nn..mm random elements in the range aa..bb */
mile *lrand(nn,mm,aa,bb) mile nn,mm,aa,bb;
{ mile *ma,cc;
  if(mm<nn) {cc=nn; nn=mm; mm=cc;}
  if(nn<1) return((mile *)0L);
  nn+=rnd(1+mm-nn)-1;
  ma=(mile *)lnew((int)nn,(int)TYPE_SHORT);
  if(bb<aa) {cc=aa; aa=bb; bb=cc;}
  cc=1+bb-aa; while(nn--) ma[nn]=aa+rnd(cc)-1;
  return(ma);
}

/* returns Hopfield weight value wij for a set of patterns */
/* if miss is !=0 then adjusts for no self recurrent wts */
double lhopsum(ii,jj,miss,gg,cc,pats)
  mile ii,jj,miss; float gg,cc; va_list pats;
{ double sum=(double)0.0,wi,wj; mile xx,numpats=0;
  void *lst,**apt=(void **)pats;
  if(!*apt) return((double)0.0);
  xx=nsl_lst_num(*apt); if(miss&&jj>=ii) ++jj;
  if(xx<=ii||xx<=jj) return((double)0.0);
  while(*apt)
    {lst=(*apt);
     if(nsl_lst_num(lst)!=xx) return((double)0.0);
     wi=nsl_lst_ndd(lst,ii); wj=nsl_lst_ndd(lst,jj);
     sum+=(wi*gg+cc)*(wj*gg+cc); ++apt; ++numpats;
    }
  return(sum/numpats);
}

/* sum elements of list */
double lsum(ma) void *ma;
{ double sum=(double)0.0; mile num,len;
  if(!ma||((lst_typ(ma)&LST_TYPE)==TYPE_PT)) return(sum);
  len=nsl_lst_num(ma);
  for(num=0;num<len;++num) sum+=nsl_lst_ndd(ma,num);
  return(sum);
}

/* multiply or add constant to elements of list: return new list */
void *nsl_lst_ma(a,con,ismul,isnew) void *a; double con; boolean ismul,isnew;
{ double val; void *b,*c; mile num,typ;
  if(!a) return((void *)0L);
  typ=lst_typ(a); if((typ&LST_TYPE)==TYPE_PT) return((void *)0L);
  if(isnew) a=lcopy(a,(void *)0L);
  if(con==(double)0.0) return(a);
  num=lst_num(a); b=lst_head(a,void);
  while(num>0)
    {c=b; val=nsl_lst_val((void **)&b,typ);
     if(ismul) val*=con; else val+=con;
     nsl_lst_put(c,val,typ); --num;}
  return(a);
}
/* multiply or add second list to first: return new list */
void *nsl_lst_vma(a,b,ismul,isnew) void *a,*b; boolean ismul,isnew;
{ double val; void *c,*d; mile num,typ,dtyp;
  if(!a||!b) return((void *)0L);
  typ=lst_typ(a); dtyp=lst_typ(b);
  if((typ&LST_TYPE)==TYPE_PT||(dtyp&LST_TYPE)==TYPE_PT) return((void *)0L);
  num=lst_num(a); if(num!=lst_num(b)) return((void *)0L);
  if(isnew) a=lcopy(a,(void *)0L); d=lst_head(b,void); b=lst_head(a,void);
  while(num>0)
    {c=b; val=nsl_lst_val((void **)&b,typ);
     if(ismul) val*=nsl_lst_val((void **)&d,dtyp);
     else val+=nsl_lst_val((void **)&d,dtyp);
     nsl_lst_put(c,val,typ); --num;}
  return(a);
}

/* normalise vector: return new list if isnew set */
void *nsl_lst_norm(a,isnew) void *a; boolean isnew;
{ double tot; void *b; mile typ;
  if(!a) return((void *)0L);
  typ=lst_typ(a); if((typ&LST_TYPE)==TYPE_PT) return((void *)0L);
  b=nsl_lst_vma(a,a,(boolean)1,(boolean)1); tot=lsum(b); lfree(b);
  if(tot>(double)0) b=nsl_lst_ma(a,(double)(1.0/sqrt(tot)),(boolean)1,isnew);
  else return((void *)0L);
  return(b);
}

/* convert between single and double dimensions */
mile stodx(s,dx) mile s,dx;
{return(s%dx);}
mile stody(s,dx) mile s,dx;
{return(s/dx);}
mile dtos(x,y,dx) mile x,y,dx;
{return((y*dx)+x);}

float generic_bound(x,top,bot) float x,top,bot;
{ if(x>top) return((float)top);
  if(x<bot) return((float)bot);
  return((float)x);
}

float generic_limiter(x,top,bot,mid) float x,top,bot,mid;
{ if(x>mid) return((float)top);
  return((float)bot);
}

float generic_sigmoid(x,top,bot,gain,c) float x,top,bot,gain,c;
{ return((float)(bot + (top-bot)/
    ((float)1.0+(float)exp((double)(-gain * (c+x))))));
}

/* version of printf() to return result as a string */
char nsl_fsb[256];
char *printfs(fmt,args) char *fmt; va_list args;
{ if(vsprintf(nsl_fsb,fmt,args)<1) return((char *)0L);
  return(nsl_fsb);
}
