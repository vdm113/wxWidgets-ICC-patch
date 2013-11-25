/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        funcmacro_log.h
// Purpose:     Logging function and macro group docs
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@defgroup group_funcmacro_log Logging
@ingroup group_funcmacro

These functions provide a variety of logging functions. The functions use
(implicitly) the currently active log target, so their descriptions here may
not apply if the log target is not the standard one (installed by wxWidgets in
the beginning of the program).

Related Overviews: @ref overview_log

Related class group: @ref group_class_logging

*/

