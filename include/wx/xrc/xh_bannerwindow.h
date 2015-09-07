/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_bannerwindow.h
// Purpose:     Declaration of wxBannerWindow XRC handler.
// Author:      Vadim Zeitlin
// Created:     2011-08-16
// Copyright:   (c) 2011 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_BANNERWINDOW_H_
#define _WX_XH_BANNERWINDOW_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_BANNERWINDOW

class WXDLLIMPEXP_XRC wxBannerWindowXmlHandler : public wxXmlResourceHandler
{
public:
    wxBannerWindowXmlHandler();

    virtual wxObject *DoCreateResource() wxOVERRIDE;
    virtual bool CanHandle(wxXmlNode *node) wxOVERRIDE;

    wxDECLARE_DYNAMIC_CLASS(wxBannerWindowXmlHandler);
};

#endif // wxUSE_XRC && wxUSE_BANNERWINDOW

#endif // _WX_XH_BANNERWINDOW_H_
