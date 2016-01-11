/* Name: nslios.h - Small Model I/O & Monitor Definitions
*  NeSeL v2.0 April 1989  Neural Network Specification Language
*/
#ifndef _nslios_h
#define _nslios_h

#ifdef __cplusplus
extern "C"
{
#endif

/* IO driver services */
#define NSLIO_NULL 0 /* cmd issued during a normal node call */
#define NSLIO_SETUP 1 /* setup & initialise driver */
#define NSLIO_INIT 2 /* reinitialise driver */
#define NSLIO_CLOSE 3 /* close / terminate net io */
#define NSLIO_REDRAW 4 /* redraw windows etc */
#define NSLIO_PATTERN 5 /* apply new pattern */
#define NSLIO_PRE 6 /* driver call prior to net cycle */
#define NSLIO_POST 7 /* driver call after net cycle */
#define NSLIO_USER 8 /* user definable */

/* connection input macros */
#define inc(n) (*(net->cpt[(int)(n)].cpt))
#define ins(n) (*((short *)(net->cpt[(int)(n)].cpt)))
#define ini(n) (*((int *)(net->cpt[(int)(n)].cpt)))
#define inl(n) (*((long *)(net->cpt[(int)(n)].cpt)))
#define inf(n) (*((float *)(net->cpt[(int)(n)].cpt)))
#define ind(n) (*((double *)(net->cpt[(int)(n)].cpt)))
#define inx(t,n) (*((t *)(net->cpt[(int)(n)].cpt)))
#define inp(n) (net->cpt[(int)(n)].cpt)

#define incp(n) ((char *)(net->cpt[(int)(n)].cpt))
#define insp(n) ((short *)(net->cpt[(int)(n)].cpt))
#define inip(n) ((int *)(net->cpt[(int)(n)].cpt))
#define inlp(n) ((long *)(net->cpt[(int)(n)].cpt))
#define infp(n) ((float *)(net->cpt[(int)(n)].cpt))
#define indp(n) ((double *)(net->cpt[(int)(n)].cpt))
#define inxp(t,n) ((t *)(net->cpt[(int)(n)].cpt))

/* node data structure reference macros */
#define dsnp(n,t) ((t *)(net->npt[(int)(n)-1].dpt))
#define dsvp(n,t,p) (&((t *)(net->npt[(int)(n)-1].dpt))->p)
#define dsc(n,t,p) ((char)((t *)(net->npt[(int)(n)-1].dpt))->p)
#define dss(n,t,p) ((short)((t *)(net->npt[(int)(n)-1].dpt))->p)
#define dsi(n,t,p) ((int)((t *)(net->npt[(int)(n)-1].dpt))->p)
#define dsl(n,t,p) ((long)((t *)(net->npt[(int)(n)-1].dpt))->p)
#define dsf(n,t,p) ((float)((t *)(net->npt[(int)(n)-1].dpt))->p)
#define dsd(n,t,p) ((double)((t *)(net->npt[(int)(n)-1].dpt))->p)
#define dsx(x,n,t,p) ((x)((t *)(net->npt[(int)(n)-1].dpt))->p)
#define dsp(n,t,p) (((t *)(net->npt[(int)(n)-1].dpt))->p)

#define rdsnp(n,t) ((t *)(net->npt[(int)(n)-1].dpt))
#define rdsvp(n,t,p) (&((t *)(net->npt[(int)(n)-1].dpt))->p)
#define rdsc(n,t,p) ((char)((t *)(net->npt[(int)(n)-1].dpt))->p)
#define rdss(n,t,p) ((short)((t *)(net->npt[(int)(n)-1].dpt))->p)
#define rdsi(n,t,p) ((int)((t *)(net->npt[(int)(n)-1].dpt))->p)
#define rdsl(n,t,p) ((long)((t *)(net->npt[(int)(n)-1].dpt))->p)
#define rdsf(n,t,p) ((float)((t *)(net->npt[(int)(n)-1].dpt))->p)
#define rdsd(n,t,p) ((double)((t *)(net->npt[(int)(n)-1].dpt))->p)
#define rdsx(x,n,t,p) ((x)((t *)(net->npt[(int)(n)-1].dpt))->p)
#define rdsp(n,t,p) (((t *)(net->npt[(int)(n)-1].dpt))->p)

#define wdsnp(n,t,v) ((t *)(net->npt[(int)(n)-1].dpt))=(v)
#define wdsvp(n,t,p,v) (&((t *)(net->npt[(int)(n)-1].dpt))->p)=(v)
#define wdsc(n,t,p,v) ((char)((t *)(net->npt[(int)(n)-1].dpt))->p)=(v)
#define wdss(n,t,p,v) ((short)((t *)(net->npt[(int)(n)-1].dpt))->p)=(v)
#define wdsi(n,t,p,v) ((int)((t *)(net->npt[(int)(n)-1].dpt))->p)=(v)
#define wdsl(n,t,p,v) ((long)((t *)(net->npt[(int)(n)-1].dpt))->p)=(v)
#define wdsf(n,t,p,v) ((float)((t *)(net->npt[(int)(n)-1].dpt))->p)=(v)
#define wdsd(n,t,p,v) ((double)((t *)(net->npt[(int)(n)-1].dpt))->p)=(v)
#define wdsx(x,n,t,p,v) ((x)((t *)(net->npt[(int)(n)-1].dpt))->p)=(v)
#define wdsp(n,t,p,v) (((t *)(net->npt[(int)(n)-1].dpt))->p)=(v)

extern int nsl_stop,nsl_abort,nsl_err,nsl_verbatim;
extern int nsl_mon_min,nsl_mon_max,nsl_mon_rate,nsl_mon_delay;

extern NSLNSLnet **nsl_net_list;

/* Core vars from nslmonfn.c */
extern char *nsl_mon_names[];
extern NSLNSLfelm nsl_mon_list[];
extern NSLNSLstruct nsl_mon_structs[];
extern NSLNSLvar nsl_mon_vars[];
extern int nsl_mon_stindx[];
extern int nsl_num_mon_list,nsl_num_mon_structs,
     nsl_num_mon_stindx,nsl_num_mon_vars;
extern NSLNSLfval nsl_mon_evaluate();

extern void nsl_mon_wnds(void);
extern int nsl_monitor(char *);
extern int nsl_install(NSLNSLnet *,char *);
extern int nsl_uninstall(char *);
extern int nsl_order(char *,void (*)());
extern int nsl_driver(NSLNSLnet *,int,...);
extern int nsl_cycle(NSLNSLnet *,long);
extern int nsl_put(NSLNSLnet *,char *);
extern NSLNSLnet *nsl_get(char *);
extern NSLNSLnet *nsl_get_p(char *);
extern void nsl_free(NSLNSLnet *);
extern long nsl_handle(NSLNSLnet *,char *);
extern void *nsl_get_user_args(NSLNSLnet *netpt);
extern void nsl_set_user_args(NSLNSLnet *netpt,void *args);
extern int nsl_write(NSLNSLnet *,long,char *,double);
extern int nsl_write_p(NSLNSLnet *,long,char *,char *);
extern double nsl_read(NSLNSLnet *,long,char *);
extern char *nsl_read_p(NSLNSLnet *,long,char *);
extern char *nsl_adjust(NSLNSLnet *,char *);
extern float
  vec_sum(void *,int,int,NSLNSLnet *,int),
  vec_norm(void *,int,int,NSLNSLnet *,int),
  vec_prod(void *,void *,int,int,int,NSLNSLnet *,NSLNSLnet *,
	int,int),
  vec_liprod(void *,void *,int,int,int,NSLNSLnet *,NSLNSLnet *,
	int,int,double,double);

#define nsl_drv(ty) ((ty *)(net->npt[net->dpt->nd-1].dpt))
#define sumc(a,b,n,i) vec_sum(b,sizeof(*(a)),n,i,TYPE_CHAR)
#define sums(a,b,n,i) vec_sum(b,sizeof(*(a)),n,i,TYPE_SHORT)
#define sumi(a,b,n,i) vec_sum(b,sizeof(*(a)),n,i,TYPE_INT)
#define suml(a,b,n,i) vec_sum(b,sizeof(*(a)),n,i,TYPE_LONG)
#define sumf(a,b,n,i) vec_sum(b,sizeof(*(a)),n,i,TYPE_FLOAT)
#define sumd(a,b,n,i) vec_sum(b,sizeof(*(a)),n,i,TYPE_DOUBLE)
#define normc(a,b,n,i) vec_norm(b,sizeof(*(a)),n,i,TYPE_CHAR)
#define norms(a,b,n,i) vec_norm(b,sizeof(*(a)),n,i,TYPE_SHORT)
#define normi(a,b,n,i) vec_norm(b,sizeof(*(a)),n,i,TYPE_INT)
#define norml(a,b,n,i) vec_norm(b,sizeof(*(a)),n,i,TYPE_LONG)
#define normf(a,b,n,i) vec_norm(b,sizeof(*(a)),n,i,TYPE_FLOAT)
#define normd(a,b,n,i) vec_norm(b,sizeof(*(a)),n,i,TYPE_DOUBLE)
#define prodcf(a,b,c,d,n,i,k) vec_prod(b,d,sizeof(*(a)),sizeof(*(c)),\
  n,i,k,TYPE_CHAR,TYPE_FLOAT)
#define prodsf(a,b,c,d,n,i,k) vec_prod(b,d,sizeof(*(a)),sizeof(*(c)),\
  n,i,k,TYPE_SHORT,TYPE_FLOAT)
#define prodif(a,b,c,d,n,i,k) vec_prod(b,d,sizeof(*(a)),sizeof(*(c)),\
  n,i,k,TYPE_INT,TYPE_FLOAT)
#define prodff(a,b,c,d,n,i,k) vec_prod(b,d,sizeof(*(a)),sizeof(*(c)),\
  n,i,k,TYPE_FLOAT,TYPE_FLOAT)
#define liprodcf(a,b,c,d,n,i,k,p,q) vec_liprod(b,d,sizeof(*(a)),sizeof(*(c)),\
  n,i,k,TYPE_CHAR,TYPE_FLOAT,p,q)
#define liprodsf(a,b,c,d,n,i,k,p,q) vec_liprod(b,d,sizeof(*(a)),sizeof(*(c)),\
  n,i,k,TYPE_SHORT,TYPE_FLOAT,p,q)
#define liprodif(a,b,c,d,n,i,k,p,q) vec_liprod(b,d,sizeof(*(a)),sizeof(*(c)),\
  n,i,k,TYPE_INT,TYPE_FLOAT,p,q)
#define liprodff(a,b,c,d,n,i,k,p,q) vec_liprod(b,d,sizeof(*(a)),sizeof(*(c)),\
  n,i,k,TYPE_FLOAT,TYPE_FLOAT,p,q)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*_nslios_h*/
