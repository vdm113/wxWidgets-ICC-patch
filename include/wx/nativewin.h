/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/nativewin.h
// Purpose:     classes allowing to wrap a native window handle
// Author:      Vadim Zeitlin
// Created:     2008-03-05
// Copyright:   (c) 2008 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_NATIVEWIN_H_
#define _WX_NATIVEWIN_H_

#include "wx/toplevel.h"

// These symbols can be tested in the user code to see if the current wx port
// has support for creating wxNativeContainerWindow and wxNativeWindow from
// native windows.
//
// Be optimistic by default, we undefine them below if necessary.
#define wxHAS_NATIVE_CONTAINER_WINDOW
#define wxHAS_NATIVE_WINDOW

// we define the following typedefs for each of the platform supporting native
// windows wrapping:
//
//  - wxNativeContainerWindowHandle is the toolkit-level handle of the native
//    window, i.e. HWND/GdkWindow*/NSWindow
//
//  - wxNativeContainerWindowId is the lowest level identifier of the native
//    window, i.e. HWND/GdkNativeWindow/NSWindow (so it's the same as above for
//    all platforms except GTK where we also can work with Window/XID)
//
//  - wxNativeWindowHandle for child windows, i.e. HWND/GtkWidget*/NSControl
#if defined(__WXMSW__)
    #include "wx/msw/wrapwin.h"

    typedef HWND wxNativeContainerWindowId;
    typedef HWND wxNativeContainerWindowHandle;
    typedef HWND wxNativeWindowHandle;
#elif defined(__WXGTK__)
    #include <gtk/gtk.h>

    // GdkNativeWindow is guint32 under GDK/X11 and gpointer under GDK/WIN32
    #ifdef __UNIX__
        typedef unsigned long wxNativeContainerWindowId;
    #else
        typedef void *wxNativeContainerWindowId;
    #endif
    typedef GdkWindow *wxNativeContainerWindowHandle;
    typedef GtkWidget *wxNativeWindowHandle;
#elif defined(__WXOSX_COCOA__)
    typedef NSView *wxNativeWindowHandle;

    // no support for using native TLWs yet
    #undef wxHAS_NATIVE_CONTAINER_WINDOW
#else
    // no support for using native windows under this platform yet
    #undef wxHAS_NATIVE_CONTAINER_WINDOW
    #undef wxHAS_NATIVE_WINDOW
#endif

#ifdef wxHAS_NATIVE_WINDOW

// ----------------------------------------------------------------------------
// wxNativeWindow: for using native windows inside wxWidgets windows
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxNativeWindow : public wxWindow
{
public:
    // Default ctor, Create() must be called later to really create the window.
    wxNativeWindow() { }

    // Create a window from an existing native window handle.
    //
    // Notice that this ctor doesn't take the usual pos and size parameters,
    // they're taken from the window handle itself.
    //
    // Use GetHandle() to check if the creation was successful, it will return
    // 0 if the handle was invalid.
    wxNativeWindow(wxWindow* parent, wxWindowID winid, wxNativeWindowHandle handle)
    {
        Create(parent, winid, handle);
    }

    // Same as non-default ctor, but with a return code.
    bool Create(wxWindow* parent, wxWindowID winid, wxNativeWindowHandle handle);

private:
    wxDECLARE_NO_COPY_CLASS(wxNativeWindow);
};

#endif // wxHAS_NATIVE_WINDOW

#ifdef wxHAS_NATIVE_CONTAINER_WINDOW

// ----------------------------------------------------------------------------
// wxNativeContainerWindow: can be used for creating other wxWindows inside it
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxNativeContainerWindow : public wxTopLevelWindow
{
public:
    // default ctor, call Create() later
    wxNativeContainerWindow() { }

    // create a window from an existing native window handle
    //
    // use GetHandle() to check if the creation was successful, it will return
    // 0 if the handle was invalid
    wxNativeContainerWindow(wxNativeContainerWindowHandle handle)
    {
        Create(handle);
    }

    // same as ctor above but with a return code
    bool Create(wxNativeContainerWindowHandle handle);

#if defined(__WXGTK__)
    // this is a convenient ctor for wxGTK applications which can also create
    // the objects of this class from the really native window handles and not
    // only the GdkWindow objects
    //
    // wxNativeContainerWindowId is Window (i.e. an XID, i.e. an int) under X11
    // (when GDK_WINDOWING_X11 is defined) or HWND under Win32
    wxNativeContainerWindow(wxNativeContainerWindowId winid) { Create(winid); }

    bool Create(wxNativeContainerWindowId winid);
#endif // wxGTK

    // unlike for the normal windows, dtor will not destroy the native window
    // as it normally doesn't belong to us
    virtual ~wxNativeContainerWindow();


    // provide (trivial) implementation of the base class pure virtuals
    virtual void SetTitle(const wxString& WXUNUSED(title)) wxOVERRIDE
    {
        wxFAIL_MSG( "not implemented for native windows" );
    }

    virtual wxString GetTitle() const wxOVERRIDE
    {
        wxFAIL_MSG( "not implemented for native windows" );

        return wxString();
    }

    virtual void Maximize(bool WXUNUSED(maximize) = true) wxOVERRIDE
    {
        wxFAIL_MSG( "not implemented for native windows" );
    }

    virtual bool IsMaximized() const wxOVERRIDE
    {
        wxFAIL_MSG( "not implemented for native windows" );

        return false;
    }

    virtual void Iconize(bool WXUNUSED(iconize) = true) wxOVERRIDE
    {
        wxFAIL_MSG( "not implemented for native windows" );
    }

    virtual bool IsIconized() const wxOVERRIDE
    {
        // this is called by wxGTK implementation so don't assert
        return false;
    }

    virtual void Restore() wxOVERRIDE
    {
        wxFAIL_MSG( "not implemented for native windows" );
    }

    virtual bool ShowFullScreen(bool WXUNUSED(show),
                                long WXUNUSED(style) = wxFULLSCREEN_ALL) wxOVERRIDE
    {
        wxFAIL_MSG( "not implemented for native windows" );

        return false;
    }

    virtual bool IsFullScreen() const wxOVERRIDE
    {
        wxFAIL_MSG( "not implemented for native windows" );

        return false;
    }

#ifdef __WXMSW__
    virtual bool IsShown() const wxOVERRIDE;
#endif // __WXMSW__

    // this is an implementation detail: called when the native window is
    // destroyed by an outside agency; deletes the C++ object too but can in
    // principle be overridden to something else (knowing that the window
    // handle of this object and all of its children is invalid any more)
    virtual void OnNativeDestroyed();

protected:
#ifdef __WXMSW__
    virtual WXLRESULT
    MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam);
#endif // __WXMSW__

private:
    wxDECLARE_NO_COPY_CLASS(wxNativeContainerWindow);
};

#endif // wxHAS_NATIVE_CONTAINER_WINDOW

#endif // _WX_NATIVEWIN_H_

