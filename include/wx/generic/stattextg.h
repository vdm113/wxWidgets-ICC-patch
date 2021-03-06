/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/stattextg.h
// Purpose:     wxGenericStaticText header
// Author:      Marcin Wojdyr
// Created:     2008-06-26
// Copyright:   Marcin Wojdyr
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GENERIC_STATTEXTG_H_
#define _WX_GENERIC_STATTEXTG_H_

// prevent it from including the platform-specific wxStaticText declaration as
// this is not going to compile if it derives from wxGenericStaticText defined
// below (currently this is only the case in wxUniv but it could also happen
// with other ports)
#define wxNO_PORT_STATTEXT_INCLUDE
#include "wx/stattext.h"
#undef wxNO_PORT_STATTEXT_INCLUDE

class WXDLLIMPEXP_CORE wxGenericStaticText : public wxStaticTextBase
{
public:
    wxGenericStaticText() { Init(); }

    wxGenericStaticText(wxWindow *parent,
                 wxWindowID id,
                 const wxString& label,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = 0,
                 const wxString& name = wxStaticTextNameStr)
    {
        Init();

        Create(parent, id, label, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& label,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = 0,
                const wxString& name = wxStaticTextNameStr);

    virtual ~wxGenericStaticText();


    // overridden base class virtual methods
    virtual void SetLabel(const wxString& label) wxOVERRIDE;
    virtual bool SetFont(const wxFont &font) wxOVERRIDE;

protected:
    virtual wxSize DoGetBestClientSize() const wxOVERRIDE;

    virtual wxString DoGetLabel() const wxOVERRIDE { return m_label; }
    virtual void DoSetLabel(const wxString& label) wxOVERRIDE;

    void DoSetSize(int x, int y, int width, int height, int sizeFlags) wxOVERRIDE;

#if wxUSE_MARKUP
    virtual bool DoSetLabelMarkup(const wxString& markup) wxOVERRIDE;
#endif // wxUSE_MARKUP

private:
    void Init()
    {
#if wxUSE_MARKUP
        m_markupText = NULL;
#endif // wxUSE_MARKUP
    }

    void OnPaint(wxPaintEvent& event);

    void DoDrawLabel(wxDC& dc, const wxRect& rect);

    // These fields are only used if m_markupText == NULL.
    wxString m_label;
    int m_mnemonic;

#if wxUSE_MARKUP
    class wxMarkupText *m_markupText;
#endif // wxUSE_MARKUP

    wxDECLARE_DYNAMIC_CLASS_NO_COPY(wxGenericStaticText);
};

#endif // _WX_GENERIC_STATTEXTG_H_

