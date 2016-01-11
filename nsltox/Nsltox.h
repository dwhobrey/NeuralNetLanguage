/* Name: nsltox.h	Type: header file	Date: 30 March 89
*	NeSeL Network Specification Language
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <varargs.h>
#include <signal.h>
#include <process.h>
#include <malloc.h>

#ifndef size_t
typedef unsigned int size_t;
#endif

typedef unsigned char boolean; /* Just for clarity */
typedef unsigned char byte; /* Hopefully an 8-bit value */
typedef char siby; /* signed byte value */
typedef unsigned short int word; /* Hopefully 16-bit value */
typedef short int mile; /* array indexing values */
typedef long yard; /* node number values */
typedef long foot; /* file position pointer values */
typedef double reals; /* the type of reals */
typedef word door; /* size of a nal block in multiples of nal_page_size */

/* DS type codes */
#define TYPE_UNKNOWN 0 /* unknown type */
#define TYPE_CHAR 1
#define TYPE_DOUBLE 2
#define TYPE_FLOAT 3
#define TYPE_INT 4
#define TYPE_LONG 5
#define TYPE_VOID 6
#define TYPE_NODE 7
#define TYPE_STRUCT 8
#define TYPE_UNION 9
#define TYPE_SHORT 10
#define TYPE_USCHAR 11
#define TYPE_USINT 12
#define TYPE_USLONG 13
#define TYPE_USSHORT 14
#define TYPE_PT 15
#define TYPE_NET 16 /* decl is a net */
#define TYPE_ARB 17 /* decl is an array base */
#define SIP_STR 18 /* literal string */
#define SIP_NODE 19 /* symbolic node pt */

#define MILE_TYPE TYPE_SHORT

#define STAR_MASK 0x60
#define STAR_TYPE 0x1f
#define STAR_ONE 0x20
#define STAR_SHFT 5

#define LST_SYS 5 /* number of reserved elements in a list */
#define LST_MEM (LST_SYS*sizeof(mile)) /* mem overheads for storing lst */
#define LST_MORE 8 /* space allocation page size in elms for lmore() */
#define LST_CODE 0x5679 /* sys code for checking list is valid */
#define LST_TYPE 0x1f /* mask for list type */
#define LST_PBIT 0x40 /* type bit set when list is in pair format */
#define LST_PAIR (0xc0+MILE_TYPE) /* pair format list type code */
#define lst_num(lp) (((mile *)(lp))[-1]) /* access number elms in lst */
#define lst_max(lp) (((mile *)(lp))[-2]) /* access max elms allowed in lst */
#define lst_siz(lp) (((mile *)(lp))[-3]) /* access size of list elm */
#define lst_typ(lp) (((mile *)(lp))[-4]) /* access elm & list type */
#define lst_code(lp) (((mile *)(lp))[-5]) /* access list code */
#define lst_head(lp,ty) ((ty *)(lp)) /* pt to 1st elm */
#define lst_tail(lp,ty) ((ty *)((char *)(lp)+(lst_num(lp)-1)*lst_siz(lp)))
#define lst_base(lp) ((char *)(lp)-LST_MEM) /* pt to 1st byte of lst sys */
#define lst_end(lp) ((char *)(lp)+lst_num(lp)*lst_siz(lp)) /*end byte+1*/
#define lst_ok(lp) ((lp)?lst_num(lp):(mile)0) /* returns num elms if lp ok */
#define lst_valid(lp) (lst_code(lp)==LST_CODE) /* returns T if list ok */

#define T ((boolean)1) /* value of boolean True */
#define F ((boolean)0) /* value of boolean False */

/* Termination number codes */
#define EXIT_OK ((int)0) /* exit successfully */
#define EXIT_ERRERR ((int)1) /* unknown error */
#define EXIT_INT ((int)2) /* exit on interrupt */
#define EXIT_BADFILE ((int)3) /* No (accessible) input/output file */
#define EXIT_OSPACE ((int)4) /* Ran out of memory space */
#define EXIT_EOF ((int)5) /* EOF encountered prematurely */
#define EXIT_FILE ((int)8) /* Access to file went wrong */
#define EXIT_INT_ERR ((int)9) /* Something has gone wrong internally */
#define EXIT_VERSION ((int)14) /* Bad nsl1 object module version number */
#define EXIT_DSP_FUL ((int)15) /* Ds pool queue overflow */
#define EXIT_BADNSS ((int)17) /* Bad nss file */

#define ERR_BASE ((int)20) /* first nonterminating error code */
#define ERR_OK ((int)0) /* must have same code as EXIT_OK */

#define ERR_NET_CHK (ERR_BASE+1) /* net consistancy is suspect */

#define WRN_BASE (ERR_BASE+10) /* first nonterminating warning code */
#define WRN_STR_UNIT (WRN_BASE+1) /* undefined struct node member */
#define WRN_VAR_UNIT (WRN_BASE+2) /* undefined var node member */
#define WRN_UA_UNIT (WRN_BASE+3) /* undefined var node member */
#define WRN_DRV_FUL (WRN_BASE+4) /* max size of NSLNSLdrv list exceeded */

#define WRN_MAX (WRN_BASE+10)

#define namesize 40
#define buffsize 250
#define bigbuffsize 2000

#ifndef SEEK_SET
#define SEEK_SET 0 /* this should be defined in stdio.h */
#endif
#ifndef SEEK_END
#define SEEK_END 2 /* so should this */
#endif

#define argstr1 "%40s" /* this 40 is the value of namesize */
#define argstr2 ".%40s"
#define ds_sea_max 10 /* size of ds buffer */
#define flo_str "(float)%g"  /* cast all float values */
#define MAX_LEVELS 128
#define MAX_TYPES 14
#define MAX_DRVS 512
/* def of net flags for NSLNSLnet */
#define NET_CDECL 1 /* net was declared and not loaded */
#define NET_CXMAP 2 /* reconstruct cx map on loading */
#define NET_ACTIV 4 /* net is active (monitor flag) */
#define NET_FREE 8 /* net has been freed (monitor flag) */
#define NET_STUB 16 /* net is stub only, no DS etc */
#define NET_FLAGS (NET_CDECL+NET_CXMAP)

/* module declaration info */
typedef struct _decl_elm
{ siby decltype; /* type of decl, net, node, struct etc */
  mile declnum, /* id number of decl */
	size,nodes;
  yard nodenum;
  foot sst_pos; /* file pos */
  char *name,
	*tag; /* struct type tag */
  struct _decl_elm *tail;
} decl_elm;

typedef struct _net_elm
{ int refs;
  boolean root_flag:1;
  mile ners,subofs;
  foot sst_pos;
  char *tag,*name;
  decl_elm *decls;
  struct _net_elm *tail;
} net_elm;

typedef struct _path_elm
{ struct _path_elm *parent,*tail;
  net_elm *net;
  decl_elm *decl_pt;
  char *pat;
  int uasize;
} path_elm;

typedef struct _di_elm
{ mile decl; /* decl index in net to branch at */
  mile indx; /* index into decl to branch at */
} di_elm;

typedef struct _node_elm /* node address value */
{ yard base;
  int di_num;/*DEBUG was 16bit word, but sizeof(node_elm) gets padded to 8*/
			 /* we do inline mem comparisons: padding could be garbage!*/
} node_elm;

typedef struct _sip_ion
{ word in_num; /* number of inputs to limb */
  word out_num; /* number of outputs from limb */
} sip_ion;

typedef struct _sip_dat
{ mile size; /* size of element if it's an array */
  mile elm;  /* the array member number of this element */
} sip_dat;

typedef union _sip_inf
{ yard nodenum; /* holds nodenum val */
  sip_dat data;
} sip_inf;

typedef struct _str_elm
{ word len;
  char base;
} str_elm;

typedef union _sip_val
{ long num; /* holds big ints */
  int nat; /* holds ints */
  sip_ion conx;
  node_elm node; /* symbolic node pointers */
  str_elm str;  /* holds chars and strings: we malloc space after char */
  char *func; /* function pointers etc */
  float floa;
  double doub; /* holds reals */
} sip_val;

/* Meaning of the conds in stub_elm's */
#define NISNUL 0 /* bits not set */
#define NISSTR 1 /* value stored as a char string, but still of type 'type' */
#define NISNODE 2 /* indicates when struct is 'node' struct */
#define NISINFO 3 /* indicates when one of info structs: ins,outs,size */
/* this struct is used to build an internal rep of node state info */
typedef struct _stub_elm
{ boolean /* all these bits should fit into one byte */
	hasparent:1, /* indicates if sip has parent */
	hassubpt:1,/* indicates substruct if this elm was a substruct */
	hasarypt:1,/* indicates nxt array member if any */
	hastail:1, /* indicates nxt struct elm at same lvl */
	isdef:1, /* elm value has been defined */
	isvar:1, /* set if val was initialised by an inline var */
	conds:2; /* indicates special conditions */
  siby type;  /* type of struct element */
  sip_inf info; /* holds either nodenum if parent sip, or size & elm details*/
} stub_elm;

typedef struct _ds_sea_elm
{ yard nodenum;
  foot sst_pos;
  foot size;
  char *ds_base; /* base adrs of ds memory */
  char *ds_end; /* end of ds block in use */
} ds_sea_elm;

#define SIZ_SIP (sizeof(stub_elm)+sizeof(word))
#define SIZ_SIP_STR (SIZ_SIP+sizeof(word)+1)
#define SIZ_SIP_INT (SIZ_SIP+sizeof(int)+1)
#define SIZ_SIP_LONG (SIZ_SIP+sizeof(long)+1)
#define SIZ_SIP_FLOAT (SIZ_SIP+sizeof(float)+1)
#define SIZ_SIP_DOUB (SIZ_SIP+sizeof(double)+1)
#define SIZ_SIP_PT (SIZ_SIP+sizeof(long *)+1)
#define SIZ_SIP_ION (SIZ_SIP+sizeof(long *))
#define SIZ_SIP_NODE (SIZ_SIP+sizeof(node_elm))

#define malmem(tt,nn) ((tt *)mem_mal((size_t)(nn)*sizeof(tt)))
#define relmem(tt,nn,pp,ss) ((tt *)mem_rel(pp,(size_t)(nn),(size_t)(ss)))

#define fremem(p) free(p)
#define chk_free(p) if(p)fremem(p)
#define nul_free(p) if(p){fremem(p);p=NULL;}

#define magy(val) (((val)>=0)?(val):(-(val)))
#define isident(c) ((boolean)(isalnum(c) || c=='_'))

#define node_size(npt) (sizeof(node_elm)+((npt)->di_num)*sizeof(di_elm))
#define node_di(npt) ((di_elm *)(npt+1))

#define ds_value(spt) ((sip_val *)(((stub_elm *)(spt))+1))
#define ds_prior(p) ((stub_elm *)(((char *)(p))- *(((word *)(p))-1)))
#define ds_next(p) ((stub_elm *)(((char *)(p))+stub_size(p)))
#define ds_subpt(p) ((((stub_elm *)(p))->hassubpt)?ds_next(p):NULL)

#define fput_data(fpt,adrs,len) fwrite(adrs,len,(size_t)1,fpt)
#define fget_data(fpt,adrs,len) fread(adrs,len,(size_t)1,fpt)
#define fmoveto(fpt,pos) fseek(fpt,pos,SEEK_SET)
#define fposeof(fpt) fseek(fpt,(foot)0,SEEK_END)

/*----------------------------------------------------------------------*/
boolean
  opt_exe,opt_txt,opt_name,opt_idx,opt_stub;
int
  raise_error,num_errors,warn_limit,
  cx_num,cx_max,func_num,
  uba_num,uba_max;
yard
  index_num;
foot
  func_bs,index_base,sst_bs,
  *cx_list,*func_sst_list,
  *conx_pt,*uba_stk;
char
  **func_list,*net_name;
path_elm
  *path_pt, /* working path pt */
  *rootpt; /* root of reference path */
ds_sea_elm ds_pool; /* cur node data-structure pool */
stub_elm *ds_stub; /* cur node data-structure pt */
node_elm *node_pt,*cx_pt;
di_elm *di_pt;
FILE
  *logfp, /* file for error reporting */
  *nalfp, /* nal object file */
  *confp, /* list of connections */
  *sstfp,*txtfp, /* substantiated file pts */
  *symfp, /* textual output file */
  *tmpfp, /* temporary file for building a nodes ds */
  *tabfp; /* tmp file for final conx list */
