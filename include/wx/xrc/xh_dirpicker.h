/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_dirpicker.h
// Purpose:     XML resource handler for wxDirPickerCtrl
// Author:      Francesco Montorsi
// Created:     2006-04-17
// Copyright:   (c) 2006 Francesco Montorsi
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_DIRPICKERCTRL_H_
#define _WX_XH_DIRPICKERCTRL_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_DIRPICKERCTRL

class WXDLLIMPEXP_XRC wxDirPickerCtrlXmlHandler : public wxXmlResourceHandler
{
    wxDECLARE_DYNAMIC_CLASS(wxDirPickerCtrlXmlHandler);

public:
    wxDirPickerCtrlXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

#endif // wxUSE_XRC && wxUSE_DIRPICKERCTRL

#endif // _WX_XH_DIRPICKERCTRL_H_
