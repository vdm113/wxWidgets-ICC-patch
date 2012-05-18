/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif defined(__INTEL_COMPILER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////

// Name:        funcmacro_string.h
// Purpose:     String function and macro group docs
// Author:      wxWidgets team
// RCS-ID:      $Id: funcmacro_gdi.h 52454 2008-03-12 19:08:48Z BP $
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@defgroup group_funcmacro_string Strings
@ingroup group_funcmacro

Global string functions and macros. See wxString for the wxWidgets string class.

Please note that all functions of this group which are documented to take @c char*
arrays are overloaded with @c wchar_t* variants.

Note also that wxWidgets wraps all standard CRT functions, even if the wrappers
are not (all) documented.

*/

