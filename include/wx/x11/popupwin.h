/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/////////////////////////////////////////////////////////////////////////////
// Name:        wx/x11/popupwin.h
// Purpose:
// Author:      Robert Roebling
// Created:
// Copyright:   (c) 2001 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __GTKPOPUPWINH__
#define __GTKPOPUPWINH__

#include "wx/defs.h"
#include "wx/panel.h"
#include "wx/icon.h"

//-----------------------------------------------------------------------------
// wxPopUpWindow
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxPopupWindow: public wxPopupWindowBase
{
public:
    wxPopupWindow() { }
    virtual ~wxPopupWindow() ;

    wxPopupWindow(wxWindow *parent, int flags = wxBORDER_NONE)
        { (void)Create(parent, flags); }

    bool Create(wxWindow *parent, int flags = wxBORDER_NONE);

    virtual bool Show( bool show = TRUE );

protected:
    virtual void DoMoveWindow(int x, int y, int width, int height);
    virtual void DoSetSize(int x, int y,
                           int width, int height,
                           int sizeFlags = wxSIZE_AUTO);

private:
    wxDECLARE_EVENT_TABLE();
    wxDECLARE_DYNAMIC_CLASS(wxPopupWindow);
};

#endif // __GTKPOPUPWINDOWH__
