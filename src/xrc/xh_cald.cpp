/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/xrc/xh_cald.cpp
// Purpose:     XRC resource for wxCalendarCtrl
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

#if wxUSE_XRC && wxUSE_CALENDARCTRL

#include "wx/xrc/xh_cald.h"

#ifndef WX_PRECOMP
    #include "wx/event.h"
#endif //WX_PRECOMP

#include "wx/calctrl.h"

wxIMPLEMENT_DYNAMIC_CLASS(wxCalendarCtrlXmlHandler, wxXmlResourceHandler);

wxCalendarCtrlXmlHandler::wxCalendarCtrlXmlHandler()
: wxXmlResourceHandler()
{
    XRC_ADD_STYLE(wxCAL_SUNDAY_FIRST);
    XRC_ADD_STYLE(wxCAL_MONDAY_FIRST);
    XRC_ADD_STYLE(wxCAL_SHOW_HOLIDAYS);
    XRC_ADD_STYLE(wxCAL_NO_YEAR_CHANGE);
    XRC_ADD_STYLE(wxCAL_NO_MONTH_CHANGE);
    XRC_ADD_STYLE(wxCAL_SEQUENTIAL_MONTH_SELECTION);
    XRC_ADD_STYLE(wxCAL_SHOW_SURROUNDING_WEEKS);

    AddWindowStyles();
}


wxObject *wxCalendarCtrlXmlHandler::DoCreateResource()
{
    XRC_MAKE_INSTANCE(calendar, wxCalendarCtrl);

    calendar->Create(m_parentAsWindow,
                     GetID(),
                     wxDefaultDateTime,
                     /*TODO: take it from resource*/
                     GetPosition(), GetSize(),
                     GetStyle(),
                     GetName());

    SetupWindow(calendar);

    return calendar;
}

bool wxCalendarCtrlXmlHandler::CanHandle(wxXmlNode *node)
{
    return IsOfClass(node, wxT("wxCalendarCtrl"));
}

#endif // wxUSE_XRC && wxUSE_CALENDARCTRL
