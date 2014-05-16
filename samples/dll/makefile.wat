# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.9 (http://www.bakefile.org)
#     Do not modify, all changes will be overwritten!
# =========================================================================

!include ../../build/msw/config.wat

# -------------------------------------------------------------------------
# Do not modify the rest of this file!
# -------------------------------------------------------------------------

# Speed up compilation a bit:
!ifdef __LOADDLL__
!  loaddll wcc      wccd
!  loaddll wccaxp   wccdaxp
!  loaddll wcc386   wccd386
!  loaddll wpp      wppdi86
!  loaddll wppaxp   wppdaxp
!  loaddll wpp386   wppd386
! if $(__VERSION__) >= 1280
!  loaddll wlink    wlinkd
! else
!  loaddll wlink    wlink
! endif
!  loaddll wlib     wlibd
!endif

# We need these variables in some bakefile-made rules:
WATCOM_CWD = $+ $(%cdrive):$(%cwd) $-

### Conditionally set variables: ###

PORTNAME =
!ifeq USE_GUI 0
PORTNAME = base
!endif
!ifeq USE_GUI 1
PORTNAME = msw$(TOOLKIT_VERSION)
!endif
COMPILER_VERSION =
!ifeq OFFICIAL_BUILD 1
COMPILER_VERSION = ERROR-COMPILER-VERSION-MUST-BE-SET-FOR-OFFICIAL-BUILD
!endif
WXDEBUGFLAG =
!ifeq BUILD debug
WXDEBUGFLAG = d
!endif
WXUNICODEFLAG =
!ifeq UNICODE 1
WXUNICODEFLAG = u
!endif
WXUNIVNAME =
!ifeq WXUNIV 1
WXUNIVNAME = univ
!endif
WXDLLFLAG =
!ifeq SHARED 1
WXDLLFLAG = dll
!endif
LIBTYPE_SUFFIX =
!ifeq SHARED 0
LIBTYPE_SUFFIX = lib
!endif
!ifeq SHARED 1
LIBTYPE_SUFFIX = dll
!endif
EXTRALIBS_FOR_BASE =
!ifeq MONOLITHIC 0
EXTRALIBS_FOR_BASE = 
!endif
!ifeq MONOLITHIC 1
EXTRALIBS_FOR_BASE =   
!endif
__wx_exe___depname =
!ifeq SHARED 0
__wx_exe___depname = $(OBJS)\wx_exe.exe
!endif
__WXLIB_CORE_p =
!ifeq MONOLITHIC 0
__WXLIB_CORE_p = &
	wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_core.lib
!endif
__WXLIB_BASE_p =
!ifeq MONOLITHIC 0
__WXLIB_BASE_p = &
	wxbase$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR).lib
!endif
__WXLIB_MONO_p =
!ifeq MONOLITHIC 1
__WXLIB_MONO_p = &
	wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR).lib
!endif
__LIB_SCINTILLA_IF_MONO_p =
!ifeq MONOLITHIC 1
!ifeq USE_STC 1
__LIB_SCINTILLA_IF_MONO_p = wxscintilla$(WXDEBUGFLAG).lib
!endif
!endif
__LIB_TIFF_p =
!ifeq USE_GUI 1
__LIB_TIFF_p = wxtiff$(WXDEBUGFLAG).lib
!endif
__LIB_JPEG_p =
!ifeq USE_GUI 1
__LIB_JPEG_p = wxjpeg$(WXDEBUGFLAG).lib
!endif
__LIB_PNG_p =
!ifeq USE_GUI 1
__LIB_PNG_p = wxpng$(WXDEBUGFLAG).lib
!endif
__CAIRO_LIB_p =
!ifeq USE_CAIRO 1
__CAIRO_LIB_p = cairo.lib
!endif
____CAIRO_LIBDIR_FILENAMES =
!ifeq USE_CAIRO 1
____CAIRO_LIBDIR_FILENAMES = libpath $(CAIRO_ROOT)\lib
!endif
__DEBUGINFO_0 =
!ifeq BUILD debug
!ifeq DEBUG_INFO default
__DEBUGINFO_0 = -d2
!endif
!endif
!ifeq BUILD release
!ifeq DEBUG_INFO default
__DEBUGINFO_0 = -d0
!endif
!endif
!ifeq DEBUG_INFO 0
__DEBUGINFO_0 = -d0
!endif
!ifeq DEBUG_INFO 1
__DEBUGINFO_0 = -d2
!endif
__DEBUGINFO =
!ifeq BUILD debug
!ifeq DEBUG_INFO default
__DEBUGINFO = debug all
!endif
!endif
!ifeq BUILD release
!ifeq DEBUG_INFO default
__DEBUGINFO = 
!endif
!endif
!ifeq DEBUG_INFO 0
__DEBUGINFO = 
!endif
!ifeq DEBUG_INFO 1
__DEBUGINFO = debug all
!endif
__OPTIMIZEFLAG =
!ifeq BUILD debug
__OPTIMIZEFLAG = -od
!endif
!ifeq BUILD release
__OPTIMIZEFLAG = -ot -ox
!endif
__THREADSFLAG =
!ifeq USE_THREADS 0
__THREADSFLAG = 
!endif
!ifeq USE_THREADS 1
__THREADSFLAG = -bm
!endif
__RUNTIME_LIBS =
!ifeq RUNTIME_LIBS dynamic
__RUNTIME_LIBS = -br
!endif
!ifeq RUNTIME_LIBS static
__RUNTIME_LIBS = 
!endif
__RTTIFLAG =
!ifeq USE_RTTI 0
__RTTIFLAG = 
!endif
!ifeq USE_RTTI 1
__RTTIFLAG = -xr
!endif
__EXCEPTIONSFLAG =
!ifeq USE_EXCEPTIONS 0
__EXCEPTIONSFLAG = 
!endif
!ifeq USE_EXCEPTIONS 1
__EXCEPTIONSFLAG = -xs
!endif
__WXUNIV_DEFINE_p =
!ifeq WXUNIV 1
__WXUNIV_DEFINE_p = -d__WXUNIVERSAL__
!endif
__DEBUG_DEFINE_p =
!ifeq DEBUG_FLAG 0
__DEBUG_DEFINE_p = -dwxDEBUG_LEVEL=0
!endif
__NDEBUG_DEFINE_p =
!ifeq BUILD release
__NDEBUG_DEFINE_p = -dNDEBUG
!endif
__EXCEPTIONS_DEFINE_p =
!ifeq USE_EXCEPTIONS 0
__EXCEPTIONS_DEFINE_p = -dwxNO_EXCEPTIONS
!endif
__RTTI_DEFINE_p =
!ifeq USE_RTTI 0
__RTTI_DEFINE_p = -dwxNO_RTTI
!endif
__THREAD_DEFINE_p =
!ifeq USE_THREADS 0
__THREAD_DEFINE_p = -dwxNO_THREADS
!endif
__UNICODE_DEFINE_p =
!ifeq UNICODE 0
__UNICODE_DEFINE_p = -dwxUSE_UNICODE=0
!endif
!ifeq UNICODE 1
__UNICODE_DEFINE_p = -d_UNICODE
!endif
____CAIRO_INCLUDEDIR_FILENAMES =
!ifeq USE_CAIRO 1
____CAIRO_INCLUDEDIR_FILENAMES = -i=$(CAIRO_ROOT)\include\cairo
!endif
__DLLFLAG_p =
!ifeq SHARED 1
__DLLFLAG_p = -dWXUSINGDLL
!endif

### Variables: ###

WX_RELEASE_NODOT = 31
COMPILER_PREFIX = wat
OBJS = &
	$(COMPILER_PREFIX)$(COMPILER_VERSION)_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
LIBDIRNAME = &
	.\..\..\lib\$(COMPILER_PREFIX)$(COMPILER_VERSION)_$(LIBTYPE_SUFFIX)$(CFG)
SETUPHDIR = &
	$(LIBDIRNAME)\$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)
MY_DLL_CXXFLAGS = -bd $(__DEBUGINFO_0) $(__OPTIMIZEFLAG) $(__THREADSFLAG) &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__NDEBUG_DEFINE_p) $(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) &
	$(__THREAD_DEFINE_p) $(__UNICODE_DEFINE_p) -i=$(SETUPHDIR) &
	-i=.\..\..\include $(____CAIRO_INCLUDEDIR_FILENAMES) -wx -wcd=549 -wcd=656 &
	-wcd=657 -wcd=667 $(__DLLFLAG_p) -dMY_DLL_BUILDING -i=. $(__RTTIFLAG) &
	$(__EXCEPTIONSFLAG) $(CPPFLAGS) $(CXXFLAGS)
MY_DLL_OBJECTS =  &
	$(OBJS)\my_dll_my_dll.obj
WX_EXE_CXXFLAGS = $(__DEBUGINFO_0) $(__OPTIMIZEFLAG) $(__THREADSFLAG) &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__NDEBUG_DEFINE_p) $(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) &
	$(__THREAD_DEFINE_p) $(__UNICODE_DEFINE_p) -i=$(SETUPHDIR) &
	-i=.\..\..\include $(____CAIRO_INCLUDEDIR_FILENAMES) -wx -wcd=549 -wcd=656 &
	-wcd=657 -wcd=667 -i=. $(__DLLFLAG_p) -i=.\..\..\samples -dNOPCH $(__RTTIFLAG) &
	$(__EXCEPTIONSFLAG) $(CPPFLAGS) $(CXXFLAGS)
WX_EXE_OBJECTS =  &
	$(OBJS)\wx_exe_wx_exe.obj
SDK_EXE_CXXFLAGS = $(__DEBUGINFO_0) $(__OPTIMIZEFLAG) $(__THREADSFLAG) &
	$(__RUNTIME_LIBS) $(__RTTIFLAG) $(__EXCEPTIONSFLAG) $(CPPFLAGS) $(CXXFLAGS)
SDK_EXE_OBJECTS =  &
	$(OBJS)\sdk_exe_sdk_exe.obj


all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)

### Targets: ###

all : .SYMBOLIC $(OBJS)\my_dll.dll $(__wx_exe___depname) $(OBJS)\sdk_exe.exe

clean : .SYMBOLIC 
	-if exist $(OBJS)\*.obj del $(OBJS)\*.obj
	-if exist $(OBJS)\*.res del $(OBJS)\*.res
	-if exist $(OBJS)\*.lbc del $(OBJS)\*.lbc
	-if exist $(OBJS)\*.ilk del $(OBJS)\*.ilk
	-if exist $(OBJS)\*.pch del $(OBJS)\*.pch
	-if exist $(OBJS)\my_dll.dll del $(OBJS)\my_dll.dll
	-if exist $(OBJS)\my_dll.lib del $(OBJS)\my_dll.lib
	-if exist $(OBJS)\wx_exe.exe del $(OBJS)\wx_exe.exe
	-if exist $(OBJS)\sdk_exe.exe del $(OBJS)\sdk_exe.exe

$(OBJS)\my_dll.dll :  $(MY_DLL_OBJECTS)
	@%create $(OBJS)\my_dll.lbc
	@%append $(OBJS)\my_dll.lbc option quiet
	@%append $(OBJS)\my_dll.lbc name $^@
	@%append $(OBJS)\my_dll.lbc option caseexact
	@%append $(OBJS)\my_dll.lbc  $(__DEBUGINFO)  libpath $(LIBDIRNAME) $(____CAIRO_LIBDIR_FILENAMES) $(LDFLAGS)
	@for %i in ($(MY_DLL_OBJECTS)) do @%append $(OBJS)\my_dll.lbc file %i
	@for %i in ( $(__WXLIB_CORE_p)  $(__WXLIB_BASE_p)  $(__WXLIB_MONO_p) $(__LIB_SCINTILLA_IF_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p)   wxzlib$(WXDEBUGFLAG).lib wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib $(EXTRALIBS_FOR_BASE) $(__CAIRO_LIB_p) kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wininet.lib) do @%append $(OBJS)\my_dll.lbc library %i
	@%append $(OBJS)\my_dll.lbc
	@%append $(OBJS)\my_dll.lbc system nt_dll
	wlink @$(OBJS)\my_dll.lbc
	wlib -q -n -b $(OBJS)\my_dll.lib +$^@

!ifeq SHARED 0
$(OBJS)\wx_exe.exe :  $(WX_EXE_OBJECTS) $(OBJS)\wx_exe_sample.res $(OBJS)\my_dll.dll
	@%create $(OBJS)\wx_exe.lbc
	@%append $(OBJS)\wx_exe.lbc option quiet
	@%append $(OBJS)\wx_exe.lbc name $^@
	@%append $(OBJS)\wx_exe.lbc option caseexact
	@%append $(OBJS)\wx_exe.lbc  $(__DEBUGINFO)  libpath $(LIBDIRNAME) system nt_win ref '_WinMain@16' $(____CAIRO_LIBDIR_FILENAMES) $(LDFLAGS)
	@for %i in ($(WX_EXE_OBJECTS)) do @%append $(OBJS)\wx_exe.lbc file %i
	@for %i in ( $(__WXLIB_CORE_p)  $(__WXLIB_BASE_p)  $(OBJS)\my_dll.lib $(__WXLIB_MONO_p) $(__LIB_SCINTILLA_IF_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p)   wxzlib$(WXDEBUGFLAG).lib wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib $(EXTRALIBS_FOR_BASE) $(__CAIRO_LIB_p) kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib wininet.lib) do @%append $(OBJS)\wx_exe.lbc library %i
	@%append $(OBJS)\wx_exe.lbc option resource=$(OBJS)\wx_exe_sample.res
	@for %i in () do @%append $(OBJS)\wx_exe.lbc option stack=%i
	wlink @$(OBJS)\wx_exe.lbc
!endif

$(OBJS)\sdk_exe.exe :  $(SDK_EXE_OBJECTS) $(OBJS)\my_dll.dll
	@%create $(OBJS)\sdk_exe.lbc
	@%append $(OBJS)\sdk_exe.lbc option quiet
	@%append $(OBJS)\sdk_exe.lbc name $^@
	@%append $(OBJS)\sdk_exe.lbc option caseexact
	@%append $(OBJS)\sdk_exe.lbc  $(__DEBUGINFO)  system nt_win ref '_WinMain@16' $(LDFLAGS)
	@for %i in ($(SDK_EXE_OBJECTS)) do @%append $(OBJS)\sdk_exe.lbc file %i
	@for %i in ( $(OBJS)\my_dll.lib user32.lib) do @%append $(OBJS)\sdk_exe.lbc library %i
	@%append $(OBJS)\sdk_exe.lbc
	@for %i in () do @%append $(OBJS)\sdk_exe.lbc option stack=%i
	wlink @$(OBJS)\sdk_exe.lbc

$(OBJS)\my_dll_my_dll.obj :  .AUTODEPEND .\my_dll.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(MY_DLL_CXXFLAGS) $<

$(OBJS)\wx_exe_sample.res :  .AUTODEPEND .\..\..\samples\sample.rc
	wrc -q -ad -bt=nt -r -fo=$^@    -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__NDEBUG_DEFINE_p) $(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) $(__UNICODE_DEFINE_p) -i=$(SETUPHDIR) -i=.\..\..\include $(____CAIRO_INCLUDEDIR_FILENAMES) -i=. $(__DLLFLAG_p) -i=.\..\..\samples -dNOPCH $<

$(OBJS)\wx_exe_wx_exe.obj :  .AUTODEPEND .\wx_exe.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WX_EXE_CXXFLAGS) $<

$(OBJS)\sdk_exe_sdk_exe.obj :  .AUTODEPEND .\sdk_exe.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(SDK_EXE_CXXFLAGS) $<

