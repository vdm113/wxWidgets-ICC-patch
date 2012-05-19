/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////


// Name:        wx/cocoa/dcscreen.h
// Purpose:     wxScreenDCImpl class
// Author:      David Elliott
// Modified by:
// Created:     2003/03/16
// RCS-ID:      $Id$
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
