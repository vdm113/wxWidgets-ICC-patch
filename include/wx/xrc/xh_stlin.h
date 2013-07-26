/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_stlin.h
// Purpose:     XML resource handler for wxStaticLine
// Author:      Vaclav Slavik
// Created:     2000/09/00
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_STLIN_H_
#define _WX_XH_STLIN_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_STATLINE

class WXDLLIMPEXP_XRC wxStaticLineXmlHandler : public wxXmlResourceHandler
{
    DECLARE_DYNAMIC_CLASS(wxStaticLineXmlHandler)

public:
    wxStaticLineXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

#endif // wxUSE_XRC && wxUSE_STATLINE

#endif // _WX_XH_STLIN_H_
