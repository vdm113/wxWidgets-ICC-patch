/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/univ/statbox.cpp
// Purpose:     wxStaticBox implementation
// Author:      Vadim Zeitlin
// Modified by:
// Created:     25.08.00
// Copyright:   (c) 2000 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_STATBOX

#ifndef WX_PRECOMP
    #include "wx/dc.h"
    #include "wx/statbox.h"
    #include "wx/validate.h"
#endif

#include "wx/univ/renderer.h"

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxStaticBox
// ----------------------------------------------------------------------------

bool wxStaticBox::Create(wxWindow *parent,
                         wxWindowID id,
                         const wxString &label,
                         const wxPoint &pos,
                         const wxSize &size,
                         long style,
                         const wxString &name)
{
    // FIXME refresh just the right/bottom parts affected in OnSize
    style |= wxFULL_REPAINT_ON_RESIZE;

    if ( !wxControl::Create(parent, id, pos, size, style, wxDefaultValidator, name) )
        return false;

    SetLabel(label);

    return true;
}

void wxStaticBox::DoDraw(wxControlRenderer *renderer)
{
    // we never have a border, so don't call the base class version which draws
    // it
    renderer->DrawFrame();
}

// ----------------------------------------------------------------------------
// geometry
// ----------------------------------------------------------------------------

wxRect wxStaticBox::GetBorderGeometry() const
{
    // FIXME should use the renderer here
    wxRect rect;
    rect.width =
    rect.x = GetCharWidth() / 2 + 1;
    rect.y = GetCharHeight() + 1;
    rect.height = rect.y / 2;

    return rect;
}

wxPoint wxStaticBox::GetBoxAreaOrigin() const
{
    wxPoint pt = wxControl::GetClientAreaOrigin();
    wxRect rect = GetBorderGeometry();
    pt.x += rect.x;
    pt.y += rect.y;

    return pt;
}

#if 0
void wxStaticBox::DoSetClientSize(int width, int height)
{
    wxRect rect = GetBorderGeometry();

    wxControl::DoSetClientSize(width + rect.x + rect.width,
                               height + rect.y + rect.height);
}

void wxStaticBox::DoGetClientSize(int *width, int *height) const
{
    wxControl::DoGetClientSize(width, height);

    wxRect rect = GetBorderGeometry();
    if ( width )
        *width -= rect.x + rect.width;
    if ( height )
        *height -= rect.y + rect.height;
}

#endif // 0

#endif // wxUSE_STATBOX

