/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
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

#include "wx/defs.h"

#if wxUSE_WEBVIEW && wxUSE_WEBVIEW_WEBKIT && defined(__WXOSX__)

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
