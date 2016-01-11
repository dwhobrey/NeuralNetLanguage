/* Name: nsliol.h - Large Model I/O & Monitor Definitions
*  NeSeL v2.0 April 1989  Neural Network Specification Language
*/

char nsl_inc(long);
short nsl_ins(long);
int nsl_ini(long);
long nsl_inl(long);
float nsl_inf(long);
double nsl_ind(long);
char *nsl_inp(long,int);
char *nsl_dsp(long,char *,int);
int nsl_wdsp(long,char *,char *,int);

extern int nsl_stop,nsl_abort,nsl_err,nsl_verbatim;

#define inc(x) (char)(nsl_inc(x))
#define ins(x) (short)(nsl_ins(x))
#define ini(x) (int)(nsl_ini(x))
#define inl(x) (long)(nsl_inl(x))
#define inf(x) (float)(nsl_inf(x))
#define ind(x) (double)(nsl_ind(x))
#define inp(x) nsl_inp(x,0)

#define incp(x) (char *)(nsl_inp((long)x,1))
#define insp(x) (short *)(nsl_inp((long)x,2))
#define inip(x) (int *)(nsl_inp((long)x,3))
#define inlp(x) (long *)(nsl_inp((long)x,4))
#define infp(x) (float *)(nsl_inp((long)x,5))
#define indp(x) (double *)(nsl_inp((long)x,6))

#define dsnp(n,t) ((t *)nsl_dsp((long)n,(char *)0L,0))
#define dsvp(n,t,p) (nsl_dsp((long)n,#p,0))
#define dsc(n,t,p) (*((char *)nsl_dsp((long)n,#p,0)))
#define dss(n,t,p) (*((short *)nsl_dsp((long)n,#p,0)))
#define dsi(n,t,p) (*((int *)nsl_dsp((long)n,#p,0)))
#define dsl(n,t,p) (*((long *)nsl_dsp((long)n,#p,0)))
#define dsf(n,t,p) (*((float *)nsl_dsp((long)n,#p,0)))
#define dsd(n,t,p) (*((double *)nsl_dsp((long)n,#p,0)))
#define dsp(n,t,p) (*((void **)nsl_dsp((long)n,#p,0)))

#define rdsnp(n,t) ((t *)nsl_dsp((long)n,(char *)0L,1))
#define rdsvp(n,t,p) (nsl_dsp((long)n,#p,1))
#define rdsc(n,t,p) (*((char *)nsl_dsp((long)n,#p,1)))
#define rdss(n,t,p) (*((short *)nsl_dsp((long)n,#p,1)))
#define rdsi(n,t,p) (*((int *)nsl_dsp((long)n,#p,1)))
#define rdsl(n,t,p) (*((long *)nsl_dsp((long)n,#p,1)))
#define rdsf(n,t,p) (*((float *)nsl_dsp((long)n,#p,1)))
#define rdsd(n,t,p) (*((double *)nsl_dsp((long)n,#p,1)))
#define rdsp(n,t,p) (*((void **)nsl_dsp((long)n,#p,1)))

#define wdsnp(n,t,v) nsl_wdsp((long)n,(char *)0L,(v),0)
#define wdsvp(n,t,p,v) nsl_wdsp((long)n,#p,(v),0)
#define wdsc(n,t,p,v) nsl_wdsp((long)n,#p,&(v),1)
#define wdss(n,t,p,v) nsl_wdsp((long)n,#p,&(v),2)
#define wdsi(n,t,p,v) nsl_wdsp((long)n,#p,&(v),3)
#define wdsl(n,t,p,v) nsl_wdsp((long)n,#p,&(v),4)
#define wdsf(n,t,p,v) nsl_wdsp((long)n,#p,&(v),5)
#define wdsd(n,t,p,v) nsl_wdsp((long)n,#p,&(v),6)
#define wdsp(n,t,p,v) nsl_wdsp((long)n,#p,&(v),7)

