/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////


// Name:        wx/osx/carbon/private/timer.h
// Purpose:     wxTimer class
// Author:      Stefan Csomor
// Created:     1998-01-01
// RCS-ID:      $Id$
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MAC_PRIVATE_TIMER_H_
#define _WX_MAC_PRIVATE_TIMER_H_

#include "wx/private/timer.h"

struct MacTimerInfo;

class WXDLLIMPEXP_CORE wxCarbonTimerImpl : public wxTimerImpl
{
public:
    wxCarbonTimerImpl(wxTimer *timer);
    virtual ~wxCarbonTimerImpl();

    virtual bool Start(int milliseconds = -1, bool one_shot = false);
    virtual void Stop();

    virtual bool IsRunning() const;

private:
    MacTimerInfo *m_info;
};

#endif // _WX_MAC_PRIVATE_TIMER_H_
