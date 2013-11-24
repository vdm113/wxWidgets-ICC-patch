/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/helpwin.h
// Purpose:     Includes Windows or OS/2 help
// Author:      Julian Smart
// Modified by:
// Created:
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_HELPWIN_H_BASE_
#define _WX_HELPWIN_H_BASE_

#if defined(__WXMSW__)
#include "wx/msw/helpwin.h"
#elif defined(__WXPM__)
#include "wx/os2/helpwin.h"
#endif

#endif
    // _WX_HELPWIN_H_BASE_
