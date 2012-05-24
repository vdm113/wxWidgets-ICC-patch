/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        splash.h
// Purpose:     interface of wxSplashScreen
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxSplashScreen

    wxSplashScreen shows a window with a thin border, displaying a bitmap
    describing your application.

    Show it in application initialisation, and then either explicitly destroy
    it or let it time-out.

    Example usage:

    @code
      wxBitmap bitmap;
      if (bitmap.LoadFile("splash16.png", wxBITMAP_TYPE_PNG))
      {
          wxSplashScreen* splash = new wxSplashScreen(bitmap,
              wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT,
              6000, NULL, -1, wxDefaultPosition, wxDefaultSize,
              wxBORDER_SIMPLE|wxSTAY_ON_TOP);
      }
      wxYield();
    @endcode

    @library{wxadv}
    @category{managedwnd}
*/
class wxSplashScreen : public wxFrame
{
public:
    /**
        Construct the splash screen passing a bitmap, a style, a timeout, a window id,
        optional position and size, and a window style.

        @a splashStyle is a bitlist of some of the following:
        - wxSPLASH_CENTRE_ON_PARENT
        - wxSPLASH_CENTRE_ON_SCREEN
        - wxSPLASH_NO_CENTRE
        - wxSPLASH_TIMEOUT
        - wxSPLASH_NO_TIMEOUT

        @a milliseconds is the timeout in milliseconds.
    */
    wxSplashScreen(const wxBitmap& bitmap, long splashStyle,
                   int milliseconds,
                   wxWindow* parent,
                   wxWindowID id,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   long style = wxBORDER_SIMPLE|wxFRAME_NO_TASKBAR|wxSTAY_ON_TOP);

    /**
        Destroys the splash screen.
    */
    virtual ~wxSplashScreen();

    /**
        Returns the splash style (see wxSplashScreen() for details).
    */
    long GetSplashStyle() const;

    /**
        Returns the window used to display the bitmap.
    */
    wxSplashScreenWindow* GetSplashWindow() const;

    /**
        Returns the timeout in milliseconds.
    */
    int GetTimeout() const;

    /**
        Reimplement this event handler if you want to set an application variable on
        window destruction, for example.
    */
    void OnCloseWindow(wxCloseEvent& event);
};

