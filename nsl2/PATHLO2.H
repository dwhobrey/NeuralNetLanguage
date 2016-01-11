/* Name: pathlo2.h   Type: header file   Date: 30 March 89
*   NeSel Network Specification Language Preprocessor
*/

/* list evaluation return codes (true code = (-1)*LEX) */
#define LEX_OK 0 /* list eval'd ok */
#define LEX_ER 1 /* err during list eval */
#define LEX_SU 2 /* list eval was suspended before any elm added */
#define LEX_AD 3 /* elms added, but list was suspended */
/* additional codes when elm not in list: */
#define LEX_CL 4 /* list was eval'd and ary_num reset to 0 at some point */
#define LEX_NE 5 /* not end of list  */
#define LEX_ZZ 6 /* end of list */
#define LEX_NO 7 /* list eval'd ok to eol but returned null list */
/* selector evaluation return codes */
#define SEX_UF 1 /* sel value was not ready yet, so take (-1) etc */
#define SEX_DS 2 /* sel was in ds ary */
#define SEX_NL 3 /* sel returns null list */
#define SEX_CL 4 /* sel not ready due to suspended list */
/* sel eval codes for when using part_leafs to eval a selector */
#define SOV_LAB (-1) /* part_level refers to a label coordinate */
#define SOV_ABS (-2) /* refers to an absolute coordinate */
#define SOV_ALL (-3) /* eval all the way */
#define SOV_END (-4) /* finished */

/* Var expression evaluation codes */
#define VAR_EVAL 'A' /* evaluate this var exp */
#define VAR_NOEVAL 'B' /* do not eval exp: contains func or adrs op */

/* Selector codes */
#define SEL_FIDENT '@' /* function ident and parm list follows */
#define SEL_VIDENT 'A' /* var ident follows */
#define SEL_FIXPTH 'B' /* fixpath follows */
#define SEL_EXCEXC 'C' /* true if end of oelm conxs */
#define SEL_EXCNUM 'D' /* relative num of cur conx */
#define SEL_EXCAT 'E' /* absolute num of cur conx */
#define SEL_EXC 'F' /* cur size of corresponding part lvl oelm */
#define SEL_AND 'G' /*. fixed size of array */
#define SEL_PIP 'H' /*. cur num elms in list */
#define SEL_HAT 'I' /*. largest value in list */
#define SEL_SUB 'J' /*. lowest value in list */
#define SEL_PERC 'K' /*. cur num input conxs to ds branch */
#define SEL_COL 'L' /*. cur num output conxs from ds branch */
#define SEL_AT 'M' /* cur elm pos */
#define SEL_HASH 'N' /* cur elm val */
#define SEL_XS '^' /* extended selector escape code */
#define SEL_XS_STAR 'A' /* return path as fcm index string */
#define SEL_XS_PLUS 'B' /* return symbolic path string */
#define SEL_XS_MINUS 'C' /* return symbolic type path string */
#define SEL_XS_TYPE 'D' /* following type case used to set elm type */
#define SEL_XS_NODE 'E' /* return path node number */

/* internal selector codes */
#define SEL_LOP_DOT '.' /* internal sel for [.] evaluation */
#define SEL_LOP_EQU '=' /* internal sel for [=] evaluation */
#define SEL_LOP_EXC '!' /* internal sel for [!] evaluation */
/* sel dirty bit for breaking recursive defs */
#define SEL_DIRTY 0x20 /* dirty bit set when sel has been ref'd */

/* Selector index type bit numbers for optional coors [I] in "$cIx" */
#define SIN_DEF ((int)'@') /* default value */
#define SIN_ABS 1 /* set if absolute path required */
#define SIN_CAR 2 /* set if cardinal coors required, relative otherwise */
#define SIN_INP 4 /* set if refering to inpath, -->    */
#define SIN_OUT 8 /* set if refering to outpath, otherwise cur path */
#define SIN_ARY 16 /* set if base decl ary of selected ary is required */
#define SIN_REV 32 /* set if reverse list is req (applies to lops only) */
#define SIN_MASK (SIN_DEF+SIN_ABS+SIN_CAR+SIN_INP+SIN_OUT) /*mask _ARY & _REV*/
#define SIN_LOP (SIN_DEF+SIN_ARY) /* default coors to use in lops */

/* internal yacc codes for loop lists */
#define LOOP_LST 0  /* non loop list */
#define LOOP_FOR ((int)'0')   /* $0((e);(e);(e))n[] */
#define LOOP_WHILE ((int)'1') /* $1(e)n[] */
#define LOOP_DO ((int)'2')    /* $2(e)n[] */
#define LOOP_IF ((int)'3')    /* $3(e)n[] */
#define LOOP_ELSE ((int)'4')  /* $4(e)n[]n[] */
#define LOOP_IDENT ((int)'5')   /* $5cident(e) */
#define LOOP_BRK ((int)'6')   /* $6 */
#define LOOP_CONT ((int)'7')  /* $7 */
#define LOOP_RET  ((int)'8')  /* $8 */
#define LOOP_EXIT ((int)'9')  /* $9 */
#define LOOP_ROF ((int)10)   /* these last 4 must just be diff to others */
#define LOOP_ELIHW ((int)11)
#define LOOP_FI ((int)12)
#define LOOP_TNEDI ((int)13)

/* loop list append codes */
#define APND_BEG '0' /* overwrite new elms to start of cur list */
#define APND_CUR '1' /* add new elms to prior loop list level */
#define APND_END '2' /* add new elms to end of cur list */

/* Type codes for list operators */
#define LIST_WILD 'A'  /* list is wild map operator [*] */
#define LIST_ATCON 'B' /* atom constant */
#define LIST_ATSEL 'C' /* atom containing selectors */
#define LIST_LSCON 'D' /* list of constants */
#define LIST_LSSEL 'E' /* list containing selectors */
#define LIST_LSLOP 'F' /* list containing list operator */
#define LIST_ATPAR 'G' /*H atom, but contained partition op $!x */
#define LIST_LSPAR 'I' /* list, but contained $!x */
#define LIST_ATEXC 'J' /*K atom, but contained $!(!@#) */
#define LIST_LSEXC 'L' /* list, but contained $!(!@#) */
#define LIST_ATONE 'M' /*N atom, but contained $!1 */
#define LIST_LSONE 'O' /*P list, but contained $!1 */
#define LIST_LSQST 'Q' /* list containing list-operator [?] [-?] [+] [-+]*/

/* list opr codes */
#define LOP_STAR 'Z' /* [*] */
#define LOP_AND 'Y' /* [&] */
#define LOP_PIP 'X' /* [|] */
#define LOP_HAT 'W' /* [^] */
#define LOP_SUB 'V' /* [_] */
#define LOP_PERC 'U' /* [%] */
#define LOP_COL 'T' /* [:] */
#define LOP_EQU 'S' /* [=] */
#define LOP_DOT 'R' /* [.]  */
#define LOP_EXC 'Q' /* [!]  */
#define LOP_PLUS 'P' /* [+] */
#define LOP_QST 'O' /* [?] */

/* extended list codes */
#define LOP_XS (-14) /* this is added to extended code, mapping it to A->M */
#define LOP_XS_HAT_ADD 'I' /* ext= 'W' */

/* string constants used by install_lab etc: Note depend on LOP_'s */
#define STR_ATLS "[D0..0]"
#define STR_AARY "[Q$O@(1)]"
#define STR_DARY "[F$R@(1)]"
#define STR_AND  "[F$Y@(1)]"
#define STR_SARY "[A$Z]"
#define STR_INARY "input[A$Z]"
#define STR_OUTARY "output[A$Z]"
#define STR_AUXMER "~input[A$Z]"
#define STR_AUXIN "~input[D0..0]"
#define STR_AUXOUT "output[F$R@(1)]"

/* communications codes */
#define COMM_NONE '@' /* no comms allowed */
#define COMM_IN 'A'
#define COMM_OUT 'B'
#define COMM_INOUT 'C' /* input & output allowed */

/* special Op token used to indicate when decl Op is via an aux */
#define AUX_AOTOK 1
/* special Ip nums to indicate when Ip decl is an aux */
#define AUX_AFLY (-1)
#define AUX_BFLY (-2)

#define VIR_MAX 3 /* number of virtual nets: pile, heap & bin */
#define FAKE_MAX 4
/* virtual net handles (stored in decl_pt->sst_pos) */
#define VIR_PILE 1
#define VIR_HEAP 2
#define VIR_BIN 3
#define VIR_NET 4 /* dummy decl for labelled declarations */

#define LAB_NULL 0 /* label id not defined yet */
#define LAB_ROOT 1 /* base label id */

/* bits returned by do_a_decl & check_path */
#define PATH_ERR 0 /* path was neither terminally or non-terminally ended */
#define PATH_NON 2 /* path was non-terminally ended */
#define PATH_TER 4 /* path was terminally ended */

#define REVAL_NUM 2 /* number of times an ary may be reeval'd before */
                    /* it's aborted (elm not in list) */

#define PARY_CHILD (path_elm *)1  /* indicates if PARY is PARENT to DARY */


/* return a declarations handle, allows for virtual net handles */
#define dechd(ppt,dpt) decl_hand(ppt,dpt)

#define lmore(a,b,c,d) ((d *)nsl_lst_more(a,(int)(b),(int)(c),(int)sizeof(d)))

extern void free_ary(),free_allarys(),free_pelm(),free_path(),
            free_tab(),pathsel_val(),lfree(),
            rem_path(),clear_pelm(),clear_ary(),zap_ary(),zap_path(),
            tidy_ary(),tidy_pelm(),tidy_eval(),tidy_allarys(),tidy_reb(),
            yank_lab(),add_map_res(),var_init(),list_operator(),blank_pelm(),
            set_rebuild(),set_wild_flag(),reset_sels(),sels_path(),
            *nsl_lst_more(),*lcopy(),*lrev();
extern char *tack_pat(),*path_selector(),*reverse_list();
extern path_elm *new_path(),*get_level(),*rel_path(),*copy_path(),
                *dup_path(),*node_path(),*tack_lvl(),*byleft(),*byright();
extern tab_elm *new_tab();
extern reals selector_val();
extern mile decl_hand(),exc_selector(),part_selector(),
            get_nth(),path_info();
extern byte stack_str(),install_lab(),new_lab_id();
extern boolean part_wild(),check_grp(),is_const(),is_list(),is_wild(),lequal();
extern int rel_level(),eval_level(),make_path(),abs_hd(),abs_tl(),lab_mx_pos();
extern double nsl_lst_hilo(),nsl_lst_val(),*nsl_lst_nth();
extern mile nsl_lst_num(),nsl_lst_pos();
