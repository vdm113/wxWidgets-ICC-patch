/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk1/spinbutt.h
// Purpose:     wxSpinButton class
// Author:      Robert Roebling
// Modified by:
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GTK_SPINBUTT_H_
#define _WX_GTK_SPINBUTT_H_

//-----------------------------------------------------------------------------
// wxSpinButton
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxSpinButton : public wxSpinButtonBase
{
public:
    wxSpinButton() { }
    wxSpinButton(wxWindow *parent,
                 wxWindowID id = -1,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxSP_VERTICAL,
                 const wxString& name = wxSPIN_BUTTON_NAME)
    {
        Create(parent, id, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id = -1,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxSP_VERTICAL,
                const wxString& name = wxSPIN_BUTTON_NAME);

    virtual int GetValue() const;
    virtual void SetValue( int value );
    virtual void SetRange( int minVal, int maxVal );
    virtual int GetMin() const;
    virtual int GetMax() const;

    static wxVisualAttributes
    GetClassDefaultAttributes(wxWindowVariant variant = wxWINDOW_VARIANT_NORMAL);

    // implementation
    void OnSize( wxSizeEvent &event );

    bool IsOwnGtkWindow( GdkWindow *window );

    GtkAdjustment  *m_adjust;
    float           m_oldPos;

protected:
    virtual wxSize DoGetBestSize() const;

private:
    wxDECLARE_EVENT_TABLE();
    wxDECLARE_DYNAMIC_CLASS(wxSpinButton);
};

#endif
    // _WX_GTK_SPINBUTT_H_
