
/* Name: nslyacc2.y   Type: yacc file   Date: 18 May 89
*  NeSeL Network Specification Language
*/
#include "globals2.h"
#include "iolo2.h"
#include "nial2.h"
#include "nsltwo2.h"
#include "pathlo2.h"
#include "nslyacc2.h"

typedef union 
{int ival;
 long lval;
 sielm sival;
 NSLNSLfval fval;
 char *sval;
} YYSTYPE;
# define SEXP 257
# define SVAR 258
# define SARY 259
# define NUMBER 260
# define FUNKY 261
# define PUNKY 262
# define ELMCAST 263
# define DOLVAR 264
# define FIXPATH 265
# define DQSTR 266
# define STRPATH 267
# define EQOP 268
# define DOTDOT 269
# define OROR 270
# define ANDAND 271
# define EQ 272
# define NE 273
# define LE 274
# define GE 275
# define LS 276
# define RS 277
# define PLUPLU 278
# define MINMIN 279
# define UMINUS 280
# define DOLEXC 281
# define DOLSEL 282
# define DOLPAR 283
# define LSTOPR 284
# define DOLXS 285
# define LOOPOPR 286
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
# define YYNPROD 80
# define YYLAST 744
short yyact[]={

  16,  33, 102,  65,  32,  67,  66,  19, 106,  16,
  74,  12,  15,  63,  64,  76,  19,   3,   2,   4,
 117,  15,  57, 113, 104,  75, 121,  59,  60,  61,
 119, 113,  73, 120,  55,  77,  16,  68,  69,  56,
  30,  28,  35,  19, 114,  16,  81, 129,  15,  58,
  72,  33,  19,   6,  38,  71,  38,  15,  33,  36,
  39,  36,  40, 127,  37, 105,  37,  33, 103,  29,
   5,  16, 128,  18,   1,   0,   0,   0,  19,  16,
 112,   0,   0,  15,   0,   0,  19,   0,   0,  82,
   0,  15,   0,  17,  33,   0,   0,   0,  38,  49,
   0,   0,  17,  36,  39, 111,  40, 110,  37,   0,
  38,  49,   0,   0, 116,  36,  39,   0,  40, 118,
  37,  43,   0,  44,  54,   0,   0,   0,   0,  17,
 130,   0,   0,  43,   0,  44,  54, 135,  17,   0,
 137, 123,   0,  38,  49, 133,   0, 109,  36,  39,
   0,  40,  38,  37, 136,  50,   0,  36,  39, 134,
  40,   0,  37, 123,  17,   0,  43,  50,  44,  54,
   0,   0,  17,   0,   0,   0,   0,  16,   0,   0,
   0,   0,   0,   0,  19,  51,   0,   0,   0,  15,
   0,   0,   0,   0,  34,   0,   0,  51,   0,   0,
  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,  31, 106,   0,   0,  20,  14,  79,
  51,  22,  13,  21,   0,   0,  20,  14, 124,   0,
  22,  13,  21, 126,  34,  23,  24,   0,  27,  25,
  26,  34,   0,  80,  23,  24,   0,  27,  25,  26,
  34, 125,   0,  20,  14,  79,   0,  22,  13,  21,
  17,   0,  20,  14,   9,   7,  22,  13,  21,  11,
   0,  23,  24,   0,  27,  25,  26,  34,   0,  80,
  23,  24,   0,  27,  25,  26,   0,  10,  20,  14,
   9,   0,  22,  13,  21,  11,  20,  14,   0,   0,
  22,  13,  21,   0,   0,   0,  23,  24,   0,  27,
  25,  26,   0,  10,  23,  24,   0,  27,  25,  26,
   0,  53,  52,  47,  48,  45,  46,  41,  42,   0,
   0,   0, 115,  53,  52,  47,  48,  45,  46,  41,
  42,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,  38,  49,   0,   0, 107,  36,  39,   0,  40,
   0,  37,   0,   0,   0,   0,  53,  52,  47,  48,
  45,  46,  41,  42,  43,   0,  44,  54,  38,  49,
   0,  41,  42,  36,  39,   0,  40,   0,  37,   0,
   0,   0,   0,   0,  20,   0,   0,   0,  22,   0,
  21,  43,   0,  44,  54,   0,   0,   0,  50,   0,
   0,   0,  23,  24,   0,  27,  25,  26,  38,  49,
   0,   0,   0,  36,  39,   0,  40,   0,  37,   0,
   0,   0,   0,   0,   0,  50,   0,   0,  51,  38,
  49,  43,   0,  44,  36,  39,   0,  40,   0,  37,
   0,  38,  49,   0,   0,   0,  36,  39,   0,  40,
   0,  37,  43,   0,  44,  51,   0,   0,   0,   0,
   0,   0,  38,  49,  43,  50,  44,  36,  39,  38,
  40,   0,  37,   0,  36,  39,   0,  40,   0,  37,
   0,  78,   0,  38,   8,  43,  50,  44,  36,  39,
   0,  40,  43,  37,  44,  51,   0,   0,  50,   0,
   0,  62,   0,   0,   0,   0,  43,   0,  44,   0,
  70,   0,   0,   0,   0,   0,  51,   8,  83,  84,
  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,
  95,  96,  97,  98,  99, 100, 101,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0, 108,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,  53,  52,  47,  48,  45,  46,
  41,  42,   0,   0,   0,   0,   0, 122,   0,   0,
   0,   0,   0,   0,   0,   0,   0, 131,   0,   0,
 132,  53,  52,  47,  48,  45,  46,  41,  42, 122,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,  52,  47,  48,  45,  46,  41,  42,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,  47,  48,  45,  46,  41,  42,
   0,   0,   0,   0,   0,   0,  47,  48,  45,  46,
  41,  42,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,  47,  48,  45,
  46,  41,  42,   0,  47,  48,  45,  46,  41,  42,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
  45,  46,  41,  42 };
short yypact[]={

-240,-1000,  12,   1, -40,-1000,-1000,-1000, 351,  -1,
 144,-1000,-1000,-1000,  -1, 144, 144, 144,-1000,  46,
-1000,-1000,-265,-258,-259, 144, 144,-1000,  46,-1000,
-1000,-1000,-1000, -33, 144,  38,  46,  46,  46,  46,
  46,  46,  46,  46,  46,  46,  46,  46,  46,  46,
  46,  46,  46,  46,  46,-1000, -39,-1000,-1000,-1000,
-1000,-1000, 324,-1000,-1000,  46,-1000,-1000,-1000,-1000,
 106, -90, -33, -13,-1000,-1000,-1000,-1000,  73,  -1,
-1000, -73,-1000,-1000,-1000,-1000,  19,  19,  17,  17,
 115, 115, 115, 115, 466, 466, 452, 445, 424, 412,
 391,  61,-1000, -11,-1000, -24,-1000,-1000, 351,-1000,
-1000, -21,-1000,-1000,   3,  46,-1000,-1000,  46,-1000,
-255,-1000, 351,-1000,  -1, 144,-1000, -24,-1000, -33,
-1000, 351, 351,-1000,-1000,-1000,-1000,-1000 };
short yypgo[]={

   0,  74, 501,  11,  73,  70,  69,  34,  68,  24,
  65,  26,  63,  35,  55,  50,  32,  10,  47,  44,
  15,  53,  42 };
short yyr1[]={

   0,   1,   1,   1,   2,   2,   2,   2,   2,   2,
   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,
   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,
   3,   3,   3,   3,   3,   3,   3,   3,   3,   7,
   7,   8,   8,  10,   9,  12,   9,  11,  11,  11,
  11,  11,   4,   4,   4,   6,  14,   6,  15,   6,
  13,  13,  16,  18,  16,  19,  17,  17,  20,  20,
  20,  20,  20,   5,  22,   5,  21,  21,  21,  21 };
short yyr2[]={

   0,   2,   4,   2,   3,   3,   3,   3,   3,   3,
   3,   3,   3,   3,   3,   3,   3,   3,   3,   3,
   3,   3,   5,   1,   1,   2,   2,   2,   2,   1,
   3,   1,   1,   1,   2,   2,   3,   2,   2,   2,
   3,   1,   3,   0,   2,   0,   3,   1,   1,   2,
   2,   1,   2,   2,   1,   1,   0,   3,   0,   4,
   3,   3,   1,   0,   4,   0,   3,   1,   1,   1,
   3,   2,   1,   1,   0,   3,   1,   2,   2,   1 };
short yychk[]={

-1000,  -1, 258, 257, 259,  -5, -21, 263,  -2, 262,
 285, 267,  -3, 265, 261,  45,  33, 126,  -4,  40,
 260, 266, 264, 278, 279, 282, 283, 281,  40,  -6,
 -13, 263,  44,  91, 284, -22,  42,  47,  37,  43,
  45, 276, 277,  60,  62, 274, 275, 272, 273,  38,
  94, 124, 271, 270,  63,  -7,  40,  -3,  -7,  -3,
  -3,  -3,  -2, 278, 279, 268, 264, 264,  -3,  -3,
  -2, -14, -15, -16, -17,  58, -20, -13,  -2, 262,
 286,  -3, -21,  -2,  -2,  -2,  -2,  -2,  -2,  -2,
  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,  -2,
  -2,  -2,  41,  -8,  -9, -10, 263,  41,  -2,  41,
 -13, -16,  93,  44, -19, 269,  -7,  93,  58,  41,
  44, -11,  -2, -13, 262, 285, 267, -12,  93, -18,
 -20,  -2,  -2,  -9,  -7,  -3, -11, -17 };
short yydef[]={

   0,  -2,   0,   0,   0,   1,  73,  74,  76,   0,
   0,  79,  23,  24,   0,   0,   0,   0,  29,   0,
  31,  32,  33,   0,   0,   0,   0,  54,   0,   3,
  55,  56,  58,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
   0,   0,   0,   0,   0,  77,  43,  78,  25,  26,
  27,  28,   0,  34,  35,   0,  37,  38,  52,  53,
   0,   0,   0,   0,  62,  65,  67,  68,  69,   0,
  72,   0,  75,   4,   5,   6,   7,   8,   9,  10,
  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,
  21,   0,  39,   0,  41,   0,  45,  30,  36,   2,
  57,   0,  60,  63,   0,   0,  71,  61,   0,  40,
  43,  44,  47,  48,   0,   0,  51,   0,  59,   0,
  66,  70,  22,  42,  49,  50,  46,  64 };
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
{YYACCEPT;} break;
case 2:
{exp_value.val.dd=tx_to_d(yypvt[-1].fval); exp_value.typ=TYPE_DOUBLE; YYACCEPT;} break;
case 3:
{YYACCEPT;} break;
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
	   yyval.fval=d_to_t((mile)TYPE_LONG,tx_to_d(yypvt[-2].fval)-tx_to_d(yypvt[-0].fval));
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
{pathsel_val(yypvt[-0].sval); yyval.fval=exp_value;} break;
case 25:
{user_func(yypvt[-1].sival.indx); yyval.fval=nsl_func_rr;} break;
case 26:
{yyval.fval=d_to_t(yypvt[-0].fval.typ,-(tx_to_d(yypvt[-0].fval)));} break;
case 27:
{yyval.fval=d_to_t(yypvt[-0].fval.typ,(double)(!(long)tx_to_d(yypvt[-0].fval)));} break;
case 28:
{yyval.fval=d_to_t(yypvt[-0].fval.typ,(double)(~(long)tx_to_d(yypvt[-0].fval)));} break;
case 30:
{yyval.fval=yypvt[-1].fval;} break;
case 31:
{yyval.fval=yypvt[-0].fval;} break;
case 32:
{yyval.fval.val.pp=(char *)yypvt[-0].sval; yyval.fval.typ=(STAR_ONE+TYPE_CHAR);} break;
case 33:
{yyval.fval=var_selector(yypvt[-0].sval,(int)DOLVAR,zerval,T);} break;
case 34:
{yyval.fval=var_selector(yypvt[-1].sval,(int)PLUPLU,zerval,T);} break;
case 35:
{yyval.fval=var_selector(yypvt[-1].sval,(int)MINMIN,zerval,T);} break;
case 36:
{yyval.fval=var_selector(yypvt[-2].sval,yypvt[-1].ival,yypvt[-0].fval,F);} break;
case 37:
{yyval.fval=var_selector(yypvt[-0].sval,(int)PLUPLU,zerval,F);} break;
case 38:
{yyval.fval=var_selector(yypvt[-0].sval,(int)MINMIN,zerval,F);} break;
case 43:
{cur_cast=STAR_PAIR;} break;
case 45:
{cur_cast=yypvt[-0].ival;} break;
case 47:
{nsl_func_rr=yypvt[-0].fval; push_exp(cur_cast);} break;
case 48:
{/* a lst_elm should be on stack already to take new list */
	 if(!lst_ok(lst_stk)) signal_error(EXIT_INT_ERR,154);
	 nsl_func_rr.val.pp=(char *)(*lst_tail(lst_stk,char *));
	 nsl_func_rr.typ=TYPE_PT;
	 push_exp((mile)0);
	 add_lst_stk(); /* create a new hole in case elist contains another list */
	} break;
case 49:
{user_func(yypvt[-1].sival.indx); push_exp(cur_cast); /* assume lst func, unless cast */
	 if(cur_cast!=STAR_ISPT) /* a list rather than a simple pt */
	   {if(!lst_ok(lst_stk)) signal_error(EXIT_INT_ERR,155);
	    /*push lst details on lst_stk for deling later*/
	    push_lst(nsl_func_rr.val.pp);
	   }
	} break;
case 50:
{nsl_func_rr.val.pp=path_selector((int)yypvt[-1].sival.sel,(int)yypvt[-1].sival.indx,(int)tx_to_d(yypvt[-0].fval));
	 nsl_func_rr.typ=TYPE_PT; push_exp((mile)0);} break;
case 51:
{pathsel_val(yypvt[-0].sval); nsl_func_rr=exp_value; push_exp((mile)0);} break;
case 52:
{yyval.fval.val.ii=(int)selector_val((int)yypvt[-1].sival.sel,
	    (int)yypvt[-1].sival.indx,(int)tx_to_d(yypvt[-0].fval),cur_cast);
	 yyval.fval.typ=TYPE_INT;
	 if(yyary_pt->suspend) YYACCEPT;} break;
case 53:
{yyval.fval.val.ii=(int)part_selector((int)yypvt[-1].sival.indx,(int)tx_to_d(yypvt[-0].fval),T);
	 yyval.fval.typ=TYPE_INT;
	 if(yyary_pt->suspend) YYACCEPT;} break;
case 54:
{yyval.fval.val.ii=(int)exc_selector((int)yypvt[-0].ival);
	 yyval.fval.typ=TYPE_INT;
	 if(yyary_pt->suspend) YYACCEPT;} break;
case 56:
{cur_cast=yypvt[-0].ival;} break;
case 58:
{if(yyary_pt->ary_list) cur_cast=lst_typ(yyary_pt->ary_list);} break;
case 61:
{list_operator((int)yypvt[-2].sival.sel,(int)yypvt[-2].sival.indx,(int)tx_to_d(yypvt[-1].fval),cur_cast);
	 if(!infunc && !list_miss && yyary_pt->ary_list) list_add=T;
	 if(yyary_pt->suspend) YYACCEPT;
	} break;
case 63:
{if(!infunc) {seppt=get_io_pos()-1; sepstate=list_type;}} break;
case 65:
{++list_miss;} break;
case 66:
{--list_miss;} break;
case 69:
{lst_ary_list((reals)tx_to_d(yypvt[-0].fval),(reals)0.0,cur_cast,T);
	 if(!infunc && !list_miss && yyary_pt->ary_list) list_add=T;} break;
case 70:
{lst_ary_list((reals)tx_to_d(yypvt[-2].fval),(reals)tx_to_d(yypvt[-0].fval),cur_cast,F);
	 if(!infunc && !list_miss && yyary_pt->ary_list) list_add=T;} break;
case 71:
{user_func(yypvt[-1].sival.indx);
	 /* add list in nsl_func_rr.pp to cur list */
	 lst_ary_ary(nsl_func_rr.val.pp,F,cur_cast);
	 lfree(nsl_func_rr.val.pp);
	 if(!infunc && !list_miss && yyary_pt->ary_list) list_add=T;
	} break;
case 72:
{list_type=yypvt[-0].ival; YYACCEPT;} break;
case 74:
{cur_cast=yypvt[-0].ival;} break;
case 76:
{if(yypvt[-0].fval.typ==TYPE_PT||(yypvt[-0].fval.typ&STAR_MASK)) exp_value=yypvt[-0].fval;
	 else {exp_value.val.dd=tx_to_d(yypvt[-0].fval); exp_value.typ=TYPE_DOUBLE;}} break;
case 77:
{user_func(yypvt[-1].sival.indx); exp_value=nsl_func_rr;} break;
case 78:
{exp_value.val.pp=path_selector((int)yypvt[-1].sival.sel,(int)yypvt[-1].sival.indx,(int)tx_to_d(yypvt[-0].fval));
	 exp_value.typ=(STAR_ONE+TYPE_CHAR);} break;
case 79:
{pathsel_val(yypvt[-0].sval);} break;
    }
  goto yystack; /* stack new state and value */
yyyabort: yyyok=1; yyyaccept: yyys=yyyss;yyyv=yyyvv;
yyerrflag=yyyer;yychar=yyych;yynerrs=yyyne; return(yyyok);
}
