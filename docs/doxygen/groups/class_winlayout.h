/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        class_winlayout.h
// Purpose:     Window Layout classes group docs
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@defgroup group_class_winlayout Window Layout
@ingroup group_class

wxWidgets makes window layout and sizing easy and painless using a set of
classes known as "sizers". Sizers allow for flexible window positioning and
sizes that can help with automatically handling localization differences, as
well as making it easy to write user resizable windows.

Related Overviews: @ref overview_sizer

*/

