/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/osx/carbon/evtloop.h
// Purpose:     declaration of wxEventLoop for wxMac
// Author:      Vadim Zeitlin
// Modified by:
// Created:     2006-01-12
// RCS-ID:      $Id$
// Copyright:   (c) 2006 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MAC_CARBON_EVTLOOP_H_
#define _WX_MAC_CARBON_EVTLOOP_H_

struct OpaqueEventRef;
typedef OpaqueEventRef *EventRef;

class WXDLLIMPEXP_CORE wxGUIEventLoop : public wxCFEventLoop
{
public:
    wxGUIEventLoop();

    virtual void WakeUp();

protected:
    virtual int DoDispatchTimeout(unsigned long timeout);

    virtual void OSXDoRun();
    virtual void OSXDoStop();

    virtual CFRunLoopRef CFGetCurrentRunLoop() const;
};

#endif // _WX_MAC_CARBON_EVTLOOP_H_

