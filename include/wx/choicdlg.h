/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/choicdlg.h
// Purpose:     Includes generic choice dialog file
// Author:      Julian Smart
// Modified by:
// Created:
// Copyright:   Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CHOICDLG_H_BASE_
#define _WX_CHOICDLG_H_BASE_

#include "wx/defs.h"

#if wxUSE_CHOICEDLG

#include "wx/generic/choicdgg.h"

#endif

#endif // _WX_CHOICDLG_H_BASE_
