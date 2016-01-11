/* Name: nial2.h   Type: header file   Date: 30 March 89
*  NeSeL Network Specification Language Preprocessor
*/

/* when new_sip is called indicates type of stub child to parent */
#define DS_SUB ((int)0)
#define DS_ARY ((int)1)
#define DS_TAIL ((int)2)

/* Char Annotations describing comms properties of node */
#define A_INP '<' /* node takes an input from outside */
#define A_OUT '>' /* node produces an output used by outside */
#define A_PIP '|' /* takes & produces a value from outside */
#define A_ARG '*' /* this node represents an input argument to another node */
#define A_HID '%' /* this node is hidden to nodes outside field, no comms */

#define MAX_OFS 50 /* num of slots in ds_sea_elms for stub offset handles*/
#define MAX_OFST2 (MAX_OFS*2)

/* node struct adrs & quick code for a fly's ref path. It's a list of these */
typedef struct _di_elm
{ mile decl; /* decl index in net to branch at */
  mile indx; /* index into decl to branch at */
} di_elm;

typedef struct _node_elm /* node address value */
{ yard base;
  int di_num; /* DEBUG was 16bit word, padding-->sizeof(node_elm)=8 */
  /* note we malloc beyond this by di_num * sizeof(di_elm)'s */
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

typedef struct _ds_sea_elm /* a LILO queue of ds's is maintained for speed */
{ boolean modified:1;
  byte refs;
  word ofs_free; /* first free ofs_list slot */
  word pool_handle;
  size_t ds_size; /* size of ds memory */
  yard nodenum;
  foot sst_pos;
  word *ofs_list; /* list of rel offsets to stubs in ds, upd if stubs moved */
  char *ds_base; /* base adrs of ds memory */
  char *ds_end; /* end of ds block in use */
} ds_sea_elm;

/* Note SIZ_SIP includes space for a backtracking word after stub */
#define SIZ_SIP (sizeof(stub_elm)+sizeof(word))
#define SIZ_SIP_STR (SIZ_SIP+sizeof(word)+1)
#define SIZ_SIP_INT (SIZ_SIP+sizeof(int)+1)
#define SIZ_SIP_LONG (SIZ_SIP+sizeof(long)+1)
#define SIZ_SIP_FLOAT (SIZ_SIP+sizeof(float)+1)
#define SIZ_SIP_DOUB (SIZ_SIP+sizeof(double)+1)
#define SIZ_SIP_PT (SIZ_SIP+sizeof(long *)+1)
/* NOTE assume sizeof(sip_ion)<=sizeof(long *),sizeof(sip_ion) etc */
#define SIZ_SIP_ION (SIZ_SIP+sizeof(long *))
#define SIZ_SIP_NODE (SIZ_SIP+sizeof(node_elm))

#define node_size(npt) (sizeof(node_elm)+((npt)->di_num)*sizeof(di_elm))
#define node_di(npt) ((di_elm *)(npt+1))

/* set ds modified bit quickly: rpt must be ds root */
#define fast_ds_mod(p) p->modified=T

/* return pt to root stub */
#define dsroot() ((stub_elm *)ds_pool->ds_base)

/* return pt to start of a stub's value */
#define ds_value(spt) ((sip_val *)(((stub_elm *)(spt))+1))

/* return pt to prior stub: p must be start of cur stub */
#define ds_prior(p) ((stub_elm *)(((char *)(p))- *(((word *)(p))-1)))

/* return pt to next stub after this one */
#define ds_next(p) ((stub_elm *)(((char *)(p))+stub_size(p)))

/* return subpt stub to stub at p: it is always the next stub if present */
#define ds_subpt(p) ((((stub_elm *)(p))->hassubpt)?ds_next(p):NULL)

extern void nal_initialise(),set_stub_size(),zap_stub(),rem_sip(),
  ds_release(),ds_flush(),ds_memdec(),update_stub(),adj_uasize(),
  nial_start(),nial_end(),nial_put_ds(),nial_get_ds();
extern size_t stub_size();
extern mile *sip_list(),sip_largest(),sip_lowest();
extern int push_stub();
extern char *ds_meminc();
extern byte *ds_level();
extern boolean ds_space();
extern stub_elm *new_stub(),*sip_pos(),*sip_inc(),*sip_decl(),*sip_arnob(),
  *find_dspt(),*node_to_ds(),*sip_ary(),*pop_stub(),*restore_stub(),
  *ds_put_str(),*ds_rem_str(),*ds_arypt(),*ds_tail(),*ds_parent(),
  *move_to_node();
extern ds_sea_elm *ds_claim();
