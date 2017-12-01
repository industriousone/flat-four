# Microsoft Developer Studio Project File - Name="Scene" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Scene - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Scene.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Scene.mak" CFG="Scene - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Scene - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Scene - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Scene - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../Bin"
# PROP Intermediate_Dir "../../Build/Release_Scene"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Ot /Oi /Oy /Ob1 /I "../../Base" /I ".." /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 core.lib math.lib memory.lib platform.lib /nologo /dll /pdb:"../../Build/Release_Scene/Scene.pdb" /machine:I386 /implib:"../../Build/Release_Scene/Scene.lib" /libpath:"../../Base/Lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Scene - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../Bin"
# PROP Intermediate_Dir "../../Build/Debug_Scene"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../Base" /I ".." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 core.lib math.lib memory.lib platform.lib /nologo /dll /pdb:"../../Build/Debug_Scene/Scene.pdb" /debug /machine:I386 /implib:"../../Build/Debug_Scene/Scene.lib" /pdbtype:sept /libpath:"../../Base/Lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Scene - Win32 Release"
# Name "Scene - Win32 Debug"
# Begin Group "Interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\attribute.h
# End Source File
# Begin Source File

SOURCE=.\body.h
# End Source File
# Begin Source File

SOURCE=.\camera.h
# End Source File
# Begin Source File

SOURCE=.\collision_visitor.h
# End Source File
# Begin Source File

SOURCE=.\entity.h
# End Source File
# Begin Source File

SOURCE=.\event_visitor.h
# End Source File
# Begin Source File

SOURCE=.\perspective_camera.h
# End Source File
# Begin Source File

SOURCE=.\physics.h
# End Source File
# Begin Source File

SOURCE=.\pose.h
# End Source File
# Begin Source File

SOURCE=.\render_visitor.h
# End Source File
# Begin Source File

SOURCE=.\space.h
# End Source File
# Begin Source File

SOURCE=.\surface.h
# End Source File
# Begin Source File

SOURCE=.\update_visitor.h
# End Source File
# Begin Source File

SOURCE=.\visitor.h
# End Source File
# End Group
# Begin Group "Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\body.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\collision_visitor.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\entity.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\event_visitor.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\exports.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\perspective_camera.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\physics.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\pose.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\render_visitor.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\simple_space.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\surface.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\update_visitor.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\scene.dox
# End Source File
# End Target
# End Project
