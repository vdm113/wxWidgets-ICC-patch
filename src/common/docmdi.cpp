/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/docmdi.cpp
// Purpose:     Frame classes for MDI document/view applications
// Author:      Julian Smart, Vadim Zeitlin
// Created:     01/02/97
// Copyright:   (c) 1997 Julian Smart
//              (c) 2010 Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#if wxUSE_MDI_ARCHITECTURE

#include "wx/docmdi.h"

wxIMPLEMENT_CLASS(wxDocMDIParentFrame, wxMDIParentFrame);
wxIMPLEMENT_CLASS(wxDocMDIChildFrame, wxMDIChildFrame);

#endif // wxUSE_DOC_VIEW_ARCHITECTURE

