/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        src/univ/inphand.cpp
// Purpose:     (trivial) wxInputHandler implementation
// Author:      Vadim Zeitlin
// Modified by:
// Created:     18.08.00
// Copyright:   (c) 2000 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
#endif // WX_PRECOMP

#include "wx/univ/inphand.h"

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxInputHandler
// ----------------------------------------------------------------------------

bool wxInputHandler::HandleMouseMove(wxInputConsumer * WXUNUSED(consumer),
                                     const wxMouseEvent& WXUNUSED(event))
{
    return false;
}

bool wxInputHandler::HandleFocus(wxInputConsumer *WXUNUSED(consumer),
                                 const wxFocusEvent& WXUNUSED(event))
{
    return false;
}

bool wxInputHandler::HandleActivation(wxInputConsumer *WXUNUSED(consumer),
                                      bool WXUNUSED(activated))
{
    return false;
}

wxInputHandler::~wxInputHandler()
{
}

