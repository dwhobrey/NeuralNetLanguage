
typedef union 
{int ival;
 ni_elm nival;
 glo_elm gval;
 NSLNSLfval fval;
 char *sval;
 char (*fpval)();
} YYSTYPE;
extern YYSTYPE yylval;
# define PR1 257
# define CEOF 258
# define NUMBER 259
# define IDENT 260
# define LABEL 261
# define FUNKY 262
# define UFFKY 263
# define UFFKY_ONE 264
# define CBREAK 265
# define CCALL 266
# define CCALL_ONE 267
# define CCONT 268
# define CCLS 269
# define CCD 270
# define CDELETE 271
# define CDO 272
# define CDO_ONE 273
# define CDIR 274
# define CDISABLE 275
# define CENABLE 276
# define CELSE 277
# define CEXIT 278
# define CDRIVER 279
# define CFOR 280
# define CFOR_ONE 281
# define CFOR_TWO 282
# define CGO 283
# define CHELP 284
# define CDEBUG 285
# define CIF 286
# define CLOAD 287
# define CLOG 288
# define CLOGCMD 289
# define CNEW 290
# define CPRINT 291
# define CRETURN 292
# define CCLOSE 293
# define COPEN 294
# define CSEARCH 295
# define CLINE 296
# define CDUMP 297
# define CSAVE 298
# define CSHELL 299
# define CSHOW 300
# define CSTEP 301
# define CSTOP 302
# define CWATCH 303
# define CWHILE 304
# define CTYPE 305
# define S_EXP 306
# define S_CMDS 307
# define CUSER 308
# define VAR 309
# define DQSTR 310
# define CNET 311
# define EQOP 312
# define OROR 313
# define ANDAND 314
# define EQ 315
# define NE 316
# define LE 317
# define GE 318
# define LS 319
# define RS 320
# define PLUPLU 321
# define MINMIN 322
# define DOLDOL 323
# define UMINUS 324
# define DOTDOT 325
