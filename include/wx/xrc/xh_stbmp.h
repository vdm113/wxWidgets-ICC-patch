/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////


// Name:        wx/xrc/xh_stbmp.h
// Purpose:     XML resource handler for wxStaticBitmap
// Author:      Vaclav Slavik
// Created:     2000/04/22
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_STBMP_H_
#define _WX_XH_STBMP_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_STATBMP

class WXDLLIMPEXP_XRC wxStaticBitmapXmlHandler : public wxXmlResourceHandler
{
    DECLARE_DYNAMIC_CLASS(wxStaticBitmapXmlHandler)

public:
    wxStaticBitmapXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

#endif // wxUSE_XRC && wxUSE_STATBMP

#endif // _WX_XH_STBMP_H_
