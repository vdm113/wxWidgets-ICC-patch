/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_animatctrl.h
// Purpose:     XML resource handler for wxAnimationCtrl
// Author:      Francesco Montorsi
// Created:     2006-10-15
// Copyright:   (c) 2006 Francesco Montorsi
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_ANIMATIONCTRL_H_
#define _WX_XH_ANIMATIONCTRL_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_ANIMATIONCTRL

class WXDLLIMPEXP_XRC wxAnimationCtrlXmlHandler : public wxXmlResourceHandler
{
    wxDECLARE_DYNAMIC_CLASS(wxAnimationCtrlXmlHandler);

public:
    wxAnimationCtrlXmlHandler();
    virtual wxObject *DoCreateResource() wxOVERRIDE;
    virtual bool CanHandle(wxXmlNode *node) wxOVERRIDE;
};

#endif // wxUSE_XRC && wxUSE_ANIMATIONCTRL

#endif // _WX_XH_ANIMATIONCTRL_H_
