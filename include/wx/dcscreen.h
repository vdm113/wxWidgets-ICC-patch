/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/dcscreen.h
// Purpose:     wxScreenDC base header
// Author:      Julian Smart
// Modified by:
// Created:
// Copyright:   (c) Julian Smart
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCSCREEN_H_BASE_
#define _WX_DCSCREEN_H_BASE_

#include "wx/defs.h"
#include "wx/dc.h"

class WXDLLIMPEXP_CORE wxScreenDC : public wxDC
{
public:
    wxScreenDC();

    static bool StartDrawingOnTop(wxWindow * WXUNUSED(window))
        { return true; }
    static bool StartDrawingOnTop(wxRect * WXUNUSED(rect) =  NULL)
        { return true; }
    static bool EndDrawingOnTop()
        { return true; }

private:
    wxDECLARE_DYNAMIC_CLASS(wxScreenDC);
};


#endif
    // _WX_DCSCREEN_H_BASE_
