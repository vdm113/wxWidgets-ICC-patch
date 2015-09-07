/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_scrol.h
// Purpose:     XML resource handler for wxScrollBar
// Author:      Brian Gavin
// Created:     2000/09/09
// Copyright:   (c) 2000 Brian Gavin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_SCROL_H_
#define _WX_XH_SCROL_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_SCROLLBAR

class WXDLLIMPEXP_XRC wxScrollBarXmlHandler : public wxXmlResourceHandler
{
public:
    wxScrollBarXmlHandler();
    virtual wxObject *DoCreateResource() wxOVERRIDE;
    virtual bool CanHandle(wxXmlNode *node) wxOVERRIDE;

    wxDECLARE_DYNAMIC_CLASS(wxScrollBarXmlHandler);
};

#endif // wxUSE_XRC && wxUSE_SCROLLBAR

#endif // _WX_XH_SCROL_H_
