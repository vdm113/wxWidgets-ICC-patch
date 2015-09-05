/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/////////////////////////////////////////////////////////////////////////////
// Name:        src/xrc/xh_stbox.cpp
// Purpose:     XRC resource for wxStaticBox
// Author:      Brian Gavin
// Created:     2000/09/09
// Copyright:   (c) 2000 Brian Gavin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC && wxUSE_STATBOX

#include "wx/xrc/xh_stbox.h"

#ifndef WX_PRECOMP
    #include "wx/statbox.h"
#endif

wxIMPLEMENT_DYNAMIC_CLASS(wxStaticBoxXmlHandler, wxXmlResourceHandler);

wxStaticBoxXmlHandler::wxStaticBoxXmlHandler()
                      :wxXmlResourceHandler()
{
    AddWindowStyles();
}

wxObject *wxStaticBoxXmlHandler::DoCreateResource()
{
    XRC_MAKE_INSTANCE(box, wxStaticBox)

    box->Create(m_parentAsWindow,
                GetID(),
                GetText(wxT("label")),
                GetPosition(), GetSize(),
                GetStyle(),
                GetName());

    SetupWindow(box);

    return box;
}

bool wxStaticBoxXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxStaticBox"));
}

#endif // wxUSE_XRC && wxUSE_STATBOX
