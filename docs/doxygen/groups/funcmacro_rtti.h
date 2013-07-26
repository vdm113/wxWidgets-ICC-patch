/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        funcmacro_rtti.h
// Purpose:     Runtime Type Information function and macro group docs
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@defgroup group_funcmacro_rtti Runtime Type Information (RTTI)
@ingroup group_funcmacro

wxWidgets uses its own RTTI ("run-time type identification") system which
predates the current standard C++ RTTI and so is kept for backwards
compatibility reasons but also because it allows some things which the standard
RTTI doesn't directly support (such as creating a class from its name). The
standard C++ RTTI can be used in the user code without any problems and in
general you shouldn't need to use the functions and the macros in this section
unless you are thinking of modifying or adding any wxWidgets classes.

Related Overviews: @ref overview_rtti

Related class group: @ref group_class_rtti

*/

