// nslw.cpp : NeSeL Monitor main Win routine. DJRW Nov 1996.
// Minimal code to simply boot the monitor 

#include "stdafx.h"
#include "nslw.h"
#include "NSLWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CNslwApp, CWinApp)
	//{{AFX_MSG_MAP(CNslwApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		// DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CNslwApp::CNslwApp()
{
}

CNslwApp theApp;

BOOL CNslwApp::InitInstance()
{	WorkerExitOnReturn=TRUE;
	NSLStartMain();
	m_pMainWnd=WorkerMainWnd;
	return TRUE;
}

int CNslwApp::ExitInstance() 
{	int nsl_status=NSLStopMain();
	int win_status=CWinApp::ExitInstance();
	if(nsl_status) return(nsl_status);
	return(win_status);
}
