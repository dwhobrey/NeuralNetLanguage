; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CNSLWnd
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "nslw.h"

ClassCount=4
Class1=CNslwApp
Class2=CNslwDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_NSLW_DIALOG
Class4=CNSLWnd

[CLS:CNslwApp]
Type=0
HeaderFile=nslw.h
ImplementationFile=nslw.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CNslwApp

[CLS:CNslwDlg]
Type=0
HeaderFile=nslwDlg.h
ImplementationFile=nslwDlg.cpp
Filter=D

[CLS:CAboutDlg]
Type=0
HeaderFile=nslwDlg.h
ImplementationFile=nslwDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[DLG:IDD_NSLW_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CNslwDlg

[CLS:CNSLWnd]
Type=0
HeaderFile=nslwnd.h
ImplementationFile=nslwnd.cpp
BaseClass=CWnd
LastObject=CNSLWnd
Filter=W
VirtualFilter=WC

