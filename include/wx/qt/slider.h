/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/slider.h
// Author:      Peter Most, Mariano Reingart
// Copyright:   (c) 2010 wxWidgets dev team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_SLIDER_H_
#define _WX_QT_SLIDER_H_

#include <QtWidgets/QSlider>

class WXDLLIMPEXP_CORE wxSlider : public wxSliderBase
{
public:
    wxSlider();
    wxSlider(wxWindow *parent,
             wxWindowID id,
             int value, int minValue, int maxValue,
             const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxSL_HORIZONTAL,
             const wxValidator& validator = wxDefaultValidator,
             const wxString& name = wxSliderNameStr);

    bool Create(wxWindow *parent,
                wxWindowID id,
                int value, int minValue, int maxValue,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxSL_HORIZONTAL,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxSliderNameStr);

    virtual int GetValue() const;
    virtual void SetValue(int value);

    virtual void SetRange(int minValue, int maxValue);
    virtual int GetMin() const;
    virtual int GetMax() const;

    virtual void DoSetTickFreq(int freq);
    virtual int GetTickFreq() const;

    virtual void SetLineSize(int lineSize);
    virtual void SetPageSize(int pageSize);
    virtual int GetLineSize() const;
    virtual int GetPageSize() const;

    virtual void SetThumbLength(int lenPixels);
    virtual int GetThumbLength() const;

    virtual QSlider *GetHandle() const;

private:
    QSlider *m_qtSlider;

    wxDECLARE_DYNAMIC_CLASS( wxSlider );
};

#endif // _WX_QT_SLIDER_H_
