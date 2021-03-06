/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/osx/radiobut.h
// Purpose:     wxRadioButton class
// Author:      Stefan Csomor
// Modified by:
// Created:     01/02/97
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_RADIOBUT_H_
#define _WX_RADIOBUT_H_

class WXDLLIMPEXP_CORE wxRadioButton: public wxControl
{
    wxDECLARE_DYNAMIC_CLASS(wxRadioButton);

public:
    inline wxRadioButton() {}
    inline wxRadioButton(wxWindow *parent, wxWindowID id,
           const wxString& label,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize, long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxRadioButtonNameStr)
    {
        Create(parent, id, label, pos, size, style, validator, name);
    }
    virtual ~wxRadioButton();

    bool Create(wxWindow *parent, wxWindowID id,
           const wxString& label,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize, long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxRadioButtonNameStr);

    virtual void SetValue(bool val);
    virtual bool GetValue() const ;

    // implementation

    void Command(wxCommandEvent& event);
    wxRadioButton *AddInCycle(wxRadioButton *cycle);
    void RemoveFromCycle();
    inline wxRadioButton *NextInCycle() {return m_cycle;}

    // osx specific event handling common for all osx-ports

    virtual bool        OSXHandleClicked( double timestampsec );
  protected:

    wxRadioButton *m_cycle;
};

// Not implemented
#if 0
class WXDLLIMPEXP_FWD_CORE wxBitmap ;

WXDLLIMPEXP_DATA_CORE(extern const wxChar) wxBitmapRadioButtonNameStr[];

class WXDLLIMPEXP_CORE wxBitmapRadioButton: public wxRadioButton
{
  wxDECLARE_DYNAMIC_CLASS(wxBitmapRadioButton);
protected:
  wxBitmap *theButtonBitmap;
public:
  inline wxBitmapRadioButton() { theButtonBitmap = NULL; }
  inline wxBitmapRadioButton(wxWindow *parent, wxWindowID id,
           const wxBitmap *label,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize, long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxBitmapRadioButtonNameStr)
  {
      Create(parent, id, label, pos, size, style, validator, name);
  }

  bool Create(wxWindow *parent, wxWindowID id,
           const wxBitmap *label,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize, long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxBitmapRadioButtonNameStr);

  virtual void SetLabel(const wxBitmap *label);
  virtual void SetValue(bool val) ;
  virtual bool GetValue() const ;
};
#endif

#endif
    // _WX_RADIOBUT_H_
