/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

#define TIFFLIB_VERSION_STR "LIBTIFF, Version 4.0.3\nCopyright (c) 1988-1996 Sam Leffler\nCopyright (c) 1991-1996 Silicon Graphics, Inc."
/*
 * This define can be used in code that requires
 * compilation-related definitions specific to a
 * version or versions of the library.  Runtime
 * version checking should be done based on the
 * string returned by TIFFGetVersion.
 */
#define TIFFLIB_VERSION 20120922
