/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_timectrl.h
// Purpose:     XML resource handler for wxTimePickerCtrl
// Author:      Vadim Zeitlin
// Created:     2011-09-22
// Copyright:   (c) 2011 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_TIMECTRL_H_
#define _WX_XH_TIMECTRL_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_TIMEPICKCTRL

class WXDLLIMPEXP_XRC wxTimeCtrlXmlHandler : public wxXmlResourceHandler
{
public:
    wxTimeCtrlXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

private:
    wxDECLARE_DYNAMIC_CLASS(wxTimeCtrlXmlHandler);
};

#endif // wxUSE_XRC && wxUSE_TIMEPICKCTRL

#endif // _WX_XH_TIMECTRL_H_
