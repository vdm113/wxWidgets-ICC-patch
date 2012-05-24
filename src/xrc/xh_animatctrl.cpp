/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/xrc/xh_animatctrl.cpp
// Purpose:     XML resource handler for wxAnimationCtrl
// Author:      Francesco Montorsi
// Created:     2006-10-15
// RCS-ID:      $Id$
// Copyright:   (c) 2006 Francesco Montorsi
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC && wxUSE_ANIMATIONCTRL

#include "wx/xrc/xh_animatctrl.h"
#include "wx/animate.h"

IMPLEMENT_DYNAMIC_CLASS(wxAnimationCtrlXmlHandler, wxXmlResourceHandler)

wxAnimationCtrlXmlHandler::wxAnimationCtrlXmlHandler() : wxXmlResourceHandler()
{
    XRC_ADD_STYLE(wxAC_NO_AUTORESIZE);
    XRC_ADD_STYLE(wxAC_DEFAULT_STYLE);
    AddWindowStyles();
}

wxObject *wxAnimationCtrlXmlHandler::DoCreateResource()
{
    XRC_MAKE_INSTANCE(ctrl, wxAnimationCtrl)

    ctrl->Create(m_parentAsWindow,
                  GetID(),
                  GetAnimation(wxT("animation")),
                  GetPosition(), GetSize(),
                  GetStyle(wxT("style"), wxAC_DEFAULT_STYLE),
                  GetName());

    // if no inactive-bitmap has been provided, GetBitmap() will return wxNullBitmap
    // which just tells wxAnimationCtrl to use the default for inactive status
    ctrl->SetInactiveBitmap(GetBitmap(wxT("inactive-bitmap")));

    SetupWindow(ctrl);

    return ctrl;
}

bool wxAnimationCtrlXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxAnimationCtrl"));
}

#endif // wxUSE_XRC && wxUSE_ANIMATIONCTRL
