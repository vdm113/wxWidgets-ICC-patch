/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/statusbar.h
// Author:      Peter Most, Javier Torres, Mariano Reingart, Sean D'Epagnier
// Copyright:   (c) 2010 wxWidgets dev team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_STATUSBAR_H_
#define _WX_QT_STATUSBAR_H_

#include "wx/statusbr.h"

#include <QtWidgets/QLabel>
#include <QtWidgets/QStatusBar>

class WXDLLIMPEXP_CORE wxStatusBar : public wxStatusBarBase
{
public:
    wxStatusBar() {}
    wxStatusBar(wxWindow *parent, wxWindowID winid = wxID_ANY,
                long style = wxSTB_DEFAULT_STYLE,
                const wxString& name = wxStatusBarNameStr);

    bool Create(wxWindow *parent, wxWindowID winid = wxID_ANY,
                long style = wxSTB_DEFAULT_STYLE,
                const wxString& name = wxStatusBarNameStr);

    virtual bool GetFieldRect(int i, wxRect& rect) const;
    virtual void SetMinHeight(int height);
    virtual int GetBorderX() const;
    virtual int GetBorderY() const;
    virtual void Refresh( bool eraseBackground = true,
                          const wxRect *rect = (const wxRect *) NULL );

    virtual QStatusBar *GetHandle() const;

protected:
    virtual void DoUpdateStatusText(int number);

private:
    void Init();
    void UpdateFields();

    QStatusBar *m_qtStatusBar;
    QList< QLabel* > m_qtPanes;

    wxDECLARE_DYNAMIC_CLASS(wxStatusBar);
};


#endif // _WX_QT_STATUSBAR_H_
