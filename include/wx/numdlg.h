/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/numdlg.h
// Purpose:     wxNumberEntryDialog class
// Author:      John Labenski
// Modified by:
// Created:     07.02.04 (extracted from wx/textdlg.h)
// Copyright:   (c) John Labenski
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_NUMDLGDLG_H_BASE_
#define _WX_NUMDLGDLG_H_BASE_

#include "wx/defs.h"

#if wxUSE_NUMBERDLG

#include "wx/generic/numdlgg.h"

#endif // wxUSE_NUMBERDLG

#endif // _WX_NUMDLGDLG_H_BASE_
