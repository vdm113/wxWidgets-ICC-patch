/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/evtloopsrc.h
// Purpose:     wxGTKEventLoopSource class
// Author:      Vadim Zeitlin
// Created:     2009-10-21
// RCS-ID:      $Id$
// Copyright:   (c) 2009 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GTK_EVTLOOPSRC_H_
#define _WX_GTK_EVTLOOPSRC_H_

// ----------------------------------------------------------------------------
// wxGTKEventLoopSource: wxEventLoopSource for GTK port
// ----------------------------------------------------------------------------

class wxGTKEventLoopSource : public wxEventLoopSource
{
public:
    // sourceId is the id of the watch in GTK context, not the FD of the file
    // this source corresponds to
    wxGTKEventLoopSource(unsigned sourceId,
                         wxEventLoopSourceHandler *handler,
                         int flags)
        : wxEventLoopSource(handler, flags),
          m_sourceId(sourceId)
    {
    }

    virtual ~wxGTKEventLoopSource();

private:
    const unsigned m_sourceId;

    wxDECLARE_NO_COPY_CLASS(wxGTKEventLoopSource);
};

#endif // _WX_GTK_EVTLOOPSRC_H_

