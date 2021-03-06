/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/private/timer.h
// Purpose:     wxTimerImpl for wxGTK
// Author:      Robert Roebling
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GTK_PRIVATE_TIMER_H_
#define _WX_GTK_PRIVATE_TIMER_H_

#if wxUSE_TIMER

#include "wx/private/timer.h"

//-----------------------------------------------------------------------------
// wxTimer
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxGTKTimerImpl : public wxTimerImpl
{
public:
    wxGTKTimerImpl(wxTimer* timer) : wxTimerImpl(timer) { m_sourceId = 0; }

    virtual bool Start( int millisecs = -1, bool oneShot = false );
    virtual void Stop();
    virtual bool IsRunning() const { return m_sourceId != 0; }

protected:
    int m_sourceId;
};

#endif // wxUSE_TIMER

#endif // _WX_GTK_PRIVATE_TIMER_H_
