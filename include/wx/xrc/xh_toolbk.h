/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_toolbk.h
// Purpose:     XML resource handler for wxToolbook
// Author:      Andrea Zanellato
// Created:     2009/12/12
// Copyright:   (c) 2010 wxWidgets development team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_TOOLBK_H_
#define _WX_XH_TOOLBK_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_TOOLBOOK

class WXDLLIMPEXP_FWD_CORE wxToolbook;

class WXDLLIMPEXP_XRC wxToolbookXmlHandler : public wxXmlResourceHandler
{
public:
    wxToolbookXmlHandler();

    virtual wxObject *DoCreateResource() wxOVERRIDE;
    virtual bool CanHandle(wxXmlNode *node) wxOVERRIDE;

private:
    bool m_isInside;
    wxToolbook *m_toolbook;

    wxDECLARE_DYNAMIC_CLASS(wxToolbookXmlHandler);
};

#endif // wxUSE_XRC && wxUSE_TOOLBOOK

#endif // _WX_XH_TOOLBK_H_
