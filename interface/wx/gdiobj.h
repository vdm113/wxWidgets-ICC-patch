/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        gdiobj.h
// Purpose:     interface of wxGDIObject
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxGDIObject

    This class allows platforms to implement functionality to optimise GDI
    objects, such as wxPen, wxBrush and wxFont. On Windows, the underling GDI
    objects are a scarce resource and are cleaned up when a usage count goes to
    zero. On some platforms this class may not have any special functionality.

    Since the functionality of this class is platform-specific, it is not
    documented here in detail.

    @library{wxcore}
    @category{gdi}

    @see wxPen, wxBrush, wxFont
*/
class wxGDIObject : public wxObject
{
public:
    /**
        Default constructor.
    */
    wxGDIObject();
};
