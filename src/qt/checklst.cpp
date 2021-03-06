/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/checklst.cpp
// Author:      Peter Most, Mariano Reingart
// Copyright:   (c) 2010 wxWidgets dev team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/checklst.h"

wxCheckListBox::wxCheckListBox()
{
}

wxCheckListBox::wxCheckListBox(wxWindow *parent, wxWindowID id,
        const wxPoint& pos,
        const wxSize& size,
        int nStrings,
        const wxString *choices,
        long style,
        const wxValidator& validator,
        const wxString& name )
{
    Create( parent, id, pos, size, nStrings, choices, style, validator, name );
}

wxCheckListBox::wxCheckListBox(wxWindow *parent, wxWindowID id,
        const wxPoint& pos,
        const wxSize& size,
        const wxArrayString& choices,
        long style,
        const wxValidator& validator,
        const wxString& name )
{
    Create( parent, id, pos, size, choices, style, validator, name );
}

wxCheckListBox::~wxCheckListBox()
{
    Clear();
}

bool wxCheckListBox::Create(wxWindow *parent, wxWindowID id,
              const wxPoint& pos,
              const wxSize& size,
              int n, const wxString choices[],
              long style,
              const wxValidator& validator,
              const wxString& name )
{
    return wxCheckListBoxBase::Create( parent, id, pos, size, n, choices, style, validator, name );
}

bool wxCheckListBox::Create(wxWindow *parent, wxWindowID id,
              const wxPoint& pos,
              const wxSize& size,
              const wxArrayString& choices,
              long style,
              const wxValidator& validator,
              const wxString& name )
{
    return wxCheckListBoxBase::Create( parent, id, pos, size, choices, style, validator, name );
}

void wxCheckListBox::Init()
{
    m_hasCheckBoxes = true;
}

bool wxCheckListBox::IsChecked(unsigned int n) const
{
    QListWidgetItem* item = m_qtListWidget->item(n);
    wxCHECK_MSG(item != NULL, false, wxT("wrong listbox index") );
    return item->checkState() == Qt::Checked;
}

void wxCheckListBox::Check(unsigned int n, bool check )
{
    QListWidgetItem* item = m_qtListWidget->item(n);
    wxCHECK_RET(item != NULL, wxT("wrong listbox index") );
    return item->setCheckState(check ? Qt::Checked : Qt::Unchecked);
}

