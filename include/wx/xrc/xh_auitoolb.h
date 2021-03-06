/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_toolb.h
// Purpose:     XML resource handler for wxAuiToolBar
// Author:      Rodolphe Suescun
// Created:     2013-11-23
// Copyright:   (c) 2013 Rodolphe Suescun
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_AUITOOLB_H_
#define _WX_XH_AUITOOLB_H_

#include "wx/aui/auibar.h"
#include "wx/menu.h"
#include "wx/vector.h"
#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_AUI

class WXDLLIMPEXP_FWD_AUI wxAuiToolBar;

class WXDLLIMPEXP_AUI wxAuiToolBarXmlHandler : public wxXmlResourceHandler
{
public:
    wxAuiToolBarXmlHandler();
    virtual wxObject *DoCreateResource() wxOVERRIDE;
    virtual bool CanHandle(wxXmlNode *node) wxOVERRIDE;

private:
    bool m_isInside;
    wxAuiToolBar *m_toolbar;
    wxSize m_toolSize;

    class MenuHandler : public wxEvtHandler
    {
    public:
        void OnDropDown(wxAuiToolBarEvent& event);
        unsigned RegisterMenu(wxAuiToolBar *toobar, int id, wxMenu *menu);

    private:
        wxVector<wxMenu*> m_menus;
    };

    MenuHandler m_menuHandler;

    wxDECLARE_DYNAMIC_CLASS(wxAuiToolBarXmlHandler);
};

#endif // wxUSE_XRC && wxUSE_AUI

#endif // _WX_XH_AUITOOLB_H_
