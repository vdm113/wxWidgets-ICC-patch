/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/sockqt.cpp
// Purpose:     implementation of wxQT-specific socket event handling
// Author:      Mariano Reingart
// Copyright:   (c) 2014 wxWidgets dev team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if wxUSE_SOCKETS

#include "wx/apptrait.h"
#include "wx/log.h"
#include "wx/private/fdiomanager.h"

#include <QtCore/QSocketNotifier>

class wxQtFDIONotifier : public QSocketNotifier
{
public:
    wxQtFDIONotifier(int fd, QSocketNotifier::Type type, wxFDIOHandler *handler)
        : QSocketNotifier(fd, type),
          m_handler(handler)
    {
        setEnabled(true);
        connect(this, &QSocketNotifier::activated, this, &wxQtFDIONotifier::activated);
    }

    wxFDIOHandler *handler()
    {
        return m_handler;
    }

    void activated()
    {
        switch (type())
        {
            case QSocketNotifier::Read:
                m_handler->OnReadWaiting();
                break;
            case QSocketNotifier::Write:
                m_handler->OnWriteWaiting();
                break;
            case QSocketNotifier::Exception:
                //m_handler->OnExceptionWaiting();
                break;
        }
    }
    wxFDIOHandler *m_handler;
};

class QtFDIOManager : public wxFDIOManager
{
public:
    virtual int AddInput(wxFDIOHandler *handler, int fd, Direction d)
    {
        QSocketNotifier::Type type;
        switch (d)
        {
            case INPUT:
                type = QSocketNotifier::Read;
                break;
            case OUTPUT:
                type = QSocketNotifier::Write;
                break;
        }
        m_qtNotifiers.insert(m_qtNotifiers.end(),
                             new wxQtFDIONotifier(fd, type, handler));
        handler->SetRegisteredEvent(d);
        return fd;
    }

    virtual void
    RemoveInput(wxFDIOHandler* handler, int fd, Direction d)
    {
        QSocketNotifier::Type type = d == INPUT ? QSocketNotifier::Read :
                                                  QSocketNotifier::Write;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   pragma prefetch
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
        for ( wxVector<wxQtFDIONotifier*>::iterator it = m_qtNotifiers.begin();
              it != m_qtNotifiers.end(); ++it )
        {
            wxQtFDIONotifier* notifier = static_cast<wxQtFDIONotifier*>(*it);
            if ( (notifier->socket() == fd) &&
                 (notifier->handler() == handler) &&
                 (notifier->type() == type) )
            {
                delete notifier;
                m_qtNotifiers.erase(it);
                break;
            }
        }
    }
    wxVector<wxQtFDIONotifier*> m_qtNotifiers;
};

wxFDIOManager *wxGUIAppTraits::GetFDIOManager()
{
    static QtFDIOManager s_manager;
    return &s_manager;
}

#endif // wxUSE_SOCKETS && __UNIX__
