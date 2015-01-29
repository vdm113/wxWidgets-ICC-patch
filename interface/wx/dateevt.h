/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        dateevt.h
// Purpose:     interface of wxDateEvent
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxDateEvent

    This event class holds information about a date change and is used together
    with wxDatePickerCtrl. It also serves as a base class
    for wxCalendarEvent.

    @library{wxadv}
    @category{events}
*/
class wxDateEvent : public wxCommandEvent
{
public:
    wxDateEvent();
    wxDateEvent(wxWindow *win, const wxDateTime& dt, wxEventType type);

    /**
        Returns the date.
    */
    const wxDateTime& GetDate() const;

    /**
        Sets the date carried by the event, normally only used by the library
        internally.
    */
    void SetDate(const wxDateTime& date);
};

wxEventType wxEVT_DATE_CHANGED;
wxEventType wxEVT_TIME_CHANGED;
