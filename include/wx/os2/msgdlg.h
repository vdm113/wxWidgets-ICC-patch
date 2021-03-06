/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/os2/msgdlg.h
// Purpose:     wxMessageDialog class. Use generic version if no
//              platform-specific implementation.
// Author:      David Webster
// Modified by:
// Created:     10/12/99
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MSGBOXDLG_H_
#define _WX_MSGBOXDLG_H_

class WXDLLIMPEXP_CORE wxMessageDialog : public wxMessageDialogBase
{
public:
    wxMessageDialog( wxWindow*       pParent
                    ,const wxString& rsMessage
                    ,const wxString& rsCaption = wxMessageBoxCaptionStr
                    ,long            lStyle = wxOK|wxCENTRE
                    ,const wxPoint&  WXUNUSED(rPos) = wxDefaultPosition
                   )
        : wxMessageDialogBase(pParent, rsMessage, rsCaption, lStyle)
    {
    }

    int ShowModal(void);

protected:
    DECLARE_DYNAMIC_CLASS(wxMessageDialog)
}; // end of CLASS wxMessageDialog

#endif // _WX_MSGBOXDLG_H_
