# Microsoft Developer Studio Project File - Name="ColliderODE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ColliderODE - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ColliderODE.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ColliderODE.mak" CFG="ColliderODE - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ColliderODE - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ColliderODE - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ColliderODE - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../Bin"
# PROP Intermediate_Dir "../../Build/Release_ColliderODE"
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
# ADD LINK32 core.lib memory.lib math.lib platform.lib /nologo /dll /pdb:"../../Build/Release_ColliderODE/ColliderODE.pdb" /machine:I386 /implib:"../../Build/Release_ColliderODE/ColliderODE.lib" /libpath:"../../Base/Lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ColliderODE - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../Bin"
# PROP Intermediate_Dir "../../Build/Debug_ColliderODE"
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
# ADD LINK32 core.lib memory.lib math.lib platform.lib /nologo /dll /pdb:"../../Build/Debug_ColliderODE/ColliderODE.pdb" /debug /machine:I386 /implib:"../../Build/Debug_ColliderODE/ColliderODE.lib" /pdbtype:sept /libpath:"../../Base/Lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ColliderODE - Win32 Release"
# Name "ColliderODE - Win32 Debug"
# Begin Group "Interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\box.h
# End Source File
# Begin Source File

SOURCE=.\capsule.h
# End Source File
# Begin Source File

SOURCE=.\collider.h
# End Source File
# Begin Source File

SOURCE=.\contact_list.h
# End Source File
# Begin Source File

SOURCE=.\mesh.h
# End Source File
# Begin Source File

SOURCE=.\plane.h
# End Source File
# Begin Source File

SOURCE=.\shape.h
# End Source File
# Begin Source File

SOURCE=.\sphere.h
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
# Begin Group "OPCODE"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\Opcode\IceAABB.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Opcode\IceContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Opcode\IceMatrix3x3.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Opcode\IceMatrix4x4.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Opcode\IcePoint.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Opcode\IceTriangle.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Opcode\OPC_AABBTree.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Opcode\OPC_Collider.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Opcode\OPC_Common.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Opcode\OPC_Model.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Opcode\OPC_OptimizedTree.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Opcode\OPC_RayCollider.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Opcode\OPC_SphereCollider.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Opcode\OPC_TreeBuilders.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Opcode\OPC_TreeCollider.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Opcode\Opcode.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Src\Ode\box.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ode\capsule.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ode\collider.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\contact_list.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\exports.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\mesh.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ode\plane.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Ode\sphere.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\collider.dox
# End Source File
# End Target
# End Project
