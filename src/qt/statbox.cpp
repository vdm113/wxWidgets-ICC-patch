/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/statbox.cpp
// Author:      Peter Most, Mariano Reingart
// Copyright:   (c) 2010 wxWidgets dev team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/statbox.h"
#include "wx/window.h"
#include "wx/qt/private/converter.h"
#include "wx/qt/private/winevent.h"
#include <QtWidgets/QGroupBox>


class wxQtGroupBox : public wxQtEventSignalHandler< QGroupBox, wxStaticBox >
{
public:
    wxQtGroupBox( wxWindow *parent, wxStaticBox *handler ):
        wxQtEventSignalHandler< QGroupBox, wxStaticBox >( parent, handler ){}
};


wxStaticBox::wxStaticBox()
{
}

wxStaticBox::wxStaticBox(wxWindow *parent, wxWindowID id,
            const wxString& label,
            const wxPoint& pos,
            const wxSize& size,
            long style,
            const wxString& name)
{
    Create( parent, id, label, pos, size, style, name );
}

bool wxStaticBox::Create(wxWindow *parent, wxWindowID id,
            const wxString& label,
            const wxPoint& pos,
            const wxSize& size,
            long style,
            const wxString& name)
{
    m_qtGroupBox = new wxQtGroupBox( parent, this );
    m_qtGroupBox->setTitle( wxQtConvertString( label ) );

    return QtCreateControl( parent, id, pos, size, style, wxDefaultValidator, name );
}

QGroupBox *wxStaticBox::GetHandle() const
{
    return m_qtGroupBox;
}

void wxStaticBox::GetBordersForSizer(int *borderTop, int *borderOther) const
{
    wxStaticBoxBase::GetBordersForSizer(borderTop, borderOther);

    // need extra space for the label:
    *borderTop += GetCharHeight();
}
