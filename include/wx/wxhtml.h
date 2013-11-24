/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/wxhtml.h
// Purpose:     wxHTML library for wxWidgets
// Author:      Vaclav Slavik
// Copyright:   (c) 1999 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_HTML_H_
#define _WX_HTML_H_

#include "wx/html/htmldefs.h"
#include "wx/html/htmltag.h"
#include "wx/html/htmlcell.h"
#include "wx/html/htmlpars.h"
#include "wx/html/htmlwin.h"
#include "wx/html/winpars.h"
#include "wx/filesys.h"
#include "wx/html/helpctrl.h"

#endif // __WXHTML_H__
