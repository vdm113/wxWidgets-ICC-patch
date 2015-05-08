/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        windowstyles.h
// Purpose:     topic overview
// Author:      wxWidgets team
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
