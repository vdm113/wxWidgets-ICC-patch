/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
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
