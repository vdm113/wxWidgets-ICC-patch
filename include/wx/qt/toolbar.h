/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/toolbar.h
// Author:      Sean D'Epagnier, Mariano Reingart
// Copyright:   (c) 2010 wxWidgets dev team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include <QtWidgets/QToolBar>

#ifndef _WX_QT_TOOLBAR_H_
#define _WX_QT_TOOLBAR_H_

class wxQtToolBar;

class WXDLLIMPEXP_CORE wxToolBar : public wxToolBarBase
{
public:

    wxToolBar() { Init(); }
    wxToolBar(wxWindow *parent,
              wxWindowID id,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style = wxTB_DEFAULT_STYLE | wxNO_BORDER,
              const wxString& name = wxToolBarNameStr)
    {
        Init();

        Create(parent, id, pos, size, style, name);
    }

    virtual ~wxToolBar();

    void Init();
    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxTB_DEFAULT_STYLE | wxNO_BORDER,
                const wxString& name = wxToolBarNameStr);

    virtual wxToolBarToolBase *FindToolForPosition(wxCoord x, wxCoord y) const;
    virtual QToolBar *GetHandle() const;

    virtual void SetWindowStyleFlag( long style );
    virtual bool Realize() wxOVERRIDE;

    virtual wxToolBarToolBase *CreateTool(int toolid,
                                          const wxString& label,
                                          const wxBitmap& bmpNormal,
                                          const wxBitmap& bmpDisabled,
                                          wxItemKind kind,
                                          wxObject *clientData,
                                          const wxString& shortHelp,
                                          const wxString& longHelp);

    virtual wxToolBarToolBase *CreateTool(wxControl *control,
                                          const wxString& label);

protected:

    QActionGroup* GetActionGroup(size_t pos);
    virtual bool DoInsertTool(size_t pos, wxToolBarToolBase *tool);
    virtual bool DoDeleteTool(size_t pos, wxToolBarToolBase *tool);
    virtual void DoEnableTool(wxToolBarToolBase *tool, bool enable);
    virtual void DoToggleTool(wxToolBarToolBase *tool, bool toggle);
    virtual void DoSetToggle(wxToolBarToolBase *tool, bool toggle);

private:
    Qt::ToolButtonStyle GetButtonStyle();

    QToolBar *m_qtToolBar;

    wxDECLARE_DYNAMIC_CLASS(wxToolBar);
};

#endif // _WX_QT_TOOLBAR_H_
