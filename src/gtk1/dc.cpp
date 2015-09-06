/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/gtk1/dc.cpp
// Purpose:
// Author:      Robert Roebling
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/gtk1/dc.h"

#include <gdk/gdk.h>
#include <gtk/gtk.h>

//-----------------------------------------------------------------------------
// wxGTKDCImpl
//-----------------------------------------------------------------------------

wxIMPLEMENT_ABSTRACT_CLASS(wxGTKDCImpl, wxDCImpl);

wxGTKDCImpl::wxGTKDCImpl(wxDC *owner)
           : wxDCImpl(owner)
{
    m_ok = FALSE;

    m_logicalFunction = wxCOPY;

    m_pen = *wxBLACK_PEN;
    m_font = *wxNORMAL_FONT;
    m_brush = *wxWHITE_BRUSH;
}

void wxGTKDCImpl::DoSetClippingRegion( wxCoord x, wxCoord y, wxCoord width, wxCoord height )
{
    m_clipping = TRUE;
    m_clipX1 = x;
    m_clipY1 = y;
    m_clipX2 = x + width;
    m_clipY2 = y + height;
}

// ---------------------------------------------------------------------------
// get DC capabilities
// ---------------------------------------------------------------------------

void wxGTKDCImpl::DoGetSizeMM( int* width, int* height ) const
{
    int w = 0;
    int h = 0;
    GetSize( &w, &h );
    if (width) *width = int( double(w) / (m_userScaleX*m_mm_to_pix_x) );
    if (height) *height = int( double(h) / (m_userScaleY*m_mm_to_pix_y) );
}

// Resolution in pixels per logical inch
wxSize wxGTKDCImpl::GetPPI() const
{
    // TODO (should probably be pure virtual)
    return wxSize(0, 0);
}

