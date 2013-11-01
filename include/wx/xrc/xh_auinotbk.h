/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_auinotbk.h
// Purpose:     XML resource handler for wxAuiNotebook
// Author:      Steve Lamerton
// Created:     2009-06-12
// Copyright:   (c) 2009 Steve Lamerton
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XRC_XH_AUINOTEBOOK_H_
#define _WX_XRC_XH_AUINOTEBOOK_H_

#include "wx/xrc/xmlres.h"

class WXDLLIMPEXP_FWD_AUI wxAuiNotebook;

#if wxUSE_XRC && wxUSE_AUI

class WXDLLIMPEXP_AUI wxAuiNotebookXmlHandler : public wxXmlResourceHandler
{
public:
    wxAuiNotebookXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

private:
    bool m_isInside;
    wxAuiNotebook *m_notebook;

    wxDECLARE_DYNAMIC_CLASS(wxAuiNotebookXmlHandler);
};

#endif // wxUSE_XRC && wxUSE_AUI

#endif // _WX_XRC_XH_AUINOTEBOOK_H_
