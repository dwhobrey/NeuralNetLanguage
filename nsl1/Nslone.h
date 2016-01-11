/* Name: nslone.h   Type: header file   Date: 30 March 89
*  NeSeL Network Specification Language
*/
#define MAX_EXPANDS 250 /* mno macro expansions per macro */
#define MAX_MAC_ARGS 20 /* mno macro arguments */
#define MAX_MACREC 40 /* mno recursive macro calls */
#define MAX_IO_STACK (20+2*MAX_MACREC) /* mno io pushes */
#define MAX_PARMS 50 /* mno arguments to user functions */
#define MAX_RECURSE 40 /* default number of recursions before abort */
#define MAX_PRAG 20 /* mno pragma pushes */

/* Tokens identified by NeSeL */
#define PRAGMA_TOKEN "pragma" /* for setting options etc from source */
#define MACRO_TOKEN "macro" /* signifies a new macro def */
#define NEURON_TOKEN "node" /* token signifying neuron struct def */
#define NET_TOKEN "net" /* token signifying net def */
#define TYPEDEF_TOKEN "typedef" /* token signifying a typedef declaration */
#define STRUCT_TOKEN "struct" /* signifies a struct tag declaration */
#define UNION_TOKEN "union" /* signifies a union tag declaration */

/* Definition types */
#define DEF_NONE 1 /* No def detected yet */
#define DEF_PRAGMA 2 /* Parsing pragma */
#define DEF_MACRO 3 /* Parsing a macro def */
#define DEF_NET 4 /* Parsing net definition */
#define DEF_NFUNC 5 /* Parsing neuron function */
#define DEF_NEURON 6 /* Parsing neuron defintion */
#define DEF_TYPEDEF 7 /* Parsing typedef */
#define DEF_STRUCT 8 /* Parsing struct */
#define DEF_UNION 9 /* Parsing union */

/* Type codes used when resolving typedef etc */
#define TYPE_UNKNOWN 0
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

#define STAR_ONE 0x10
#define STAR_TWO 0x20
#define STAR_MASK 0x30
#define STAR_RES 0x4f
#define STAR_TYPE 0x0f
#define STAR_SHFT 4
#define STAR_VARG 0x30

/* Opath Combination bit codes */
#define COMB_NULL 0
#define COMB_PLMI 1 /* 0:(+)plus or 1:(-)minus  */
#define COMB_OMNI 2 /* 1: special omni form of minus & plus (--) (++) */
#define COMB_COPY 4 /* 1: cause heap/pile to copy conx */
#define COMB_MERGE 8 /* 1: merge conxs */
#define COMB_FORK 16 /* 1: fork conxs */
#define COMB_DIVERT 32 /* 1: redirect conx */
#define COMB_MASK 64 /* always set if combop is valid */

/* communications bits */
#define COMM_NONE '@'
#define COMM_IN 'A'
#define COMM_OUT 'B'
#define COMM_INOUT 'C'

#define inc_outs 20	/* array expansion size increase, in ary elms */
#define inc_ins 20
#define inc_vars 20
#define inc_comms 10
#define inc_strs 5
#define inc_labs 20
#define inc_conxs 20
#define inc_mod 40
#define inc_structs 20
#define inc_neuron 20
#define inc_tdefs 10
#define inc_sor 4
#define inc_sst 10
#define inc_glo 10
#define inc_mac 5
#define inc_id 5
#define inc_func 4
#define inc_undef 5
#define inc_muldef 5

typedef struct _stack_elm
{ BOOL isroot; /* set if node def type & used as a root struct */
  int def_type; /* type of definition found (typedef, neuron etc) */
  int file_num; /* indx num of source filename */
  int line_num; /* line num in source file of definition */
  int num_lines; /* number of lines in def */
  int abs_line; /* absolute line num in source file */
  long file_pos; /* magic file pos, for positioning */
  long sst_pos; /* magic file pos in substantiated file */
  long nodes;
  struct _stack_elm *tail; /* point to rest of stack */
  char name; /* first char of string of name being defined */
 /* note we stick rest of name after this char using malloc */
} stack_elm;

/* holds In:Out decls found while parsing net modules */
typedef struct _inouts_elm
{ int num;
  int line_num;
  long *list;
  char pat;
} inouts_elm;

/* holds Out decls found while parsing net modules */
typedef struct _outs_elm
{ int num;
  char pat;
} outs_elm;

typedef struct _cond_elm
{  char label[max_ident];
   int type_code;
   int ary_size;
   long nodes;
   char tag[max_ident];
   struct _cond_elm *tail;
} cond_elm;

extern cond_elm *cond_top;

extern int outs_align,stk_align,inout_align;
/* Macro to return start of outs_elm structure given its name */
#define outsrec(p) ( (outs_elm *)( ((char *)(p)) - outs_align ) )

/* Macro to return start of stack_elm structure given its name */
#define stkrec(p) ( (stack_elm *)( ((char *)(p)) - stk_align) )

/* Macro to return start of In/Out decl record */
#define inoutrec(p) ( (inouts_elm *)( ((char *)(p)) - inout_align) )

extern BOOL pretty_C, /* make ouput accetable by cpp again */
 line_info, /* indicates if to include line info in files */
 monitor, /* indicates if monitor info to be added to output */
 logerrs,
 func_info, /* produce nslusrfn.c file */
 mx_info, /* extract node funcs, suppresses user network() */
 core_def, /* add core defs to *.nsc */
 hd_info, /* extract C header info from net file */
 fsym_info, /* produce nslmonfn.c file */
 add_info, /* indicates if extra info to be added to structs */
 var_info, /* extract variable info */
 time_info, /* indicates if timing info to be added */
 node_info, /* indicates if node data to be added */
 group_info, /* indicates if group data(node_info is added as well) */
 conx_info; /* indicates if ins, outs & size data to be added */

extern int cur_file_num, /* current file name indx */
 cur_def_type, /* current definition type */
 err_file_num; /* last error free file name indx */

extern int cur_line_num, /* current line number in file */
 abs_line_num,
 err_line_num, /* last error free line number in file */
 decl_line,
 hline; /* indicates if a hash_line has occurred, holds last lnum */

extern int raise_error, /* Global error indicator, an error has occured*/
 num_errors,
 tdefs_num, /* number of typdef decls parsed */
 source_num, /* number of source files */
 module_num, /* number of unique modules */
 neuron_num,
 labs_num, /* number of labels in module */
 aux_num,
 outs_num, /* number of Output decls in module */
 ins_num,/* number of Input decls in module */
 vars_num,/* number of Var decls in module */
 comms_num, strs_num,
 undef_num, /* number of undefined modules encountered */
 muldef_num, /* number of multiply defined labels found in module */
 func_num, glo_num, mac_num, id_num;
 max_tdefs, /* current max num of typedef decls allowed */
 max_module, /* current modules etc */
 max_vars, max_comms, max_strs, max_ins,
 max_outs, max_labs, max_aux, max_sor, max_glo,
 max_neuron, max_func, max_undef, max_muldef, max_mac, max_id,
 recurse_max; /* max level of recursion before abort */

extern int nss_ok; /* indicates if nss file is ok */
extern unsigned int cur_mod_size; /* size of current module in  bytes */
extern long cur_file_pos; /* current magic position in source file */
extern long num_structs; /* this is used as a tag generator in firstpass */
/* and struct type counter in secondpass */

extern char *version,version_num,version_upd; /* version details */

extern char cur_ident[max_ident], /* current ident str */
 decl_name[max_ident], decl_id[max_ident],
 *decl_label,*decl_dollar, *decl_path,
 **undef_list, /* undefined modules, labels */
 **muldef_list, /* multiply defined labels */
 **func_list, /* == "funcname=typecode" */
 **source_list, /* source file names */
 *base_list[], /* == "filename" */
 **tdefs_list, /* typedef decls == "typename\0typedecl\0typespec" */
 **module_list, /* index into stack of module names etc == "module name" */
 **macro_list, /* macro's */
 **id_list, /* user vars */
 **neuron_list, /* index into stack of neuron struct names == "struct name" */
 **labs_list, /* module labels */
 **aux_list,
 **glo_list,
 **outs_list, /* Output decls */
 **ins_list, /* Input decls */
 **vars_list, /*  Var decls */
 **comms_list, /* import/outport communincations decls */
 **strs_list; /* struct initialisers */

extern stack_elm *stack_top;
