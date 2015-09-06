/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/richmsgdlgg.h
// Purpose:     wxGenericRichMessageDialog
// Author:      Rickard Westerlund
// Created:     2010-07-04
// Copyright:   (c) 2010 wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GENERIC_RICHMSGDLGG_H_
#define _WX_GENERIC_RICHMSGDLGG_H_

class WXDLLIMPEXP_FWD_CORE wxCheckBox;
class WXDLLIMPEXP_FWD_CORE wxCollapsiblePane;
class WXDLLIMPEXP_FWD_CORE wxCollapsiblePaneEvent;

class WXDLLIMPEXP_CORE wxGenericRichMessageDialog
                        : public wxRichMessageDialogBase
{
public:
    wxGenericRichMessageDialog(wxWindow *parent,
                               const wxString& message,
                               const wxString& caption = wxMessageBoxCaptionStr,
                               long style = wxOK | wxCENTRE)
        : wxRichMessageDialogBase( parent, message, caption, style ),
          m_checkBox(NULL),
          m_detailsPane(NULL)
    { }

    virtual bool IsCheckBoxChecked() const;

protected:
    wxCheckBox *m_checkBox;
    wxCollapsiblePane *m_detailsPane;

    // overrides methods in the base class
    virtual void AddMessageDialogCheckBox(wxSizer *sizer);
    virtual void AddMessageDialogDetails(wxSizer *sizer);

private:
    void OnPaneChanged(wxCollapsiblePaneEvent& event);

    wxDECLARE_EVENT_TABLE();

    wxDECLARE_NO_COPY_CLASS(wxGenericRichMessageDialog);
};

#endif // _WX_GENERIC_RICHMSGDLGG_H_
