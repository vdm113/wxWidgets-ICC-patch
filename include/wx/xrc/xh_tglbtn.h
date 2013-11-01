/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_tglbtn.h
// Purpose:     XML resource handler for wxToggleButton
// Author:      Julian Smart
// Created:     2004-08-30
// Copyright:   (c) 2004 Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_TGLBTN_H_
#define _WX_XH_TGLBTN_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_TOGGLEBTN

class WXDLLIMPEXP_XRC wxToggleButtonXmlHandler : public wxXmlResourceHandler
{
    DECLARE_DYNAMIC_CLASS(wxToggleButtonXmlHandler)

public:
    wxToggleButtonXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

protected:
    virtual void DoCreateToggleButton(wxObject *control);
#if !defined(__WXUNIVERSAL__) && !defined(__WXMOTIF__) && !defined(__WXPM__) && !(defined(__WXGTK__) && !defined(__WXGTK20__))
    virtual void DoCreateBitmapToggleButton(wxObject *control);
#endif
};

#endif // wxUSE_XRC && wxUSE_TOGGLEBTN

#endif // _WX_XH_TGLBTN_H_
