/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

// normally this is defined by the IDE by "Using MFC" option but if it isn't
// (e.g. because we use bakefile-generated projects), do it ourselves to ensure
// that MFC libraries are linked in
#ifndef _AFXDLL
    #define _AFXDLL
#endif
#include <afxwin.h>
