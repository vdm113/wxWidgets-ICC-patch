/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/////////////////////////////////////////////////////////////////////////////
// Name:        tooltip.h
// Purpose:     interface of wxToolTip
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxToolTip

    This class holds information about a tooltip associated with a window (see
    wxWindow::SetToolTip()).

    The four static methods, wxToolTip::Enable(), wxToolTip::SetDelay()
    wxToolTip::SetAutoPop() and wxToolTip::SetReshow() can be used to globally
    alter tooltips behaviour.

    @library{wxcore}
    @category{help}
*/
class wxToolTip : public wxObject
{
public:
    /**
        Constructor.
    */
    wxToolTip(const wxString& tip);

    /**
        Enable or disable tooltips globally.

        @note May not be supported on all platforms (eg. wxCocoa).
    */
    static void Enable(bool flag);

    /**
        Get the tooltip text.
    */
    wxString GetTip() const;

    /**
        Get the associated window.
    */
    wxWindow* GetWindow() const;

    /**
        Set the delay after which the tooltip disappears or how long a tooltip
        remains visible.

        @note May not be supported on all platforms (eg. wxCocoa, GTK).
    */
    static void SetAutoPop(long msecs);

    /**
        Set the delay after which the tooltip appears.

        @note May not be supported on all platforms (eg. wxCocoa).
    */
    static void SetDelay(long msecs);

    /**
        Set tooltip maximal width in pixels.

        By default, tooltips are wrapped at a suitably chosen width. You can
        pass -1 as @a width to disable wrapping them completely, 0 to restore
        the default behaviour or an arbitrary positive value to wrap them at
        the given width.

        Notice that this function does not change the width of the tooltips
        created before calling it.

        @note Currently this function is wxMSW-only.
    */
    static void SetMaxWidth(int width);

    /**
        Set the delay between subsequent tooltips to appear.

        @note May not be supported on all platforms (eg. wxCocoa, GTK).
    */
    static void SetReshow(long msecs);

    /**
        Set the tooltip text.
    */
    void SetTip(const wxString& tip);
};

