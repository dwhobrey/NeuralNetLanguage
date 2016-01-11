# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

!IF "$(CFG)" == ""
CFG=cpp - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to cpp - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "cpp - Win32 Release" && "$(CFG)" != "cpp - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "cpp.mak" CFG="cpp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cpp - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "cpp - Win32 Debug" (based on "Win32 (x86) Console Application")
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
# PROP Target_Last_Scanned "cpp - Win32 Debug"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cpp - Win32 Release"

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

ALL : "$(OUTDIR)\cpp.exe"

CLEAN : 
	-@erase "$(INTDIR)\alloc.obj"
	-@erase "$(INTDIR)\Comment.obj"
	-@erase "$(INTDIR)\Define.obj"
	-@erase "$(INTDIR)\Hash.obj"
	-@erase "$(INTDIR)\If_expr.obj"
	-@erase "$(INTDIR)\Include.obj"
	-@erase "$(INTDIR)\Input.obj"
	-@erase "$(INTDIR)\Macro.obj"
	-@erase "$(INTDIR)\Main.obj"
	-@erase "$(INTDIR)\Pound.obj"
	-@erase "$(INTDIR)\Process.obj"
	-@erase "$(INTDIR)\Token.obj"
	-@erase "$(INTDIR)\Utils.obj"
	-@erase "$(INTDIR)\Ztype.obj"
	-@erase "$(OUTDIR)\cpp.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/cpp.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/cpp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:no\
 /pdb:"$(OUTDIR)/cpp.pdb" /machine:I386 /out:"$(OUTDIR)/cpp.exe" 
LINK32_OBJS= \
	"$(INTDIR)\alloc.obj" \
	"$(INTDIR)\Comment.obj" \
	"$(INTDIR)\Define.obj" \
	"$(INTDIR)\Hash.obj" \
	"$(INTDIR)\If_expr.obj" \
	"$(INTDIR)\Include.obj" \
	"$(INTDIR)\Input.obj" \
	"$(INTDIR)\Macro.obj" \
	"$(INTDIR)\Main.obj" \
	"$(INTDIR)\Pound.obj" \
	"$(INTDIR)\Process.obj" \
	"$(INTDIR)\Token.obj" \
	"$(INTDIR)\Utils.obj" \
	"$(INTDIR)\Ztype.obj"

"$(OUTDIR)\cpp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "cpp - Win32 Debug"

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

ALL : "$(OUTDIR)\cpp.exe"

CLEAN : 
	-@erase "$(INTDIR)\alloc.obj"
	-@erase "$(INTDIR)\Comment.obj"
	-@erase "$(INTDIR)\Define.obj"
	-@erase "$(INTDIR)\Hash.obj"
	-@erase "$(INTDIR)\If_expr.obj"
	-@erase "$(INTDIR)\Include.obj"
	-@erase "$(INTDIR)\Input.obj"
	-@erase "$(INTDIR)\Macro.obj"
	-@erase "$(INTDIR)\Main.obj"
	-@erase "$(INTDIR)\Pound.obj"
	-@erase "$(INTDIR)\Process.obj"
	-@erase "$(INTDIR)\Token.obj"
	-@erase "$(INTDIR)\Utils.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\Ztype.obj"
	-@erase "$(OUTDIR)\cpp.exe"
	-@erase "$(OUTDIR)\cpp.ilk"
	-@erase "$(OUTDIR)\cpp.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /Fp"$(INTDIR)/cpp.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/cpp.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)/cpp.pdb" /debug /machine:I386 /out:"$(OUTDIR)/cpp.exe" 
LINK32_OBJS= \
	"$(INTDIR)\alloc.obj" \
	"$(INTDIR)\Comment.obj" \
	"$(INTDIR)\Define.obj" \
	"$(INTDIR)\Hash.obj" \
	"$(INTDIR)\If_expr.obj" \
	"$(INTDIR)\Include.obj" \
	"$(INTDIR)\Input.obj" \
	"$(INTDIR)\Macro.obj" \
	"$(INTDIR)\Main.obj" \
	"$(INTDIR)\Pound.obj" \
	"$(INTDIR)\Process.obj" \
	"$(INTDIR)\Token.obj" \
	"$(INTDIR)\Utils.obj" \
	"$(INTDIR)\Ztype.obj"

"$(OUTDIR)\cpp.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "cpp - Win32 Release"
# Name "cpp - Win32 Debug"

!IF  "$(CFG)" == "cpp - Win32 Release"

!ELSEIF  "$(CFG)" == "cpp - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Ztype.c
DEP_CPP_ZTYPE=\
	".\Global.h"\
	".\ztype.h"\
	

"$(INTDIR)\Ztype.obj" : $(SOURCE) $(DEP_CPP_ZTYPE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Define.c
DEP_CPP_DEFIN=\
	".\Global.h"\
	

"$(INTDIR)\Define.obj" : $(SOURCE) $(DEP_CPP_DEFIN) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Hash.c
DEP_CPP_HASH_=\
	".\alloc.h"\
	".\Global.h"\
	".\ztype.h"\
	

"$(INTDIR)\Hash.obj" : $(SOURCE) $(DEP_CPP_HASH_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\If_expr.c
DEP_CPP_IF_EX=\
	".\Global.h"\
	".\ztype.h"\
	

"$(INTDIR)\If_expr.obj" : $(SOURCE) $(DEP_CPP_IF_EX) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Include.c
DEP_CPP_INCLU=\
	".\Global.h"\
	

"$(INTDIR)\Include.obj" : $(SOURCE) $(DEP_CPP_INCLU) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Input.c
DEP_CPP_INPUT=\
	".\Global.h"\
	

"$(INTDIR)\Input.obj" : $(SOURCE) $(DEP_CPP_INPUT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Macro.c
DEP_CPP_MACRO=\
	".\alloc.h"\
	".\Global.h"\
	

"$(INTDIR)\Macro.obj" : $(SOURCE) $(DEP_CPP_MACRO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Main.c
DEP_CPP_MAIN_=\
	".\Global.h"\
	".\Patchlev.h"\
	".\ztype.h"\
	

"$(INTDIR)\Main.obj" : $(SOURCE) $(DEP_CPP_MAIN_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Pound.c
DEP_CPP_POUND=\
	".\Global.h"\
	

"$(INTDIR)\Pound.obj" : $(SOURCE) $(DEP_CPP_POUND) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Process.c
DEP_CPP_PROCE=\
	".\Global.h"\
	

"$(INTDIR)\Process.obj" : $(SOURCE) $(DEP_CPP_PROCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Token.c
DEP_CPP_TOKEN=\
	".\alloc.h"\
	".\Global.h"\
	".\ztype.h"\
	

"$(INTDIR)\Token.obj" : $(SOURCE) $(DEP_CPP_TOKEN) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Utils.c
DEP_CPP_UTILS=\
	".\Global.h"\
	".\ztype.h"\
	

"$(INTDIR)\Utils.obj" : $(SOURCE) $(DEP_CPP_UTILS) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Comment.c
DEP_CPP_COMME=\
	".\Global.h"\
	

"$(INTDIR)\Comment.obj" : $(SOURCE) $(DEP_CPP_COMME) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\alloc.c
DEP_CPP_ALLOC=\
	".\Global.h"\
	

"$(INTDIR)\alloc.obj" : $(SOURCE) $(DEP_CPP_ALLOC) "$(INTDIR)"


# End Source File
# End Target
# End Project
################################################################################
