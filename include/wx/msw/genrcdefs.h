/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/*
 * Name:        wx/msw/genrcdefs.h
 * Purpose:     Emit preprocessor symbols into rcdefs.h for resource compiler
 * Author:      Mike Wetherell
 * Copyright:   (c) 2005 Mike Wetherell
 * Licence:     wxWindows licence
 */

#define EMIT(line) line

EMIT(#ifndef _WX_RCDEFS_H)
EMIT(#define _WX_RCDEFS_H)

#ifdef _MSC_FULL_VER
#if _MSC_FULL_VER < 140040130
EMIT(#define wxUSE_RC_MANIFEST 1)
#endif
#else
EMIT(#define wxUSE_RC_MANIFEST 1)
#endif

#ifdef _M_AMD64 || defined __x86_64__
EMIT(#define WX_CPU_AMD64)
#endif

#ifdef _M_ARM
EMIT(#define WX_CPU_ARM)
#endif

#ifdef _M_IA64 || defined __ia64__
EMIT(#define WX_CPU_IA64)
#endif

#if defined _M_IX86 || defined _X86_
EMIT(#define WX_CPU_X86)
#endif

#ifdef _M_PPC
EMIT(#define WX_CPU_PPC)
#endif

#ifdef _M_SH
EMIT(#define WX_CPU_SH)
#endif

EMIT(#endif)
