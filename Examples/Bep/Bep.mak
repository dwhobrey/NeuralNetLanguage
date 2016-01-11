# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Bep - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Bep - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Bep - Win32 Release" && "$(CFG)" != "Bep - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Bep.mak" CFG="Bep - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Bep - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Bep - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "Bep - Win32 Debug"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "Bep - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\Bep.exe"

CLEAN : 
	-@erase "$(INTDIR)\Bep.obj"
	-@erase "$(INTDIR)\NSLApp.obj"
	-@erase "$(OUTDIR)\Bep.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\..\include" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/Bep.pch" /YX /Fo"$(INTDIR)/"\
 /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Bep.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/Bep.pdb" /machine:I386 /out:"$(OUTDIR)/Bep.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Bep.obj" \
	"$(INTDIR)\NSLApp.obj" \
	"..\..\include\nsllib.lib"

"$(OUTDIR)\Bep.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Bep - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\Bep.exe"

CLEAN : 
	-@erase "$(INTDIR)\Bep.obj"
	-@erase "$(INTDIR)\NSLApp.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\Bep.exe"
	-@erase "$(OUTDIR)\Bep.ilk"
	-@erase "$(OUTDIR)\Bep.pdb"
	-@erase ".\bep.nsc"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\include" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/Bep.pch" /YX\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Bep.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/Bep.pdb" /debug /machine:I386 /out:"$(OUTDIR)/Bep.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Bep.obj" \
	"$(INTDIR)\NSLApp.obj" \
	"..\..\include\nsllib.lib"

"$(OUTDIR)\Bep.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "Bep - Win32 Release"
# Name "Bep - Win32 Debug"

!IF  "$(CFG)" == "Bep - Win32 Release"

!ELSEIF  "$(CFG)" == "Bep - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Bep.c
DEP_CPP_BEP_C=\
	"..\..\include\memtrack.h"\
	"..\..\include\nslios.h"\
	"..\..\include\nslstd.h"\
	"..\..\include\nsltype.h"\
	"..\..\include\nslwsio.h"\
	".\bep.nsc"\
	".\bep.nsd"\
	".\bep.nsh"\
	

"$(INTDIR)\Bep.obj" : $(SOURCE) $(DEP_CPP_BEP_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Bep.n

!IF  "$(CFG)" == "Bep - Win32 Release"

!ELSEIF  "$(CFG)" == "Bep - Win32 Debug"

# Begin Custom Build - Running NeSeL...
ProjDir=.
InputPath=.\Bep.n
InputName=Bep

"$(ProjDir)\$(InputName).nsc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   c:\usr\bin\nsl.exe $(InputName) -chdir $(ProjDir)

# End Custom Build

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Home\Darren\Thesis\NeSeL\include\nsllib.lib

!IF  "$(CFG)" == "Bep - Win32 Release"

!ELSEIF  "$(CFG)" == "Bep - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Home\Darren\Thesis\NeSeL\include\NSLApp.cpp
DEP_CPP_NSLAP=\
	"..\..\include\NSLApp.h"\
	"..\..\include\NSLWnd.h"\
	

"$(INTDIR)\NSLApp.obj" : $(SOURCE) $(DEP_CPP_NSLAP) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
# End Target
# End Project
################################################################################
