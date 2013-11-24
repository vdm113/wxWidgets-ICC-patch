/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_scrol.h
// Purpose:     XML resource handler for wxScrollBar
// Author:      Brian Gavin
// Created:     2000/09/09
// Copyright:   (c) 2000 Brian Gavin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_SCROL_H_
#define _WX_XH_SCROL_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_SCROLLBAR

class WXDLLIMPEXP_XRC wxScrollBarXmlHandler : public wxXmlResourceHandler
{
public:
    wxScrollBarXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

    DECLARE_DYNAMIC_CLASS(wxScrollBarXmlHandler)
};

#endif // wxUSE_XRC && wxUSE_SCROLLBAR

#endif // _WX_XH_SCROL_H_
