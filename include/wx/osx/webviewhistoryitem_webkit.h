/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        include/wx/osx/webviewhistoryitem.h
// Purpose:     wxWebViewHistoryItem header for OSX
// Author:      Steven Lamerton
// Copyright:   (c) 2011 Steven Lamerton
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_OSX_WEBVIEWHISTORYITEM_H_
#define _WX_OSX_WEBVIEWHISTORYITEM_H_

#include "wx/setup.h"

#if wxUSE_WEBVIEW && wxUSE_WEBVIEW_WEBKIT && (defined(__WXOSX_COCOA__) \
                                          ||  defined(__WXOSX_CARBON__))

#include "wx/osx/core/objcid.h"

class WXDLLIMPEXP_WEBVIEW wxWebViewHistoryItem
{
public:
    wxWebViewHistoryItem(const wxString& url, const wxString& title) :
                     m_url(url), m_title(title) {}
    wxString GetUrl() { return m_url; }
    wxString GetTitle() { return m_title; }

    friend class wxWebViewWebKit;

private:
    wxString m_url, m_title;
    wxObjCID m_histItem;
};

#endif // wxUSE_WEBVIEW && wxUSE_WEBVIEW_WEBKIT

#endif // _WX_OSX_WEBVIEWHISTORYITEM_H_
