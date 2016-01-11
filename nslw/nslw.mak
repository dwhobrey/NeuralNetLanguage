# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=nslw - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to nslw - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "nslw - Win32 Release" && "$(CFG)" != "nslw - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "nslw.mak" CFG="nslw - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "nslw - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "nslw - Win32 Debug" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "nslw - Win32 Debug"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "nslw - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\nslw.exe"

CLEAN : 
	-@erase "$(INTDIR)\nslmain.obj"
	-@erase "$(INTDIR)\nslw.obj"
	-@erase "$(INTDIR)\nslw.res"
	-@erase "$(OUTDIR)\nslw.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/nslw.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x809 /fo"$(INTDIR)/nslw.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/nslw.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/nslw.pdb" /machine:I386 /out:"$(OUTDIR)/nslw.exe" 
LINK32_OBJS= \
	"$(INTDIR)\nslmain.obj" \
	"$(INTDIR)\nslw.obj" \
	"$(INTDIR)\nslw.res" \
	"..\include\nsllib.lib"

"$(OUTDIR)\nslw.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "nslw - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\nslw.exe"

CLEAN : 
	-@erase "$(INTDIR)\nslmain.obj"
	-@erase "$(INTDIR)\nslw.obj"
	-@erase "$(INTDIR)\nslw.res"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\nslw.exe"
	-@erase "$(OUTDIR)\nslw.ilk"
	-@erase "$(OUTDIR)\nslw.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\include" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/nslw.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x809 /fo"$(INTDIR)/nslw.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/nslw.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/nslw.pdb" /debug /machine:I386 /out:"$(OUTDIR)/nslw.exe" 
LINK32_OBJS= \
	"$(INTDIR)\nslmain.obj" \
	"$(INTDIR)\nslw.obj" \
	"$(INTDIR)\nslw.res" \
	"..\include\nsllib.lib"

"$(OUTDIR)\nslw.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "nslw - Win32 Release"
# Name "nslw - Win32 Debug"

!IF  "$(CFG)" == "nslw - Win32 Release"

!ELSEIF  "$(CFG)" == "nslw - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\nslw.rc
DEP_RSC_NSLW_=\
	".\res\nslw.ico"\
	".\res\nslw.rc2"\
	

"$(INTDIR)\nslw.res" : $(SOURCE) $(DEP_RSC_NSLW_) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\nslw.cpp

!IF  "$(CFG)" == "nslw - Win32 Release"

DEP_CPP_NSLW_C=\
	".\nslw.h"\
	".\StdAfx.h"\
	
NODEP_CPP_NSLW_C=\
	".\NSLWnd.h"\
	

"$(INTDIR)\nslw.obj" : $(SOURCE) $(DEP_CPP_NSLW_C) "$(INTDIR)"\
 "$(INTDIR)\nslw.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/nslw.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "nslw - Win32 Debug"

DEP_CPP_NSLW_C=\
	"..\include\NSLWnd.h"\
	".\nslw.h"\
	".\StdAfx.h"\
	
# ADD CPP /YX

"$(INTDIR)\nslw.obj" : $(SOURCE) $(DEP_CPP_NSLW_C) "$(INTDIR)"
   $(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\include" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/nslw.pch" /YX\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\nslmain.c

!IF  "$(CFG)" == "nslw - Win32 Release"

NODEP_CPP_NSLMA=\
	".\Nslms.h"\
	".\nslwsio.h"\
	

"$(INTDIR)\nslmain.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\nslw.pch"
   $(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/nslw.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "nslw - Win32 Debug"

DEP_CPP_NSLMA=\
	"..\include\memtrack.h"\
	"..\include\nslios.h"\
	"..\include\nslms.h"\
	"..\include\nslstd.h"\
	"..\include\nsltype.h"\
	"..\include\nslwsio.h"\
	
# ADD CPP /YX

"$(INTDIR)\nslmain.obj" : $(SOURCE) $(DEP_CPP_NSLMA) "$(INTDIR)"
   $(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\include" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/nslw.pch" /YX\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Home\Darren\Thesis\NeSeL\include\nsllib.lib

!IF  "$(CFG)" == "nslw - Win32 Release"

!ELSEIF  "$(CFG)" == "nslw - Win32 Debug"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
