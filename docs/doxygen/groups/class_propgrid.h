/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        class_propgrid.h
// Purpose:     wxPropertyGrid classes group docs
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@defgroup group_class_propgrid wxPropertyGrid
@ingroup group_class

wxPropertyGrid is a specialized grid for editing properties (that is,
name=value pairs). This style of control has also been known as
property sheet or object grid.

Related Overviews: @ref overview_propgrid

*/
