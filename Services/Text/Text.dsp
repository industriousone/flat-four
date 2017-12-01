# Microsoft Developer Studio Project File - Name="Text" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Text - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Text.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Text.mak" CFG="Text - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Text - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Text - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Text - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../Bin"
# PROP Intermediate_Dir "../../Build/Release_Text"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Ot /Oi /Oy /Ob1 /I "./FreeType/include" /I "../../Base" /I ".." /I "./Src/FreeType/include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 core.lib memory.lib platform.lib /nologo /dll /pdb:"../../Build/Release_Text/Text.pdb" /machine:I386 /implib:"../../Build/Release_Text/Text.lib" /libpath:"../../Base/Lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Text - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../Bin"
# PROP Intermediate_Dir "../../Build/Debug_Text"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./Src/FreeType/include" /I "../../Base" /I ".." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 core.lib memory.lib platform.lib /nologo /dll /pdb:"../../Build/Debug_Text/Text.pdb" /debug /machine:I386 /implib:"../../Build/Debug_Text/Text.lib" /pdbtype:sept /libpath:"../../Base/Lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Text - Win32 Release"
# Name "Text - Win32 Debug"
# Begin Group "Interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\font_factory.h
# End Source File
# Begin Source File

SOURCE=.\texture_font.h
# End Source File
# Begin Source File

SOURCE=.\visual.h
# End Source File
# End Group
# Begin Group "Sources"

# PROP Default_Filter ""
# Begin Group "FreeType"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\FreeType\src\autohint\autohint.c
# End Source File
# Begin Source File

SOURCE=.\Src\FreeType\src\base\ftbase.c
# End Source File
# Begin Source File

SOURCE=.\Src\FreeType\src\base\ftglyph.c
# End Source File
# Begin Source File

SOURCE=.\Src\FreeType\src\base\ftinit.c
# End Source File
# Begin Source File

SOURCE=.\Src\FreeType\src\sfnt\sfnt.c
# End Source File
# Begin Source File

SOURCE=.\Src\FreeType\src\smooth\smooth.c
# End Source File
# Begin Source File

SOURCE=.\Src\FreeType\src\truetype\truetype.c
# End Source File
# Begin Source File

SOURCE=.\Src\FreeType\src\type1\type1.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\Src\exports.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\f4system.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\font_factory.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\texture_font.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\visual.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\text.dox
# End Source File
# End Target
# End Project
