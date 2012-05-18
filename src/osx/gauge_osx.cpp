/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif defined(__INTEL_COMPILER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////

// Name:        src/osx/gauge_osx.cpp
// Purpose:     wxGauge class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id$
// Copyright:   (c) Stefan Csomor
// Licence:       wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#if wxUSE_GAUGE

#include "wx/gauge.h"

#include "wx/osx/private.h"

bool wxGauge::Create( wxWindow *parent,
    wxWindowID id,
    int range,
    const wxPoint& pos,
    const wxSize& s,
    long style,
    const wxValidator& validator,
    const wxString& name )
{    
    DontCreatePeer();
    
    if ( !wxGaugeBase::Create( parent, id, range, pos, s, style & 0xE0FFFFFF, validator, name ) )
        return false;

    wxSize size = s;

    SetPeer(wxWidgetImpl::CreateGauge( this, parent, id, GetValue() , 0, GetRange(), pos, size, style, GetExtraStyle() ));

    MacPostControlCreate( pos, size );

    return true;
}

void wxGauge::SetRange(int r)
{
    // we are going via the base class in case there is
    // some change behind the values by it
    wxGaugeBase::SetRange( r ) ;
    if ( GetPeer() )
        GetPeer()->SetMaximum( GetRange() ) ;
}

void wxGauge::SetValue(int pos)
{
    // we are going via the base class in case there is
    // some change behind the values by it
    wxGaugeBase::SetValue( pos ) ;

    if ( GetPeer() )
        GetPeer()->SetValue( GetValue() ) ;
}

int wxGauge::GetValue() const
{
    return m_gaugePos ;
}

void wxGauge::Pulse()
{
    GetPeer()->PulseGauge();
}

#endif // wxUSE_GAUGE

