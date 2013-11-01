/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

/*
 * Name:        wx/x11/chkconf.h
 * Purpose:     Compiler-specific configuration checking
 * Author:      Julian Smart
 * Modified by:
 * Created:     01/02/97
 * Copyright:   (c) Julian Smart
 * Licence:     wxWindows licence
 */

/* THIS IS A C FILE, DON'T USE C++ FEATURES (IN PARTICULAR COMMENTS) IN IT */

#ifndef _WX_X11_CHKCONF_H_
#define _WX_X11_CHKCONF_H_

/* wxPalette is always needed */
#if !wxUSE_PALETTE
#   error "wxX11 requires wxUSE_PALETTE=1"
#endif

#if wxUSE_SOCKETS && !wxUSE_SELECT_DISPATCHER
#   ifdef wxABORT_ON_CONFIG_ERROR
#       error "wxSocket requires wxSelectDispatcher in wxX11"
#   else
#       undef wxUSE_SELECT_DISPATCHER
#       define wxUSE_SELECT_DISPATCHER 1
#   endif
#endif

#endif /* _WX_X11_CHKCONF_H_ */
