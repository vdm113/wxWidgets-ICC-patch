/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/radiobut.h
// Purpose:     wxRadioButton class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_RADIOBUT_H_
#define _WX_RADIOBUT_H_

class WXDLLIMPEXP_CORE wxRadioButton: public wxControl
{
public:
    // ctors and creation functions
    wxRadioButton() { Init(); }

    wxRadioButton(wxWindow *parent,
                  wxWindowID id,
                  const wxString& label,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = 0,
                  const wxValidator& validator = wxDefaultValidator,
                  const wxString& name = wxRadioButtonNameStr)
    {
        Init();

        Create(parent, id, label, pos, size, style, validator, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& label,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxRadioButtonNameStr);

    // implement the radio button interface
    virtual void SetValue(bool value);
    virtual bool GetValue() const;

    // implementation only from now on
    virtual bool MSWCommand(WXUINT param, WXWORD id);
    virtual void Command(wxCommandEvent& event);
    virtual bool HasTransparentBackground() { return true; }

    virtual WXDWORD MSWGetStyle(long style, WXDWORD *exstyle) const;

protected:
    virtual wxBorder GetDefaultBorder() const { return wxBORDER_NONE; }
    virtual wxSize DoGetBestSize() const;

private:
    // common part of all ctors
    void Init();

    // we need to store the state internally as the result of GetValue()
    // sometimes gets out of sync in WM_COMMAND handler
    bool m_isChecked;

    DECLARE_DYNAMIC_CLASS_NO_COPY(wxRadioButton)
};

#endif
    // _WX_RADIOBUT_H_
