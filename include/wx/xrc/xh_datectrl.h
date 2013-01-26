/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_datectrl.h
// Purpose:     XML resource handler for wxDatePickerCtrl
// Author:      Vaclav Slavik
// Created:     2005-02-07
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_DATECTRL_H_
#define _WX_XH_DATECTRL_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_DATEPICKCTRL

class WXDLLIMPEXP_XRC wxDateCtrlXmlHandler : public wxXmlResourceHandler
{
    DECLARE_DYNAMIC_CLASS(wxDateCtrlXmlHandler)

public:
    wxDateCtrlXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

#endif // wxUSE_XRC && wxUSE_DATEPICKCTRL

#endif // _WX_XH_DATECTRL_H_
