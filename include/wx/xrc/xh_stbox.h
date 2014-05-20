/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_stbox.h
// Purpose:     XML resource handler for wxStaticBox
// Author:      Brian Gavin
// Created:     2000/09/00
// Copyright:   (c) 2000 Brian Gavin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_STBOX_H_
#define _WX_XH_STBOX_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_STATBOX

class WXDLLIMPEXP_XRC wxStaticBoxXmlHandler : public wxXmlResourceHandler
{
    DECLARE_DYNAMIC_CLASS(wxStaticBoxXmlHandler)

public:
    wxStaticBoxXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

#endif // wxUSE_XRC && wxUSE_STATBOX

#endif // _WX_XH_STBOX_H_
