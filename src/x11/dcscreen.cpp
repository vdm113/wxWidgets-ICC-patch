/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/x11/dcscreen.cpp
// Purpose:     wxScreenDC class
// Author:      Julian Smart, Robert Roebling
// Modified by:
// Created:     17/09/98
// Copyright:   (c) Julian Smart, Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/dcscreen.h"
#include "wx/x11/dcscreen.h"

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/utils.h"
    #include "wx/window.h"
    #include "wx/frame.h"
#endif

#include "wx/fontutil.h"

#include "wx/x11/private.h"

//-----------------------------------------------------------------------------
// wxScreenDC
//-----------------------------------------------------------------------------

wxIMPLEMENT_ABSTRACT_CLASS(wxScreenDCImpl, wxPaintDCImpl);

wxScreenDCImpl::wxScreenDCImpl( wxDC* owner )
              : wxPaintDCImpl( owner )
{
    m_ok = false;

    m_display = (WXDisplay *) wxGlobalDisplay();

    int screen = DefaultScreen( (Display*) m_display );
    m_cmap = (WXColormap) DefaultColormap( (Display*) m_display, screen );

    m_x11window = (WXWindow) RootWindow( (Display*) m_display, screen );

    m_isScreenDC = true;

#if wxUSE_UNICODE
    m_context = wxTheApp->GetPangoContext();
    m_fontdesc = wxNORMAL_FONT->GetNativeFontInfo()->description;
#endif

    SetUpDC();

    XSetSubwindowMode( (Display*) m_display, (GC) m_penGC, IncludeInferiors );
    XSetSubwindowMode( (Display*) m_display, (GC) m_brushGC, IncludeInferiors );
    XSetSubwindowMode( (Display*) m_display, (GC) m_textGC, IncludeInferiors );
    XSetSubwindowMode( (Display*) m_display, (GC) m_bgGC, IncludeInferiors );
}

wxScreenDCImpl::~wxScreenDCImpl()
{
    XSetSubwindowMode( (Display*) m_display, (GC) m_penGC, ClipByChildren );
    XSetSubwindowMode( (Display*) m_display, (GC) m_brushGC, ClipByChildren );
    XSetSubwindowMode( (Display*) m_display, (GC) m_textGC, ClipByChildren );
    XSetSubwindowMode( (Display*) m_display, (GC) m_bgGC, ClipByChildren );
}

void wxScreenDCImpl::DoGetSize(int *width, int *height) const
{
    wxDisplaySize(width, height);
}
