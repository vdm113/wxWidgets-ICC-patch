/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/evtloop.h
// Author:      Peter Most, Javier Torres, Mariano Reingart, Sean D'Epagnier
// Copyright:   (c) 2010 wxWidgets dev team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_EVTLOOP_H_
#define _WX_QT_EVTLOOP_H_

#include <QtCore/QTimer>

class WXDLLIMPEXP_BASE wxQtEventLoopBase : public wxEventLoopBase
{
public:
    wxQtEventLoopBase();
    ~wxQtEventLoopBase();

    virtual int DoRun();
    virtual void ScheduleExit(int rc = 0);
    virtual bool Pending() const;
    virtual bool Dispatch();
    virtual int DispatchTimeout(unsigned long timeout);
    virtual void WakeUp();
    virtual void DoYieldFor(long eventsToProcess);

#if wxUSE_EVENTLOOP_SOURCE
    virtual wxEventLoopSource *AddSourceForFD(int fd, wxEventLoopSourceHandler *handler, int flags);
#endif // wxUSE_EVENTLOOP_SOURCE
protected:

private:
    QTimer *m_qtIdleTimer;
    
    wxDECLARE_NO_COPY_CLASS(wxQtEventLoopBase);
};


#if wxUSE_GUI

class WXDLLIMPEXP_CORE wxGUIEventLoop : public wxQtEventLoopBase
{
public:
    wxGUIEventLoop();
};

#else // !wxUSE_GUI

#if wxUSE_CONSOLE_EVENTLOOP

class WXDLLIMPEXP_BASE wxConsoleEventLoop : public wxQtEventLoopBase
{
public:
    wxConsoleEventLoop();
};

#endif // wxUSE_CONSOLE_EVENTLOOP

#endif // wxUSE_GUI


class wxQtEventLoopBase;

class wxQtIdleTimer : public QTimer
{

public:
    wxQtIdleTimer( wxQtEventLoopBase *eventLoop );
    virtual bool eventFilter( QObject * watched, QEvent * event  );

private:
    void idle();

private:
    wxQtEventLoopBase *m_eventLoop;
};

#endif // _WX_QT_EVTLOOP_H_
