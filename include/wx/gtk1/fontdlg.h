/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk1/fontdlg.h
// Purpose:     wxFontDialog
// Author:      Robert Roebling
// Created:
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __GTK_FONTDLGH__
#define __GTK_FONTDLGH__

//-----------------------------------------------------------------------------
// wxFontDialog
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxFontDialog : public wxFontDialogBase
{
public:
    wxFontDialog() : wxFontDialogBase() { /* must be Create()d later */ }
    wxFontDialog(wxWindow *parent)
        : wxFontDialogBase(parent) { Create(parent); }
    wxFontDialog(wxWindow *parent, const wxFontData& data)
        : wxFontDialogBase(parent, data) { Create(parent, data); }

    virtual ~wxFontDialog();

    // implementation only
    void SetChosenFont(const char *name);

#if WXWIN_COMPATIBILITY_2_6
    // deprecated interface, don't use
    wxDEPRECATED( wxFontDialog(wxWindow *parent, const wxFontData *data) );
#endif // WXWIN_COMPATIBILITY_2_6

protected:
    // create the GTK dialog
    virtual bool DoCreate(wxWindow *parent);

private:
    DECLARE_DYNAMIC_CLASS(wxFontDialog)
};

#if WXWIN_COMPATIBILITY_2_6
    // deprecated interface, don't use
inline wxFontDialog::wxFontDialog(wxWindow *parent, const wxFontData *data)
        : wxFontDialogBase(parent) { InitFontData(data); Create(parent); }
#endif // WXWIN_COMPATIBILITY_2_6

#endif
