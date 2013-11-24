/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        class_aui.h
// Purpose:     Window Docking (wxAUI) classes group docs
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@defgroup group_class_aui Window Docking (wxAUI)
@ingroup group_class

wxAUI is a set classes for writing a customizable application interface
with built-in docking, floatable panes and a flexible MDI-like interface.

Related Overviews: @ref overview_aui

*/

