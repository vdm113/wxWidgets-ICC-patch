/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        class_docview.h
// Purpose:     Document/View Framework classes group docs
// Author:      wxWidgets team
// RCS-ID:      $Id: class_dc.h 52454 2008-03-12 19:08:48Z BP $
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@defgroup group_class_docview Document/View Framework
@ingroup group_class

wxWidgets supports a document/view framework which provides housekeeping for a
document-centric application.

Related Overviews: @ref overview_docview

*/

