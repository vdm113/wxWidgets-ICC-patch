/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
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

#if wxUSE_CHOICEDLG

#include "wx/generic/choicdgg.h"

#endif

#endif
    // _WX_CHOICDLG_H_BASE_
