# Microsoft Developer Studio Project File - Name="png" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=png - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "png.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "png.mak" CFG="png - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "png - Win32 DLL Universal Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE "png - Win32 DLL Universal Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "png - Win32 DLL Universal Release" (based on "Win32 (x86) Application")
!MESSAGE "png - Win32 DLL Universal Debug" (based on "Win32 (x86) Application")
!MESSAGE "png - Win32 DLL Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE "png - Win32 DLL Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "png - Win32 DLL Release" (based on "Win32 (x86) Application")
!MESSAGE "png - Win32 DLL Debug" (based on "Win32 (x86) Application")
!MESSAGE "png - Win32 Universal Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE "png - Win32 Universal Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "png - Win32 Universal Release" (based on "Win32 (x86) Application")
!MESSAGE "png - Win32 Universal Debug" (based on "Win32 (x86) Application")
!MESSAGE "png - Win32 Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE "png - Win32 Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "png - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "png - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "png - Win32 DLL Universal Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_mswunivudll"
# PROP BASE Intermediate_Dir "vc_mswunivudll\png"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vc_mswunivudll"
# PROP Intermediate_Dir "vc_mswunivudll\png"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /Fdvc_mswunivudll\png.pdb /O1 /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswunivu" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MD /Fdvc_mswunivudll\png.pdb /O1 /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswunivu" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswunivu" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x405 /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswunivu" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmswuniv25u_core.lib wxbase25u.lib wxtiff25.lib wxjpeg25.lib wxpng25.lib wxzlib25.lib wxregexu25.lib wxexpat25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivudll\png.exe" /libpath:".\..\..\lib\vc_dll" /subsystem:windows
# ADD LINK32 wxmswuniv25u_core.lib wxbase25u.lib wxtiff25.lib wxjpeg25.lib wxpng25.lib wxzlib25.lib wxregexu25.lib wxexpat25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivudll\png.exe" /libpath:".\..\..\lib\vc_dll" /subsystem:windows

!ELSEIF  "$(CFG)" == "png - Win32 DLL Universal Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswunivuddll"
# PROP BASE Intermediate_Dir "vc_mswunivuddll\png"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswunivuddll"
# PROP Intermediate_Dir "vc_mswunivuddll\png"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswunivuddll\png.pdb /Od /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswunivud" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswunivuddll\png.pdb /Od /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswunivud" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "__WXDEBUG__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswunivud" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "__WXDEBUG__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswunivud" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmswuniv25ud_core.lib wxbase25ud.lib wxtiffd25.lib wxjpegd25.lib wxpngd25.lib wxzlibd25.lib wxregexud25.lib wxexpatd25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivuddll\png.exe" /debug /libpath:".\..\..\lib\vc_dll" /subsystem:windows
# ADD LINK32 wxmswuniv25ud_core.lib wxbase25ud.lib wxtiffd25.lib wxjpegd25.lib wxpngd25.lib wxzlibd25.lib wxregexud25.lib wxexpatd25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivuddll\png.exe" /debug /libpath:".\..\..\lib\vc_dll" /subsystem:windows

!ELSEIF  "$(CFG)" == "png - Win32 DLL Universal Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_mswunivdll"
# PROP BASE Intermediate_Dir "vc_mswunivdll\png"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vc_mswunivdll"
# PROP Intermediate_Dir "vc_mswunivdll\png"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /Fdvc_mswunivdll\png.pdb /O1 /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswuniv" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MD /Fdvc_mswunivdll\png.pdb /O1 /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswuniv" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "__WXMSW__" /d "__WXUNIVERSAL__" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswuniv" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x405 /d "__WXMSW__" /d "__WXUNIVERSAL__" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswuniv" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmswuniv25_core.lib wxbase25.lib wxtiff25.lib wxjpeg25.lib wxpng25.lib wxzlib25.lib wxregex25.lib wxexpat25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivdll\png.exe" /libpath:".\..\..\lib\vc_dll" /subsystem:windows
# ADD LINK32 wxmswuniv25_core.lib wxbase25.lib wxtiff25.lib wxjpeg25.lib wxpng25.lib wxzlib25.lib wxregex25.lib wxexpat25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivdll\png.exe" /libpath:".\..\..\lib\vc_dll" /subsystem:windows

!ELSEIF  "$(CFG)" == "png - Win32 DLL Universal Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswunivddll"
# PROP BASE Intermediate_Dir "vc_mswunivddll\png"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswunivddll"
# PROP Intermediate_Dir "vc_mswunivddll\png"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswunivddll\png.pdb /Od /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswunivd" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswunivddll\png.pdb /Od /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswunivd" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "__WXDEBUG__" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswunivd" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "__WXDEBUG__" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswunivd" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmswuniv25d_core.lib wxbase25d.lib wxtiffd25.lib wxjpegd25.lib wxpngd25.lib wxzlibd25.lib wxregexd25.lib wxexpatd25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivddll\png.exe" /debug /libpath:".\..\..\lib\vc_dll" /subsystem:windows
# ADD LINK32 wxmswuniv25d_core.lib wxbase25d.lib wxtiffd25.lib wxjpegd25.lib wxpngd25.lib wxzlibd25.lib wxregexd25.lib wxexpatd25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivddll\png.exe" /debug /libpath:".\..\..\lib\vc_dll" /subsystem:windows

!ELSEIF  "$(CFG)" == "png - Win32 DLL Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_mswudll"
# PROP BASE Intermediate_Dir "vc_mswudll\png"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vc_mswudll"
# PROP Intermediate_Dir "vc_mswudll\png"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /Fdvc_mswudll\png.pdb /O1 /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswu" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MD /Fdvc_mswudll\png.pdb /O1 /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswu" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "__WXMSW__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswu" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x405 /d "__WXMSW__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswu" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25u_core.lib wxbase25u.lib wxtiff25.lib wxjpeg25.lib wxpng25.lib wxzlib25.lib wxregexu25.lib wxexpat25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswudll\png.exe" /libpath:".\..\..\lib\vc_dll" /subsystem:windows
# ADD LINK32 wxmsw25u_core.lib wxbase25u.lib wxtiff25.lib wxjpeg25.lib wxpng25.lib wxzlib25.lib wxregexu25.lib wxexpat25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswudll\png.exe" /libpath:".\..\..\lib\vc_dll" /subsystem:windows

!ELSEIF  "$(CFG)" == "png - Win32 DLL Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswuddll"
# PROP BASE Intermediate_Dir "vc_mswuddll\png"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswuddll"
# PROP Intermediate_Dir "vc_mswuddll\png"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswuddll\png.pdb /Od /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswud" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswuddll\png.pdb /Od /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswud" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswud" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswud" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25ud_core.lib wxbase25ud.lib wxtiffd25.lib wxjpegd25.lib wxpngd25.lib wxzlibd25.lib wxregexud25.lib wxexpatd25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswuddll\png.exe" /debug /libpath:".\..\..\lib\vc_dll" /subsystem:windows
# ADD LINK32 wxmsw25ud_core.lib wxbase25ud.lib wxtiffd25.lib wxjpegd25.lib wxpngd25.lib wxzlibd25.lib wxregexud25.lib wxexpatd25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswuddll\png.exe" /debug /libpath:".\..\..\lib\vc_dll" /subsystem:windows

!ELSEIF  "$(CFG)" == "png - Win32 DLL Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_mswdll"
# PROP BASE Intermediate_Dir "vc_mswdll\png"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vc_mswdll"
# PROP Intermediate_Dir "vc_mswdll\png"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /Fdvc_mswdll\png.pdb /O1 /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_dll\msw" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MD /Fdvc_mswdll\png.pdb /O1 /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_dll\msw" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "__WXMSW__" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\msw" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x405 /d "__WXMSW__" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\msw" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25_core.lib wxbase25.lib wxtiff25.lib wxjpeg25.lib wxpng25.lib wxzlib25.lib wxregex25.lib wxexpat25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswdll\png.exe" /libpath:".\..\..\lib\vc_dll" /subsystem:windows
# ADD LINK32 wxmsw25_core.lib wxbase25.lib wxtiff25.lib wxjpeg25.lib wxpng25.lib wxzlib25.lib wxregex25.lib wxexpat25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswdll\png.exe" /libpath:".\..\..\lib\vc_dll" /subsystem:windows

!ELSEIF  "$(CFG)" == "png - Win32 DLL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswddll"
# PROP BASE Intermediate_Dir "vc_mswddll\png"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswddll"
# PROP Intermediate_Dir "vc_mswddll\png"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswddll\png.pdb /Od /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswd" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswddll\png.pdb /Od /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_dll\mswd" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswd" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /i ".\..\..\include" /i ".\..\..\lib\vc_dll\mswd" /i "." /d "WXUSINGDLL" /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25d_core.lib wxbase25d.lib wxtiffd25.lib wxjpegd25.lib wxpngd25.lib wxzlibd25.lib wxregexd25.lib wxexpatd25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswddll\png.exe" /debug /libpath:".\..\..\lib\vc_dll" /subsystem:windows
# ADD LINK32 wxmsw25d_core.lib wxbase25d.lib wxtiffd25.lib wxjpegd25.lib wxpngd25.lib wxzlibd25.lib wxregexd25.lib wxexpatd25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswddll\png.exe" /debug /libpath:".\..\..\lib\vc_dll" /subsystem:windows

!ELSEIF  "$(CFG)" == "png - Win32 Universal Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_mswunivu"
# PROP BASE Intermediate_Dir "vc_mswunivu\png"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vc_mswunivu"
# PROP Intermediate_Dir "vc_mswunivu\png"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /Fdvc_mswunivu\png.pdb /O1 /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswunivu" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MD /Fdvc_mswunivu\png.pdb /O1 /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswunivu" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswunivu" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x405 /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswunivu" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmswuniv25u_core.lib wxbase25u.lib wxtiff25.lib wxjpeg25.lib wxpng25.lib wxzlib25.lib wxregexu25.lib wxexpat25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivu\png.exe" /libpath:".\..\..\lib\vc_lib" /subsystem:windows
# ADD LINK32 wxmswuniv25u_core.lib wxbase25u.lib wxtiff25.lib wxjpeg25.lib wxpng25.lib wxzlib25.lib wxregexu25.lib wxexpat25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivu\png.exe" /libpath:".\..\..\lib\vc_lib" /subsystem:windows

!ELSEIF  "$(CFG)" == "png - Win32 Universal Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswunivud"
# PROP BASE Intermediate_Dir "vc_mswunivud\png"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswunivud"
# PROP Intermediate_Dir "vc_mswunivud\png"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswunivud\png.pdb /Od /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswunivud" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswunivud\png.pdb /Od /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswunivud" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "__WXDEBUG__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswunivud" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "__WXDEBUG__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswunivud" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmswuniv25ud_core.lib wxbase25ud.lib wxtiffd25.lib wxjpegd25.lib wxpngd25.lib wxzlibd25.lib wxregexud25.lib wxexpatd25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivud\png.exe" /debug /libpath:".\..\..\lib\vc_lib" /subsystem:windows
# ADD LINK32 wxmswuniv25ud_core.lib wxbase25ud.lib wxtiffd25.lib wxjpegd25.lib wxpngd25.lib wxzlibd25.lib wxregexud25.lib wxexpatd25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivud\png.exe" /debug /libpath:".\..\..\lib\vc_lib" /subsystem:windows

!ELSEIF  "$(CFG)" == "png - Win32 Universal Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_mswuniv"
# PROP BASE Intermediate_Dir "vc_mswuniv\png"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vc_mswuniv"
# PROP Intermediate_Dir "vc_mswuniv\png"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /Fdvc_mswuniv\png.pdb /O1 /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswuniv" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MD /Fdvc_mswuniv\png.pdb /O1 /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswuniv" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "__WXMSW__" /d "__WXUNIVERSAL__" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswuniv" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x405 /d "__WXMSW__" /d "__WXUNIVERSAL__" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswuniv" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmswuniv25_core.lib wxbase25.lib wxtiff25.lib wxjpeg25.lib wxpng25.lib wxzlib25.lib wxregex25.lib wxexpat25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswuniv\png.exe" /libpath:".\..\..\lib\vc_lib" /subsystem:windows
# ADD LINK32 wxmswuniv25_core.lib wxbase25.lib wxtiff25.lib wxjpeg25.lib wxpng25.lib wxzlib25.lib wxregex25.lib wxexpat25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswuniv\png.exe" /libpath:".\..\..\lib\vc_lib" /subsystem:windows

!ELSEIF  "$(CFG)" == "png - Win32 Universal Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswunivd"
# PROP BASE Intermediate_Dir "vc_mswunivd\png"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswunivd"
# PROP Intermediate_Dir "vc_mswunivd\png"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswunivd\png.pdb /Od /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswunivd" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswunivd\png.pdb /Od /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswunivd" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXUNIVERSAL__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "__WXDEBUG__" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswunivd" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXUNIVERSAL__" /d "__WXDEBUG__" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswunivd" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmswuniv25d_core.lib wxbase25d.lib wxtiffd25.lib wxjpegd25.lib wxpngd25.lib wxzlibd25.lib wxregexd25.lib wxexpatd25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivd\png.exe" /debug /libpath:".\..\..\lib\vc_lib" /subsystem:windows
# ADD LINK32 wxmswuniv25d_core.lib wxbase25d.lib wxtiffd25.lib wxjpegd25.lib wxpngd25.lib wxzlibd25.lib wxregexd25.lib wxexpatd25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswunivd\png.exe" /debug /libpath:".\..\..\lib\vc_lib" /subsystem:windows

!ELSEIF  "$(CFG)" == "png - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_mswu"
# PROP BASE Intermediate_Dir "vc_mswu\png"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vc_mswu"
# PROP Intermediate_Dir "vc_mswu\png"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /Fdvc_mswu\png.pdb /O1 /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswu" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MD /Fdvc_mswu\png.pdb /O1 /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswu" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "__WXMSW__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswu" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x405 /d "__WXMSW__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswu" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25u_core.lib wxbase25u.lib wxtiff25.lib wxjpeg25.lib wxpng25.lib wxzlib25.lib wxregexu25.lib wxexpat25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswu\png.exe" /libpath:".\..\..\lib\vc_lib" /subsystem:windows
# ADD LINK32 wxmsw25u_core.lib wxbase25u.lib wxtiff25.lib wxjpeg25.lib wxpng25.lib wxzlib25.lib wxregexu25.lib wxexpat25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswu\png.exe" /libpath:".\..\..\lib\vc_lib" /subsystem:windows

!ELSEIF  "$(CFG)" == "png - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswud"
# PROP BASE Intermediate_Dir "vc_mswud\png"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswud"
# PROP Intermediate_Dir "vc_mswud\png"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswud\png.pdb /Od /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswud" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswud\png.pdb /Od /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswud" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswud" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /d "_UNICODE" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswud" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25ud_core.lib wxbase25ud.lib wxtiffd25.lib wxjpegd25.lib wxpngd25.lib wxzlibd25.lib wxregexud25.lib wxexpatd25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswud\png.exe" /debug /libpath:".\..\..\lib\vc_lib" /subsystem:windows
# ADD LINK32 wxmsw25ud_core.lib wxbase25ud.lib wxtiffd25.lib wxjpegd25.lib wxpngd25.lib wxzlibd25.lib wxregexud25.lib wxexpatd25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswud\png.exe" /debug /libpath:".\..\..\lib\vc_lib" /subsystem:windows

!ELSEIF  "$(CFG)" == "png - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_msw"
# PROP BASE Intermediate_Dir "vc_msw\png"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vc_msw"
# PROP Intermediate_Dir "vc_msw\png"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /Fdvc_msw\png.pdb /O1 /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_lib\msw" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MD /Fdvc_msw\png.pdb /O1 /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_lib\msw" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "__WXMSW__" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\msw" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x405 /d "__WXMSW__" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\msw" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25_core.lib wxbase25.lib wxtiff25.lib wxjpeg25.lib wxpng25.lib wxzlib25.lib wxregex25.lib wxexpat25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_msw\png.exe" /libpath:".\..\..\lib\vc_lib" /subsystem:windows
# ADD LINK32 wxmsw25_core.lib wxbase25.lib wxtiff25.lib wxjpeg25.lib wxpng25.lib wxzlib25.lib wxregex25.lib wxexpat25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_msw\png.exe" /libpath:".\..\..\lib\vc_lib" /subsystem:windows

!ELSEIF  "$(CFG)" == "png - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc_mswd"
# PROP BASE Intermediate_Dir "vc_mswd\png"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc_mswd"
# PROP Intermediate_Dir "vc_mswd\png"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswd\png.pdb /Od /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswd" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /c
# ADD CPP /nologo /FD /MDd /Zi /Gm /GZ /Fdvc_mswd\png.pdb /Od /GR /GX /I ".\..\..\include" /I ".\..\..\lib\vc_lib\mswd" /W4 /I "." /I ".\..\..\samples" /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /c
# ADD BASE MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "_DEBUG" /D "__WXMSW__" /D "__WXDEBUG__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswd" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
# ADD RSC /l 0x405 /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /i ".\..\..\include" /i ".\..\..\lib\vc_lib\mswd" /i "." /d "_WINDOWS" /i ".\..\..\samples" /d NOPCH
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw25d_core.lib wxbase25d.lib wxtiffd25.lib wxjpegd25.lib wxpngd25.lib wxzlibd25.lib wxregexd25.lib wxexpatd25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswd\png.exe" /debug /libpath:".\..\..\lib\vc_lib" /subsystem:windows
# ADD LINK32 wxmsw25d_core.lib wxbase25d.lib wxtiffd25.lib wxjpegd25.lib wxpngd25.lib wxzlibd25.lib wxregexd25.lib wxexpatd25.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib odbc32.lib /nologo /machine:i386 /out:"vc_mswd\png.exe" /debug /libpath:".\..\..\lib\vc_lib" /subsystem:windows

!ENDIF

# Begin Target

# Name "png - Win32 DLL Universal Unicode Release"
# Name "png - Win32 DLL Universal Unicode Debug"
# Name "png - Win32 DLL Universal Release"
# Name "png - Win32 DLL Universal Debug"
# Name "png - Win32 DLL Unicode Release"
# Name "png - Win32 DLL Unicode Debug"
# Name "png - Win32 DLL Release"
# Name "png - Win32 DLL Debug"
# Name "png - Win32 Universal Unicode Release"
# Name "png - Win32 Universal Unicode Debug"
# Name "png - Win32 Universal Release"
# Name "png - Win32 Universal Debug"
# Name "png - Win32 Unicode Release"
# Name "png - Win32 Unicode Debug"
# Name "png - Win32 Release"
# Name "png - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\pngdemo.cpp
# End Source File
# Begin Source File

SOURCE=.\pngdemo.rc
# End Source File
# End Group
# End Target
# End Project

