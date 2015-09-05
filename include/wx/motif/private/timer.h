/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/////////////////////////////////////////////////////////////////////////////
// Name:        wx/motif/private/timer.h
// Purpose:     wxTimer class
// Author:      Julian Smart
// Created:     17/09/98
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MOTIF_PRIVATE_TIMER_H_
#define _WX_MOTIF_PRIVATE_TIMER_H_

#include "wx/private/timer.h"

class WXDLLIMPEXP_CORE wxMotifTimerImpl : public wxTimerImpl
{
public:
    wxMotifTimerImpl(wxTimer* timer) : wxTimerImpl(timer) { m_id = 0; }
    virtual ~wxMotifTimerImpl();

    virtual bool Start(int milliseconds = -1, bool oneShot = false);
    virtual void Stop();
    virtual bool IsRunning() const { return m_id != 0; }

    // override this to rearm the timer if necessary (i.e. if not one shot) as
    // X timeouts are removed automatically when they expire
    virtual void Notify();

protected:
    // common part of Start() and Notify()
    void DoStart();

    long m_id;
};

#endif // _WX_MOTIF_PRIVATE_TIMER_H_
