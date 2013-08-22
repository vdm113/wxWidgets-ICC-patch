/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
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

