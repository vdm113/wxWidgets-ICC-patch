/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/timer.h
// Author:      Javier Torres
// Copyright:   (c) Javier Torres
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_TIMER_H_
#define _WX_QT_TIMER_H_

#if wxUSE_TIMER

#include "wx/private/timer.h"

#include <QtCore/QObject>

//-----------------------------------------------------------------------------
// wxTimer
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxQtTimerImpl : public wxTimerImpl, QObject
{
public:
    wxQtTimerImpl( wxTimer* timer );

    virtual bool Start( int millisecs = -1, bool oneShot = false );
    virtual void Stop();
    virtual bool IsRunning() const;

protected:
    virtual void timerEvent( QTimerEvent * event );
    
private:
    int m_timerId;
};

#endif // wxUSE_TIMER

#endif // _WX_QT_TIMER_H_
