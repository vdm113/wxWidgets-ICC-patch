/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/*
 * Name:        wx/motif/chkconf.h
 * Purpose:     Motif-specific config settings checks
 * Author:      Vadim Zeitlin
 * Modified by:
 * Created:     2005-04-05 (extracted from wx/chkconf.h)
 * RCS-ID:      $Id$
 * Copyright:   (c) 2005 Vadim Zeitlin <vadim@wxwidgets.org>
 * Licence:     wxWindows licence
 */

/* THIS IS A C FILE, DON'T USE C++ FEATURES (IN PARTICULAR COMMENTS) IN IT */

#if !defined(wxUSE_GADGETS)
#   define wxUSE_GADGETS 0
#endif
