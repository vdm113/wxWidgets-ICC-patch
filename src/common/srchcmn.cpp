/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/srchcmn.cpp
// Purpose:     common (to all ports) bits of wxSearchCtrl
// Author:      Robin Dunn
// Modified by:
// Created:     19-Dec-2006
// Copyright:   (c) wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_SEARCHCTRL

#include "wx/srchctrl.h"

#ifndef WX_PRECOMP
#endif

// ----------------------------------------------------------------------------

const char wxSearchCtrlNameStr[] = "searchCtrl";

wxDEFINE_EVENT(wxEVT_SEARCHCTRL_CANCEL_BTN, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_SEARCHCTRL_SEARCH_BTN, wxCommandEvent);


#endif // wxUSE_SEARCHCTRL
