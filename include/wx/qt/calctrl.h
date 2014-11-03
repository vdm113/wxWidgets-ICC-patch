/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/calctrl.h
// Purpose:     wxCalendarCtrl control implementation for wxQt
// Author:      Kolya Kosenko, Mariano Reingart
// Copyright:   (c) 2010 wxWidgets dev team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_CALCTRL_H_
#define _WX_QT_CALCTRL_H_

#include "wx/calctrl.h"
#include <QtWidgets/QCalendarWidget>

class WXDLLIMPEXP_ADV wxCalendarCtrl : public wxCalendarCtrlBase
{
public:
    wxCalendarCtrl() { Init(); }
    wxCalendarCtrl(wxWindow *parent,
                   wxWindowID id,
                   const wxDateTime& date = wxDefaultDateTime,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = wxCAL_SHOW_HOLIDAYS,
                   const wxString& name = wxCalendarNameStr)
    {
        Init();
        Create(parent, id, date, pos, size, style, name);
    }

    virtual ~wxCalendarCtrl();

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxDateTime& date = wxDefaultDateTime,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxCAL_SHOW_HOLIDAYS,
                const wxString& name = wxCalendarNameStr);

    virtual bool SetDate(const wxDateTime& date);
    virtual wxDateTime GetDate() const;

    virtual bool SetDateRange(const wxDateTime& lowerdate = wxDefaultDateTime,
                              const wxDateTime& upperdate = wxDefaultDateTime);
    virtual bool GetDateRange(wxDateTime *lowerdate, wxDateTime *upperdate) const;

    virtual bool EnableMonthChange(bool enable = true);
    virtual void Mark(size_t day, bool mark);

    // holidays colours
    virtual void SetHoliday(size_t day);
    virtual void SetHolidayColours(const wxColour& colFg, const wxColour& colBg);
    virtual const wxColour& GetHolidayColourFg() const { return m_colHolidayFg; }
    virtual const wxColour& GetHolidayColourBg() const { return m_colHolidayBg; }

    // header colours
    virtual void SetHeaderColours(const wxColour& colFg, const wxColour& colBg);
    virtual const wxColour& GetHeaderColourFg() const { return m_colHeaderFg; }
    virtual const wxColour& GetHeaderColourBg() const { return m_colHeaderBg; }

    // day attributes
    virtual wxCalendarDateAttr *GetAttr(size_t day) const;
    virtual void SetAttr(size_t day, wxCalendarDateAttr *attr);
    virtual void ResetAttr(size_t day) { SetAttr(day, NULL); }


    virtual void SetWindowStyleFlag(long style);

    using wxCalendarCtrlBase::GenerateAllChangeEvents;

    virtual QCalendarWidget *GetHandle() const;

protected:
    virtual void RefreshHolidays();

private:
    void Init();
    void UpdateStyle();

    QCalendarWidget *m_qtCalendar;
    wxColour m_colHeaderFg,
             m_colHeaderBg,
             m_colHolidayFg,
             m_colHolidayBg;

    wxCalendarDateAttr *m_attrs[31];


    DECLARE_DYNAMIC_CLASS(wxCalendarCtrl)
};

#endif // _WX_QT_CALCTRL_H_
