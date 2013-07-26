/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/grid.h
// Purpose:     wxGrid base header
// Author:      Julian Smart
// Modified by:
// Created:
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GRID_H_BASE_
#define _WX_GRID_H_BASE_

#include "wx/generic/grid.h"

// these headers used to be included from the above header but isn't any more,
// still do it from here for compatibility
#include "wx/generic/grideditors.h"
#include "wx/generic/gridctrl.h"

#endif // _WX_GRID_H_BASE_
