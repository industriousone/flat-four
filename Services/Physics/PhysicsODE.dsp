# Microsoft Developer Studio Project File - Name="PhysicsODE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=PhysicsODE - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PhysicsODE.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PhysicsODE.mak" CFG="PhysicsODE - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PhysicsODE - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PhysicsODE - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PhysicsODE - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../Bin"
# PROP Intermediate_Dir "../../Build/Release_PhysicsODE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Ot /Oi /Oy /Ob1 /I "../../Base" /I ".." /I "../Common/ODE/include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 core.lib math.lib memory.lib platform.lib /nologo /dll /pdb:"../../Build/Release_PhysicsODE/PhysicsODE.pdb" /machine:I386 /implib:"../../Build/Release_PhysicsODE/PhysicsODE.lib" /libpath:"../../Base/Lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PhysicsODE - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../Bin"
# PROP Intermediate_Dir "../../Build/Debug_PhysicsODE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../Base" /I ".." /I "../Common/ODE/include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 core.lib math.lib memory.lib platform.lib /nologo /dll /pdb:"../../Build/Debug_PhysicsODE/PhysicsODE.pdb" /debug /machine:I386 /implib:"../../Build/Debug_PhysicsODE/PhysicsODE.lib" /pdbtype:sept /libpath:"../../Base/Lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "PhysicsODE - Win32 Release"
# Name "PhysicsODE - Win32 Debug"
# Begin Group "Interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\contact_physics.h
# End Source File
# Begin Source File

SOURCE=.\mass_helper.h
# End Source File
# Begin Source File

SOURCE=.\physics.h
# End Source File
# Begin Source File

SOURCE=.\physics_object.h
# End Source File
# Begin Source File

SOURCE=.\rigid_body.h
# End Source File
# End Group
# Begin Group "Sources"

# PROP Default_Filter ""
# Begin Group "ODE"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\Ode\src\array.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Ode\src\error.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Ode\src\fastdot.c
# End Source File
# Begin Source File

SOURCE=..\Common\Ode\src\fastldlt.c
# End Source File
# Begin Source File

SOURCE=..\Common\Ode\src\fastlsolve.c
# End Source File
# Begin Source File

SOURCE=..\Common\Ode\src\fastltsolve.c
# End Source File
# Begin Source File

SOURCE=..\Common\Ode\src\geom.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Ode\src\joint.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Ode\src\lcp.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Ode\src\mass.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Ode\src\mat.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Ode\src\matrix.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Ode\src\memory.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Ode\src\misc.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Ode\src\obstack.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Ode\src\ode.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Ode\src\odemath.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Ode\src\rotation.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Ode\src\space.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Ode\src\step.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Ode\src\testing.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Ode\src\timer.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Src\Ode\contact_physics.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\exports.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ode\impl.h
# End Source File
# Begin Source File

SOURCE=.\Src\Ode\mass_helper.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ode\physics.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ode\rigid_body.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\physics.dox
# End Source File
# End Target
# End Project
