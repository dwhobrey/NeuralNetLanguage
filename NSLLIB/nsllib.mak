# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

!IF "$(CFG)" == ""
CFG=nsllib - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to nsllib - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "nsllib - Win32 Release" && "$(CFG)" != "nsllib - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "nsllib.mak" CFG="nsllib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "nsllib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "nsllib - Win32 Debug" (based on "Win32 (x86) Static Library")
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
# PROP Target_Last_Scanned "nsllib - Win32 Debug"
CPP=cl.exe

!IF  "$(CFG)" == "nsllib - Win32 Release"

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

ALL : "$(OUTDIR)\nsllib.lib"

CLEAN : 
	-@erase "$(INTDIR)\memtrack.obj"
	-@erase "$(INTDIR)\Nsldebug.obj"
	-@erase "$(INTDIR)\Nslios.obj"
	-@erase "$(INTDIR)\nslmonfn.obj"
	-@erase "$(INTDIR)\Nslmshi.obj"
	-@erase "$(INTDIR)\nslmslo.obj"
	-@erase "$(INTDIR)\Nslmsmon.obj"
	-@erase "$(INTDIR)\Nslstd.obj"
	-@erase "$(INTDIR)\NSLWnd.obj"
	-@erase "$(INTDIR)\nslwsio.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\ytab.obj"
	-@erase "$(OUTDIR)\nsllib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "..\include" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/nsllib.pch" /YX\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/nsllib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/nsllib.lib" 
LIB32_OBJS= \
	"$(INTDIR)\memtrack.obj" \
	"$(INTDIR)\Nsldebug.obj" \
	"$(INTDIR)\Nslios.obj" \
	"$(INTDIR)\nslmonfn.obj" \
	"$(INTDIR)\Nslmshi.obj" \
	"$(INTDIR)\nslmslo.obj" \
	"$(INTDIR)\Nslmsmon.obj" \
	"$(INTDIR)\Nslstd.obj" \
	"$(INTDIR)\NSLWnd.obj" \
	"$(INTDIR)\nslwsio.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ytab.obj"

"$(OUTDIR)\nsllib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "nsllib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\nsllib.lib"

CLEAN : 
	-@erase "$(INTDIR)\memtrack.obj"
	-@erase "$(INTDIR)\Nsldebug.obj"
	-@erase "$(INTDIR)\Nslios.obj"
	-@erase "$(INTDIR)\nslmonfn.obj"
	-@erase "$(INTDIR)\Nslmshi.obj"
	-@erase "$(INTDIR)\nslmslo.obj"
	-@erase "$(INTDIR)\Nslmsmon.obj"
	-@erase "$(INTDIR)\Nslstd.obj"
	-@erase "$(INTDIR)\NSLWnd.obj"
	-@erase "$(INTDIR)\nslwsio.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\ytab.obj"
	-@erase "$(OUTDIR)\nsllib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /Z7 /Od /I "..\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /c
CPP_PROJ=/nologo /MLd /W3 /GX /Z7 /Od /I "..\include" /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)/nsllib.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/nsllib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/nsllib.lib" 
LIB32_OBJS= \
	"$(INTDIR)\memtrack.obj" \
	"$(INTDIR)\Nsldebug.obj" \
	"$(INTDIR)\Nslios.obj" \
	"$(INTDIR)\nslmonfn.obj" \
	"$(INTDIR)\Nslmshi.obj" \
	"$(INTDIR)\nslmslo.obj" \
	"$(INTDIR)\Nslmsmon.obj" \
	"$(INTDIR)\Nslstd.obj" \
	"$(INTDIR)\NSLWnd.obj" \
	"$(INTDIR)\nslwsio.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\ytab.obj"

"$(OUTDIR)\nsllib.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
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

# Name "nsllib - Win32 Release"
# Name "nsllib - Win32 Debug"

!IF  "$(CFG)" == "nsllib - Win32 Release"

!ELSEIF  "$(CFG)" == "nsllib - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\memtrack.c

"$(INTDIR)\memtrack.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Nsldebug.c

"$(INTDIR)\Nsldebug.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Nslios.c
DEP_CPP_NSLIO=\
	"..\include\nslios.h"\
	"..\include\NSLStd.h"\
	"..\include\NSLType.h"\
	

"$(INTDIR)\Nslios.obj" : $(SOURCE) $(DEP_CPP_NSLIO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Nslmshi.c
DEP_CPP_NSLMS=\
	"..\include\memtrack.h"\
	"..\include\nslios.h"\
	"..\include\nslms.h"\
	"..\include\NSLStd.h"\
	"..\include\NSLType.h"\
	

"$(INTDIR)\Nslmshi.obj" : $(SOURCE) $(DEP_CPP_NSLMS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\nslmslo.c
DEP_CPP_NSLMSL=\
	"..\include\memtrack.h"\
	"..\include\nslios.h"\
	"..\include\nslms.h"\
	"..\include\NSLStd.h"\
	"..\include\NSLType.h"\
	

"$(INTDIR)\nslmslo.obj" : $(SOURCE) $(DEP_CPP_NSLMSL) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Nslmsmon.c
DEP_CPP_NSLMSM=\
	"..\include\memtrack.h"\
	"..\include\nslios.h"\
	"..\include\nslms.h"\
	"..\include\nslmsmon.h"\
	"..\include\NSLStd.h"\
	"..\include\NSLType.h"\
	"..\include\nslwsio.h"\
	".\Ytab.h"\
	

"$(INTDIR)\Nslmsmon.obj" : $(SOURCE) $(DEP_CPP_NSLMSM) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Nslstd.c
DEP_CPP_NSLST=\
	"..\include\memtrack.h"\
	"..\include\NSLStd.h"\
	"..\include\NSLType.h"\
	

"$(INTDIR)\Nslstd.obj" : $(SOURCE) $(DEP_CPP_NSLST) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\NSLWnd.cpp

!IF  "$(CFG)" == "nsllib - Win32 Release"

DEP_CPP_NSLWN=\
	"..\include\NSLStd.h"\
	"..\include\NSLType.h"\
	"..\include\NSLWnd.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\NSLWnd.obj" : $(SOURCE) $(DEP_CPP_NSLWN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "nsllib - Win32 Debug"

DEP_CPP_NSLWN=\
	"..\include\NSLStd.h"\
	"..\include\NSLType.h"\
	"..\include\NSLWnd.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\NSLWnd.obj" : $(SOURCE) $(DEP_CPP_NSLWN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\nslwsio.cpp
DEP_CPP_NSLWS=\
	"..\include\memtrack.h"\
	"..\include\nslios.h"\
	"..\include\nslms.h"\
	"..\include\NSLStd.h"\
	"..\include\NSLType.h"\
	"..\include\NSLWnd.h"\
	"..\include\nslwsio.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\nslwsio.obj" : $(SOURCE) $(DEP_CPP_NSLWS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ytab.c
DEP_CPP_YTAB_=\
	"..\include\memtrack.h"\
	"..\include\nslios.h"\
	"..\include\nslms.h"\
	"..\include\nslmsmon.h"\
	"..\include\NSLStd.h"\
	"..\include\NSLType.h"\
	"..\include\nslwsio.h"\
	

"$(INTDIR)\ytab.obj" : $(SOURCE) $(DEP_CPP_YTAB_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\nslmonfn.c

"$(INTDIR)\nslmonfn.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
