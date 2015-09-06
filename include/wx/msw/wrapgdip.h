/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/wrapgdip.h
// Purpose:     wrapper around <gdiplus.h> header
// Author:      Vadim Zeitlin
// Created:     2007-03-15
// Copyright:   (c) 2007 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MSW_WRAPGDIP_H_
#define _WX_MSW_WRAPGDIP_H_

#include "wx/msw/wrapwin.h"

// these macros must be defined before gdiplus.h is included but we explicitly
// prevent windows.h from defining them in wx/msw/wrapwin.h as they conflict
// with standard functions of the same name elsewhere, so we have to pay for it
// by manually redefining them ourselves here
#ifndef max
    #define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
    #define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

// There are many clashes between the names of the member fields and parameters
// in the standard gdiplus.h header and each of them results in C4458 with
// VC14, so disable this warning for this file as there is no other way to
// avoid it.
#ifdef __VISUALC__
    #pragma warning(push)
    #pragma warning(disable:4458) // declaration of 'xxx' hides class member
#endif

#include <gdiplus.h>
using namespace Gdiplus;

#ifdef __VISUALC__
    #pragma warning(pop)
#endif

#endif // _WX_MSW_WRAPGDIP_H_

