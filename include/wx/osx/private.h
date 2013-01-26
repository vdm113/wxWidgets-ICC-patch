/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

#ifndef _WX_PRIVATE_OSX_H_
#define _WX_PRIVATE_OSX_H_

#include "wx/osx/core/private.h"

#if wxOSX_USE_IPHONE
    #include "wx/osx/iphone/private.h"
#elif wxOSX_USE_CARBON
    #include "wx/osx/carbon/private.h"
#elif wxOSX_USE_COCOA
    #include "wx/osx/cocoa/private.h"
#elif wxUSE_GUI
    #error "Must include wx/defs.h first"
#endif

#endif
