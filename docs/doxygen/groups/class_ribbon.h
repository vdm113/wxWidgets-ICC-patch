/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        class_ribbon.h
// Purpose:     Ribbon classes group docs
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@defgroup group_class_ribbon Ribbon User Interface
@ingroup group_class

The wxRibbon library is a set of classes for writing a ribbon user interface.
At the most generic level, this is a combination of a tab control with a
toolbar. At a more functional level, it is similar to the user interface
present in recent versions of Microsoft Office.

*/
