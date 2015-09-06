/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_mdi.h
// Purpose:     XML resource handler for wxMDI
// Author:      David M. Falkinder & Vaclav Slavik
// Created:     14/02/2005
// Copyright:   (c) 2005 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_MDI_H_
#define _WX_XH_MDI_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_MDI

class WXDLLIMPEXP_FWD_CORE wxWindow;

class WXDLLIMPEXP_XRC wxMdiXmlHandler : public wxXmlResourceHandler
{
    wxDECLARE_DYNAMIC_CLASS(wxMdiXmlHandler);

public:
    wxMdiXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

private:
    wxWindow *CreateFrame();
};

#endif // wxUSE_XRC && wxUSE_MDI

#endif // _WX_XH_MDI_H_
