/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        funcmacro_appinitterm.h
// Purpose:     App Initialization and Termination fn and macro group docs
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@defgroup group_funcmacro_appinitterm Application Initialization and Termination
@ingroup group_funcmacro

The functions in this section are used on application startup/shutdown and also
to control the behaviour of the main event loop of the GUI programs.

Related macros/global-functions group: @ref group_class_appmanagement.

*/

