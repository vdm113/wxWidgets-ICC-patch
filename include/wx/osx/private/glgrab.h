/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

#include <CoreFoundation/CoreFoundation.h>

#if defined __cplusplus
    extern "C" {
#endif

    CF_RETURNS_RETAINED CGImageRef grabViaOpenGL(CGDirectDisplayID display,
                             CGRect srcRect);

#if defined __cplusplus
    }
#endif

