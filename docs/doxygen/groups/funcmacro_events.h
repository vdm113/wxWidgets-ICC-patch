/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        funcmacro_events.h
// Purpose:     Event functions and macro group doc
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@defgroup group_funcmacro_events Events
@ingroup group_funcmacro

Below are a number of functions/macros used with wxWidgets event-handling system.

Related class group: @ref group_class_events

*/

