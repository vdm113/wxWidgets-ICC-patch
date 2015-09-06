/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/filedlg.cpp
// Author:      Sean D'Epagnier
// Copyright:   (c) 2014 Sean D'Epagnier
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/qt/private/winevent.h"
#include "wx/fontdlg.h"

class wxQtFontDialog : public wxQtEventSignalHandler< QFontDialog, wxFontDialog >
{
public:
    wxQtFontDialog( wxWindow *parent, wxFontDialog *handler)
        : wxQtEventSignalHandler(parent, handler)
        {
            connect(this, &QFontDialog::currentFontChanged, this, &wxQtFontDialog::updateFont);
        }

    void updateFont(const QFont &font)
        {
            GetHandler()->GetFontData().SetChosenFont(wxFont(font));
        }
};

wxIMPLEMENT_DYNAMIC_CLASS(wxFontDialog, wxDialog);

bool wxFontDialog::DoCreate(wxWindow *parent)
{
    m_qtWindow = new wxQtFontDialog( parent, this );
    GetHandle()->setCurrentFont(m_fontData.GetInitialFont().GetHandle());
    return wxFontDialogBase::DoCreate(parent);
}


