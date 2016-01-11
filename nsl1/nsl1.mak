# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=nsl1 - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to nsl1 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "nsl1 - Win32 Release" && "$(CFG)" != "nsl1 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "nsl1.mak" CFG="nsl1 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "nsl1 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "nsl1 - Win32 Debug" (based on "Win32 (x86) Console Application")
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
# PROP Target_Last_Scanned "nsl1 - Win32 Debug"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "nsl1 - Win32 Release"

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

ALL : "$(OUTDIR)\nsl1.exe"

CLEAN : 
	-@erase "$(INTDIR)\Firstpa.obj"
	-@erase "$(INTDIR)\General.obj"
	-@erase "$(INTDIR)\Globals.obj"
	-@erase "$(INTDIR)\Iolo.obj"
	-@erase "$(INTDIR)\Nslone.obj"
	-@erase "$(INTDIR)\Secondpa.obj"
	-@erase "$(INTDIR)\Yacclo.obj"
	-@erase "$(INTDIR)\Ytab.obj"
	-@erase "$(OUTDIR)\nsl1.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/nsl1.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/nsl1.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/nsl1.pdb" /machine:I386 /out:"$(OUTDIR)/nsl1.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Firstpa.obj" \
	"$(INTDIR)\General.obj" \
	"$(INTDIR)\Globals.obj" \
	"$(INTDIR)\Iolo.obj" \
	"$(INTDIR)\Nslone.obj" \
	"$(INTDIR)\Secondpa.obj" \
	"$(INTDIR)\Yacclo.obj" \
	"$(INTDIR)\Ytab.obj"

"$(OUTDIR)\nsl1.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "nsl1 - Win32 Debug"

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

ALL : "$(OUTDIR)\nsl1.exe"

CLEAN : 
	-@erase "$(INTDIR)\Firstpa.obj"
	-@erase "$(INTDIR)\General.obj"
	-@erase "$(INTDIR)\Globals.obj"
	-@erase "$(INTDIR)\Iolo.obj"
	-@erase "$(INTDIR)\Nslone.obj"
	-@erase "$(INTDIR)\Secondpa.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\Yacclo.obj"
	-@erase "$(INTDIR)\Ytab.obj"
	-@erase "$(OUTDIR)\nsl1.exe"
	-@erase "$(OUTDIR)\nsl1.ilk"
	-@erase "$(OUTDIR)\nsl1.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/nsl1.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/nsl1.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/nsl1.pdb" /debug /machine:I386 /out:"$(OUTDIR)/nsl1.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Firstpa.obj" \
	"$(INTDIR)\General.obj" \
	"$(INTDIR)\Globals.obj" \
	"$(INTDIR)\Iolo.obj" \
	"$(INTDIR)\Nslone.obj" \
	"$(INTDIR)\Secondpa.obj" \
	"$(INTDIR)\Yacclo.obj" \
	"$(INTDIR)\Ytab.obj"

"$(OUTDIR)\nsl1.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "nsl1 - Win32 Release"
# Name "nsl1 - Win32 Debug"

!IF  "$(CFG)" == "nsl1 - Win32 Release"

!ELSEIF  "$(CFG)" == "nsl1 - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Ytab.c

!IF  "$(CFG)" == "nsl1 - Win32 Release"

DEP_CPP_YTAB_=\
	".\general.h"\
	".\globals.h"\
	".\iolo.h"\
	".\nslone.h"\
	".\nslyacc.h"\
	

"$(INTDIR)\Ytab.obj" : $(SOURCE) $(DEP_CPP_YTAB_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "nsl1 - Win32 Debug"

DEP_CPP_YTAB_=\
	".\general.h"\
	".\globals.h"\
	".\iolo.h"\
	".\nslone.h"\
	".\nslyacc.h"\
	

"$(INTDIR)\Ytab.obj" : $(SOURCE) $(DEP_CPP_YTAB_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\General.c
DEP_CPP_GENER=\
	".\globals.h"\
	".\iolo.h"\
	".\nslone.h"\
	".\nslyacc.h"\
	

"$(INTDIR)\General.obj" : $(SOURCE) $(DEP_CPP_GENER) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Globals.c
DEP_CPP_GLOBA=\
	".\general.h"\
	".\globals.h"\
	".\iolo.h"\
	".\nslone.h"\
	

"$(INTDIR)\Globals.obj" : $(SOURCE) $(DEP_CPP_GLOBA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Iolo.c
DEP_CPP_IOLO_=\
	".\general.h"\
	".\globals.h"\
	".\iolo.h"\
	".\nslone.h"\
	".\nslyacc.h"\
	

"$(INTDIR)\Iolo.obj" : $(SOURCE) $(DEP_CPP_IOLO_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Nslone.c
DEP_CPP_NSLON=\
	".\general.h"\
	".\globals.h"\
	".\iolo.h"\
	".\nslone.h"\
	".\nslyacc.h"\
	

"$(INTDIR)\Nslone.obj" : $(SOURCE) $(DEP_CPP_NSLON) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Secondpa.c
DEP_CPP_SECON=\
	".\general.h"\
	".\globals.h"\
	".\iolo.h"\
	".\nslone.h"\
	".\nslyacc.h"\
	

"$(INTDIR)\Secondpa.obj" : $(SOURCE) $(DEP_CPP_SECON) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Yacclo.c

!IF  "$(CFG)" == "nsl1 - Win32 Release"

DEP_CPP_YACCL=\
	".\general.h"\
	".\globals.h"\
	".\iolo.h"\
	".\nslone.h"\
	".\nslyacc.h"\
	".\ytab.h"\
	

"$(INTDIR)\Yacclo.obj" : $(SOURCE) $(DEP_CPP_YACCL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "nsl1 - Win32 Debug"

DEP_CPP_YACCL=\
	".\general.h"\
	".\globals.h"\
	".\iolo.h"\
	".\nslone.h"\
	".\nslyacc.h"\
	".\ytab.h"\
	

"$(INTDIR)\Yacclo.obj" : $(SOURCE) $(DEP_CPP_YACCL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Firstpa.c
DEP_CPP_FIRST=\
	".\general.h"\
	".\globals.h"\
	".\iolo.h"\
	".\nslone.h"\
	".\nslyacc.h"\
	

"$(INTDIR)\Firstpa.obj" : $(SOURCE) $(DEP_CPP_FIRST) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
