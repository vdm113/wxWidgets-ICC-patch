/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////


// Name:        src/common/fddlgcmn.cpp
// Purpose:     common parts of wxFindReplaceDialog implementations
// Author:      Vadim Zeitlin
// Modified by:
// Created:     01.08.01
// RCS-ID:      $Id$
// Copyright:   (c) 2001 Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_FINDREPLDLG

#ifndef WX_PRECOMP
#endif

#include "wx/fdrepdlg.h"

// ----------------------------------------------------------------------------
// wxWin macros
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxFindDialogEvent, wxCommandEvent)

wxDEFINE_EVENT( wxEVT_COMMAND_FIND, wxFindDialogEvent );
wxDEFINE_EVENT( wxEVT_COMMAND_FIND_NEXT, wxFindDialogEvent );
wxDEFINE_EVENT( wxEVT_COMMAND_FIND_REPLACE, wxFindDialogEvent );
wxDEFINE_EVENT( wxEVT_COMMAND_FIND_REPLACE_ALL, wxFindDialogEvent );
wxDEFINE_EVENT( wxEVT_COMMAND_FIND_CLOSE, wxFindDialogEvent );

// ============================================================================
// implementations
// ============================================================================

// ----------------------------------------------------------------------------
// wxFindReplaceData
// ----------------------------------------------------------------------------

void wxFindReplaceData::Init()
{
    m_Flags = 0;
}

// ----------------------------------------------------------------------------
// wxFindReplaceDialogBase
// ----------------------------------------------------------------------------

wxFindReplaceDialogBase::~wxFindReplaceDialogBase()
{
}

void wxFindReplaceDialogBase::Send(wxFindDialogEvent& event)
{
    // we copy the data to dialog->GetData() as well

    m_FindReplaceData->m_Flags = event.GetFlags();
    m_FindReplaceData->m_FindWhat = event.GetFindString();
    if ( HasFlag(wxFR_REPLACEDIALOG) &&
         (event.GetEventType() == wxEVT_COMMAND_FIND_REPLACE ||
          event.GetEventType() == wxEVT_COMMAND_FIND_REPLACE_ALL) )
    {
        m_FindReplaceData->m_ReplaceWith = event.GetReplaceString();
    }

    // translate wxEVT_COMMAND_FIND_NEXT to wxEVT_COMMAND_FIND if needed
    if ( event.GetEventType() == wxEVT_COMMAND_FIND_NEXT )
    {
        if ( m_FindReplaceData->m_FindWhat != m_lastSearch )
        {
            event.SetEventType(wxEVT_COMMAND_FIND);

            m_lastSearch = m_FindReplaceData->m_FindWhat;
        }
    }

    if ( !GetEventHandler()->ProcessEvent(event) )
    {
        // the event is not propagated upwards to the parent automatically
        // because the dialog is a top level window, so do it manually as
        // in 9 cases of 10 the message must be processed by the dialog
        // owner and not the dialog itself
        (void)GetParent()->GetEventHandler()->ProcessEvent(event);
    }
}

#endif // wxUSE_FINDREPLDLG

