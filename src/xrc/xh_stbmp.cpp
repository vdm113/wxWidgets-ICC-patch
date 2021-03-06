/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/xrc/xh_stbmp.cpp
// Purpose:     XRC resource for wxStaticBitmap
// Author:      Vaclav Slavik
// Created:     2000/04/22
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC && wxUSE_STATBMP

#include "wx/xrc/xh_stbmp.h"

#ifndef  WX_PRECOMP
    #include "wx/statbmp.h"
#endif

wxIMPLEMENT_DYNAMIC_CLASS(wxStaticBitmapXmlHandler, wxXmlResourceHandler);

wxStaticBitmapXmlHandler::wxStaticBitmapXmlHandler()
                         :wxXmlResourceHandler()
{
    AddWindowStyles();
}

wxObject *wxStaticBitmapXmlHandler::DoCreateResource()
{
    XRC_MAKE_INSTANCE(bmp, wxStaticBitmap)

    bmp->Create(m_parentAsWindow,
                GetID(),
                GetBitmap(wxT("bitmap"), wxART_OTHER, GetSize()),
                GetPosition(), GetSize(),
                GetStyle(),
                GetName());

    SetupWindow(bmp);

    return bmp;
}

bool wxStaticBitmapXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxStaticBitmap"));
}

#endif // wxUSE_XRC && wxUSE_STATBMP
