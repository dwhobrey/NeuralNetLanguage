# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=BAM - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to BAM - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "BAM - Win32 Release" && "$(CFG)" != "BAM - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "BAM.mak" CFG="BAM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BAM - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "BAM - Win32 Debug" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "BAM - Win32 Debug"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "BAM - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\BAM.exe"

CLEAN : 
	-@erase "$(INTDIR)\Bam.obj"
	-@erase "$(INTDIR)\NSLApp.obj"
	-@erase "$(OUTDIR)\BAM.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/BAM.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/BAM.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/BAM.pdb" /machine:I386 /out:"$(OUTDIR)/BAM.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Bam.obj" \
	"$(INTDIR)\NSLApp.obj" \
	"..\..\include\nsllib.lib"

"$(OUTDIR)\BAM.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "BAM - Win32 Debug"

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

ALL : "$(OUTDIR)\BAM.exe"

CLEAN : 
	-@erase "$(INTDIR)\Bam.obj"
	-@erase "$(INTDIR)\NSLApp.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\BAM.exe"
	-@erase "$(OUTDIR)\BAM.ilk"
	-@erase "$(OUTDIR)\BAM.pdb"
	-@erase ".\Bam.nsc"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\include" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/BAM.pch" /YX\
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)/BAM.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/BAM.pdb" /debug /machine:I386 /out:"$(OUTDIR)/BAM.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Bam.obj" \
	"$(INTDIR)\NSLApp.obj" \
	"..\..\include\nsllib.lib"

"$(OUTDIR)\BAM.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "BAM - Win32 Release"
# Name "BAM - Win32 Debug"

!IF  "$(CFG)" == "BAM - Win32 Release"

!ELSEIF  "$(CFG)" == "BAM - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Bam.n

!IF  "$(CFG)" == "BAM - Win32 Release"

!ELSEIF  "$(CFG)" == "BAM - Win32 Debug"

# Begin Custom Build - Running NeSeL...
ProjDir=.
InputPath=.\Bam.n
InputName=Bam

"$(ProjDir)\$(InputName).nsc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   c:\usr\bin\nsl.exe $(InputName) -chdir $(ProjDir) -nsl2 -j

# End Custom Build

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Bam.c

!IF  "$(CFG)" == "BAM - Win32 Release"

DEP_CPP_BAM_C=\
	".\Bam.nsc"\
	".\bam.nsd"\
	".\Bam.nsh"\
	
NODEP_CPP_BAM_C=\
	".\memtrack.h"\
	".\nslios.h"\
	".\nslstd.h"\
	".\nsltype.h"\
	".\nslwsio.h"\
	

"$(INTDIR)\Bam.obj" : $(SOURCE) $(DEP_CPP_BAM_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "BAM - Win32 Debug"

DEP_CPP_BAM_C=\
	"..\..\include\memtrack.h"\
	"..\..\include\nslios.h"\
	"..\..\include\nslstd.h"\
	"..\..\include\nsltype.h"\
	"..\..\include\nslwsio.h"\
	".\Bam.nsc"\
	".\bam.nsd"\
	".\Bam.nsh"\
	

"$(INTDIR)\Bam.obj" : $(SOURCE) $(DEP_CPP_BAM_C) "$(INTDIR)" ".\Bam.nsc"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\Home\Darren\Thesis\NeSeL\include\nsllib.lib

!IF  "$(CFG)" == "BAM - Win32 Release"

!ELSEIF  "$(CFG)" == "BAM - Win32 Debug"

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
