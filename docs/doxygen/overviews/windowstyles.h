/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        windowstyles.h
// Purpose:     topic overview
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@page overview_windowstyles Window Styles

@tableofcontents

Window styles are used to specify alternative behaviour and appearances for
windows, when they are created. The symbols are defined in such a way that they
can be combined in a 'bit-list' using the C++ @e bitwise-or operator.

For example:

@code
wxCAPTION | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER
@endcode

For the window styles specific to each window class, please see the
documentation for the window.

Most windows can use the generic styles listed for wxWindow in addition to
their own styles.

*/
