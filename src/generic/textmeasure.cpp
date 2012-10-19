/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        src/generic/textmeasure.cpp
// Purpose:
// Author:      Vadim Zeitlin
// Created:     2012-10-17
// RCS-ID:      $Id$
// Copyright:   (c) 2012 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/window.h"
    #include "wx/dc.h"
#endif //WX_PRECOMP

#include "wx/private/textmeasure.h"

#if wxUSE_GENERIC_TEXTMEASURE

// ============================================================================
// wxTextMeasure generic implementation
// ============================================================================

// We assume that the ports not providing platform-specific wxTextMeasure
// implementation implement the corresponding functions in their wxDC and
// wxWindow classes, so forward back to them instead of using wxTextMeasure
// from there, as usual.
void wxTextMeasure::DoGetTextExtent(const wxString& string,
                                    wxCoord *width,
                                    wxCoord *height,
                                    wxCoord *descent,
                                    wxCoord *externalLeading)
{
    if ( m_dc )
    {
        m_dc->GetTextExtent(string, width, height,
                            descent, externalLeading, m_font);
    }
    else if ( m_win )
    {
        m_win->GetTextExtent(string, width, height,
                             descent, externalLeading, m_font);
    }
    //else: we already asserted in the ctor, don't do it any more
}

bool wxTextMeasure::DoGetPartialTextExtents(const wxString& text,
                                            wxArrayInt& widths,
                                            double scaleX)
{
    return wxTextMeasureBase::DoGetPartialTextExtents(text, widths, scaleX);
}

#endif // wxUSE_GENERIC_TEXTMEASURE
