/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif defined(__INTEL_COMPILER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////

// Name:        wx/xpmhand.h
// Purpose:     XPM handler base header
// Author:      Julian Smart
// Modified by:
// Created:
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XPMHAND_H_BASE_
#define _WX_XPMHAND_H_BASE_

// Only wxMSW and wxPM currently defines a separate XPM handler, since
// mostly Windows and Presentation Manager apps won't need XPMs.
#if defined(__WXMSW__)
#error xpmhand.h is no longer needed since wxImage now handles XPMs.
#endif
#if defined(__WXPM__)
#include "wx/os2/xpmhand.h"
#endif

#endif
    // _WX_XPMHAND_H_BASE_
