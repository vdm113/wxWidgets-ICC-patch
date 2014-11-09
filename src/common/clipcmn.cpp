/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/clipcmn.cpp
// Purpose:     common (to all ports) wxClipboard functions
// Author:      Robert Roebling
// Modified by:
// Created:     28.06.99
// Copyright:   (c) Robert Roebling
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

#if wxUSE_CLIPBOARD

#include "wx/clipbrd.h"

#ifndef WX_PRECOMP
    #include "wx/dataobj.h"
    #include "wx/module.h"
#endif

// ---------------------------------------------------------
// wxClipboardEvent
// ---------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxClipboardEvent,wxEvent)

wxDEFINE_EVENT( wxEVT_CLIPBOARD_CHANGED, wxClipboardEvent );

bool wxClipboardEvent::SupportsFormat( const wxDataFormat &format ) const
{
#ifdef __WXGTK20__
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma unroll
#   pragma swp
#endif
    for (wxVector<wxDataFormat>::size_type n = 0; n < m_formats.size(); n++)
    {
        if (m_formats[n] == format)
            return true;
    }

    return false;
#else
    // All other ports just query the clipboard directly
    // from here
    wxClipboard* clipboard = (wxClipboard*) GetEventObject();
    return clipboard->IsSupported( format );
#endif
}

void wxClipboardEvent::AddFormat(const wxDataFormat& format)
{
    m_formats.push_back( format );
}

// ---------------------------------------------------------
// wxClipboardBase
// ---------------------------------------------------------

static wxClipboard *gs_clipboard = NULL;

/*static*/ wxClipboard *wxClipboardBase::Get()
{
    if ( !gs_clipboard )
    {
        gs_clipboard = new wxClipboard;
    }
    return gs_clipboard;
}

bool wxClipboardBase::IsSupportedAsync( wxEvtHandler *sink )
{
    // We just imitate an asynchronous API on most platforms.
    // This method is overridden uner GTK.
    wxClipboardEvent *event = new wxClipboardEvent(wxEVT_CLIPBOARD_CHANGED);
    event->SetEventObject( this );

    sink->QueueEvent( event );

    return true;
}


// ----------------------------------------------------------------------------
// wxClipboardModule: module responsible for destroying the global clipboard
// object
// ----------------------------------------------------------------------------

class wxClipboardModule : public wxModule
{
public:
    bool OnInit() wxOVERRIDE { return true; }
    void OnExit() wxOVERRIDE { wxDELETE(gs_clipboard); }

private:
    DECLARE_DYNAMIC_CLASS(wxClipboardModule)
};

IMPLEMENT_DYNAMIC_CLASS(wxClipboardModule, wxModule)

#endif // wxUSE_CLIPBOARD
