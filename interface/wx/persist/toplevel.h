/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/persist/toplevel.h
// Purpose:     interface of wxPersistentTLW
// Author:      Vadim Zeitlin
// Copyright:   (c) 2009 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
    Persistence adapter for wxTopLevelWindow.

    This adapter saves and restores the geometry (i.e. position and size) and
    the state (iconized, maximized or normal) of top level windows. It can be
    used with both wxFrame and wxDialog.

    Note that it does @em not save nor restore the window visibility.
 */
class wxPersistentTLW : public wxPersistentWindow<wxTopLevelWindow>
{
public:
    /**
        Constructor.

        @param book
            The associated window.
     */
    wxPersistentTLW(wxTopLevelWindow *book);

    /**
        Save the current window geometry.
     */
    virtual void Save() const;

    /**
        Restore the window geometry.
     */
    virtual bool Restore();
};

/// Overload allowing persistence adapter creation for wxTopLevelWindow-derived
/// objects.
wxPersistentObject *wxCreatePersistentObject(wxTopLevelWindow *book);
