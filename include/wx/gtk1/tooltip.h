/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk1/tooltip.h
// Purpose:     wxToolTip class
// Author:      Robert Roebling
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __GTKTOOLTIPH__
#define __GTKTOOLTIPH__

#include "wx/defs.h"
#include "wx/string.h"
#include "wx/object.h"

//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_FWD_CORE wxToolTip;
class WXDLLIMPEXP_FWD_CORE wxWindow;

//-----------------------------------------------------------------------------
// wxToolTip
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxToolTip : public wxObject
{
public:
    // globally change the tooltip parameters
    static void Enable( bool flag );
    static void SetDelay( long msecs );
        // set the delay after which the tooltip disappears or how long the tooltip remains visible
    static void SetAutoPop(long msecs);
        // set the delay between subsequent tooltips to appear
    static void SetReshow(long msecs);

    wxToolTip( const wxString &tip );

    // get/set the tooltip text
    void SetTip( const wxString &tip );
    wxString GetTip() const { return m_text; }

    wxWindow *GetWindow() const { return m_window; }
    bool IsOk() const { return m_window != NULL; }

    // implementation
    void Apply( wxWindow *win );

private:
    wxString     m_text;
    wxWindow    *m_window;

    wxDECLARE_ABSTRACT_CLASS(wxToolTip);
};

#endif // __GTKTOOLTIPH__
