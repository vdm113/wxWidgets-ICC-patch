/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/helphtml.h
// Purpose:     Includes wx/html/helpctrl.h, for wxHtmlHelpController.
// Author:      Julian Smart
// Modified by:
// Created:     2003-05-24
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_HELPHTML_H_
#define __WX_HELPHTML_H_

#if wxUSE_WXHTML_HELP
#include "wx/html/helpctrl.h"
#endif

#endif // __WX_HELPHTML_H_

