/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

// Scintilla source code edit control
/** @file FontQuality.h
 ** Definitions to control font anti-aliasing.
 **/
// Copyright 1998-2009 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#define SC_EFF_QUALITY_MASK            0xF
#define SC_EFF_QUALITY_DEFAULT           0
#define SC_EFF_QUALITY_NON_ANTIALIASED   1
#define SC_EFF_QUALITY_ANTIALIASED       2
#define SC_EFF_QUALITY_LCD_OPTIMIZED     3

#define SCWIN_TECH_GDI 0
#define SCWIN_TECH_DIRECTWRITE 1
