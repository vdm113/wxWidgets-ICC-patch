/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/unix/private.h
// Purpose:     miscellaneous private things for Unix wx ports
// Author:      Vadim Zeitlin
// Created:     2005-09-25
// Copyright:   (c) 2005 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIX_PRIVATE_H_
#define _WX_UNIX_PRIVATE_H_

// this file is currently empty as its original contents was moved to
// include/wx/private/fd.h but let's keep it for now in case we need it for
// something again in the future
#include "wx/private/fd.h"

#endif // _WX_UNIX_PRIVATE_H_

