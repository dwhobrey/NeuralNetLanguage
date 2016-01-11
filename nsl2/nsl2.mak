# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=nsl2 - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to nsl2 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "nsl2 - Win32 Release" && "$(CFG)" != "nsl2 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "nsl2.mak" CFG="nsl2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "nsl2 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "nsl2 - Win32 Debug" (based on "Win32 (x86) Console Application")
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
# PROP Target_Last_Scanned "nsl2 - Win32 Debug"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "nsl2 - Win32 Release"

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

ALL : "$(OUTDIR)\nsl2.exe"

CLEAN : 
	-@erase "$(INTDIR)\Conxhi2.obj"
	-@erase "$(INTDIR)\Conxlo2.obj"
	-@erase "$(INTDIR)\Globals2.obj"
	-@erase "$(INTDIR)\Iohi2.obj"
	-@erase "$(INTDIR)\Iolo2.obj"
	-@erase "$(INTDIR)\Nial2.obj"
	-@erase "$(INTDIR)\Nsltwo2.obj"
	-@erase "$(INTDIR)\Nslusrfn.obj"
	-@erase "$(INTDIR)\Pathhi2.obj"
	-@erase "$(INTDIR)\Pathlo2.obj"
	-@erase "$(INTDIR)\Thirdpa2.obj"
	-@erase "$(INTDIR)\Yacclo2.obj"
	-@erase "$(INTDIR)\Ytab2.obj"
	-@erase "$(OUTDIR)\nsl2.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/nsl2.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/nsl2.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/nsl2.pdb" /machine:I386 /out:"$(OUTDIR)/nsl2.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Conxhi2.obj" \
	"$(INTDIR)\Conxlo2.obj" \
	"$(INTDIR)\Globals2.obj" \
	"$(INTDIR)\Iohi2.obj" \
	"$(INTDIR)\Iolo2.obj" \
	"$(INTDIR)\Nial2.obj" \
	"$(INTDIR)\Nsltwo2.obj" \
	"$(INTDIR)\Nslusrfn.obj" \
	"$(INTDIR)\Pathhi2.obj" \
	"$(INTDIR)\Pathlo2.obj" \
	"$(INTDIR)\Thirdpa2.obj" \
	"$(INTDIR)\Yacclo2.obj" \
	"$(INTDIR)\Ytab2.obj"

"$(OUTDIR)\nsl2.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "nsl2 - Win32 Debug"

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

ALL : "$(OUTDIR)\nsl2.exe"

CLEAN : 
	-@erase "$(INTDIR)\Conxhi2.obj"
	-@erase "$(INTDIR)\Conxlo2.obj"
	-@erase "$(INTDIR)\Globals2.obj"
	-@erase "$(INTDIR)\Iohi2.obj"
	-@erase "$(INTDIR)\Iolo2.obj"
	-@erase "$(INTDIR)\Nial2.obj"
	-@erase "$(INTDIR)\Nsltwo2.obj"
	-@erase "$(INTDIR)\Nslusrfn.obj"
	-@erase "$(INTDIR)\Pathhi2.obj"
	-@erase "$(INTDIR)\Pathlo2.obj"
	-@erase "$(INTDIR)\Thirdpa2.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\Yacclo2.obj"
	-@erase "$(INTDIR)\Ytab2.obj"
	-@erase "$(OUTDIR)\nsl2.exe"
	-@erase "$(OUTDIR)\nsl2.ilk"
	-@erase "$(OUTDIR)\nsl2.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/nsl2.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/nsl2.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/nsl2.pdb" /debug /machine:I386 /out:"$(OUTDIR)/nsl2.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Conxhi2.obj" \
	"$(INTDIR)\Conxlo2.obj" \
	"$(INTDIR)\Globals2.obj" \
	"$(INTDIR)\Iohi2.obj" \
	"$(INTDIR)\Iolo2.obj" \
	"$(INTDIR)\Nial2.obj" \
	"$(INTDIR)\Nsltwo2.obj" \
	"$(INTDIR)\Nslusrfn.obj" \
	"$(INTDIR)\Pathhi2.obj" \
	"$(INTDIR)\Pathlo2.obj" \
	"$(INTDIR)\Thirdpa2.obj" \
	"$(INTDIR)\Yacclo2.obj" \
	"$(INTDIR)\Ytab2.obj"

"$(OUTDIR)\nsl2.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "nsl2 - Win32 Release"
# Name "nsl2 - Win32 Debug"

!IF  "$(CFG)" == "nsl2 - Win32 Release"

!ELSEIF  "$(CFG)" == "nsl2 - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Ytab2.c
DEP_CPP_YTAB2=\
	".\globals2.h"\
	".\iolo2.h"\
	".\nial2.h"\
	".\nsltwo2.h"\
	".\nslyacc2.h"\
	".\pathlo2.h"\
	

"$(INTDIR)\Ytab2.obj" : $(SOURCE) $(DEP_CPP_YTAB2) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Conxlo2.c
DEP_CPP_CONXL=\
	".\conxlo2.h"\
	".\globals2.h"\
	".\iohi2.h"\
	".\iolo2.h"\
	".\nial2.h"\
	".\nsltwo2.h"\
	".\nslyacc2.h"\
	".\pathhi2.h"\
	".\pathlo2.h"\
	

"$(INTDIR)\Conxlo2.obj" : $(SOURCE) $(DEP_CPP_CONXL) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Globals2.c
DEP_CPP_GLOBA=\
	".\globals2.h"\
	".\iohi2.h"\
	".\iolo2.h"\
	".\nial2.h"\
	".\nsltwo2.h"\
	".\pathlo2.h"\
	

"$(INTDIR)\Globals2.obj" : $(SOURCE) $(DEP_CPP_GLOBA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Iohi2.c
DEP_CPP_IOHI2=\
	".\globals2.h"\
	".\iohi2.h"\
	".\iolo2.h"\
	".\nial2.h"\
	".\nsltwo2.h"\
	".\pathlo2.h"\
	

"$(INTDIR)\Iohi2.obj" : $(SOURCE) $(DEP_CPP_IOHI2) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Iolo2.c
DEP_CPP_IOLO2=\
	".\globals2.h"\
	".\iolo2.h"\
	".\nial2.h"\
	".\nsltwo2.h"\
	".\pathlo2.h"\
	

"$(INTDIR)\Iolo2.obj" : $(SOURCE) $(DEP_CPP_IOLO2) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Nial2.c
DEP_CPP_NIAL2=\
	".\conxlo2.h"\
	".\globals2.h"\
	".\iohi2.h"\
	".\iolo2.h"\
	".\nial2.h"\
	".\nsltwo2.h"\
	".\pathlo2.h"\
	

"$(INTDIR)\Nial2.obj" : $(SOURCE) $(DEP_CPP_NIAL2) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Nsltwo2.c
DEP_CPP_NSLTW=\
	".\globals2.h"\
	".\iohi2.h"\
	".\iolo2.h"\
	".\nial2.h"\
	".\nsltwo2.h"\
	".\nslyacc2.h"\
	".\pathhi2.h"\
	

"$(INTDIR)\Nsltwo2.obj" : $(SOURCE) $(DEP_CPP_NSLTW) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Nslusrfn.c

"$(INTDIR)\Nslusrfn.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Pathhi2.c
DEP_CPP_PATHH=\
	".\conxhi2.h"\
	".\conxlo2.h"\
	".\globals2.h"\
	".\iohi2.h"\
	".\iolo2.h"\
	".\nial2.h"\
	".\nsltwo2.h"\
	".\nslyacc2.h"\
	".\pathhi2.h"\
	".\pathlo2.h"\
	

"$(INTDIR)\Pathhi2.obj" : $(SOURCE) $(DEP_CPP_PATHH) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Pathlo2.c
DEP_CPP_PATHL=\
	".\conxhi2.h"\
	".\conxlo2.h"\
	".\globals2.h"\
	".\iohi2.h"\
	".\iolo2.h"\
	".\nial2.h"\
	".\nsltwo2.h"\
	".\nslyacc2.h"\
	".\pathhi2.h"\
	".\pathlo2.h"\
	

"$(INTDIR)\Pathlo2.obj" : $(SOURCE) $(DEP_CPP_PATHL) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thirdpa2.c
DEP_CPP_THIRD=\
	".\conxhi2.h"\
	".\conxlo2.h"\
	".\globals2.h"\
	".\iohi2.h"\
	".\iolo2.h"\
	".\nial2.h"\
	".\nsltwo2.h"\
	".\nslyacc2.h"\
	".\pathhi2.h"\
	".\pathlo2.h"\
	

"$(INTDIR)\Thirdpa2.obj" : $(SOURCE) $(DEP_CPP_THIRD) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Yacclo2.c
DEP_CPP_YACCL=\
	".\globals2.h"\
	".\iohi2.h"\
	".\iolo2.h"\
	".\nial2.h"\
	".\nsltwo2.h"\
	".\nslyacc2.h"\
	".\pathhi2.h"\
	".\pathlo2.h"\
	".\ytab2.h"\
	

"$(INTDIR)\Yacclo2.obj" : $(SOURCE) $(DEP_CPP_YACCL) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Conxhi2.c
DEP_CPP_CONXH=\
	".\conxhi2.h"\
	".\conxlo2.h"\
	".\globals2.h"\
	".\iohi2.h"\
	".\iolo2.h"\
	".\nial2.h"\
	".\nsltwo2.h"\
	".\nslyacc2.h"\
	".\pathhi2.h"\
	".\pathlo2.h"\
	

"$(INTDIR)\Conxhi2.obj" : $(SOURCE) $(DEP_CPP_CONXH) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
