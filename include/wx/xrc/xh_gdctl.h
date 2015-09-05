/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_gdctl.h
// Purpose:     XML resource handler for wxGenericDirCtrl
// Author:      Markus Greither
// Created:     2002/01/20
// Copyright:   (c) 2002 Markus Greither
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_GDCTL_H_
#define _WX_XH_GDCTL_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_DIRDLG

class WXDLLIMPEXP_XRC wxGenericDirCtrlXmlHandler : public wxXmlResourceHandler
{
    wxDECLARE_DYNAMIC_CLASS(wxGenericDirCtrlXmlHandler);

public:
    wxGenericDirCtrlXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

#endif // wxUSE_XRC && wxUSE_DIRDLG

#endif // _WX_XH_GDCTL_H_
