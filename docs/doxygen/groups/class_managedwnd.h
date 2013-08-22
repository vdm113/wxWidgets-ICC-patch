/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        class_managedwnd.h
// Purpose:     Managed Windows classes group docs
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@defgroup group_class_managedwnd Managed Windows
@ingroup group_class

There are several types of window that are directly controlled by the window
manager (such as MS Windows, or the Motif Window Manager). Frames and dialogs
are similar in wxWidgets, but only dialogs may be modal.

Related Overviews: @ref overview_cmndlg

Related macros/global-functions group: @ref group_funcmacro_dialog

*/

