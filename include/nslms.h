/* Name: nslms.h   Type: Small Model Monitor  Date: 30 March 89
*  NeSeL  Neural Net Specification Language
*/
#ifndef _nslms_h
#define _nslms_h

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <process.h>
#include <malloc.h>
#include <stdarg.h>
#include "nsltype.h"
#include "nslstd.h"
#include "nslios.h"

#ifndef size_t
typedef unsigned int size_t;
#endif
#ifndef SEEK_SET
#define SEEK_SET 0 /* this should be defined in stdio.h */
#endif
#ifndef SEEK_END
#define SEEK_END 2 /* so should this */
#endif

#define namesize 40 /* max chrs per filename  */
#define buffsize 250 /* max chrs in input line buffer */
#define bigbuffsize 2000
#define MAX_VAR_STR 400
#define nal_page_size 64 /* nal disk blocks allocated in multiples of this */
/* Termination number codes */
#define EXIT_OK ((int)0) /* exit successfully */
#define EXIT_ERRERR ((int)1) /* unknown error */
#define EXIT_INT ((int)2) /* exit on interrupt */
#define EXIT_INT_ERR ((int)3) /* Something has gone wrong internally */

#define ERR_BASE ((int)20) /* first nonterminating error code */
#define ERR_OK ((int)0) /* must have same code as EXIT_OK */

#define ERR_BADFILE (ERR_BASE+1) /* No (accessible) input/output file */
#define ERR_OSPACE (ERR_BASE+2) /* Ran out of memory space */
#define ERR_FILE (ERR_BASE+3) /* Access to file went wrong */
#define ERR_VERSION (ERR_BASE+4) /* Bad nsl1 object module version number */
#define ERR_NET_CHK (ERR_BASE+5) /* net consistancy is suspect */
#define ERR_MON_INF (ERR_BASE+6) /* monitor struct details not found */

#define WRN_BASE (ERR_BASE+10) /* first nonterminating warning code */
#define WRN_NOT_IMP (WRN_BASE+1) /* function not implemented yet */
#define WRN_VAR_PT (WRN_BASE+2) /* cannot save pt'er types in this version*/
#define WRN_VAR_UNI (WRN_BASE+3) /* cannot save union's in this version*/
#define WRN_STR_UNIT (WRN_BASE+4) /* undefined struct node member */
#define WRN_VAR_UNIT (WRN_BASE+5) /* undefined var node member */
#define WRN_USR_NAM (WRN_BASE+6)
#define WRN_USR_FUN (WRN_BASE+7)
#define WRN_DS_STUB (WRN_BASE+8)

#define WRN_MAX (WRN_BASE+10)

#define STAR_MASK 0x60 /* mask for pts, used on data_struc members */
#define STAR_TYPE 0x1f /* mask for base type */
#define STAR_ONE 0x20  /* 2-bit val holds number of pt indirections */
#define STAR_TWO 0x40  /* --"-- */
#define STAR_SHFT 5
#define STAR_CAST 0x80 /* set when val is a cast code rather than type code */
#define STAR_MODE STAR_ONE /* vert/horiz mode for list casts */
#define STAR_PBIT STAR_TWO /* type bit set when list is in pair format */
#define STAR_PAIR (STAR_CAST+TYPE_INT) /* std pair fmt lst code */
#define STAR_ISPT (STAR_CAST+TYPE_PT) /*cast for when func rets a pt not list*/
#define STAR_VARG 0x30 /* code for a func with variable args */

/* def of NSLNSLnet flags */
#define NET_CDECL 1 /* net was declared and not loaded */
#define NET_CXMAP 2 /* reconstruct cx map on loading */
#define NET_ACTIV 4 /* net is active */
#define NET_FREE  8 /* net has been freed */
#define NET_STUB 16 /* net is stub only, no DS etc */

/* when new_sip is called indicates type of stub child to parent */
#define DS_SUB ((int)0)
#define DS_ARY ((int)1)
#define DS_TAIL ((int)2)

#define MAX_OFS 20 /* num of slots in nsl_sea for stub offsets */

typedef unsigned char byte; /* Hopefully an 8-bit value */
typedef char siby; /* signed byte value */
typedef unsigned short int word; /* Hopefully 16-bit value */
typedef long foot; /* file position pointer values */
typedef double reals; /* the type of reals */
typedef word door; /* size of a nal block in multiples of nal_page_size */

/* module declaration info */
typedef struct _decl_elm
{ siby decltype;
  mile declnum,size,nodes;
  yard nodenum;
  foot sst_pos;
  char *name,*tag;
  struct _decl_elm *tail;
} decl_elm;

typedef struct _net_elm
{ int refs;
  boolean root_flag:1;
  mile ners,subofs,dec_num;
  foot sst_pos;
  NSLNSLstruct *sindx;
  char *tag,*name;
  decl_elm *decls;
  struct _net_elm *tail;
} net_elm;

typedef struct _path_elm
{ struct _path_elm *parent,*tail;
  net_elm *net;
  decl_elm *decl_pt;
  char *pat,*mempt;
  mile size,indx;
  int uasize; /*from nsl3 */
  boolean isua;
} path_elm;

/* node struct adrs & quick code for a fly's ref path. It's a list of these */
typedef struct _di_elm
{ mile decl; /* decl index in net to branch at */
  mile indx; /* index into decl to branch at */
} di_elm;

typedef struct _node_elm /* node address value */
{ yard base;
  int di_num;/*DEBUG was 16bit word, padding-->sizeof(node_elm)=8 */
  /* note we malloc beyond this by di_num * sizeof(di_elm)'s */
} node_elm;

typedef struct _aux_elm
{ word hd;  /* cx handle */
  foot pos; /* pos of real cx ds adrs on disk */
} aux_elm;
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

/* Note nial assumes num is a long, nat is an int etc, when determining
*  DS storage size... */
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
{ byte ofs_free;
  size_t ds_size;
  yard nodenum;
  foot sst_pos;
  word ofs_list[MAX_OFS+2];
  char *ds_base;
  char *ds_end;
} ds_sea_elm;

#define syssize (sizeof(foot)+sizeof(door)+sizeof(yard))
#define ds_page_size (40*(sizeof(stub_elm)+2*sizeof(long)))

#define SIZ_SIP (sizeof(stub_elm)+sizeof(word))
#define SIZ_SIP_STR (SIZ_SIP+sizeof(word)+1)
#define SIZ_SIP_INT (SIZ_SIP+sizeof(int)+1)
#define SIZ_SIP_LONG (SIZ_SIP+sizeof(long)+1)
#define SIZ_SIP_FLOAT (SIZ_SIP+sizeof(float)+1)
#define SIZ_SIP_DOUB (SIZ_SIP+sizeof(double)+1)
#define SIZ_SIP_PT (SIZ_SIP+sizeof(long *)+1)
#define SIZ_SIP_ION (SIZ_SIP+sizeof(long *))
#define SIZ_SIP_NODE (SIZ_SIP+sizeof(node_elm))

#define isident(c) ((boolean)(isalnum(c) || c=='_'))

#define node_size(npt) (sizeof(node_elm)+((npt)->di_num)*sizeof(di_elm))
#define node_di(npt) ((di_elm *)(npt+1))

#define fmoveto(fpt,pos) fseek(fpt,pos,SEEK_SET)
#define fposeof(fpt) fseek(fpt,(foot)0,SEEK_END)
#define fput_data(fpt,adrs,len) fwrite(adrs,len,(size_t)1,fpt)
#define fget_data(fpt,adrs,len) fread(adrs,len,(size_t)1,fpt)

#if 1
#define malmem(tt,nn) ((tt *)nsl_lst_more(NULL,(nn)*sizeof(tt),TYPE_CHAR,sizeof(char)))
#define relmem(tt,nn,pp) ((tt *)nsl_lst_more(pp,(nn)*sizeof(tt),TYPE_CHAR,sizeof(char)))
#define chk_free(p) if(p)lfree(p)
#define nul_free(p) if(p){lfree(p);p=NULL;}
#else
#include "memtrack.h"
#define malmem(tt,nn) ((tt *)malloc((size_t)(nn)*sizeof(tt)))
#define relmem(tt,nn,pp) ((tt *)realloc(pp,(size_t)(nn)))
#define chk_free(p) if(p)free(p)
#define nul_free(p) if(p){free(p);p=NULL;}
#endif

#define ds_value(spt) ((sip_val *)(((stub_elm *)(spt))+1))
#define ds_prior(p) ((stub_elm *)(((char *)(p))- *(((word *)(p))-1)))
#define ds_next(p) ((stub_elm *)(((char *)(p))+stub_size(p)))
#define ds_subpt(p) ((((stub_elm *)(p))->hassubpt)?ds_next(p):NULL)

/*------------------------------------------------------*/
extern FILE *nsl_nalfp,*nsl_sstfp,*nsl_confp,*nsl_dsifp,*nsl_logfp;
extern stub_elm *nsl_stub;
extern net_elm *nsl_net_top;
extern aux_elm *nsl_ax_list;
extern foot *nsl_cx_list,*nsl_ndfn_sst_list;
extern ds_sea_elm nsl_sea;
extern int nsl_num_conxs,nsl_cx_num,nsl_ax_num,nsl_cx_max,nsl_ax_max,nsl_ndfn_num;
extern char **nsl_ndfn_list,*nsl_sst_name,*nsl_aux_mp;

extern NSLNSLnet **nsl_net_list;
extern NSLNSLnet **my_net_p;
/*---------------------------------------------------------------------*/
extern int nsl_stop,nsl_abort,nsl_err;
extern void signal_error(int error_no,...),set_stub_size(),
  release_net(),nsl_free(NSLNSLnet *),
  close_put_files(),close_get_files(),nial_put_ds(),push_stub(),
  flush_nets(),enlarge_array(),get_funcs(),nial_free_ds(),ds_flush();
extern boolean sst_is_bad();
extern mile ua_size();
extern int nial_get_ds(),open_put_files(),open_get_files(),claim_net(),lsize(),
  find_name(),find_var(),nsl_put(NSLNSLnet *,char *),
  bld_cx_map(),sqchrtobin(),dqstr_to_mem();
extern long nsl_node_num(),nsl_num_adrs();
extern size_t stub_size();
extern char *strsave(char *),*ds_meminc(),*fcat(char *a,char *b),
	*find_symadr(),*indx_adrs(),
  *memsave(),*memlist(),**new_slot();
extern stub_elm *pop_stub(),*sip_inc(),*sip_decl(),*new_stub(),*sip_ary(),
 *ds_arypt(),*ds_parent(),*ds_put_str(),*ds_tail(),*nial_new_ds();
extern NSLNSLnet *nsl_get(char *),*nsl_get_p(char *);
extern NSLNSLstruct *find_tag();

extern void nsl_lst_put();
extern double nsl_lst_val(void **,mile);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /*_nslms_h*/
