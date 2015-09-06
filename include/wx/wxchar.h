/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

//////////////////////////////////////////////////////////////////////////////
// Name:        wx/wxchar.h
// Purpose:     Declarations common to wx char/wchar_t usage (wide chars)
// Author:      Joel Farley, Ove Kåven
// Modified by: Vadim Zeitlin, Robert Roebling, Ron Lee
// Created:     1998/06/12
// Copyright:   (c) 1998-2006 wxWidgets dev team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WXCHAR_H_
#define _WX_WXCHAR_H_

// This header used to define CRT functions wrappers in wxWidgets 2.8. This is
// now done in (headers included by) wx/crt.h, so include it for compatibility:
#include "wx/crt.h"

#endif /* _WX_WXCHAR_H_ */
