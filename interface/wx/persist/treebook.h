/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/persist/treebook.h
// Purpose:     interface of wxPersistentTreeBook
// Author:      Vadim Zeitlin
// Copyright:   (c) 2009 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
    Persistence adapter for wxTreebook.

    This adapter saves and restores the expanded branches of the wxTreeCtrl
    used by wxTreebook, in addition to saving and restoring the selection as
    implemented by the base wxPersistentBookCtrl class.
 */
class wxPersistentTreeBookCtrl : public wxPersistentBookCtrl
{
public:
    /**
        Constructor.

        @param book
            The associated tree book control.
     */
    wxPersistentTreeBookCtrl(wxTreebook *book);

    /**
        Save the currently opened branches.
     */
    virtual void Save() const;

    /**
        Restore the opened branches.

        The book control must be initialized before calling this function, i.e.
        all of its pages should be already added to it.
     */
    virtual bool Restore();
};

/// Overload allowing persistence adapter creation for wxTreebook objects.
wxPersistentObject *wxCreatePersistentObject(wxTreebook *book);
