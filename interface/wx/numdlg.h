/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif defined(__INTEL_COMPILER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////

// Name:        numdlg.h
// Purpose:     interface of global functions
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/** @addtogroup group_funcmacro_dialog */
//@{

/**
    Shows a dialog asking the user for numeric input. The dialogs title is set
    to @c caption, it contains a (possibly) multiline @c message above the
    single line @c prompt and the zone for entering the number.

    The number entered must be in the range @c min to @c max (both of which
    should be positive) and @c value is the initial value of it. If the user
    enters an invalid value, it is forced to fall into the specified range. If
    the user cancels the dialog, the function returns -1.

    Dialog is centered on its @c parent unless an explicit position is given
    in @c pos.

    @header{wx/numdlg.h}
*/
long wxGetNumberFromUser(const wxString& message,
                         const wxString& prompt,
                         const wxString& caption,
                         long value,
                         long min = 0,
                         long max = 100,
                         wxWindow* parent = NULL,
                         const wxPoint& pos = wxDefaultPosition);

//@}

