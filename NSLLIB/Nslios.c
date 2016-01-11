/* Name: nslios.c - Small Model I/O & Monitor Functions
*  NeSeL v2.0 April 1989  Neural Network Specification Language
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <process.h>
#include <malloc.h>
#include <stdarg.h>
#include <math.h>

#include "nsltype.h"
#include "nslstd.h"
#include "nslios.h"

int nsl_stop=0,nsl_abort=0,nsl_err=NSLER_OK,nsl_verbatim=0;

int nsl_driver(NSLNSLnet *netpt,int cmd,char *args)
{ NSLNSLnode *npt; NSLNSLdrv *dpt; int num;
  if(!netpt) {nsl_err=NSLER_NU; return(nsl_err);}
  nsl_err=NSLER_OK;
  npt=netpt->npt; dpt=netpt->dpt; num=netpt->num_drv;
  while(num--)
     {npt=netpt->npt+dpt->nd-1;
	  dpt->iop->cmd=cmd; dpt->iop->args=args;
      (*((netpt->fpt)[npt->fnum]))(netpt,npt->dpt);
      if(nsl_abort) break;
	  ++dpt;
	 }
  if(nsl_stop) nsl_err=NSLER_ST;
  if(nsl_abort) nsl_err=NSLER_AB;
  return(nsl_err);
}

int nsl_cycle(netpt,num)
  NSLNSLnet *netpt; long num;
{ NSLNSLconx *cpt; NSLNSLnode *npt; char *apt,*bpt; int len;
  if(!netpt) {nsl_err=NSLER_NU; return(nsl_err);}
  nsl_err=NSLER_OK; nsl_stop=0; nsl_abort=0;
  while(num--)
    {cpt=netpt->cpt; ++netpt->time;
     while(len=cpt->len)
      {apt=cpt->cpt; bpt=cpt->dpt; while(len--) *apt++ = *bpt++; ++cpt;}
     npt=netpt->npt;
     while(npt->dpt)
       {if(npt->tpt)
          {if(npt->tpt->rate)
             {if(npt->tpt->wait<1)
                {npt->tpt->wait=npt->tpt->rate-1;
                 (*((netpt->fpt)[npt->fnum]))(netpt,npt->dpt);
                }
              else --(npt->tpt->wait);
             }
          }
        else (*((netpt->fpt)[npt->fnum]))(netpt,npt->dpt);
        ++npt; if(nsl_abort) break;}
     if(nsl_stop) {nsl_err=NSLER_ST; break;}
     if(nsl_abort) {nsl_err=NSLER_AB; break;}
    }
  return(nsl_err);
}
float vec_elm(void *vva,NSLNSLnet *net, int ta)
{ float sa; long *va=(long *)vva;
  switch(ta)
    {case TYPE_CHAR: case TYPE_USCHAR:
       if(net) sa=(float)inc(*va);
       else sa=(float)*((char *)va); break;
     case TYPE_DOUBLE: if(net) sa=(float)ind(*va);
       else sa=(float)*((double *)va); break;
     case TYPE_FLOAT: if(net) sa=(float)inf(*va);
       else sa= *((float *)va); break;
     case TYPE_LONG: case TYPE_USLONG: if(net) sa=(float)inl(*va);
       else sa=(float)*((long *)va); break;
     case TYPE_SHORT: case TYPE_USSHORT: if(net) sa=(float)ins(*va);
       else sa=(float)*((short *)va); break;
     case TYPE_PT: break;
     default:
       if(net) sa=(float)ini(*va); else sa=(float)*((int *)va);
    }
  return(sa);
}

float vec_sum(void *vvec,int siz,int num,NSLNSLnet *net,int ta)
{ float sum=(float)0; char *vec=(char *)vvec;
  while(num>0) {sum+=vec_elm(vec,net,ta); vec+=siz; --num;}
  return(sum);
}
/* return norm of vector */
float vec_norm(void *vvec,int siz,int num,NSLNSLnet *net,int ta)
{ float sum=(float)0,sa; char *vec=(char *)vvec;
  while(num>0) {sa=vec_elm(vec,net,ta); sum+=sa*sa; vec+=siz; --num;}
  if(sum<(float)0) return((float)0);
  return((float)sqrt(sum));
}

float vec_prod(void *vva,void *vvb,
			   int siza,int sizb,int num,
			   NSLNSLnet *anet,NSLNSLnet *bnet,
			   int ta,int tb)
{ float sum=(float)0; char *va=(char *)vva,*vb=(char *)vvb;
  while(num>0)
    {sum+=vec_elm(va,anet,ta)*vec_elm(vb,bnet,tb); va+=siza; vb+=sizb; --num;}
  return(sum);
}
/* return linear product of two vectors: sum = ( (va*gg+kk)*vb ) */
float vec_liprod(void *fva,void *fvb, 
				 int siza,int sizb,int num,
				 NSLNSLnet *anet,NSLNSLnet *bnet,
				 int ta,int tb, double gg,double kk)
{ float sum=(float)0; char *va=(char *)fva,*vb=(char *)fvb;
  while(num>0)
    {sum+=(vec_elm(va,anet,ta) * (float)gg + (float)kk) * vec_elm(vb,bnet,tb);
     va+=siza; vb+=sizb; --num;}
  return(sum);
}
