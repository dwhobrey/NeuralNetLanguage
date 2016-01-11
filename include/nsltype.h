/* Name: nsltype.c - Type Definitions
*  NeSeL v2.0 April 1989  Neural Network Specification Language
*/
#ifndef _nsltype_h
#define _nsltype_h

#ifdef __cplusplus
extern "C"
{
#endif

/* General types: */
#ifndef BOOL
/* NeSeL can't handle enums, so..
typedef enum {FALSE=0,TRUE=1} BOOL;
*/
typedef int BOOL;
#define FALSE 0
#define TRUE 1
#endif
typedef short int mile;  /* 16-bit quantity used for list values */
typedef long yard; /* node numbers & symbolic connection addresses */
typedef unsigned char boolean;

/* function handling types: */
typedef struct _NSLNSLfelm
{ short swt; /* switch number to use when calling func_evaluate() */
  char (*fn_pt)(), /* pt to user func */
   *ats; /* func return & arg type codes */
} NSLNSLfelm;
typedef union _NSLNSLftyp
{ char cc,*pp;short ss;int ii;long ll;float ff;double dd;
  unsigned char ucc;unsigned short uss;unsigned int uii;unsigned long ull;
}NSLNSLftyp;
typedef struct _NSLNSLfval
{ mile typ; NSLNSLftyp val;
}NSLNSLfval;
typedef union _NSLNSLfuni
{ void (*vv)(); char (*cc)(),*(*pp)();
  short (*ss)(); int (*ii)(); long (*ll)();
  float (*ff)(); double (*dd)();
  unsigned char (*ucc)(); unsigned short (*uss)();
  unsigned int (*uii)(); unsigned long (*ull)();
} NSLNSLfuni;

/* Node info types: */
typedef struct _NSLNSLid /* name and path of node */
{long name; char *path;
} NSLNSLid;

typedef struct _NSLNSLtime /* timing info for updating a node */
{int rate,wait;
} NSLNSLtime;

/* Monitor info types: */
typedef struct _NSLNSLstruct /* details on structs used by nodes */
{long sub_nodes,ary_size; int type_code; char *offset,*member,*tag;
} NSLNSLstruct;

typedef struct _NSLNSLvar /* details on global variables used */
{ short type_code; char *adrs,*name;
} NSLNSLvar;

typedef struct _NSLNSLio /* substruct for io driver args */
{ short cmd; char *args;
} NSLNSLio;

typedef struct _NSLNSLdrv /* list elm for net io drivers */
{ long nd; NSLNSLio *iop;
} NSLNSLdrv;

typedef struct _NSLNSLconx /* details for backing up connection outputs */
{short len; char *cpt,*dpt;
} NSLNSLconx;

typedef struct _NSLNSLnode /* for each node, its func id, DS pt & time info */
{short fnum; char *dpt; NSLNSLtime *tpt;
} NSLNSLnode;

typedef struct _NSLNSLnet /* all the details needed to 'run' a net */
{ int flags,   /* internal flags */
    num_funcs, /* num of different node funcs used by net */
    num_conxs, /* num of unique output connections in net */
    num_nodes; /* num of nodes in net */
  long time; /* current age of net in cycles */
  void (**fpt)(); /* addresses of node functions */
  long sst_bs,*sstpt; /* file positions of node function resource details */
  NSLNSLconx *cpt; /* pts to info on connections */
  NSLNSLnode *npt; /* pts to info on node data structures */
  char *name,*sst_name,*var_name,*ins_name;  /* name of net */
  int *num_ufs, /* pt to num of user functions */
    *num_sts,   /* pt to num of user structs */
    *num_six,   /* pt to num in struct index */
    *num_vas,   /* pt to num of user vars */
    *sixpt;    /* pt to struct index */
  void (*ept)(); /* func to eval user func */
  char **upt; /*  user func names */
  NSLNSLfelm *apt; /* user func addresses & arg details */
  NSLNSLstruct *spt; /* pt to struct details if set */
  NSLNSLvar *vpt; /* pt to var details if set */
  NSLNSLdrv *dpt; /* pt to net io node list if set */
  int num_drv; /* num of io drivers */
  void **mpt; /* list of pts to malloc'd net vars */
  void *args; /* user maintained pt */
} NSLNSLnet;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*_nsltype_h*/
