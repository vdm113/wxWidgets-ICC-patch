/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_radbt.h
// Purpose:     XML resource handler for wxRadioButton
// Author:      Bob Mitchell
// Created:     2000/03/21
// Copyright:   (c) 2000 Bob Mitchell and Verant Interactive
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_RADBT_H_
#define _WX_XH_RADBT_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_RADIOBTN

class WXDLLIMPEXP_XRC wxRadioButtonXmlHandler : public wxXmlResourceHandler
{
    DECLARE_DYNAMIC_CLASS(wxRadioButtonXmlHandler)

public:
    wxRadioButtonXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

#endif // wxUSE_XRC && wxUSE_RADIOBOX

#endif // _WX_XH_RADBT_H_
