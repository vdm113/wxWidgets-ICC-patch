/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/xrc/xh_gauge.cpp
// Purpose:     XRC resource for wxGauge
// Author:      Bob Mitchell
// Created:     2000/03/21
// Copyright:   (c) 2000 Bob Mitchell and Verant Interactive
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC && wxUSE_GAUGE

#include "wx/xrc/xh_gauge.h"

#ifndef WX_PRECOMP
    #include "wx/gauge.h"
#endif

static const long DEFAULT_RANGE = 100;

wxIMPLEMENT_DYNAMIC_CLASS(wxGaugeXmlHandler, wxXmlResourceHandler);

wxGaugeXmlHandler::wxGaugeXmlHandler()
                  :wxXmlResourceHandler()
{
    XRC_ADD_STYLE(wxGA_HORIZONTAL);
    XRC_ADD_STYLE(wxGA_VERTICAL);
    XRC_ADD_STYLE(wxGA_SMOOTH);   // windows only
    AddWindowStyles();
}

wxObject *wxGaugeXmlHandler::DoCreateResource()
{
    XRC_MAKE_INSTANCE(control, wxGauge)

    control->Create(m_parentAsWindow,
                    GetID(),
                    GetLong(wxT("range"), DEFAULT_RANGE),
                    GetPosition(), GetSize(),
                    GetStyle(),
                    wxDefaultValidator,
                    GetName());

    if( HasParam(wxT("value")))
    {
        control->SetValue(GetLong(wxT("value")));
    }

    SetupWindow(control);

    return control;
}

bool wxGaugeXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxGauge"));
}

#endif // wxUSE_XRC && wxUSE_GAUGE
