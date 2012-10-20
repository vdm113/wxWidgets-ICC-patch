/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/* VDM: BEGIN */
#if defined(_MSC_VER) && defined(MY_MSC_NO_ITERATOR_DEBUGGING) && MY_MSC_NO_ITERATOR_DEBUGGING==1
#   if defined(_SECURE_SCL)
#       undef _SECURE_SCL
#   endif
#   define _SECURE_SCL 0

#   if defined(_HAS_ITERATOR_DEBUGGING)
#       undef _HAS_ITERATOR_DEBUGGING
#   endif
#   define _HAS_ITERATOR_DEBUGGING 0
#endif
/* VDM: END   */

/*
 * File:        src/common/dummy.cpp
 * Purpose:     See below
 * Author:      Julian Smart
 * Created:     1993
 * Updated:
 * Copyright:   (c) 1993, AIAI, University of Edinburgh
 */

/* A dummy file to include wx.h. If precompiling wx.h,
 * always start by compiling this and producing the PCH file.
 * Then subsequent source files use the PCH file.
 *
 * If precompiling wx.h for wxWidgets and derived apps,
 * link dummy.obj with your program.
 *
 * This will produce a big PCH file.
 */

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifdef __WINDOWS__
    #include "wx/msw/msvcrt.h"
#endif

#ifdef __VISAGECPP__
    char wxDummyChar = 0;
#endif
