///////////////////////////////////////////////////////////////////////////////
// Name:        src/osx/carbon/evtloop.cpp
// Purpose:     implementation of wxEventLoop for wxMac
// Author:      Vadim Zeitlin
// Modified by:
// Created:     2006-01-12
// RCS-ID:      $Id$
// Copyright:   (c) 2006 Vadim Zeitlin <vadim@wxwindows.org>
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

#include "wx/evtloop.h"

#ifndef WX_PRECOMP
    #include "wx/app.h"
#endif // WX_PRECOMP

#include "wx/osx/private.h"

// ============================================================================
// wxEventLoop implementation
// ============================================================================

wxGUIEventLoop::wxGUIEventLoop()
{
    m_sleepTime = kEventDurationNoWait;
}

void wxGUIEventLoop::WakeUp()
{
    extern void wxMacWakeUp();

    wxMacWakeUp();
}

bool wxGUIEventLoop::Pending() const
{
    EventRef theEvent;

    return ReceiveNextEvent
           (
            0,          // we want any event at all so we don't specify neither
            NULL,       // the number of event types nor the types themselves
            kEventDurationNoWait,
            false,      // don't remove the event from queue
            &theEvent
           ) == noErr;
}

bool wxGUIEventLoop::Dispatch()
{
    if ( !wxTheApp )
        return false;

    wxMacAutoreleasePool autoreleasepool;

    EventRef theEvent;

    OSStatus status = ReceiveNextEvent(0, NULL, m_sleepTime, true, &theEvent) ;

    switch (status)
    {
        case eventLoopTimedOutErr :
            if ( wxTheApp->ProcessIdle() )
                m_sleepTime = kEventDurationNoWait ;
            else
            {
                m_sleepTime = kEventDurationSecond;
#if wxUSE_THREADS
                wxMutexGuiLeave();
                wxMilliSleep(20);
                wxMutexGuiEnter();
#endif
            }
            break;

        case eventLoopQuitErr :
            // according to QA1061 this may also occur
            // when a WakeUp Process is executed
            break;

        default:
            if ( wxTheApp )
                wxTheApp->MacSetCurrentEvent( theEvent, NULL );

            OSStatus status = SendEventToEventTarget(theEvent, GetEventDispatcherTarget());
            if (status == eventNotHandledErr && wxTheApp)
                wxTheApp->MacHandleUnhandledEvent(theEvent);

            ReleaseEvent( theEvent );
            m_sleepTime = kEventDurationNoWait ;
            break;
    }

    return true;
}
