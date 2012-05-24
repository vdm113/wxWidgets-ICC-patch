/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_cmdlinkbn.h
// Purpose:     XML resource handler for command link buttons
// Author:      Kinaou Herve
// Created:     2010-10-20
// RCS-ID:      $Id:$
// Copyright:   (c) 2010 wxWidgets development team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_CMDLINKBN_H_
#define _WX_XH_CMDLINKBN_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_COMMANDLINKBUTTON

class WXDLLIMPEXP_XRC wxCommandLinkButtonXmlHandler : public wxXmlResourceHandler
{
public:
    wxCommandLinkButtonXmlHandler();

    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

private:
    wxDECLARE_DYNAMIC_CLASS(wxCommandLinkButtonXmlHandler);
};

#endif // wxUSE_XRC && wxUSE_COMMANDLINKBUTTON

#endif // _WX_XH_CMDLINKBN_H_
