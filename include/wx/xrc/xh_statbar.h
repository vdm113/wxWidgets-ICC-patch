/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_statbar.h
// Purpose:     XML resource handler for wxStatusBar
// Author:      Brian Ravnsgaard Riis
// Created:     2004/01/21
// Copyright:   (c) 2004 Brian Ravnsgaard Riis
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_STATBAR_H_
#define _WX_XH_STATBAR_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_STATUSBAR

class WXDLLIMPEXP_XRC wxStatusBarXmlHandler : public wxXmlResourceHandler
{
    wxDECLARE_DYNAMIC_CLASS(wxStatusBarXmlHandler);

public:
    wxStatusBarXmlHandler();
    virtual wxObject *DoCreateResource() wxOVERRIDE;
    virtual bool CanHandle(wxXmlNode *node) wxOVERRIDE;
};

#endif // wxUSE_XRC && wxUSE_STATUSBAR

#endif // _WX_XH_STATBAR_H_
