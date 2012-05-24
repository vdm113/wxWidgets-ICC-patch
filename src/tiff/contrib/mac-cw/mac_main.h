/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/* 
 * mac_main.h  -- redefines main entry point
 */
 
#ifndef _mac_main_h
#define _mac_main_h

#undef main
#define main tool_main

#endif  /* _mac_main_h */

