/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        funcmacro_gdi.h
// Purpose:     Graphics Device Interface function and macro docs
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@defgroup group_funcmacro_gdi Graphics Device Interface (GDI)
@ingroup group_funcmacro

The following are functions and macros related to GDI (Graphics Device
Interface) access.

Related Overviews: @ref overview_dc

Related class group: @ref group_class_gdi

*/

