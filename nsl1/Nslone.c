/* Name: nslone.c   Type: main C file    Date: 30 March 89
*  NeSeL Network Specification Language Preprocessor
*/
#include "globals.h"
#include "iolo.h"
#include "nslone.h"
#include "general.h"
#include "nslyacc.h"
#include <signal.h>

/* Note format is critical */
extern char *version="NeSeL(a)v%c.%c(DJRW)1989\n",version_num='2',version_upd='1';

extern BOOL pretty_C=FALSE, /* make ouput accetable by cpp again */
 line_info=FALSE, /* indicates if to include line info in files */
 monitor=FALSE, /* indicates if monitor info to be added to output */
 logerrs=FALSE,
 func_info=FALSE, /* produce nslusrfn.c file */
 mx_info=FALSE, /* extract node funcs, suppresses user network() */
 core_def=FALSE, /* add core defs to *.nsc */
 hd_info=FALSE, /* extract C header info from net file */
 fsym_info=FALSE, /* produce nslmonfn.c file */
 add_info=FALSE, /* indicates if extra info to be added to structs */
 var_info=FALSE, /* extract variable info */
 time_info=FALSE, /* indicates if timing info to be added */
 node_info=FALSE, /* indicates if node data to be added */
 group_info=FALSE, /* indicates if group data(node_info is added as well) */
 conx_info=FALSE; /* indicates if ins, outs & size data to be added */

extern int cur_file_num=0, /* current file name indx */
 cur_def_type=0, /* current definition type */
 err_file_num=0; /* last error free file name indx */

extern int cur_line_num=0, /* current line number in file */
 abs_line_num=0,
 err_line_num=0, /* last error free line number in file */
 decl_line=0,
 hline=0; /* indicates if a hash_line has occurred, holds last lnum */

extern int raise_error=0, /* Global error indicator, an error has occured*/
 num_errors=0,
 tdefs_num=0, /* number of typdef decls parsed */
 source_num=0, /* number of source files */
 module_num=0, /* number of unique modules */
 neuron_num=0,
 labs_num=0, /* number of labels in module */
 aux_num=0,
 outs_num=0, /* number of Output decls in module */
 ins_num=0,/* number of Input decls in module */
 vars_num=0,/* number of Var decls in module */
 comms_num=0, strs_num=0,
 undef_num=0, /* number of undefined modules encountered */
 muldef_num=0, /* number of multiply defined labels found in module */
 func_num=0, glo_num=0, mac_num=0, id_num=0;
 max_tdefs=0, /* current max num of typedef decls allowed */
 max_module=0, /* current modules etc */
 max_vars=0, max_comms=0, max_strs=0, max_ins=0,
 max_outs=0, max_labs=0, max_aux=0, max_sor=0, max_glo=0,
 max_neuron=0, max_func=0, max_undef=0, max_muldef=0, max_mac=0, max_id=0,
 recurse_max=0; /* max level of recursion before abort */

extern unsigned int cur_mod_size=0; /* size of current module in  bytes */
extern long cur_file_pos=0; /* current magic position in source file */
extern long num_structs=0; /* this is used as a tag generator in firstpass */
/* and struct type counter in secondpass */

extern char cur_ident[max_ident]={0}, /* current ident str */
 decl_name[max_ident]={0}, decl_id[max_ident]={0},
 *decl_label=NULL,*decl_dollar=NULL, *decl_path=NULL,
 **undef_list=NULL, /* undefined modules, labels */
 **muldef_list=NULL, /* multiply defined labels */
 **func_list=NULL, /* == "funcname=typecode" */
 **source_list=NULL, /* source file names */
 **tdefs_list=NULL, /* typedef decls == "typename\0typedecl\0typespec" */
 **module_list=NULL, /* index into stack of module names etc == "module name" */
 **macro_list=NULL, /* macro's */
 **id_list=NULL, /* user vars */
 **neuron_list=NULL, /* index into stack of neuron struct names == "struct name" */
 **labs_list=NULL, /* module labels */
 **aux_list=NULL,
 **glo_list=NULL,
 **outs_list=NULL, /* Output decls */
 **ins_list=NULL, /* Input decls */
 **vars_list=NULL, /*  Var decls */
 **comms_list=NULL, /* import/outport communincations decls */
 **strs_list=NULL; /* struct initialisers */

extern stack_elm *stack_top=NULL;

extern cond_elm *cond_top=NULL;

extern int outs_align=0,stk_align=0,inout_align=0;

extern void firstpass(),secondpass();

void keyint(int sig)
{ printf("\nCompilation aborted.\n");
  exit(sig);
}

void booter()
{ logfp=stdout; source_num=0;
  max_sor=0; source_list=NULL; recurse_max=MAX_RECURSE;
  signal(SIGINT,keyint); /*signal(SIGTSTP,keyint);*/
}

void initialise()
{ stack_elm selm; inouts_elm ielm; outs_elm oelm;
  stk_align= (int)((char *)&selm.name - (char *)&selm);
  outs_align= (int)((char *)&oelm.pat - (char *)&oelm);
  inout_align= (int)((char *)&ielm.pat - (char *)&ielm);
  initialise_io();
  initialise_yacc();
  openfiles();
}

extern void copy_kludge();

/* The main one */
int main(argc,argv)
  int argc; char *argv[];
{ booter();
  copy_kludge();
  getmainargs(argc,argv);
  initialise();
  firstpass();
  secondpass();
  return(num_errors>0);
}

