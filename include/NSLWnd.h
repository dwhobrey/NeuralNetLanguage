// NSLWnd.h : header file
//
#include <wingdi.h>
#include <afxmt.h>

#define M_OBJECT_DELAY ((DWORD)30*1000)

#define NSLKEYSIZ 128 // typeahead keyboard buffer size

#define NSLSTDXVIRTUAL 1000
#define NSLSTDYVIRTUAL 1000
#define NSLSTDCHARHEIGHT 20
#define NSLSTDCHARWIDTH 60
#define NSLSTDPIXELHEIGHT 150
#define NSLSTDPIXELWIDTH 200
#define NSLSTDFONTHEIGHT 14
#define NSLSTDFONTWIDTH 8
#define NSLSTDPENWIDTH 1
#define StdColorNum 255
#define StdNumColors (StdColorNum+1)
#define StdColorBlack 0
#define StdColorWhite StdColorNum
#define StdColorInc ((COLORREF)(((long)0x1000000)/StdColorNum))
extern int StdFontHeight;
extern int StdFontWidth;
extern int StdPenWidth;
extern int StdWndCharHeight;
extern int StdWndCharWidth;
extern int StdWndPixelHeight;
extern int StdWndPixelWidth;

/* object types */
typedef enum
{	M_PIXEL,M_FILL,M_LINE,M_RECTANGLE,M_ELLIPSE,M_ARC
} M_ObjectType;
typedef struct
{	M_ObjectType m_type;
	BOOL IsHidden,IsFill;
	int color,penwidth,x1,y1,x2,y2,x3,y3,x4,y4;
} M_object;

///////////////////////////////////////////////////////////////////////////
// CNSLWnd dialog
class CNSLWnd : public CWnd
{	DECLARE_DYNAMIC(CNSLWnd)
	friend class CNslrock;
	// Construction
public:
	CNSLWnd(CWnd *pParent=NULL,
			char *name=NULL,
			int nCols=0,int nRows=0,
			int nWidth=0,int nHeight=0);   // standard constructor
	~CNSLWnd();
	void InitWnd(CWnd *pParent);
	long ReSize(int nWidth,int nHeight);
	void SetFont(int width,int height);
	void SetTextSize(int width,int height);
	void SetScales(int x,int y);
	void SetPalette(COLORREF *p);
	COLORREF idx2color(int n);
	void G_destroy();
	void G_drawtext();
	void G_blanktext(BOOL DoFrame);
	void G_scrolltext();

	int RealX(int x) {return((m_IsFramed?m_font_width:0)+(m_IsScaled?((int)(m_xscale*(float)(x))):(x)));}
	int RealY(int y) {return((m_IsFramed?m_font_height:0)+(m_IsScaled?((int)(m_yscale*(float)(y))):(y)));}
	
	UINT GetChar();
	UINT GetCharNoWait();
	UINT HitChar();
	UINT GetTextChar();
	void SetTextPos(int y,int x);
	void SetGrafPos(int x,int y);
	void SetPenWidth(int n);
	void SetColor(int color);
	void SetBackgroundColor(int color);
	COLORREF SetPixel(int x,int y);
	COLORREF GetPixel(int x,int y);
	void PutImage(int x,int y,char *buff,int iserase);
	int DoRectangle(int isfill,int x1,int y1,int x2,int y2);
	void DoLineTo(int x,int y);
	int DoArc(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4);
	int DoFloodFill(int x,int y,int fillcolor);
	int DoEllipse(int isfill,int x1,int y1,int x2,int y2);
	void WndClear(BOOL DoFrame);
	void WndClose();
	void PutChar(int chr,int foreground,int background);
	void Update();
	void Focus();
	void MyFocus();
	long Origin(int x,int y);
	void DisplayCursor(int IsOn);
	void SetCursorPos();

	BOOL EqualObjects(M_object *p,M_object *q);
	BOOL CheckObject(M_object *p);
	void AddObject(M_ObjectType o_type,...);
	void FreeObjects();
	void FreeText();

	void DebugWnd();

	BOOL m_IsFramed;
  	int xsize,ysize;
	int m_xtextsize,m_ytextsize;
	int m_font_width;
	int m_font_height;

private:
	CWnd *m_pParent;
	BOOL m_IsReady;		/* set when wnd is ready for use */
	BOOL m_IsEmpty;		/* set if bitmap not defined */
	BOOL m_IsScaled;	/* set if to apply x,y scaling */
	BOOL m_CursorOn,m_HasCursor;
	int m_guard,m_IsClosing;
	int m_xtextpos,m_ytextpos;
	int m_xgrafpos,m_ygrafpos;
	UINT **m_textbf;
	int m_xvirtual,m_yvirtual;
	float m_xscale,m_yscale;

	CMutex m_objects_guard,m_text_guard;
	int m_num_objects; 
	M_object *m_objects;
	COLORREF *m_palette;

	int m_keyin,m_keyout;
	UINT m_keybf[NSLKEYSIZ];
	int m_background,m_foreground,m_penwidth;
	CFont m_font;
	char *m_font_name;
	char *m_name;
	CDC memDC;
	CBitmap memBitmap;
	LOGPEN m_logpen;
	LOGBRUSH m_logbrush;

// Implementation
protected:
	void PostNcDestroy();
	// Generated message map functions
	//{{AFX_MSG(CNSLWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnClose();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class CNslrock : public CWnd
{
// Construction
public:
	CNslrock();
	~CNslrock();
// Implementation
protected:
	void PostNcDestroy();
	// Generated message map functions
	//{{AFX_MSG(CNslrock)
	afx_msg LRESULT OnNSLMessage(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CNSLWnd *NSLWndOpen(char *name,int x,int y,int cx,int cy);

// NSL main & thread control declarations:
// NSLCWnd message 
#define WM_NSLMESSAGE (WM_USER + 100)

#define NSLINITINSTANCE(appclassname)\
BOOL appclassname::InitInstance()\
{	WorkerExitOnReturn=TRUE;\
	NSLStartMain();\
	m_pMainWnd=WorkerMainWnd;\
	return TRUE;\
}

#define NSLEXITINSTANCE(appclassname)\
int appclassname::ExitInstance()\
{	int nsl_status=NSLStopMain();\
	int win_status=CWinApp::ExitInstance();\
	if(nsl_status) return(nsl_status);\
	return(win_status);\
}

extern "C" int NSLStartMain();
extern "C" int NSLStopMain();
extern "C" int NSLMainHasStopped();
extern "C" int NSLWinIsRunning();
extern "C" int NSLCtrl_C();
extern "C" int NSLmain(int argc,char *argv[]);
extern CWnd *WorkerMainWnd;
extern BOOL WorkerExitOnReturn;


