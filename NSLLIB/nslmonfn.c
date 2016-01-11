char *NSLM="NeSeL(u)v2.1(DJRW)1989:UserFuncs";
typedef unsigned int size_t;
typedef unsigned short wchar_t;
typedef wchar_t wint_t;
typedef wchar_t wctype_t;
typedef char *  va_list;
struct _iobuf {
char *_ptr;
int   _cnt;
char *_base;
int   _flag;
int   _file;
int   _charbuf;
int   _bufsiz;
char *_tmpfname;
};
typedef struct _iobuf FILE;
extern FILE _iob[];
typedef struct fpos_t {
unsigned int lopart;
int          hipart;
} fpos_t;
int _filbuf(FILE *);
int _flsbuf(int, FILE *);
FILE * _fsopen(const char *, const char *, int);
void clearerr(FILE *);
int fclose(FILE *);
int _fcloseall(void);
FILE * _fdopen(int, const char *);
int feof(FILE *);
int ferror(FILE *);
int fflush(FILE *);
int fgetc(FILE *);
int _fgetchar(void);
int fgetpos(FILE *, fpos_t *);
char * fgets(char *, int, FILE *);
int _fileno(FILE *);
int _flushall(void);
FILE * fopen(const char *, const char *);
int fprintf(FILE *, const char *, ...);
int fputc(int, FILE *);
int _fputchar(int);
int fputs(const char *, FILE *);
size_t fread(void *, size_t, size_t, FILE *);
FILE * freopen(const char *, const char *, FILE *);
int fscanf(FILE *, const char *, ...);
int fsetpos(FILE *, const fpos_t *);
int fseek(FILE *, long, int);
long ftell(FILE *);
size_t fwrite(const void *, size_t, size_t, FILE *);
int getc(FILE *);
int getchar(void);
int _getmaxstdio(void);
char * gets(char *);
int _getw(FILE *);
void perror(const char *);
int _pclose(FILE *);
FILE * _popen(const char *, const char *);
int printf(const char *, ...);
int putc(int, FILE *);
int putchar(int);
int puts(const char *);
int _putw(int, FILE *);
int remove(const char *);
int rename(const char *, const char *);
void rewind(FILE *);
int _rmtmp(void);
int scanf(const char *, ...);
void setbuf(FILE *, char *);
int _setmaxstdio(int);
int setvbuf(FILE *, char *, int, size_t);
int _snprintf(char *, size_t, const char *, ...);
int sprintf(char *, const char *, ...);
int sscanf(const char *, const char *, ...);
char * _tempnam(const char *, const char *);
FILE * tmpfile(void);
char * tmpnam(char *);
int ungetc(int, FILE *);
int _unlink(const char *);
int vfprintf(FILE *, const char *, va_list);
int vprintf(const char *, va_list);
int _vsnprintf(char *, size_t, const char *, va_list);
int vsprintf(char *, const char *, va_list);
FILE * _wfsopen(const wchar_t *, const wchar_t *, int);
wint_t fgetwc(FILE *);
wint_t _fgetwchar(void);
wint_t fputwc(wint_t, FILE *);
wint_t _fputwchar(wint_t);
wint_t getwc(FILE *);
wint_t getwchar(void);
wint_t putwc(wint_t, FILE *);
wint_t putwchar(wint_t);
wint_t ungetwc(wint_t, FILE *);
wchar_t * fgetws(wchar_t *, int, FILE *);
int fputws(const wchar_t *, FILE *);
wchar_t * _getws(wchar_t *);
int _putws(const wchar_t *);
int fwprintf(FILE *, const wchar_t *, ...);
int wprintf(const wchar_t *, ...);
int _snwprintf(wchar_t *, size_t, const wchar_t *, ...);
int swprintf(wchar_t *, const wchar_t *, ...);
int vfwprintf(FILE *, const wchar_t *, va_list);
int vwprintf(const wchar_t *, va_list);
int _vsnwprintf(wchar_t *, size_t, const wchar_t *, va_list);
int vswprintf(wchar_t *, const wchar_t *, va_list);
int fwscanf(FILE *, const wchar_t *, ...);
int swscanf(const wchar_t *, const wchar_t *, ...);
int wscanf(const wchar_t *, ...);
FILE * _wfdopen(int, const wchar_t *);
FILE * _wfopen(const wchar_t *, const wchar_t *);
FILE * _wfreopen(const wchar_t *, const wchar_t *, FILE *);
void _wperror(const wchar_t *);
FILE * _wpopen(const wchar_t *, const wchar_t *);
int _wremove(const wchar_t *);
wchar_t * _wtempnam(const wchar_t *, const wchar_t *);
wchar_t * _wtmpnam(wchar_t *);
int fcloseall(void);
FILE * fdopen(int, const char *);
int fgetchar(void);
int fileno(FILE *);
int flushall(void);
int fputchar(int);
int getw(FILE *);
int putw(int, FILE *);
int rmtmp(void);
char * tempnam(const char *, const char *);
int unlink(const char *);
void * memcpy(void *, const void *, size_t);
int memcmp(const void *, const void *, size_t);
void * memset(void *, int, size_t);
char * _strset(char *, int);
char * strcpy(char *, const char *);
char * strcat(char *, const char *);
int strcmp(const char *, const char *);
size_t strlen(const char *);
void * _memccpy(void *, const void *, int, unsigned int);
void * memchr(const void *, int, size_t);
int _memicmp(const void *, const void *, unsigned int);
void * memmove(void *, const void *, size_t);
char * strchr(const char *, int);
int _strcmpi(const char *, const char *);
int _stricmp(const char *, const char *);
int strcoll(const char *, const char *);
int _stricoll(const char *, const char *);
int _strncoll(const char *, const char *, size_t);
int _strnicoll(const char *, const char *, size_t);
size_t strcspn(const char *, const char *);
char * _strdup(const char *);
char * _strerror(const char *);
char * strerror(int);
char * _strlwr(char *);
char * strncat(char *, const char *, size_t);
int strncmp(const char *, const char *, size_t);
int _strnicmp(const char *, const char *, size_t);
char * strncpy(char *, const char *, size_t);
char * _strnset(char *, int, size_t);
char * strpbrk(const char *, const char *);
char * strrchr(const char *, int);
char * _strrev(char *);
size_t strspn(const char *, const char *);
char * strstr(const char *, const char *);
char * strtok(char *, const char *);
char * _strupr(char *);
size_t strxfrm (char *, const char *, size_t);
void * memccpy(void *, const void *, int, unsigned int);
int memicmp(const void *, const void *, unsigned int);
int strcmpi(const char *, const char *);
int stricmp(const char *, const char *);
char * strdup(const char *);
char * strlwr(char *);
int strnicmp(const char *, const char *, size_t);
char * strnset(char *, int, size_t);
char * strrev(char *);
char * strset(char *, int);
char * strupr(char *);
wchar_t * wcscat(wchar_t *, const wchar_t *);
wchar_t * wcschr(const wchar_t *, wchar_t);
int wcscmp(const wchar_t *, const wchar_t *);
wchar_t * wcscpy(wchar_t *, const wchar_t *);
size_t wcscspn(const wchar_t *, const wchar_t *);
size_t wcslen(const wchar_t *);
wchar_t * wcsncat(wchar_t *, const wchar_t *, size_t);
int wcsncmp(const wchar_t *, const wchar_t *, size_t);
wchar_t * wcsncpy(wchar_t *, const wchar_t *, size_t);
wchar_t * wcspbrk(const wchar_t *, const wchar_t *);
wchar_t * wcsrchr(const wchar_t *, wchar_t);
size_t wcsspn(const wchar_t *, const wchar_t *);
wchar_t * wcsstr(const wchar_t *, const wchar_t *);
wchar_t * wcstok(wchar_t *, const wchar_t *);
wchar_t * _wcsdup(const wchar_t *);
int _wcsicmp(const wchar_t *, const wchar_t *);
int _wcsnicmp(const wchar_t *, const wchar_t *, size_t);
wchar_t * _wcsnset(wchar_t *, wchar_t, size_t);
wchar_t * _wcsrev(wchar_t *);
wchar_t * _wcsset(wchar_t *, wchar_t);
wchar_t * _wcslwr(wchar_t *);
wchar_t * _wcsupr(wchar_t *);
size_t wcsxfrm(wchar_t *, const wchar_t *, size_t);
int wcscoll(const wchar_t *, const wchar_t *);
int _wcsicoll(const wchar_t *, const wchar_t *);
int _wcsncoll(const wchar_t *, const wchar_t *, size_t);
int _wcsnicoll(const wchar_t *, const wchar_t *, size_t);
wchar_t * wcsdup(const wchar_t *);
int wcsicmp(const wchar_t *, const wchar_t *);
int wcsnicmp(const wchar_t *, const wchar_t *, size_t);
wchar_t * wcsnset(wchar_t *, wchar_t, size_t);
wchar_t * wcsrev(wchar_t *);
wchar_t * wcsset(wchar_t *, wchar_t);
wchar_t * wcslwr(wchar_t *);
wchar_t * wcsupr(wchar_t *);
int wcsicoll(const wchar_t *, const wchar_t *);
extern unsigned short _ctype[];
extern unsigned short *_pctype;
extern wctype_t *_pwctype;
int _isctype(int, int);
int isalpha(int);
int isupper(int);
int islower(int);
int isdigit(int);
int isxdigit(int);
int isspace(int);
int ispunct(int);
int isalnum(int);
int isprint(int);
int isgraph(int);
int iscntrl(int);
int toupper(int);
int tolower(int);
int _tolower(int);
int _toupper(int);
int __isascii(int);
int __toascii(int);
int __iscsymf(int);
int __iscsym(int);
int iswalpha(wint_t);
int iswupper(wint_t);
int iswlower(wint_t);
int iswdigit(wint_t);
int iswxdigit(wint_t);
int iswspace(wint_t);
int iswpunct(wint_t);
int iswalnum(wint_t);
int iswprint(wint_t);
int iswgraph(wint_t);
int iswcntrl(wint_t);
int iswascii(wint_t);
int isleadbyte(int);
wchar_t towupper(wchar_t);
wchar_t towlower(wchar_t);
int iswctype(wint_t, wctype_t);
int is_wctype(wint_t, wctype_t);
extern int __mb_cur_max;
typedef struct _heapinfo {
int * _pentry;
size_t _size;
int _useflag;
} _HEAPINFO;
extern unsigned int _amblksiz;
void * calloc(size_t, size_t);
void free(void *);
void * malloc(size_t);
void * realloc(void *, size_t);
void * _alloca(size_t);
void * _expand(void *, size_t);
size_t _get_sbh_threshold(void);
int _set_sbh_threshold(size_t);
int _heapadd(void *, size_t);
int _heapchk(void);
int _heapmin(void);
int _heapset(unsigned int);
int _heapwalk(_HEAPINFO *);
size_t _heapused(size_t *, size_t *);
size_t _msize(void *);
struct _exception {
int type;
char *name;
double arg1;
double arg2;
double retval;
} ;
struct _complex {
double x,y;
} ;
extern double _HUGE;
int abs(int);
double acos(double);
double asin(double);
double atan(double);
double atan2(double, double);
double cos(double);
double cosh(double);
double exp(double);
double fabs(double);
double fmod(double, double);
long labs(long);
double log(double);
double log10(double);
double pow(double, double);
double sin(double);
double sinh(double);
double tan(double);
double tanh(double);
double sqrt(double);
double atof(const char *);
double _cabs(struct _complex);
double ceil(double);
double floor(double);
double frexp(double, int *);
double _hypot(double, double);
double _j0(double);
double _j1(double);
double _jn(int, double);
double ldexp(double, int);
int _matherr(struct _exception *);
double modf(double, double *);
double _y0(double);
double _y1(double);
double _yn(int, double);
extern double HUGE;
double cabs(struct _complex);
double hypot(double, double);
double j0(double);
double j1(double);
double jn(int, double);
int _matherr (struct _exception *);
double y0(double);
double y1(double);
double yn(int, double);
typedef int BOOL;
typedef short int mile;
typedef long yard;
typedef unsigned char boolean;
typedef struct _NSLNSLfelm
{ short swt;
char (*fn_pt)(),
*ats;
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
typedef struct _NSLNSLid
{long name; char *path;
} NSLNSLid;
typedef struct _NSLNSLtime
{int rate,wait;
} NSLNSLtime;
typedef struct _NSLNSLstruct
{long sub_nodes,ary_size; int type_code; char *offset,*member,*tag;
} NSLNSLstruct;
typedef struct _NSLNSLvar
{ short type_code; char *adrs,*name;
} NSLNSLvar;
typedef struct _NSLNSLio
{ short cmd; char *args;
} NSLNSLio;
typedef struct _NSLNSLdrv
{ long nd; NSLNSLio *iop;
} NSLNSLdrv;
typedef struct _NSLNSLconx
{short len; char *cpt,*dpt;
} NSLNSLconx;
typedef struct _NSLNSLnode
{short fnum; char *dpt; NSLNSLtime *tpt;
} NSLNSLnode;
typedef struct _NSLNSLnet
{ int flags,
num_funcs,
num_conxs,
num_nodes;
long time;
void (**fpt)();
long sst_bs,*sstpt;
NSLNSLconx *cpt;
NSLNSLnode *npt;
char *name,*sst_name,*var_name,*ins_name;
int *num_ufs,
*num_sts,
*num_six,
*num_vas,
*sixpt;
void (*ept)();
char **upt;
NSLNSLfelm *apt;
NSLNSLstruct *spt;
NSLNSLvar *vpt;
NSLNSLdrv *dpt;
int num_drv;
void **mpt;
void *args;
} NSLNSLnet;
extern boolean
lequal(void *,void *);
extern char
*printfs(char *,va_list);
extern void srnd(int),
lfree(void *),
*nsl_lst_ma(void *,double,boolean,boolean),
*nsl_lst_vma(void *,void *,boolean,boolean),
*nsl_lst_norm(void *,boolean),
*nsl_lst_more(void *,int,int,int),
*lnew(int,int),
*lcopy(void *,void *),
*lrev(void *),
*lcat(void *,void *),
*llcat(va_list),
*lintersect(void *,void *),
*lminus(void *,void *),
*lunion(void *,void *),
*lmap(boolean,void *(*)(),va_list);
extern mile
*lrand(mile,mile,mile,mile),
*lsmap(mile *),
dtos(mile,mile,mile),
stodx(mile,mile),stody(mile,mile),
nsl_lst_num(void *),
nsl_lst_pos(void *,double);
extern float rndf(void);
extern int rnd(int);
extern float generic_sigmoid(double,double,double,double,double),
generic_limiter(double,double,double,double),
generic_bound(double,double,double);
extern double
nsl_lst_hilo(void *,boolean),
nsl_lst_ndd(void *,mile),
nsl_lst_val(void **,mile),
*nsl_lst_nth(void *,mile),
lsum(void *),
lhopsum(mile,mile,mile,double,double,char *);
extern int nsl_stop,nsl_abort,nsl_err,nsl_verbatim;
extern int nsl_mon_min,nsl_mon_max,nsl_mon_rate,nsl_mon_delay;
extern NSLNSLnet **nsl_net_list;
extern char *nsl_mon_names[];
extern NSLNSLfelm nsl_mon_list[];
extern NSLNSLstruct nsl_mon_structs[];
extern NSLNSLvar nsl_mon_vars[];
extern int nsl_mon_stindx[];
extern int nsl_num_mon_list,nsl_num_mon_structs,
nsl_num_mon_stindx,nsl_num_mon_vars;
extern NSLNSLfval nsl_mon_evaluate();
extern void nsl_mon_wnds(void);
extern int nsl_monitor(char *);
extern int nsl_install(NSLNSLnet *,char *);
extern int nsl_uninstall(char *);
extern int nsl_order(char *,void (*)());
extern int nsl_driver(NSLNSLnet *,int,...);
extern int nsl_cycle(NSLNSLnet *,long);
extern int nsl_put(NSLNSLnet *,char *);
extern NSLNSLnet *nsl_get(char *);
extern NSLNSLnet *nsl_get_p(char *);
extern void nsl_free(NSLNSLnet *);
extern long nsl_handle(NSLNSLnet *,char *);
extern void *nsl_get_user_args(NSLNSLnet *netpt);
extern void nsl_set_user_args(NSLNSLnet *netpt,void *args);
extern int nsl_write(NSLNSLnet *,long,char *,double);
extern int nsl_write_p(NSLNSLnet *,long,char *,char *);
extern double nsl_read(NSLNSLnet *,long,char *);
extern char *nsl_read_p(NSLNSLnet *,long,char *);
extern char *nsl_adjust(NSLNSLnet *,char *);
extern float
vec_sum(void *,int,int,NSLNSLnet *,int),
vec_norm(void *,int,int,NSLNSLnet *,int),
vec_prod(void *,void *,int,int,int,NSLNSLnet *,NSLNSLnet *,
	int,int),
vec_liprod(void *,void *,int,int,int,NSLNSLnet *,NSLNSLnet *,
	int,int,double,double);
extern int wnd_num_colors,wnd_white,wnd_black,wnd_std,wnd_all;
extern int w_exit_code,w_exit_called;
extern int w_x(long yx);
extern int w_y(long yx);
extern long w_xy(int x,int y);
extern BOOL w_active(int);
extern int w_arc(int,int,int,int,int,int,int,int,int);
extern void w_blank(int,BOOL);
extern int w_bordercolor(int,int);
extern int w_circle(int,int);
extern void w_clear(int);
extern void w_clear_line(int);
extern void w_close(int);
extern void w_dump(int,int,...);
extern int w_ellipse(int,int,int);
extern void w_exit(int retcode);
extern void w_focus(int w);
extern int w_fill(int);
extern int w_fillstate(int,int);
extern void w_fpcheck();
extern int w_getch(int);
extern int w_getpixel(int,int,int);
extern char *w_gets(int,char *);
extern int w_handle(char *);
extern void w_init();
extern int w_more(int,int);
extern int w_move(int,int);
extern int w_moveto(int,int,int);
extern int w_onscreen(int);
extern int w_open(char *,BOOL,BOOL,BOOL,BOOL,BOOL,int,int,int,int);
extern long w_origin(int w,int x,int y);
extern int w_papercolor(int,int);
extern int w_penchar(int,int);
extern int w_pencolor(int,int);
extern int w_pendown(int,int);
extern int w_penwidth(int w,int n);
extern int w_poly(int,int,int);
extern long w_position(int,int,int);
extern int w_printf(int,char *,...);
extern void w_puts(int,char *);
extern int w_rectangle(int,int,int,int,int);
extern void w_refresh(int,BOOL);
extern void w_report(int);
extern long w_resize(int w,int x,int y);
extern int w_setjmp();
extern int w_setpixel(int,int,int);
extern long w_size(int,BOOL);
extern char *w_title(int,int,int,...);
extern int w_track(int,int,...);
extern int w_turn(int,int);
extern int w_turnto(int,int);
extern void w_update(int w);
NSLNSLstruct nsl_mon_structs[]={
{0}};
int nsl_num_mon_structs=1;
int nsl_mon_stindx[]={
(-1)};
int nsl_num_mon_stindx=0;
NSLNSLvar nsl_mon_vars[]={
{4,(char *)&nsl_abort,"nsl_abort"},
{4,(char *)&nsl_err,"nsl_err"},
{4,(char *)&nsl_mon_delay,"nsl_mon_delay"},
{4,(char *)&nsl_mon_max,"nsl_mon_max"},
{4,(char *)&nsl_mon_min,"nsl_mon_min"},
{4,(char *)&nsl_mon_rate,"nsl_mon_rate"},
{72,(char *)&nsl_net_list,"nsl_net_list"},
{4,(char *)&nsl_num_mon_list,"nsl_num_mon_list"},
{4,(char *)&nsl_num_mon_stindx,"nsl_num_mon_stindx"},
{4,(char *)&nsl_num_mon_structs,"nsl_num_mon_structs"},
{4,(char *)&nsl_num_mon_vars,"nsl_num_mon_vars"},
{4,(char *)&nsl_stop,"nsl_stop"},
{4,(char *)&nsl_verbatim,"nsl_verbatim"},
{4,(char *)&w_exit_called,"w_exit_called"},
{4,(char *)&w_exit_code,"w_exit_code"},
{4,(char *)&wnd_all,"wnd_all"},
{4,(char *)&wnd_black,"wnd_black"},
{4,(char *)&wnd_num_colors,"wnd_num_colors"},
{4,(char *)&wnd_std,"wnd_std"},
{4,(char *)&wnd_white,"wnd_white"},
{0,(char *)0L,(char *)0L}};
int nsl_num_mon_vars=20;
char *nsl_mon_names[]={
"__isascii","__iscsym","__iscsymf","__toascii",
"_cabs","_expand","_fcloseall","_fdopen",
"_fgetchar","_fgetwchar","_filbuf","_fileno",
"_flsbuf","_flushall","_fputchar","_fputwchar",
"_fsopen","_get_sbh_threshold","_getmaxstdio","_getw",
"_getws","_heapadd","_heapchk","_heapmin",
"_heapset","_heapused","_heapwalk","_hypot",
"_isctype","_j0","_j1","_jn",
"_matherr","_memccpy","_memicmp","_msize",
"_pclose","_popen","_putw","_putws",
"_rmtmp","_set_sbh_threshold","_setmaxstdio","_snprintf",
"_snwprintf","_strcmpi","_strdup","_strerror",
"_stricmp","_stricoll","_strlwr","_strncoll",
"_strnicmp","_strnicoll","_strnset","_strrev",
"_strset","_strupr","_tempnam","_tolower",
"_toupper","_unlink","_vsnprintf","_vsnwprintf",
"_wcsdup","_wcsicmp","_wcsicoll","_wcslwr",
"_wcsncoll","_wcsnicmp","_wcsnicoll","_wcsnset",
"_wcsrev","_wcsset","_wcsupr","_wfdopen",
"_wfopen","_wfreopen","_wfsopen","_wperror",
"_wpopen","_wremove","_wtempnam","_wtmpnam",
"_y0","_y1","_yn","abs",
"acos","asin","atan","atan2",
"atof","cabs","calloc","ceil",
"clearerr","cos","cosh","dtos",
"exp","fabs","fclose","fcloseall",
"fdopen","feof","ferror","fflush",
"fgetc","fgetchar","fgetpos","fgets",
"fgetwc","fgetws","fileno","floor",
"flushall","fmod","fopen","fprintf",
"fputc","fputchar","fputs","fputwc",
"fputws","fread","free","freopen",
"frexp","fscanf","fseek","fsetpos",
"ftell","fwprintf","fwrite","fwscanf",
"generic_bound","generic_limiter","generic_sigmoid","getc",
"getchar","gets","getw","getwc",
"getwchar","hypot","is_wctype","isalnum",
"isalpha","iscntrl","isdigit","isgraph",
"isleadbyte","islower","isprint","ispunct",
"isspace","isupper","iswalnum","iswalpha",
"iswascii","iswcntrl","iswctype","iswdigit",
"iswgraph","iswlower","iswprint","iswpunct",
"iswspace","iswupper","iswxdigit","isxdigit",
"j0","j1","jn","labs",
"lcat","lcopy","ldexp","lequal",
"lfree","lhopsum","lintersect","llcat",
"lmap","lminus","lnew","log",
"log10","lrand","lrev","lsmap",
"lsum","lunion","malloc","memccpy",
"memchr","memcmp","memcpy","memicmp",
"memmove","memset","modf","nsl_adjust",
"nsl_cycle","nsl_driver","nsl_free","nsl_get",
"nsl_get_p","nsl_get_user_args","nsl_handle","nsl_install",
"nsl_lst_hilo","nsl_lst_ma","nsl_lst_more","nsl_lst_ndd",
"nsl_lst_norm","nsl_lst_nth","nsl_lst_num","nsl_lst_pos",
"nsl_lst_val","nsl_lst_vma","nsl_mon_evaluate","nsl_mon_wnds",
"nsl_monitor","nsl_order","nsl_put","nsl_read",
"nsl_read_p","nsl_set_user_args","nsl_uninstall","nsl_write",
"nsl_write_p","perror","pow","printf",
"printfs","putc","putchar","puts",
"putw","putwc","putwchar","realloc",
"remove","rename","rewind","rmtmp",
"rnd","rndf","scanf","setbuf",
"setvbuf","sin","sinh","sprintf",
"sqrt","srnd","sscanf","stodx",
"stody","strcat","strchr","strcmp",
"strcmpi","strcoll","strcpy","strcspn",
"strdup","strerror","stricmp","strlen",
"strlwr","strncat","strncmp","strncpy",
"strnicmp","strnset","strpbrk","strrchr",
"strrev","strset","strspn","strstr",
"strtok","strupr","strxfrm","swprintf",
"swscanf","tan","tanh","tempnam",
"tmpfile","tmpnam","tolower","toupper",
"towlower","towupper","ungetc","ungetwc",
"unlink","vec_liprod","vec_norm","vec_prod",
"vec_sum","vfprintf","vfwprintf","vprintf",
"vsprintf","vswprintf","vwprintf","w_active",
"w_arc","w_blank","w_bordercolor","w_circle",
"w_clear","w_clear_line","w_close","w_dump",
"w_ellipse","w_exit","w_fill","w_fillstate",
"w_focus","w_fpcheck","w_getch","w_getpixel",
"w_gets","w_handle","w_init","w_more",
"w_move","w_moveto","w_onscreen","w_open",
"w_origin","w_papercolor","w_penchar","w_pencolor",
"w_pendown","w_penwidth","w_poly","w_position",
"w_printf","w_puts","w_rectangle","w_refresh",
"w_report","w_resize","w_setjmp","w_setpixel",
"w_size","w_title","w_track","w_turn",
"w_turnto","w_update","w_x","w_xy",
"w_y","wcscat","wcschr","wcscmp",
"wcscoll","wcscpy","wcscspn","wcsdup",
"wcsicmp","wcsicoll","wcslen","wcslwr",
"wcsncat","wcsncmp","wcsncpy","wcsnicmp",
"wcsnset","wcspbrk","wcsrchr","wcsrev",
"wcsset","wcsspn","wcsstr","wcstok",
"wcsupr","wcsxfrm","wprintf","wscanf",
"y0","y1","yn",(char *)0};
NSLNSLfelm nsl_mon_list[]={
{0,(char (*)())__isascii,"DD"},
{0,(char (*)())__iscsym,"DD"},
{0,(char (*)())__iscsymf,"DD"},
{0,(char (*)())__toascii,"DD"},
{1,(char (*)())_cabs,"BH"},
{2,(char (*)())_expand,"VVL"},
{3,(char (*)())_fcloseall,"DF"},
{4,(char (*)())_fdopen,"XDQ"},
{3,(char (*)())_fgetchar,"DF"},
{5,(char (*)())_fgetwchar,"NF"},
{6,(char (*)())_filbuf,"DX"},
{6,(char (*)())_fileno,"DX"},
{7,(char (*)())_flsbuf,"DDX"},
{3,(char (*)())_flushall,"DF"},
{0,(char (*)())_fputchar,"DD"},
{8,(char (*)())_fputwchar,"NN"},
{9,(char (*)())_fsopen,"XQQD"},
{10,(char (*)())_get_sbh_threshold,"LF"},
{3,(char (*)())_getmaxstdio,"DF"},
{6,(char (*)())_getw,"DX"},
{11,(char (*)())_getws,"^^"},
{12,(char (*)())_heapadd,"DVL"},
{3,(char (*)())_heapchk,"DF"},
{3,(char (*)())_heapmin,"DF"},
{13,(char (*)())_heapset,"DL"},
{14,(char (*)())_heapused,"L\\"},
{6,(char (*)())_heapwalk,"DX"},
{15,(char (*)())_hypot,"BBB"},
{16,(char (*)())_isctype,"DDD"},
{17,(char (*)())_j0,"BB"},
{17,(char (*)())_j1,"BB"},
{18,(char (*)())_jn,"BDB"},
{6,(char (*)())_matherr,"DX"},
{19,(char (*)())_memccpy,"VVVDL"},
{20,(char (*)())_memicmp,"DVVL"},
{21,(char (*)())_msize,"LV"},
{6,(char (*)())_pclose,"DX"},
{22,(char (*)())_popen,"XQQ"},
{7,(char (*)())_putw,"DDX"},
{23,(char (*)())_putws,"D^"},
{3,(char (*)())_rmtmp,"DF"},
{13,(char (*)())_set_sbh_threshold,"DL"},
{0,(char (*)())_setmaxstdio,"DD"},
{24,(char (*)())_snprintf,"DQLQ0"},
{25,(char (*)())_snwprintf,"D^L^0"},
{26,(char (*)())_strcmpi,"DQQ"},
{27,(char (*)())_strdup,"QQ"},
{27,(char (*)())_strerror,"QQ"},
{26,(char (*)())_stricmp,"DQQ"},
{26,(char (*)())_stricoll,"DQQ"},
{27,(char (*)())_strlwr,"QQ"},
{28,(char (*)())_strncoll,"DQQL"},
{28,(char (*)())_strnicmp,"DQQL"},
{28,(char (*)())_strnicoll,"DQQL"},
{29,(char (*)())_strnset,"QQDL"},
{27,(char (*)())_strrev,"QQ"},
{30,(char (*)())_strset,"QQD"},
{27,(char (*)())_strupr,"QQ"},
{31,(char (*)())_tempnam,"QQQ"},
{0,(char (*)())_tolower,"DD"},
{0,(char (*)())_toupper,"DD"},
{32,(char (*)())_unlink,"DQ"},
{24,(char (*)())_vsnprintf,"DQLQ0"},
{25,(char (*)())_vsnwprintf,"D^L^0"},
{11,(char (*)())_wcsdup,"^^"},
{33,(char (*)())_wcsicmp,"D^^"},
{33,(char (*)())_wcsicoll,"D^^"},
{11,(char (*)())_wcslwr,"^^"},
{34,(char (*)())_wcsncoll,"D^^L"},
{34,(char (*)())_wcsnicmp,"D^^L"},
{34,(char (*)())_wcsnicoll,"D^^L"},
{35,(char (*)())_wcsnset,"^^NL"},
{11,(char (*)())_wcsrev,"^^"},
{36,(char (*)())_wcsset,"^^N"},
{11,(char (*)())_wcsupr,"^^"},
{37,(char (*)())_wfdopen,"XD^"},
{38,(char (*)())_wfopen,"X^^"},
{39,(char (*)())_wfreopen,"X^^X"},
{40,(char (*)())_wfsopen,"X^^D"},
{41,(char (*)())_wperror,"F^"},
{38,(char (*)())_wpopen,"X^^"},
{23,(char (*)())_wremove,"D^"},
{42,(char (*)())_wtempnam,"^^^"},
{11,(char (*)())_wtmpnam,"^^"},
{17,(char (*)())_y0,"BB"},
{17,(char (*)())_y1,"BB"},
{18,(char (*)())_yn,"BDB"},
{0,(char (*)())abs,"DD"},
{17,(char (*)())acos,"BB"},
{17,(char (*)())asin,"BB"},
{17,(char (*)())atan,"BB"},
{15,(char (*)())atan2,"BBB"},
{43,(char (*)())atof,"BQ"},
{1,(char (*)())cabs,"BH"},
{44,(char (*)())calloc,"VLL"},
{17,(char (*)())ceil,"BB"},
{45,(char (*)())clearerr,"FX"},
{17,(char (*)())cos,"BB"},
{17,(char (*)())cosh,"BB"},
{46,(char (*)())dtos,"JJJJ"},
{17,(char (*)())exp,"BB"},
{17,(char (*)())fabs,"BB"},
{6,(char (*)())fclose,"DX"},
{3,(char (*)())fcloseall,"DF"},
{4,(char (*)())fdopen,"XDQ"},
{6,(char (*)())feof,"DX"},
{6,(char (*)())ferror,"DX"},
{6,(char (*)())fflush,"DX"},
{6,(char (*)())fgetc,"DX"},
{3,(char (*)())fgetchar,"DF"},
{47,(char (*)())fgetpos,"DXX"},
{48,(char (*)())fgets,"QQDX"},
{49,(char (*)())fgetwc,"NX"},
{50,(char (*)())fgetws,"^^DX"},
{6,(char (*)())fileno,"DX"},
{17,(char (*)())floor,"BB"},
{3,(char (*)())flushall,"DF"},
{15,(char (*)())fmod,"BBB"},
{22,(char (*)())fopen,"XQQ"},
{51,(char (*)())fprintf,"DXQ0"},
{7,(char (*)())fputc,"DDX"},
{0,(char (*)())fputchar,"DD"},
{52,(char (*)())fputs,"DQX"},
{53,(char (*)())fputwc,"NNX"},
{54,(char (*)())fputws,"D^X"},
{55,(char (*)())fread,"LVLLX"},
{56,(char (*)())free,"FV"},
{57,(char (*)())freopen,"XQQX"},
{58,(char (*)())frexp,"BBT"},
{51,(char (*)())fscanf,"DXQ0"},
{59,(char (*)())fseek,"DXED"},
{47,(char (*)())fsetpos,"DXX"},
{60,(char (*)())ftell,"EX"},
{61,(char (*)())fwprintf,"DX^0"},
{55,(char (*)())fwrite,"LVLLX"},
{61,(char (*)())fwscanf,"DX^0"},
{62,(char (*)())generic_bound,"CBBB"},
{63,(char (*)())generic_limiter,"CBBBB"},
{64,(char (*)())generic_sigmoid,"CBBBBB"},
{6,(char (*)())getc,"DX"},
{3,(char (*)())getchar,"DF"},
{27,(char (*)())gets,"QQ"},
{6,(char (*)())getw,"DX"},
{49,(char (*)())getwc,"NX"},
{5,(char (*)())getwchar,"NF"},
{15,(char (*)())hypot,"BBB"},
{65,(char (*)())is_wctype,"DNN"},
{0,(char (*)())isalnum,"DD"},
{0,(char (*)())isalpha,"DD"},
{0,(char (*)())iscntrl,"DD"},
{0,(char (*)())isdigit,"DD"},
{0,(char (*)())isgraph,"DD"},
{0,(char (*)())isleadbyte,"DD"},
{0,(char (*)())islower,"DD"},
{0,(char (*)())isprint,"DD"},
{0,(char (*)())ispunct,"DD"},
{0,(char (*)())isspace,"DD"},
{0,(char (*)())isupper,"DD"},
{66,(char (*)())iswalnum,"DN"},
{66,(char (*)())iswalpha,"DN"},
{66,(char (*)())iswascii,"DN"},
{66,(char (*)())iswcntrl,"DN"},
{65,(char (*)())iswctype,"DNN"},
{66,(char (*)())iswdigit,"DN"},
{66,(char (*)())iswgraph,"DN"},
{66,(char (*)())iswlower,"DN"},
{66,(char (*)())iswprint,"DN"},
{66,(char (*)())iswpunct,"DN"},
{66,(char (*)())iswspace,"DN"},
{66,(char (*)())iswupper,"DN"},
{66,(char (*)())iswxdigit,"DN"},
{0,(char (*)())isxdigit,"DD"},
{17,(char (*)())j0,"BB"},
{17,(char (*)())j1,"BB"},
{18,(char (*)())jn,"BDB"},
{67,(char (*)())labs,"EE"},
{68,(char (*)())lcat,"VVV"},
{68,(char (*)())lcopy,"VVV"},
{69,(char (*)())ldexp,"BBD"},
{70,(char (*)())lequal,"KVV"},
{56,(char (*)())lfree,"FV"},
{71,(char (*)())lhopsum,"BJJJBBQ"},
{68,(char (*)())lintersect,"VVV"},
{72,(char (*)())llcat,"V0"},
{73,(char (*)())lmap,"VKO"},
{68,(char (*)())lminus,"VVV"},
{74,(char (*)())lnew,"VDD"},
{17,(char (*)())log,"BB"},
{17,(char (*)())log10,"BB"},
{75,(char (*)())lrand,"ZJJJJ"},
{76,(char (*)())lrev,"VV"},
{77,(char (*)())lsmap,"ZZ"},
{78,(char (*)())lsum,"BV"},
{68,(char (*)())lunion,"VVV"},
{79,(char (*)())malloc,"VL"},
{19,(char (*)())memccpy,"VVVDL"},
{80,(char (*)())memchr,"VVDL"},
{20,(char (*)())memcmp,"DVVL"},
{81,(char (*)())memcpy,"VVVL"},
{20,(char (*)())memicmp,"DVVL"},
{81,(char (*)())memmove,"VVVL"},
{80,(char (*)())memset,"VVDL"},
{82,(char (*)())modf,"BBR"},
{83,(char (*)())nsl_adjust,"QXQ"},
{84,(char (*)())nsl_cycle,"DXE"},
{85,(char (*)())nsl_driver,"DXD0"},
{45,(char (*)())nsl_free,"FX"},
{86,(char (*)())nsl_get,"XQ"},
{86,(char (*)())nsl_get_p,"XQ"},
{87,(char (*)())nsl_get_user_args,"VX"},
{88,(char (*)())nsl_handle,"EXQ"},
{89,(char (*)())nsl_install,"DXQ"},
{90,(char (*)())nsl_lst_hilo,"BVK"},
{91,(char (*)())nsl_lst_ma,"VVBKK"},
{92,(char (*)())nsl_lst_more,"VVDDD"},
{93,(char (*)())nsl_lst_ndd,"BVJ"},
{94,(char (*)())nsl_lst_norm,"VVK"},
{95,(char (*)())nsl_lst_nth,"RVJ"},
{96,(char (*)())nsl_lst_num,"JV"},
{97,(char (*)())nsl_lst_pos,"JVB"},
{98,(char (*)())nsl_lst_val,"BfJ"},
{99,(char (*)())nsl_lst_vma,"VVVKK"},
{100,(char (*)())nsl_mon_evaluate,"H"},
{101,(char (*)())nsl_mon_wnds,"FF"},
{32,(char (*)())nsl_monitor,"DQ"},
{102,(char (*)())nsl_order,"DQO"},
{89,(char (*)())nsl_put,"DXQ"},
{103,(char (*)())nsl_read,"BXEQ"},
{104,(char (*)())nsl_read_p,"QXEQ"},
{105,(char (*)())nsl_set_user_args,"FXV"},
{32,(char (*)())nsl_uninstall,"DQ"},
{106,(char (*)())nsl_write,"DXEQB"},
{107,(char (*)())nsl_write_p,"DXEQQ"},
{108,(char (*)())perror,"FQ"},
{15,(char (*)())pow,"BBB"},
{109,(char (*)())printf,"DQ0"},
{110,(char (*)())printfs,"QQ0"},
{7,(char (*)())putc,"DDX"},
{0,(char (*)())putchar,"DD"},
{32,(char (*)())puts,"DQ"},
{7,(char (*)())putw,"DDX"},
{53,(char (*)())putwc,"NNX"},
{8,(char (*)())putwchar,"NN"},
{2,(char (*)())realloc,"VVL"},
{32,(char (*)())remove,"DQ"},
{26,(char (*)())rename,"DQQ"},
{45,(char (*)())rewind,"FX"},
{3,(char (*)())rmtmp,"DF"},
{0,(char (*)())rnd,"DD"},
{111,(char (*)())rndf,"CF"},
{109,(char (*)())scanf,"DQ0"},
{112,(char (*)())setbuf,"FXQ"},
{113,(char (*)())setvbuf,"DXQDL"},
{17,(char (*)())sin,"BB"},
{17,(char (*)())sinh,"BB"},
{114,(char (*)())sprintf,"DQQ0"},
{17,(char (*)())sqrt,"BB"},
{115,(char (*)())srnd,"FD"},
{114,(char (*)())sscanf,"DQQ0"},
{116,(char (*)())stodx,"JJJ"},
{116,(char (*)())stody,"JJJ"},
{31,(char (*)())strcat,"QQQ"},
{30,(char (*)())strchr,"QQD"},
{26,(char (*)())strcmp,"DQQ"},
{26,(char (*)())strcmpi,"DQQ"},
{26,(char (*)())strcoll,"DQQ"},
{31,(char (*)())strcpy,"QQQ"},
{117,(char (*)())strcspn,"LQQ"},
{27,(char (*)())strdup,"QQ"},
{118,(char (*)())strerror,"QD"},
{26,(char (*)())stricmp,"DQQ"},
{119,(char (*)())strlen,"LQ"},
{27,(char (*)())strlwr,"QQ"},
{120,(char (*)())strncat,"QQQL"},
{28,(char (*)())strncmp,"DQQL"},
{120,(char (*)())strncpy,"QQQL"},
{28,(char (*)())strnicmp,"DQQL"},
{29,(char (*)())strnset,"QQDL"},
{31,(char (*)())strpbrk,"QQQ"},
{30,(char (*)())strrchr,"QQD"},
{27,(char (*)())strrev,"QQ"},
{30,(char (*)())strset,"QQD"},
{117,(char (*)())strspn,"LQQ"},
{31,(char (*)())strstr,"QQQ"},
{31,(char (*)())strtok,"QQQ"},
{27,(char (*)())strupr,"QQ"},
{121,(char (*)())strxfrm,"LQQL"},
{122,(char (*)())swprintf,"D^^0"},
{122,(char (*)())swscanf,"D^^0"},
{17,(char (*)())tan,"BB"},
{17,(char (*)())tanh,"BB"},
{31,(char (*)())tempnam,"QQQ"},
{123,(char (*)())tmpfile,"XF"},
{27,(char (*)())tmpnam,"QQ"},
{0,(char (*)())tolower,"DD"},
{0,(char (*)())toupper,"DD"},
{8,(char (*)())towlower,"NN"},
{8,(char (*)())towupper,"NN"},
{7,(char (*)())ungetc,"DDX"},
{53,(char (*)())ungetwc,"NNX"},
{32,(char (*)())unlink,"DQ"},
{124,(char (*)())vec_liprod,"CVVDDDXXDDBB"},
{125,(char (*)())vec_norm,"CVDDXD"},
{126,(char (*)())vec_prod,"CVVDDDXXDD"},
{125,(char (*)())vec_sum,"CVDDXD"},
{51,(char (*)())vfprintf,"DXQ0"},
{61,(char (*)())vfwprintf,"DX^0"},
{109,(char (*)())vprintf,"DQ0"},
{114,(char (*)())vsprintf,"DQQ0"},
{122,(char (*)())vswprintf,"D^^0"},
{127,(char (*)())vwprintf,"D^0"},
{0,(char (*)())w_active,"DD"},
{128,(char (*)())w_arc,"DDDDDDDDDD"},
{129,(char (*)())w_blank,"FDD"},
{16,(char (*)())w_bordercolor,"DDD"},
{16,(char (*)())w_circle,"DDD"},
{115,(char (*)())w_clear,"FD"},
{115,(char (*)())w_clear_line,"FD"},
{115,(char (*)())w_close,"FD"},
{130,(char (*)())w_dump,"FDD0"},
{131,(char (*)())w_ellipse,"DDDD"},
{115,(char (*)())w_exit,"FD"},
{0,(char (*)())w_fill,"DD"},
{16,(char (*)())w_fillstate,"DDD"},
{115,(char (*)())w_focus,"FD"},
{132,(char (*)())w_fpcheck,"F"},
{0,(char (*)())w_getch,"DD"},
{131,(char (*)())w_getpixel,"DDDD"},
{133,(char (*)())w_gets,"QDQ"},
{32,(char (*)())w_handle,"DQ"},
{132,(char (*)())w_init,"F"},
{16,(char (*)())w_more,"DDD"},
{16,(char (*)())w_move,"DDD"},
{131,(char (*)())w_moveto,"DDDD"},
{0,(char (*)())w_onscreen,"DD"},
{134,(char (*)())w_open,"DQDDDDDDDDD"},
{135,(char (*)())w_origin,"EDDD"},
{16,(char (*)())w_papercolor,"DDD"},
{16,(char (*)())w_penchar,"DDD"},
{16,(char (*)())w_pencolor,"DDD"},
{16,(char (*)())w_pendown,"DDD"},
{16,(char (*)())w_penwidth,"DDD"},
{131,(char (*)())w_poly,"DDDD"},
{135,(char (*)())w_position,"EDDD"},
{136,(char (*)())w_printf,"DDQ0"},
{137,(char (*)())w_puts,"FDQ"},
{138,(char (*)())w_rectangle,"DDDDDD"},
{129,(char (*)())w_refresh,"FDD"},
{115,(char (*)())w_report,"FD"},
{135,(char (*)())w_resize,"EDDD"},
{139,(char (*)())w_setjmp,"D"},
{131,(char (*)())w_setpixel,"DDDD"},
{140,(char (*)())w_size,"EDD"},
{141,(char (*)())w_title,"QDDD0"},
{142,(char (*)())w_track,"DDD0"},
{16,(char (*)())w_turn,"DDD"},
{16,(char (*)())w_turnto,"DDD"},
{115,(char (*)())w_update,"FD"},
{143,(char (*)())w_x,"DE"},
{140,(char (*)())w_xy,"EDD"},
{143,(char (*)())w_y,"DE"},
{42,(char (*)())wcscat,"^^^"},
{36,(char (*)())wcschr,"^^N"},
{33,(char (*)())wcscmp,"D^^"},
{33,(char (*)())wcscoll,"D^^"},
{42,(char (*)())wcscpy,"^^^"},
{144,(char (*)())wcscspn,"L^^"},
{11,(char (*)())wcsdup,"^^"},
{33,(char (*)())wcsicmp,"D^^"},
{33,(char (*)())wcsicoll,"D^^"},
{145,(char (*)())wcslen,"L^"},
{11,(char (*)())wcslwr,"^^"},
{146,(char (*)())wcsncat,"^^^L"},
{34,(char (*)())wcsncmp,"D^^L"},
{146,(char (*)())wcsncpy,"^^^L"},
{34,(char (*)())wcsnicmp,"D^^L"},
{35,(char (*)())wcsnset,"^^NL"},
{42,(char (*)())wcspbrk,"^^^"},
{36,(char (*)())wcsrchr,"^^N"},
{11,(char (*)())wcsrev,"^^"},
{36,(char (*)())wcsset,"^^N"},
{144,(char (*)())wcsspn,"L^^"},
{42,(char (*)())wcsstr,"^^^"},
{42,(char (*)())wcstok,"^^^"},
{11,(char (*)())wcsupr,"^^"},
{147,(char (*)())wcsxfrm,"L^^L"},
{127,(char (*)())wprintf,"D^0"},
{127,(char (*)())wscanf,"D^0"},
{17,(char (*)())y0,"BB"},
{17,(char (*)())y1,"BB"},
{18,(char (*)())yn,"BDB"},
{0,(char (*)())0L,(char *)0L}};
int nsl_num_mon_list=391;
NSLNSLfval nsl_mon_evaluate(cp,fp,sw)char *cp;NSLNSLfuni fp;int sw;
{NSLNSLfval rv;switch(sw){
case 0:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((int *)(cp+0))); break;
case 1:
rv.typ=2;rv.val.dd=(*fp.dd)(
*((int *)(cp+0))); break;
case 2:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((unsigned int *)(cp+4))); break;
case 3:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((int *)(cp+0))); break;
case 4:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((int *)(cp+0)),*((char **)(cp+4))); break;
case 5:
rv.typ=14;rv.val.uss=(*fp.uss)(
*((int *)(cp+0))); break;
case 6:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0))); break;
case 7:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((int *)(cp+0)),*((char **)(cp+4))); break;
case 8:
rv.typ=14;rv.val.uss=(*fp.uss)(
*((unsigned short *)(cp+0))); break;
case 9:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((char **)(cp+4)),*((int *)(cp+8))
); break;
case 10:
rv.typ=12;rv.val.uii=(*fp.uii)(
*((int *)(cp+0))); break;
case 11:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0))); break;
case 12:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((unsigned int *)(cp+4))); break;
case 13:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((unsigned int *)(cp+0))); break;
case 14:
rv.typ=12;rv.val.uii=(*fp.uii)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 15:
rv.typ=2;rv.val.dd=(*fp.dd)(
*((double *)(cp+0)),*((double *)(cp+8))); break;
case 16:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((int *)(cp+0)),*((int *)(cp+4))); break;
case 17:
rv.typ=2;rv.val.dd=(*fp.dd)(
*((double *)(cp+0))); break;
case 18:
rv.typ=2;rv.val.dd=(*fp.dd)(
*((int *)(cp+0)),*((double *)(cp+4))); break;
case 19:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((char **)(cp+4)),*((int *)(cp+8)),
*((unsigned int *)(cp+12))); break;
case 20:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((char **)(cp+4)),*((unsigned int *)(cp+8))
); break;
case 21:
rv.typ=12;rv.val.uii=(*fp.uii)(
*((char **)(cp+0))); break;
case 22:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 23:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0))); break;
case 24:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((unsigned int *)(cp+4)),*((char **)(cp+8)),
((char *)(cp+12))); break;
case 25:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((unsigned int *)(cp+4)),*((char **)(cp+8)),
((char *)(cp+12))); break;
case 26:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 27:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0))); break;
case 28:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((char **)(cp+4)),*((unsigned int *)(cp+8))
); break;
case 29:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((int *)(cp+4)),*((unsigned int *)(cp+8))
); break;
case 30:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((int *)(cp+4))); break;
case 31:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 32:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0))); break;
case 33:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 34:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((char **)(cp+4)),*((unsigned int *)(cp+8))
); break;
case 35:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((unsigned short *)(cp+4)),*((unsigned int *)(cp+6))
); break;
case 36:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((unsigned short *)(cp+4))); break;
case 37:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((int *)(cp+0)),*((char **)(cp+4))); break;
case 38:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 39:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((char **)(cp+4)),*((char **)(cp+8))
); break;
case 40:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((char **)(cp+4)),*((int *)(cp+8))
); break;
case 41:
rv.typ=4;rv.val.ii=0;(*fp.vv)(
*((char **)(cp+0))); break;
case 42:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 43:
rv.typ=2;rv.val.dd=(*fp.dd)(
*((char **)(cp+0))); break;
case 44:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((unsigned int *)(cp+0)),*((unsigned int *)(cp+4))); break;
case 45:
rv.typ=4;rv.val.ii=0;(*fp.vv)(
*((char **)(cp+0))); break;
case 46:
rv.typ=10;rv.val.ss=(*fp.ss)(
*((short *)(cp+0)),*((short *)(cp+2)),*((short *)(cp+4))
); break;
case 47:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 48:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((int *)(cp+4)),*((char **)(cp+8))
); break;
case 49:
rv.typ=14;rv.val.uss=(*fp.uss)(
*((char **)(cp+0))); break;
case 50:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((int *)(cp+4)),*((char **)(cp+8))
); break;
case 51:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((char **)(cp+4)),((char *)(cp+8))
); break;
case 52:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 53:
rv.typ=14;rv.val.uss=(*fp.uss)(
*((unsigned short *)(cp+0)),*((char **)(cp+2))); break;
case 54:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 55:
rv.typ=12;rv.val.uii=(*fp.uii)(
*((char **)(cp+0)),*((unsigned int *)(cp+4)),*((unsigned int *)(cp+8)),
*((char **)(cp+12))); break;
case 56:
rv.typ=4;rv.val.ii=0;(*fp.vv)(
*((char **)(cp+0))); break;
case 57:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((char **)(cp+4)),*((char **)(cp+8))
); break;
case 58:
rv.typ=2;rv.val.dd=(*fp.dd)(
*((double *)(cp+0)),*((char **)(cp+8))); break;
case 59:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((long *)(cp+4)),*((int *)(cp+8))
); break;
case 60:
rv.typ=5;rv.val.ll=(*fp.ll)(
*((char **)(cp+0))); break;
case 61:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((char **)(cp+4)),((char *)(cp+8))
); break;
case 62:
rv.typ=3;rv.val.ff=(*fp.ff)(
*((double *)(cp+0)),*((double *)(cp+8)),*((double *)(cp+16))
); break;
case 63:
rv.typ=3;rv.val.ff=(*fp.ff)(
*((double *)(cp+0)),*((double *)(cp+8)),*((double *)(cp+16)),
*((double *)(cp+24))); break;
case 64:
rv.typ=3;rv.val.ff=(*fp.ff)(
*((double *)(cp+0)),*((double *)(cp+8)),*((double *)(cp+16)),
*((double *)(cp+24)),*((double *)(cp+32))); break;
case 65:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((unsigned short *)(cp+0)),*((unsigned short *)(cp+2))); break;
case 66:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((unsigned short *)(cp+0))); break;
case 67:
rv.typ=5;rv.val.ll=(*fp.ll)(
*((long *)(cp+0))); break;
case 68:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 69:
rv.typ=2;rv.val.dd=(*fp.dd)(
*((double *)(cp+0)),*((int *)(cp+8))); break;
case 70:
rv.typ=11;rv.val.ucc=(*fp.ucc)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 71:
rv.typ=2;rv.val.dd=(*fp.dd)(
*((short *)(cp+0)),*((short *)(cp+2)),*((short *)(cp+4)),
*((double *)(cp+6)),*((double *)(cp+14)),*((char **)(cp+22))
); break;
case 72:
rv.typ=15;rv.val.pp=(*fp.pp)(
((char *)(cp+0))); break;
case 73:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((unsigned char *)(cp+0)),*((char **)(cp+1))); break;
case 74:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((int *)(cp+0)),*((int *)(cp+4))); break;
case 75:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((short *)(cp+0)),*((short *)(cp+2)),*((short *)(cp+4)),
*((short *)(cp+6))); break;
case 76:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0))); break;
case 77:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0))); break;
case 78:
rv.typ=2;rv.val.dd=(*fp.dd)(
*((char **)(cp+0))); break;
case 79:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((unsigned int *)(cp+0))); break;
case 80:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((int *)(cp+4)),*((unsigned int *)(cp+8))
); break;
case 81:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((char **)(cp+4)),*((unsigned int *)(cp+8))
); break;
case 82:
rv.typ=2;rv.val.dd=(*fp.dd)(
*((double *)(cp+0)),*((char **)(cp+8))); break;
case 83:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 84:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((long *)(cp+4))); break;
case 85:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((int *)(cp+4)),((char *)(cp+8))
); break;
case 86:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0))); break;
case 87:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0))); break;
case 88:
rv.typ=5;rv.val.ll=(*fp.ll)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 89:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 90:
rv.typ=2;rv.val.dd=(*fp.dd)(
*((char **)(cp+0)),*((unsigned char *)(cp+4))); break;
case 91:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((double *)(cp+4)),*((unsigned char *)(cp+12)),
*((unsigned char *)(cp+13))); break;
case 92:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((int *)(cp+4)),*((int *)(cp+8)),
*((int *)(cp+12))); break;
case 93:
rv.typ=2;rv.val.dd=(*fp.dd)(
*((char **)(cp+0)),*((short *)(cp+4))); break;
case 94:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((unsigned char *)(cp+4))); break;
case 95:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((short *)(cp+4))); break;
case 96:
rv.typ=10;rv.val.ss=(*fp.ss)(
*((char **)(cp+0))); break;
case 97:
rv.typ=10;rv.val.ss=(*fp.ss)(
*((char **)(cp+0)),*((double *)(cp+4))); break;
case 98:
rv.typ=2;rv.val.dd=(*fp.dd)(
*((char **)(cp+0)),*((short *)(cp+4))); break;
case 99:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((char **)(cp+4)),*((unsigned char *)(cp+8)),
*((unsigned char *)(cp+9))); break;
case 100:
rv.typ=4;rv.val.ii=(*fp.ii)(
); break;
case 101:
rv.typ=4;rv.val.ii=0;(*fp.vv)(
*((int *)(cp+0))); break;
case 102:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 103:
rv.typ=2;rv.val.dd=(*fp.dd)(
*((char **)(cp+0)),*((long *)(cp+4)),*((char **)(cp+8))
); break;
case 104:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((long *)(cp+4)),*((char **)(cp+8))
); break;
case 105:
rv.typ=4;rv.val.ii=0;(*fp.vv)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 106:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((long *)(cp+4)),*((char **)(cp+8)),
*((double *)(cp+12))); break;
case 107:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((long *)(cp+4)),*((char **)(cp+8)),
*((char **)(cp+12))); break;
case 108:
rv.typ=4;rv.val.ii=0;(*fp.vv)(
*((char **)(cp+0))); break;
case 109:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),((char *)(cp+4))); break;
case 110:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),((char *)(cp+4))); break;
case 111:
rv.typ=3;rv.val.ff=(*fp.ff)(
*((int *)(cp+0))); break;
case 112:
rv.typ=4;rv.val.ii=0;(*fp.vv)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 113:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((char **)(cp+4)),*((int *)(cp+8)),
*((unsigned int *)(cp+12))); break;
case 114:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((char **)(cp+4)),((char *)(cp+8))
); break;
case 115:
rv.typ=4;rv.val.ii=0;(*fp.vv)(
*((int *)(cp+0))); break;
case 116:
rv.typ=10;rv.val.ss=(*fp.ss)(
*((short *)(cp+0)),*((short *)(cp+2))); break;
case 117:
rv.typ=12;rv.val.uii=(*fp.uii)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 118:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((int *)(cp+0))); break;
case 119:
rv.typ=12;rv.val.uii=(*fp.uii)(
*((char **)(cp+0))); break;
case 120:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((char **)(cp+4)),*((unsigned int *)(cp+8))
); break;
case 121:
rv.typ=12;rv.val.uii=(*fp.uii)(
*((char **)(cp+0)),*((char **)(cp+4)),*((unsigned int *)(cp+8))
); break;
case 122:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((char **)(cp+4)),((char *)(cp+8))
); break;
case 123:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((int *)(cp+0))); break;
case 124:
rv.typ=3;rv.val.ff=(*fp.ff)(
*((char **)(cp+0)),*((char **)(cp+4)),*((int *)(cp+8)),
*((int *)(cp+12)),*((int *)(cp+16)),*((char **)(cp+20)),
*((char **)(cp+24)),*((int *)(cp+28)),*((int *)(cp+32)),
*((double *)(cp+36)),*((double *)(cp+44))); break;
case 125:
rv.typ=3;rv.val.ff=(*fp.ff)(
*((char **)(cp+0)),*((int *)(cp+4)),*((int *)(cp+8)),
*((char **)(cp+12)),*((int *)(cp+16))); break;
case 126:
rv.typ=3;rv.val.ff=(*fp.ff)(
*((char **)(cp+0)),*((char **)(cp+4)),*((int *)(cp+8)),
*((int *)(cp+12)),*((int *)(cp+16)),*((char **)(cp+20)),
*((char **)(cp+24)),*((int *)(cp+28)),*((int *)(cp+32))
); break;
case 127:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),((char *)(cp+4))); break;
case 128:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((int *)(cp+0)),*((int *)(cp+4)),*((int *)(cp+8)),
*((int *)(cp+12)),*((int *)(cp+16)),*((int *)(cp+20)),
*((int *)(cp+24)),*((int *)(cp+28)),*((int *)(cp+32))
); break;
case 129:
rv.typ=4;rv.val.ii=0;(*fp.vv)(
*((int *)(cp+0)),*((int *)(cp+4))); break;
case 130:
rv.typ=4;rv.val.ii=0;(*fp.vv)(
*((int *)(cp+0)),*((int *)(cp+4)),((char *)(cp+8))
); break;
case 131:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((int *)(cp+0)),*((int *)(cp+4)),*((int *)(cp+8))
); break;
case 132:
rv.typ=4;rv.val.ii=0;(*fp.vv)(
); break;
case 133:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((int *)(cp+0)),*((char **)(cp+4))); break;
case 134:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((int *)(cp+4)),*((int *)(cp+8)),
*((int *)(cp+12)),*((int *)(cp+16)),*((int *)(cp+20)),
*((int *)(cp+24)),*((int *)(cp+28)),*((int *)(cp+32)),
*((int *)(cp+36))); break;
case 135:
rv.typ=5;rv.val.ll=(*fp.ll)(
*((int *)(cp+0)),*((int *)(cp+4)),*((int *)(cp+8))
); break;
case 136:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((int *)(cp+0)),*((char **)(cp+4)),((char *)(cp+8))
); break;
case 137:
rv.typ=4;rv.val.ii=0;(*fp.vv)(
*((int *)(cp+0)),*((char **)(cp+4))); break;
case 138:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((int *)(cp+0)),*((int *)(cp+4)),*((int *)(cp+8)),
*((int *)(cp+12)),*((int *)(cp+16))); break;
case 139:
rv.typ=4;rv.val.ii=(*fp.ii)(
); break;
case 140:
rv.typ=5;rv.val.ll=(*fp.ll)(
*((int *)(cp+0)),*((int *)(cp+4))); break;
case 141:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((int *)(cp+0)),*((int *)(cp+4)),*((int *)(cp+8)),
((char *)(cp+12))); break;
case 142:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((int *)(cp+0)),*((int *)(cp+4)),((char *)(cp+8))
); break;
case 143:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((long *)(cp+0))); break;
case 144:
rv.typ=12;rv.val.uii=(*fp.uii)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 145:
rv.typ=12;rv.val.uii=(*fp.uii)(
*((char **)(cp+0))); break;
case 146:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((char **)(cp+4)),*((unsigned int *)(cp+8))
); break;
case 147:
rv.typ=12;rv.val.uii=(*fp.uii)(
*((char **)(cp+0)),*((char **)(cp+4)),*((unsigned int *)(cp+8))
); break;
default:rv.typ=4;rv.val.ii=0;}
return(rv);}
