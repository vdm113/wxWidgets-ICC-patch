/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/ptr_scpd.h
// Purpose:     compatibility wrapper for wxScoped{Ptr,Array}
// Author:      Vadim Zeitlin
// Created:     2009-02-03
// Copyright:   (c) 2009 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// do not include this file in any new code, include either wx/scopedptr.h or
// wx/scopedarray.h (or both) instead
#include "wx/scopedarray.h"
#include "wx/scopedptr.h"
