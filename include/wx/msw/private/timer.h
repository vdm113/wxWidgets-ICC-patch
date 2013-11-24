/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/private/timer.h
// Purpose:     wxTimer class
// Author:      Julian Smart
// Created:     01/02/97
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MSW_PRIVATE_TIMER_H_
#define _WX_MSW_PRIVATE_TIMER_H_

#if wxUSE_TIMER

#include "wx/private/timer.h"
#include "wx/msw/wrapwin.h"     // for WPARAM

class WXDLLIMPEXP_BASE wxMSWTimerImpl : public wxTimerImpl
{
public:
    wxMSWTimerImpl(wxTimer *timer) : wxTimerImpl(timer) { m_id = 0; }

    virtual bool Start(int milliseconds = -1, bool oneShot = false);
    virtual void Stop();

    virtual bool IsRunning() const { return m_id != 0; }

protected:
    // this must be 64 bit under Win64 as WPARAM (storing timer ids) is 64 bit
    // there and so the ids may possibly not fit in 32 bits
    WPARAM m_id;
};

#endif // wxUSE_TIMER

#endif // _WX_TIMERH_
