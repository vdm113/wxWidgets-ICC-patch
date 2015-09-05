/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/fontdlg.h
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_FONTDLG_H_
#define _WX_QT_FONTDLG_H_

#include <QtWidgets/QFontDialog>

class WXDLLIMPEXP_CORE wxFontDialog : public wxFontDialogBase
{
public:
    wxFontDialog() { }
    wxFontDialog(wxWindow *parent) { Create(parent); }
    wxFontDialog(wxWindow *parent, const wxFontData& data) { Create(parent, data); }
    
    virtual QFontDialog *GetHandle() const { return static_cast<QFontDialog *>(m_qtWindow); }

protected:
    bool DoCreate(wxWindow *parent);
    
private:

    wxFontData m_data;

    wxDECLARE_DYNAMIC_CLASS(wxFontDialog);
};

#endif // _WX_QT_FONTDLG_H_
