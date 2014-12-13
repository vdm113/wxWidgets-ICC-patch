/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/filedlg.h
// Author:      Sean D'Epagnier
// Copyright:   (c) 2014 Sean D'Epagnier
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_FILEDLG_H_
#define _WX_QT_FILEDLG_H_

#include <QtWidgets/QFileDialog>

class WXDLLIMPEXP_CORE wxFileDialog : public wxFileDialogBase
{
public:
    wxFileDialog() { }
    wxFileDialog(wxWindow *parent,
                 const wxString& message = wxFileSelectorPromptStr,
                 const wxString& defaultDir = wxEmptyString,
                 const wxString& defaultFile = wxEmptyString,
                 const wxString& wildCard = wxFileSelectorDefaultWildcardStr,
                 long style = wxFD_DEFAULT_STYLE,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& sz = wxDefaultSize,
                 const wxString& name = wxFileDialogNameStr);
    bool Create(wxWindow *parent,
                 const wxString& message = wxFileSelectorPromptStr,
                 const wxString& defaultDir = wxEmptyString,
                 const wxString& defaultFile = wxEmptyString,
                 const wxString& wildCard = wxFileSelectorDefaultWildcardStr,
                 long style = wxFD_DEFAULT_STYLE,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& sz = wxDefaultSize,
                 const wxString& name = wxFileDialogNameStr);

    virtual wxString GetPath() const wxOVERRIDE;
    virtual void GetPaths(wxArrayString& paths) const wxOVERRIDE;
    virtual wxString GetFilename() const wxOVERRIDE;
    virtual void GetFilenames(wxArrayString& files) const wxOVERRIDE;
    virtual int GetFilterIndex() const wxOVERRIDE;

    virtual void SetMessage(const wxString& message) wxOVERRIDE;
    virtual void SetPath(const wxString& path) wxOVERRIDE;
    virtual void SetDirectory(const wxString& dir) wxOVERRIDE;
    virtual void SetFilename(const wxString& name) wxOVERRIDE;
    virtual void SetWildcard(const wxString& wildCard) wxOVERRIDE;
    virtual void SetFilterIndex(int filterIndex) wxOVERRIDE;

    virtual bool SupportsExtraControl() const wxOVERRIDE { return true; }

    virtual QFileDialog *GetHandle() const;
    
private:
    
    DECLARE_DYNAMIC_CLASS(wxFileDialog)
};

#endif // _WX_QT_FILEDLG_H_
