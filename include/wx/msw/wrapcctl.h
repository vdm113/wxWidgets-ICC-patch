/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/wrapcctl.h
// Purpose:     Wrapper for the standard <commctrl.h> header
// Author:      Vadim Zeitlin
// Modified by:
// Created:     03.08.2003
// Copyright:   (c) 2003 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MSW_WRAPCCTL_H_
#define _WX_MSW_WRAPCCTL_H_

#include "wx/msw/wrapwin.h"

#include <commctrl.h>

// define things which might be missing from our commctrl.h
#include "wx/msw/missing.h"

// Set Unicode format for a common control
inline void wxSetCCUnicodeFormat(HWND hwnd)
{
    ::SendMessage(hwnd, CCM_SETUNICODEFORMAT, wxUSE_UNICODE, 0);
}

#if wxUSE_GUI
// Return the default font for the common controls
//
// this is implemented in msw/settings.cpp
class wxFont;
extern wxFont wxGetCCDefaultFont();

// this is just a wrapper for HDITEM which we can't use in the public header
// because we don't want to include commctrl.h (and hence windows.h) from there
struct wxHDITEM : public HDITEM
{
    wxHDITEM()
    {
        ::ZeroMemory(this, sizeof(*this));
    }
};

#endif // wxUSE_GUI

#endif // _WX_MSW_WRAPCCTL_H_
