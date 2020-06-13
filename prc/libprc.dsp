# Microsoft Developer Studio Project File - Name="libprc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=libprc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libprc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libprc.mak" CFG="libprc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libprc - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libprc - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Tamarind/libprc", UOAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libprc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBPRC_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "libprc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBPRC_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "libprc - Win32 Release"
# Name "libprc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\appiset.c
# End Source File
# Begin Source File

SOURCE=.\attrtos.c
# End Source File
# Begin Source File

SOURCE=.\bufto.c
# End Source File
# Begin Source File

SOURCE=.\close.c
# End Source File
# Begin Source File

SOURCE=.\create.c
# End Source File
# Begin Source File

SOURCE=.\destroy.c
# End Source File
# Begin Source File

SOURCE=.\dllmain.c
# End Source File
# Begin Source File

SOURCE=.\errno.c
# End Source File
# Begin Source File

SOURCE=.\flush.c
# End Source File
# Begin Source File

SOURCE=.\fpopen.c
# End Source File
# Begin Source File

SOURCE=.\frhead.c
# End Source File
# Begin Source File

SOURCE=.\frrec.c
# End Source File
# Begin Source File

SOURCE=.\frrsrc.c
# End Source File
# Begin Source File

SOURCE=.\fwhead.c
# End Source File
# Begin Source File

SOURCE=.\fwrec.c
# End Source File
# Begin Source File

SOURCE=.\fwrsrc.c
# End Source File
# Begin Source File

SOURCE=.\ihead.c
# End Source File
# Begin Source File

SOURCE=.\open.c
# End Source File
# Begin Source File

SOURCE=.\recapp.c
# End Source File
# Begin Source File

SOURCE=.\recdes.c
# End Source File
# Begin Source File

SOURCE=.\recget.c
# End Source File
# Begin Source File

SOURCE=.\recmap.c
# End Source File
# Begin Source File

SOURCE=.\recnew.c
# End Source File
# Begin Source File

SOURCE=.\recnext.c
# End Source File
# Begin Source File

SOURCE=.\recrem.c
# End Source File
# Begin Source File

SOURCE=.\recset.c
# End Source File
# Begin Source File

SOURCE=.\rsrcapp.c
# End Source File
# Begin Source File

SOURCE=.\rsrcdes.c
# End Source File
# Begin Source File

SOURCE=.\rsrcget.c
# End Source File
# Begin Source File

SOURCE=.\rsrcmap.c
# End Source File
# Begin Source File

SOURCE=.\rsrcnew.c
# End Source File
# Begin Source File

SOURCE=.\rsrcnext.c
# End Source File
# Begin Source File

SOURCE=.\rsrcrem.c
# End Source File
# Begin Source File

SOURCE=.\rsrcset.c
# End Source File
# Begin Source File

SOURCE=.\setname.c
# End Source File
# Begin Source File

SOURCE=.\sortiset.c
# End Source File
# Begin Source File

SOURCE=.\stotime.c
# End Source File
# Begin Source File

SOURCE=.\stotype.c
# End Source File
# Begin Source File

SOURCE=.\strerror.c
# End Source File
# Begin Source File

SOURCE=.\strmread.c
# End Source File
# Begin Source File

SOURCE=.\strmwrit.c
# End Source File
# Begin Source File

SOURCE=.\time.c
# End Source File
# Begin Source File

SOURCE=.\timetos.c
# End Source File
# Begin Source File

SOURCE=.\tobuf.c
# End Source File
# Begin Source File

SOURCE=.\typetos.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\prc.h
# End Source File
# Begin Source File

SOURCE=.\prcp.h
# End Source File
# Begin Source File

SOURCE=.\prcsys.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
