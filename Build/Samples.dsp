# Microsoft Developer Studio Project File - Name="Samples" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=Samples - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Samples.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Samples.mak" CFG="Samples - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Samples - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE "Samples - Win32 Debug" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe

!IF  "$(CFG)" == "Samples - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Bin"
# PROP Intermediate_Dir "Release_Samples"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "Samples - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Bin"
# PROP Intermediate_Dir "Debug_Samples"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "Samples - Win32 Release"
# Name "Samples - Win32 Debug"
# Begin Source File

SOURCE=..\Samples\boxes.lua
# End Source File
# Begin Source File

SOURCE=..\Bin\core.cfg
# End Source File
# Begin Source File

SOURCE=..\Samples\fontfactory.lua
# End Source File
# Begin Source File

SOURCE=..\Samples\fork.lua
# End Source File
# Begin Source File

SOURCE=..\Samples\objviewer.lua
# End Source File
# Begin Source File

SOURCE=..\Samples\scene01.lua
# End Source File
# Begin Source File

SOURCE=..\Samples\scene02.lua
# End Source File
# Begin Source File

SOURCE=..\Samples\workspace.lua
# End Source File
# End Target
# End Project
