/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/motif/slider.h
// Purpose:     wxSlider class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_SLIDER_H_
#define _WX_SLIDER_H_

#include "wx/control.h"

// Slider
class WXDLLIMPEXP_CORE wxSlider: public wxSliderBase
{
    wxDECLARE_DYNAMIC_CLASS(wxSlider);

public:
    wxSlider();

    wxSlider(wxWindow *parent, wxWindowID id,
        int value, int minValue, int maxValue,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxSL_HORIZONTAL,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxSliderNameStr)
    {
        Create(parent, id, value, minValue, maxValue, pos, size, style, validator, name);
    }

    virtual ~wxSlider();

    bool Create(wxWindow *parent, wxWindowID id,
        int value, int minValue, int maxValue,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxSL_HORIZONTAL,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxSliderNameStr);

    virtual int GetValue() const ;
    virtual void SetValue(int);

    void SetRange(int minValue, int maxValue);

    inline int GetMin() const { return m_rangeMin; }
    inline int GetMax() const { return m_rangeMax; }

    // For trackbars only
    void SetPageSize(int pageSize);
    int GetPageSize() const ;
    void SetLineSize(int lineSize);
    int GetLineSize() const ;
    void SetThumbLength(int len) ;
    int GetThumbLength() const ;

    void Command(wxCommandEvent& event);

protected:
    int           m_rangeMin;
    int           m_rangeMax;
    int           m_pageSize;
    int           m_lineSize;

    virtual void DoSetSize(int x, int y,
        int width, int height,
        int sizeFlags = wxSIZE_AUTO);

private:
    wxDECLARE_EVENT_TABLE();
};

#endif
// _WX_SLIDER_H_
