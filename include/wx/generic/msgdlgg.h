/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/msgdlgg.h
// Purpose:     Generic wxMessageDialog
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GENERIC_MSGDLGG_H_
#define _WX_GENERIC_MSGDLGG_H_

class WXDLLIMPEXP_FWD_CORE wxSizer;

class WXDLLIMPEXP_CORE wxGenericMessageDialog : public wxMessageDialogBase
{
public:
    wxGenericMessageDialog(wxWindow *parent,
                           const wxString& message,
                           const wxString& caption = wxMessageBoxCaptionStr,
                           long style = wxOK|wxCENTRE,
                           const wxPoint& pos = wxDefaultPosition);

    virtual int ShowModal() wxOVERRIDE;

protected:
    // Creates a message dialog taking any options that have been set after
    // object creation into account such as custom labels.
    void DoCreateMsgdialog();

    void OnYes(wxCommandEvent& event);
    void OnNo(wxCommandEvent& event);
    void OnHelp(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);

    // can be overridden to provide more contents to the dialog
    virtual void AddMessageDialogCheckBox(wxSizer *WXUNUSED(sizer)) { }
    virtual void AddMessageDialogDetails(wxSizer *WXUNUSED(sizer)) { }

private:
    // Creates and returns a standard button sizer using the style of this
    // dialog and the custom labels, if any.
    //
    // May return NULL on smart phone platforms not using buttons at all.
    wxSizer *CreateMsgDlgButtonSizer();

    wxPoint m_pos;
    bool m_created;

    wxDECLARE_EVENT_TABLE();
    wxDECLARE_DYNAMIC_CLASS(wxGenericMessageDialog);
};

#endif // _WX_GENERIC_MSGDLGG_H_
