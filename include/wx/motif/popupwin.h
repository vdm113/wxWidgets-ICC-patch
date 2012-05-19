/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

///////////////////////////////////////////////////////////////////////////////


// Name:        wx/motif/popupwin.h
// Purpose:     wxPopupWindow class for wxMotif
// Author:      Mattia Barbon
// Modified by:
// Created:     28.08.03
// RCS-ID:      $Id$
// Copyright:   (c) 2003 Mattia Barbon
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MOTIF_POPUPWIN_H_
#define _WX_MOTIF_POPUPWIN_H_

// ----------------------------------------------------------------------------
// wxPopupWindow
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxPopupWindow : public wxPopupWindowBase
{
public:
    wxPopupWindow() { Init(); }

    wxPopupWindow( wxWindow *parent, int flags = wxBORDER_NONE )
        { Init(); (void)Create( parent, flags ); }

    bool Create( wxWindow *parent, int flags = wxBORDER_NONE );

    virtual bool Show( bool show = true );
private:
    void Init() { m_isShown = false; }

    DECLARE_DYNAMIC_CLASS_NO_COPY(wxPopupWindow)
};

#endif // _WX_MOTIF_POPUPWIN_H_
