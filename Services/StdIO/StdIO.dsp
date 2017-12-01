# Microsoft Developer Studio Project File - Name="StdIO" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=StdIO - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "StdIO.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "StdIO.mak" CFG="StdIO - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "StdIO - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "StdIO - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "StdIO - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../Bin"
# PROP Intermediate_Dir "../../Build/Release_StdIO"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Ot /Oi /Oy /Ob1 /I "../../Base" /I ".." /I "./Src" /I "./Src/libpng" /I "./Src/zlib" /I "./Src/lib3ds" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 core.lib memory.lib platform.lib /nologo /dll /pdb:"../../Build/Release_StdIO/StdIO.pdb" /machine:I386 /implib:"../../Build/Release_StdIO/StdIO.lib" /libpath:"../../Base/Lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "StdIO - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../Bin"
# PROP Intermediate_Dir "../../Build/Debug_StdIO"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../Base" /I ".." /I "./Src" /I "./Src/libpng" /I "./Src/zlib" /I "./Src/lib3ds" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 core.lib memory.lib platform.lib /nologo /dll /pdb:"../../Build/Debug_StdIO/StdIO.pdb" /debug /machine:I386 /implib:"../../Build/Debug_StdIO/StdIO.lib" /pdbtype:sept /libpath:"../../Base/Lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "StdIO - Win32 Release"
# Name "StdIO - Win32 Debug"
# Begin Group "Interface"

# PROP Default_Filter ""
# End Group
# Begin Group "Sources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\exports.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\flt_zlib.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\register.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\rw_4fnt.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\rw_bmp.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\rw_obj.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\rw_png.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\rw_tga.cpp
# End Source File
# End Group
# Begin Group "Libs"

# PROP Default_Filter ""
# Begin Group "libpng"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\libpng\png.c
# End Source File
# Begin Source File

SOURCE=.\Src\libpng\pngerror.c
# End Source File
# Begin Source File

SOURCE=.\Src\libpng\pngget.c
# End Source File
# Begin Source File

SOURCE=.\Src\libpng\pngmem.c
# End Source File
# Begin Source File

SOURCE=.\Src\libpng\pngpread.c
# End Source File
# Begin Source File

SOURCE=.\Src\libpng\pngread.c
# End Source File
# Begin Source File

SOURCE=.\Src\libpng\pngrio.c
# End Source File
# Begin Source File

SOURCE=.\Src\libpng\pngrtran.c
# End Source File
# Begin Source File

SOURCE=.\Src\libpng\pngrutil.c
# End Source File
# Begin Source File

SOURCE=.\Src\libpng\pngset.c
# End Source File
# Begin Source File

SOURCE=.\Src\libpng\pngtrans.c
# End Source File
# Begin Source File

SOURCE=.\Src\libpng\pngwio.c
# End Source File
# Begin Source File

SOURCE=.\Src\libpng\pngwrite.c
# End Source File
# Begin Source File

SOURCE=.\Src\libpng\pngwtran.c
# End Source File
# Begin Source File

SOURCE=.\Src\libpng\pngwutil.c
# End Source File
# End Group
# Begin Group "zlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\zlib\adler32.c
# End Source File
# Begin Source File

SOURCE=.\Src\zlib\compress.c
# End Source File
# Begin Source File

SOURCE=.\Src\zlib\crc32.c
# End Source File
# Begin Source File

SOURCE=.\Src\zlib\deflate.c
# End Source File
# Begin Source File

SOURCE=.\Src\zlib\gzio.c
# End Source File
# Begin Source File

SOURCE=.\Src\zlib\infblock.c
# End Source File
# Begin Source File

SOURCE=.\Src\zlib\infcodes.c
# End Source File
# Begin Source File

SOURCE=.\Src\zlib\inffast.c
# End Source File
# Begin Source File

SOURCE=.\Src\zlib\inflate.c
# End Source File
# Begin Source File

SOURCE=.\Src\zlib\inftrees.c
# End Source File
# Begin Source File

SOURCE=.\Src\zlib\infutil.c
# End Source File
# Begin Source File

SOURCE=.\Src\zlib\trees.c
# End Source File
# Begin Source File

SOURCE=.\Src\zlib\uncompr.c
# End Source File
# Begin Source File

SOURCE=.\Src\zlib\zutil.c
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\std_io.dox
# End Source File
# End Target
# End Project
