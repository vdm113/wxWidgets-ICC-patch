/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/help.h
// Purpose:     wxHelpController base header
// Author:      wxWidgets Team
// Modified by:
// Created:
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_HELP_H_BASE_
#define _WX_HELP_H_BASE_

#include "wx/defs.h"

#if wxUSE_HELP

#include "wx/helpbase.h"

#ifdef __WXWINCE__
    #include "wx/msw/wince/helpwce.h"

    #define wxHelpController wxWinceHelpController
#elif defined(__WXMSW__)
    #include "wx/msw/helpchm.h"

    #define wxHelpController wxCHMHelpController
#else // !MSW

#if wxUSE_WXHTML_HELP
    #include "wx/html/helpctrl.h"
    #define wxHelpController wxHtmlHelpController
#else
    #include "wx/generic/helpext.h"
    #define wxHelpController wxExtHelpController
#endif

#endif // MSW/!MSW

#endif // wxUSE_HELP

#endif
    // _WX_HELP_H_BASE_
