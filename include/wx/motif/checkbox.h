/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/motif/checkbox.h
// Purpose:     wxCheckBox class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CHECKBOX_H_
#define _WX_CHECKBOX_H_

// Checkbox item (single checkbox)
class WXDLLIMPEXP_CORE wxCheckBox: public wxCheckBoxBase
{
    wxDECLARE_DYNAMIC_CLASS(wxCheckBox);

public:
    inline wxCheckBox() { Init(); }
    inline wxCheckBox(wxWindow *parent, wxWindowID id, const wxString& label,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxCheckBoxNameStr)
    {
        Init();

        Create(parent, id, label, pos, size, style, validator, name);
    }

    bool Create(wxWindow *parent, wxWindowID id, const wxString& label,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxCheckBoxNameStr);
    virtual void SetValue(bool);
    virtual bool GetValue() const ;
    virtual void Command(wxCommandEvent& event);

    // Implementation
    virtual void ChangeBackgroundColour();
private:
    // common part of all constructors
    void Init()
    {
        m_evtType = wxEVT_CHECKBOX;
    }

    virtual void DoSet3StateValue(wxCheckBoxState state);

    virtual wxCheckBoxState DoGet3StateValue() const;

    // public for the callback
public:
    // either wxEVT_CHECKBOX or ..._TOGGLEBUTTON
    wxEventType m_evtType;
};

#endif
// _WX_CHECKBOX_H_
