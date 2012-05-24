/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/osx/textentry_osx.cpp
// Purpose:     wxTextEntry
// Author:      Stefan Csomor
// Modified by: Kevin Ollivier
// Created:     1998-01-01
// RCS-ID:      $Id$
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#if wxUSE_TEXTCTRL

#include "wx/textctrl.h"

#ifndef WX_PRECOMP
    #include "wx/intl.h"
    #include "wx/app.h"
    #include "wx/utils.h"
    #include "wx/dc.h"
    #include "wx/button.h"
    #include "wx/menu.h"
    #include "wx/settings.h"
    #include "wx/msgdlg.h"
    #include "wx/toplevel.h"
#endif

#ifdef __DARWIN__
    #include <sys/types.h>
    #include <sys/stat.h>
#else
    #include <stat.h>
#endif

#if wxUSE_STD_IOSTREAM
    #if wxUSE_IOSTREAMH
        #include <fstream.h>
    #else
        #include <fstream>
    #endif
#endif

#include "wx/filefn.h"
#include "wx/sysopt.h"
#include "wx/thread.h"
#include "wx/textcompleter.h"

#include "wx/osx/private.h"

wxTextEntry::wxTextEntry()
{
    m_completer = NULL;
    m_editable = true;
    m_maxLength = 0;
}

wxTextEntry::~wxTextEntry()
{
    delete m_completer;
}

wxString wxTextEntry::DoGetValue() const
{
    wxCHECK_MSG( GetTextPeer(), wxString(), "Must create the control first" );

    return GetTextPeer()->GetStringValue() ;
}

void wxTextEntry::GetSelection(long* from, long* to) const
{
    wxCHECK_RET( GetTextPeer(), "Must create the control first" );

    GetTextPeer()->GetSelection( from , to ) ;
}

void wxTextEntry::SetMaxLength(unsigned long len)
{
    if ( GetTextPeer()->CanClipMaxLength() )
        GetTextPeer()->SetMaxLength(len);
    m_maxLength = len ;
}

// Clipboard operations

void wxTextEntry::Copy()
{
    wxCHECK_RET( GetTextPeer(), "Must create the control first" );

    if (CanCopy())
        GetTextPeer()->Copy() ;
}

void wxTextEntry::Cut()
{
    wxCHECK_RET( GetTextPeer(), "Must create the control first" );

    if (CanCut())
        GetTextPeer()->Cut() ;
}

void wxTextEntry::Paste()
{
    wxCHECK_RET( GetTextPeer(), "Must create the control first" );

    if (CanPaste())
        GetTextPeer()->Paste() ;
}

bool wxTextEntry::CanCopy() const
{
    // Can copy if there's a selection
    long from, to;
    GetSelection( &from, &to );

    return (from != to);
}

bool wxTextEntry::CanCut() const
{
    if ( !IsEditable() )
        return false;

    // Can cut if there's a selection
    long from, to;
    GetSelection( &from, &to );

    return (from != to);
}

bool wxTextEntry::CanPaste() const
{
    if (!IsEditable())
        return false;

    wxCHECK_MSG( GetTextPeer(), false, "Must create the control first" );

    return GetTextPeer()->CanPaste() ;
}

void wxTextEntry::SetEditable(bool editable)
{
    if ( editable == m_editable )
        return;

    m_editable = editable ;

    wxCHECK_RET( GetTextPeer(), "Must create the control first" );
    GetTextPeer()->SetEditable( editable ) ;
}

void wxTextEntry::SetInsertionPoint(long pos)
{
    SetSelection( pos , pos ) ;
}

void wxTextEntry::SetInsertionPointEnd()
{
    long pos = GetLastPosition();
    SetInsertionPoint( pos );
}

long wxTextEntry::GetInsertionPoint() const
{
    long begin, end ;
    GetSelection( &begin , &end ) ;

    return begin ;
}

wxTextPos wxTextEntry::GetLastPosition() const
{
    wxCHECK_MSG( GetTextPeer(), -1, "Must create the control first" );

    return GetTextPeer()->GetLastPosition() ;
}

void wxTextEntry::Remove(long from, long to)
{
    wxCHECK_RET( GetTextPeer(), "Must create the control first" );

    {
        EventsSuppressor noevents(this);
        GetTextPeer()->Remove( from , to );
    }

    SendTextUpdatedEventIfAllowed();
}

void wxTextEntry::SetSelection(long from, long to)
{
    wxCHECK_RET( GetTextPeer(), "Must create the control first" );

    GetTextPeer()->SetSelection( from , to ) ;
}

void wxTextEntry::WriteText(const wxString& str)
{
    wxCHECK_RET( GetTextPeer(), "Must create the control first" );

    {
        EventsSuppressor noevents(this);
        GetTextPeer()->WriteText( str );
    }

    SendTextUpdatedEventIfAllowed();
}

void wxTextEntry::Clear()
{
    wxCHECK_RET( GetTextPeer(), "Must create the control first" );

    {
        EventsSuppressor noevents(this);
        GetTextPeer()->Clear();
    }

    SendTextUpdatedEventIfAllowed();
}

bool wxTextEntry::IsEditable() const
{
    return m_editable ;
}

// ----------------------------------------------------------------------------
// Undo/redo
// ----------------------------------------------------------------------------

void wxTextEntry::Undo()
{
    wxCHECK_RET( GetTextPeer(), "Must create the control first" );

    if (CanUndo())
        GetTextPeer()->Undo() ;
}

void wxTextEntry::Redo()
{
    wxCHECK_RET( GetTextPeer(), "Must create the control first" );

    if (CanRedo())
        GetTextPeer()->Redo() ;
}

bool wxTextEntry::CanUndo() const
{
    if ( !IsEditable() )
        return false ;

    wxCHECK_MSG( GetTextPeer(), false, "Must create the control first" );

    return GetTextPeer()->CanUndo() ;
}

bool wxTextEntry::CanRedo() const
{
    if ( !IsEditable() )
        return false ;

    wxCHECK_MSG( GetTextPeer(), false, "Must create the control first" );

    return GetTextPeer()->CanRedo() ;
}

wxTextWidgetImpl * wxTextEntry::GetTextPeer() const
{
    wxWindow * const win = const_cast<wxTextEntry *>(this)->GetEditableWindow();

    return win ? dynamic_cast<wxTextWidgetImpl *>(win->GetPeer()) : NULL;
}

// ----------------------------------------------------------------------------
// Auto-completion
// ----------------------------------------------------------------------------

bool wxTextEntry::DoAutoCompleteStrings(const wxArrayString& choices)
{
    wxTextCompleterFixed * const completer = new wxTextCompleterFixed;
    completer->SetCompletions(choices);

    return DoAutoCompleteCustom(completer);
}

bool wxTextEntry::DoAutoCompleteCustom(wxTextCompleter *completer)
{
    m_completer = completer;

    return true;
}

#endif // wxUSE_TEXTCTRL
