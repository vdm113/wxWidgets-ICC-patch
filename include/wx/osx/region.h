/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

#if wxOSX_USE_COCOA_OR_CARBON
#include "wx/osx/carbon/region.h"
#else
#define wxRegionGeneric wxRegion
#define wxRegionIteratorGeneric wxRegionIterator

#include "wx/generic/region.h"
#endif
