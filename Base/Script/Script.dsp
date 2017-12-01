# Microsoft Developer Studio Project File - Name="Script" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Script - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Script.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Script.mak" CFG="Script - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Script - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Script - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Script - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../Bin"
# PROP Intermediate_Dir "../../Build/Release_Script"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Ot /Oi /Oy /Ob1 /I ".." /I "./Src/Lua/Lua/include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 core.lib memory.lib platform.lib math.lib /nologo /dll /pdb:"../../Build/Release_Script/Script.pdb" /machine:I386 /implib:"../../Build/Release_Script/Script.lib" /libpath:"../Lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Script - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../Bin"
# PROP Intermediate_Dir "../../Build/Debug_Script"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".." /I "./Src/Lua/Lua/include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 core.lib memory.lib platform.lib math.lib /nologo /dll /pdb:"../../Build/Debug_Script/Script.pdb" /debug /machine:I386 /implib:"../../Build/Debug_Script/Script.lib" /pdbtype:sept /libpath:"../Lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Script - Win32 Release"
# Name "Script - Win32 Debug"
# Begin Group "Interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\script.h
# End Source File
# End Group
# Begin Group "Sources"

# PROP Default_Filter ""
# Begin Group "Lua"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\lapi.c
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\lib\lauxlib.c
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\lib\lbaselib.c
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\lcode.c
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\ldebug.c
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\ldo.c
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\lfunc.c
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\lgc.c
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\llex.c
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\lib\lmathlib.c
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\lmem.c
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\lobject.c
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\lopcodes.c
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\lparser.c
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\lstate.c
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\lstring.c
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\lib\lstrlib.c
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\ltable.c
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\ltests.c
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\ltm.c
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\lundump.c
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\lvm.c
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\Lua\src\lzio.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\Src\Lua\context.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\extensions.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\extensions.h
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\quaternion.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\refs.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\script.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\thunks.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Lua\vector.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\script.dox
# End Source File
# End Target
# End Project
