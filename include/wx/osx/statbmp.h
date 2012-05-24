/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

#if wxOSX_USE_CARBON
#include "wx/osx/carbon/statbmp.h"
#else
#define wxGenericStaticBitmap wxStaticBitmap
#include "wx/generic/statbmpg.h"
#endif
