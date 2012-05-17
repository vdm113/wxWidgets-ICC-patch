/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif defined(__INTEL_COMPILER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

///////////////////////////////////////////////////////////////////////////////

// Name:        src/ribbon/control.cpp
// Purpose:     Extension of wxControl with common ribbon methods
// Author:      Peter Cawley
// Modified by:
// Created:     2009-06-05
// RCS-ID:      $Id$
// Copyright:   (C) Peter Cawley
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_RIBBON

#include "wx/ribbon/control.h"

#ifndef WX_PRECOMP
#endif

#ifdef __WXMSW__
#include "wx/msw/private.h"
#endif

IMPLEMENT_CLASS(wxRibbonControl, wxControl)

bool wxRibbonControl::Create(wxWindow *parent, wxWindowID id,
                    const wxPoint& pos,
                    const wxSize& size, long style,
                    const wxValidator& validator,
                    const wxString& name)
{
    if ( !wxControl::Create(parent, id, pos, size, style, validator, name) )
        return false;

    wxRibbonControl *ribbon_parent = wxDynamicCast(parent, wxRibbonControl);
    if(ribbon_parent)
    {
        m_art = ribbon_parent->GetArtProvider();
    }

    return true;
}

void wxRibbonControl::SetArtProvider(wxRibbonArtProvider* art)
{
    m_art = art;
}

wxSize wxRibbonControl::DoGetNextSmallerSize(wxOrientation direction,
                                           wxSize size) const
{
    // Dummy implementation for code which doesn't check for IsSizingContinuous() == true
    wxSize minimum(GetMinSize());
    if((direction & wxHORIZONTAL) && size.x > minimum.x)
    {
        size.x--;
    }
    if((direction & wxVERTICAL) && size.y > minimum.y)
    {
        size.y--;
    }
    return size;
}

wxSize wxRibbonControl::DoGetNextLargerSize(wxOrientation direction,
                                          wxSize size) const
{
    // Dummy implementation for code which doesn't check for IsSizingContinuous() == true
    if(direction & wxHORIZONTAL)
    {
        size.x++;
    }
    if(direction & wxVERTICAL)
    {
        size.y++;
    }
    return size;
}

wxSize wxRibbonControl::GetNextSmallerSize(wxOrientation direction,
                                           wxSize relative_to) const
{
    return DoGetNextSmallerSize(direction, relative_to);
}

wxSize wxRibbonControl::GetNextLargerSize(wxOrientation direction,
                                          wxSize relative_to) const
{
    return DoGetNextLargerSize(direction, relative_to);
}

wxSize wxRibbonControl::GetNextSmallerSize(wxOrientation direction) const
{
    return DoGetNextSmallerSize(direction, GetSize());
}

wxSize wxRibbonControl::GetNextLargerSize(wxOrientation direction) const
{
    return DoGetNextLargerSize(direction, GetSize());
}

bool wxRibbonControl::Realize()
{
    return true;
}

#endif // wxUSE_RIBBON
