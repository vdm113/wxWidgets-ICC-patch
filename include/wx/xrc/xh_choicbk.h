/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_choicbk.h
// Purpose:     XML resource handler for wxChoicebook
// Author:      Vaclav Slavik
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_CHOICEBK_H_
#define _WX_XH_CHOICEBK_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_CHOICEBOOK

class WXDLLIMPEXP_FWD_CORE wxChoicebook;

class WXDLLIMPEXP_XRC wxChoicebookXmlHandler : public wxXmlResourceHandler
{
    wxDECLARE_DYNAMIC_CLASS(wxChoicebookXmlHandler);

public:
    wxChoicebookXmlHandler();
    virtual wxObject *DoCreateResource() wxOVERRIDE;
    virtual bool CanHandle(wxXmlNode *node) wxOVERRIDE;

private:
    bool m_isInside;
    wxChoicebook *m_choicebook;
};

#endif // wxUSE_XRC && wxUSE_CHOICEBOOK

#endif // _WX_XH_CHOICEBK_H_
