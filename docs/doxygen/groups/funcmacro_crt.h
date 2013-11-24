/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        funcmacro_crt.h
// Purpose:     wx wrappers for CRT functions
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@defgroup group_funcmacro_crt Wrappers of CRT functions
@ingroup group_funcmacro

For documentation of these functions please refer to the documentation
of the standard CRT functions (see e.g. http://www.cppreference.com/wiki/c/start).

*/

