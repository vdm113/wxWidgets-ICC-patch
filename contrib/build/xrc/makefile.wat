# =========================================================================
#     This makefile was generated by
#     Bakefile 0.1.4 (http://bakefile.sourceforge.net)
#     Do not modify, all changes will be overwritten!
# =========================================================================

!include ../../../build/msw/config.wat

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
!  loaddll wlink    wlink
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
PORTNAME = msw
!endif
VENDORTAG =
!ifeq OFFICIAL_BUILD 0
VENDORTAG = _$(VENDOR)
!endif
!ifeq OFFICIAL_BUILD 1
VENDORTAG = 
!endif
WXDEBUGFLAG =
!ifeq BUILD debug
!ifeq DEBUG_FLAG default
WXDEBUGFLAG = d
!endif
!endif
!ifeq DEBUG_FLAG 1
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
__xrcdll___depname =
!ifeq SHARED 1
__xrcdll___depname = &
	$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)252$(WXUNICODEFLAG)$(WXDEBUGFLAG)_xrc_wat$(VENDORTAG).dll
!endif
__DEBUGINFO_2 =
!ifeq BUILD debug
!ifeq DEBUG_INFO default
__DEBUGINFO_2 = debug all
!endif
!endif
!ifeq BUILD release
!ifeq DEBUG_INFO default
__DEBUGINFO_2 = 
!endif
!endif
!ifeq DEBUG_INFO 0
__DEBUGINFO_2 = 
!endif
!ifeq DEBUG_INFO 1
__DEBUGINFO_2 = debug all
!endif
__WXLIB_MONO_p =
!ifeq MONOLITHIC 1
__WXLIB_MONO_p = &
	wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib
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
__WXLIB_HTML_p =
!ifeq MONOLITHIC 0
__WXLIB_HTML_p = &
	wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_html.lib
!endif
__WXLIB_ADV_p =
!ifeq MONOLITHIC 0
__WXLIB_ADV_p = &
	wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_adv.lib
!endif
__WXLIB_CORE_p =
!ifeq MONOLITHIC 0
__WXLIB_CORE_p = &
	wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_core.lib
!endif
__WXLIB_XML_p =
!ifeq MONOLITHIC 0
__WXLIB_XML_p = wxbase25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_xml.lib
!endif
__WXLIB_BASE_p =
!ifeq MONOLITHIC 0
__WXLIB_BASE_p = wxbase25$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib
!endif
__xrclib___depname =
!ifeq SHARED 0
__xrclib___depname = &
	$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_xrc.lib
!endif
__DEBUGINFO_1 =
!ifeq BUILD debug
!ifeq DEBUG_INFO default
__DEBUGINFO_1 = -d2
!endif
!endif
!ifeq BUILD release
!ifeq DEBUG_INFO default
__DEBUGINFO_1 = -d0
!endif
!endif
!ifeq DEBUG_INFO 0
__DEBUGINFO_1 = -d0
!endif
!ifeq DEBUG_INFO 1
__DEBUGINFO_1 = -d2
!endif
__OPTIMIZEFLAG =
!ifeq BUILD debug
__OPTIMIZEFLAG = -od
!endif
!ifeq BUILD release
__OPTIMIZEFLAG = -ot -ox
!endif
__RUNTIME_LIBS =
!ifeq RUNTIME_LIBS dynamic
__RUNTIME_LIBS = -br
!endif
!ifeq RUNTIME_LIBS static
__RUNTIME_LIBS = 
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
!ifeq BUILD debug
!ifeq DEBUG_FLAG default
__DEBUG_DEFINE_p = -d__WXDEBUG__
!endif
!endif
!ifeq DEBUG_FLAG 1
__DEBUG_DEFINE_p = -d__WXDEBUG__
!endif
__UNICODE_DEFINE_p =
!ifeq UNICODE 1
__UNICODE_DEFINE_p = -d_UNICODE
!endif
LIBDIRNAME =
!ifeq SHARED 0
LIBDIRNAME = ..\..\src\xrc\..\..\..\lib\wat_lib$(CFG)
!endif
!ifeq SHARED 1
LIBDIRNAME = ..\..\src\xrc\..\..\..\lib\wat_dll$(CFG)
!endif

### Variables: ###

OBJS = &
	wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
SETUPHDIR = &
	$(LIBDIRNAME)\$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)
XRCDLL_CXXFLAGS = -bd $(__DEBUGINFO_1) $(__OPTIMIZEFLAG) -bm $(__RUNTIME_LIBS) &
	-d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__UNICODE_DEFINE_p) &
	-i=..\..\src\xrc\..\..\..\include -i=$(SETUPHDIR) &
	-i=..\..\src\xrc\..\..\include -dWXUSINGDLL -dWXMAKINGDLL_XRC &
	/fh=$(OBJS)\wxprec_xrcdll.pch $(__EXCEPTIONSFLAG) $(CPPFLAGS) $(CXXFLAGS)
XRCDLL_OBJECTS =  &
	$(OBJS)\xrcdll_dummy.obj &
	$(OBJS)\xrcdll_xmlres.obj &
	$(OBJS)\xrcdll_xh_bmp.obj &
	$(OBJS)\xrcdll_xh_bmpbt.obj &
	$(OBJS)\xrcdll_xh_bttn.obj &
	$(OBJS)\xrcdll_xh_cald.obj &
	$(OBJS)\xrcdll_xh_chckb.obj &
	$(OBJS)\xrcdll_xh_tglbtn.obj &
	$(OBJS)\xrcdll_xh_chckl.obj &
	$(OBJS)\xrcdll_xh_choic.obj &
	$(OBJS)\xrcdll_xh_combo.obj &
	$(OBJS)\xrcdll_xh_dlg.obj &
	$(OBJS)\xrcdll_xh_frame.obj &
	$(OBJS)\xrcdll_xh_gauge.obj &
	$(OBJS)\xrcdll_xh_gdctl.obj &
	$(OBJS)\xrcdll_xh_html.obj &
	$(OBJS)\xrcdll_xh_listb.obj &
	$(OBJS)\xrcdll_xh_listc.obj &
	$(OBJS)\xrcdll_xh_menu.obj &
	$(OBJS)\xrcdll_xh_notbk.obj &
	$(OBJS)\xrcdll_xh_panel.obj &
	$(OBJS)\xrcdll_xh_radbt.obj &
	$(OBJS)\xrcdll_xh_radbx.obj &
	$(OBJS)\xrcdll_xh_scrol.obj &
	$(OBJS)\xrcdll_xh_scwin.obj &
	$(OBJS)\xrcdll_xh_sizer.obj &
	$(OBJS)\xrcdll_xh_slidr.obj &
	$(OBJS)\xrcdll_xh_spin.obj &
	$(OBJS)\xrcdll_xh_split.obj &
	$(OBJS)\xrcdll_xh_statbar.obj &
	$(OBJS)\xrcdll_xh_stbmp.obj &
	$(OBJS)\xrcdll_xh_stbox.obj &
	$(OBJS)\xrcdll_xh_stlin.obj &
	$(OBJS)\xrcdll_xh_sttxt.obj &
	$(OBJS)\xrcdll_xh_text.obj &
	$(OBJS)\xrcdll_xh_toolb.obj &
	$(OBJS)\xrcdll_xh_tree.obj &
	$(OBJS)\xrcdll_xh_unkwn.obj &
	$(OBJS)\xrcdll_xh_wizrd.obj &
	$(OBJS)\xrcdll_xmlrsall.obj
XRCLIB_CXXFLAGS = $(__DEBUGINFO_1) $(__OPTIMIZEFLAG) -bm $(__RUNTIME_LIBS) &
	-d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__UNICODE_DEFINE_p) &
	-i=..\..\src\xrc\..\..\..\include -i=$(SETUPHDIR) &
	-i=..\..\src\xrc\..\..\include -i=..\..\src\xrc\expat\xmlparse &
	-i=..\..\src\xrc\expat\xmltok /fh=$(OBJS)\wxprec_xrclib.pch &
	$(__EXCEPTIONSFLAG) $(CPPFLAGS) $(CXXFLAGS)
XRCLIB_OBJECTS =  &
	$(OBJS)\xrclib_dummy.obj &
	$(OBJS)\xrclib_xmlres.obj &
	$(OBJS)\xrclib_xh_bmp.obj &
	$(OBJS)\xrclib_xh_bmpbt.obj &
	$(OBJS)\xrclib_xh_bttn.obj &
	$(OBJS)\xrclib_xh_cald.obj &
	$(OBJS)\xrclib_xh_chckb.obj &
	$(OBJS)\xrclib_xh_tglbtn.obj &
	$(OBJS)\xrclib_xh_chckl.obj &
	$(OBJS)\xrclib_xh_choic.obj &
	$(OBJS)\xrclib_xh_combo.obj &
	$(OBJS)\xrclib_xh_dlg.obj &
	$(OBJS)\xrclib_xh_frame.obj &
	$(OBJS)\xrclib_xh_gauge.obj &
	$(OBJS)\xrclib_xh_gdctl.obj &
	$(OBJS)\xrclib_xh_html.obj &
	$(OBJS)\xrclib_xh_listb.obj &
	$(OBJS)\xrclib_xh_listc.obj &
	$(OBJS)\xrclib_xh_menu.obj &
	$(OBJS)\xrclib_xh_notbk.obj &
	$(OBJS)\xrclib_xh_panel.obj &
	$(OBJS)\xrclib_xh_radbt.obj &
	$(OBJS)\xrclib_xh_radbx.obj &
	$(OBJS)\xrclib_xh_scrol.obj &
	$(OBJS)\xrclib_xh_scwin.obj &
	$(OBJS)\xrclib_xh_sizer.obj &
	$(OBJS)\xrclib_xh_slidr.obj &
	$(OBJS)\xrclib_xh_spin.obj &
	$(OBJS)\xrclib_xh_split.obj &
	$(OBJS)\xrclib_xh_statbar.obj &
	$(OBJS)\xrclib_xh_stbmp.obj &
	$(OBJS)\xrclib_xh_stbox.obj &
	$(OBJS)\xrclib_xh_stlin.obj &
	$(OBJS)\xrclib_xh_sttxt.obj &
	$(OBJS)\xrclib_xh_text.obj &
	$(OBJS)\xrclib_xh_toolb.obj &
	$(OBJS)\xrclib_xh_tree.obj &
	$(OBJS)\xrclib_xh_unkwn.obj &
	$(OBJS)\xrclib_xh_wizrd.obj &
	$(OBJS)\xrclib_xmlrsall.obj


all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)

### Targets: ###

all : .SYMBOLIC $(__xrcdll___depname) $(__xrclib___depname)

clean : .SYMBOLIC 
	-if exist $(OBJS)\*.obj del $(OBJS)\*.obj
	-if exist $(OBJS)\*.res del $(OBJS)\*.res
	-if exist $(OBJS)\*.lbc del $(OBJS)\*.lbc
	-if exist $(OBJS)\*.ilk del $(OBJS)\*.ilk
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)252$(WXUNICODEFLAG)$(WXDEBUGFLAG)_xrc_wat$(VENDORTAG).dll del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)252$(WXUNICODEFLAG)$(WXDEBUGFLAG)_xrc_wat$(VENDORTAG).dll
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_xrc.lib del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_xrc.lib
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_xrc.lib del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_xrc.lib

!ifeq SHARED 1
$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)252$(WXUNICODEFLAG)$(WXDEBUGFLAG)_xrc_wat$(VENDORTAG).dll :  $(XRCDLL_OBJECTS)
	@%create $(OBJS)\xrcdll.lbc
	@%append $(OBJS)\xrcdll.lbc option quiet
	@%append $(OBJS)\xrcdll.lbc name $^@
	@%append $(OBJS)\xrcdll.lbc option caseexact
	@%append $(OBJS)\xrcdll.lbc $(LDFLAGS) $(__DEBUGINFO_2)  libpath $(LIBDIRNAME)
	@for %i in ($(XRCDLL_OBJECTS)) do @%append $(OBJS)\xrcdll.lbc file %i
	@for %i in ( $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p) wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib   kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib  $(__WXLIB_HTML_p)  $(__WXLIB_ADV_p)  $(__WXLIB_CORE_p)  $(__WXLIB_XML_p)  $(__WXLIB_BASE_p) ) do @%append $(OBJS)\xrcdll.lbc library %i
	@%append $(OBJS)\xrcdll.lbc
	@%append $(OBJS)\xrcdll.lbc system nt_dll
	wlink @$(OBJS)\xrcdll.lbc
	wlib -q -n -b $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_xrc.lib +$^@
!endif

!ifeq SHARED 0
$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_xrc.lib :  $(XRCLIB_OBJECTS)
	@%create $(OBJS)\xrclib.lbc
	@for %i in ($(XRCLIB_OBJECTS)) do @%append $(OBJS)\xrclib.lbc +%i
	wlib -q -p4096 -n -b $^@ @$(OBJS)\xrclib.lbc
!endif

$(OBJS)\xrcdll_dummy.obj :  .AUTODEPEND ../../src/xrc\..\..\..\src\msw\dummy.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xmlres.obj :  .AUTODEPEND ../../src/xrc\xmlres.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_bmp.obj :  .AUTODEPEND ../../src/xrc\xh_bmp.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_bmpbt.obj :  .AUTODEPEND ../../src/xrc\xh_bmpbt.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_bttn.obj :  .AUTODEPEND ../../src/xrc\xh_bttn.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_cald.obj :  .AUTODEPEND ../../src/xrc\xh_cald.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_chckb.obj :  .AUTODEPEND ../../src/xrc\xh_chckb.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_tglbtn.obj :  .AUTODEPEND ../../src/xrc\xh_tglbtn.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_chckl.obj :  .AUTODEPEND ../../src/xrc\xh_chckl.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_choic.obj :  .AUTODEPEND ../../src/xrc\xh_choic.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_combo.obj :  .AUTODEPEND ../../src/xrc\xh_combo.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_dlg.obj :  .AUTODEPEND ../../src/xrc\xh_dlg.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_frame.obj :  .AUTODEPEND ../../src/xrc\xh_frame.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_gauge.obj :  .AUTODEPEND ../../src/xrc\xh_gauge.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_gdctl.obj :  .AUTODEPEND ../../src/xrc\xh_gdctl.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_html.obj :  .AUTODEPEND ../../src/xrc\xh_html.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_listb.obj :  .AUTODEPEND ../../src/xrc\xh_listb.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_listc.obj :  .AUTODEPEND ../../src/xrc\xh_listc.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_menu.obj :  .AUTODEPEND ../../src/xrc\xh_menu.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_notbk.obj :  .AUTODEPEND ../../src/xrc\xh_notbk.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_panel.obj :  .AUTODEPEND ../../src/xrc\xh_panel.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_radbt.obj :  .AUTODEPEND ../../src/xrc\xh_radbt.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_radbx.obj :  .AUTODEPEND ../../src/xrc\xh_radbx.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_scrol.obj :  .AUTODEPEND ../../src/xrc\xh_scrol.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_scwin.obj :  .AUTODEPEND ../../src/xrc\xh_scwin.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_sizer.obj :  .AUTODEPEND ../../src/xrc\xh_sizer.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_slidr.obj :  .AUTODEPEND ../../src/xrc\xh_slidr.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_spin.obj :  .AUTODEPEND ../../src/xrc\xh_spin.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_split.obj :  .AUTODEPEND ../../src/xrc\xh_split.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_statbar.obj :  .AUTODEPEND ../../src/xrc\xh_statbar.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_stbmp.obj :  .AUTODEPEND ../../src/xrc\xh_stbmp.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_stbox.obj :  .AUTODEPEND ../../src/xrc\xh_stbox.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_stlin.obj :  .AUTODEPEND ../../src/xrc\xh_stlin.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_sttxt.obj :  .AUTODEPEND ../../src/xrc\xh_sttxt.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_text.obj :  .AUTODEPEND ../../src/xrc\xh_text.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_toolb.obj :  .AUTODEPEND ../../src/xrc\xh_toolb.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_tree.obj :  .AUTODEPEND ../../src/xrc\xh_tree.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_unkwn.obj :  .AUTODEPEND ../../src/xrc\xh_unkwn.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xh_wizrd.obj :  .AUTODEPEND ../../src/xrc\xh_wizrd.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrcdll_xmlrsall.obj :  .AUTODEPEND ../../src/xrc\xmlrsall.cpp
	$(CXX) -zq -fo=$^@ $(XRCDLL_CXXFLAGS) $<

$(OBJS)\xrclib_dummy.obj :  .AUTODEPEND ../../src/xrc\..\..\..\src\msw\dummy.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xmlres.obj :  .AUTODEPEND ../../src/xrc\xmlres.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_bmp.obj :  .AUTODEPEND ../../src/xrc\xh_bmp.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_bmpbt.obj :  .AUTODEPEND ../../src/xrc\xh_bmpbt.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_bttn.obj :  .AUTODEPEND ../../src/xrc\xh_bttn.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_cald.obj :  .AUTODEPEND ../../src/xrc\xh_cald.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_chckb.obj :  .AUTODEPEND ../../src/xrc\xh_chckb.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_tglbtn.obj :  .AUTODEPEND ../../src/xrc\xh_tglbtn.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_chckl.obj :  .AUTODEPEND ../../src/xrc\xh_chckl.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_choic.obj :  .AUTODEPEND ../../src/xrc\xh_choic.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_combo.obj :  .AUTODEPEND ../../src/xrc\xh_combo.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_dlg.obj :  .AUTODEPEND ../../src/xrc\xh_dlg.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_frame.obj :  .AUTODEPEND ../../src/xrc\xh_frame.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_gauge.obj :  .AUTODEPEND ../../src/xrc\xh_gauge.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_gdctl.obj :  .AUTODEPEND ../../src/xrc\xh_gdctl.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_html.obj :  .AUTODEPEND ../../src/xrc\xh_html.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_listb.obj :  .AUTODEPEND ../../src/xrc\xh_listb.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_listc.obj :  .AUTODEPEND ../../src/xrc\xh_listc.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_menu.obj :  .AUTODEPEND ../../src/xrc\xh_menu.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_notbk.obj :  .AUTODEPEND ../../src/xrc\xh_notbk.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_panel.obj :  .AUTODEPEND ../../src/xrc\xh_panel.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_radbt.obj :  .AUTODEPEND ../../src/xrc\xh_radbt.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_radbx.obj :  .AUTODEPEND ../../src/xrc\xh_radbx.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_scrol.obj :  .AUTODEPEND ../../src/xrc\xh_scrol.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_scwin.obj :  .AUTODEPEND ../../src/xrc\xh_scwin.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_sizer.obj :  .AUTODEPEND ../../src/xrc\xh_sizer.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_slidr.obj :  .AUTODEPEND ../../src/xrc\xh_slidr.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_spin.obj :  .AUTODEPEND ../../src/xrc\xh_spin.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_split.obj :  .AUTODEPEND ../../src/xrc\xh_split.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_statbar.obj :  .AUTODEPEND ../../src/xrc\xh_statbar.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_stbmp.obj :  .AUTODEPEND ../../src/xrc\xh_stbmp.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_stbox.obj :  .AUTODEPEND ../../src/xrc\xh_stbox.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_stlin.obj :  .AUTODEPEND ../../src/xrc\xh_stlin.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_sttxt.obj :  .AUTODEPEND ../../src/xrc\xh_sttxt.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_text.obj :  .AUTODEPEND ../../src/xrc\xh_text.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_toolb.obj :  .AUTODEPEND ../../src/xrc\xh_toolb.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_tree.obj :  .AUTODEPEND ../../src/xrc\xh_tree.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_unkwn.obj :  .AUTODEPEND ../../src/xrc\xh_unkwn.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xh_wizrd.obj :  .AUTODEPEND ../../src/xrc\xh_wizrd.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<

$(OBJS)\xrclib_xmlrsall.obj :  .AUTODEPEND ../../src/xrc\xmlrsall.cpp
	$(CXX) -zq -fo=$^@ $(XRCLIB_CXXFLAGS) $<
