/* Name: nslmsmon.h   Type: header file   Date: 18 May 89
*   NeSeL Network Specification Language
*/
#ifndef _nslmsmon_h
#define _nslmsmon_h

#ifdef __cplusplus
extern "C"
{
#endif

/* Buffer sizes */
#define MAX_VAL_STR 400 /* value evaluation */
#define MAX_IO_CMD 200 /* size of cmd buffer */
#define TMP_OFS 4 /* offset into tmp_buff of first str byte */

typedef char **farg_elm;

typedef struct _ni_elm
{ int net,indx;
} ni_elm;
typedef struct _cmd_elm
{ char *name;
  int token;
  BOOL addsemi;
} cmd_elm;
/* a list of files accessed is maintained */
typedef struct _fly_elm
{ char *name; /* name of file */
  int users; /* cur number of file users */
  FILE *fp;
} fly_elm;

/* stack of file positions & io state during i/o pushes */
typedef struct _fsk_elm
{ BOOL cmd_exe,cmd_brk,cmd_pat,cmd_yacc;
  char io_last_chr,*cmd_const;
  int
	cmd_exit,
    fnum, /* fly number */
    lofs; /* offset into line */
  int entry; /* yacc_entry */
  long pos; /* file pos of line */
} fsk_elm;

/* a list of user funcs is maintained */
typedef struct _fun_elm
{ char *name, /* name of user func */
    *ats, /* return type & parameter type code string */
    **parms; /* list of parameter ids */
  int p_num; /* number of parameters */
  fsk_elm iop; /* details of where func is stored (starting from left brace)*/
} fun_elm;

/* during eval of cmds & user funcs occurring in an exp,
*  a stack of arg numbers--positions is maintained */
typedef struct _exp_elm
{ char *par_stk,*par_str;
  int *par_ofs;
} exp_elm;

/* a list of vars declared in user files is maintained */
typedef struct _glo_elm
{ char *name; /* name of var */
  NSLNSLfval val; /* type & value */
} glo_elm;

/* stack of where we are in user routines */
typedef struct _uff_elm
{ int fnum;
  char *arg_stk; /* pt to mem holding user func arg values */
  glo_elm *fup_list; /* func parm list, pts into arg_stk */
  glo_elm *loc_list; /* local var list */
} uff_elm;

typedef struct _user_elm
{ char *name;
  char (*fpt)();
} user_elm;

#define set_io_pos(str) io_buff_pt=str
#define get_io_pos() io_buff_pt
#define get_io_ofs() (io_buff_pt-io_buff)

extern NSLNSLfval
  tmp_fval,
  parm_val, /* initialiser val for var */
  zerval, /* a zero int val */
  trueval, /* a true int val */
  dumval, /* a dummy val signifies no expression (type=TYPE_UNKNOWN) */
  nsl_func_rr; /* result of cur user func */

extern BOOL 
  cmd_pat,cmd_exe,cmd_brk,cmd_yacc,
  parm_pat,parm_exe,parm_brk,parm_yacc;

extern int
  cmd_exit,
  parm_exit,
  io_fnum,
  parm_iof;

extern int
  cmd_exp,
  yacc_entry,
  tmp_cmd, /* tmp store for a cmd code */
  parm_entry,
  parm_type, /* cur parm type code */
  parm_ofs, /* cur parm i/o offset */
  parm_num,parm_max; /* parameters to user func */

extern long
  io_file_pos, /* pos of cur line in file */
  io_tmp_pos, /* pos of stdin in tmp file */
  io_tmp_end, /* cur eof pos for tmpfp */
  parm_pos; /* pos of func in file */

extern double tmp_val; /* tmp store for a loop/if exp */

extern char
  tmp_chr,
  tmp_buff[],
  io_buff[],
  parm_args[], /* cur func type codes */
  *tmp_pt, /* pt into tmp_buff */
  *io_buff_pt, /* cur io pos */
  *cmd_id,
  *cmd_const,
  *cmd_entry,
  *par_stk,*par_str, /* cur user func arg list */
  *parm_name, /* cur uff name */
  *parm_const,
  **parm_list, /* cur user func parm list during parsing */
  **was_list; /* watch & stop list */

extern int 
  *par_ofs, /* offset of arg in arg list */
  *state_stk; /* cmd state stack */

extern exp_elm *exp_stk; /* for pushing func args */
extern uff_elm *uff_stk; /* user file func stack */
extern farg_elm *farg_stk; /* file arg stack */
extern fsk_elm *fsk_stk; /* i/o stack */
extern glo_elm *glo_list; /* global vars from user files */
extern fun_elm *fun_list; /* list of user file funcs */
extern NSLNSLnet *tmp_net,*mon_net;

extern FILE 
    *infp, /* input file */
    *tmpfp, /* echo of stdin for io pushing & popping */
    *cmdfp, /* user settable stdin echo */
    *logfp; /* log file */

extern void cir_free(),yyerror(),del_io(),skip_statement(),skip_to_eol(),
  save_exp(),restore_exp(),push_exp(),user_func(),get_args(),
  inc_file(),dec_file(),mputs(),do_dir(),do_cd(),do_cnet(),do_shell(),
  pop_io(),pop_farg(),pop_uff(),do_load(),do_save(),do_driver(),
  do_gen_func(),do_show(),do_delete(),do_enadis(),do_was(int num, char was),do_debug(),debug(),
  do_help(),do_log(),do_id_func(),do_step(),init_mon(),do_dump(),
  do_fspage(),do_fsopen(),do_fssearch(),do_fsclose(),do_fsdsp(),
  push_uff(),push_io(),push_farg(),push_state(),go_io(),set_io();
extern BOOL is_set(),fs_get(),do_fsgo();
extern char *cir_save(),skip_to_chr(),**new_slot();
extern int yylex(),yyparse(),mprintf();
extern int pop_state(),last_state(),skip_to_loop(),pop_to_loop(),add_file();
extern double tx_to_d();
extern NSLNSLfval d_to_tx(),d_to_t(),var_selector();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*_nslmsmon_h*/
