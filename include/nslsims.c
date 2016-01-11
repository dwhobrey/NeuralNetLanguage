/* Name: nslsims.c - Small Model Simulator
*  NeSeL v1.0 April 1989  Neural Network Specification Language
*/

int nsl_stop=0,nsl_abort=0,nsl_err=NSLER_OK,nsl_verbatim=0;

int nsl_driver(netpt,cmd,args)
  NSLNSLnet *netpt; int cmd; char *args;
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
