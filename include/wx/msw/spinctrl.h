/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif defined(__INTEL_COMPILER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

////////////////////////////////////////////////////////////////////////////

// Name:        wx/msw/spinctrl.h
// Purpose:     wxSpinCtrl class declaration for Win32
// Author:      Vadim Zeitlin
// Modified by:
// Created:     22.07.99
// RCS-ID:      $Id$
// Copyright:   (c) Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MSW_SPINCTRL_H_
#define _WX_MSW_SPINCTRL_H_

#include "wx/spinbutt.h"    // the base class

#if wxUSE_SPINCTRL

#include "wx/dynarray.h"

class WXDLLIMPEXP_FWD_CORE wxSpinCtrl;
WX_DEFINE_EXPORTED_ARRAY_PTR(wxSpinCtrl *, wxArraySpins);

// ----------------------------------------------------------------------------
// Under Win32, wxSpinCtrl is a wxSpinButton with a buddy (as MSDN docs call
// it) text window whose contents is automatically updated when the spin
// control is clicked.
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxSpinCtrl : public wxSpinButton
{
public:
    wxSpinCtrl() { Init(); }

    wxSpinCtrl(wxWindow *parent,
               wxWindowID id = wxID_ANY,
               const wxString& value = wxEmptyString,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxSP_ARROW_KEYS | wxALIGN_RIGHT,
               int min = 0, int max = 100, int initial = 0,
               const wxString& name = wxT("wxSpinCtrl"))
    {
        Init();

        Create(parent, id, value, pos, size, style, min, max, initial, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id = wxID_ANY,
                const wxString& value = wxEmptyString,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxSP_ARROW_KEYS | wxALIGN_RIGHT,
                int min = 0, int max = 100, int initial = 0,
                const wxString& name = wxT("wxSpinCtrl"));

    // a wxTextCtrl-like method (but we can't have GetValue returning wxString
    // because the base class already has one returning int!)
    void SetValue(const wxString& text);

    // another wxTextCtrl-like method
    void SetSelection(long from, long to);

    // implementation only from now on
    // -------------------------------

    virtual ~wxSpinCtrl();

    virtual void SetValue(int val);
    virtual int  GetValue() const;
    virtual void SetRange(int minVal, int maxVal);
    virtual bool SetFont(const wxFont &font);
    virtual void SetFocus();

    virtual bool Enable(bool enable = true);
    virtual bool Show(bool show = true);

    virtual bool Reparent(wxWindowBase *newParent);

    // wxSpinButton doesn't accept focus, but we do
    virtual bool AcceptsFocus() const { return wxWindow::AcceptsFocus(); }

    // we're like wxTextCtrl and not (default) wxButton
    virtual wxVisualAttributes GetDefaultAttributes() const
    {
        return GetClassDefaultAttributes(GetWindowVariant());
    }

    static wxVisualAttributes
    GetClassDefaultAttributes(wxWindowVariant variant = wxWINDOW_VARIANT_NORMAL)
    {
        return GetCompositeControlsDefaultAttributes(variant);
    }

    // for internal use only

    // get the subclassed window proc of the buddy text
    WXFARPROC GetBuddyWndProc() const { return m_wndProcBuddy; }

    // return the spinctrl object whose buddy is the given window or NULL
    static wxSpinCtrl *GetSpinForTextCtrl(WXHWND hwndBuddy);

    // process a WM_COMMAND generated by the buddy text control
    bool ProcessTextCommand(WXWORD cmd, WXWORD id);

    // recognize buddy window as part of this control at wx level
    virtual bool ContainsHWND(WXHWND hWnd) const { return hWnd == m_hwndBuddy; }

protected:
    virtual void DoGetPosition(int *x, int *y) const;
    virtual void DoMoveWindow(int x, int y, int width, int height);
    virtual wxSize DoGetBestSize() const;
    virtual void DoGetSize(int *width, int *height) const;
    virtual void DoGetClientSize(int *x, int *y) const;
#if wxUSE_TOOLTIPS
    virtual void DoSetToolTip( wxToolTip *tip );
#endif // wxUSE_TOOLTIPS

    virtual bool MSWOnNotify(int idCtrl, WXLPARAM lParam, WXLPARAM *result);
    virtual bool MSWOnScroll(int orientation, WXWORD wParam,
                             WXWORD pos, WXHWND control);

    // handle processing of special keys
    void OnChar(wxKeyEvent& event);
    void OnSetFocus(wxFocusEvent& event);
    void OnKillFocus(wxFocusEvent& event);

    // generate spin control update event with the given value
    void SendSpinUpdate(int value);

    // called to ensure that the value is in the correct range
    virtual void NormalizeValue();


    // the value of the control before the latest change (which might not have
    // changed anything in fact -- this is why we need this field)
    int m_oldValue;

    // the data for the "buddy" text ctrl
    WXHWND     m_hwndBuddy;
    WXFARPROC  m_wndProcBuddy;

    // Block text update event after SetValue()
    bool m_blockEvent;

private:
    // Common part of all ctors.
    void Init();

    DECLARE_DYNAMIC_CLASS(wxSpinCtrl)
    DECLARE_EVENT_TABLE()
    wxDECLARE_NO_COPY_CLASS(wxSpinCtrl);
};

#endif // wxUSE_SPINCTRL

#endif // _WX_MSW_SPINCTRL_H_


