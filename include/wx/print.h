/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/print.h
// Purpose:     Base header for printer classes
// Author:      Julian Smart
// Modified by:
// Created:
// Copyright:   (c) Julian Smart
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PRINT_H_BASE_
#define _WX_PRINT_H_BASE_

#include "wx/defs.h"

#if wxUSE_PRINTING_ARCHITECTURE

#if defined(__WXMSW__) && !defined(__WXUNIVERSAL__)

#include "wx/msw/printwin.h"

#elif defined(__WXMAC__)

#include "wx/osx/printmac.h"


#elif defined(__WXQT__)

#include "wx/qt/printqt.h"
#else

#include "wx/generic/printps.h"

#endif

#endif // wxUSE_PRINTING_ARCHITECTURE
#endif
    // _WX_PRINT_H_BASE_
