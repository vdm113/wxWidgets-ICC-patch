/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/unix/evtloopsrc.h
// Purpose:     wxUnixEventLoopSource class
// Author:      Vadim Zeitlin
// Created:     2009-10-21
// Copyright:   (c) 2009 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIX_EVTLOOPSRC_H_
#define _WX_UNIX_EVTLOOPSRC_H_

class wxFDIODispatcher;
class wxFDIOHandler;

// ----------------------------------------------------------------------------
// wxUnixEventLoopSource: wxEventLoopSource for Unix-like toolkits using fds
// ----------------------------------------------------------------------------

class wxUnixEventLoopSource : public wxEventLoopSource
{
public:
    // dispatcher and fdioHandler are only used here to allow us to unregister
    // from the event loop when we're destroyed
    wxUnixEventLoopSource(wxFDIODispatcher *dispatcher,
                          wxFDIOHandler *fdioHandler,
                          int fd,
                          wxEventLoopSourceHandler *handler,
                          int flags)
        : wxEventLoopSource(handler, flags),
          m_dispatcher(dispatcher),
          m_fdioHandler(fdioHandler),
          m_fd(fd)
    {
    }

    virtual ~wxUnixEventLoopSource();

private:
    wxFDIODispatcher * const m_dispatcher;
    wxFDIOHandler * const m_fdioHandler;
    const int m_fd;

    wxDECLARE_NO_COPY_CLASS(wxUnixEventLoopSource);
};

#endif // _WX_UNIX_EVTLOOPSRC_H_

