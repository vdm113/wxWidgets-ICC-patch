/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/osx/control.h
// Purpose:     wxControl class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CONTROL_H_
#define _WX_CONTROL_H_

WXDLLIMPEXP_DATA_CORE(extern const char) wxControlNameStr[];

// General item class
class WXDLLIMPEXP_CORE wxControl : public wxControlBase
{
    wxDECLARE_ABSTRACT_CLASS(wxControl);

public:
   wxControl();
   wxControl(wxWindow *parent, wxWindowID winid,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize, long style = 0,
             const wxValidator& validator = wxDefaultValidator,
             const wxString& name = wxControlNameStr)
    {
        Create(parent, winid, pos, size, style, validator, name);
    }

    bool Create(wxWindow *parent, wxWindowID winid,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize, long style = 0,
            const wxValidator& validator = wxDefaultValidator,
            const wxString& name = wxControlNameStr);

   // Simulates an event
   virtual void Command(wxCommandEvent& event) { ProcessCommand(event); }

   // implementation from now on
   // --------------------------

   // Calls the callback and appropriate event handlers
   bool ProcessCommand(wxCommandEvent& event);

   void                 OnKeyDown( wxKeyEvent &event ) ;
};

#endif
    // _WX_CONTROL_H_
