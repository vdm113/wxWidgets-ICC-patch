/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/*
 * wx/setup.h
 *
 * This file should not normally be used, except where makefiles
 * have not yet been adjusted to take into account of the new scheme
 * whereby a setup.h is created under the lib directory.
 *
 * Copyright:   (c) Vadim Zeitlin
 * Licence:     wxWindows Licence
 */

#ifdef __WXMSW__
#include "wx/msw/setup.h"
#else
#error Please adjust your include path to pick up the wx/setup.h file under lib first.
#endif

