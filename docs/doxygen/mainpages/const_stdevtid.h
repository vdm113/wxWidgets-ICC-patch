/////////////////////////////////////////////////////////////////////////////
// Name:        const_stdevtid.h
// Purpose:     std event values enumerated
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////


/**

 @page page_stdevtid Standard event identifiers

 wxWidgets defines a special identifier value @c wxID_ANY which is used in
 the following two situations:

 @li when creating a new window you may specify @c wxID_ANY to let
     wxWidgets assign an unused identifier to it automatically
 @li when installing an event handler using either the event table
     macros or wxEvtHandler::Connect,
     you may use it to indicate that you want to handle the events
     coming from any control, regardless of its identifier

 Another standard special identifier value is @c wxID_NONE: this is a value
 which is not matched by any other id.

 wxWidgets also defines a few standard command identifiers which may be used by
 the user code and also are sometimes used by wxWidgets itself. These reserved
 identifiers are all in the range between @c wxID_LOWEST and
 @c wxID_HIGHEST and, accordingly, the user code should avoid defining its
 own constants in this range.

 @verbatim
     wxID_LOWEST = 4999,

     wxID_OPEN,
     wxID_CLOSE,
     wxID_NEW,
     wxID_SAVE,
     wxID_SAVEAS,
     wxID_REVERT,
     wxID_EXIT,
     wxID_UNDO,
     wxID_REDO,
     wxID_HELP,
     wxID_PRINT,
     wxID_PRINT_SETUP,
     wxID_PAGE_SETUP,
     wxID_PREVIEW,
     wxID_ABOUT,
     wxID_HELP_CONTENTS,
     wxID_HELP_INDEX,
     wxID_HELP_SEARCH,
     wxID_HELP_COMMANDS,
     wxID_HELP_PROCEDURES,
     wxID_HELP_CONTEXT,
     wxID_CLOSE_ALL,

     wxID_EDIT = 5030,
     wxID_CUT,
     wxID_COPY,
     wxID_PASTE,
     wxID_CLEAR,
     wxID_FIND,
     wxID_DUPLICATE,
     wxID_SELECTALL,
     wxID_DELETE,
     wxID_REPLACE,
     wxID_REPLACE_ALL,
     wxID_PROPERTIES,

     wxID_VIEW_DETAILS,
     wxID_VIEW_LARGEICONS,
     wxID_VIEW_SMALLICONS,
     wxID_VIEW_LIST,
     wxID_VIEW_SORTDATE,
     wxID_VIEW_SORTNAME,
     wxID_VIEW_SORTSIZE,
     wxID_VIEW_SORTTYPE,

     wxID_FILE = 5050,
     wxID_FILE1,
     wxID_FILE2,
     wxID_FILE3,
     wxID_FILE4,
     wxID_FILE5,
     wxID_FILE6,
     wxID_FILE7,
     wxID_FILE8,
     wxID_FILE9,

     // Standard button IDs
     wxID_OK = 5100,
     wxID_CANCEL,
     wxID_APPLY,
     wxID_YES,
     wxID_NO,
     wxID_STATIC,
     wxID_FORWARD,
     wxID_BACKWARD,
     wxID_DEFAULT,
     wxID_MORE,
     wxID_SETUP,
     wxID_RESET,
     wxID_CONTEXT_HELP,
     wxID_YESTOALL,
     wxID_NOTOALL,
     wxID_ABORT,
     wxID_RETRY,
     wxID_IGNORE,

     wxID_UP,
     wxID_DOWN,
     wxID_HOME,
     wxID_REFRESH,
     wxID_STOP,
     wxID_INDEX,

     wxID_BOLD,
     wxID_ITALIC,
     wxID_JUSTIFY_CENTER,
     wxID_JUSTIFY_FILL,
     wxID_JUSTIFY_RIGHT,
     wxID_JUSTIFY_LEFT,
     wxID_UNDERLINE,
     wxID_INDENT,
     wxID_UNINDENT,
     wxID_ZOOM_100,
     wxID_ZOOM_FIT,
     wxID_ZOOM_IN,
     wxID_ZOOM_OUT,
     wxID_UNDELETE,
     wxID_REVERT_TO_SAVED,

     // System menu IDs (used by wxUniv):
     wxID_SYSTEM_MENU = 5200,
     wxID_CLOSE_FRAME,
     wxID_MOVE_FRAME,
     wxID_RESIZE_FRAME,
     wxID_MAXIMIZE_FRAME,
     wxID_ICONIZE_FRAME,
     wxID_RESTORE_FRAME,

     // IDs used by generic file dialog (13 consecutive starting from this value)
     wxID_FILEDLGG = 5900,

     wxID_HIGHEST = 5999
 @endverbatim

*/
