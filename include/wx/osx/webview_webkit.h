/////////////////////////////////////////////////////////////////////////////
// Name:        include/wx/osx/webkit.h
// Purpose:     wxWebViewWebKit - embeddable web kit control,
//                             OS X implementation of web view component
// Author:      Jethro Grassie / Kevin Ollivier / Marianne Gagnon
// Modified by:
// Created:     2004-4-16
// RCS-ID:      $Id$
// Copyright:   (c) Jethro Grassie / Kevin Ollivier / Marianne Gagnon
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WEBKIT_H
#define _WX_WEBKIT_H

#include "wx/setup.h"

#if wxUSE_WEBVIEW_WEBKIT && (defined(__WXOSX_COCOA__) \
                         ||  defined(__WXOSX_CARBON__))

#include "wx/control.h"
#include "wx/webview.h"

// ----------------------------------------------------------------------------
// Web Kit Control
// ----------------------------------------------------------------------------

class wxMacControl;
class wxWidgetCocoaImpl;

class WXDLLIMPEXP_WEB wxWebViewWebKit : public wxWebView
{
public:
    wxDECLARE_DYNAMIC_CLASS(wxWebViewWebKit);

    wxWebViewWebKit() {}
    wxWebViewWebKit(wxWindow *parent,
                    wxWindowID winID = wxID_ANY,
                    const wxString& strURL = wxWebViewDefaultURLStr,
                    const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize, long style = 0,
                    const wxString& name = wxWebViewNameStr)
    {
        Create(parent, winID, strURL, pos, size, style, name);
    }
    bool Create(wxWindow *parent,
                wxWindowID winID = wxID_ANY,
                const wxString& strURL = wxWebViewDefaultURLStr,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize, long style = 0,
                const wxString& name = wxWebViewNameStr);
    virtual ~wxWebViewWebKit();

    void InternalLoadURL(const wxString &url);

    virtual bool CanGoBack();
    virtual bool CanGoForward();
    virtual void GoBack();
    virtual void GoForward();
    virtual void Reload(wxWebViewReloadFlags flags = wxWEB_VIEW_RELOAD_DEFAULT);
    virtual void Stop();
    virtual wxString GetPageSource();
    virtual wxString GetPageText() { return ""; }
    virtual void SetPageTitle(const wxString& title) { m_pageTitle = title; }
    virtual wxString GetPageTitle(){ return m_pageTitle; }

    //We do not want to hide the other overloads
    using wxWebView::SetPage;
    virtual void SetPage(const wxString& html, const wxString& baseUrl);

    virtual void Print();

    virtual void LoadUrl(const wxString& url);
    virtual wxString GetCurrentURL();
    virtual wxString GetCurrentTitle();
    virtual wxWebViewZoom GetZoom();
    virtual void SetZoom(wxWebViewZoom zoom);

    virtual void SetZoomType(wxWebViewZoomType zoomType);
    virtual wxWebViewZoomType GetZoomType() const;
    virtual bool CanSetZoomType(wxWebViewZoomType type) const;

    virtual bool IsBusy() { return m_busy; }
    
    //History functions
    virtual void ClearHistory();
    virtual void EnableHistory(bool enable = true);
    virtual wxVector<wxSharedPtr<wxWebHistoryItem> > GetBackwardHistory()
            { return wxVector<wxSharedPtr<wxWebHistoryItem> >(); }
    virtual wxVector<wxSharedPtr<wxWebHistoryItem> > GetForwardHistory() 
            { return wxVector<wxSharedPtr<wxWebHistoryItem> >(); }
    virtual void LoadHistoryItem(wxSharedPtr<wxWebHistoryItem> WXUNUSED(item)) {}
    
    //Undo / redo functionality
    virtual bool CanUndo();
    virtual bool CanRedo();
    virtual void Undo();
    virtual void Redo();

    //Clipboard functions
    virtual bool CanCut() { return false; }
    virtual bool CanCopy() { return false; }
    virtual bool CanPaste() { return false; }
    virtual void Cut();
    virtual void Copy();
    virtual void Paste();
    
    //Editing functions
    virtual void SetEditable(bool enable = true);
    virtual bool IsEditable();
    
    //Selection
    virtual void DeleteSelection();
    virtual bool HasSelection();
    virtual void SelectAll() {};
    virtual wxString GetSelectedText();
    virtual wxString GetSelectedSource() { return ""; }
    virtual void ClearSelection() {}
    
    void RunScript(const wxString& javascript);
    
    //Virtual Filesystem Support
    virtual void RegisterHandler(wxWebHandler* WXUNUSED(handler)) {};

    // ---- methods not from the parent (common) interface
    bool  CanGetPageSource();

    void  SetScrollPos(int pos);
    int   GetScrollPos();

    bool  CanIncreaseTextSize();
    void  IncreaseTextSize();
    bool  CanDecreaseTextSize();
    void  DecreaseTextSize();

    float GetWebkitZoom();
    void  SetWebkitZoom(float zoom);

    // don't hide base class virtuals
    virtual void SetScrollPos( int orient, int pos, bool refresh = true )
        { return wxControl::SetScrollPos(orient, pos, refresh); }
    virtual int GetScrollPos( int orient ) const
        { return wxControl::GetScrollPos(orient); }

    //we need to resize the webview when the control size changes
    void OnSize(wxSizeEvent &event);
    void OnMove(wxMoveEvent &event);
    void OnMouseEvents(wxMouseEvent &event);

    bool m_busy;

protected:
    DECLARE_EVENT_TABLE()
    void MacVisibilityChanged();

private:
    wxWindow *m_parent;
    wxWindowID m_windowID;
    wxString m_pageTitle;

    struct objc_object *m_webView;

    // we may use this later to setup our own mouse events,
    // so leave it in for now.
    void* m_webKitCtrlEventHandler;
    //It should be WebView*, but WebView is an Objective-C class
    //TODO: look into using DECLARE_WXCOCOA_OBJC_CLASS rather than this.
    
#if wxOSX_USE_CARBON
    wxMacControl *m_peer;
#else
    wxWidgetCocoaImpl *m_peer;
#endif
};

#endif // wxUSE_WEBKIT

#endif // _WX_WEBKIT_H_
