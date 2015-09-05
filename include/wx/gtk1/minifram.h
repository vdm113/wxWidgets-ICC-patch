/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk1/minifram.h
// Purpose:     wxMiniFrame class
// Author:      Robert Roebling
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __GTKMINIFRAMEH__
#define __GTKMINIFRAMEH__

#include "wx/defs.h"

#if wxUSE_MINIFRAME

#include "wx/object.h"
#include "wx/frame.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_FWD_CORE wxMiniFrame;

//-----------------------------------------------------------------------------
// wxMiniFrame
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxMiniFrame: public wxFrame
{
    wxDECLARE_DYNAMIC_CLASS(wxMiniFrame);

public:
    wxMiniFrame() {}
    wxMiniFrame(wxWindow *parent,
            wxWindowID id,
            const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE | wxTINY_CAPTION,
            const wxString& name = wxFrameNameStr)
    {
        Create(parent, id, title, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
            wxWindowID id,
            const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE | wxTINY_CAPTION,
            const wxString& name = wxFrameNameStr);

    virtual void SetTitle( const wxString &title );
 // implementation

    bool   m_isDragging;
    int    m_oldX,m_oldY;
    int    m_diffX,m_diffY;
};

#endif

#endif
  //  __GTKMINIFRAMEH__
