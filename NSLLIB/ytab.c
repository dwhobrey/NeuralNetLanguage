
/* Name: nslmsmon.y	Type: yacc file	Date: 18 May 89
*  NeSeL Network Specification Language
*/
#include "nslms.h"
#include "nslmsmon.h"
#include "nslwsio.h"
extern int mon_wnd;

typedef union 
{int ival;
 ni_elm nival;
 glo_elm gval;
 NSLNSLfval fval;
 char *sval;
 char (*fpval)();
} YYSTYPE;
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
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256
short yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
	};
# define YYNPROD 185
# define YYLAST 1364
short yyact[]={

  60, 233, 146,  60,   3,   2, 279, 291, 288, 162,
 163, 173, 172, 102,  96,  95,  92,  88,  87, 113,
  82, 285, 217, 106, 153, 162, 163, 150, 267, 225,
  77,  63, 270, 151, 282, 280, 187, 218,  54, 237,
 181,  78, 123, 290, 231, 239, 175, 121, 240, 278,
 234, 177, 122, 269, 252, 164, 235, 281, 246, 180,
 123, 134,  60, 156, 178, 121, 124, 123, 125, 117,
 122, 164, 121, 124, 108, 125, 109, 122, 105,  81,
  75, 103,  97, 128,  94, 129,  93,  44,  79,  45,
 169, 116, 118,  91, 114, 115, 140, 141, 142, 143,
  86,  85,  84,  83, 149, 228, 258, 147, 104,  76,
   7, 167, 152,   6, 251, 227, 168, 135, 100, 101,
 123, 170, 171, 224,  60, 121, 124, 160, 125, 179,
 122, 157,  65, 244, 223,  60, 221, 219, 287,  68,
 283, 277, 284, 128, 276, 129, 268, 136, 256,  44,
 174,  45, 123, 134, 186, 192, 119, 121, 124, 189,
 125, 232, 122,  67, 190,  80, 123,   1,   5,   0,
   0, 121, 124,  74, 125, 128, 122, 129,  65,   0,
   0,  60,   0,  71,   0,  68,   0,   0,   0,   0,
  64, 162, 163,   0, 229,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   4,   0,   0,   0, 135,
   0,  44, 241,  45,   0,   0,   0,   0,   0,   0,
   0,   0,  53,   0,   0,  66,  57,  58, 272,   8,
  11,   0,   9,  16,  18,  34,  46, 164,  17,  35,
  36,  52,  21,  31,  48,  49,  50,  13,  14,  15,
  51,  32,  22,  23,  20,  24,  10,  27,  28,  26,
  25,  29,  33,  38,  37,  12,  39,  40,  47,  41,
 112,  66,  30,  59, 159,  19,  59, 259, 255, 110,
 111, 176, 271, 248,  53,  55,  56,  61,  57,  58,
  61,   8,  11, 264,   9,  16,  18,  34,  46, 292,
  17,  35,  36,  52,  21,  31,  48,  49,  50,  13,
  14,  15,  51,  32,  22,  23,  20,  24,  10,  27,
  28,  26,  25,  29,  33,  38,  37,  12,  39,  40,
  47,  41, 286,   0,  30,  59,   0,  19, 132, 133,
 130, 131, 126, 127,   0,   0,  53,  55,  56,  61,
  57,  58,   0,   8,  11, 158,   9,  16,  18,  34,
  46,   0,  17,  35,  36,  52,  21,  31,  48,  49,
  50,  13,  14,  15,  51,  32,  22,  23,  20,  24,
  10,  27,  28,  26,  25,  29,  33,  38,  37,  12,
  39,  40,  47,  41,   0,   0,  30,  59,   0,  19,
 130, 131, 126, 127,  69, 165, 166,  57,  58,  55,
  56,  61,  65,   0,   0,  60,  74,  71,   0,  68,
  55,  56,  61, 247,  64, 162, 163, 215,   0,  74,
 132, 133, 130, 131, 126, 127, 123, 134, 230,   0,
   0, 121, 124,   0, 125, 162, 242, 260, 126, 127,
   0,   0,   0,   0,  59,  70, 159, 226,   0, 128,
   0, 129, 139,   0,   0,   0,  55,  56,  61,   0,
 154, 164, 123, 134,   0,   0, 155, 121, 124,   0,
 125,   0, 122,   0, 123, 134,   0,   0, 257, 121,
 124, 164, 125, 135, 122, 128,  65, 129, 139,  60,
   0,  71,   0,  68,   0,  66,   0, 128,  64, 129,
 139, 123, 134, 220,   0,   0, 121, 124,   0, 125,
   0, 122,   0, 136,   0,   0,   0,   0, 274, 135,
   0,   0, 245,   0, 128,   0, 129, 139, 123, 134,
   0, 135, 238, 121, 124,   0, 125, 243, 122,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0, 136,
   0, 128,   0, 129, 139, 289,   0,   0, 135,   0,
   0, 136, 289,   0,   0,   0, 123, 134,   0, 250,
   0, 121, 124,   0, 125,   0, 122,   0,   0,  66,
   0,   0,   0,   0,   0, 135, 193, 226, 136, 128,
 262, 129, 139,   0, 123, 134,   0,   0, 216, 121,
 124,   0, 125,   0, 122,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0, 136,   0, 128,   0, 129,
 139,   0,   0, 135,   0, 222,   0,   0,  69, 165,
 166,  57,  58,   0,   0,   0, 123, 134,   0,   0,
 214, 121, 124,   0, 125,   0, 122,   0,   0,   0,
 123, 135,   0, 136,   0, 121, 124,   0, 125, 128,
 122, 129, 139,   0,   0,  65,   0,   0,  60,   0,
  71,   0,  68, 128,   0, 129,   0,  64,  59,  70,
 159, 136,   0,   0,   0,   0,   0, 249,   0,   0,
  55,  56,  61, 135, 154,   0,   0,   0,   0,   0,
   0,   0, 138, 137, 132, 133, 130, 131, 126, 127,
 261,   0,  69, 165, 166,  57,  58, 265,   0,  65,
   0,   0,  60, 136,  71,  72,  68, 185,  43, 273,
  43,  64,   0,   0,   0,   0,   0,   0, 138, 137,
 132, 133, 130, 131, 126, 127,   0,   0,   0,   0,
 138, 137, 132, 133, 130, 131, 126, 127,  66,   0,
   0,   0,  59,  70, 194,   0,   0,   0,   0,   0,
   0,   0,   0,   0,  55,  56,  61, 138, 137, 132,
 133, 130, 131, 126, 127,   0,   0,  73,   0,   0,
  42,   0,  42, 123, 134,   0,   0,   0, 121, 124,
   0, 125,   0, 122, 138, 137, 132, 133, 130, 131,
 126, 127,  66,   0,   0,   0, 128,  65, 129, 139,
  60,   0,  71,   0,  68,   0,   0,  65,   0,  64,
  60,   0,  71,   0,  68,   0,   0,   0,   0,  64,
   0,   0, 138, 137, 132, 133, 130, 131, 126, 127,
 135,  65,   0,   0,  60,   0,  71,   0,  68,   0,
   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,
 138, 137, 132, 133, 130, 131, 126, 127, 123, 134,
 136,   0,   0, 121, 124,   0, 125,   0, 122,   0,
   0,  69, 165, 166,  57,  58,   0,   0,   0,   0,
   0, 128,   0, 129, 139,   0,   0,   0,   0,   0,
  66,   0, 138, 137, 132, 133, 130, 131, 126, 127,
  66,   0,   0,   0,   0,   0,   0,   0, 132, 133,
 130, 131, 126, 127,   0, 135,   0,   0,   0,   0,
   0,  59,  70,  43,  66,  69,   0,   0,  57,  58,
   0,   0,   0,  55,  56,  61,   0,   0, 123, 134,
   0,   0,   0, 121, 124, 136, 125,  43, 122, 123,
 134,   0,   0,  43, 121, 124,   0, 125,   0, 122,
   0, 128,   0, 129,   0,   0,  43,   0,   0,   0,
   0,   0, 128,   0, 129,  59,  70,   0,   0,   0,
   0,   0,   0,   0,   0,  42,   0,  55,  56,  61,
   0,   0,   0,   0, 120, 135,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,  42,
   0,   0,   0,   0,   0,  42,   0,   0,   0,   0,
   0,   0,   0,  69,   0, 136,  57,  58,  42,   0,
   0,   0,   0,  69,  99,   0,  57,  58,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0, 138,
 137, 132, 133, 130, 131, 126, 127,  69,   0,   0,
  57,  58,   0, 161,   0,   0,   0,  62,   0, 145,
   0,   0,   0,  59,  70,   0,   0,   0,   0,   0,
   0,   0,   0,  59,  70,  55,  56,  61,  89,  90,
   0,   0,   0,   0,   0,  55,  56,  61,  98,  98,
  98,   0,   0,   0,   0,   0,   0,  59,  70,   0,
   0,   0,   0, 107, 107,   0,   0,   0,   0,  55,
  56,  61,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0, 144,   0, 138, 137, 132, 133, 130, 131,
 126, 127,   0,   0,   0, 148,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0, 107,
   0,   0,   0, 182,   0,   0, 183, 184,   0,   0,
   0, 188,   0, 191,   0, 195, 196, 197, 198, 199,
 200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
 210, 211, 212, 213,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0, 137, 132, 133, 130, 131,
 126, 127,   0,   0,   0,   0,   0, 132, 133, 130,
 131, 126, 127,   0,   0,   0,   0,   0,   0,   0,
   0,   0, 236,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
 253, 254,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0, 188,   0, 196,   0,   0, 263,
   0,   0,   0,   0,   0, 266,   0,   0,   0,   0,
   0,   0, 107,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0, 275 };
short yypact[]={

-302,-1000,  88, 828,  88,-1000, -18, -18,-1000,-1000,
  39,-240,-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,
-1000,-1000,-242,-243, 828, 828,-1000,-1000,-1000,-244,
-1000,-1000,-245,-246, 804, 804, 804,-247,-1000,-1000,
-1000,-1000,-1000,-1000,-1000,-1000,-1000,-1000,  38, 828,
 828,  36,-1000,-1000, -42, -33, -33,  29,  29,-1000,
-1000,-1000, 766,-1000, 828, 828, 828, 828, 794,-1000,
-1000,-307,-1000,-1000,-1000,-1000, -18,-1000,-1000,-1000,
-1000, 828,-1000,  29,  29, 379, 379,-1000,-1000, 851,
 851, 379,-1000,  29,  29,-248,-249,-1000, 851,-1000,
-1000,-1000,-1000,   9,  24, 828,  18, 851, -19, 828,
-1000,-1000, 828, 828,-1000,-1000,-1000, 696,-1000, 463,
-1000, 828, 828, 828, 828, 828, 828, 828, 828, 828,
 828, 828, 828, 828, 828, 828, 828, 828, 828, 828,
-1000,-1000,-1000,-1000, 609,   9,-1000,-1000, 567, -22,
-1000,-1000,-1000,-1000,-1000,-1000,-1000, -21, 642,-1000,
 -62, 539,-1000,-1000,-1000,-1000,-1000,-1000,-1000, 145,
-1000,-1000,-1000,-1000,   0,-1000,-259,  14, 828, -20,
-1000,-1000, 501, 851, 851,-1000,   4,-1000, 851, -22,
-1000, 399,-1000,-1000,-1000,-1000,-1000,-1000,   5,   5,
  30,  30, 129, 129, 129, 129,  83,  83, 623, 942,
 115,  23, 931, 474,-1000,  17,-1000,  88,-1000, -37,
 642,-1000,-1000, -21,-1000,  12, 828, 828,-1000,-1000,
-1000,   9,-1000,-1000,-1000,-1000, 447,-1000,-1000,-1000,
 828,  88,  99, 642, -21, 828,-1000,  26,-1000,-1000,
 642, 828, -65, 851, 851,-1000,  -8,-1000, 828,-1000,
 -36,-1000, 642, 851,-1000,-1000, 435,-1000,-1000, 828,
-299, -24,-1000,-1000,-1000, 851,  16, -25,-1000,-1000,
-1000,-102,-299,   9,-1000,-1000,-1000,  -1,-1000,-253,
   9,-1000,-1000 };
short yypgo[]={

   0, 167,1093,  31, 165,  23,  38, 164, 797, 735,
 281, 163, 161, 205,  33, 159,  22, 156, 355, 476,
 155, 154,  36, 150,  46, 148, 146, 144, 142, 141,
  49, 140, 138,   8,  27,  24, 137,  63, 136, 134,
 133, 131, 127, 123, 115, 114,  82, 168, 113,  80,
 110, 109,  30, 108, 106, 105, 104, 103, 102, 101,
 100,  93,  90,  86,  84,  81 };
short yyr1[]={

   0,   1,   1,   1,   2,   2,   2,   2,   2,   2,
   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,
   3,   3,   3,   3,   9,  15,   9,   8,   8,   8,
   8,   8,   8,   8,   6,  17,   6,   6,   7,   7,
  14,  14,  21,  21,  22,  11,  23,  23,  25,  24,
  12,  12,  26,  26,  26,  28,  28,  27,  27,  29,
  29,  31,  30,  32,  32,  33,  10,  10,  10,   5,
   5,  34,  34,   4,   4,  36,  35,  35,  35,  37,
  37,  39,  37,  20,  40,  20,  38,  38,  19,  19,
  41,  41,  42,  44,  42,  43,  45,  43,  43,  46,
  46,  18,  18,  18,  13,  13,  47,  47,  49,  49,
  16,  16,  51,  51,  52,  50,  50,  50,  53,  50,
  54,  50,  50,  50,  50,  50,  50,  55,  55,  48,
  48,  48,  56,  48,  57,  48,  58,  48,  48,  48,
  48,  59,  48,  60,  48,  48,  48,  48,  48,  48,
  48,  48,  48,  48,  61,  48,  62,  48,  63,  48,
  64,  48,  48,  48,  48,  48,  48,  48,  48,  48,
  48,  65,  48,  48,  48 };
short yyr2[]={

   0,   2,   3,   0,   3,   3,   3,   3,   3,   3,
   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
   3,   3,   5,   1,   2,   2,   2,   2,   3,   1,
   1,   2,   1,   1,   2,   0,   6,   1,   2,   2,
   3,   3,   2,   2,   1,   0,   3,   1,   3,   1,
   2,   3,   1,   3,   1,   4,   3,   1,   0,   4,
   1,   1,   0,   2,   4,   0,   1,   0,   1,   1,
   3,   0,   3,   1,   3,   2,   0,   1,   2,   0,
   1,   0,   1,   0,   3,   0,   3,   1,   1,   0,
   2,   0,   4,   1,   0,   4,   0,   1,   3,   1,
   2,   3,   1,   0,   3,   0,   0,   4,   3,   1,
   1,   1,   1,   1,   2,   1,   2,   2,   0,   1,
   0,   1,   2,   1,   1,   1,   1,   1,   0,   5,
   0,   7,   3,   3,   4,   1,   1,   1,   1,   1,
   1,   2,   0,   6,   0,   3,   0,   3,   1,   1,
   1,   0,   3,   0,   3,   1,   1,   1,   2,   2,
   2,   2,   1,   1,   0,   3,   0,   4,   0,   3,
   0,   3,   3,   3,   2,   2,   2,   2,   1,   1,
   1,   0,   3,   1,   1 };
short yychk[]={

-1000,  -1, 307, 306, -13, -47, -48, -50, 265, 268,
 292, 266, 301, 283, 284, 285, 269, 274, 270, 311,
 290, 278, 288, 289, 291, 296, 295, 293, 294, 297,
 308, 279, 287, 298, 271, 275, 276, 300, 299, 302,
 303, 305,  -8,  -9, 123, 125, 272, 304, 280, 281,
 282, 286, 277, 258,  -6, 321, 322, 262, 263, 309,
  36, 323,  -2,  -3,  45,  33, 126, -11,  40, 259,
 310,  38,  -9,  -8, -47, -49, -51, -52,  59, -49,
  -4,  40, 260, -57, -58, -59, -60, 260, 260,  -2,
  -2, -61, 260, -63, -64, 260, 260, -46,  -2, 260,
 -46, -46, 260, -65, -53,  40,  -5,  -2,  -5,  40,
 321, 322, 312,  61,  -6,  -6, -14,  40, -14, -17,
 258,  42,  47,  37,  43,  45, 319, 320,  60,  62,
 317, 318, 315, 316,  38,  94, 124, 314, 313,  63,
  -3,  -3,  -3,  -3,  -2, 305, 309, -52,  -2, -56,
 -34, -14, -34, -35, 325, -19, -37, -41, -18, 311,
 -42,  -2,  46,  47,  92, 260, 261, -35, -35, -62,
 -34, -34, 260, 260, -23, -24, -10,  42,  40,  -5,
  41,  59,  -2,  -2,  -2,  41, -21, -22,  -2, -15,
  -7,  -2, -20, -19, 311,  -2,  -2,  -2,  -2,  -2,
  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,
  -2,  -2,  -2,  -2,  41, -10,  41, -16,  59, -36,
 -18, -38, -19, -39, -43,  91,  58, -44, -55, -35,
 293,  44, -12, 260, 309,  42,  -2,  59,  41,  41,
  44, -16,  47, -18, -40,  58,  41, -13, -37, -19,
 -18, -45,  42,  -2,  -2, -24, -25,  41, -54, -22,
 -13, -19, -18,  -2, 267, -19,  -2,  93, -26,  61,
  40,  -5, 264, -19,  93,  -2, -27, -29, -30, 305,
  59,  41,  59, -31, -28, 123, -30, -32, -33, -10,
  44, 260, -33 };
short yydef[]={

   3,  -2,   0,   0,   1, 115, 118, 118, 139, 140,
  83,   0, 144, 146, 148, 149, 150, 151, 153, 155,
 156, 157,   0,   0,   0,   0, 162, 163, 164,   0,
 168, 170,   0,   0,   0,   0,   0,   0, 178, 179,
 180, 181, 183, 184, 125, 126, 127, 128,   0,  79,
  79,   0, 135, 136,  37,   0,   0,   0,   0,  44,
  45,  47,   0,  23,   0,   0,   0,   0,   0,  29,
  30,   0,  32,  33, 114, 116, 119, 123, 124, 117,
 141,   0, 142,  81,  81,  89,  89, 158, 159, 160,
 161,  89, 166,  81,  81,   0,   0, 174, 109, 110,
 175, 176, 177,  76,   0,  79,   0,  80,   0,   0,
  38,  39,   0,   0,  42,  43,  34,   0,  35,   0,
   2,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  24,  25,  26,  27,   0,  76,  31, 122,   0, 120,
 145,  82, 147, 152,  85,  87,  88,  99,  96,  91,
 105,   0, 111, 112, 113, 102, 103, 154, 165,  89,
 169, 171, 172, 173, 182,  57,   0,  77,   0,   0,
 132, 133,   0,  40,  41,  50,   0,  52,  54, 120,
  46,   0,  49,  93,  94,   4,   5,   6,   7,   8,
   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,
  19,  20,  21,   0,  28,   0,  84,   0, 121,  89,
   0,  90,  97,   0, 100, 106,   0,   0, 167, 137,
 138,  76,  58,  60,  61,  78,   0, 130, 134,  51,
   0,   0, 112,   0,   0,   0,  55,   0,  86,  98,
   0,   0,   0, 101, 104,  56,  62, 129,  79,  53,
   0,  48,   0,  22, 143,  92,   0, 108,  59,   0,
  67,   0,  36,  95, 107,  63,   0,  68,  69,  71,
 131,  65,   0,  76,  64,  66,  70,  72,  73,   0,
  76,  75,  74 };
/* NOTE this is a recursive version of the yyparse() rountine. DJRW Jan 1992.
*  Use this file in place of the normal 'yaccpar' file when making yacc.
*  See the utility 'yactorec.c' for converting your yaccpar file if need be.
*/
# define YYFLAG -1000
# define YYERROR goto yyerrlab
# define YYACCEPT goto yyyaccept
# define YYABORT goto yyyabort

/*parser for yacc output*/

#ifdef YYDEBUG
  int yydebug = 0; /* 1 for debugging */
#endif
YYSTYPE yyv[YYMAXDEPTH],*yyyv=yyv; /* where the values are stored */
short yys[YYMAXDEPTH],*yyys=yys;
int yychar = -1; /* current input token number */
int yynerrs = 0;  /* number of errors */
short yyerrflag = 0;  /* error recovery flag */

yyparse()
{ short yyyer=yyerrflag,yyyok=0,yyj,yym;
int yyych=yychar,yyyne=yynerrs;
short *yyyss=yyys; YYSTYPE *yyyvv=yyyv;
  register YYSTYPE *yypvt; register short yystate, *yyps, yyn;
  register YYSTYPE *yypv; register short *yyxi;
  yystate=0; yychar=(-1); yynerrs=0; yyerrflag=0;
  yyps=yyys; yypv=yyyv;
  yystack:    /* put a state and value onto the stack */
#ifdef YYDEBUG
  if( yydebug  ) printf( "state %d, char 0%o\n", yystate, yychar );
#endif
  if(++yyps>&yys[YYMAXDEPTH]) {yyerror("yacc stack overflow"); goto yyyabort;}
  *yyps=yystate; ++yypv; *yypv=yyval;
  yynewstate:
  yyn=yypact[yystate];
  if(yyn<=YYFLAG) goto yydefault; /* simple state */
  if(yychar<0) if((yychar=yylex())<0) yychar=0;
  if((yyn+=yychar)<0 || yyn>=YYLAST) goto yydefault;
  if(yychk[ yyn=yyact[yyn]]==yychar)
    { /* valid shift */
     yychar=(-1); yyval=yylval; yystate=yyn; if(yyerrflag>0) --yyerrflag;
     goto yystack;
    }
  yydefault: /* default state action */
  if((yyn=yydef[yystate])==(-2))
    {if(yychar<0) if((yychar=yylex())<0) yychar=0;
     /* look through exception table */
     for(yyxi=yyexca; (*yyxi!=(-1)) || (yyxi[1]!=yystate); yyxi+=2); /*VOID*/
     while(*(yyxi+=2)>=0)
       {if(*yyxi==yychar) break; }
     if((yyn=yyxi[1])<0) goto yyyaccept; /* accept */
    }
  if(yyn==0) /* error ... attempt to resume parsing */
    {switch(yyerrflag)
       {case 0: /* brand new error */
          yyerror( "syntax error" );
          yyerrlab:
          ++yynerrs;
        case 1:
        case 2: /* incompletely recovered error ... try again */
          yyerrflag = 3;
          /* find a state where "error" is a legal shift action */
          while(yyps>=yyys+1)
            {yyn=yypact[*yyps]+YYERRCODE;
             if(yyn>=0 && yyn<YYLAST && yychk[yyact[yyn]]==YYERRCODE)
               {yystate=yyact[yyn]; /* simulate a shift of "error" */
                goto yystack; }
             yyn=yypact[*yyps];
             /* the current yyps has no shift onn "error", pop stack */
#ifdef YYDEBUG
  if( yydebug ) printf( "error recovery pops state %d, uncovers %d\n", *yyps, yyps[-1] );
#endif
             --yyps; --yypv;
            }
          /* there is no state on the stack with an error shift ... abort */
          yyabort: goto yyyabort;
        case 3: /* no shift yet; clobber input char */
#ifdef YYDEBUG
  if( yydebug ) printf( "error recovery discards char %d\n", yychar );
#endif
          if(yychar==0) goto yyabort; /* don't discard EOF, quit */
          yychar=(-1);
          goto yynewstate; /* try again in the same state */
       }
    }
  /* reduction by production yyn */
#ifdef YYDEBUG
  if( yydebug ) printf("reduce %d\n",yyn);
#endif
  yyps-=yyr2[yyn]; yypvt=yypv; yypv-=yyr2[yyn]; yyval=yypv[1]; yym=yyn;
  /* consult goto table to find next state */
  yyn=yyr1[yyn]; yyj=yypgo[yyn]+ *yyps + 1;
  if(yyj>=YYLAST || yychk[yystate=yyact[yyj]]!=(-yyn))
    yystate=yyact[yypgo[yyn]];
  yyys=yyps; yyyv=yypv; switch(yym)
    {

case 1:
{cmd_exit=2; YYACCEPT;} break;
case 2:
{cmd_exit=2; tmp_val=tx_to_d(yypvt[-1].fval); yyclearin; YYACCEPT;} break;
case 3:
{cmd_exit=2; tmp_val=(double)0.0; yyclearin; YYACCEPT;} break;
case 4:
{yyval.fval=d_to_tx(yypvt[-2].fval.typ,yypvt[-0].fval.typ,tx_to_d(yypvt[-2].fval)*tx_to_d(yypvt[-0].fval));} break;
case 5:
{yyval.fval=d_to_tx(yypvt[-2].fval.typ,yypvt[-0].fval.typ,tx_to_d(yypvt[-2].fval)/tx_to_d(yypvt[-0].fval));} break;
case 6:
{yyval.fval=d_to_tx(yypvt[-2].fval.typ,yypvt[-0].fval.typ,(double)((long)tx_to_d(yypvt[-2].fval) % (long)tx_to_d(yypvt[-0].fval)));} break;
case 7:
{yyval.fval=d_to_tx(yypvt[-2].fval.typ,yypvt[-0].fval.typ,tx_to_d(yypvt[-2].fval)+tx_to_d(yypvt[-0].fval));} break;
case 8:
{if((yypvt[-2].fval.typ==TYPE_PT||yypvt[-2].fval.typ&STAR_MASK)&&(yypvt[-0].fval.typ==TYPE_PT||yypvt[-0].fval.typ&STAR_MASK))
		yyval.fval=d_to_t((int)TYPE_LONG,tx_to_d(yypvt[-2].fval)-tx_to_d(yypvt[-0].fval));
	 else  yyval.fval=d_to_tx(yypvt[-2].fval.typ,yypvt[-0].fval.typ,tx_to_d(yypvt[-2].fval)-tx_to_d(yypvt[-0].fval));} break;
case 9:
{yyval.fval=d_to_tx(yypvt[-2].fval.typ,yypvt[-0].fval.typ,(double)((long)tx_to_d(yypvt[-2].fval) << (long)tx_to_d(yypvt[-0].fval)));} break;
case 10:
{yyval.fval=d_to_tx(yypvt[-2].fval.typ,yypvt[-0].fval.typ,(double)((long)tx_to_d(yypvt[-2].fval) >> (long)tx_to_d(yypvt[-0].fval)));} break;
case 11:
{yyval.fval=d_to_tx(yypvt[-2].fval.typ,yypvt[-0].fval.typ,(double)(tx_to_d(yypvt[-2].fval)<tx_to_d(yypvt[-0].fval)));} break;
case 12:
{yyval.fval=d_to_tx(yypvt[-2].fval.typ,yypvt[-0].fval.typ,(double)(tx_to_d(yypvt[-2].fval)>tx_to_d(yypvt[-0].fval)));} break;
case 13:
{yyval.fval=d_to_tx(yypvt[-2].fval.typ,yypvt[-0].fval.typ,(double)(tx_to_d(yypvt[-2].fval)<=tx_to_d(yypvt[-0].fval)));} break;
case 14:
{yyval.fval=d_to_tx(yypvt[-2].fval.typ,yypvt[-0].fval.typ,(double)(tx_to_d(yypvt[-2].fval)>=tx_to_d(yypvt[-0].fval)));} break;
case 15:
{yyval.fval=d_to_tx(yypvt[-2].fval.typ,yypvt[-0].fval.typ,(double)(tx_to_d(yypvt[-2].fval)==tx_to_d(yypvt[-0].fval)));} break;
case 16:
{yyval.fval=d_to_tx(yypvt[-2].fval.typ,yypvt[-0].fval.typ,(double)(tx_to_d(yypvt[-2].fval)!=tx_to_d(yypvt[-0].fval)));} break;
case 17:
{yyval.fval=d_to_tx(yypvt[-2].fval.typ,yypvt[-0].fval.typ,(double)((long)tx_to_d(yypvt[-2].fval) & (long)tx_to_d(yypvt[-0].fval)));} break;
case 18:
{yyval.fval=d_to_tx(yypvt[-2].fval.typ,yypvt[-0].fval.typ,(double)((long)tx_to_d(yypvt[-2].fval) ^ (long)tx_to_d(yypvt[-0].fval)));} break;
case 19:
{yyval.fval=d_to_tx(yypvt[-2].fval.typ,yypvt[-0].fval.typ,(double)((long)tx_to_d(yypvt[-2].fval) | (long)tx_to_d(yypvt[-0].fval)));} break;
case 20:
{yyval.fval=d_to_tx(yypvt[-2].fval.typ,yypvt[-0].fval.typ,(double)((long)tx_to_d(yypvt[-2].fval) && (long)tx_to_d(yypvt[-0].fval)));} break;
case 21:
{yyval.fval=d_to_tx(yypvt[-2].fval.typ,yypvt[-0].fval.typ,(double)((long)tx_to_d(yypvt[-2].fval) || (long)tx_to_d(yypvt[-0].fval)));} break;
case 22:
{if((long)tx_to_d(yypvt[-4].fval)) yyval.fval=yypvt[-2].fval; else yyval.fval=yypvt[-0].fval;} break;
case 23:
{yyval.fval=yypvt[-0].fval;} break;
case 24:
{yyval.fval=d_to_t(yypvt[-0].fval.typ,-(tx_to_d(yypvt[-0].fval)));} break;
case 25:
{yyval.fval=d_to_t(yypvt[-0].fval.typ,(double)(!(long)tx_to_d(yypvt[-0].fval)));} break;
case 26:
{yyval.fval=d_to_t(yypvt[-0].fval.typ,(double)(~(long)tx_to_d(yypvt[-0].fval)));} break;
case 27:
{yyval.fval=d_to_t(yypvt[-1].ival,tx_to_d(yypvt[-0].fval));} break;
case 28:
{yyval.fval=yypvt[-1].fval;} break;
case 29:
{yyval.fval=yypvt[-0].fval;} break;
case 30:
{yyval.fval.val.pp=(char *)yypvt[-0].sval; yyval.fval.typ=(STAR_ONE+TYPE_CHAR);} break;
case 31:
{yyval.fval=yypvt[-0].gval.val; yyval.fval.typ=TYPE_PT+STAR_ONE;} break;
case 34:
{user_func(yypvt[-1].nival.net,yypvt[-1].nival.indx); yyval.fval=nsl_func_rr;} break;
case 35:
{push_state(UFFKY); push_uff(yypvt[-1].ival); push_io();
	 set_io(&fun_list[(yypvt[-1].ival)-1].iop); inc_file(io_fnum); yyclearin;} break;
case 36:
{restore_exp(); pop_state(); pop_uff(); dec_file(io_fnum);
	 pop_io(); yyclearin; yyval.fval=nsl_func_rr;} break;
case 37:
{yyval.fval=var_selector(yypvt[-0].fval,zerval,(int)VAR,TRUE);} break;
case 38:
{yyval.fval=var_selector(yypvt[-1].fval,zerval,(int)PLUPLU,TRUE);} break;
case 39:
{yyval.fval=var_selector(yypvt[-1].fval,zerval,(int)MINMIN,TRUE);} break;
case 40:
{yyval.fval=var_selector(yypvt[-2].fval,yypvt[-0].fval,yypvt[-1].ival,FALSE);} break;
case 41:
{yyval.fval=var_selector(yypvt[-2].fval,yypvt[-0].fval,(int)'=',FALSE);} break;
case 42:
{yyval.fval=var_selector(yypvt[-0].fval,zerval,(int)PLUPLU,FALSE);} break;
case 43:
{yyval.fval=var_selector(yypvt[-0].fval,zerval,(int)MINMIN,FALSE);} break;
case 44:
{yyval.fval=yypvt[-0].gval.val;} break;
case 45:
{tmp_pt=tmp_buff; *tmp_pt='\0'; tmp_net=mon_net;} break;
case 46:
{yyval.fval=yypvt[-0].fval;} break;
case 47:
{yyval.fval.typ=nsl_func_rr.typ; yyval.fval.val.pp=(char *)&nsl_func_rr.val;} break;
case 48:
{long hstval,nd=(long)tx_to_d(yypvt[-2].fval); if(nd<1) nd=0L;
	 if(is_set(mon_net))
		{hstval=nsl_num_adrs(mon_net,nd,tmp_buff,&yyval.fval.val.pp,NULL);
		if(nd<1) {yyval.fval.typ=TYPE_LONG; yyval.fval.val.pp=(char *)&hstval;}
		else yyval.fval.typ=(int)hstval;
		}
	 else yyval.fval=zerval;
	} break;
case 49:
{long timval; NSLNSLnet *npt;
	 if(strncmp(tmp_buff,"time.",5)==0)
		{npt=nsl_get_p(tmp_buff+5); if(npt) timval=npt->time; else timval=0L;
		yyval.fval.typ=TYPE_LONG; yyval.fval.val.pp=(char *)&timval;}
	 else if(is_set(tmp_net))
		yyval.fval.typ=(int)nsl_num_adrs(tmp_net,(long)(-1),
			tmp_buff,&yyval.fval.val.pp,NULL);
	 else yyval.fval=zerval;
	} break;
case 54:
{nsl_func_rr=yypvt[-0].fval; push_exp();} break;
case 55:
{yyval.ival=(yypvt[-2].ival|yypvt[-1].ival);} break;
case 58:
{parm_args[0]=(char)(yypvt[-1].ival|parm_type); parm_name=strsave(yypvt[-0].sval);
	 parm_pos=(long)-1; parm_num=0; parm_val=zerval;
	} break;
case 59:
{do_id_func();} break;
case 61:
{yyval.sval=yypvt[-0].gval.name;} break;
case 63:
{parm_val=yypvt[-0].fval;} break;
case 66:
{parm_ofs=get_io_ofs()-1; parm_pos=io_file_pos; parm_const=cmd_const;
	 parm_pat=cmd_pat; parm_exe=cmd_exe; parm_brk=cmd_brk; parm_exit=cmd_exit;
	 parm_entry=yacc_entry; parm_yacc=cmd_yacc;
	 parm_iof=io_fnum; inc_file(parm_iof); skip_to_chr('}',FALSE);
	 yyclearin; yacc_entry=';';} break;
case 71:
{parm_type=yypvt[-0].ival;} break;
case 75:
{enlarge_array(&parm_list,&parm_max,&parm_num,1,8,sizeof(char *));
	 parm_list[parm_num-1]=strsave(yypvt[-0].sval);
	 parm_args[parm_num]=(char)(yypvt[-1].ival|parm_type);
	} break;
case 76:
{yyval.ival=0;/*nothing*/} break;
case 77:
{yyval.ival=STAR_ONE;} break;
case 78:
{yyval.ival=STAR_TWO;} break;
case 79:
{yyval.fval=trueval;} break;
case 83:
{yyval.fval=zerval;} break;
case 84:
{yyval.fval=yypvt[-1].fval;} break;
case 85:
{tmp_pt=fcat(tmp_pt,yypvt[-0].sval);} break;
case 91:
{tmp_pt=fcat(tmp_pt,yypvt[-0].sval);} break;
case 94:
{tmp_pt=fcat(tmp_pt,yypvt[-0].sval);} break;
case 101:
{sprintf(tmp_pt,"%d:%d\0",(int)tx_to_d(yypvt[-2].fval),(int)tx_to_d(yypvt[-0].fval));
	 tmp_pt+=strlen(tmp_pt);
	} break;
case 102:
{tmp_pt=fcat(tmp_pt,yypvt[-0].sval);} break;
case 103:
{tmp_pt=fcat(tmp_pt,yypvt[-0].sval);} break;
case 104:
{sprintf(tmp_pt,":%d\0",(int)tx_to_d(yypvt[-0].fval)); tmp_pt+=strlen(tmp_pt);} break;
case 106:
{cmd_pat=FALSE;} break;
case 107:
{sprintf(tmp_pt,"[%d]\0",(int)tx_to_d(yypvt[-1].fval)); cmd_pat=TRUE;
	 tmp_pt+=strlen(tmp_pt);
	} break;
case 108:
{tmp_pt=fcat(tmp_pt,"[*]\0");} break;
case 109:
{cmd_id=NULL; cmd_exp=(int)tx_to_d(yypvt[-0].fval);} break;
case 110:
{cmd_id=yypvt[-0].sval;} break;
case 111:
{do_path_sep: cmd_pat=TRUE; tmp_pt=fcat(tmp_pt,yypvt[-0].sval);} break;
case 112:
{goto do_path_sep;} break;
case 113:
{goto do_path_sep;} break;
case 124:
{tmp_chr=';'; do_send:
	 switch(tmp_cmd=last_state())
		{case '{': break;
		case CFOR_ONE: do_for_one: pop_state();
		  go_io(0); yacc_entry=CFOR_ONE; yyclearin; break;
		case CWHILE: do_while: go_io(0); yacc_entry=CWHILE; yyclearin; break;
		case CIF: pop_state(); break;
		case UFFKY: if(tmp_chr=='}')
			{nsl_func_rr=zerval; yacc_entry=UFFKY_ONE; yyclearin; break;}
		default: ;
		}
	} break;
case 125:
{push_state('{');} break;
case 126:
{pop_state(); tmp_chr='}'; goto do_send;} break;
case 127:
{push_io(); push_state(CDO_ONE);} break;
case 128:
{push_io();} break;
case 129:
{tmp_val=tx_to_d(yypvt[-1].fval);
	 if(last_state()==CDO_ONE)
		{del_io(1);
		if(tmp_val!=(double)0.0) {go_io(0); yyclearin;}
		else {del_io(1); pop_state();}
		}
	 else
		{if(last_state()!=CWHILE) push_state(CWHILE); else del_io(1);
		if(tmp_val==(double)0.0)
		  {do_while_one: del_io(1); pop_state(); skip_statement(FALSE); yyclearin;}
		}
	} break;
case 130:
{push_io();} break;
case 131:
{push_io(); tmp_val=tx_to_d(yypvt[-1].fval); do_for: skip_to_chr(')',FALSE);
	 if(tmp_val==(double)0.0) {del_io(2); skip_statement(FALSE); yyclearin;}
	 else push_state(CFOR_ONE);
	} break;
case 132:
{go_io(1); yacc_entry=CFOR_TWO; yyclearin;} break;
case 133:
{tmp_val=tx_to_d(yypvt[-1].fval); goto do_for;} break;
case 134:
{if(tx_to_d(yypvt[-1].fval)==(double)0.0) {skip_statement(FALSE); yyclearin;}
	 else push_state(CIF);
	} break;
case 135:
{if(tmp_cmd==CIF) {skip_statement(FALSE); yyclearin;}} break;
case 136:
{tmp_fval=zerval; do_eof: yyclearin;
	 switch(tmp_cmd=pop_to_loop(TRUE))
		{case CCALL: yacc_entry=CCALL_ONE; break;
		case UFFKY: yacc_entry=UFFKY_ONE; break;
		default:
		  if(cmd_brk) cmd_exit=2; /* User Intrpt, reset to stdio */
		  else {init_mon(); cmd_exit=1;}/* runaway error: reset stacks */
		  YYACCEPT;
		}
	 nsl_func_rr=tmp_fval;
	} break;
case 138:
{fcat(tmp_buff,"close");} break;
case 139:
{tmp_cmd=pop_to_loop(FALSE);
	 if(tmp_cmd==CFOR_ONE)
		{pop_state(); go_io(0); tmp_val=(double)0.0; goto do_for;}
	 else if(tmp_cmd==CWHILE) {go_io(0); goto do_while_one;}
	} break;
case 140:
{tmp_cmd=pop_to_loop(FALSE);/*look for loop cmd, or func end & clr to if found */
	 if(tmp_cmd==CFOR_ONE) goto do_for_one;
	 else if(tmp_cmd==CWHILE) goto do_while;
	} break;
case 141:
{tmp_fval=yypvt[-0].fval; goto do_eof;} break;
case 142:
{sprintf(tmp_buff,"%s.nat\0",yypvt[-0].sval);
	 push_farg();
	 get_args(); /* reads in rest of line */
	 save_exp(); push_state(CCALL); push_uff((int)0); push_io();
	 if(add_file(tmp_buff,TRUE)<1)
		{mprintf("unable to open `%s'\n",tmp_buff); goto do_call_one;}
	 yyclearin;
	} break;
case 143:
{do_call_one: pop_farg();
	 restore_exp(); pop_state(); pop_uff(); dec_file(io_fnum); pop_io();
	 yyclearin;
	} break;
case 144:
{save_exp(); par_str="EEE0";} break;
case 145:
{do_step(TRUE);} break;
case 146:
{save_exp(); par_str="0";} break;
case 147:
{do_step(FALSE);} break;
case 148:
{do_help();} break;
case 149:
{do_debug();} break;
case 150:
{w_clear(mon_wnd);} break;
case 151:
{tmp_pt=tmp_buff; *tmp_pt='\0';} break;
case 152:
{do_dir(tmp_buff);} break;
case 153:
{tmp_pt=tmp_buff; *tmp_pt='\0';} break;
case 154:
{do_cd(tmp_buff);} break;
case 155:
{do_cnet(yypvt[-0].sval);} break;
case 156:
{init_mon(); cmd_exit=1; YYACCEPT;} break;
case 157:
{cmd_exit=0; YYACCEPT;} break;
case 158:
{do_log(yypvt[-0].sval,"nog",&logfp);} break;
case 159:
{do_log(yypvt[-0].sval,"nat",&cmdfp);} break;
case 160:
{mprintf("%g\n",tx_to_d(yypvt[-0].fval));} break;
case 161:
{do_fsgo((long)(tx_to_d(yypvt[-0].fval)));} break;
case 162:
{do_fssearch();} break;
case 163:
{do_fsclose();} break;
case 164:
{tmp_pt=tmp_buff; *tmp_pt='\0';} break;
case 165:
{do_fsopen(tmp_buff);} break;
case 166:
{tmp_pt=tmp_buff;*tmp_pt='\0';} break;
case 167:
{do_dump(yypvt[-2].sval,tmp_buff);} break;
case 168:
{save_exp(); par_str="0";} break;
case 169:
{do_gen_func(yypvt[-2].fpval);} break;
case 170:
{save_exp(); par_str="0";} break;
case 171:
{do_driver();} break;
case 172:
{do_load(yypvt[-1].sval,yypvt[-0].sval);} break;
case 173:
{do_save(yypvt[-1].sval,yypvt[-0].sval);} break;
case 174:
{if(cmd_id) do_delete(cmd_id); else do_was(cmd_exp,'k');} break;
case 175:
{if(cmd_id) do_enadis(cmd_id,FALSE); else do_was(cmd_exp,'d');} break;
case 176:
{if(cmd_id) do_enadis(cmd_id,TRUE); else do_was(cmd_exp,'e');} break;
case 177:
{do_show(yypvt[-0].sval);} break;
case 178:
{do_shell(tmp_buff+TMP_OFS);} break;
case 179:
{do_was(0,'s');} break;
case 180:
{do_was(0,'w');} break;
case 181:
{parm_type=yypvt[-0].ival;} break;
    }
  goto yystack; /* stack new state and value */
yyyabort: yyyok=1; yyyaccept: yyys=yyyss;yyyv=yyyvv;
yyerrflag=yyyer;yychar=yyych;yynerrs=yyyne; return(yyyok);
}
