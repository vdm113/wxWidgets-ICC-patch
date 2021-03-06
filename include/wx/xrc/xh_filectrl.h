/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_filectrl.h
// Purpose:     XML resource handler for wxFileCtrl
// Author:      Kinaou Hervé
// Created:     2009-05-11
// Copyright:   (c) 2009 wxWidgets development team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_FILECTRL_H_
#define _WX_XH_FILECTRL_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_FILECTRL

class WXDLLIMPEXP_XRC wxFileCtrlXmlHandler : public wxXmlResourceHandler
{
public:
    wxFileCtrlXmlHandler();
    virtual wxObject *DoCreateResource() wxOVERRIDE;
    virtual bool CanHandle(wxXmlNode *node) wxOVERRIDE;

private:
    wxDECLARE_DYNAMIC_CLASS(wxFileCtrlXmlHandler);
};

#endif // wxUSE_XRC && wxUSE_FILECTRL

#endif // _WX_XH_FILEPICKERCTRL_H_
