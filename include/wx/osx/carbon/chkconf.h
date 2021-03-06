/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/*
 * Name:        wx/osx/carbon/chkconf.h
 * Purpose:     Compiler-specific configuration checking
 * Author:      Julian Smart
 * Modified by:
 * Created:     01/02/97
 * Copyright:   (c) Julian Smart
 * Licence:     wxWindows licence
 */

#ifndef _WX_OSX_CARBON_CHKCONF_H_
#define _WX_OSX_CARBON_CHKCONF_H_

/*
 * native (1) or emulated (0) toolbar
 * also support old notation wxMAC_USE_NATIVE_TOOLBAR
 */



#ifdef wxMAC_USE_NATIVE_TOOLBAR
    #define wxOSX_USE_NATIVE_TOOLBAR wxMAC_USE_NATIVE_TOOLBAR
#endif

#ifndef wxOSX_USE_NATIVE_TOOLBAR
    #define wxOSX_USE_NATIVE_TOOLBAR 1
#endif

/*
 * text rendering system
 */

#define wxOSX_USE_ATSU_TEXT 1

/*
 * Audio System
 */

#define wxOSX_USE_QUICKTIME 1
#define wxOSX_USE_AUDIOTOOLBOX 0

#endif
    /* _WX_OSX_CARBON_CHKCONF_H_ */

