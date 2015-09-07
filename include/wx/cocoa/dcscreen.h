/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/dcscreen.h
// Purpose:     wxScreenDCImpl class
// Author:      David Elliott
// Modified by:
// Created:     2003/03/16
// Copyright:   (c) 2003 David Elliott
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_COCOA_DCSCREEN_H__
#define __WX_COCOA_DCSCREEN_H__

#include "wx/dcscreen.h"
#include "wx/cocoa/dc.h"

class WXDLLIMPEXP_CORE wxScreenDCImpl: public wxCocoaDCImpl
{
    DECLARE_DYNAMIC_CLASS(wxScreenDCImpl)
public:
    wxScreenDCImpl(wxScreenDC *owner);
    wxScreenDCImpl(wxScreenDC *owner, wxDC *dc ); // Create compatible DC
    virtual ~wxScreenDCImpl(void);

    // Compatibility with X's requirements for drawing on top of all windows
    static bool StartDrawingOnTop(wxWindow* WXUNUSED(window)) { return true; }
    static bool StartDrawingOnTop(wxRect* WXUNUSED(rect) = NULL) { return true; }
    static bool EndDrawingOnTop() { return true; }
};

#endif // __WX_COCOA_DCSCREEN_H__
