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
long nsl_aseed=123,nsl_bseed=7131,nsl_cseed=30013;
void srnd(seed) int seed;
{ nsl_aseed=171*(seed%177); nsl_bseed=172*(seed%176); nsl_cseed=170*(seed%178);
}
float rndf()
{float temp; double t1;
nsl_aseed=171*(nsl_aseed % 177)-2*(nsl_aseed/177);
if(nsl_aseed<0) nsl_aseed+=30269;
nsl_bseed=172*(nsl_bseed % 176)-35*(nsl_bseed/176);
if(nsl_bseed<0) nsl_bseed+=30307;
nsl_cseed=170*(nsl_cseed % 178)-63*(nsl_cseed/178);
if(nsl_cseed<0) nsl_cseed+=30323;
temp=(float)((float)nsl_aseed/(float)30269.0
+(float)nsl_bseed/(float)30307.0+(float)nsl_cseed/(float)30323.0);
return((float)modf((double)temp,&t1));
}
int rnd(randmax)
int randmax;
{ if(randmax==0) return(0);
return((int)(1+randmax*rndf()));
}
extern char *mem_rel(),*mem_mal();
extern void mem_free();
void lfree(a) void *a;
{ if(a)
{if(!((((mile *)(a))[-5]) ==0x5679 ) ) fprintf((&_iob[2]) ,"Bad list\n");
else
{(((mile *)(a))[-5]) =0;
mem_free(((char *)(a)-(5 *sizeof(mile)) ) );
}
}
}
void *nsl_lst_more(mlp,need,lstype,elm_size)
void *mlp; int need,lstype,elm_size;
{ int max; char *lp=(char *)mlp;
if(!lp)
{max=8 +need;
lp=(char *)mem_mal((5 *sizeof(mile)) +elm_size*max)+(5 *sizeof(mile)) ;
(((mile *)(lp))[-2]) =(mile)max; (((mile *)(lp))[-3]) =(mile)elm_size;
(((mile *)(lp))[-1]) =(mile)need; (((mile *)(lp))[-4]) =(mile)lstype;
(((mile *)(lp))[-5]) =0x5679 ;
}
else
{max=(int)(((mile *)(lp))[-2]) ;
if(((((mile *)(lp))[-1]) +need+2)>=max)
{
int mm=(5 *sizeof(mile)) +(((mile *)(lp))[-3]) *max;
max+=need+8 ;
lp=(char *)mem_rel(((char *)(lp)-(5 *sizeof(mile)) ) ,(5 *sizeof(mile)) +(((mile *)(lp))[-3]) *max,mm)+(5 *sizeof(mile)) ;
(((mile *)(lp))[-2]) =(mile)max;
}
(((mile *)(lp))[-1]) +=(mile)need;
}
return((void *)lp);
}
void *lnew(num,lstype) int num,lstype;
{ size_t siz;
if(lstype>0) switch(lstype&0x1f )
{case 1 : case 11 : siz=sizeof(char); break;
case 2 : siz=sizeof(double); break;
case 3 : siz=sizeof(float); break;
case 4 : case 12 : siz=sizeof(int); break;
case 5 : case 13 : siz=sizeof(long); break;
case 10 : case 14 : siz=sizeof(short); break;
case 15 : siz=sizeof(char *); break;
default: return((void *)0L);
}
else {siz=(-lstype); lstype=0 ;}
return(nsl_lst_more((char *)0L,(int)num,(int)lstype,(int)siz));
}
boolean lequal(ma,mb)
void *ma,*mb;
{ int num; char *a=(char *)ma,*b=(char *)mb;
if(!a&&!b) return(1 ); if(!a||!b) return(0 );
num=(((mile *)(a))[-1]) ;
if(num!=(((mile *)(b))[-1]) ||(((mile *)(a))[-4]) !=(((mile *)(b))[-4]) ||(((mile *)(a))[-3]) !=(((mile *)(b))[-3]) )
return(0 );
num*=(((mile *)(a))[-3]) ; a=((char *)(a)) ; b=((char *)(b)) ;
while(num--) {if(*a!= *b) return(0 ); ++a; ++b;}
return(1 );
}
void *lcopy(ma,mb) void *ma,*mb;
{ mile num,siz,*c=(mile *)0L; char *a=(char *)ma,*b=(char *)mb;
if(ma)
{siz=(((mile *)(ma))[-3]) ; num=(((mile *)(ma))[-1]) ;
c=nsl_lst_more(mb,(int)num,(int)(((mile *)(ma))[-4]) ,(int)siz);
num*=siz; a=((char *)(ma)) ; b=((char *)(c)) ;
while(num--) *b++ = *a++;
}
return((void *)c);
}
void *lrev(lp)
void *lp;
{ char chr,*a=(char *)lp,*b; mile num,siz,val;
if(a)
{num=((((mile *)(a))[-1]) +1)/2; siz=(((mile *)(a))[-3]) ;
b=((char *)(a)+(((mile *)(a))[-1]) *(((mile *)(a))[-3]) ) ; a=((char *)(a)) ;
while(num--)
{b-=siz; val=siz;
while(val--) {chr=(*a); *a++ =(*b); *b++ =chr;}
a+=siz;
}
}
return(lp);
}
void nsl_lst_put(lp,val,typ)
void *lp; double val; mile typ;
{ switch(typ&0x1f )
{case 15 : *((char **)lp)=(char *)((long)val); break;
case 3 : *((float *)lp)=(float)val; break;
case 2 : *((double *)lp)=val; break;
case 5 : case 13 : *((long *)lp)=(long)val; break;
case 4 : case 12 :  *((int *)lp)=(int)val; break;
case 1 : case 11 : *((char *)lp)=(char)val; break;
default: *((mile *)lp)=(mile)val;
}
}
double nsl_lst_val(mlpt,lstype)
void **mlpt; mile lstype;
{ char *apt=(char *)*mlpt; double val;
switch(lstype&0x1f )
{case 15 : break;
val=(double)((long)(*((char **)apt)));
apt+=sizeof(char *); break;
case 3 :
val=(double)(*((float *)apt)); apt+=sizeof(float); break;
case 2 :
val=(*((double *)apt)); apt+=sizeof(double); break;
case 5 : case 13 :
val=(double)(*((long *)apt)); apt+=sizeof(long); break;
case 4 : case 12 :
val=(double)(*((int *)apt)); apt+=sizeof(int); break;
case 1 : case 11 :
val=(double)(*apt); apt+=sizeof(char); break;
default:
val=(double)(*((mile *)apt)); apt+=sizeof(mile);
}
*mlpt=(void *)apt;
return(val);
}
double nsl_lst_hilo(lp,islar)
void *lp; boolean islar;
{ mile num,lstype; double val,lar;
if(islar) lar=(double)(-1.0); else lar=(double)0.0; val=lar;
if(lp)
{num=(((mile *)(lp))[-1]) ; lstype=(((mile *)(lp))[-4]) ;
lp=((void *)(lp)) ;
while(num--)
{val=nsl_lst_val((void **)&lp,lstype);
if(islar) {if(val>lar) lar=val;}
else if(val<lar) lar=val;
}
}
return(lar);
}
mile nsl_lst_num(lp) void *lp;
{ mile num,lstype,pip=0; double val; char *apt=(char *)lp;
if(apt)
{if((num=(((mile *)(apt))[-1]) ))
{lstype=(((mile *)(apt))[-4]) ;
if(lstype&0x40 )
{apt=((char *)(apt)) ;
while(num>0)
{++pip;
val=nsl_lst_val((void **)&apt,lstype);
val-=nsl_lst_val((void **)&apt,lstype); num-=2;
if(val<(double)0.0) val=(-val);
pip+=(mile)val;}
if(pip==0) return(1);
}
else return(num);
}
}
return(pip);
}
double *nsl_lst_nth(lp,nth) void *lp; mile nth;
{ mile lstype,num_elms,asize,aelm; double bl; static double al;
char *ary=(char *)lp;
if(!ary) return((double *)0L);
if(nth>=0)
{asize=(((mile *)(ary))[-1]) ; lstype=(((mile *)(ary))[-4]) ;
if(lstype&0x40 )
{ary=((char *)(ary)) ; num_elms=0;
while(asize)
{al=nsl_lst_val((void **)&ary,lstype);
bl=nsl_lst_val((void **)&ary,lstype);
asize-=2; aelm=num_elms; ++num_elms; bl=al-bl;
if(bl<(double)0.0) num_elms-=(mile)bl; else num_elms+=(mile)bl;
if(nth<num_elms)
{nth-=aelm;
if(bl<(double)0.0) al+=(double)nth; else al-=(double)nth;
return((double *)&al);
}
}
}
else
{if(nth>=asize) return((double *)0L);
ary=(((char *)(ary)) +nth*(((mile *)(ary))[-3]) );
al=nsl_lst_val((void **)&ary,lstype);
return((double *)&al);
}
}
return((double *)0L);
}
double nsl_lst_ndd(a,nth) void *a; mile nth;
{ double *dp=nsl_lst_nth(a,nth);
if(dp) return((double)*dp);
return((double)0.0);
}
mile nsl_lst_pos(a,val) void *a; double val;
{ mile xx,nn; double *dp;
if(!a) return(-1);
nn=nsl_lst_num(a);
for(xx=0;xx<nn;++xx)
{if(!(dp=nsl_lst_nth(a,xx))) break;
if(*dp==val) return(xx);
}
return(-1);
}
void *lmi_in(a,b,is_mi) void *a,*b; boolean is_mi;
{ void *d; mile lena,lenb,aa,bb,typ; double xx,val; boolean is_ok;
if(!a) return(b); if(!b) return(a);
lena=nsl_lst_num(a); lenb=nsl_lst_num(b);
typ=(((mile *)(a))[-4]) ; if(typ&0x40 ) typ^=0x40 ;
aa=lena; if(!is_mi) aa+=lenb;
d=lnew((int)aa,(int)typ); (((mile *)(d))[-1]) =0;
for(aa=0;aa<lena;++aa)
{xx=nsl_lst_ndd(a,aa); is_ok=is_mi;
for(bb=0;bb<lenb;++bb)
{val=nsl_lst_ndd(b,bb);
if(val==xx) {is_ok=(boolean)!is_mi; if(is_mi) break;}
}
if(is_ok) {nsl_lst_put(((char *)(d)+(((mile *)(d))[-1]) *(((mile *)(d))[-3]) ) ,xx,typ); ++(((mile *)(d))[-1]) ;}
}
return(d);
}
void *lminus(a,b) void *a,*b;
{ return(lmi_in(a,b,1 ));
}
void *lintersect(a,b) void *a,*b;
{ return(lmi_in(a,b,0 ));
}
void *lcatuni(a,b,iscat) void *a,*b; boolean iscat;
{ mile lena,lenb,aa,typ,*d; double val;
if(!a) return(lcopy(b,(mile *)0L)); if(!b) return(lcopy(a,(mile *)0L));
lena=nsl_lst_num(a); lenb=nsl_lst_num(b);
typ=(((mile *)(a))[-4]) ; if(typ&0x40 ) typ^=0x40 ;
d=(mile *)lnew((int)(lena+lenb),(int)typ); (((mile *)(d))[-1]) =0;
for(aa=0;aa<lena;++aa)
{val=nsl_lst_ndd(a,aa);
nsl_lst_put(((char *)(d)+(((mile *)(d))[-1]) *(((mile *)(d))[-3]) ) ,val,typ); ++(((mile *)(d))[-1]) ;
}
for(aa=0;aa<lenb;++aa)
{val=nsl_lst_ndd(b,aa);
if(iscat||(nsl_lst_pos(a,val)<0))
{nsl_lst_put(((char *)(d)+(((mile *)(d))[-1]) *(((mile *)(d))[-3]) ) ,val,typ); ++(((mile *)(d))[-1]) ;}
}
return((void *)d);
}
void *lunion(a,b) void *a,*b;
{ return(lcatuni(a,b,0 ));
}
void *lcat(a,b) void *a,*b;
{ return(lcatuni(a,b,1 ));
}
void *llcat(args) va_list args;
{ mile typ,jj,kk,xx,yy=0; double val; char **apt=(char **)args; void *lst;
while(*apt++) ++yy; apt=(char **)args;
if(yy<2) return((void *)*apt);
xx=nsl_lst_num(*apt); typ=(((mile *)(*apt))[-4]) ; if(typ&0x40 ) typ^=0x40 ;
for(kk=1;kk<yy;++kk) {if(xx!=nsl_lst_num(*(apt+kk))) return((void *)0L);}
lst=lnew(xx*yy,(int)typ); (((mile *)(lst))[-1]) =0;
for(jj=0;jj<xx;++jj)
{for(kk=0;kk<yy;++kk)
{val=nsl_lst_ndd(*(apt+kk),jj);
nsl_lst_put(((char *)(lst)+(((mile *)(lst))[-1]) *(((mile *)(lst))[-3]) ) ,val,typ); ++(((mile *)(lst))[-1]) ;
}
}
return(lst);
}
void *lmap(islist,fnpt,args) boolean islist; void *(*fnpt)(); va_list args;
{ mile typ,jj,kk,xx,yy=0,*lp,*lst=(mile *)0L,(*fp)(); double val;
char **apt=(char **)args; void *cst,*nst,*rst=(void *)0L;
while(*apt++) ++yy; apt=(char **)args;
if(yy<1||yy>4) return((void *)*apt);
xx=nsl_lst_num(*apt); typ=(((mile *)(*apt))[-4]) ; if(typ&0x40 ) typ^=0x40 ;
for(kk=0;kk<yy;++kk)
{if((((mile *)(*(apt+kk)))[-3]) !=(mile)sizeof(mile)) return((void *)0L);
jj=nsl_lst_num(*(apt+kk)); if(jj!=xx) return((void *)0L);}
if(!islist) {fp=(mile(*)())fnpt; lst=(mile *)lnew((int)1,(int)typ);}
lp=(mile *)lnew((int)yy,(int)typ);
for(jj=0;jj<xx;++jj)
{for(kk=0;kk<yy;++kk) {val=nsl_lst_ndd(*(apt+kk),jj); lp[kk]=(mile)val;}
cst=rst;
if(islist)
{switch(yy)
{case 4: nst=(*fnpt)(lp[0],lp[1],lp[2],lp[3]); break;
case 3: nst=(*fnpt)(lp[0],lp[1],lp[2]); break;
case 2: nst=(*fnpt)(lp[0],lp[1]); break;
default: nst=(*fnpt)(lp[0]);
}
rst=lcat(cst,nst); lfree(nst);
}
else
{switch(yy)
{case 4: xx=(*fp)(lp[0],lp[1],lp[2],lp[3]); break;
case 3: xx=(*fp)(lp[0],lp[1],lp[2]); break;
case 2: xx=(*fp)(lp[0],lp[1]); break;
default: xx=(*fp)(lp[0]);
}
lst[0]=xx;
rst=lcat(cst,lst);
}
lfree(cst);
}
lfree(lst); lfree(lp);
return(rst);
}
mile *lsmap(a) mile *a;
{ mile *b,lena,j,k,y,n;
if(!a||(((mile *)(a))[-3]) !=sizeof(mile)) return((mile *)0L);
lena=(((mile *)(a))[-1]) ; b=(mile *)lnew((int)lena,(int)(((mile *)(a))[-4]) );
for(j=0;j<lena;++j)
{y=a[j]; n=0;
for(k=0;k<(j-1);++k) {if(a[k]<y) ++n;}
y-=n; if(y>0) b[j]=y; else b[j]=0;
}
return(b);
}
mile *lrand(nn,mm,aa,bb) mile nn,mm,aa,bb;
{ mile *ma,cc;
if(mm<nn) {cc=nn; nn=mm; mm=cc;}
if(nn<1) return((mile *)0L);
nn+=rnd(1+mm-nn)-1;
ma=(mile *)lnew((int)nn,(int)10 );
if(bb<aa) {cc=aa; aa=bb; bb=cc;}
cc=1+bb-aa; while(nn--) ma[nn]=aa+rnd(cc)-1;
return(ma);
}
double lhopsum(ii,jj,miss,gg,cc,pats)
mile ii,jj,miss; float gg,cc; va_list pats;
{ double sum=(double)0.0,wi,wj; mile xx,numpats=0;
void *lst,**apt=(void **)pats;
if(!*apt) return((double)0.0);
xx=nsl_lst_num(*apt); if(miss&&jj>=ii) ++jj;
if(xx<=ii||xx<=jj) return((double)0.0);
while(*apt)
{lst=(*apt);
if(nsl_lst_num(lst)!=xx) return((double)0.0);
wi=nsl_lst_ndd(lst,ii); wj=nsl_lst_ndd(lst,jj);
sum+=(wi*gg+cc)*(wj*gg+cc); ++apt; ++numpats;
}
return(sum/numpats);
}
double lsum(ma) void *ma;
{ double sum=(double)0.0; mile num,len;
if(!ma||(((((mile *)(ma))[-4]) &0x1f )==15 )) return(sum);
len=nsl_lst_num(ma);
for(num=0;num<len;++num) sum+=nsl_lst_ndd(ma,num);
return(sum);
}
void *nsl_lst_ma(a,con,ismul,isnew) void *a; double con; boolean ismul,isnew;
{ double val; void *b,*c; mile num,typ;
if(!a) return((void *)0L);
typ=(((mile *)(a))[-4]) ; if((typ&0x1f )==15 ) return((void *)0L);
if(isnew) a=lcopy(a,(void *)0L);
if(con==(double)0.0) return(a);
num=(((mile *)(a))[-1]) ; b=((void *)(a)) ;
while(num>0)
{c=b; val=nsl_lst_val((void **)&b,typ);
if(ismul) val*=con; else val+=con;
nsl_lst_put(c,val,typ); --num;}
return(a);
}
void *nsl_lst_vma(a,b,ismul,isnew) void *a,*b; boolean ismul,isnew;
{ double val; void *c,*d; mile num,typ,dtyp;
if(!a||!b) return((void *)0L);
typ=(((mile *)(a))[-4]) ; dtyp=(((mile *)(b))[-4]) ;
if((typ&0x1f )==15 ||(dtyp&0x1f )==15 ) return((void *)0L);
num=(((mile *)(a))[-1]) ; if(num!=(((mile *)(b))[-1]) ) return((void *)0L);
if(isnew) a=lcopy(a,(void *)0L); d=((void *)(b)) ; b=((void *)(a)) ;
while(num>0)
{c=b; val=nsl_lst_val((void **)&b,typ);
if(ismul) val*=nsl_lst_val((void **)&d,dtyp);
else val+=nsl_lst_val((void **)&d,dtyp);
nsl_lst_put(c,val,typ); --num;}
return(a);
}
void *nsl_lst_norm(a,isnew) void *a; boolean isnew;
{ double tot; void *b; mile typ;
if(!a) return((void *)0L);
typ=(((mile *)(a))[-4]) ; if((typ&0x1f )==15 ) return((void *)0L);
b=nsl_lst_vma(a,a,(boolean)1,(boolean)1); tot=lsum(b); lfree(b);
if(tot>(double)0) b=nsl_lst_ma(a,(double)(1.0/sqrt(tot)),(boolean)1,isnew);
else return((void *)0L);
return(b);
}
mile stodx(s,dx) mile s,dx;
{return(s%dx);}
mile stody(s,dx) mile s,dx;
{return(s/dx);}
mile dtos(x,y,dx) mile x,y,dx;
{return((y*dx)+x);}
float generic_bound(x,top,bot) float x,top,bot;
{ if(x>top) return((float)top);
if(x<bot) return((float)bot);
return((float)x);
}
float generic_limiter(x,top,bot,mid) float x,top,bot,mid;
{ if(x>mid) return((float)top);
return((float)bot);
}
float generic_sigmoid(x,top,bot,gain,c) float x,top,bot,gain,c;
{ return((float)(bot + (top-bot)/
((float)1.0+(float)exp((double)(-gain * (c+x))))));
}
char nsl_fsb[256];
char *printfs(fmt,args) char *fmt; va_list args;
{ if(vsprintf(nsl_fsb,fmt,args)<1) return((char *)0L);
return(nsl_fsb);
}
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
"atof","calloc","ceil","clearerr",
"cos","cosh","dtos","exp",
"fabs","fclose","feof","ferror",
"fflush","fgetc","fgetpos","fgets",
"fgetwc","fgetws","floor","fmod",
"fopen","fprintf","fputc","fputs",
"fputwc","fputws","fread","free",
"freopen","frexp","fscanf","fseek",
"fsetpos","ftell","fwprintf","fwrite",
"fwscanf","generic_bound","generic_limiter","generic_sigmoid",
"getc","getchar","gets","getwc",
"getwchar","is_wctype","isalnum","isalpha",
"iscntrl","isdigit","isgraph","isleadbyte",
"islower","isprint","ispunct","isspace",
"isupper","iswalnum","iswalpha","iswascii",
"iswcntrl","iswctype","iswdigit","iswgraph",
"iswlower","iswprint","iswpunct","iswspace",
"iswupper","iswxdigit","isxdigit","labs",
"lcat","lcatuni","lcopy","ldexp",
"lequal","lfree","lhopsum","lintersect",
"llcat","lmap","lmi_in","lminus",
"lnew","log","log10","lrand",
"lrev","lsmap","lsum","lunion",
"malloc","mem_free","mem_mal","mem_rel",
"memchr","memcmp","memcpy","memmove",
"memset","modf","nsl_lst_hilo","nsl_lst_ma",
"nsl_lst_more","nsl_lst_ndd","nsl_lst_norm","nsl_lst_nth",
"nsl_lst_num","nsl_lst_pos","nsl_lst_put","nsl_lst_val",
"nsl_lst_vma","perror","pow","printf",
"printfs","putc","putchar","puts",
"putwc","putwchar","realloc","remove",
"rename","rewind","rnd","rndf",
"scanf","setbuf","setvbuf","sin",
"sinh","sprintf","sqrt","srnd",
"sscanf","stodx","stody","strcat",
"strchr","strcmp","strcoll","strcpy",
"strcspn","strerror","strlen","strncat",
"strncmp","strncpy","strpbrk","strrchr",
"strspn","strstr","strtok","strxfrm",
"swprintf","swscanf","tan","tanh",
"tmpfile","tmpnam","tolower","toupper",
"towlower","towupper","ungetc","ungetwc",
"vfprintf","vfwprintf","vprintf","vsprintf",
"vswprintf","vwprintf","wcscat","wcschr",
"wcscmp","wcscoll","wcscpy","wcscspn",
"wcslen","wcsncat","wcsncmp","wcsncpy",
"wcspbrk","wcsrchr","wcsspn","wcsstr",
"wcstok","wcsxfrm","wprintf","wscanf",
(char *)0};
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
{44,(char (*)())calloc,"VLL"},
{17,(char (*)())ceil,"BB"},
{45,(char (*)())clearerr,"FX"},
{17,(char (*)())cos,"BB"},
{17,(char (*)())cosh,"BB"},
{46,(char (*)())dtos,"JJJJ"},
{17,(char (*)())exp,"BB"},
{17,(char (*)())fabs,"BB"},
{6,(char (*)())fclose,"DX"},
{6,(char (*)())feof,"DX"},
{6,(char (*)())ferror,"DX"},
{6,(char (*)())fflush,"DX"},
{6,(char (*)())fgetc,"DX"},
{47,(char (*)())fgetpos,"DXX"},
{48,(char (*)())fgets,"QQDX"},
{49,(char (*)())fgetwc,"NX"},
{50,(char (*)())fgetws,"^^DX"},
{17,(char (*)())floor,"BB"},
{15,(char (*)())fmod,"BBB"},
{22,(char (*)())fopen,"XQQ"},
{51,(char (*)())fprintf,"DXQ0"},
{7,(char (*)())fputc,"DDX"},
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
{62,(char (*)())generic_bound,"CCCC"},
{63,(char (*)())generic_limiter,"CCCCC"},
{64,(char (*)())generic_sigmoid,"CCCCCC"},
{6,(char (*)())getc,"DX"},
{3,(char (*)())getchar,"DF"},
{27,(char (*)())gets,"QQ"},
{49,(char (*)())getwc,"NX"},
{5,(char (*)())getwchar,"NF"},
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
{67,(char (*)())labs,"EE"},
{68,(char (*)())lcat,"VVV"},
{69,(char (*)())lcatuni,"VVVK"},
{68,(char (*)())lcopy,"VVV"},
{70,(char (*)())ldexp,"BBD"},
{71,(char (*)())lequal,"KVV"},
{56,(char (*)())lfree,"FV"},
{72,(char (*)())lhopsum,"BJJJCC0"},
{68,(char (*)())lintersect,"VVV"},
{73,(char (*)())llcat,"V0"},
{74,(char (*)())lmap,"VKV0"},
{69,(char (*)())lmi_in,"VVVK"},
{68,(char (*)())lminus,"VVV"},
{75,(char (*)())lnew,"VDD"},
{17,(char (*)())log,"BB"},
{17,(char (*)())log10,"BB"},
{76,(char (*)())lrand,"ZJJJJ"},
{77,(char (*)())lrev,"VV"},
{78,(char (*)())lsmap,"ZZ"},
{79,(char (*)())lsum,"BV"},
{68,(char (*)())lunion,"VVV"},
{80,(char (*)())malloc,"VL"},
{81,(char (*)())mem_free,"F"},
{82,(char (*)())mem_mal,"Q"},
{82,(char (*)())mem_rel,"Q"},
{83,(char (*)())memchr,"VVDL"},
{20,(char (*)())memcmp,"DVVL"},
{84,(char (*)())memcpy,"VVVL"},
{84,(char (*)())memmove,"VVVL"},
{83,(char (*)())memset,"VVDL"},
{85,(char (*)())modf,"BBR"},
{86,(char (*)())nsl_lst_hilo,"BVK"},
{87,(char (*)())nsl_lst_ma,"VVBKK"},
{88,(char (*)())nsl_lst_more,"VVDDD"},
{89,(char (*)())nsl_lst_ndd,"BVJ"},
{90,(char (*)())nsl_lst_norm,"VVK"},
{91,(char (*)())nsl_lst_nth,"RVJ"},
{92,(char (*)())nsl_lst_num,"JV"},
{93,(char (*)())nsl_lst_pos,"JVB"},
{94,(char (*)())nsl_lst_put,"FVBJ"},
{95,(char (*)())nsl_lst_val,"BfJ"},
{96,(char (*)())nsl_lst_vma,"VVVKK"},
{97,(char (*)())perror,"FQ"},
{15,(char (*)())pow,"BBB"},
{98,(char (*)())printf,"DQ0"},
{99,(char (*)())printfs,"QQ0"},
{7,(char (*)())putc,"DDX"},
{0,(char (*)())putchar,"DD"},
{32,(char (*)())puts,"DQ"},
{53,(char (*)())putwc,"NNX"},
{8,(char (*)())putwchar,"NN"},
{2,(char (*)())realloc,"VVL"},
{32,(char (*)())remove,"DQ"},
{26,(char (*)())rename,"DQQ"},
{45,(char (*)())rewind,"FX"},
{0,(char (*)())rnd,"DD"},
{100,(char (*)())rndf,"C"},
{98,(char (*)())scanf,"DQ0"},
{101,(char (*)())setbuf,"FXQ"},
{102,(char (*)())setvbuf,"DXQDL"},
{17,(char (*)())sin,"BB"},
{17,(char (*)())sinh,"BB"},
{103,(char (*)())sprintf,"DQQ0"},
{17,(char (*)())sqrt,"BB"},
{104,(char (*)())srnd,"FD"},
{103,(char (*)())sscanf,"DQQ0"},
{105,(char (*)())stodx,"JJJ"},
{105,(char (*)())stody,"JJJ"},
{31,(char (*)())strcat,"QQQ"},
{30,(char (*)())strchr,"QQD"},
{26,(char (*)())strcmp,"DQQ"},
{26,(char (*)())strcoll,"DQQ"},
{31,(char (*)())strcpy,"QQQ"},
{106,(char (*)())strcspn,"LQQ"},
{107,(char (*)())strerror,"QD"},
{108,(char (*)())strlen,"LQ"},
{109,(char (*)())strncat,"QQQL"},
{28,(char (*)())strncmp,"DQQL"},
{109,(char (*)())strncpy,"QQQL"},
{31,(char (*)())strpbrk,"QQQ"},
{30,(char (*)())strrchr,"QQD"},
{106,(char (*)())strspn,"LQQ"},
{31,(char (*)())strstr,"QQQ"},
{31,(char (*)())strtok,"QQQ"},
{110,(char (*)())strxfrm,"LQQL"},
{111,(char (*)())swprintf,"D^^0"},
{111,(char (*)())swscanf,"D^^0"},
{17,(char (*)())tan,"BB"},
{17,(char (*)())tanh,"BB"},
{112,(char (*)())tmpfile,"XF"},
{27,(char (*)())tmpnam,"QQ"},
{0,(char (*)())tolower,"DD"},
{0,(char (*)())toupper,"DD"},
{8,(char (*)())towlower,"NN"},
{8,(char (*)())towupper,"NN"},
{7,(char (*)())ungetc,"DDX"},
{53,(char (*)())ungetwc,"NNX"},
{51,(char (*)())vfprintf,"DXQ0"},
{61,(char (*)())vfwprintf,"DX^0"},
{98,(char (*)())vprintf,"DQ0"},
{103,(char (*)())vsprintf,"DQQ0"},
{111,(char (*)())vswprintf,"D^^0"},
{113,(char (*)())vwprintf,"D^0"},
{42,(char (*)())wcscat,"^^^"},
{36,(char (*)())wcschr,"^^N"},
{33,(char (*)())wcscmp,"D^^"},
{33,(char (*)())wcscoll,"D^^"},
{42,(char (*)())wcscpy,"^^^"},
{114,(char (*)())wcscspn,"L^^"},
{115,(char (*)())wcslen,"L^"},
{116,(char (*)())wcsncat,"^^^L"},
{34,(char (*)())wcsncmp,"D^^L"},
{116,(char (*)())wcsncpy,"^^^L"},
{42,(char (*)())wcspbrk,"^^^"},
{36,(char (*)())wcsrchr,"^^N"},
{114,(char (*)())wcsspn,"L^^"},
{42,(char (*)())wcsstr,"^^^"},
{42,(char (*)())wcstok,"^^^"},
{117,(char (*)())wcsxfrm,"L^^L"},
{113,(char (*)())wprintf,"D^0"},
{113,(char (*)())wscanf,"D^0"},
{0,(char (*)())0L,(char *)0L}};
int nsl_num_mon_list=284;
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
*((float *)(cp+0)),*((float *)(cp+4)),*((float *)(cp+8))
); break;
case 63:
rv.typ=3;rv.val.ff=(*fp.ff)(
*((float *)(cp+0)),*((float *)(cp+4)),*((float *)(cp+8)),
*((float *)(cp+12))); break;
case 64:
rv.typ=3;rv.val.ff=(*fp.ff)(
*((float *)(cp+0)),*((float *)(cp+4)),*((float *)(cp+8)),
*((float *)(cp+12)),*((float *)(cp+16))); break;
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
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((char **)(cp+4)),*((unsigned char *)(cp+8))
); break;
case 70:
rv.typ=2;rv.val.dd=(*fp.dd)(
*((double *)(cp+0)),*((int *)(cp+8))); break;
case 71:
rv.typ=11;rv.val.ucc=(*fp.ucc)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 72:
rv.typ=2;rv.val.dd=(*fp.dd)(
*((short *)(cp+0)),*((short *)(cp+2)),*((short *)(cp+4)),
*((float *)(cp+6)),*((float *)(cp+10)),((char *)(cp+14))
); break;
case 73:
rv.typ=15;rv.val.pp=(*fp.pp)(
((char *)(cp+0))); break;
case 74:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((unsigned char *)(cp+0)),*((char **)(cp+1)),((char *)(cp+5))
); break;
case 75:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((int *)(cp+0)),*((int *)(cp+4))); break;
case 76:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((short *)(cp+0)),*((short *)(cp+2)),*((short *)(cp+4)),
*((short *)(cp+6))); break;
case 77:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0))); break;
case 78:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0))); break;
case 79:
rv.typ=2;rv.val.dd=(*fp.dd)(
*((char **)(cp+0))); break;
case 80:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((unsigned int *)(cp+0))); break;
case 81:
rv.typ=4;rv.val.ii=0;(*fp.vv)(
); break;
case 82:
rv.typ=15;rv.val.pp=(*fp.pp)(
); break;
case 83:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((int *)(cp+4)),*((unsigned int *)(cp+8))
); break;
case 84:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((char **)(cp+4)),*((unsigned int *)(cp+8))
); break;
case 85:
rv.typ=2;rv.val.dd=(*fp.dd)(
*((double *)(cp+0)),*((char **)(cp+8))); break;
case 86:
rv.typ=2;rv.val.dd=(*fp.dd)(
*((char **)(cp+0)),*((unsigned char *)(cp+4))); break;
case 87:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((double *)(cp+4)),*((unsigned char *)(cp+12)),
*((unsigned char *)(cp+13))); break;
case 88:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((int *)(cp+4)),*((int *)(cp+8)),
*((int *)(cp+12))); break;
case 89:
rv.typ=2;rv.val.dd=(*fp.dd)(
*((char **)(cp+0)),*((short *)(cp+4))); break;
case 90:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((unsigned char *)(cp+4))); break;
case 91:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((short *)(cp+4))); break;
case 92:
rv.typ=10;rv.val.ss=(*fp.ss)(
*((char **)(cp+0))); break;
case 93:
rv.typ=10;rv.val.ss=(*fp.ss)(
*((char **)(cp+0)),*((double *)(cp+4))); break;
case 94:
rv.typ=4;rv.val.ii=0;(*fp.vv)(
*((char **)(cp+0)),*((double *)(cp+4)),*((short *)(cp+12))
); break;
case 95:
rv.typ=2;rv.val.dd=(*fp.dd)(
*((char **)(cp+0)),*((short *)(cp+4))); break;
case 96:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((char **)(cp+4)),*((unsigned char *)(cp+8)),
*((unsigned char *)(cp+9))); break;
case 97:
rv.typ=4;rv.val.ii=0;(*fp.vv)(
*((char **)(cp+0))); break;
case 98:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),((char *)(cp+4))); break;
case 99:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),((char *)(cp+4))); break;
case 100:
rv.typ=3;rv.val.ff=(*fp.ff)(
); break;
case 101:
rv.typ=4;rv.val.ii=0;(*fp.vv)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 102:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((char **)(cp+4)),*((int *)(cp+8)),
*((unsigned int *)(cp+12))); break;
case 103:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((char **)(cp+4)),((char *)(cp+8))
); break;
case 104:
rv.typ=4;rv.val.ii=0;(*fp.vv)(
*((int *)(cp+0))); break;
case 105:
rv.typ=10;rv.val.ss=(*fp.ss)(
*((short *)(cp+0)),*((short *)(cp+2))); break;
case 106:
rv.typ=12;rv.val.uii=(*fp.uii)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 107:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((int *)(cp+0))); break;
case 108:
rv.typ=12;rv.val.uii=(*fp.uii)(
*((char **)(cp+0))); break;
case 109:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((char **)(cp+4)),*((unsigned int *)(cp+8))
); break;
case 110:
rv.typ=12;rv.val.uii=(*fp.uii)(
*((char **)(cp+0)),*((char **)(cp+4)),*((unsigned int *)(cp+8))
); break;
case 111:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),*((char **)(cp+4)),((char *)(cp+8))
); break;
case 112:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((int *)(cp+0))); break;
case 113:
rv.typ=4;rv.val.ii=(*fp.ii)(
*((char **)(cp+0)),((char *)(cp+4))); break;
case 114:
rv.typ=12;rv.val.uii=(*fp.uii)(
*((char **)(cp+0)),*((char **)(cp+4))); break;
case 115:
rv.typ=12;rv.val.uii=(*fp.uii)(
*((char **)(cp+0))); break;
case 116:
rv.typ=15;rv.val.pp=(*fp.pp)(
*((char **)(cp+0)),*((char **)(cp+4)),*((unsigned int *)(cp+8))
); break;
case 117:
rv.typ=12;rv.val.uii=(*fp.uii)(
*((char **)(cp+0)),*((char **)(cp+4)),*((unsigned int *)(cp+8))
); break;
default:rv.typ=4;rv.val.ii=0;}
return(rv);}
