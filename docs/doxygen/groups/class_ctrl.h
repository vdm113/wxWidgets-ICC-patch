/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        class_ctrl.h
// Purpose:     Control classes group docs
// Author:      wxWidgets team
// RCS-ID:      $Id: class_dc.h 52454 2008-03-12 19:08:48Z BP $
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@defgroup group_class_ctrl Controls
@ingroup group_class

Typically, these are small windows which provide interaction with the user.
Controls that are not static can have wxValidator associated with them.

*/

