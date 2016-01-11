// Name: nslwsio.c   Type: NeSeL Win Monitor    Date: December 1996

#include "stdafx.h"
#include <math.h>
#include "NSLWnd.h"
#include "nslms.h"
#include "nslwsio.h"

#define NIO_MARGINS 12
#define w_gethd(w) ((w)<0?(-w):(w))

typedef struct _wnd_track
{ int obj_typ; /* object type: free, chr, pixel, image */
  BOOL demal, /* demalloc when erasing track */
	hide, /* track is disabled */
	active; /* object is on screen */
  char chr, /* object chr, or... */
	*buff; /* pt to image */
  int x,y; /* object position */
  int bc,fc; /* object background & foreground colors */
} wnd_track;
/* A shared wnd has this much common info */
typedef struct _wnd_com
{ char *name;
  BOOL
	frame, /* has frame */
	scale, /* adjust coors for aspect ratio */
	wrap, /* wrap chrs at eol */
	share, /* window is shared by two or more processes */
	ismore, /* <more> prompt at end of wnd */
	isdump; /* wnd is to be dumped to file */
  int
	ulist, /* hd of wnd_elm users list */
	fbc,ffc, /* frame colors */
	tx,ty, /* true outer width in chrs */
	cx,cy, /* virtual inner width in chrs (non-scaled) */
	px,py; /* virtual dimension in pixels */
  CNSLWnd *pWnd;
  FILE *dumfp; /* dump file */
  char **dp; /* pt to dump image */
  char *margins[NIO_MARGINS];
  BOOL mars[NIO_MARGINS];
} wnd_com;
/* The following info is private to each wopen called by a process */
typedef struct _wnd_elm
{ BOOL
	inuse, /* indicates if this is an open window */
	ischr, /* pen units in chrs or pixels */
	ispen; /* turtle pen up or down */
  int isfill, /* object fill state */
	com, /* common info index */
	curx,cury, /* cur position */
	angle, /* current turtle angle */
	borderfc, /* border foreground color */
	fc,bc,pw, /* current text & pixel foreground & background colors, penwidth */
	ulist; /* share tail pt */
  wnd_track *tracks; /* objects being tracked in window */
} wnd_elm;

wnd_com *wnd_com_list=NULL;
wnd_elm *wnd_list=NULL;

extern int 
  w_exit_code=0,w_exit_called=0,
  wnd_num_colors=StdNumColors,
  wnd_white=StdColorWhite,wnd_black=StdColorBlack,
  wnd_std=0,wnd_all=NIO_DEFAULT;

int
  wnd_bc=0,wnd_fc=0;

/* index codes for window frame corners */
#define BDTL 0
#define BDTR 1
#define BDBR 2
#define BDBL 3
#define BDVS 4
#define BDHS 5

#if 1
unsigned char wnd_frame_bd[]={218,191,217,192,179,196};
unsigned char wnd_scale_bd[]={201,187,188,200,186,205};
#else
unsigned char wnd_frame_bd[]={'/','\\','/','\\','|','-'};
unsigned char wnd_scale_bd[]={'/','\\','/','\\','|','-'};
#endif

char out_buff[buffsize];

/*************************************************************************/
/* Exception handler */
#include <signal.h>
#include <setjmp.h>
#include <float.h>
#include <math.h>

jmp_buf w_setjmp_mark;  /* Address for long jump to jump to */
int     fperr;             /* Global error number */
int w_setjmp_ok=0;

void __cdecl w_fphandler( int sig, int num );   /* Prototypes */
void w_fpcheck( void );

void w_fphandler( int sig, int num )
{	fperr = num;
	_fpreset();
	if(w_setjmp_ok)	longjmp(w_setjmp_mark, -1 );
}
void w_fpcheck()
{  char fpstr[30];
   switch( fperr )
   {case _FPE_INVALID: strcpy( fpstr, "Invalid number" ); break;
	case _FPE_OVERFLOW: strcpy( fpstr, "Overflow" ); break;
    case _FPE_UNDERFLOW: strcpy( fpstr, "Underflow" ); break;
    case _FPE_ZERODIVIDE: strcpy( fpstr, "Divide by zero" ); break;
    default: strcpy( fpstr, "Floating point error" ); break;
   }
   w_printf(wnd_std,"Error %d: %s\n", fperr, fpstr );
}

int w_setjmp()
{	w_setjmp_ok=1;
	return(setjmp(w_setjmp_mark));
}

void w_exit(int retcode)
{	w_exit_code=retcode; w_exit_called=1;
	if(w_setjmp_ok)	longjmp(w_setjmp_mark, -1 );
	throw("w_exit");
}

/******************************************************************************/

void dump_clr_line(wnd_com *wc,int nn)
{ char *ap=wc->dp[nn];
  nn=wc->tx; while(nn--) {*ap=' '; ++ap;} *ap='\0';
}
void dumpxy(wnd_com *wc,int x,int y,char chr)
{ int nn;
  if(!wc->dp)
	{wc->dp=lmore(NULL,wc->ty,TYPE_PT,char *);
	 nn=wc->ty;
	 while(nn--)
		{wc->dp[nn]=lmore(NULL,1+wc->tx,TYPE_CHAR,char); dump_clr_line(wc,nn);}
	}
  if(x<1||y<1||x>wc->tx||y>wc->ty) return;
  if(chr) wc->dp[y-1][x-1]=chr;
}
void dump_scroll(wnd_com *wc)
{ char *ap; int nn;
  if(!wc->dp) return;
  ap=wc->dp[0];
  if(wc->dumfp) fprintf(wc->dumfp,"%s\n",ap); /* dump scrolled line */
  for(nn=0;nn<wc->ty;++nn) wc->dp[nn]=wc->dp[nn+1];
  wc->dp[wc->ty-1]=ap;
  dump_clr_line(wc,wc->ty-1);
}
void dump_clear(wnd_com *wc)
{ int nn;
  if(!wc->dp) return;
  for(nn=0;nn<wc->ty;++nn) dump_clr_line(wc,nn);
}
BOOL dump_blk(wnd_com *wc, int nn)
{ int xx;
  for(xx=0;xx<wc->tx;++xx) {if(wc->dp[nn][xx]!=' ') return(FALSE);}
  return(TRUE);
}
void dump_file(wnd_com *wc)
{ int fir,las,nn;
  if(!wc->isdump||!wc->dumfp) return;
  fprintf(wc->dumfp,"Window:%s\n",wc->name);
  for(nn=0;nn<wc->ty;++nn) {if(!dump_blk(wc,nn)) break;} fir=nn;
  for(nn=wc->ty-1;nn>=0;--nn) {if(!dump_blk(wc,nn)) break;} las=nn;
  for(nn=fir;nn<=las;++nn) fprintf(wc->dumfp,"%s\n",wc->dp[nn]);
}

void dump_close(wnd_com *wc)
{ wc->isdump=FALSE;
  if(wc->dp)
	{int kk;
	 for(kk=0;kk<wc->ty;++kk) lfree(wc->dp[kk]);
	 lfree(wc->dp); wc->dp=NULL;}
  if(wc->dumfp) {fclose(wc->dumfp); wc->dumfp=NULL;}
}

void w_scrollup(wnd_com *wc)
{	if(!wc||!wc->pWnd) return;
	if(wc->isdump) dump_scroll(wc);
	wc->pWnd->G_scrolltext();
	wc->pWnd->Update();
}

void w_xychr(wnd_com *wc,int x, int y,unsigned char chr)
{ 	wc->pWnd->SetTextPos(y,x);
	wc->pWnd->PutChar((int)chr,wnd_fc,wnd_bc);
  if(wc->isdump) dumpxy(wc,x,y,chr);
}

void w_select(wnd_elm *wp,BOOL istext, BOOL iscolor)
{ wnd_com *wc=wnd_com_list+wp->com;
  if(istext)
	{if(iscolor)
		{wnd_bc=wp->bc; wnd_fc=wp->fc;
		 wc->pWnd->SetTextPos(wc->frame+wp->cury,wc->frame+wp->curx);}
	}
  else
	{if(iscolor) 
		{wc->pWnd->SetColor(wp->fc); 
		 wc->pWnd->SetGrafPos(wp->curx,wp->cury);
		 wc->pWnd->SetPenWidth(wp->pw);
		} 
    }
}

void zaptrks(wnd_track *aa)
{ int nn=lst_ok(aa);
  while(nn--) {aa->active=FALSE; ++aa;}
}

void clrtrks(int w)
{ wnd_elm *wp; wnd_com *wc; int  uhd;
  w=w_gethd(w);
  wp=wnd_list+w;
  zaptrks(wp->tracks);
  wc=wnd_com_list+wp->com;
  if(wc->share)
	{uhd=wc->ulist;
	 while(uhd>=0)
		{wp=wnd_list+uhd;
		if(uhd!=w) zaptrks(wp->tracks);
		uhd=wp->ulist;
		}
	}
}

void w_clear_box(int w,int x,int y,int lx,int ly)
{ wnd_elm *wp; wnd_com *wc; int nn;
  if(!wnd_list){w=0;w_init();}
  w=w_gethd(w);
  wp=wnd_list+w; wc=wnd_com_list+wp->com;
  wnd_bc=wp->bc; wnd_fc=wp->fc;
  while(ly--)
	{nn=lx;
	 while(nn--) w_xychr(wc,x+nn,y,(unsigned char)' ');
	 ++y;
	}
}

void w_blank(int w, BOOL frame)
{ wnd_elm *wp; wnd_com *wc; 
  if(!wnd_list){w=0;w_init();}
  w=w_gethd(w);
  wp=wnd_list+w; wc=wnd_com_list+wp->com;
  wc->pWnd->SetBackgroundColor(wp->bc);
  if(wp->ischr) {wp->curx=1; wp->cury=1;} else {wp->cury=0; wp->curx=0;}
  clrtrks(w);
  wc->pWnd->WndClear(frame&&wc->frame);
}

void w_raw(wnd_elm *wp, wnd_com *wc, char chr)
{ switch(chr)
	{case '\n': goto cret;
	 case '\f': break;
	 case '\r': wp->curx=1; break;
	 case '\b': if(wp->curx>1) --wp->curx;
		else if(wp->cury>1&&wc->wrap) {wp->curx=wc->cx; --wp->cury;} break;
	 case '\t': chr=' ';
	 default: wc->pWnd->PutChar((int)chr,wnd_fc,wnd_bc);
		if(wc->isdump) dumpxy(wc,wc->frame+wp->curx,wc->frame+wp->cury,chr);
		++wp->curx;
		if(wp->curx>wc->cx)
		 {if(!wc->wrap) --wp->curx;
		  else
			{cret: if(wp->cury>=wc->cy) w_scrollup(wc); else ++wp->cury;
			 wp->curx=1;
			}
		 }
	}
 wc->pWnd->SetTextPos(wc->frame+wp->cury,wc->frame+wp->curx);
}

void do_more(int w)
{ char chr;
  w=w_gethd(w);
  if(w_y(w_position(w,NIO_DEFAULT,NIO_DEFAULT))>=(w_y(w_size(w,TRUE))/*-1*/))
	{w_puts(w,"--More?--");
	 chr=(char)w_getch(w); w_clear_line(w);
	 if(chr=='q'||chr=='e') wnd_com_list[w].ismore=FALSE;
	 else if(chr!=' ') w_blank(w,FALSE);
	}
}

void w_puts(int w,char *s)
{ wnd_elm *wp; wnd_com *wc; char chr,las='\0'; 
  int IsNoUpdate=(int)(w<0); if(IsNoUpdate) w=(-w);
  if(!wnd_list){w=0;w_init();}
  wp=wnd_list+w; wc=wnd_com_list+wp->com;
  w_select(wp,TRUE,TRUE); 
  while(chr=(*s)) {las=chr; w_raw(wp,wc,chr); ++s;}
  if(wc->ismore&&(las=='\n')) do_more(w);
  else if(!IsNoUpdate) w_update(w);
}

int w_putc(int w, char c)
{ char buff[2];
  *buff=c; buff[1]='\0'; w_puts(w,buff);
  return((int)c);
}

int w_track(int w,int cmd,...);

/* erase/close track, or all tracks if t=NIO_DEFAULT */
void do_erase(int w,int t, BOOL hide)
{ wnd_track *bp,*tp; int nn,tk;
  if(w<0) w=(-w);
  tp=wnd_list[w].tracks;
  if(tp)
	{nn=lst_num(tp); if(t>=nn&&t!=NIO_DEFAULT) return;
	 bp=tp; if(t!=NIO_DEFAULT) {tk=t; tp+=t; nn=1;} else tk=0;
	 while(nn--)
		{if(tp->obj_typ!=NIO_FREE)
		  {if(hide) w_track(w,NIO_HIDE,tk);
			if(tp->demal&&tp->buff) {lfree(tp->buff); tp->buff=NULL;}
			if(t!=NIO_DEFAULT) tp->obj_typ=NIO_FREE;
		  }
		++tp; ++tk;
		}
	 if(t==NIO_DEFAULT) {lfree(bp); wnd_list[w].tracks=NULL;}
	}
}
/* close a window, or all if w=NIO_DEFAULT */
void w_close(int w)
{ wnd_elm *wp; wnd_com *wc; int nn,kk,uhd,lhd,hd;
  w=w_gethd(w);
  if(wnd_list)
	{nn=lst_num(wnd_list);
	 if(w==0||(w>=nn&&w!=NIO_DEFAULT)) return;
	 wp=wnd_list;
	 if(w!=NIO_DEFAULT) {wp+=w; nn=1; hd=w;} else hd=0;
	 while(nn--)
		{if(wp->inuse)
		   {do_erase(hd,NIO_DEFAULT,TRUE);
			wc=wnd_com_list+wp->com;
			if(wc->share)
				{uhd=wc->ulist; lhd=uhd;
				 if(uhd==hd) wc->ulist=wp->ulist;
				 else
					{while((uhd>=0)&&(uhd!=hd))
							{lhd=uhd; uhd=wnd_list[uhd].ulist;}
					 wnd_list[lhd].ulist=wp->ulist;
					}
				 if(wc->ulist==(-1)) wc->share=FALSE;
				}
			if(!wc->share)
				{wc->pWnd->WndClose(); wc->pWnd=NULL;
				 dump_close(wc);
				 for(kk=0;kk<NIO_MARGINS;++kk)
					{if(wc->margins[kk])
				  		{if(wc->mars[kk]) lfree(wc->margins[kk]);
						 wc->margins[kk]=NULL;
						}
					}
				 lfree(wc->name); wc->name=NULL;
				}
			wp->inuse=FALSE;
		   }
		 ++wp; ++hd;
		}
	}
  if(w==NIO_DEFAULT)
	{if(wnd_list) {lfree(wnd_list); wnd_list=NULL;}
	 if(wnd_com_list) {lfree(wnd_com_list); wnd_com_list=NULL;}
	}
}

/* write title at pos, nn=len, get chrs from sp if set */
void do_title(int w,int pos,int nn,char *sp)
{ wnd_elm *wp; wnd_com *wc; unsigned char *bp,chr,nchr;
  int fr,nx,ny,xlen,ylen,x=1,y=1,ix=0,iy=0;
  if(!wnd_list){w=0;w_init();}
  w=w_gethd(w);
  wp=wnd_list+w; wc=wnd_com_list+wp->com;
  if(wc->scale) bp=wnd_scale_bd; else bp=wnd_frame_bd;
  fr=wc->frame; xlen=wc->cx; ylen=wc->cy;
  if(nn>xlen) nx=xlen; else nx=nn; if(nn>ylen) ny=ylen; else ny=nn;
  switch(pos)
	  {case NIO_BR: y=2*fr+ylen;
		case NIO_TR: x=fr+xlen-nx; goto do_a_x;
		case NIO_BC: y=2*fr+ylen;
		case NIO_TC: x=(1+xlen-nx)/2; goto do_a_x;
		case NIO_BL: y=2*fr+ylen;
		case NIO_TL: 
		 do_a_x: x+=fr; nn=nx; ix=1; chr=bp[BDHS]; break;
		case NIO_RC: x=2*fr+xlen;
		case NIO_LC: y=(1+ylen-ny)/2; goto do_a_y;
		case NIO_RB: x=2*fr+xlen;
		case NIO_LB: y=fr+ylen-ny; goto do_a_y;
		case NIO_RT: x=2*fr+xlen;
		case NIO_LT:
		 do_a_y: y+=fr; nn=ny; iy=1; chr=bp[BDVS]; break;
	  }
  while(nn--)
	{if(sp&&*sp) nchr=(unsigned char)(*sp++); else nchr=chr;
	 w_xychr(wc,x,y,nchr); x+=ix; y+=iy;}
}

/* annotate box with title in margin at pos. (w,pos,cmd,[[demal,sp]])
*  if cmd=NIO_FREE, frees current title str & redraws margin.
*  if cmd=NIO_REDRAW, draws current title,
*  if cmd==NIO_DRAW, if sp is set it becomes new title, draws title.
*  demal indicates if new str should be freed on wclose etc.
*  returns current title.
*/
char *w_title(int w,int pos,int cmd,...)
{ wnd_elm *wp; wnd_com *wc; char *ap; int jj; BOOL demal=FALSE;
  char *sp=NULL; va_list args; va_start(args,cmd);
  w=w_gethd(w);
  if(!wnd_list){w=0;w_init();} wp=wnd_list+w;
  wc=wnd_com_list+wp->com;
  wnd_bc=wc->fbc; wnd_fc=wc->ffc;
  if(pos<0||pos>=NIO_MARGINS) {sp=NULL; pos=0; jj=NIO_MARGINS;}
  else
  	{jj=pos+1;
	 if(cmd==NIO_DRAW)
		{demal=(BOOL)va_arg(args,int); sp=va_arg(args,char *);}
	 else if(cmd==NIO_REDRAW) cmd=NIO_DRAW;
	}
  while(pos<jj)
	{ap=wc->margins[pos];
	 if(ap) /* erase old margin */
		{if(*ap) do_title(w,pos,strlen(ap),(char *)NULL);
		if(cmd==NIO_FREE||sp)
		  {if(wc->mars[pos]) lfree(ap); wc->margins[pos]=NULL;}
		}
	 if(cmd==NIO_DRAW)
		{if(sp) {wc->margins[pos]=sp; wc->mars[pos]=demal; ap=sp;}
		if(ap&&*ap) do_title(w,pos,strlen(ap),ap);
		}
	 ++pos;
	}
  va_end(args);
  return(wc->margins[jj-1]);
}

/* dump a window to file etc */
void w_dump(int w,int cmd,...)
{ wnd_elm *wp; wnd_com *wc; va_list args; char *name; int nn;
  va_start(args,cmd);
  if(!wnd_list) goto byebye;
  w=w_gethd(w);
  nn=lst_num(wnd_list);
  if(w>=nn&&w!=NIO_DEFAULT) goto byebye;
  wp=wnd_list;
  if(w!=NIO_DEFAULT) {wp+=w; nn=1;} else w=0;
  while(nn--)
	{wc=wnd_com_list+wp->com;
	switch(cmd)
	  {case NIO_DP_CLOSE: /*()*/
		 dump_close(wc); break;
		case NIO_DP_OPEN: /*(char *name)*/
		 name=va_arg(args,char *);
		 dump_close(wc);
		 if(name) wc->dumfp=fopen(name,"w+");
		 wc->isdump=(BOOL)(wc->dumfp!=NULL);
		 w_clear(w);
		 break;
		case NIO_DP_DUMP: /*()*/
		 dump_file(wc); break;
		default:;
	  }
	++wp; ++w;
	}
  byebye: va_end(args);
}

void w_clear(int w)
{ wnd_elm *wp; wnd_com *wc; unsigned char *bp; int nn;
  if(!wnd_list){w=0;w_init();}
  w=w_gethd(w);
  wp=wnd_list+w; wc=wnd_com_list+wp->com;
  wc->pWnd->SetBackgroundColor(wp->bc);
  wc->pWnd->WndClear(TRUE);
  if(wp->ischr) {wp->curx=1; wp->cury=1;} else {wp->cury=0; wp->curx=0;}
  if(wc->frame)
	{if(wc->scale) bp=wnd_scale_bd; else bp=wnd_frame_bd;
	 wnd_bc=wc->fbc; wnd_fc=wc->ffc;
	 for(nn=2;nn<wc->ty;++nn)
		{w_xychr(wc,1,nn,bp[BDVS]); w_xychr(wc,wc->tx,nn,bp[BDVS]);}
	 for(nn=2;nn<wc->tx;++nn)
		{w_xychr(wc,nn,1,bp[BDHS]); w_xychr(wc,nn,wc->ty,bp[BDHS]);}
	 w_xychr(wc,1,1,bp[BDTL]); w_xychr(wc,wc->tx,1,bp[BDTR]);
	 w_xychr(wc,1,wc->ty,bp[BDBL]); w_xychr(wc,wc->tx,wc->ty,bp[BDBR]);
	}
  w_title(w,NIO_DEFAULT,NIO_DRAW);
  clrtrks(w);
  wc->pWnd->Update();
}

void w_clear_line(int w)
{ wnd_elm *wp; wnd_com *wc; int ow;
  if(!wnd_list){w=0;w_init();}
  ow=w_gethd(w);
  wp=wnd_list+ow; wc=wnd_com_list+wp->com;
  wp->curx=1;
  w_clear_box(w,1+wc->frame,wc->frame+wp->cury,wc->cx,1);
}

void w_reset_private(wnd_elm *wp)
{ wp->tracks=NULL; wp->inuse=TRUE; 
  wp->ispen=TRUE; wp->ischr=TRUE;
  wp->isfill=FALSE; wp->angle=0; wp->com=0;
  wp->borderfc=wnd_black; wp->fc=wnd_black; wp->bc=wnd_white;
  wp->pw=StdPenWidth;
  wp->curx=1; wp->cury=1; wp->ulist=(-1);
}

void w_reset_common(wnd_com *wc)
{ int nn;
  for(nn=0;nn<NIO_MARGINS;++nn) {wc->margins[nn]=NULL; wc->mars[nn]=FALSE;}
  wc->name=NULL; wc->dp=NULL; wc->isdump=FALSE; wc->dumfp=NULL;
  wc->frame=FALSE; wc->share=TRUE; wc->scale=FALSE; wc->wrap=TRUE; wc->ismore=FALSE;
  wc->ulist=(-1);
  wc->cx=0; wc->cy=0;
  wc->tx=wc->cx; wc->ty=wc->cy; 
  wc->px=0; wc->py=0;
  wc->ffc=wnd_black; wc->fbc=wnd_white;
}

/* initialise windows, mode etc */
int w_init_guard=0;
void w_init()
{ if(wnd_list) return;
  if(w_init_guard++) return;
  wnd_std=0;
  wnd_num_colors=StdNumColors; wnd_white=StdColorWhite;
  wnd_std=w_open("stdwnd",TRUE,FALSE,FALSE,TRUE,FALSE,0,0,wnd_white,wnd_black);

 /* _controlfp( 0, _MCW_EM );*/
  signal( SIGFPE,(void (__cdecl *)(int))w_fphandler );

  w_init_guard=0;
}

int w_handle(char *name)
{ wnd_com *wc=wnd_com_list; int nn=lst_ok(wnd_com_list);
  while(nn--)
	{if(strcmp(wc->name,name)==0) return(wc->ulist);
	++wc;
	}
  return(-1);
}

/* open an io window, returns handle if ok, or zero */
int w_open(char *name, BOOL share,BOOL frame,BOOL scale,
			BOOL ischr,BOOL iswrap, 
			int lx,int ly,int fc,int bc)
{ wnd_elm *wp; wnd_com *wc; int u,w,nn;
  if(!wnd_list)w_init(); /* first time call so initialise */
  if(!name) return(0);
  /* make a new slot for wnd details */
  wp=wnd_list; w=0; nn=lst_ok(wnd_list);
  while(nn--) {if(!wp->inuse) goto gotwnd; ++wp; ++w;}
  w=lst_ok(wnd_list);
  wnd_list=lmore(wnd_list,1,TYPE_STRUCT,wnd_elm);
  wp=wnd_list+w;
  gotwnd: w_reset_private(wp);
  wc=wnd_com_list; nn=lst_ok(wnd_com_list); u=nn; nn=0;
  /* see if shared wnd already open */
  if(share)
	{while(nn<u)
		{if(wc->name&&wc->share&&(strcmp(wc->name,name)==0))
		  {wp->ulist=wc->ulist; wc->ulist=w; wp->com=nn; return(w);}
		++wc; ++nn;
		}
	}
  /* either wnd not shared, or first open on a shared wnd */
  /* set up common block */
  wc=wnd_com_list; nn=u;
  u=0; while(nn--) {if(!wc->name) goto gotcom; ++wc; ++u;}
  u=lst_ok(wnd_com_list);
  wnd_com_list=lmore(wnd_com_list,1,TYPE_STRUCT,wnd_com);
  wc=wnd_com_list+u;
  gotcom: w_reset_common(wc);
  wp->com=u; wc->ulist=w; if(frame) {frame=1; wc->ffc=fc; wc->fbc=bc;}
  wc->share=share; wc->frame=frame; wc->wrap=iswrap;
  wc->name=strsave(name);

  nn=1+2*frame;
  /* get window size */
	if(ischr)
	{	if(lx<nn||ly<nn) {lx=StdWndCharWidth; ly=StdWndCharHeight;}
		wc->cx=lx; wc->cy=ly; wc->px=0; wc->py=0;
		
		if(frame) {lx+=2; ly+=2;} /* allow for border */
		wc->tx=lx; wc->ty=ly;

		wc->pWnd=NSLWndOpen(wc->name,0,0,lx,ly);
		if(!wc->pWnd) {lfree(wc->name); return(0);}

		wc->px=wc->pWnd->xsize; wc->py=wc->pWnd->ysize;
		if(frame) {wc->px-=(2*wc->pWnd->m_font_width);
					wc->py-=(2*wc->pWnd->m_font_height);}
	}
	else
	{	if(lx<(nn*StdFontWidth)) lx=StdWndPixelWidth;
		if(ly<(nn*StdFontHeight)) ly=StdWndPixelHeight;
		wc->px=lx; wc->py=ly; wc->cx=0; wc->cy=0;

		if(frame) {frame=1; lx+=2*StdFontWidth; ly+=2*StdFontHeight;} 

		wc->pWnd=NSLWndOpen(wc->name,lx,ly,0,0);
		if(!wc->pWnd) {lfree(wc->name); return(0);}

		wc->tx=wc->pWnd->m_xtextsize; wc->ty=wc->pWnd->m_ytextsize;
		wc->cx=wc->tx; wc->cy=wc->ty;
		if(frame) {wc->cx-=2; wc->cy-=2;}
	}
  
  wc->pWnd->m_IsFramed=(BOOL)wc->frame;
  wc->pWnd->SetScales(wc->px,wc->py);

  if(frame) w_title(w,NIO_TL,NIO_DRAW,FALSE,wc->name);
  w_clear(w);
  return(w);
}

long w_origin(int w,int x,int y)
{ wnd_elm *wp; wnd_com *wc; 
  if(!wnd_list)w_init();
  w=w_gethd(w);
  wp=wnd_list+w; wc=wnd_com_list+wp->com;
  return(wc->pWnd->Origin(x,y));	
}

long w_resize(int w,int x,int y)
{ wnd_elm *wp; wnd_com *wc; 
  if(!wnd_list)w_init();
  w=w_gethd(w);
  wp=wnd_list+w; wc=wnd_com_list+wp->com;
  return(wc->pWnd->ReSize(x,y));	
}

void w_focus(int w)
{ wnd_elm *wp; wnd_com *wc; 
  if(!wnd_list)w_init();
  w=w_gethd(w);
  wp=wnd_list+w; wc=wnd_com_list+wp->com;
  wc->pWnd->Focus();	
}

void w_report(int w)
{ wnd_elm *wp; wnd_com *wc; int nn,xx; long xy,rxy;
  if(!wnd_list)w_init();
  w=w_gethd(w);
  wp=wnd_list; nn=lst_num(wnd_list);
  w_puts(-w,"Windows:\n");
  for(xx=0;xx<nn;++xx)
   {if(wp->inuse)
      {wc=wnd_com_list+wp->com;
       xy=w_origin(xx,0,0);
	   rxy=w_resize(xx,0,0);
  	   w_printf(-w,"%d:%s:",xx,wc->name);
       if(wc->share) w_puts(-w,"shared,");
       if(wc->isdump) w_puts(-w,"dumping,");
       w_printf(-w,"cx=%d,cy=%d,px=%d,py=%d,ox=%d,oy=%d,rx=%d,ry=%d\n",
		   wc->cx,wc->cy,wc->px,wc->py,
		   w_x(xy),w_y(xy),w_x(rxy),w_y(rxy));
      }
    ++wp;
   }
  w_update(w);
}

/* establish a new object to be tracked, returns track handle */
int do_track(int w,int cmd,va_list args)
{ wnd_elm *wp; wnd_track *tp; int fc,bc,nn,tk=0;
  w=w_gethd(w);
  wp=wnd_list+w;
  fc=(int)va_arg(args,int); bc=(int)va_arg(args,int);
  tp=wp->tracks; nn=lst_ok(tp);
  while(nn--) {if(tp->obj_typ==NIO_FREE) goto gottk; ++tp; ++tk;}
  tp=lmore(wp->tracks,1,TYPE_STRUCT,wnd_track); wp->tracks=tp;
  if(!tp) return(0);
  tk=lst_num(tp)-1; tp+=tk; gottk:
  tp->buff=NULL; tp->demal=FALSE; tp->hide=FALSE; tp->active=FALSE;
  switch(cmd)
	{case NIO_IMAGE: tp->buff=va_arg(args,char *);
		tp->demal=(BOOL)va_arg(args,int);
	 case NIO_PIXEL: break;
	 default: cmd=NIO_CHR; tp->chr=(char)va_arg(args,int);
	}
  tp->obj_typ=(int)cmd; tp->fc=fc; tp->bc=bc; 
  tp->x=1; tp->y=1; /* initial pos of object */
  return(tk);
}

/* search track list for match */
BOOL chktrks(wnd_track *tp,wnd_track *aa, 
				int w, int obj,int x,int y,int act)
{ int tk=0,nn;
  if(!aa) return(FALSE);
  nn=lst_num(aa);
  while(nn--)
	{if(act)
		{if((aa!=tp)&&aa->x==x&&aa->y==y&&!aa->hide&&aa->obj_typ==(int)obj)
		  {w_track(w,NIO_DRAW,tk); return(TRUE);}
		}
	 else
		{if((aa!=tp)&&aa->x==x&&aa->y==y&&!aa->hide&&!aa->active)
		  {w_track(w,NIO_ERASE,tk); return(TRUE);}
		}
	 ++aa; ++tk;
	}
  return(FALSE);
}
/* activate/deactivate first track at x,y as long as not cur track */
void activate(wnd_track *tp,int w,int x,int y,int act)
{ wnd_elm *wp; wnd_com *wc; int uhd,obj=(int)tp->obj_typ;
  w=w_gethd(w); wp=wnd_list+w;
  if(chktrks(tp,wp->tracks,w,obj,x,y,act)) return;
  wc=wnd_com_list+wp->com;
  if(wc->share) /* chk shared trks */
	{uhd=wc->ulist;
	 while(uhd>=0)
		{wp=wnd_list+uhd;
		if(uhd!=w)
		  {if(chktrks((wnd_track *)NULL,wp->tracks,uhd,obj,x,y,act)) return;}
		uhd=wp->ulist;
		}
	}
}

/* create,update a track object */
int w_track(int w,int cmd,...)
{ wnd_elm *wp; wnd_com *wc; wnd_track *tp; va_list args;
  int tk,x,y,ix,iy,obj,cc; float fx,fy; char *buf="?";
  w=w_gethd(w); wp=wnd_list+w;
  va_start(args,cmd);
  if(cmd==NIO_CHR||cmd==NIO_PIXEL||cmd==NIO_IMAGE)
	{tk=do_track(w,cmd,args); goto byebye;}
  tk=(int)va_arg(args,int);
  switch(cmd)
	{case NIO_FREE:	do_erase(w,tk,FALSE); goto byebye;
	 case NIO_FLOAT: fx=(float)va_arg(args,double);
  		fy=(float)va_arg(args,double); break;
	 case NIO_INT:
		ix=(int)(va_arg(args,int)); iy=(int)(va_arg(args,int)); break;
	 case NIO_HIDE: case NIO_DRAW: case NIO_ERASE: break;
	 default: tk=0; goto byebye;
	}
  wc=wnd_com_list+wp->com;
  tp=wp->tracks; cc=lst_ok(tp);
  if(tk>=0&&tk<cc) {cc=1; tp+=tk;}
  while(cc--)
	{obj=tp->obj_typ; if(obj<NIO_CHR) {++tp; continue;}
  	/* get objects new coors */
  	if(cmd==NIO_FLOAT)
		{if(obj==NIO_CHR)
				{x=1+(int)(fx*(wc->cx-1)+(float)0.1);
				y=1+(int)(fy*(wc->cy-1)+(float)0.1);}
	 	else
				{x=(int)(fx*(wc->px-1)+(float)0.1);
			 y=(int)(fy*(wc->py-1)+(float)0.1);}
		}
  	else if(cmd==NIO_INT) {x=ix; y=iy;}
	else
	  {x=tp->x; y=tp->y;
		if(cmd==NIO_DRAW) goto drawobj; else goto eraseobj;
	  }
  	/* remove old object from wnd */
  	if(tp->active&&(tp->hide||x!=tp->x||y!=tp->y))
  	  {eraseobj: if(cmd==NIO_HIDE) tp->hide=TRUE;
		switch(obj)
		 {case NIO_PIXEL: wc->pWnd->SetColor(tp->bc); 
				wc->pWnd->SetPixel(tp->x,tp->y); break;
		  case NIO_IMAGE: wc->pWnd->SetColor(tp->bc);
			wc->pWnd->PutImage(tp->x,tp->y,tp->buff,TRUE); break;
			default: wnd_fc=tp->bc;
				w_xychr(wc,wc->frame+tp->x,wc->frame+tp->y,(unsigned char)' ');
		 }
  		tp->active=FALSE;
	  }
  	/* reactivate any underlying track by recursing */
  	if(!tp->active&&cmd<NIO_DRAW) activate(tp,w,tp->x,tp->y,TRUE);
  	if(cmd>NIO_DRAW) goto byebye;
  	tp->x=x; tp->y=y;
  	/* draw object at new pos */
  	if(!tp->hide&&!tp->active)
	  {/* deactivate any underlying track */
		if(cmd<NIO_DRAW) activate(tp,w,x,y,FALSE);
		drawobj:/* recursive call via activate() */
		if(cmd==NIO_DRAW) tp->hide=FALSE;
		switch(obj)
		  {case NIO_PIXEL: 
				wc->pWnd->SetColor(tp->fc); 
				wc->pWnd->SetPixel(tp->x,tp->y); break;
	 		case NIO_IMAGE: 
				wc->pWnd->SetColor(tp->fc);
				wc->pWnd->PutImage(tp->x,tp->y,tp->buff,FALSE); break;
			default: wnd_fc=tp->fc;
				 w_xychr(wc,wc->frame+x,wc->frame+y,(unsigned char)tp->chr);
		  }
  		tp->active=TRUE; tp->hide=FALSE;
	  }
	 ++tp;
	}
  byebye: va_end(args);
  return(tk);
}
/* redraw window & its tracks: if alltrks is set includes shared trks */
void w_refresh(int w,BOOL alltrks)
{ w_clear(w); 
  w=w_gethd(w);
  if(alltrks) w=wnd_com_list[wnd_list[w].com].ulist;
  do{w_track(w,NIO_DRAW,NIO_DEFAULT);
	 w=wnd_list[w].ulist;
	}while(alltrks&&w>=0&&w!=NIO_DEFAULT);
}

/* updates the display */
void w_update(int w)
{	wnd_elm *wp;wnd_com *wc;
	if(!wnd_list) return;
	w=w_gethd(w);
	wp=wnd_list+w;
	wc=wnd_com_list+wp->com;
	if(!wc||!wc->pWnd) return;
	wc->pWnd->Update();
}

/* returns TRUE if key hit, doesn't wait though! */
BOOL w_active(int w)
{	wnd_elm *wp; wnd_com *wc;
    w=w_gethd(w); wp=wnd_list+w; wc=wnd_com_list+wp->com;
	return(wc->pWnd->HitChar()!=(UINT)0);
}
/* waits for any key to be pressed & returns it, no echo */
int w_getch(int w)
{	wnd_elm *wp; wnd_com *wc;
    w=w_gethd(w); wp=wnd_list+w; wc=wnd_com_list+wp->com;
	while(!w_active(w)); return((int)wc->pWnd->GetChar());
}

/* write text to text window */
int w_printf(int w,char *fmt,...)
{ va_list args;
  va_start(args,fmt);
  vsprintf(out_buff,fmt,args);
  w_puts(w,out_buff);
  va_end(args);
  return(strlen(out_buff));
}
/* get string from console */
char *w_gets(int w,char *bp)
{ wnd_elm *wp; wnd_com *wc; char bf[2],*ap=bp,chr;
  if(!bp) return(bp);
  if(!wnd_list){w=0;w_init();}
  w=w_gethd(w);
  wp=wnd_list+w; wc=wnd_com_list+wp->com;
  w_select(wp,TRUE,TRUE); 
  w_focus(w);
  bf[1]='\0';
  for(;;)
	{wc->pWnd->DisplayCursor(TRUE);
	 chr=(char)wc->pWnd->GetChar();
	 wc->pWnd->DisplayCursor(FALSE);
	 if(chr=='\r') break;
	 if(chr=='\b') {if(ap<=bp) continue; --ap;}
	 else
	   {if(chr<' '||chr>127) continue; /* ignore non-ascii */
	    *ap=chr; ++ap;}
	 *bf=chr; w_puts(w,bf); 
	 if(chr=='\b')
		{wc->pWnd->PutChar((int)' ',wnd_fc,wnd_bc);
		if(wc->isdump) dumpxy(wc,wc->frame+wp->curx,wc->frame+wp->cury,' ');
		}
	}
  *ap='\n'; *(ap+1)='\0';
  return(bp);
}

/* these are functions rather than macro or inline, so that can be called from mon*/
int w_x(long yx)
{ return ((int)((yx)&0xffff));
}
int w_y(long yx) 
{ return ((int)((yx)>>16));
}
long w_xy(int x,int y) 
{ return ((long)((((y)&0xffff)<<16)+((x)&0xffff)));
}

/* Sets current pen position if x & y valid coordinates,
*  or returns current if x or y equals NIO_DEFAULT.
*  returns current position pacted into a long: (MSW=y,LSW=x)
*/
long w_position(int w,int x,int y)
{ wnd_elm *wp; wnd_com *wc; int px;
  if(!wnd_list){w=0;w_init();}
  w=w_gethd(w);
  wp=wnd_list+w; wc=wnd_com_list+wp->com;
  if(x!=NIO_DEFAULT&&y!=NIO_DEFAULT)
	{if(wp->ischr) {if(x==0) ++x; if(y==0) ++y;}
	 px=x;
	 if(wp->ischr)
		{if(x>0&&y>0&&x<=wc->cx&&y<=wc->cy)
		  {posok: wp->curx=px; wp->cury=y; return(w_xy(x,y));}
		}
	 else
		{if(x>=0&&y>=0&&x<wc->px&&y<wc->py) goto posok;}
	}
  return(w_xy(wp->curx,wp->cury));
}
/* returns relative size of window in chrs or pixels
*  size pacted into long: (MSW=y, LSW=x)
*/
long w_size(int w, BOOL ischr)
{ wnd_com *wc;
  if(!wnd_list){w=0;w_init();}
  w=w_gethd(w);
  wc=wnd_com_list+wnd_list[w].com;
  if(ischr) return(w_xy(wc->cx,wc->cy));
  return(w_xy(wc->px,wc->py));
}

/* Returns nonzero if the current location is onscreen */
int w_onscreen(int w)
{ wnd_elm *wp; wnd_com *wc; int y,x;
  if(!wnd_list){w=0;w_init();}
  w=w_gethd(w);
  wp=wnd_list+w; y=wp->cury; x=wp->curx;
  wc=wnd_com_list+wp->com;
  return(!((x<0)||(x>=wc->px)||(y<0)||(y>=wc->px)));
}

/* set window more state */
int w_more(int w,int state)
{ wnd_com *wc;
  if(!wnd_list){w=0;w_init();}
  w=w_gethd(w);
  wc=wnd_com_list+w;
  if(state!=NIO_DEFAULT) return(wc->ismore=(BOOL)state);
  return(wc->ismore);
}

/* Places the turtle pen used in move() and moveto() up or down.
	<state> can be TRUE (down), FALSE (up),
	or NIO_DEFAULT (return current)
	Returns current state.
*/
int w_pendown(int w,int state)
{ wnd_elm *wp;
  if(!wnd_list){w=0;w_init();}
  w=w_gethd(w);
  wp=wnd_list+w;
  if(state!=NIO_DEFAULT) return(wp->ispen=(BOOL)state);
  return(wp->ispen);
}
/* sets the pen move units to chars(TRUE) or pixels(FALSE) */
int w_penchar(int w,int state)
{ wnd_elm *wp;
  if(!wnd_list){w=0;w_init();}
  w=w_gethd(w);
  wp=wnd_list+w;
  if(state!=NIO_DEFAULT) return(wp->ischr=(BOOL)state);
  return(wp->ischr);
}

/* Sets color of pen, <c> is new color (use NIO_DEFAULT to get current)
	Returns current color */
int w_pencolor(int w,int c)
{ wnd_elm *wp;
  if(!wnd_list){w=0;w_init();}
  w=w_gethd(w);
  wp=wnd_list+w;
  if(c!=NIO_DEFAULT) wp->fc=c;
  return(wp->fc);
}

int w_penwidth(int w,int n)
{ wnd_elm *wp;
  if(!wnd_list){w=0;w_init();}
  w=w_gethd(w);
  wp=wnd_list+w;
  if(n>0) wp->pw=n;
  return(wp->pw);
}
/* Sets background color */
int w_papercolor(int w,int c)
{ wnd_elm *wp;
  if(!wnd_list){w=0;w_init();}
  w=w_gethd(w);
  wp=wnd_list+w;
  if(c!=NIO_DEFAULT) wp->bc=c;
  return(wp->bc);
}

/* Sets the color to be used as a boundary in fills.
	<c> is new border (use NIO_DEFAULT to get current).
	Returns border color.
*/
int w_bordercolor(int w,int c)
{ wnd_elm *wp;
  if(!wnd_list){w=0;w_init();}
  w=w_gethd(w);
  wp=wnd_list+w;
  if(c!=NIO_DEFAULT) wp->borderfc=c;
  return(wp->borderfc);
}

/* Determines whether figures should be filled.
	<state> can be TRUE (fill), FALSE (border only), or NIO_DEFAULT (return current)
	Returns current state.
*/
int w_fillstate(int w,int state)
{ wnd_elm *wp;
  if(!wnd_list){w=0;w_init();}
  w=w_gethd(w);
  wp=wnd_list+w;
  switch(state)
		{case 0: wp->isfill=FALSE; break;
		 case 1: wp->isfill = TRUE;
		 default:;
		}
  return(wp->isfill);
}

/* Sets a new turtle <angle> relative to the current angle.
	Returns the new angle.
*/
int w_turn(int w,int angle)
{ wnd_elm *wp;
  if(!wnd_list){w=0;w_init();}
  w=w_gethd(w);
  wp=wnd_list+w;
  return(wp->angle=((wp->angle+angle)%360));
}

/* Sets a specified turtle <angle>. Returns the new angle */
int w_turnto(int w,int angle)
{ wnd_elm *wp; 
  if(!wnd_list){w=0;w_init();}
  w=w_gethd(w); wp=wnd_list+w;
  return(wp->angle=(angle%360));
}

/* Moves from the current position to a	specified position. A line is
	drawn if the pen is down. The current position is reset.
	<x> and <y> are destination coordinates.
	Returns 0 if new position is off screen.
*/
int w_moveto(int w,int x,int y)
{ wnd_elm *wp; wnd_com *wc; int ox,oy;
  int IsNoUpdate=(int)(w<0); if(IsNoUpdate) w=(-w);
  if(!wnd_list){w=0;w_init();}
  wp=wnd_list+w; wc=wnd_com_list+wp->com;
  w_select(wp,wp->ischr,(BOOL)(wp->ispen&&!wp->ischr));
  ox=wp->curx; oy=wp->cury;
  w_position(w,x,y); /* this sets curx & cury, or leaves if off wnd */
  if(ox==wp->curx&&oy==wp->cury) goto miss;
  if(wp->ispen&&!wp->ischr) 
	{wc->pWnd->DoLineTo(wp->curx,wp->cury);
	 if(!IsNoUpdate) wc->pWnd->Update();
	}
  else {miss: wc->pWnd->SetGrafPos(wp->curx,wp->cury);}
  return(w_onscreen(w));
}

/* Moves from the current position in the current direction. A line is
	drawn if the pen is down. The current position is reset.
	<distance> is the adjusted length of line.
	Returns 0 if the new position is off	the screen.
*/
int w_move(int w,int d)
{ wnd_elm *wp; int dX,dY,ow=w; double workangle;
  if(w<0) w=(-w);
  if(!wnd_list){w=0;w_init();}
  wp=wnd_list+w;
  workangle=(wp->angle-90)*3.141593/180;
  dX=(int)(d*cos(workangle)); dY=(int)(d*sin(workangle));
  return(w_moveto(ow,wp->curx+dX,wp->cury+dY));
}

/* Set pixel */
int w_setpixel(int w,int x,int y)
{ wnd_elm *wp;wnd_com *wc; int retcode; 
  int IsNoUpdate=(int)(w<0); if(IsNoUpdate) w=(-w);
  if(!wnd_list){w=0;w_init();}
  wp=wnd_list+w;
  w_select(wp,FALSE,TRUE);
  wc=wnd_com_list+wp->com;
  retcode=((int)wc->pWnd->SetPixel(x,y));
  if(!IsNoUpdate) wc->pWnd->Update();
  return(retcode);
}
/* Returns color of pixel at x,y */
int w_getpixel(int w,int x,int y)
{ wnd_elm *wp;wnd_com *wc;
  if(!wnd_list){w=0;w_init();}
  w=w_gethd(w);
  wp=wnd_list+w;
  wc=wnd_com_list+wp->com;
  return((int)wc->pWnd->GetPixel(x,y));
}

/* Draws an arc */
int w_arc(int w,int x1,int y1,int x2,int y2,
		   int x3,int y3,int x4,int y4)
{ wnd_elm *wp; wnd_com *wc; int retcode;
  int IsNoUpdate=(int)(w<0); if(IsNoUpdate) w=(-w);
  if(!wnd_list){w=0;w_init();}
  wp=wnd_list+w;
  w_select(wp,FALSE,TRUE);
  wc=wnd_com_list+wp->com;
  retcode=(wc->pWnd->DoArc(x1,y1,x2,y2,x3,y3,x4,y4));
  if(!IsNoUpdate) wc->pWnd->Update();
  return(retcode);
}

/* Fills starting at the current location.
	Returns nonzero if successful. */
int w_fill(int w)
{ wnd_elm *wp; wnd_com *wc; int retcode;
  int IsNoUpdate=(int)(w<0); if(IsNoUpdate) w=(-w);
  if(!wnd_list){w=0;w_init();}
  wp=wnd_list+w;
  w_select(wp,FALSE,TRUE);
  wc=wnd_com_list+wp->com;
  retcode=(wc->pWnd->DoFloodFill(wp->curx,wp->cury,wp->borderfc));
  if(!IsNoUpdate) wc->pWnd->Update();
  return(retcode);
}

/* Draws a rectangle */
/* note if coors go out of range it crashes */
int w_rectangle(int w,int x1,int y1,int x2,int y2)
{ wnd_elm *wp; wnd_com *wc; int retcode;
  int IsNoUpdate=(int)(w<0); if(IsNoUpdate) w=(-w);
  if(!wnd_list){w=0;w_init();}
  wp=wnd_list+w;
  w_select(wp,FALSE,TRUE);
  wc=wnd_com_list+wp->com;
  retcode=(wc->pWnd->DoRectangle(wp->isfill,x1,y1,x2,y2));
  if(!IsNoUpdate) wc->pWnd->Update();
  return(retcode);
}

/* Draws a polygon
	Returns 0 if any part of polygon is off screen.
*/
int w_poly(int w,int num,int side)
{ wnd_elm *wp; wnd_com *wc; int ow=w,i,ret=1; double angle;
  int IsNoUpdate=(int)(w<0); if(IsNoUpdate) w=(-w);
  if(!wnd_list){w=0;w_init();}
  wp=wnd_list+w;
  w_select(wp,FALSE,TRUE);
  angle=(double)(360/num);
  for(i=1;i<=num;++i) {ret=w_move(ow,side)&&ret; w_turn(w,(int)angle);}
  if(!IsNoUpdate) {wc=wnd_com_list+wp->com; wc->pWnd->Update();}
  return(ret);
}
/* Puts an ellipse with radii rx & ry at current location.
	Returns nonzero if successful. */
int w_ellipse(int w,int rx,int ry)
{ wnd_elm *wp; wnd_com *wc; int retcode;
  int IsNoUpdate=(int)(w<0); if(IsNoUpdate) w=(-w);
  if(!wnd_list){w=0;w_init();}
  wp=wnd_list+w;
  w_select(wp,FALSE,TRUE);
  wc=wnd_com_list+wp->com;
  retcode=(wc->pWnd->DoEllipse(wp->isfill,wp->curx-rx,wp->cury-ry,
		 wp->curx+rx,wp->cury+ry));
  if(!IsNoUpdate) wc->pWnd->Update();
  return(retcode);
}

/* Puts a circle with radius <r> at current location.
	Returns nonzero if successful. */
int w_circle(int w,int r)
{  return(w_ellipse(w,r,r));
}

extern "C" int mon_wnd=0;
extern "C" void mon_w_puts(char *str)
{	w_puts(mon_wnd,str); 
}
