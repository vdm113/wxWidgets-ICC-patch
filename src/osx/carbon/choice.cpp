/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/osx/carbon/choice.cpp
// Purpose:     wxChoice
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#if wxUSE_CHOICE

#include "wx/choice.h"

#ifndef WX_PRECOMP
    #include "wx/menu.h"
    #include "wx/dcclient.h"
#endif

#include "wx/osx/private.h"

// adapt the number offset (mac menu are 1 based)

class wxMacChoiceCarbonControl : public wxMacControl
{
public :
    wxMacChoiceCarbonControl( wxWindowMac* peer ) : wxMacControl( peer )
    {
    }

    void SetValue(wxInt32 v)
    {
        wxMacControl::SetValue( v + 1 );
    }

    wxInt32 GetValue() const
    {
        return wxMacControl::GetValue() - 1;
    }
 };

wxWidgetImplType* wxWidgetImpl::CreateChoice( wxWindowMac* wxpeer,
                                    wxWindowMac* parent,
                                    wxWindowID WXUNUSED(id),
                                    wxMenu* menu,
                                    const wxPoint& pos,
                                    const wxSize& size,
                                    long WXUNUSED(style),
                                    long WXUNUSED(extraStyle))
{
    Rect bounds = wxMacGetBoundsForControl( wxpeer , pos , size );

    wxMacControl* peer = new wxMacChoiceCarbonControl( wxpeer ) ;
    OSStatus err = CreatePopupButtonControl(
        MAC_WXHWND(parent->MacGetTopLevelWindowRef()) , &bounds , CFSTR("") ,
        -12345 , false /* no variable width */ , 0 , 0 , 0 , peer->GetControlRefAddr() );
    verify_noerr( err );

    peer->SetData<MenuHandle>( kControlNoPart , kControlPopupButtonMenuHandleTag , (MenuHandle) menu->GetHMenu() ) ;
    return peer;
}

#endif // wxUSE_CHOICE
