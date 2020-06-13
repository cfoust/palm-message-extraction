# Microsoft Developer Studio Project File - Name="par" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=PAR - WIN32 RELEASE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "par.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "par.mak" CFG="PAR - WIN32 RELEASE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "par - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "par - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "par - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "PRC_WIN32_STATIC_BUILD" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 setargv.obj kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "PRC_WIN32_STATIC_BUILD" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 setargv.obj kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "par - Win32 Release"
# Name "par - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\appiset.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\attrtos.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\bufto.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\close.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\create.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\destroy.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\errno.c
# End Source File
# Begin Source File

SOURCE=.\flush.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\fpopen.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\frhead.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\frrec.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\frrsrc.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\fwhead.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\fwrec.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\fwrsrc.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ihead.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\misc.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\nrecord.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\open.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\par.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\recapp.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\recdes.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\recget.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\recmap.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\recnew.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\recnext.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\recrem.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\recset.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rsrcapp.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rsrcdes.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rsrcget.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rsrcmap.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rsrcnew.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rsrcnext.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rsrcrem.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\rsrcset.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\setname.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\sortiset.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\stotime.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\stotype.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

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

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\timetos.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\tobuf.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\typetos.c

!IF  "$(CFG)" == "par - Win32 Release"

!ELSEIF  "$(CFG)" == "par - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

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

SOURCE=.\prcpsys.h
# End Source File
# End Group
# End Target
# End Project
