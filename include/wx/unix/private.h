/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
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

