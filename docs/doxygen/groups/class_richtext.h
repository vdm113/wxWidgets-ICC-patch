/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif defined(__INTEL_COMPILER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////

// Name:        class_richtext.h
// Purpose:     Rich Text classes group docs
// Author:      wxWidgets team
// RCS-ID:      $Id: class_dc.h 52454 2008-03-12 19:08:48Z BP $
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@defgroup group_class_richtext Rich Text
@ingroup group_class

wxWidgets provides a set of generic classes to edit and print simple rich text
with character and paragraph formatting.

*/

