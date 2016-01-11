// NSLWnd.cpp : implementation file
//
#include "stdafx.h"
#include <stdio.h>
#include <stdarg.h>
#include <process.h>
#include "NSLWnd.h"
#include "NSLType.h"
#include "NSLStd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CNSLWnd, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CNSLWnd dialog
int StdXVirtual=NSLSTDXVIRTUAL;
int StdYVirtual=NSLSTDYVIRTUAL;
int StdFontHeight=NSLSTDFONTHEIGHT;
int StdFontWidth=NSLSTDFONTWIDTH;
int StdWndCharHeight=NSLSTDCHARHEIGHT;
int StdWndCharWidth=NSLSTDCHARWIDTH;
int StdWndPixelHeight=NSLSTDPIXELHEIGHT;
int StdWndPixelWidth=NSLSTDPIXELWIDTH;
int StdPenWidth=NSLSTDPENWIDTH;

int yCalibrate=0;
int xCalibrate=0;
char *StdFontName="MSLineDraw";

#define HY_PEN_WX 1	 // width of lines
#define HY_PEN_WY 1

#define w_xy(x,y) ((long)((((y)&0xffff)<<16)+((x)&0xffff)))

//////////////////////////////////////////////////////////////////
// NSLmain thread control
// From win app thread:
// a) call NSLStartMain when ready to start your main routine
// b) call NSLStopMain to stop(terminate) your main thread
//
extern BOOL WorkerExitOnReturn=TRUE;
extern CWnd *WorkerMainWnd=NULL;

static BOOL WorkerWinIsRunning=TRUE;
static BOOL WorkerMainHasStopped=TRUE;
static int WorkerRetCode=0;
static int WorkerCtrl_C=0;
static DWORD WorkerMainThreadId=0;
static CNslrock *WorkerMainRock=NULL;

extern "C" int NSLWinIsRunning()
{	return WorkerWinIsRunning;
}
extern "C" int NSLMainHasStopped()
{	return WorkerMainHasStopped;
}

extern "C" int NSLCtrl_C()
{	int tmp=WorkerCtrl_C; WorkerCtrl_C=0;
	return tmp;
}
extern "C" void w_sleep(int milli)
{	Sleep((DWORD)milli);
}
// Thread stuff
void __cdecl WorkerInitThread(void *dummy)
{	WorkerRetCode=NSLmain(__argc,__argv);
	if(WorkerExitOnReturn) WorkerMainWnd->PostMessage(WM_CLOSE);
	WorkerMainHasStopped=TRUE;
}

// stop NSLmain thread
extern "C" int NSLStopMain()
{	int num_secs=60; WorkerWinIsRunning=FALSE;
	while(num_secs--)
	{if(WorkerMainHasStopped) 
		{	if(WorkerMainRock) 
			{	WorkerMainRock->DestroyWindow();
				WorkerMainRock=NULL;
			}
			return(WorkerRetCode);
		}
	 Sleep(1000);
	}
	return(1); // hadn't stopped;
}

extern "C" int NSLStartMain()
{	if(WorkerMainWnd||WorkerMainRock) return(!WorkerMainHasStopped);
	WorkerMainRock=new CNslrock;
	return(!WorkerMainHasStopped);
}

/////////////////////////////////////////////////////////////////////////////
// CNslrock dialog
// This is just a minimal CWnd class to piggy back messages on for CNSLWnd's

// start up worker thread & attach us to a CWnd message pump
CNslrock::CNslrock()
{	if(!CreateEx(WS_EX_NOPARENTNOTIFY,
		 AfxRegisterWndClass(CS_NOCLOSE),
		 "",WS_DISABLED,0,0,0,0,NULL,(HMENU)0))
		{TRACE("Unable to create NeSeL Rock\n"); return;}
	WorkerMainWnd=this;
	WorkerMainHasStopped=FALSE; WorkerWinIsRunning=TRUE; 
	WorkerMainThreadId=GetCurrentThreadId(); 
	unsigned long threadhnd= _beginthread( WorkerInitThread, 0, (void *)AfxGetApp());
	WorkerMainHasStopped=(threadhnd==(-1)); 
}

CNslrock::~CNslrock() 
{	// free any mem
}

void CNslrock::PostNcDestroy()
{	WorkerMainRock=NULL;
	delete this;
}

BEGIN_MESSAGE_MAP(CNslrock, CWnd)
	//{{AFX_MSG_MAP(CNslrock)
    ON_MESSAGE(WM_NSLMESSAGE, OnNSLMessage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define NSLWNDCMD_ONCLOSE ((LPARAM)(-1))
#define NSLWNDCMD_FOCUS ((LPARAM)(-2))

LRESULT CNslrock::OnNSLMessage(WPARAM wParam, LPARAM lParam)
{	switch(lParam)
	{	case NSLWNDCMD_ONCLOSE:
			((CNSLWnd *)wParam)->OnClose();
			break;
		case NSLWNDCMD_FOCUS:
			((CNSLWnd *)wParam)->MyFocus();
			break;
		default:
			((CNSLWnd *)wParam)->InitWnd((CWnd *)lParam);
	}
	return (LRESULT)0;
}

//////////////////////////

void CNSLWnd::SetPalette(COLORREF *p)
{	if(m_IsClosing) return;
	if(StdNumColors<=256)
	{	int k=(256/StdNumColors),n; if(k<1) k=1;
		if(m_palette) {free(m_palette); m_palette=NULL;}
		m_palette=(COLORREF *)malloc(StdNumColors*sizeof(COLORREF));
		for(n=0;n<StdNumColors;n++)
		{	if(p) m_palette[n]=p[n];
			else m_palette[n]=RGB(k*n,k*n,k*n);
		}
	}
}

COLORREF CNSLWnd::idx2color(int n) 
{	if(m_palette) return(m_palette[n]);
	return((COLORREF)((n)?((n)*StdColorInc-1):0));
}

// set the font size based on num of cols & rows
void CNSLWnd::SetFont(int nCols,int nRows)
{	int width,height;
	if(nCols<1||nRows<1) return;
	if(m_guard++) return;
	width=xsize/nCols; if(width*nCols<xsize) ++width;
	height=ysize/nRows; if(height*nRows<ysize) ++height;
	if(width%2) ++width; if(height%2) ++height;
	m_font.DeleteObject();
	LOGFONT lf; // Used to create the CFont.
	memset(&lf, 0, sizeof(LOGFONT));   // Clear out structure.
	lf.lfHeight = height;
	lf.lfWidth  = width;
	lf.lfPitchAndFamily = FIXED_PITCH | FF_MODERN; // fixed width
	lf.lfCharSet = SYMBOL_CHARSET;
	strcpy(lf.lfFaceName, m_font_name);	// with face name "CourierNew".
	m_font.CreateFontIndirect(&lf);    // Create the font.
	m_font.GetLogFont(&lf);
	m_font_height=lf.lfHeight;
	m_font_width=lf.lfWidth;
	// snap wnd size to fit font exactly
	width=(xsize-m_font_width*nCols);
	height=(ysize-m_font_height*nRows);
	if(width!=0||height!=0) ReSize(xsize-width,ysize-height);
	m_guard=0;
}

// set the window text size in rows and columns
// allocate line buffers & copy from old where necessary
void CNSLWnd::SetTextSize(int width,int height)
{	int num,cp_num,cp_base;	UINT **oldbf=m_textbf;
	CSingleLock m_mutex(&m_text_guard); /*make sure no painting in progress*/
	if(m_mutex.Lock(M_OBJECT_DELAY)==0) return; /* return on failure */ 
	if(width<1||height<1||m_IsClosing) return;
	m_textbf=(UINT **)malloc(height*sizeof(UINT *));
	for(num=0;num<height;++num) m_textbf[num]=(UINT *)NULL;
	if(oldbf)
	{	if(height>m_ytextsize) 
			{cp_base=0; cp_num=m_ytextsize;}
		else 
			{cp_base=(m_ytextsize-height); cp_num=height;}
		for(num=0;num<cp_num;++num) 
		{	m_textbf[num]=oldbf[cp_base+num];
			oldbf[cp_base+num]=NULL;
		}
		for(num=0;num<m_ytextsize;++num)
			{if(oldbf[num]) free(oldbf[num]);}
		free(oldbf);
	}

	for(num=0;num<height;++num)
	{	if(m_textbf[num]) {cp_num=m_xtextsize;} else cp_num=0;
		m_textbf[num]=(UINT *)realloc(m_textbf[num],width*sizeof(UINT));
		for(cp_base=cp_num;cp_base<width;cp_base++) 
				m_textbf[num][cp_base]=(UINT)0;
	}
	m_xtextsize=width; m_ytextsize=height;

	SetFont(m_xtextsize,m_ytextsize);
}

int StdXLocation=10,StdYLocation=10;

void CNSLWnd::InitWnd(CWnd *pParent)
{	// initialise members
	m_IsEmpty=TRUE; m_IsScaled=TRUE;
	m_CursorOn=FALSE; m_HasCursor=FALSE;
	m_num_objects=0; m_objects=NULL; m_palette=NULL;
	m_textbf=NULL; m_xtextpos=0; m_ytextpos=0;
	m_background=StdColorBlack; m_foreground=StdColorWhite;
	m_penwidth=StdPenWidth;
	if(!pParent) pParent=AfxGetMainWnd();
	m_pParent=pParent;
	m_xvirtual=StdXVirtual; m_yvirtual=StdYVirtual;
	m_xscale=(float)1.0; m_yscale=(float)1.0;
	m_font_name=StdFontName;
	m_keyin=0; m_keyout=0; m_keybf[0]=(UINT)0;
	m_xgrafpos=0; m_ygrafpos=0;
	
	m_logpen.lopnStyle = PS_INSIDEFRAME;
	m_logpen.lopnWidth.x = m_penwidth;
	m_logpen.lopnWidth.y = m_penwidth;
	m_logpen.lopnColor = idx2color(StdColorWhite); // line color
	m_logbrush.lbStyle = BS_SOLID;
	m_logbrush.lbColor = idx2color(StdColorWhite); // for fills
	m_logbrush.lbHatch = HS_HORIZONTAL;

	int oldx=xsize,oldy=ysize;
	// note, create calls our OnSize, so need to init before calling
	++m_guard;
	if(!CreateEx(WS_EX_CLIENTEDGE,
				 AfxRegisterWndClass(
					CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW|CS_NOCLOSE,
					LoadCursor(NULL,IDC_ARROW),
					0, //(HBRUSH)(1+COLOR_WINDOW),
					LoadIcon(NULL,IDI_WINLOGO)),
				 "NeSeL Window",
				 WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_POPUP | WS_VISIBLE 
				 | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME,
                 StdXLocation,StdYLocation,
				 xsize+xCalibrate,ysize+yCalibrate,
				 pParent?pParent->GetSafeHwnd():NULL,
				 (HMENU)0))
		TRACE("Unable to create NeSeL Wnd\n");
	else 
	{	if(oldx!=xsize||oldy!=ysize)
		{	if(oldx!=xsize) xCalibrate+=(oldx-xsize); 
		    if(oldy!=ysize) yCalibrate+=(oldy-ysize);
			if(xCalibrate<0) xCalibrate=0; if(yCalibrate<0) yCalibrate=0;
			ReSize(oldx,oldy);
		}
		m_guard=0;
		SetTextSize(m_xtextsize,m_ytextsize);

		m_xvirtual=xsize; m_yvirtual=ysize;
		m_xscale=(float)xsize/(float)m_xvirtual;
		m_yscale=(float)ysize/(float)m_yvirtual;
		m_IsScaled=TRUE;

		SetWindowText((LPCTSTR)m_name);

		StdXLocation+=2*StdFontWidth; 
		if(StdXLocation>20*StdFontWidth) StdXLocation=StdFontWidth;
		StdYLocation+=2*StdFontHeight;
		if(StdYLocation>20*StdFontHeight) StdYLocation=StdFontHeight;

		m_IsReady=TRUE;
	}

	//{{AFX_DATA_INIT(CNSLWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CNSLWnd::CNSLWnd(CWnd* pParent/*=NULL*/,char *name,/*=NULL*/
				 int nWidth/*=0*/,int nHeight,/*=0*/
				 int nCols/*=0*/,int nRows/*=0*/
				)
{	m_IsReady=FALSE; m_guard=0; m_IsClosing=0; m_name=name; 
	if(nCols>0&&nRows>0)
	{	if(nHeight>nRows&&nWidth>nCols)
		{	nHeight-=(nHeight%nRows); 
			nWidth-=(nWidth%nCols);
		}
		 else 
		{	nHeight=StdFontHeight*nRows; 
			nWidth=StdFontWidth*nCols;
		}
	}
	else if(nWidth>StdFontWidth&&nHeight>StdFontHeight)
	{	if(nWidth<128) nWidth=128; // Windows OS minium width
		if(nWidth%StdFontWidth) 
			nWidth+=(StdFontWidth-(nWidth%StdFontWidth));
		nCols=nWidth/StdFontWidth;
		if(nHeight%StdFontHeight) 
			nHeight+=(StdFontHeight-(nHeight%StdFontHeight));
		nRows=nHeight/StdFontHeight;
	}
	else 
	{	nCols=40; nRows=10; 
		nWidth=nCols*StdFontWidth;
		nHeight=nRows*StdFontHeight;
	}
	xsize=nWidth; ysize=nHeight; 
	m_xtextsize=nCols; m_ytextsize=nRows;

	if(WorkerMainThreadId && WorkerMainThreadId!=GetCurrentThreadId())
	{	WorkerMainWnd->PostMessage(WM_NSLMESSAGE,(WPARAM)this,(LPARAM)pParent);
		int num_secs=30*(1000/50); 
		while(num_secs--)
		{	if(m_IsReady) 
				return;
			Sleep(50);
		}
		return; // yikes !!
	}
	else InitWnd(pParent);
}

BEGIN_MESSAGE_MAP(CNSLWnd, CWnd)
	//{{AFX_MSG_MAP(CNSLWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CHAR()
	ON_WM_ACTIVATE()
	ON_WM_CLOSE()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
/* free graphics memory & device context */
void CNSLWnd::G_destroy()
{	if(m_IsEmpty) return; 
	m_IsEmpty=TRUE;
	memDC.DeleteDC();
	memBitmap.DeleteObject();
}

CNSLWnd::~CNSLWnd() 
{	++m_IsClosing;
	if(WorkerMainThreadId && WorkerMainThreadId!=GetCurrentThreadId())
	{int tmp=0; /* being cleared by wrong thread!!*/
	}
	G_destroy();
	FreeObjects();
	FreeText();
}

void CNSLWnd::PostNcDestroy()
{	delete this;
}

void CNSLWnd::OnClose() 
{	++m_IsClosing;
	CWnd::OnClose();
	DestroyWindow();
}

void CNSLWnd::Update()
{	if(m_IsClosing) return;
	InvalidateRgn(NULL,FALSE);
}

void CNSLWnd::SetScales(int x,int y)
{	m_xvirtual=x; m_yvirtual=y;
	if(m_IsFramed)
	{	m_xscale=(float)(xsize-2*m_font_width)/(float)m_xvirtual;
		m_yscale=(float)(ysize-2*m_font_height)/(float)m_yvirtual;
	}
	else
	{	m_xscale=(float)xsize/(float)m_xvirtual;
		m_yscale=(float)ysize/(float)m_yvirtual;
	}
	m_IsScaled=TRUE;
}

long CNSLWnd::Origin(int x,int y)
{	WINDOWPLACEMENT val;
	if(!GetWindowPlacement(&val)) return((long)0);
	if(x>0&&y>0)
	{	int wx=val.rcNormalPosition.right-val.rcNormalPosition.left,
			wy=val.rcNormalPosition.bottom-val.rcNormalPosition.top;
		val.rcNormalPosition.left=x; val.rcNormalPosition.right=x+wx;
		val.rcNormalPosition.top=y; val.rcNormalPosition.bottom=y+wy;
		SetWindowPlacement(&val);
	}
	return(w_xy(val.rcNormalPosition.left,val.rcNormalPosition.top));
}

void CNSLWnd::MyFocus()
{	SetActiveWindow();
		SetFocus();
}

void CNSLWnd::Focus()
{	if(WorkerMainThreadId && WorkerMainThreadId!=GetCurrentThreadId())
		WorkerMainWnd->PostMessage(WM_NSLMESSAGE,(WPARAM)this,NSLWNDCMD_FOCUS);
	else MyFocus();
}

void CNSLWnd::OnSize(UINT nType, int cx, int cy) 
{	if(m_IsClosing) return;
	CWnd::OnSize(nType, cx, cy);

	G_destroy();

	xsize=cx; ysize=cy;
	if(m_IsEmpty=(BOOL)(ysize*xsize==0)) return;

	// set up a memory device context for drawing into 
	CClientDC dc(this); 
	memDC.CreateCompatibleDC(&dc);
	memBitmap.CreateCompatibleBitmap(&dc,xsize,ysize);

	if(m_guard) return;
	SetFont(m_xtextsize,m_ytextsize);

	if(m_IsFramed)
	{	m_xscale=(float)(xsize-2*m_font_width)/(float)m_xvirtual;
		m_yscale=(float)(ysize-2*m_font_height)/(float)m_yvirtual;
	}
	else
	{	m_xscale=(float)xsize/(float)m_xvirtual;
		m_yscale=(float)ysize/(float)m_yvirtual;
	}

	Update();
}

long CNSLWnd::ReSize(int nWidth,int nHeight)
{	RECT location; GetWindowRect(&location);
	if(nWidth>0&&nHeight>0)
	{	MoveWindow(location.left,location.top,nWidth+xCalibrate,nHeight+yCalibrate);
	}
	return(w_xy(xsize,ysize));
	
}

void CNSLWnd::WndClear(BOOL DoFrame)
{	if(m_IsClosing) return;
	FreeObjects();
	G_blanktext(DoFrame);
}

void CNSLWnd::WndClose()
{	if(m_IsClosing++) return;
	if(WorkerMainThreadId && WorkerMainThreadId!=GetCurrentThreadId())
		WorkerMainWnd->PostMessage(WM_NSLMESSAGE,(WPARAM)this,NSLWNDCMD_ONCLOSE);
	else OnClose();
}

CNSLWnd *NSLWndOpen(char *name,int x,int y,int cx,int cy)
{	return new CNSLWnd(NULL,name,x,y,cx,cy);
}

void CNSLWnd::SetColor(int color)
{	m_foreground=color;
}
void CNSLWnd::SetBackgroundColor(int color)
{	m_background=color;
}

void CNSLWnd::SetPenWidth(int n)
{	m_penwidth=n;
}

void CNSLWnd::SetTextPos(int y,int x)
{	if(x>0&&x<=m_xtextsize) m_xtextpos=x-1; 
	if(y>0&&y<=m_ytextsize) m_ytextpos=y-1;
}

void CNSLWnd::SetGrafPos(int x,int y)
{	if(x>=0&&x<xsize) m_xgrafpos=x; 
	if(y>=0&&y<ysize) m_ygrafpos=y;
}

// returns prior color of pixel 
COLORREF CNSLWnd::GetPixel(int x,int y)
{	return memDC.GetPixel(x,y);	
}

void CNSLWnd::FreeObjects()
{	CSingleLock m_mutex(&m_objects_guard); /*make sure no painting in progress*/
	if(m_mutex.Lock(M_OBJECT_DELAY)==0) ; /*kill it anyway...*/
	m_num_objects=0;
	if(m_objects) free(m_objects);
	m_objects=NULL;  
}

void CNSLWnd::FreeText()
{	CSingleLock m_mutex(&m_text_guard); /*make sure no painting in progress*/
	if(m_mutex.Lock(M_OBJECT_DELAY)==0) ; /*kill it anyway...*/
	if(m_textbf)
	{	int num; 
		for(num=0;num<m_ytextsize;++num) 
			{if(m_textbf[num]) free(m_textbf[num]);}
		free(m_textbf); m_textbf=NULL;
	}
	if(m_palette) {free(m_palette); m_palette=NULL;}
}


// compare objects, apart from color
BOOL CNSLWnd::EqualObjects(M_object *p,M_object *q)
{	if(p->m_type!=q->m_type) return(FALSE);
	switch(p->m_type)
	{	case M_PIXEL: case M_FILL:
			return(p->x1==q->x1&&p->y1==q->y1);
		case M_RECTANGLE: case M_ELLIPSE:
			if(p->IsFill!=q->IsFill) return(FALSE);
		case M_LINE: case M_ARC:
			if(p->penwidth!=q->penwidth) return(FALSE);
			if(p->x1!=q->x1||p->y1!=q->y1
				||p->x2!=q->x2||p->y2!=q->y2) return(FALSE);
			if(p->m_type!=M_ARC) break;
			if(p->x3!=q->x3||p->y3!=q->y3
				||p->x4!=q->x4||p->y4!=q->y4) return(FALSE);
		default:;
	}
	return(TRUE);
}

// check if object already on list. remove if color=m_background
// returns TRUE if object ok to add
BOOL CNSLWnd::CheckObject(M_object *p)
{	M_object *op=m_objects; int num;
	for(num=0;num<m_num_objects;++num)
	{	if(EqualObjects(p,op))
		{	// del old obj
			int k;
			for(k=num;k<(m_num_objects-1);++k) m_objects[k]=m_objects[k+1]; 
			--m_num_objects; --num;
			if(p->color==m_background) return(FALSE);
		}
		else ++op;
	}
	return(TRUE);
}

void CNSLWnd::AddObject(M_ObjectType o_type,...)
{	M_object *p,t; va_list args; 
    va_start(args,o_type);
	CSingleLock m_mutex(&m_objects_guard); /*make sure no painting in progress*/
	if(m_mutex.Lock(M_OBJECT_DELAY)==0) return; /* return on failure */ 
	if(m_IsClosing) return;
	// load object details from arg stack to object struct
	t.m_type=o_type; t.IsHidden=FALSE; t.IsFill=FALSE;
	t.color=va_arg(args,int);
	t.penwidth=va_arg(args,int);
	switch(o_type)
	{	case M_PIXEL: case M_FILL:
			t.x1=va_arg(args,int);
			t.y1=va_arg(args,int);
			break;
		case M_RECTANGLE: case M_ELLIPSE:
			t.IsFill=(BOOL)va_arg(args,int);
		case M_LINE: case M_ARC:
			t.x1=va_arg(args,int);
			t.y1=va_arg(args,int);
			t.x2=va_arg(args,int);
			t.y2=va_arg(args,int);
			if(o_type!=M_ARC) break;
			t.x3=va_arg(args,int);
			t.y3=va_arg(args,int);
			t.x4=va_arg(args,int);
			t.y4=va_arg(args,int);
		default:;
	}
	if(CheckObject(&t))
	{	p=(M_object *)realloc(m_objects,sizeof(M_object)*(m_num_objects+1));
		if(p) 
		{	m_objects=p; p+=m_num_objects; ++m_num_objects;
			*p=t;
		}
	}
	va_end(args);
}

// sets & returns prior color of pixel too	
COLORREF CNSLWnd::SetPixel(int x,int y)
{	COLORREF color=GetPixel(x,y);
	AddObject(M_PIXEL,m_foreground,m_penwidth,x,y);
	return(color);
}
int CNSLWnd::DoFloodFill(int x,int y,int fillcolor)
{	AddObject(M_FILL,fillcolor,m_penwidth,x,y);
	return(0);
}
void CNSLWnd::DoLineTo(int x,int y)
{	AddObject(M_LINE,m_foreground,m_penwidth,m_xgrafpos,m_ygrafpos,x,y);
}
int CNSLWnd::DoRectangle(int isfill,int x1,int y1,int x2,int y2)
{	AddObject(M_RECTANGLE,m_foreground,m_penwidth,isfill,x1,y1,x2,y2);
	return(0);
}
int CNSLWnd::DoEllipse(int isfill,int x1,int y1,int x2,int y2)
{	AddObject(M_ELLIPSE,m_foreground,m_penwidth,isfill,x1,y1,x2,y2);
	return(0);
}
int CNSLWnd::DoArc(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{	AddObject(M_ARC,m_foreground,m_penwidth,x1,y1,x2,y2,x3,y3,x4,y4);
	return(0);
}

void CNSLWnd::PutImage(int x,int y,char *buff,int iserase)
{
}

void CNSLWnd::PutChar(int chr,int foreground,int background)
{	if(!m_textbf) return;
	m_textbf[m_ytextpos][m_xtextpos]=((background&0xff)<<24)
		+((foreground&0xff)<<16)+(chr&0xffff);
}

UINT CNSLWnd::GetTextChar()
{	if(!m_textbf) return(0);
	return((int)(m_textbf[m_ytextpos][m_xtextpos]&0xffff));
}

void CNSLWnd::G_blanktext(BOOL DoFrame)
{	int x,y; if(!m_textbf||m_ytextsize<1||m_xtextsize<1) return;
	if(DoFrame)
	{	for(y=0;y<m_ytextsize;++y)
		{	UINT *tmp=m_textbf[y];
			for(x=0;x<m_xtextsize;++x) tmp[x]=(m_background<<24);
		}
	}
	else
	{	for(y=1;y<(m_ytextsize-1);++y)
		{	UINT *tmp=m_textbf[y];
			for(x=1;x<m_xtextsize-1;++x) tmp[x]=(m_background<<24);
		}

	}
}

void CNSLWnd::G_scrolltext()
{	int x,y; if(!m_textbf||m_ytextsize<1||m_xtextsize<1) return;
	if(m_IsFramed)
	{	for(y=1;y<(m_ytextsize-2);++y)
		{ memcpy((void *)(m_textbf[y]+1),(void *)(m_textbf[y+1]+1),
			(size_t)sizeof(UINT)*(m_xtextsize-2));
		}
		UINT *tmp=m_textbf[m_ytextsize-2];
		for(x=1;x<m_xtextsize-1;++x) tmp[x]=(m_background<<24);

	}
	else
	{	UINT *tmp=m_textbf[0];
		for(x=0;x<m_xtextsize;++x) tmp[x]=(m_background<<24);
		for(y=1;y<=(m_ytextsize-1);++y)
		{	m_textbf[y-1]=m_textbf[y];}
		m_textbf[m_ytextsize-1]=tmp;
	}
}

// copy the text buffer to bitmap
void CNSLWnd::G_drawtext()
{	int x,y; if(!m_textbf||m_ytextsize<1||m_xtextsize<1) return;
	CFont *pOldFont = memDC.SelectObject(&m_font);
	float scroll_height=(float)ysize/(float)m_ytextsize;
	float scroll_width=(float)xsize/(float)m_xtextsize;
	char buff[2]; buff[1]='\0';	int tx,ty;
	for(y=0;y<m_ytextsize;++y)
	{	ty=(int)(scroll_height*(float)y);
		for(x=0;x<m_xtextsize;++x)
		{	if(buff[0]=(m_textbf[y][x]&0xffff))
			{	tx=(int)(scroll_width*(float)x);
				memDC.SetTextColor(idx2color((m_textbf[y][x]>>16)&0xff));
				memDC.SetBkColor(idx2color((m_textbf[y][x]>>24)&0xff));
				memDC.TextOut(tx,ty,(LPCTSTR)buff,1);
			}
		}
	}
	memDC.SelectObject(pOldFont); // reselect old font 
}

void CNSLWnd::OnPaint() 
{	CPaintDC dc(this); 
	if(m_IsEmpty||m_IsClosing) return;
 	CSingleLock m_muobject(&m_objects_guard); /* guard objects while painting*/
 	CSingleLock m_mutext(&m_text_guard); /* guard objects while painting*/

	if(m_muobject.Lock(M_OBJECT_DELAY)==0 
		|| m_mutext.Lock(M_OBJECT_DELAY)==0) 
	{	Update(); return;} /* return on failure */ 

	CBitmap *pOldBmp = memDC.SelectObject(&memBitmap); 

	memDC.SetMapMode(MM_TEXT);

	// Clear the bitmap with the background color
	memDC.FillSolidRect(0,0,xsize,ysize,idx2color(m_background));

	CRgn mybox; 
	if(m_IsFramed) // prevent any graphics disturbing frame
	{	mybox.CreateRectRgn(m_font_width,m_font_height,
			xsize-m_font_width,ysize-m_font_height);
		memDC.SelectClipRgn(&mybox);
	}

	int o_type,num=m_num_objects; M_object *p=m_objects; 
	while(num--)
	{	o_type=p->m_type;
		if(!p->IsHidden) switch(o_type)
		{	case M_PIXEL:
				{	memDC.SetPixel(RealX(p->x1),RealY(p->y1),idx2color(p->color));
				}
				break;
			case M_FILL:
				{	CBrush brush,*pOldBrush;
					m_logbrush.lbColor=idx2color(p->color);
					m_logbrush.lbStyle=BS_SOLID;
					brush.CreateBrushIndirect(&m_logbrush);
					pOldBrush = memDC.SelectObject(&brush);
					memDC.FloodFill(RealX(p->x1),RealY(p->y1),idx2color(p->color));
					memDC.SelectObject(pOldBrush);
				}
				break;
			case M_RECTANGLE: case M_ELLIPSE:
				{	CBrush brush,*pOldBrush; CPen pen,*pOldPen;
					m_logbrush.lbColor=idx2color(p->color);
					if(p->IsFill) m_logbrush.lbStyle=BS_SOLID;
					else m_logbrush.lbStyle=BS_HOLLOW;
					brush.CreateBrushIndirect(&m_logbrush);
					pOldBrush = memDC.SelectObject(&brush);
					m_logpen.lopnColor=idx2color(p->color);
					m_logpen.lopnWidth.x = p->penwidth;
					m_logpen.lopnWidth.y = p->penwidth;
					pen.CreatePenIndirect(&m_logpen);
					pOldPen = memDC.SelectObject(&pen);
					if(o_type==M_ELLIPSE)
						memDC.Ellipse(RealX(p->x1),RealY(p->y1),
							RealX(p->x2),RealY(p->y2));
					else
						memDC.Rectangle(RealX(p->x1),RealY(p->y1),
							RealX(p->x2),RealY(p->y2));
					memDC.SelectObject(pOldPen);
					memDC.SelectObject(pOldBrush);
				}
				break;
			case M_LINE:
				{	CPen pen,*pOldPen;
					m_logpen.lopnColor=idx2color(p->color);
					m_logpen.lopnWidth.x = p->penwidth;
					m_logpen.lopnWidth.y = p->penwidth;
					if (!pen.CreatePenIndirect(&m_logpen)) break;
					pOldPen = memDC.SelectObject(&pen);
					memDC.MoveTo(RealX(p->x1),RealY(p->y1));
					memDC.LineTo(RealX(p->x2),RealY(p->y2));
					memDC.SelectObject(pOldPen);
				}
				break;
			case M_ARC:
				{	CPen pen,*pOldPen;
					m_logpen.lopnColor=idx2color(p->color);
					m_logpen.lopnWidth.x = p->penwidth;
					m_logpen.lopnWidth.y = p->penwidth;
					if (!pen.CreatePenIndirect(&m_logpen)) break;
					pOldPen = memDC.SelectObject(&pen);
					memDC.Arc(RealX(p->x1),RealY(p->y1),
							RealX(p->x2),RealY(p->y2),
							RealX(p->x3),RealY(p->y3),
							RealX(p->x4),RealY(p->y4));
					memDC.SelectObject(pOldPen);
				}
				break;
			default:;
		}
		++p;
	}

	if(m_IsFramed)
	{	memDC.SelectClipRgn(NULL);
		mybox.DeleteObject();
	}

    G_drawtext();
	SetCursorPos();

	// Blit the bitmap to the display
	CClientDC clientdc(this);	 // get client device context 
	clientdc.BitBlt(0,0,xsize,ysize,&memDC,0,0,SRCCOPY);

	memDC.SelectObject(pOldBmp); 
}

void CNSLWnd::DisplayCursor(int IsOn)
{	m_CursorOn=(BOOL)IsOn;
	if(m_HasCursor)
	{	if(m_CursorOn) ShowCaret();
		else HideCaret();
	}
}

void CNSLWnd::SetCursorPos()
{	if(!m_HasCursor||m_ytextsize<1||m_xtextsize<1) return;
	DestroyCaret();
	CreateSolidCaret(m_font_width,m_font_height);
	POINT cpos; 
	float scroll_height=(float)ysize/(float)m_ytextsize;
	float scroll_width=(float)xsize/(float)m_xtextsize;
	cpos.x=(int)((float)m_xtextpos*scroll_width);
	cpos.y=(int)((float)m_ytextpos*scroll_height);
	SetCaretPos(cpos);
	if(m_CursorOn) ShowCaret();	else HideCaret();
}

void CNSLWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{	CWnd::OnActivate(nState, pWndOther, bMinimized);
	if(nState==WA_ACTIVE||nState==WA_CLICKACTIVE)
	{	CreateSolidCaret(m_font_width,m_font_height);
		m_HasCursor=TRUE;
		SetCursorPos();
	}
	else if(nState==WA_INACTIVE)
	{	if(m_HasCursor) DestroyCaret();
		m_HasCursor=FALSE;
	}
}

void CNSLWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar==0x21||nChar==0x22) // PageUp & PgDwn keys
	{ if(nChar==0x21) nChar=0x5b; else nChar=0x5d;
		while(nRepCnt-->0)
		{	OnChar(nChar,(UINT)1,nFlags);
			OnChar((UINT)0xd,(UINT)1,nFlags);
		}
	  return;
	}
	
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CNSLWnd::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	if(nChar==0x3) {WorkerCtrl_C=TRUE; return;}
	if(!m_keybf) return;
	while(nRepCnt--) 
	{	m_keybf[m_keyin++]=nChar; 
		if(m_keyin>=NSLKEYSIZ) m_keyin=0;
		m_keybf[m_keyin]=(UINT)0;
	}
}

UINT CNSLWnd::GetCharNoWait()
{	UINT chr;
	if(!m_keybf) return((UINT)0);
	if(chr=m_keybf[m_keyout]) 
	{++m_keyout; if(m_keyout>=NSLKEYSIZ) m_keyout=0;
	}
	return(chr);
}

UINT CNSLWnd::GetChar()
{	UINT chr; 
	if(!m_keybf) return((UINT)0);
	while((chr=GetCharNoWait())==(UINT)0) ;
	return(chr);
}

UINT CNSLWnd::HitChar()
{	if(!m_keybf) return((UINT)0);
	return(m_keybf[m_keyout]); 
}

///////////////////////////////
void CNSLWnd::DebugWnd()
{	CBitmap *pBmp = memDC.SelectObject(&memBitmap); 
    CFont *pFont = memDC.SelectObject(&m_font); 
	char buff[100];
 	memDC.SetTextColor(idx2color(StdColorWhite));
	memDC.SetBkColor(idx2color(StdColorBlack));
	
	sprintf(buff,"xsize=%d,ysize=%d",xsize,ysize);
	memDC.TextOut(0,0,buff,strlen(buff));
	sprintf(buff,"m_xscale=%g,m_yscale=%g",m_xscale,m_yscale);
	memDC.TextOut(0,20,buff,strlen(buff));
		
	memDC.SelectObject(pFont); // reselect old objects 
	memDC.SelectObject(pBmp); 
}

