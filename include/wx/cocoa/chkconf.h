/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/*
 * Name:        wx/cocoa/chkconf.h
 * Purpose:     wxCocoa-specific config settings checks
 * Author:      Vadim Zeitlin
 * Created:     2008-09-11
 * Copyright:   (c) 2008 Vadim Zeitlin <vadim@wxwidgets.org>
 * Licence:     wxWindows licence
 */

/* THIS IS A C FILE, DON'T USE C++ FEATURES (IN PARTICULAR COMMENTS) IN IT */

#ifndef _WX_COCOA_CHKCONF_H_
#define _WX_COCOA_CHKCONF_H_

/*
    wxLogDialog doesn't currently work correctly in wxCocoa.
 */
#undef wxUSE_LOG_DIALOG
#define wxUSE_LOG_DIALOG 0

#endif /* _WX_COCOA_CHKCONF_H_ */
