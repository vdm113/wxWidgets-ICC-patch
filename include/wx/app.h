/////////////////////////////////////////////////////////////////////////////
// Name:        app.h
// Purpose:     wxAppBase class and macros used for declaration of wxApp
//              derived class in the user code
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_APP_H_BASE_
#define _WX_APP_H_BASE_

#ifdef __GNUG__
    #pragma interface "appbase.h"
#endif

// ----------------------------------------------------------------------------
// typedefs
// ----------------------------------------------------------------------------

#ifdef __WXMSW__
    class WXDLLEXPORT wxApp;
    typedef wxApp* (*wxAppInitializerFunction)();
#else
    // returning wxApp* won't work with gcc
    #include "wx/object.h"

    typedef wxObject* (*wxAppInitializerFunction)();
#endif

// ----------------------------------------------------------------------------
// headers we have to include here
// ----------------------------------------------------------------------------

#include "wx/event.h"       // for the base class

#include "wx/window.h"      // for wxTopLevelWindows

#if wxUSE_LOG
    #include "wx/log.h"
#endif

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

static const int wxPRINT_WINDOWS = 1;
static const int wxPRINT_POSTSCRIPT = 2;

// ----------------------------------------------------------------------------
// the common part of wxApp implementations for all platforms
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxAppBase : public wxEvtHandler
{
public:
    // the virtual functions which may/must be overridden in the derived class
    // -----------------------------------------------------------------------

        // called during the program initialization, returning FALSE from here
        // prevents the program from continuing - it's a good place to create
        // the top level program window and return TRUE.
        //
        // Override: always.
    virtual bool OnInit() { return FALSE; };

        // a platform-dependent version of OnInit(): the code here is likely to
        // depend on the toolkit. default version does nothing.
        //
        // Override: rarely.
    virtual bool OnInitGui() { return TRUE; }

        // called to start program execution - the default version just enters
        // the main GUI loop in which events are received and processed until
        // the last window is not deleted (if GetExitOnFrameDelete) or
        // ExitMainLoop() is called.
        //
        // Override: rarely.
    virtual int OnRun() { return MainLoop(); };

        // called after the main loop termination. This is a good place for
        // cleaning up (it may be too late in dtor) and is also useful if you
        // want to return some non-default exit code - this is just the return
        // value of this method.
        //
        // Override: often.
    virtual int OnExit() { return 0; }

        // called when a fatal exception occurs, this function should take care
        // not to do anything which might provoke a nested exception! It may be
        // overridden if you wish to react somehow in non-default way (core
        // dump under Unix, application crash under Windows) to fatal program
        // errors, however extreme care should be taken if you don't want this
        // function to crash.
        //
        // Override: rarely.
    virtual void OnFatalException() { }

    // the worker functions - usually not used directly by the user code
    // -----------------------------------------------------------------

        // execute the main GUI loop, the function returns when the loop ends
    virtual int MainLoop() = 0;

        // exit the main GUI loop during the next iteration (i.e. it does not
        // stop the program immediately!)
    virtual void ExitMainLoop() = 0;

        // returns TRUE if the program is initialized
    virtual bool Initialized() = 0;

        // returns TRUE if there are unprocessed events in the event queue
    virtual bool Pending() = 0;

        // process the first event in the event queue (blocks until an event
        // apperas if there are none currently)
    virtual void Dispatch() = 0;

    // application info: name, description, vendor
    // -------------------------------------------

    // NB: all these should be set by the application itself, there are no
    //     reasonable default except for the application name which is taken to
    //     be argv[0]

        // set/get the application name
    wxString GetAppName() const
    {
        if ( !m_appName )
            return m_className;
        else
            return m_appName;
    }
    void SetAppName(const wxString& name) { m_appName = name; }

        // set/get the app class name
    wxString GetClassName() const { return m_className; }
    void SetClassName(const wxString& name) { m_className = name; }

        // set/get the vendor name
    const wxString& GetVendorName() const { return m_vendorName; }
    void SetVendorName(const wxString& name) { m_vendorName = name; }

    // top level window functions
    // --------------------------

        // set the "main" top level window
    void SetTopWindow(wxWindow *win) { m_topWindow = win; }

        // return the "main" top level window (if it hadn't been set previously
        // with SetTopWindow(), will return just some top level window and, if
        // there are none, will return NULL)
    wxWindow *GetTopWindow() const
    {
        if (m_topWindow)
            return m_topWindow;
        else if (wxTopLevelWindows.GetCount() > 0)
            return wxTopLevelWindows.GetFirst()->GetData();
        else
            return (wxWindow *)NULL;
    }

        // control the exit behaviour: by default, the program will exit the
        // main loop (and so, usually, terminate) when the last top-level
        // program window is deleted. Beware that if you disabel this (with
        // SetExitOnFrameDelete(FALSE)), you'll have to call ExitMainLoop()
        // explicitly from somewhere.
    void SetExitOnFrameDelete(bool flag) { m_exitOnFrameDelete = flag; }
    bool GetExitOnFrameDelete() const { return m_exitOnFrameDelete; }

    // miscellaneous customization functions
    // -------------------------------------

#if wxUSE_LOG
        // override this function to create default log target of arbitrary
        // user-defined class (default implementation creates a wxLogGui
        // object) - this log object is used by default by all wxLogXXX()
        // functions.
    virtual wxLog *CreateLogTarget() { return new wxLogGui; }
#endif // wxUSE_LOG


        // get the standard icon used by wxWin dialogs - this allows the user
        // to customize the standard dialogs. The 'which' parameter is one of
        // wxICON_XXX values
    virtual wxIcon GetStdIcon(int which) const = 0;

        // VZ: what does this do exactly?
    void SetWantDebugOutput( bool flag ) { m_wantDebugOutput = flag; }
    bool GetWantDebugOutput() const { return m_wantDebugOutput; }

        // set/get printing mode: see wxPRINT_XXX constants.
        //
        // default behaviour is the normal one for Unix: always use PostScript
        // printing.
    virtual void SetPrintMode(int WXUNUSED(mode)) { }
    int GetPrintMode() const { return wxPRINT_POSTSCRIPT; }

    // implementation only from now on
    // -------------------------------

    // helpers for dynamic wxApp construction
    static void SetInitializerFunction(wxAppInitializerFunction fn)
        { m_appInitFn = fn; }
    static wxAppInitializerFunction GetInitializerFunction()
        { return m_appInitFn; }

    // access to the command line arguments
    int      argc;
    wxChar **argv;

//private:
protected:
    // function used for dynamic wxApp creation
    static wxAppInitializerFunction m_appInitFn;

    // application info (must be set from the user code)
    wxString m_vendorName,      // vendor name (ACME Inc)
             m_appName,         // app name
             m_className;       // class name

    // if TRUE, exit the main loop when the last top level window is deleted
    bool m_exitOnFrameDelete;

    // TRUE if the application wants to get debug output
    bool m_wantDebugOutput;

    // the main top level window - may be NULL
    wxWindow *m_topWindow;
};

// ----------------------------------------------------------------------------
// now include the declaration of the real class
// ----------------------------------------------------------------------------

#if defined(__WXMSW__)
    #include "wx/msw/app.h"
#elif defined(__WXMOTIF__)
    #include "wx/motif/app.h"
#elif defined(__WXQT__)
    #include "wx/qt/app.h"
#elif defined(__WXGTK__)
    #include "wx/gtk/app.h"
#elif defined(__WXMAC__)
    #include "wx/mac/app.h"
#elif defined(__WXSTUBS__)
    #include "wx/stubs/app.h"
#endif

// ----------------------------------------------------------------------------
// the global data
// ----------------------------------------------------------------------------

// the one and only application object - use of wxTheApp in application code
// is discouraged, consider using DECLARE_APP() after which you may call
// wxGetApp() which will return the object of the correct type (i.e. MyApp and
// not wxApp)
WXDLLEXPORT_DATA(extern wxApp*) wxTheApp;

// ----------------------------------------------------------------------------
// global functions
// ----------------------------------------------------------------------------

// Force an exit from main loop
void WXDLLEXPORT wxExit();

// Yield to other apps/messages
bool WXDLLEXPORT wxYield();

// ----------------------------------------------------------------------------
// macros for dynamic creation of the application object
// ----------------------------------------------------------------------------

// Having a global instance of this class allows wxApp to be aware of the app
// creator function. wxApp can then call this function to create a new app
// object. Convoluted, but necessary.

class WXDLLEXPORT wxAppInitializer
{
public:
    wxAppInitializer(wxAppInitializerFunction fn)
        { wxApp::SetInitializerFunction(fn); }
};

// Here's a macro you can use if your compiler really, really wants main() to
// be in your main program (e.g. hello.cpp). Now IMPLEMENT_APP should add this
// code if required.

#if defined(__AIX__) || defined(__HPUX__)
    #define IMPLEMENT_WXWIN_MAIN \
        extern int wxEntry( int argc, char *argv[] ); \
        int main(int argc, char *argv[]) { return wxEntry(argc, argv); }
#elif defined(__WXMSW__) && defined(WXUSINGDLL)
    // NT defines APIENTRY, 3.x not
    #if !defined(WXAPIENTRY)
        #ifdef __WATCOMC__
            #define WXAPIENTRY PASCAL
        #else
            #define WXAPIENTRY FAR PASCAL
        #endif
    #endif

    #define IMPLEMENT_WXWIN_MAIN \
        int WXAPIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,\
                               LPSTR m_lpCmdLine, int nCmdShow )\
        {\
            return wxEntry((WXHINSTANCE) hInstance, \
                           (WXHINSTANCE) hPrevInstance,\
                           m_lpCmdLine, nCmdShow);\
        }

#else
    #define IMPLEMENT_WXWIN_MAIN
#endif

// use this macro exactly once, the argument is the name of the wxApp-derived
// class which is the class of your application
#define IMPLEMENT_APP(appname)                          \
    wxApp *wxCreateApp() { return new appname; }      \
    wxAppInitializer wxTheAppInitializer((wxAppInitializerFunction) wxCreateApp); \
    appname& wxGetApp() { return *(appname *)wxTheApp; } \
    IMPLEMENT_WXWIN_MAIN

#define DECLARE_APP(appname) extern appname& wxGetApp();

#endif
    // _WX_APP_H_BASE_
