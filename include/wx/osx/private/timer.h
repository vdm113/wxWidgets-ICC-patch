/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

/* common warning snippet for all osx direct includes */

#ifndef wxOSX_USE_CARBON
#error "this files should only be included after platform.h was included"
#endif

#if 1 // revert to wxOSX_USE_COCOA_OR_IPHONE in case of problems
    #include "wx/osx/core/private/timer.h"
#elif wxOSX_USE_CARBON
    #include "wx/osx/carbon/private/timer.h"
#endif
