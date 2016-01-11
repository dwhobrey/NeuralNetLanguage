/* Name: nsltwo2.h   Type: header file   Date: 30 March 89
*   NeSeL Network Specification Language Preprocessor
*/

/* Type codes used when resolving typedef, Opath type checking etc */
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
#define SIP_CONX 20 /* tmp stub value when creating a new conx */

#define MILE_TYPE TYPE_SHORT 

/* list access macros & functions */
#define LST_SYS 5 /* number of reserved elements in a list */
#define LST_MEM (LST_SYS*sizeof(mile)) /* mem overheads for storing lst */
#define LST_MORE 8 /* space allocation page size in elms for lmore() */
#define LST_CODE 0x5679 /* sys code for checking list is valid */
#define LST_TYPE 0x1f /* mask for list type */
#define LST_PBIT 0x40 /* type bit set when list is in pair format */
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

#define STAR_MASK 0x60 /* mask for pts, used on data_struc members */
#define STAR_TYPE 0x1f /* mask for base type */
#define STAR_ONE 0x20  /* 2-bit val holds number of pt indirections */
#define STAR_TWO 0x40  /* --"-- */
#define STAR_SHFT 5
#define STAR_CAST 0x80 /* set when val is a cast code rather than type code */
#define STAR_MODE STAR_ONE /* vert/horiz mode for list casts */
#define STAR_LIST (STAR_CAST+LST_PBIT+STAR_TYPE) /* mask out STAR_MODE */
#define STAR_PAIR (STAR_CAST+LST_PBIT+MILE_TYPE) /* std pair fmt lst code */
#define STAR_ISPT (STAR_CAST+TYPE_PT) /*cast for when func rets a pt not list*/
#define STAR_VARG 0x30 /* code for a func with variable args */

/* opath combination bit codes */
#define COMB_NULL 0
#define COMB_PLMI 1 /* 0:(+)plus or 1:(-)minus  */
#define COMB_OMNI 2 /* 1: special omni form of minus & plus (--) (++) */
#define COMB_COPY 4 /* 1: cause heap/pile to copy conx */
#define COMB_MERGE 8 /* 1: merge conxs */
#define COMB_FORK 16 /* 1: fork conxs */
#define COMB_DIVERT 32 /* 1: redirect conx */
#define COMB_MASK 64 /* always set if combop is valid */
#define COMB_BREAK 256 /* 1: force a group break */

/* Buffer sizes. Note: should put some overflow checks on buffs...*/
#define MAX_VAL_STR 400 /* value evaluation */
#define MAX_PATH_STR 400 /* path string returned by $* selector */
#define MAX_TOKEN_STR 1000 /* token handler */

#define inc_ary 4
#define inc_eval 5

typedef struct _var_elm
{ char *name,*val,*cast;
  struct _var_elm *subpt,*parent,*tail;
  boolean subase:1,axvar:1,ispath:1;
  byte level:8;
  word line_num;
} var_elm;

typedef struct _out_elm
{ token name;
  var_elm *invar;
} out_elm;

typedef struct _out_rec
{ token outok; /* token value of output string */
  short combop;  /* combination operator code */
  var_elm *invar;
} out_rec;

typedef struct _inp_elm
{ word line_num;
  char *name; /* inpath str */
  out_rec *out_list; /* list of Opaths to combine with inpath */
  struct _inp_elm *tail;
} inp_elm;

typedef struct _comm_elm
{ char *name;
  struct _comm_elm *tail;
} comm_elm;

/* module declaration info */
typedef struct _decl_elm
{ siby decltype; /* type of decl, net, node, struct etc */
  byte comcode; /* communications code */
  mile declnum, /* id number of decl */
    size,nodes;
  yard nodenum;
  foot sst_pos; /* file pos */
  char *name,
    *tag; /* struct type tag */
  var_elm *var; /* node arg initialisers */
  struct _decl_elm *tail;
} decl_elm;

typedef struct _lab_elm
{ byte comcode;
  char *name,*val,*ary;
  decl_elm *decl_pt;
  struct _lab_elm *tail;
} lab_elm;

typedef struct _net_elm
{ int refs;
  boolean root_flag:1;
  int line_num,sor_num,line_sec,sor_sec;
  mile ners,subofs,dec_num;
  foot sst_pos;
  char *tag,*name;
  decl_elm *decls;
  lab_elm *lab;
  inp_elm *inp;
  out_elm *out;
  var_elm *var;
  comm_elm *comm;
  struct _net_elm *tail;
} net_elm;

/* floated Ins are stored in the fly list */
typedef struct _fly_elm
{ boolean tilde:1; /* indicates if Inpath was floated via '~~'or'~' */
  int num; /* inpaths indx in net def */
  di_elm *dipt; /* reference of inpath to descend to & eval */
  struct _fly_elm *tail;
} fly_elm;

/* heap & pile is a list of these elms: each holds a conxs details */
typedef struct _pile_elm
{ int type;
  mile size,indx;
  node_elm *node;
} pile_elm;

/* fly stack doubles for floated paths & local conx pile */
typedef struct _fly_stk /* a fly_list is maintained for each path level */
{ fly_elm *fly_top,*fly_bot; /* list of floated Ins to eval */
  struct _fly_stk *parent,*tail;
  pile_elm *pile_list;
} fly_stk;

/* Note NSL1 assumes user func defs are as below & writes them to nslusrfn.c */
/* DO NOT change these next three typedefs !!! */
typedef struct _NSLNSLfelm
{ short swt; /* switch number to use when calling func_evaluate() */
  char (*fn_pt)(), /* pt to user func */
   *ats; /* func return & arg type codes */
} NSLNSLfelm;
typedef union _NSLNSLftyp
{char cc,*pp;short ss;int ii;long ll;float ff;double dd;
 unsigned char ucc;unsigned short uss;unsigned int uii;unsigned long ull;
}NSLNSLftyp;
typedef struct _NSLNSLfval
{mile typ; NSLNSLftyp val;
}NSLNSLfval;
typedef union _NSLNSLfuni
{ void (*vv)(); char (*cc)(),*(*pp)();
  short (*ss)(); int (*ii)(); long (*ll)();
  float (*ff)(); double (*dd)();
  unsigned char (*ucc)(); unsigned short (*uss)();
  unsigned int (*uii)(); unsigned long (*ull)();
} NSLNSLfuni;

typedef struct _eval_elm
{ int state, /* the type of list being dealt with, e.g. loop list, const */
    str_ofs, /* offset into str being eval'd */
    id, /* offset into str of name of for loop var, if any */
    bsary_num; /* the base ary_num at which elms where added, for - -- */
  NSLNSLfval val; /* for loop count (remaining) & var values */
  boolean discan:1, /* set when rescanning is disabled */
    priscan:1; /* prior rescan state */
} eval_elm;

typedef struct _ary_elm
{ char *ary; /* array str "[...]"*/
  mile *ary_list; /* cur ary vals */
  eval_elm *eval_list; /* evaluation stack for ary */
  boolean isdary:1, /* indicates if ary is a dary rather than a pary */
    suspend:1; /* if set eval is suspended until selector is changed */
  mile atval, /* cur ary pos */
    hasval,/* cur ary elm */
    pipval,/* # elms in ary */
    fixval;/* holds ary size limit (if any) during evaluation */
} ary_elm;

typedef struct _sak_elm /* this is used soley in do_an_ary */
{ ary_elm ary;
  struct _sak_elm *tail;
} sak_elm;

typedef struct _lary_elm
{ struct _path_elm *tmplcpt;
  sak_elm *stail,*stktop;
  lab_elm *new_lab,*bs_pt,*lab_pt;
  ary_elm sdary,spary;
  byte dec_id:8;
} lary_elm;

typedef struct _path_elm
{ struct _path_elm *parent,*tail,*aryparent,*arychild,*wldpth;
  net_elm *net;
  decl_elm *decl_pt;  /* Note decl_pt->name == current match name */
  lab_elm *lab_pt;  /* & lab_pt->name == current label if any */
  char *pat;          /* (wild)pattern to match on */
  ary_elm pary,dary;  /* Info on path member & decl member */
  boolean laryflg:1,  /* indicates if label was a LARY */
    partflg:1, /* indicates if node preceded by a part symb */
    curwldflg:1, /* set if path is wild due to parental wildness */
    wldflg:1, /* set if path is wild from here on */
    rebflg:1, /* node to be rebuilt using prior decl_pt etc */
    rebwldflg:1; /* tmp store for '**' proc flg during rebuild & escape */
  byte pat_id:8, /* all nodes belonging to same label have same lab_id */
    dec_id:8, /* lab_id's are unique within a path */
    reb_id:8, /* tmp store for install_lab id during rebuild & escape */
    tac_id:8; /* another tmp store for rebuild */
  yard net_base;
  mile larypos,laryat,decl;
  lary_elm *lary;
} path_elm;

typedef struct _flag_elm
{ boolean /* flags indicate when: */
    inp:1, /* dealing with an Inpath at present */
    build:1, /* when the ds should be built during descent */
    inout:1, /* if in in/out path mode */
    pamat:1, /* in partition match mode */
    last:1, /* counting leaves in last part */
    fork:1, /* evaling fork path */
    info:1, /* gathering path information */
    ary:1, /* forces escape mode when conx_to_node is hit */
    fly:1, /* in fly eval mode: evaling floated list of Ins */
    wild:1, /* non fatal errors are not reported when this is set */
    escape:1, /* ascend path in escape mode: don't do any more elms */
    state:1; /* dealing with var initialising, otherwise Inpth evaling*/
} flag_elm;

/* info on a list var */
typedef struct _tab_elm
{ struct _tab_elm *tail;
  var_elm *vpt;
  mile holbits;
  ary_elm pippa;
} tab_elm;

/* info on current Var decl being eval'd */
typedef struct _vee_elm
{ var_elm *curpt; /* cur pos in var decl */
  tab_elm *tabpt; /* table of any list vars being evald */
  byte lab_id:8, /* var path label id for partition counting */
    usedlvl:8; /* lvl at which var was used by do_var, 0 if not*/
} vee_elm;

/* info on current Inpath--Opath decl being eval'd */
typedef struct _ilk_elm
{ out_rec *baspt,*curpt; /* base oelm & cur oelm being evaled */
  vee_elm veed;
  path_elm *opt, /* outpath associated with cur oelm, clr'd upto uneval'd */
    *orefpt; /* ref path used for output str evaluation */
  node_elm *node; /* cur output conx, address of base element: struct index */
  decl_elm *aux; /* set to opath aux decl if there is one */
  int *part_list, /* list of num of leafs in a partition in oelm */
    part_num, /* num of parts in list */
    type; /* type-code of output conx */
  mile cxnum, /* cur conx number offest into oelm */
    abnum, /* absolute conx number for $??, (-1) if EOC */
    size, /* size of output conx decl */
    indx; /* index of output conx relative to base:limb */
  boolean endofcx:1, /* set when reach end of oelm */
    self:1, /* set when Op references cur Ip via ~ or ~~ */
    glob:1, /* set when a global input from node */
    startaux:1, /* set when an Op's aux is first seen */
    discan:1; /* set when rescanning is disabled */
} ilk_elm;

/* when resolving aux defs, this holds prior net resolve state */
typedef struct _state_elm
{ flag_elm flags;
  int dslot;
  path_elm *path_pt,*rootpt,*inpt,*outpt,*til_pt,*org_pt,*flo_pt;
  ds_sea_elm *ds_pool;
  ilk_elm *ilk_pt;
  fly_stk *fly_top,*fly_bot;
  fly_elm *fly_pt;
  di_elm *fly_di;
} state_elm;

/* a stack of aux & par states is maintained for sub(sub)auxs etc */
typedef struct _aux_elm
{ struct _aux_elm *parent,*tail;
  yard aux_base;
  state_elm par_state;
  inp_elm inp; /* Ip info for aux resolving */
  ilk_elm ilk; /* Op info */
} aux_elm;

/*----------------------------------------------------------------------*/
/* globals vars */
boolean
  opt_logerr, /* log errs to log file */
  err_filter, /* filter similar errors & do not report */
  bookeep, /* set if book keeping is on, this severely slows things down */
  mod_ison, /* set if flasher is on */
  is_new_net, /* set if calling do_module() with a new net */
  vax_flg; /* set when sel in aux var should be applied to parent state */

int
  exp_align, /* exp stack alignment value */
  raise_error, /* global error indicator, an error has occured*/
  num_errors,
  err_line_num, /* cur file line num for err reporting */
  err_file_num, /* cur file num */
  warn_limit; /* warning message level */

yard net_node_max; /* cur max node number, first free node is +1 */

token TOK_AUXOUT; /* refs token value of STR_AUXOUT */

/* version string, format is crtical! */
extern char *version,version_num,version_upd;

aux_elm *aux_top,*aux_bot; /* net resolving stack */

vee_elm *vee_pt; /* Var decl info */

extern decl_elm *fake_pt; /*a dummy elm for labelled additional declarations*/
pile_elm *heap_list; /* global stack of conxs */
extern word ds_sea_max;
extern mile str_cir_max;
/*-----------------------------------------------------------------------*/
/* the vars below describe present state, aux recursion pushes all these */
path_elm *path_pt, /* working path pt */
  *rootpt, /* root of reference path */
  *inpt,*outpt, /* root pts of cur I/O paths for error reporting */
  *til_pt, /* cur ref path tail: i.e tilde pt */
  *org_pt, /* pt to path node that cur inpath was decl in */
  *flo_pt; /* floated path node of cur inpath */

ds_sea_elm *ds_pool; /* cur node data-structure pool */
stub_elm *ds_stub; /* cur node data-structure pt */

ilk_elm *ilk_pt; /* cur inp info */

fly_stk *fly_top,*fly_bot; /* fly stack pts */
fly_elm *fly_pt; /* cur fly being eval */
di_elm *fly_di; /* cur fly path indx */

flag_elm flags; /* global flags */
