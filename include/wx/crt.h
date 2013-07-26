/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

//////////////////////////////////////////////////////////////////////////////
// Name:        wx/crt.h
// Purpose:     Header to include all headers with wrappers for CRT functions
// Author:      Robert Roebling
// Created:     2007-05-30
// Copyright:   (c) 2007 wxWidgets dev team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CRT_H_
#define _WX_CRT_H_

#include "wx/defs.h"

// include wxChar type definition:
#include "wx/chartype.h"

// and wrappers for CRT functions:
#include "wx/wxcrt.h"
#include "wx/wxcrtvararg.h"

#endif // _WX_CRT_H_
