/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/anybutton.h
// Purpose:     wxQT wxAnyButton class declaration
// Author:      Mariano Reingart
// Copyright:   (c) 2014 wxWidgets dev team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_ANYBUTTON_H_
#define _WX_QT_ANYBUTTON_H_

#include <QtWidgets/QPushButton>

//-----------------------------------------------------------------------------
// wxAnyButton
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxAnyButton : public wxAnyButtonBase
{
public:
    wxAnyButton()
    {
    }

    // implementation
    // --------------

    virtual void SetLabel( const wxString &label );
    virtual void DoSetBitmap(const wxBitmap& bitmap, State which);

    virtual QPushButton *GetHandle() const;

protected:

    QPushButton *m_qtPushButton;

    void QtCreate(wxWindow *parent);
    void QtSetBitmap( const wxBitmap &bitmap );

private:
    typedef wxAnyButtonBase base_type;

    wxDECLARE_NO_COPY_CLASS(wxAnyButton);
};


#endif // _WX_QT_ANYBUTTON_H_
