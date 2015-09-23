/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/filedlg.h
// Purpose:     wxFileDialog class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_FILEDLG_H_
#define _WX_FILEDLG_H_

//-------------------------------------------------------------------------
// wxFileDialog
//-------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxFileDialog: public wxFileDialogBase
{
public:
    wxFileDialog(wxWindow *parent,
                 const wxString& message = wxFileSelectorPromptStr,
                 const wxString& defaultDir = wxEmptyString,
                 const wxString& defaultFile = wxEmptyString,
                 const wxString& wildCard = wxFileSelectorDefaultWildcardStr,
                 long style = wxFD_DEFAULT_STYLE,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& sz = wxDefaultSize,
                 const wxString& name = wxFileDialogNameStr);

    virtual void GetPaths(wxArrayString& paths) const;
    virtual void GetFilenames(wxArrayString& files) const;
    virtual bool SupportsExtraControl() const { return true; }
    void MSWOnInitDialogHook(WXHWND hwnd);

    virtual int ShowModal();

    // wxMSW-specific implementation from now on
    // -----------------------------------------

    // called from the hook procedure on CDN_INITDONE reception
    virtual void MSWOnInitDone(WXHWND hDlg);

    // called from the hook procedure on CDN_SELCHANGE.
    void MSWOnSelChange(WXHWND hDlg);

protected:

    virtual void DoMoveWindow(int x, int y, int width, int height);
    virtual void DoCentre(int dir);
    virtual void DoGetSize( int *width, int *height ) const;
    virtual void DoGetPosition( int *x, int *y ) const;

private:
    wxArrayString m_fileNames;

    // remember if our SetPosition() or Centre() (which requires special
    // treatment) was called
    bool m_bMovedWindow;
    int m_centreDir;        // nothing to do if 0

    wxDECLARE_DYNAMIC_CLASS(wxFileDialog);
    wxDECLARE_NO_COPY_CLASS(wxFileDialog);
};

#endif // _WX_FILEDLG_H_

