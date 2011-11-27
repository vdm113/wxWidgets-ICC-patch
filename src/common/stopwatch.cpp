///////////////////////////////////////////////////////////////////////////////
// Name:        src/common/stopwatch.cpp
// Purpose:     wxStopWatch and other non-GUI stuff from wx/timer.h
// Author:
//    Original version by Julian Smart
//    Vadim Zeitlin got rid of all ifdefs (11.12.99)
//    Sylvain Bougnoux added wxStopWatch class
//    Guillermo Rodriguez <guille@iies.es> rewrote from scratch (Dic/99)
// Modified by:
// Created:     20.06.2003 (extracted from common/timercmn.cpp)
// RCS-ID:      $Id$
// Copyright:   (c) 1998-2003 wxWidgets Team
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

#include "wx/stopwatch.h"

#if wxUSE_STOPWATCH

#ifndef WX_PRECOMP
    #ifdef __WXMSW__
        #include "wx/msw/wrapwin.h"
    #endif
    #include "wx/log.h"
#endif //WX_PRECOMP

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxStopWatch
// ----------------------------------------------------------------------------

namespace
{

#ifdef __WXMSW__

struct PerfCounter
{
    PerfCounter()
    {
        init = false;
    }

    bool CanBeUsed() const
    {
        return freq.QuadPart != 0;
    }

    wxCriticalSection cs;
    LARGE_INTEGER freq;
    bool init;
} gs_perfCounter;

#endif // __WXMSW__

const int MILLISECONDS_PER_SECOND = 1000;
const int MICROSECONDS_PER_MILLISECOND = 1000;
const int MICROSECONDS_PER_SECOND = 1000*1000;

} // anonymous namespace

void wxStopWatch::DoStart()
{
#ifdef __WXMSW__
    if ( !gs_perfCounter.init )
    {
        wxCriticalSectionLocker lock(gs_perfCounter.cs);
        ::QueryPerformanceFrequency(&gs_perfCounter.freq);

        // Just a sanity check: it's not supposed to happen but verify that
        // ::QueryPerformanceCounter() succeeds so that we can really use it.
        LARGE_INTEGER counter;
        if ( !::QueryPerformanceCounter(&counter) )
        {
            wxLogDebug("QueryPerformanceCounter() unexpected failed (%s), "
                       "will not use it.", wxSysErrorMsg());

            gs_perfCounter.freq.QuadPart = 0;
        }

        gs_perfCounter.init = true;
    }
#endif // __WXMSW__

    m_t0 = GetCurrentClockValue();
}

wxLongLong wxStopWatch::GetClockFreq() const
{
#ifdef __WXMSW__
    // Under MSW we use the high resolution performance counter timer which has
    // its own frequency (usually related to the CPU clock speed).
    if ( gs_perfCounter.CanBeUsed() )
        return gs_perfCounter.freq.QuadPart;
#endif // __WXMSW__

    // Currently milliseconds are used everywhere else.
    return MILLISECONDS_PER_SECOND;
}

void wxStopWatch::Start(long t0)
{
    DoStart();

    m_t0 -= (wxLongLong(t0)*GetClockFreq())/MILLISECONDS_PER_SECOND;
}

wxLongLong wxStopWatch::GetCurrentClockValue() const
{
#ifdef __WXMSW__
    if ( gs_perfCounter.CanBeUsed() )
    {
        LARGE_INTEGER counter;
        ::QueryPerformanceCounter(&counter);
        return counter.QuadPart;
    }
#endif // __WXMSW__

    return wxGetUTCTimeMillis();
}

wxLongLong wxStopWatch::TimeInMicro() const
{
    const wxLongLong elapsed(m_pauseCount ? m_elapsedBeforePause
                                          : GetCurrentClockValue() - m_t0);

    return (elapsed*MICROSECONDS_PER_SECOND)/GetClockFreq();
}

#endif // wxUSE_STOPWATCH

// ----------------------------------------------------------------------------
// old timer functions superceded by wxStopWatch
// ----------------------------------------------------------------------------

#if wxUSE_LONGLONG

static wxLongLong wxStartTime = 0l;

// starts the global timer
void wxStartTimer()
{
    wxStartTime = wxGetUTCTimeMillis();
}

// Returns elapsed time in milliseconds
long wxGetElapsedTime(bool resetTimer)
{
    wxLongLong oldTime = wxStartTime;
    wxLongLong newTime = wxGetUTCTimeMillis();

    if ( resetTimer )
        wxStartTime = newTime;

    return (newTime - oldTime).GetLo();
}

#endif // wxUSE_LONGLONG
