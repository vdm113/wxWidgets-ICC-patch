/////////////////////////////////////////////////////////////////////////////
// Name:        stattext.h
// Purpose:     wxStaticText class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_STATTEXT_H_
#define _WX_STATTEXT_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "stattext.h"
#endif

class WXDLLEXPORT wxStaticText: public wxStaticTextBase
{
    DECLARE_DYNAMIC_CLASS(wxStaticText)
        
public:
    wxStaticText() { }
    
    wxStaticText(wxWindow *parent, wxWindowID id,
        const wxString& label,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxString& name = wxStaticTextNameStr)
    {
        Create(parent, id, label, pos, size, style, name);
    }
    
    bool Create(wxWindow *parent, wxWindowID id,
        const wxString& label,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxString& name = wxStaticTextNameStr);
    
    // implementation
    // --------------
    
    // operations
    virtual bool ProcessCommand(wxCommandEvent& WXUNUSED(event))
    {
        return false;
    }
    
    virtual void SetLabel(const wxString& label);
    
    // Get the widget that corresponds to the label
    // (for font setting, label setting etc.)
    virtual WXWidget GetLabelWidget() const
        { return m_labelWidget; }
    
protected:
    WXWidget              m_labelWidget;
};

#endif
// _WX_STATTEXT_H_
