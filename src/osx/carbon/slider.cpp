/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/osx/carbon/slider.cpp
// Purpose:     wxSlider
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// Copyright:   (c) Stefan Csomor
// Licence:       wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#if wxUSE_SLIDER

#include "wx/slider.h"
#include "wx/osx/private.h"

class wxMacSliderCarbonControl : public wxMacControl
{
public :
    wxMacSliderCarbonControl( wxWindowMac* peer ) : wxMacControl( peer )
    {
    }
    
    // work around an OSX bug : if the control is having the keyboard focus it cannot
    // be set to the full max/min values by dragging
    virtual bool CanFocus() const
    { 
        return false; 
    }
};


wxWidgetImplType* wxWidgetImpl::CreateSlider( wxWindowMac* wxpeer,
                                    wxWindowMac* parent,
                                    wxWindowID WXUNUSED(id),
                                    wxInt32 value,
                                    wxInt32 minimum,
                                    wxInt32 maximum,
                                    const wxPoint& pos,
                                    const wxSize& size,
                                    long style,
                                    long WXUNUSED(extraStyle))
{
    Rect bounds = wxMacGetBoundsForControl( wxpeer, pos, size );
    int tickMarks = 0;
    if ( style & wxSL_AUTOTICKS )
        tickMarks = (maximum - minimum) + 1; // +1 for the 0 value

    // keep the number of tickmarks from becoming unwieldy, therefore below it is ok to cast
    // it to a UInt16
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   pragma prefetch
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
    while (tickMarks > 20)
        tickMarks /= 5;


    wxMacControl* peer = new wxMacSliderCarbonControl( wxpeer );
    OSStatus err = CreateSliderControl(
        MAC_WXHWND(parent->MacGetTopLevelWindowRef()), &bounds,
        value, minimum, maximum,
        kControlSliderPointsDownOrRight,
        (UInt16) tickMarks, true /* liveTracking */,
        GetwxMacLiveScrollbarActionProc(),
        peer->GetControlRefAddr() );
    verify_noerr( err );

    return peer;
}

#endif // wxUSE_SLIDER
