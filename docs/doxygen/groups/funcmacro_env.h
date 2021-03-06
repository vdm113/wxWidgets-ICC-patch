/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        funcmacro_env.h
// Purpose:     Environment function and macro group docs
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@defgroup group_funcmacro_env Environment
@ingroup group_funcmacro

These functions allow access to get or change the values of environment
variables in a portable way. They are currently implemented under Win32 and
POSIX-like systems (Unix).

Remember that these functions add/change/delete environment variables of the
current process only. Child processes copy the environment variables of the parent
but do not share them (a wxSetEnv() in the parent process won't change the value
returned by wxGetEnv() in the child process and viceversa).

For more evoluted interprocess communication tecniques see @ref overview_ipc.

*/

