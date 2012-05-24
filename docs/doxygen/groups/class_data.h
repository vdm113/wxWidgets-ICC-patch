/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        class_data.h
// Purpose:     Data Structure classes group docs
// Author:      wxWidgets team
// RCS-ID:      $Id: class_dc.h 52454 2008-03-12 19:08:48Z BP $
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@defgroup group_class_data Data Structures
@ingroup group_class

These are the data structure classes provided by wxWidgets.
Some of them are used to store generic data (e.g. wxPoint, wxSize, etc),
others are mainly helpers of other classes (e.g. wxListItem, wxCalendarDateAttr,
wxFindReplaceDialogData, etc).

*/

