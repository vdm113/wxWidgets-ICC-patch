/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
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

