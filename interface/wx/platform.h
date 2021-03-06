/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        platform.h
// Purpose:     interface of global functions
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/** @addtogroup group_funcmacro_version */
//@{

/**
    Returns @true if the compiler being used is GNU C++ and its version is
    at least major.minor or greater. Returns @false otherwise.

    @header{wx/platform.h}
*/
#define wxCHECK_GCC_VERSION( major, minor )

/**
    Returns @true if the compiler being used is Sun CC Pro and its version is
    at least major.minor or greater. Returns @false otherwise.

    @header{wx/platform.h}
*/
#define wxCHECK_SUNCC_VERSION( major, minor )

/**
    Returns @true if the compiler being used is Visual C++ and its version is
    at least major or greater. Returns @false otherwise.

    @header{wx/platform.h}
*/
#define wxCHECK_VISUALC_VERSION( major )

/**
    Returns @true if the version of w32api headers used is major.minor or
    greater. Otherwise, and also if we are not compiling with MinGW32/Cygwin
    under Win32 at all, returns @false.

    @header{wx/platform.h}
*/
#define wxCHECK_W32API_VERSION( major, minor )

//@}

