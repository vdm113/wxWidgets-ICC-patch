/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

#define VERSION \
"LIBTIFF, Version 3.4beta028 \n"\
"Copyright (c) 1988-1995 Sam Leffler\n"\
"Copyright (c) 1991-1996 Silicon Graphics, Inc."
