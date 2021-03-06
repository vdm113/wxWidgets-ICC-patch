/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        src/common/preferencescmn.cpp
// Purpose:     wxPreferencesEditor implementation common to all platforms.
// Author:      Vaclav Slavik
// Created:     2013-02-19
// Copyright:   (c) 2013 Vaclav Slavik <vslavik@fastmail.fm>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_PREFERENCES_EDITOR

#include "wx/private/preferences.h"
#include "wx/intl.h"

// ============================================================================
// implementation
// ============================================================================

wxString wxStockPreferencesPage::GetName() const
{
    wxString name;
    switch ( m_kind )
    {
        case Kind_General:
            name = _("General");
            break;
        case Kind_Advanced:
            name = _("Advanced");
            break;
    }
    return name;
}

wxPreferencesEditor::wxPreferencesEditor(const wxString& title)
    : m_impl(wxPreferencesEditorImpl::Create(title))
{
}

wxPreferencesEditor::~wxPreferencesEditor()
{
    delete m_impl;
}

void wxPreferencesEditor::AddPage(wxPreferencesPage* page)
{
    wxCHECK_RET( page, "can't set NULL page" );
    m_impl->AddPage(page);
}

void wxPreferencesEditor::Show(wxWindow* parent)
{
    m_impl->Show(parent);
}

void wxPreferencesEditor::Dismiss()
{
    m_impl->Dismiss();
}

#endif // wxUSE_PREFERENCES_EDITOR
