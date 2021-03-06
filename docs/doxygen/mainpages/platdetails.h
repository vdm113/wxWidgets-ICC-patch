/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        platdetails.h
// Purpose:     Platform details page of the Doxygen manual
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


/**

@page page_port Platform Details

@tableofcontents

wxWidgets defines a common API across platforms, but uses the native graphical
user interface (GUI) on each platform, so your program will take on the native
look and feel that users are familiar with. Unfortunately native toolkits and
hardware do not always support the functionality that the wxWidgets API
requires. This chapter collects notes about differences among supported
platforms and ports.



@section page_port_wxgtk wxGTK

wxGTK is a port of wxWidgets using the GTK+ library. It makes use of GTK+'s
native widgets wherever possible and uses wxWidgets' generic controls when
needed. GTK+ itself has been ported to a number of systems, but so far only the
original X11 version is supported. Support for other GTK+ backends is planned,
such as the new DirectFB backend.

All work is being done on GTK+ version 2.0 and above. Support for GTK+ 1.2 will
be deprecated in a later release.

You will need GTK+ 2.6 or higher which is available from:

http://www.gtk.org

The newer version of GTK+ you use, the more native widgets and features will be
utilized. We have gone to great lengths to allow compiling wxWidgets
applications with the latest version of GTK+, with the resulting binary working
on systems even with a much earlier version of GTK+. You will have to ensure
that the application is launched with lazy symbol binding for that.

In order to configure wxWidgets to compile wxGTK you will need use the
@c --with-gtk argument to the @c configure script. This is the default for many
systems.

GTK+ 1.2 can still be used, albeit discouraged. For that you can pass
@c --with-gtk=1 to the @c configure script.

Support for GTK+ 3 is available starting with wxWidgets 2.9.4, use @c configure
option @c --with-gtk=3 to enable it.

For further information, please see the files in @c docs/gtk in the
distribution.



@section page_port_wxosx wxOSX

@subsection page_port_wxosx_cocoa wxOSX/Cocoa

wxOSX/Cocoa is the currently recommended port of wxWidgets for the Macintosh OS
platform. It requires OS X 10.7 or later and, unlike wxOSX/Carbon, fully
supports 64 bit builds.

This is the default port when building wxOSX, but in order to select it
explicitly you can use

@verbatim configure --with-osx_cocoa @endverbatim

For further information, please see the files in @c docs/osx in the
distribution.


@subsection page_port_wxosx_carbon wxOSX/Carbon

wxOSX/Carbon is an older port of wxWidgets for the Macintosh OS platform. Currently
OS X 10.5 or higher are supported. wxOSX/Carbon can be compiled both using
Apple's command line developer tools as well as Apple's Xcode IDE. wxOSX/Carbon
supports Intel and PowerPC architectures and can be used to produce
"universal binaries" in order create application which can run both
architecture. Unfortunately, wxOSX/Carbon does not support any 64-bit
architecture since Apple decided not to port its Carbon API entirely to 64-bit.

@note Carbon has been deprecated by Apple as of OS X 10.5 and will likely be
removed entirely in a future OS version. It's recommended you look into
switching your app over to wxOSX/Cocoa as soon as possible.

To build wxWidgets using wxOSX/Carbon you need to do

@verbatim configure --with-osx_carbon @endverbatim

For further information, please see the files in @c docs/osx in the
distribution.




@section page_port_wxx11 wxX11

wxX11 is a port of wxWidgets using X11 (The X Window System) as the underlying
graphics backend. wxX11 draws its widgets using the wxUniversal widget set
which is now part of wxWidgets. wxX11 is well-suited for a number of special
applications such as those running on systems with few resources (PDAs) or for
applications which need to use a special themed look.

In order to configure wxWidgets to compile wxX11 you will need to type:

@verbatim configure --with-x11 --with-universal @endverbatim

For further information, please see the files in @c docs/x11 in the
distribution. There is also a page on the use of wxWidgets for embedded
applications on the wxWidgets web site.



@section page_port_wxmotif wxMotif

wxMotif is a port of wxWidgets for X11 systems using Motif libraries. Motif
libraries provide a clean and fast user interface at the expense of the beauty
and candy of newer interfaces like GTK.

For further information, please see the files in @c docs/motif in the
distribution.



@section page_port_wxmsw wxMSW

wxMSW is a port of wxWidgets for the Windows platforms (Windows XP and later
are supported). wxMSW provides native look and feel for each Windows version.
This port can be compiled with several compilers including Microsoft Studio
VC++ 2003 or later, Borland 5.5, MinGW32, Cygwin as well as cross-compilation
with a Linux-hosted MinGW32 tool chain.

For further information, please see the files in docs/msw in the distribution.

@subsection page_port_wxmsw_resources Resources and Application Icon

All applications using wxMSW should have a Windows resource file (@c .rc
extension) and this file should include @c include/wx/msw/wx.rc file which
defines resources used by wxWidgets itself.

Among other things, @c wx.rc defines some standard icons, all of which have
names starting with the "wx" prefix. This normally ensures that any icons
defined in the application's own resource file come before them in alphabetical
order which is important because Explorer (Windows shell) selects the first
icon in alphabetical order to use as the application icon which is displayed
when viewing its file in the file manager. So if all the icons defined in your
application start with "x", "y" or "z", they won't be used by Explorer. To
avoid this, ensure that the icon which is meant to be used as the main
application icon has a name preceding "wxICON" in alphabetical order.


@subsection page_port_wxmsw_themedborders Themed Borders

Starting with wxWidgets 2.8.5, you can specify the @c wxBORDER_THEME style to
have wxWidgets use a themed border. Using the default XP theme, this is a thin
1-pixel blue border, with an extra 1-pixel border in the window client
background colour (usually white) to separate the client area's scrollbars from
the border.

If you don't specify a border style for a wxTextCtrl in rich edit mode,
wxWidgets now gives the control themed borders automatically, where previously
they would take the Windows 95-style sunken border. Other native controls such
as wxTextCtrl in non-rich edit mode, and wxComboBox already paint themed
borders where appropriate. To use themed borders on other windows, such as
wxPanel, pass the @c wxBORDER_THEME style, or (apart from wxPanel) pass no
border style.

In general, specifying @c wxBORDER_THEME will cause a border of some kind to be
used, chosen by the platform and control class. To leave the border decision
entirely to wxWidgets, pass @c wxBORDER_DEFAULT. This is not to be confused
with specifying @c wxBORDER_NONE, which says that there should definitely be
@e no border.

@subsubsection page_port_wxmsw_themedborders_details Internal Border Implementation

The way that wxMSW decides whether to apply a themed border is as follows. The
theming code calls wxWindow::GetBorder() to obtain a border. If no border style
has been passed to the window constructor, GetBorder() calls GetDefaultBorder()
for this window. If wxBORDER_THEME was passed to the window constructor,
GetBorder() calls GetDefaultBorderForControl().

The implementation of wxWindow::GetDefaultBorder() on wxMSW calls
wxWindow::CanApplyThemeBorder() which is a virtual function that tells
wxWidgets whether a control can have a theme applied explicitly (some native
controls already paint a theme in which case we should not apply it ourselves).
Note that wxPanel is an exception to this rule because in many cases we wish to
create a window with no border (for example, notebook pages). So wxPanel
overrides GetDefaultBorder() in order to call the generic
wxWindowBase::GetDefaultBorder(), returning wxBORDER_NONE.

@section page_port_nativedocs Native Toolkit Documentation

It's sometimes useful to interface directly with the underlying toolkit
used by wxWidgets to e.g. use toolkit-specific features.
In such case (or when you want to e.g. write a port-specific patch) it can be
necessary to use the underlying toolkit API directly:

- wxMSW port uses win32 API: see MSDN docs at http://msdn2.microsoft.com/en-us/library/ms649779.aspx
- wxGTK port uses GTK+ and other lower-level libraries; see 
  - GTK+ docs at http://library.gnome.org/devel/gtk/unstable/
  - GDK docs at http://library.gnome.org/devel/gdk/unstable/
  - GLib docs at http://library.gnome.org/devel/glib/unstable/
  - GObject docs at http://library.gnome.org/devel/gobject/unstable/
  - Pango docs at http://library.gnome.org/devel/pango/unstable/
- wxMac port uses the Carbon API: see Carbon docs at http://developer.apple.com/carbon
- wxCocoa port uses the Cocoa API: see Cocoa docs at http://developer.apple.com/cocoa

*/
