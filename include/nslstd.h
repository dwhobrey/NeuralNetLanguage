/* Name: nslstd.h - Model Independent Function Library
*  NeSeL v2.1 April 1989  Neural Network Specification Language
*/
#ifndef _nslstd_h
#define _nslstd_h

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef T
#define T 1
#define F 0
#endif

/* Error return codes */
#define NSLER_OK 0 /* no error */
#define NSLER_OO 1 /* intermediate value for ER_OK */
#define NSLER_AS 2 /* array size error */
#define NSLER_BF 3 /* bad file */
#define NSLER_FL 4 /* access to file went wrong */
#define NSLER_OS 5 /* out of memory space */
#define NSLER_DC 6 /* declaration err */
#define NSLER_NL 7 /* access to node object block went wrong */
#define NSLER_VR 8 /* version number error */
#define NSLER_IX 9 /* nodenum not in index */
#define NSLER_MO 10 /* monitor struct info err */
#define NSLER_RE 11 /* conx map reconstruct err */
#define NSLER_VA 12 /* access to var failed */
#define NSLER_MP 13 /* monitor parsing err */
#define NSLER_NU 14 /* no network defined */
#define NSLER_NM 15 /* name in use already */
#define NSLER_ST 20 /* cycle was stopped */
#define NSLER_AB 21 /* cycle was aborted */

/* type codes used in lists etc */
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
#define TYPE_NET 16
#define TYPE_ARB 17
#define SIP_STR 18
#define SIP_NODE 19
#define SIP_CONX 20

/* list access macros & functions */
#define LST_SYS 5 /* number of reserved elements in a list */
#define LST_MEM (LST_SYS*sizeof(mile)) /* mem overheads for storing lst */
#define LST_MORE 8 /* space allocation page size in elms for lmore() */
#define LST_CODE 0x5679 /* sys code for checking list is valid */
#define LST_TYPE 0x1f /* mask for list type */
#define LST_PBIT 0x40 /* type bit set when list is in pair format */
#define LST_PAIR (0xc0+TYPE_SHORT) /* pair format list type code */
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

#ifdef NSL1
macro NSLAOp(s,p){s$p}
macro NSLSOpSize(x){$&x}
macro NSLSOpNum(x){$|x}
macro NSLSOpHi(x){$^x}
macro NSLSOpLo(x){$_x}
macro NSLSOpIn(x){$%x}
macro NSLSOpOut(x){$:x}
macro NSLSOpNode(x){$.x}
macro NSLSOpPos(x){$@x}
macro NSLSOpVal(x){$\#x}
macro NSLSOpAbs(x){$!@x}
macro NSLSOpRel(x){$!\#x}
macro NSLSOpEoc(x){$!!x}
macro NSLSOpLeaf(x){$!x}
macro NSLSOpDip(x){$*x}
macro NSLSOpPath(x){$+x}
macro NSLSOpType(x){$-x}
macro NSLLOpWild(x){*x}
macro NSLLOpSize(x){&x}
macro NSLLOpNum(x){|x}
macro NSLLOpHiLo(x){^x}
macro NSLLOpLoHi(x){_x}
macro NSLLOpIn(x){%x}
macro NSLLOpOut(x){:x}
macro NSLLOpEval(x){=x}
macro NSLLOpDS(x){.x}
macro NSLLOpCopy(x){!x}
macro NSLLOpAdd(x){+x}
macro NSLLOpApp(x){@x}
macro NSLLOpFit(x){?x}
macro NSLCOpAbs(x){@x}
macro NSLCOpCard(x){|x}
macro NSLCOpIn(x){?x}
macro NSLCOpOut(x){!x}
macro NSLCOpBase(x){.x}

/* User friendly macros for selectors & list operators */
/* Note meaning may change depending on x (x may be coors, exp or fix_path) */
/* Coordinate modifiers */
macro c_abs(x){NSLCOpAbs(x)}/* apply absolute coordinates */
macro c_card(x){NSLCOpCard(x)}/* apply cardinal coordinates */
macro c_in(x){NSLCOpIn(x)}/* apply to input path using cardinal */
macro c_out(x){NSLCOpOut(x)}/* apply to output path using cardinal */
macro c_base(x){NSLCOpBase(x)}/* apply to DS ary of base to specified path member*/
/* Selectors */
macro s_size(x){NSLSOpSize(x)}/* fixed size of array */
macro s_num(x){NSLSOpNum(x)}/* cur num elms in array */
macro s_hi(x){NSLSOpHi(x)}/* largest value in array */
macro s_lo(x){NSLSOpLo(x)}/* lowest value in array */
macro s_in(x){NSLSOpIn(x)}/* num elms in decl, or num inputs after '.' */
macro s_out(x){NSLSOpOut(x)}/* num elms in decl, or num outputs after '.' */
macro s_node(x){NSLSOpNode(x)} /* current node num */
macro s_pos(x){NSLSOpPos(x)}/* position of cur elm in array */
macro s_val(x){NSLSOpVal(x)}/* value of cur elm in array */
macro s_abs(x){NSLSOpAbs(x)}/* absolute num of next conx */
macro s_rel(x){NSLSOpRel(x)}/* relative num of next conx */
macro s_eoc(x){NSLSOpEoc(x)}/* returns non zero if not end of oelm */
macro s_leaf(p){NSLSOpLeaf(p)}/* num leaves in next oelm at part level p */
macro s_dip(x){NSLSOpDip(x)}/* path dip string, must copy before reuse */
macro s_path(x){NSLSOpPath(x)}/* path name string, copy before reuse */
macro s_type(x){NSLSOpType(x)}/* path type string, copy before reuse */
/* Selector Application path modifiers */
macro a_app(s,p){NSLAOp(s,p)}/* Application path selector */
macro a_in(s,p){NSLAOp(s,?p)}/* select input path */
macro a_out(s,p){NSLAOp(s,!p)}/* select output path */
macro a_label(s,p){NSLAOp(s,$p)}/* label relative path */

/* List Operators */
/* Note the following list macros should be enclosed by '[' & ']' when used */
macro l_wild(){NSLLOpWild()}/* [*] = wild array, use when a dummy needed */
macro l_size(x){NSLLOpSize(x)}/* [0..(s_size(x)-1)] if (s_size(x)-1)>0, else [0..0] */
macro l_num(x){NSLLOpNum(x)}/* [0..(s_num(x)-1)] if (s_num(x)-1)>0, else [0..0] */
macro l_hilo(x){NSLLOpHiLo(x)}/* [s_hi(x)..s_lo(x)] */
macro l_lohi(x){NSLLOpLoHi(x)}/* [s_lo(x)..s_hi(x)] */
macro l_in(x){NSLLOpIn(x)}/* [0..(s_in(x)-1)] if (s_in(x)-1)>0, else [0..0] */
macro l_out(x){NSLLOpOut(x)}/* [0..(s_out(x)-1)] if (s_out(x)-1)>0, else [0..0] */
macro l_eval(x){NSLLOpEval(x)}/* eval array string, returns resulting array */
macro l_ds(x){NSLLOpDS(x)}/* returns DS ary if in Ds, else [l_size(x)] */
macro l_copy(x){NSLLOpCopy(x)}/* takes corresponding ary as result */
macro l_add(x){NSLLOpAdd(x)}/* [(1+s_hi(c_base(x))..(1+s_hi(c_base(x))+s_leaf(-1))] append */
macro l_app(x){NSLLOpApp(x)}/* [(1+s_hi(c_base(x))..(2+s_hi(c_base(x))] append element */
macro l_fit(x){NSLLOpFit(x)}/* [l_add(x)] if +oelm or var, or [l_ds(x)] if -oelm */
#endif

/* Some useful macros */
#define repeat(x,r) int r;for(r=0;r<(x);++r)

extern boolean
  lequal(void *,void *); /* test lists for equality */
extern char
 *printfs(char *,va_list); /* like printf() but returns result as a quoted string */

extern void srnd(int), /* srnd(int seed), reseed p.r.n. generator */
  lfree(void *), /* free mem used by list */
  *nsl_lst_ma(void *,double,boolean,boolean), /*mult or add constant to lst*/
  *nsl_lst_vma(void *,void *,boolean,boolean), /*mult or add 2nd lst to 1st*/
  *nsl_lst_norm(void *,boolean), /* normalise vector */
  *nsl_lst_more(void *,int,int,int), /* increase list space */
  *lnew(int,int), /* create space for a new list */
  *lcopy(void *,void *), /* copy list */
  *lrev(void *), /* reverse list */
  *lcat(void *,void *), /* catenate lists */
  *llcat(va_list), /* catenate a list of lists to single list */
  *lintersect(void *,void *), /* intersection of lists */
  *lminus(void *,void *), /* subtracts elements of lists */
  *lunion(void *,void *), /* union of lists */
  *lmap(boolean,void *(*)(),va_list); /* map function to list args */
extern mile
 *lrand(mile,mile,mile,mile), /* list of random elements */
 *lsmap(mile *), /* (stack map) maps ary onto heap or pile */
 dtos(mile,mile,mile),  /* map two dimension to one */
 stodx(mile,mile),stody(mile,mile),  /* map one dimension to two */
 nsl_lst_num(void *), /* returns number of elms in list */
 nsl_lst_pos(void *,double); /* return pos of elm with value in list. */
extern float rndf(void); /* returns pseudo random number between 0.0 & 1.0 */
extern int rnd(int); /* rnd(int max): returns random int between 1 & max */
extern float generic_sigmoid(double,double,double,double,double),
  generic_limiter(double,double,double,double),
  generic_bound(double,double,double);
extern double
  nsl_lst_hilo(void *,boolean), /* returns smallest/largest value in list */
  nsl_lst_ndd(void *,mile), /* returns val of nth elm cast as a double */
  nsl_lst_val(void **,mile),
  *nsl_lst_nth(void *,mile),
  lsum(void *), /* sum elements of list */
  lhopsum(mile,mile,mile,double,double,char *);

#define lmore(a,b,c,d) ((d *)nsl_lst_more(a,(int)(b),(int)(c),(int)sizeof(d)))
#define lnum(a) nsl_lst_num(a)
#define lpos(a,v) nsl_lst_pos(a,v)
#define lval(a,n) nsl_lst_ndd(a,b)
#define lnorm(a) nsl_lst_norm(a,(boolean)1)
#define llo(a) nsl_lst_hilo(a,(boolean)0)
#define lhi(a) nsl_lst_hilo(a,(boolean)1)
#define lmul(a,c) nsl_lst_ma(a,c,(boolean)1,(boolean)1)
#define ladd(a,c) nsl_lst_ma(a,c,(boolean)0,(boolean)1)
#define llmul(a,b) nsl_lst_vma(a,b,(boolean)1,(boolean)1)
#define lladd(a,b) nsl_lst_vma(a,b,(boolean)0,(boolean)1)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*_nslstd_h*/
