/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/xrc/xh_clrpicker.cpp
// Purpose:     XML resource handler for wxColourPickerCtrl
// Author:      Francesco Montorsi
// Created:     2006-04-17
// Copyright:   (c) 2006 Francesco Montorsi
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC && wxUSE_COLOURPICKERCTRL

#include "wx/xrc/xh_clrpicker.h"
#include "wx/clrpicker.h"

IMPLEMENT_DYNAMIC_CLASS(wxColourPickerCtrlXmlHandler, wxXmlResourceHandler)

wxColourPickerCtrlXmlHandler::wxColourPickerCtrlXmlHandler() : wxXmlResourceHandler()
{
    XRC_ADD_STYLE(wxCLRP_USE_TEXTCTRL);
    XRC_ADD_STYLE(wxCLRP_SHOW_LABEL);
    XRC_ADD_STYLE(wxCLRP_DEFAULT_STYLE);
    AddWindowStyles();
}

wxObject *wxColourPickerCtrlXmlHandler::DoCreateResource()
{
   XRC_MAKE_INSTANCE(picker, wxColourPickerCtrl)

   picker->Create(m_parentAsWindow,
                  GetID(),
                  GetColour(wxT("value"), *wxBLACK),
                  GetPosition(), GetSize(),
                  GetStyle(wxT("style"), wxCLRP_DEFAULT_STYLE),
                  wxDefaultValidator,
                  GetName());

    SetupWindow(picker);

    return picker;
}

bool wxColourPickerCtrlXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxColourPickerCtrl"));
}

#endif // wxUSE_XRC && wxUSE_COLOURPICKERCTRL
