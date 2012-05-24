/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        class_rtti.h
// Purpose:     Runtime Type Information (RTTI) classes group docs
// Author:      wxWidgets team
// RCS-ID:      $Id: class_dc.h 52454 2008-03-12 19:08:48Z BP $
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@defgroup group_class_rtti Runtime Type Information (RTTI)
@ingroup group_class

wxWidgets supports runtime manipulation of class information, and dynamic
creation of objects given class names.

Related Overviews: @ref overview_rtti

Related macros/global-functions group: @ref group_funcmacro_rtti

*/

