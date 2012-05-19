/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/* Copyright (c) 1998, 1999 Thai Open Source Software Center Ltd


   See the file COPYING for copying permission.
*/

#include <stddef.h>

#ifdef XML_UNICODE
int filemap(const wchar_t *name,
            void (*processor)(const void *, size_t,
                              const wchar_t *, void *arg),
            void *arg);
#else
int filemap(const char *name,
            void (*processor)(const void *, size_t,
                              const char *, void *arg),
            void *arg);
#endif
