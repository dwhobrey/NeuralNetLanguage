// NSLApp.cpp : Generic NeSeL CWinApp. DJRW Nov 1996.
// Minimal code to simply boot the monitor 

#include "NSLApp.h"
#include "NSLWnd.h"

BEGIN_MESSAGE_MAP(CNSLApp, CWinApp)
	//{{AFX_MSG_MAP(CNSLApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		// DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CNSLApp::CNSLApp()
{
}

CNSLApp theApp;

BOOL CNSLApp::InitInstance()
{	WorkerExitOnReturn=TRUE;
	NSLStartMain();
	m_pMainWnd=WorkerMainWnd;
	return TRUE;
}

int CNSLApp::ExitInstance() 
{	int nsl_status=NSLStopMain();
	int win_status=CWinApp::ExitInstance();
	if(nsl_status) return(nsl_status);
	return(win_status);
}
