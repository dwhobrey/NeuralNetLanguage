/* NeSeL v2.0 April 1989 */
/* BAM Stats gathering program : optimised based on NeSeL output */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "memtrack.h"
/*#include <malloc.h> */

#include <stdarg.h>
#include <math.h>

#include "nsltype.h"
#include "nslstd.h"
#include "nslios.h"
#include "nslwsio.h"

/*#include "bam.h"*/
#include "bam.nsh"

/* define this for nodes to take average input */
/* #define TRANSDUCE */

typedef int quale;
#define QSCALE ((quant)254)
#define QHALF ((quant)127)
#define EMPTY ((quant)(QSCALE+1))

#define TABLE_X 32
#define TABLE_Y 110
#define DIST_X 32
#define DIST_Y 100
#define DIST_H (DIST_Y/2)
#define DIST_M (DIST_H-1)
#define DIST_D (DIST_Y-1)

FILE *bam_logfp=NULL,*pat_logfp=NULL;
/* we use two p.r.n.g's: one for patterns & one for comms */
long com_aseed=123,com_bseed=7131,com_cseed=30013;
long pat_aseed=123,pat_bseed=7131,pat_cseed=30013;
int samples;
int stats=0,max_path=0,max_tab=0,curcc,nsl_stop;
int dist_table[DIST_X][DIST_Y+1] = {0};
unsigned long paths[TABLE_Y] = {0};
unsigned long table[TABLE_X][TABLE_Y] = {0};

#define qua(n,t,p) ((quant)((t *)(net->npt[(int)(n)-1].dpt))->p)
#define inqua(x) (*((quant *)(net->cpt[(int)(x)].cpt)))

/* return random number, 0.0 <= rndf() < 1.0 */
float comm_rndf()
{float temp;
 com_aseed=171*(com_aseed % 177)-2*(com_aseed/177);
 if(com_aseed<0) com_aseed+=30269;
 com_bseed=172*(com_bseed % 176)-35*(com_bseed/176);
 if(com_bseed<0) com_bseed+=30307;
 com_cseed=170*(com_cseed % 178)-63*(com_cseed/178);
 if(com_cseed<0) com_cseed+=30323;
 temp=(float)com_aseed/(float)30269.0
      +(float)com_bseed/(float)30307.0+(float)com_cseed/(float)30323.0;
 return(temp-(int)temp);
}
float pat_rndf()
{float temp;
 pat_aseed=171*(pat_aseed % 177)-2*(pat_aseed/177);
 if(pat_aseed<0) pat_aseed+=30269;
 pat_bseed=172*(pat_bseed % 176)-35*(pat_bseed/176);
 if(pat_bseed<0) pat_bseed+=30307;
 pat_cseed=170*(pat_cseed % 178)-63*(pat_cseed/178);
 if(pat_cseed<0) pat_cseed+=30323;
 temp=(float)pat_aseed/(float)30269.0
      +(float)pat_bseed/(float)30307.0+(float)pat_cseed/(float)30323.0;
 return(temp-(int)temp);
}

/* return linear product of two vectors: sum = ( (va*gg+kk)*vb ) 
float vec_liprod(va,vb,num)
  char *va,*vb; int int;
{ float sum=(float)0;
  while(num-->0)
    {sum+=((float)(*((quant *)va)) / ((float)QHALF)
           - (float)1.0) * *((float *)vb);
     va+=sizeof(delay_synapse); vb+=sizeof(delay_synapse);}
  return(sum);
}
*/
void clr_table()
{ int xx,yy;
  for(xx=0;xx<TABLE_X;++xx)
     for(yy=1;yy<TABLE_Y;++yy) table[xx][yy]=0;
}

void dsp_table()
{ int xx,yy;
  fprintf(pat_logfp,"#%d\n",stats);
  for(xx=0;xx<=max_tab;++xx)
     {fprintf(pat_logfp,"%d:%u:",xx,table[xx][0]);
      yy=1;
      while(table[xx][yy])
        fprintf(pat_logfp,"%u,",table[xx][yy++]);
      fprintf(pat_logfp,"\n");
     }
}

void dsp_dist()
{ int xx,yy,jj; long val;
  fprintf(pat_logfp,"%%%%\n");
  for(xx=0;xx<=max_tab;++xx)
     {jj=0; fprintf(pat_logfp,"%d:%d:",xx,dist_table[xx][0]);
      for(yy=1;yy<DIST_Y;++yy)
       {++jj; if(jj>10) {fprintf(pat_logfp,"\n"); jj=0;}
         fprintf(pat_logfp,"%d,",dist_table[xx][yy]);}
      fprintf(pat_logfp,"\n");
     }
  fprintf(bam_logfp,"%%%%\n");
  for(xx=0;xx<=max_path;++xx)
     {val=(long)(paths[xx]/samples);
      fprintf(bam_logfp,"%ld\n",val);
     }
}

float diff(float a,float b)
{ float cc;
  cc=(float)(a-b);
  if(cc>=(float)0.0) return(cc);
  cc=(float)(b-a);
  return(cc);
}

void dsp_stats(NSLNSLnet *net)
{ int rr,qq,pp,nn,num,parts,flds,outnum=0,probnum=0;
  hop_ctrl_ds *hp=nsl_drv(hop_ctrl_ds); bio_ds *np=(&hp->nio);
  float val,hamap,hamai,phamap,phamai,ham,prob;
  flds=np->fld_siz; num=dsi(np->flda,hop_tree_ds,size.part);
  parts=flds/num; ++stats;
  for(rr=0;rr<num_pats;++rr)
    {hamap=(float)0.0; hamai=(float)0.0; phamap=(float)0.0; phamai=(float)0.0;
     for(pp=0;pp<parts;++pp)
      {for(qq=0;qq<num;++qq)
         {nn=pp*num+qq;
          val=(float)((quant)qua(np->flda+pp,hop_tree_ds,part[qq].output))
            /((float)QSCALE);
          hamap+=diff((float)patsa[rr][nn],val);  val=(float)1.0-val;
          hamai+=diff((float)patsa[rr][nn],val);
          val=(float)((quant)qua(np->fldb+pp,hop_tree_ds,part[qq].output))
            /((float)QSCALE);
          hamap+=diff((float)patsb[rr][nn],val);  val=(float)1.0-val;
          hamai+=diff((float)patsb[rr][nn],val);
          val=dsf(np->ctrl,hop_ctrl_ds,output[nn]);
          phamap+=diff((float)patsa[rr][nn],val);  val=(float)1.0-val;
          phamai+=diff((float)patsa[rr][nn],val);
          val=dsf(np->ctrl,hop_ctrl_ds,output[nn+flds]);
          phamap+=diff((float)patsb[rr][nn],val);  val=(float)1.0-val;
          phamai+=diff((float)patsb[rr][nn],val);
         }
      }
    if(rr==0) {ham=hamap; prob=phamap;}
    if(hamap<ham) {outnum=2*rr; ham=hamap;}
    if(hamai<ham) {outnum=2*rr+1; ham=hamai;}
    if(phamap<prob) {probnum=2*rr; prob=phamap;}
    if(phamai<prob) {probnum=2*rr+1; prob=phamai;}
    }
  fprintf(bam_logfp,"%d,%d,%.3f,%.3f,%d,%d,",
     stats,curcc,ham,prob,outnum,probnum);
  dsp_table();
  clr_table();
}
void dsp_quick(NSLNSLnet *net)
{ int rr,qq,pp,nn,num,parts,flds,outnum=0;
  hop_ctrl_ds *hp=nsl_drv(hop_ctrl_ds); bio_ds *np=(&hp->nio);
  float val,hamap,hamai,ham;
  flds=np->fld_siz; num=dsi(np->flda,hop_tree_ds,size.part);
  parts=flds/num;
  for(rr=0;rr<num_pats;++rr)
    {hamap=(float)0.0; hamai=(float)0.0;
     for(pp=0;pp<parts;++pp)
      {for(qq=0;qq<num;++qq)
         {nn=pp*num+qq;
          val=(float)((quant)qua(np->flda+pp,hop_tree_ds,part[qq].output))
            /((float)QSCALE);
          hamap+=diff((float)patsa[rr][nn],val);  val=(float)1.0-val;
          hamai+=diff((float)patsa[rr][nn],val);
          val=(float)((quant)qua(np->fldb+pp,hop_tree_ds,part[qq].output))
            /((float)QSCALE);
          hamap+=diff((float)patsb[rr][nn],val);  val=(float)1.0-val;
          hamai+=diff((float)patsb[rr][nn],val);
         }
      }
    if(rr==0) ham=hamap;
    if(hamap<ham) {outnum=2*rr; ham=hamap;}
    if(hamai<ham) {outnum=2*rr+1; ham=hamai;}
    }
  fprintf(bam_logfp,"%d,%.3f,%d\n",curcc,ham,outnum);
}

int delta(int a,int b,int x)
{ int d=a-b;
  if(d<0) d=(-d);
  if(d>(x/2)) {d=1+x-d; if(x%2) --d;}
  return(d);
}
/* max delayed comm path between nodes */
int flush_max(NSLNSLnet *net)
{ hop_ctrl_ds *hp=nsl_drv(hop_ctrl_ds); bio_ds *np=(&hp->nio);
  int len=np->xdim;
  len+=(int)((double)6.0 * (double)len/base_sigma);
  return(len);
}
/* works out minimum torus path length between node numbers a & b */
int com_len(NSLNSLnet *net,int a,int b)
{ hop_ctrl_ds *hp=nsl_drv(hop_ctrl_ds); bio_ds *np=(&hp->nio);
  int len,xd=np->xdim;
  len=base_len+delta(a%xd,b%xd,xd)+delta(a/xd,b/xd,xd);
  return(len);
}
#define MY_PIE ((double)3.141592654)
double gauss(xx,kk,mm,cc) double xx,kk,mm,cc;
{ xx-=mm; xx*=xx; xx/=kk;
  return(exp(-xx)/cc);
}

/* work out quick look up table for path length normal distribution */
void calc_dist(int len,int max)
{ double cc,mm,kk,ss,x1,x2,xx,dd,sum=(double)0.0,aa;
  int ff,bb,nn,no,iy,ix=1;
  dist_table[len][0]=(int)max;
  x1=(double)len; x2=(double)max; nn=(int)x1;
  x1-=(double)0.5; x2+=(double)0.5;
  mm=(x1+x2)/(double)2.0;
  dd=(double)0.02; ff=50;
  if(len<1) goto skip;
  ss=(double)len/(double)base_sigma;  /* approximation for sigma */
  kk=(double)2.0*ss*ss;
  cc=ss*(double)sqrt((double)2.0*MY_PIE);
  for(xx=x1;xx<x2;xx+=dd) sum+=gauss(xx,kk,mm,cc);
  xx=x1;
  while(ix<DIST_H)
    {iy=ff; aa=(double)0.0;
     while(iy--) {aa+=gauss(xx,kk,mm,cc); xx+=dd;}
     iy=(int)(aa*(double)DIST_D/sum);
     if(iy<1) iy=1;
     if(xx+dd>mm) iy=DIST_H;
     while(iy--&&ix<DIST_H) dist_table[len][ix++]=(int)nn;
     ++nn;
    } skip:
  nn=(int)dist_table[len][DIST_M]; dist_table[len][DIST_H]=(int)nn;
  ix=DIST_H; iy=DIST_H+1; no=nn; bb=nn;
  if(bb<max) {if(len%2) ++bb; else if(max%2) ++bb;}
  while(iy<DIST_Y)
    {nn=(int)dist_table[len][ix];
     if(nn!=no) {if(bb<max) bb++; no=nn;}
     dist_table[len][iy]=(int)bb;
     ++iy; --ix;
    }
  for(iy=1;iy<DIST_Y;++iy) {if(dist_table[len][iy]) --dist_table[len][iy];}
}

/* allocate mem for comm paths */
void com_mem(NSLNSLnet *net,delay_synapse *dp, 
	int pp,int size,int nn,long num)
{ hop_ctrl_ds *hp=nsl_drv(hop_ctrl_ds); bio_ds *np=(&hp->nio);
  delay_synapse *cp=dp; quant *apt,*bpt;
  int ww,xx,yy,zz,mm=0;
  if(num>=np->fldb)
    {zz=1; yy=1+np->fld_siz+(int)(num-np->fldb)*pp+nn;}
  else {zz=1+np->fld_siz; yy=1+(int)(num-np->flda)*pp+nn;}
  for(xx=0;xx<size;++xx)
    {dp->len=(int)com_len(net,(int)(zz+xx),yy);
     if(dp->len==0) dp->len=1;
     dp->max=(int)((int)dp->len+
       (int)((double)6.0 * (double)dp->len/base_sigma));  /* L + 6 sigma */
     mm+=dp->max+1;
     ww=dp->len; table[ww][0]=(unsigned)dp->max; if(ww>max_tab) max_tab=ww;
     if((int)dp->max>max_path) max_path=(int)dp->max;
     if(!dist_table[ww][0]) calc_dist(ww,(int)dp->max);
     ++dp;
    }
  if(!(apt=(quant *)malloc((unsigned int)mm*sizeof(quant))))
    {fprintf(stderr,"no space\n"); return;}
  bpt=apt;
  while(mm--) (*bpt++)=EMPTY;
  xx=size;
  while(xx--) {cp->delay=apt; apt+=cp->max+1; ++cp;}
}

/* a & b are [-1.0,+1.0] numbers coded into [0..QSCALE].
* Their average, after converting & reducing, is simply (a+b)/2. so... */
#define average(a,b) (quant)(((quale)(a)+(quale)(b))/2)

/* propagate signals down connection channels */
void com_delay(NSLNSLnet *net,delay_synapse *dp,
			   int pp,int size,int nn,long num)
{ quant *fp;
  if(base_nocomms)
    {while(size--) {dp->tap=inqua(dp->input); ++dp;}}
  else
    {if(!dp->len) com_mem(net,dp,pp,size,nn,num);
     while(size--)
      {pp=dp->max; nn=dp->len; fp=dp->delay;
       while(pp--) {*fp=(*(fp+1)); ++fp;}
       pp=(int)dist_table[nn][1+(int)(comm_rndf()*(float)DIST_D)];
       ++table[nn][2+pp-nn]; ++paths[pp];
       fp=dp->delay+pp;
       if(*fp==EMPTY) *fp=inqua(dp->input);
       else *fp=average(*fp,inqua(dp->input));
       if(*(fp=dp->delay)!=EMPTY)
#ifdef TRANSDUCE
            dp->tap=average(dp->tap,*fp); /*take running average as input*/
#else
            dp->tap=(*fp); /*take new input value as current input*/
#endif
       ++dp;
      }
   }
}

#include "bam.nsc"
#include "bam.nsd"

#ifdef TRANSDUCE
char transchar='d';
#else
char transchar='b';
#endif

/* Run from command line 
* Args: num_samples[1..n] base_len[0..5] base_sigma[1..30] base_rate
* eg bam 1 0 12
*/
int NSLmain(int argc,char *argv[])
{ int nn,xx; int do_stoch=0,bs,aa; char tchr='l',buff[20];
  NSLNSLnet *net= &network;
  hop_ctrl_ds *hp=nsl_drv(hop_ctrl_ds); bio_ds *np=(&hp->nio);

  bam_logfp=stdout; pat_logfp=stdout;

  sscanf(argv[1],"%d",&samples);
  sscanf(argv[3],"%d",&bs);
  base_sigma=(double)bs;
  sscanf(argv[2],"%d",&aa);
  if(argc>4) {do_stoch=1; --transchar;}
  if(aa<0)
    {tchr='f'; aa=(-aa); base_sigma+=(double)aa/(double)10.0; base_len=0;}
  else base_len=(int)aa;
  sprintf(buff,"%c%ds%d%c%d.dat",
    transchar,np->fld_siz,bs,tchr,aa);
  if(!(bam_logfp=fopen(buff,"w"))) return(0);
  sprintf(buff,"%c%ds%d%c%d.pat",
    transchar,np->fld_siz,bs,tchr,aa);
  if(!(pat_logfp=fopen(buff,"w"))) return(0);
  if(do_stoch)
    {sscanf(argv[4],"%lf",&base_rate);
     base_flush=2;
     if(base_rate>(double)0.19) base_cycles=20+(long)((double)20.0/base_rate);
     else base_cycles=100;
     printf("%d,%d,%lf\n",(int)base_flush,(int)base_cycles,(double)base_rate);
    }
  else {base_flush+=flush_max(net); base_cycles+=flush_max(net);}
  fprintf(bam_logfp,"%d,%d,%lf\n",(int)base_flush,(int)base_cycles,
    (double)base_rate);
  for(nn=0;nn<samples;++nn)
   {/* new pattern */printf("%d\n",nn);
    curcc=0;
    dsi(np->ctrl,hop_ctrl_ds,flush)=base_flush;
    for(xx=0;xx<np->num;++xx) dsf(np->ctrl,hop_ctrl_ds,output[xx])=pat_rndf();
    base_nocomms=do_stoch; /* comms on */
    base_stoch=do_stoch; /* stoch processing on */
    nsl_cycle(net,base_cycles);
    dsp_stats(net); /* dsp stats */
    curcc=0;
    dsi(np->ctrl,hop_ctrl_ds,flush)=base_flush;
    base_nocomms=1; /* comms off */
    base_stoch=0;
    nsl_cycle(net,base_cycles);
    dsp_quick(net);
   }
  dsp_dist();
  fclose(bam_logfp); fclose(pat_logfp);
  return(0);
}
