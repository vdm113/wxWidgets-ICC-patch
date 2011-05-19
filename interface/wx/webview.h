/////////////////////////////////////////////////////////////////////////////
// Name:        webview.h
// Purpose:     interface of wxWebView
// Author:      wxWidgets team
// RCS-ID:      $Id:$
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
    Zoom levels availiable in wxWebView
*/
enum wxWebViewZoom
{
    wxWEB_VIEW_ZOOM_TINY,
    wxWEB_VIEW_ZOOM_SMALL,
    wxWEB_VIEW_ZOOM_MEDIUM, //!< default size
    wxWEB_VIEW_ZOOM_LARGE,
    wxWEB_VIEW_ZOOM_LARGEST
};

/**
    The type of zooming that the web view control can perform
*/
enum wxWebViewZoomType
{
    /** 
        The entire layout scales when zooming, including images 
    */
    wxWEB_VIEW_ZOOM_TYPE_LAYOUT,
    /** 
        Only the text changes in size when zooming, images and other layout
        elements retain their initial size 
    */
    wxWEB_VIEW_ZOOM_TYPE_TEXT
};

/** 
    Types of errors that can cause navigation to fail
*/
enum wxWebNavigationError
{
    /** Connection error (timeout, etc.) */
    wxWEB_NAV_ERR_CONNECTION,
    /** Invalid certificate */
    wxWEB_NAV_ERR_CERTIFICATE,
    /** Authentication required */
    wxWEB_NAV_ERR_AUTH,
    /** Other security error */
    wxWEB_NAV_ERR_SECURITY,
    /** Requested resource not found */
    wxWEB_NAV_ERR_NOT_FOUND,
    /** Invalid request/parameters (e.g. bad URL, bad protocol,
        unsupported resource type) */
    wxWEB_NAV_ERR_REQUEST,
    /** The user cancelled (e.g. in a dialog) */
    wxWEB_NAV_ERR_USER_CANCELLED,
    /** Another (exotic) type of error that didn't fit in other categories*/
    wxWEB_NAV_ERR_OTHER
};

/** 
    Type of refresh 
*/
enum wxWebViewReloadFlags
{
    /** Default reload, will access cache */
    wxWEB_VIEW_RELOAD_DEFAULT,
    /** Reload the current view without accessing the cache */
    wxWEB_VIEW_RELOAD_NO_CACHE 
};


/**
 * List of available backends for wxWebView
 */
enum wxWebViewBackend
{
    /** Value that may be passed to wxWebView to let it pick an appropriate
     * engine for the current platform*/
    wxWEB_VIEW_BACKEND_DEFAULT,

    /** The OSX-native WebKit web engine */
    wxWEB_VIEW_BACKEND_OSX_WEBKIT,

    /** The GTK port of the WebKit engine */
    wxWEB_VIEW_BACKEND_GTK_WEBKIT,

    /** Use Microsoft Internet Explorer as web engine */
    wxWEB_VIEW_BACKEND_IE
};

/**
    @class wxWebView
  
    This control may be used to render web (HTML / CSS / javascript) documents.
    Capabilities of the HTML renderer will depend upon the backed.
    TODO: describe each backend and its capabilities here
  
    Note that errors are generally reported asynchronously though the
    @c wxEVT_COMMAND_WEB_VIEW_ERROR event described below.
  
    @beginEventEmissionTable{wxWebNavigationEvent}
    @event{EVT_WEB_VIEW_NAVIGATING(id, func)}
       Process a @c wxEVT_COMMAND_WEB_VIEW_NAVIGATING event, generated before trying
       to get a resource. This event may be vetoed to prevent navigating to this
       resource. Note that if the displayed HTML document has several frames, one
       such event will be generated per frame.
    @event{EVT_WEB_VIEW_NAVIGATED(id, func)}
       Process a @c wxEVT_COMMAND_WEB_VIEW_NAVIGATED event generated after it was
       confirmed that a resource would be requested. This event may not be vetoed.
       Note that if the displayed HTML document has several frames, one such event
       will be generated per frame.
    @event{EVT_WEB_VIEW_LOADED(id, func)}
       Process a @c wxEVT_COMMAND_WEB_VIEW_LOADED event generated when the document
       is fully loaded and displayed.
    @event{EVT_WEB_VIEW_ERRROR(id, func)}
       Process a @c wxEVT_COMMAND_WEB_VIEW_ERROR event generated when a navigation
       error occurs.
       The integer associated with this event will be a wxWebNavigationError item.
       The string associated with this event may contain a backend-specific more
       precise error message/code.
    @endEventTable
   
    @library{wxweb}
    @category{ctrl}
 */
class wxWebView : public wxControl
{
public:

    /**
        Creation function for two-step creation.
    */
    virtual bool Create(wxWindow* parent,
                        wxWindowID id,
                        const wxString& url,
                        const wxPoint& pos,
                        const wxSize& size,
                        long style,
                        const wxString& name) = 0;

    /**
        Factory function to create a new wxWebView for two-step creation
        (you need to call wxWebView::Create on the returned object)
        @param backend which web engine to use as backend for wxWebView
        @return the created wxWebView, or NULL if the requested backend is
                not available
     */
    static wxWebView* New(wxWebViewBackend backend = wxWEB_VIEW_BACKEND_DEFAULT);

    /**
        Factory function to create a new wxWebView
        @param parent parent window to create this view in
        @param id ID of this control
        @param url URL to load by default in the web view
        @param pos position to create this control at
               (you may use wxDefaultPosition if you use sizers)
        @param size size to create this control with
               (you may use wxDefaultSize if you use sizers)
        @param backend which web engine to use as backend for wxWebView
        @return the created wxWebView, or NULL if the requested backend
                is not available
    */
    static wxWebView* New(wxWindow* parent,
           wxWindowID id,
           const wxString& url = wxWebViewDefaultURLStr,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           wxWebViewBackend backend = wxWEB_VIEW_BACKEND_DEFAULT,
           long style = 0,
           const wxString& name = wxWebViewNameStr);


    /** 
        Get whether it is possible to navigate back in the history of
        visited pages
    */
    virtual bool CanGoBack() = 0;

    /** 
        Get whether it is possible to navigate forward in the history of
        visited pages
    */
    virtual bool CanGoForward() = 0;

    /** 
        Navigate back in the history of visited pages.
        Only valid if CanGoBack() returned true.
    */
    virtual void GoBack() = 0;

    /**
        Navigate forwardin the history of visited pages.
        Only valid if CanGoForward() returned true.
    */
    virtual void GoForward() = 0;

    /**
        Load a HTMl document (web page) from a URL
        @param url the URL where the HTML document to display can be found
        @note web engines generally report errors asynchronously, so if you wish
            to know whether loading the URL was successful, register to receive
            navigation error events
    */
    virtual void LoadUrl(const wxString& url) = 0;

    /**
        Stop the current page loading process, if any.
        May trigger an error event of type @c wxWEB_NAV_ERR_USER_CANCELLED.
        TODO: make @c wxWEB_NAV_ERR_USER_CANCELLED errors uniform across ports.
    */
    virtual void Stop() = 0;

    /**
        Reload the currently displayed URL.
        @param flags A bit array that may optionnally contain reload options
    */
    virtual void Reload(wxWebViewReloadFlags flags = wxWEB_VIEW_RELOAD_DEFAULT) = 0;


    /**
        Get the URL of the currently displayed document
    */
    virtual wxString GetCurrentURL() = 0;

    /**
        Get the title of the current web page, or its URL/path if title is not
        available
    */
    virtual wxString GetCurrentTitle() = 0;

    /**
        Get the HTML source code of the currently displayed document
        @return the HTML source code, or an empty string if no page is currently
                shown
    */
    virtual wxString GetPageSource() = 0;

    /**
        Get the zoom factor of the page
        @return How much the HTML document is zoomed (scaleed)
    */
    virtual wxWebViewZoom GetZoom() = 0;

    /**
        Set the zoom factor of the page
        @param zoom How much to zoom (scale) the HTML document
    */
    virtual void SetZoom(wxWebViewZoom zoom) = 0;

    /**
        Set how to interpret the zoom factor
        @param zoomType how the zoom factor should be interpreted by the
                        HTML engine
        @note invoke    canSetZoomType() first, some HTML renderers may not
                        support all zoom types
    */
    virtual void SetZoomType(wxWebViewZoomType zoomType) = 0;

    /**
        Get how the zoom factor is currently interpreted
        @return how the zoom factor is currently interpreted by the HTML engine
    */
    virtual wxWebViewZoomType GetZoomType() const = 0;

    /**
        Retrieve whether the current HTML engine supports a type of zoom
        @param type the type of zoom to test
        @return whether this type of zoom is supported by this HTML engine
                (and thus can be set through setZoomType())
    */
    virtual bool CanSetZoomType(wxWebViewZoomType type) const = 0;

    /**
        Set the displayed page source to the contents of the given string
        @param html    the string that contains the HTML data to display
        @param baseUrl URL assigned to the HTML data, to be used to resolve
                    relative paths, for instance
    */
    virtual void SetPage(const wxString& html, const wxString& baseUrl) = 0;

    /**
        Set the displayed page source to the contents of the given stream
        @param html    the stream to read HTML data from
        @param baseUrl URL assigned to the HTML data, to be used to resolve
                    relative paths, for instance
    */
    virtual void SetPage(wxInputStream& html, wxString baseUrl)
    {
        wxStringOutputStream stream;
        stream.Write(html);
        SetPage(stream.GetString(), baseUrl);
    }

    /**
        Opens a print dialog so that the user may print the currently
        displayed page.
    */
    virtual void Print() = 0;

    /**
        Returns whether the web control is currently busy (e.g. loading a page)
    */
    virtual bool IsBusy() = 0;
};




/**
    @class wxWebNavigationEvent

    A navigation  event holds information about events associated with 
    wxWebView objects.

    @beginEventEmissionTable{wxWebNavigationEvent}
    @event{EVT_WEB_VIEW_NAVIGATING(id, func)}
       Process a @c wxEVT_COMMAND_WEB_VIEW_NAVIGATING event, generated before trying
       to get a resource. This event may be vetoed to prevent navigating to this
       resource. Note that if the displayed HTML document has several frames, one
       such event will be generated per frame.
    @event{EVT_WEB_VIEW_NAVIGATED(id, func)}
       Process a @c wxEVT_COMMAND_WEB_VIEW_NAVIGATED event generated after it was
       confirmed that a resource would be requested. This event may not be vetoed.
       Note that if the displayed HTML document has several frames, one such event
       will be generated per frame.
    @event{EVT_WEB_VIEW_LOADED(id, func)}
       Process a @c wxEVT_COMMAND_WEB_VIEW_LOADED event generated when the document
       is fully loaded and displayed.
    @event{EVT_WEB_VIEW_ERRROR(id, func)}
       Process a @c wxEVT_COMMAND_WEB_VIEW_ERROR event generated when a navigation
       error occurs.
       The integer associated with this event will be a wxWebNavigationError item.
       The string associated with this event may contain a backend-specific more
       precise error message/code.
    @endEventTable

    @library{wxweb}
    @category{events}

    @see wxWebView
*/
class wxWebNavigationEvent : public wxCommandEvent
{
public:
    wxWebNavigationEvent();
    wxWebNavigationEvent(wxEventType type, int id, const wxString href,
                         const wxString target, bool canVeto);
    /**
        Get the URL being visited
    */
    const wxString& GetHref() const { return m_href; }

    /**
        Get the target (frame or window) in which the URL that caused this event
        is viewed, or an empty string if not available.
    */
    const wxString& GetTarget() const;

    // default copy ctor, assignment operator and dtor are ok
    virtual wxEvent* Clone() const;

    /** 
        Get whether this event may be vetoed (stopped/prevented). Only
        meaningful for events fired before navigation takes place.
     */
    bool CanVeto() const;

    /** 
        Whether this event was vetoed (stopped/prevented). Only meaningful for
        events fired before navigation takes place.
     */
    bool IsVetoed() const;

    /** 
        Veto (prevent/stop) this event. Only meaningful for events fired
        before navigation takes place. Only valid if CanVeto() returned true.
     */
    void Veto();
};