/////////////////////////////////////////////////////////////////////////////
// Name:        wx/utils.h
// Purpose:     Miscellaneous utilities
// Author:      Julian Smart
// Modified by:
// Created:     29/01/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UTILS_H_
#define _WX_UTILS_H_

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/object.h"
#include "wx/list.h"
#include "wx/filefn.h"
#if wxUSE_GUI
    #include "wx/gdicmn.h"
#endif

class WXDLLIMPEXP_FWD_BASE wxArrayString;
class WXDLLIMPEXP_FWD_BASE wxArrayInt;

// need this for wxGetDiskSpace() as we can't, unfortunately, forward declare
// wxLongLong
#include "wx/longlong.h"

// need for wxOperatingSystemId
#include "wx/platinfo.h"

#ifdef __WATCOMC__
    #include <direct.h>
#elif defined(__X__)
    #include <dirent.h>
    #include <unistd.h>
#endif

#include <stdio.h>

// ----------------------------------------------------------------------------
// Forward declaration
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_FWD_BASE wxProcess;
class WXDLLIMPEXP_FWD_CORE wxFrame;
class WXDLLIMPEXP_FWD_CORE wxWindow;
class WXDLLIMPEXP_FWD_CORE wxWindowList;

// ----------------------------------------------------------------------------
// Macros
// ----------------------------------------------------------------------------

#define wxMax(a,b)            (((a) > (b)) ? (a) : (b))
#define wxMin(a,b)            (((a) < (b)) ? (a) : (b))
#define wxClip(a,b,c)         (((a) < (b)) ? (b) : (((a) > (c)) ? (c) : (a)))

// wxGetFreeMemory can return huge amount of memory on 32-bit platforms as well
// so to always use long long for its result type on all platforms which
// support it
#if wxUSE_LONGLONG
    typedef wxLongLong wxMemorySize;
#else
    typedef long wxMemorySize;
#endif

// ----------------------------------------------------------------------------
// String functions (deprecated, use wxString)
// ----------------------------------------------------------------------------

// A shorter way of using strcmp
#define wxStringEq(s1, s2) (s1 && s2 && (wxStrcmp(s1, s2) == 0))

// ----------------------------------------------------------------------------
// Miscellaneous functions
// ----------------------------------------------------------------------------

// Sound the bell
#if !defined __EMX__ && \
    (defined __WXMOTIF__ || defined __WXGTK__ || defined __WXX11__)
WXDLLIMPEXP_CORE void wxBell();
#else
WXDLLIMPEXP_BASE void wxBell();
#endif

#if wxUSE_MSGDLG
// Show wxWidgets information
WXDLLIMPEXP_CORE void wxInfoMessageBox(wxWindow* parent);
#endif // wxUSE_MSGDLG

// Get OS description as a user-readable string
WXDLLIMPEXP_BASE wxString wxGetOsDescription();

// Get OS version
WXDLLIMPEXP_BASE wxOperatingSystemId wxGetOsVersion(int *majorVsn = (int *) NULL,
                                                    int *minorVsn = (int *) NULL);

// Get platform endianness
WXDLLIMPEXP_BASE bool wxIsPlatformLittleEndian();

// Get platform architecture
WXDLLIMPEXP_BASE bool wxIsPlatform64Bit();

// Return a string with the current date/time
WXDLLIMPEXP_BASE wxString wxNow();

// Return path where wxWidgets is installed (mostly useful in Unices)
WXDLLIMPEXP_BASE const wxChar *wxGetInstallPrefix();
// Return path to wxWin data (/usr/share/wx/%{version}) (Unices)
WXDLLIMPEXP_BASE wxString wxGetDataDir();

/*
 * Class to make it easier to specify platform-dependent values
 *
 * Examples:
 *  long val = wxPlatform::If(wxMac, 1).ElseIf(wxGTK, 2).ElseIf(stPDA, 5).Else(3);
 *  wxString strVal = wxPlatform::If(wxMac, wxT("Mac")).ElseIf(wxMSW, wxT("MSW")).Else(wxT("Other"));
 *
 * A custom platform symbol:
 *
 *  #define stPDA 100
 *  #ifdef __WXWINCE__
 *      wxPlatform::AddPlatform(stPDA);
 *  #endif
 *
 *  long windowStyle = wxCAPTION | (long) wxPlatform::IfNot(stPDA, wxRESIZE_BORDER);
 *
 */

class WXDLLIMPEXP_BASE wxPlatform
{
public:
    wxPlatform() { Init(); }
    wxPlatform(const wxPlatform& platform) { Copy(platform); }
    void operator = (const wxPlatform& platform) { if (&platform != this) Copy(platform); }
    void Copy(const wxPlatform& platform);

    // Specify an optional default value
    wxPlatform(int defValue) { Init(); m_longValue = (long)defValue; }
    wxPlatform(long defValue) { Init(); m_longValue = defValue; }
    wxPlatform(const wxString& defValue) { Init(); m_stringValue = defValue; }
    wxPlatform(double defValue) { Init(); m_doubleValue = defValue; }

    static wxPlatform If(int platform, long value);
    static wxPlatform IfNot(int platform, long value);
    wxPlatform& ElseIf(int platform, long value);
    wxPlatform& ElseIfNot(int platform, long value);
    wxPlatform& Else(long value);

    static wxPlatform If(int platform, int value) { return If(platform, (long)value); }
    static wxPlatform IfNot(int platform, int value) { return IfNot(platform, (long)value); }
    wxPlatform& ElseIf(int platform, int value) { return ElseIf(platform, (long) value); }
    wxPlatform& ElseIfNot(int platform, int value) { return ElseIfNot(platform, (long) value); }
    wxPlatform& Else(int value) { return Else((long) value); }

    static wxPlatform If(int platform, double value);
    static wxPlatform IfNot(int platform, double value);
    wxPlatform& ElseIf(int platform, double value);
    wxPlatform& ElseIfNot(int platform, double value);
    wxPlatform& Else(double value);

    static wxPlatform If(int platform, const wxString& value);
    static wxPlatform IfNot(int platform, const wxString& value);
    wxPlatform& ElseIf(int platform, const wxString& value);
    wxPlatform& ElseIfNot(int platform, const wxString& value);
    wxPlatform& Else(const wxString& value);

    long GetInteger() const { return m_longValue; }
    const wxString& GetString() const { return m_stringValue; }
    double GetDouble() const { return m_doubleValue; }

    operator int() const { return (int) GetInteger(); }
    operator long() const { return GetInteger(); }
    operator double() const { return GetDouble(); }
    operator const wxString&() const { return GetString(); }

    static void AddPlatform(int platform);
    static bool Is(int platform);
    static void ClearPlatforms();

private:

    void Init() { m_longValue = 0; m_doubleValue = 0.0; }

    long                m_longValue;
    double              m_doubleValue;
    wxString            m_stringValue;
    static wxArrayInt*  sm_customPlatforms;
};

/// Function for testing current platform
inline bool wxPlatformIs(int platform) { return wxPlatform::Is(platform); }

#if wxUSE_GUI

// Get the state of a key (true if pressed, false if not)
// This is generally most useful getting the state of
// the modifier or toggle keys.
WXDLLIMPEXP_CORE bool wxGetKeyState(wxKeyCode key);


// Don't synthesize KeyUp events holding down a key and producing
// KeyDown events with autorepeat. On by default and always on
// in wxMSW.
WXDLLIMPEXP_CORE bool wxSetDetectableAutoRepeat( bool flag );


// wxMouseState is used to hold information about button and modifier state
// and is what is returned from wxGetMouseState.
class WXDLLIMPEXP_CORE wxMouseState
{
public:
    wxMouseState()
        : m_x(0), m_y(0),
          m_leftDown(false), m_middleDown(false), m_rightDown(false),
          m_aux1Down(false), m_aux2Down(false),
          m_controlDown(false), m_shiftDown(false), m_altDown(false),
          m_metaDown(false)
    {}

    wxCoord GetX() const { return m_x; }
    wxCoord GetY() const { return m_y; }
    wxPoint GetPosition() const { return wxPoint(m_x, m_y); }

    bool LeftDown()    const { return m_leftDown; }
    bool MiddleDown()  const { return m_middleDown; }
    bool RightDown()   const { return m_rightDown; }
    bool Aux1Down()    const { return m_aux1Down; }
    bool Aux2Down()    const { return m_aux2Down; }

    bool ControlDown() const { return m_controlDown; }
    bool ShiftDown()   const { return m_shiftDown; }
    bool AltDown()     const { return m_altDown; }
    bool MetaDown()    const { return m_metaDown; }
    bool CmdDown() const
    {
#if defined(__WXMAC__) || defined(__WXCOCOA__)
        return MetaDown();
#else
        return ControlDown();
#endif
    }

    void SetX(wxCoord x) { m_x = x; }
    void SetY(wxCoord y) { m_y = y; }

    void SetLeftDown(bool down)   { m_leftDown = down; }
    void SetMiddleDown(bool down) { m_middleDown = down; }
    void SetRightDown(bool down)  { m_rightDown = down; }
    void SetAux1Down(bool down)   { m_aux1Down = down; }
    void SetAux2Down(bool down)   { m_aux2Down = down; }

    void SetControlDown(bool down) { m_controlDown = down; }
    void SetShiftDown(bool down)   { m_shiftDown = down; }
    void SetAltDown(bool down)     { m_altDown = down; }
    void SetMetaDown(bool down)    { m_metaDown = down; }

private:
    wxCoord m_x,
            m_y;

    bool m_leftDown     : 1;
    bool m_middleDown   : 1;
    bool m_rightDown    : 1;
    bool m_aux1Down     : 1;
    bool m_aux2Down     : 1;

    bool m_controlDown  : 1;
    bool m_shiftDown    : 1;
    bool m_altDown      : 1;
    bool m_metaDown     : 1;
};


// Returns the current state of the mouse position, buttons and modifers
WXDLLIMPEXP_CORE wxMouseState wxGetMouseState();

#endif // wxUSE_GUI

// ----------------------------------------------------------------------------
// Window ID management
// ----------------------------------------------------------------------------

// Ensure subsequent IDs don't clash with this one
WXDLLIMPEXP_BASE void wxRegisterId(long id);

// Return the current ID
WXDLLIMPEXP_BASE long wxGetCurrentId();

// Generate a unique ID
WXDLLIMPEXP_BASE long wxNewId();

// ----------------------------------------------------------------------------
// Various conversions
// ----------------------------------------------------------------------------

// Convert 2-digit hex number to decimal
WXDLLIMPEXP_BASE int wxHexToDec(const wxString& buf);

// Convert decimal integer to 2-character hex string
WXDLLIMPEXP_BASE void wxDecToHex(int dec, wxChar *buf);
WXDLLIMPEXP_BASE void wxDecToHex(int dec, char* ch1, char* ch2);
WXDLLIMPEXP_BASE wxString wxDecToHex(int dec);

// ----------------------------------------------------------------------------
// Process management
// ----------------------------------------------------------------------------

// NB: for backwards compatibility reasons the values of wxEXEC_[A]SYNC *must*
//     be 0 and 1, don't change!

enum
{
    // execute the process asynchronously
    wxEXEC_ASYNC    = 0,

    // execute it synchronously, i.e. wait until it finishes
    wxEXEC_SYNC     = 1,

    // under Windows, don't hide the child even if it's IO is redirected (this
    // is done by default)
    wxEXEC_NOHIDE   = 2,

    // under Unix, if the process is the group leader then passing wxKILL_CHILDREN to wxKill
    // kills all children as well as pid
    wxEXEC_MAKE_GROUP_LEADER = 4,

    // by default synchronous execution disables all program windows to avoid
    // that the user interacts with the program while the child process is
    // running, you can use this flag to prevent this from happening
    wxEXEC_NODISABLE = 8,

    // by default, the event loop is run while waiting for synchronous execution
    // to complete and this flag can be used to simply block the main process
    // until the child process finishes
    wxEXEC_NOEVENTS = 16,

    // convenient synonym for flags given system()-like behaviour
    wxEXEC_BLOCK = wxEXEC_SYNC | wxEXEC_NOEVENTS
};

// Execute another program.
//
// If flags contain wxEXEC_SYNC, return -1 on failure and the exit code of the
// process if everything was ok. Otherwise (i.e. if wxEXEC_ASYNC), return 0 on
// failure and the PID of the launched process if ok.
WXDLLIMPEXP_BASE long wxExecute(const wxString& command,
                                int flags = wxEXEC_ASYNC,
                                wxProcess *process = NULL);
WXDLLIMPEXP_BASE long wxExecute(char **argv,
                                int flags = wxEXEC_ASYNC,
                                wxProcess *process = NULL);
#if wxUSE_UNICODE
WXDLLIMPEXP_BASE long wxExecute(wchar_t **argv,
                                int flags = wxEXEC_ASYNC,
                                wxProcess *process = NULL);
#endif // wxUSE_UNICODE

// execute the command capturing its output into an array line by line, this is
// always synchronous
WXDLLIMPEXP_BASE long wxExecute(const wxString& command,
                                wxArrayString& output,
                                int flags = 0);

// also capture stderr (also synchronous)
WXDLLIMPEXP_BASE long wxExecute(const wxString& command,
                                wxArrayString& output,
                                wxArrayString& error,
                                int flags = 0);

#if defined(__WXMSW__) && wxUSE_IPC
// ask a DDE server to execute the DDE request with given parameters
WXDLLIMPEXP_BASE bool wxExecuteDDE(const wxString& ddeServer,
                                   const wxString& ddeTopic,
                                   const wxString& ddeCommand);
#endif // __WXMSW__ && wxUSE_IPC

enum wxSignal
{
    wxSIGNONE = 0,  // verify if the process exists under Unix
    wxSIGHUP,
    wxSIGINT,
    wxSIGQUIT,
    wxSIGILL,
    wxSIGTRAP,
    wxSIGABRT,
    wxSIGIOT = wxSIGABRT,   // another name
    wxSIGEMT,
    wxSIGFPE,
    wxSIGKILL,
    wxSIGBUS,
    wxSIGSEGV,
    wxSIGSYS,
    wxSIGPIPE,
    wxSIGALRM,
    wxSIGTERM

    // further signals are different in meaning between different Unix systems
};

enum wxKillError
{
    wxKILL_OK,              // no error
    wxKILL_BAD_SIGNAL,      // no such signal
    wxKILL_ACCESS_DENIED,   // permission denied
    wxKILL_NO_PROCESS,      // no such process
    wxKILL_ERROR            // another, unspecified error
};

enum wxKillFlags
{
    wxKILL_NOCHILDREN = 0,  // don't kill children
    wxKILL_CHILDREN = 1     // kill children
};

enum wxShutdownFlags
{
    wxSHUTDOWN_POWEROFF,    // power off the computer
    wxSHUTDOWN_REBOOT       // shutdown and reboot
};

// Shutdown or reboot the PC
WXDLLIMPEXP_BASE bool wxShutdown(wxShutdownFlags wFlags);

// send the given signal to the process (only NONE and KILL are supported under
// Windows, all others mean TERM), return 0 if ok and -1 on error
//
// return detailed error in rc if not NULL
WXDLLIMPEXP_BASE int wxKill(long pid,
                       wxSignal sig = wxSIGTERM,
                       wxKillError *rc = NULL,
                       int flags = wxKILL_NOCHILDREN);

// Execute a command in an interactive shell window (always synchronously)
// If no command then just the shell
WXDLLIMPEXP_BASE bool wxShell(const wxString& command = wxEmptyString);

// As wxShell(), but must give a (non interactive) command and its output will
// be returned in output array
WXDLLIMPEXP_BASE bool wxShell(const wxString& command, wxArrayString& output);

// Sleep for nSecs seconds
WXDLLIMPEXP_BASE void wxSleep(int nSecs);

// Sleep for a given amount of milliseconds
WXDLLIMPEXP_BASE void wxMilliSleep(unsigned long milliseconds);

// Sleep for a given amount of microseconds
WXDLLIMPEXP_BASE void wxMicroSleep(unsigned long microseconds);

// Sleep for a given amount of milliseconds (old, bad name), use wxMilliSleep
wxDEPRECATED( WXDLLIMPEXP_BASE void wxUsleep(unsigned long milliseconds) );

// Get the process id of the current process
WXDLLIMPEXP_BASE unsigned long wxGetProcessId();

// Get free memory in bytes, or -1 if cannot determine amount (e.g. on UNIX)
WXDLLIMPEXP_BASE wxMemorySize wxGetFreeMemory();

#if wxUSE_ON_FATAL_EXCEPTION

// should wxApp::OnFatalException() be called?
WXDLLIMPEXP_BASE bool wxHandleFatalExceptions(bool doit = true);

#endif // wxUSE_ON_FATAL_EXCEPTION

// ----------------------------------------------------------------------------
// Environment variables
// ----------------------------------------------------------------------------

// returns true if variable exists (value may be NULL if you just want to check
// for this)
WXDLLIMPEXP_BASE bool wxGetEnv(const wxString& var, wxString *value);

// set the env var name to the given value, return true on success
WXDLLIMPEXP_BASE bool wxSetEnv(const wxString& var, const wxString& value);

// remove the env var from environment
WXDLLIMPEXP_BASE bool wxUnsetEnv(const wxString& var);

#if WXWIN_COMPATIBILITY_2_8
inline bool wxSetEnv(const wxString& var, const char *value)
    { return wxSetEnv(var, wxString(value)); }
inline bool wxSetEnv(const wxString& var, const wchar_t *value)
    { return wxSetEnv(var, wxString(value)); }
template<typename T>
inline bool wxSetEnv(const wxString& var, const wxCharTypeBuffer<T>& value)
    { return wxSetEnv(var, wxString(value)); }
inline bool wxSetEnv(const wxString& var, const wxCStrData& value)
    { return wxSetEnv(var, wxString(value)); }

// this one is for passing NULL directly - don't use it, use wxUnsetEnv instead
wxDEPRECATED( inline bool wxSetEnv(const wxString& var, int value) );
inline bool wxSetEnv(const wxString& var, int value)
{
    wxASSERT_MSG( value == 0, "using non-NULL integer as string?" );

    wxUnusedVar(value); // fix unused parameter warning in release build

    return wxUnsetEnv(var);
}
#endif // WXWIN_COMPATIBILITY_2_8

// ----------------------------------------------------------------------------
// Network and username functions.
// ----------------------------------------------------------------------------

// NB: "char *" functions are deprecated, use wxString ones!

// Get eMail address
WXDLLIMPEXP_BASE bool wxGetEmailAddress(wxChar *buf, int maxSize);
WXDLLIMPEXP_BASE wxString wxGetEmailAddress();

// Get hostname.
WXDLLIMPEXP_BASE bool wxGetHostName(wxChar *buf, int maxSize);
WXDLLIMPEXP_BASE wxString wxGetHostName();

// Get FQDN
WXDLLIMPEXP_BASE wxString wxGetFullHostName();
WXDLLIMPEXP_BASE bool wxGetFullHostName(wxChar *buf, int maxSize);

// Get user ID e.g. jacs (this is known as login name under Unix)
WXDLLIMPEXP_BASE bool wxGetUserId(wxChar *buf, int maxSize);
WXDLLIMPEXP_BASE wxString wxGetUserId();

// Get user name e.g. Julian Smart
WXDLLIMPEXP_BASE bool wxGetUserName(wxChar *buf, int maxSize);
WXDLLIMPEXP_BASE wxString wxGetUserName();

// Get current Home dir and copy to dest (returns pstr->c_str())
WXDLLIMPEXP_BASE wxString wxGetHomeDir();
WXDLLIMPEXP_BASE const wxChar* wxGetHomeDir(wxString *pstr);

// Get the user's (by default use the current user name) home dir,
// return empty string on error
WXDLLIMPEXP_BASE wxString wxGetUserHome(const wxString& user = wxEmptyString);


#if wxUSE_LONGLONG
    typedef wxLongLong wxDiskspaceSize_t;
#else
    typedef long wxDiskspaceSize_t;
#endif

// get number of total/free bytes on the disk where path belongs
WXDLLIMPEXP_BASE bool wxGetDiskSpace(const wxString& path,
                                     wxDiskspaceSize_t *pTotal = NULL,
                                     wxDiskspaceSize_t *pFree = NULL);



extern "C"
{
typedef int (wxCMPFUNC_CONV *CMPFUNCDATA)(const void* pItem1, const void* pItem2, const void* user_data);
}


WXDLLIMPEXP_BASE void wxQsort(void *const pbase, size_t total_elems,
                              size_t size, CMPFUNCDATA cmp, const void* user_data);


#if wxUSE_GUI // GUI only things from now on

// ----------------------------------------------------------------------------
// Launch default browser
// ----------------------------------------------------------------------------

// flags for wxLaunchDefaultBrowser
enum
{
    wxBROWSER_NEW_WINDOW   = 0x01,
    wxBROWSER_NOBUSYCURSOR = 0x02
};

// Launch url in the user's default internet browser
WXDLLIMPEXP_CORE bool wxLaunchDefaultBrowser(const wxString& url, int flags = 0);

// Launch document in the user's default application
WXDLLIMPEXP_CORE bool wxLaunchDefaultApplication(const wxString& path, int flags = 0);

// ----------------------------------------------------------------------------
// Menu accelerators related things
// ----------------------------------------------------------------------------

// flags for wxStripMenuCodes
enum
{
    // strip '&' characters
    wxStrip_Mnemonics = 1,

    // strip everything after '\t'
    wxStrip_Accel = 2,

    // strip everything (this is the default)
    wxStrip_All = wxStrip_Mnemonics | wxStrip_Accel
};

// strip mnemonics and/or accelerators from the label
WXDLLIMPEXP_CORE wxString
wxStripMenuCodes(const wxString& str, int flags = wxStrip_All);

#if WXWIN_COMPATIBILITY_2_6
// obsolete and deprecated version, do not use, use the above overload instead
wxDEPRECATED(
    WXDLLIMPEXP_CORE wxChar* wxStripMenuCodes(const wxChar *in, wxChar *out = NULL)
);

#if wxUSE_ACCEL
class WXDLLIMPEXP_FWD_CORE wxAcceleratorEntry;

// use wxAcceleratorEntry::Create() or FromString() methods instead
wxDEPRECATED(
    WXDLLIMPEXP_CORE wxAcceleratorEntry *wxGetAccelFromString(const wxString& label)
);
#endif // wxUSE_ACCEL

#endif // WXWIN_COMPATIBILITY_2_6

// ----------------------------------------------------------------------------
// Window search
// ----------------------------------------------------------------------------

// Returns menu item id or wxNOT_FOUND if none.
WXDLLIMPEXP_CORE int wxFindMenuItemId(wxFrame *frame, const wxString& menuString, const wxString& itemString);

// Find the wxWindow at the given point. wxGenericFindWindowAtPoint
// is always present but may be less reliable than a native version.
WXDLLIMPEXP_CORE wxWindow* wxGenericFindWindowAtPoint(const wxPoint& pt);
WXDLLIMPEXP_CORE wxWindow* wxFindWindowAtPoint(const wxPoint& pt);

// NB: this function is obsolete, use wxWindow::FindWindowByLabel() instead
//
// Find the window/widget with the given title or label.
// Pass a parent to begin the search from, or NULL to look through
// all windows.
WXDLLIMPEXP_CORE wxWindow* wxFindWindowByLabel(const wxString& title, wxWindow *parent = (wxWindow *) NULL);

// NB: this function is obsolete, use wxWindow::FindWindowByName() instead
//
// Find window by name, and if that fails, by label.
WXDLLIMPEXP_CORE wxWindow* wxFindWindowByName(const wxString& name, wxWindow *parent = (wxWindow *) NULL);

// ----------------------------------------------------------------------------
// Message/event queue helpers
// ----------------------------------------------------------------------------

// Yield to other apps/messages and disable user input
WXDLLIMPEXP_CORE bool wxSafeYield(wxWindow *win = NULL, bool onlyIfNeeded = false);

// Enable or disable input to all top level windows
WXDLLIMPEXP_CORE void wxEnableTopLevelWindows(bool enable = true);

// Check whether this window wants to process messages, e.g. Stop button
// in long calculations.
WXDLLIMPEXP_CORE bool wxCheckForInterrupt(wxWindow *wnd);

// Consume all events until no more left
WXDLLIMPEXP_CORE void wxFlushEvents();

// a class which disables all windows (except, may be, the given one) in its
// ctor and enables them back in its dtor
class WXDLLIMPEXP_CORE wxWindowDisabler
{
public:
    // this ctor conditionally disables all windows: if the argument is false,
    // it doesn't do anything
    wxWindowDisabler(bool disable = true);

    // ctor disables all windows except winToSkip
    wxWindowDisabler(wxWindow *winToSkip);

    // dtor enables back all windows disabled by the ctor
    ~wxWindowDisabler();

private:
    // disable all windows except the given one (used by both ctors)
    void DoDisable(wxWindow *winToSkip = NULL);


    wxWindowList *m_winDisabled;
    bool m_disabled;

    DECLARE_NO_COPY_CLASS(wxWindowDisabler)
};

// ----------------------------------------------------------------------------
// Cursors
// ----------------------------------------------------------------------------

// Set the cursor to the busy cursor for all windows
WXDLLIMPEXP_CORE void wxBeginBusyCursor(const wxCursor *cursor = wxHOURGLASS_CURSOR);

// Restore cursor to normal
WXDLLIMPEXP_CORE void wxEndBusyCursor();

// true if we're between the above two calls
WXDLLIMPEXP_CORE bool wxIsBusy();

// Convenience class so we can just create a wxBusyCursor object on the stack
class WXDLLIMPEXP_CORE wxBusyCursor
{
public:
    wxBusyCursor(const wxCursor* cursor = wxHOURGLASS_CURSOR)
        { wxBeginBusyCursor(cursor); }
    ~wxBusyCursor()
        { wxEndBusyCursor(); }

    // FIXME: These two methods are currently only implemented (and needed?)
    //        in wxGTK.  BusyCursor handling should probably be moved to
    //        common code since the wxGTK and wxMSW implementations are very
    //        similar except for wxMSW using HCURSOR directly instead of
    //        wxCursor..  -- RL.
    static const wxCursor &GetStoredCursor();
    static const wxCursor GetBusyCursor();
};

void WXDLLIMPEXP_CORE wxGetMousePosition( int* x, int* y );

// MSW only: get user-defined resource from the .res file.
// Returns NULL or newly-allocated memory, so use delete[] to clean up.
#ifdef __WXMSW__
    extern WXDLLIMPEXP_CORE const wxChar* wxUserResourceStr;
    WXDLLIMPEXP_CORE wxChar* wxLoadUserResource(const wxString& resourceName, const wxString& resourceType = wxUserResourceStr);
#endif // MSW

// ----------------------------------------------------------------------------
// X11 Display access
// ----------------------------------------------------------------------------

#if defined(__X__) || defined(__WXGTK__)

#ifdef __WXGTK__
    WXDLLIMPEXP_CORE void *wxGetDisplay();
#endif

#ifdef __X__
    WXDLLIMPEXP_CORE WXDisplay *wxGetDisplay();
    WXDLLIMPEXP_CORE bool wxSetDisplay(const wxString& display_name);
    WXDLLIMPEXP_CORE wxString wxGetDisplayName();
#endif // X or GTK+

// use this function instead of the functions above in implementation code
inline struct _XDisplay *wxGetX11Display()
{
    return (_XDisplay *)wxGetDisplay();
}

#endif // X11 || wxGTK

#endif // wxUSE_GUI

// ----------------------------------------------------------------------------
// wxYield(): these functions are obsolete, please use wxApp methods instead!
// ----------------------------------------------------------------------------

// avoid redeclaring this function here if it had been already declated by
// wx/app.h, this results in warnings from g++ with -Wredundant-decls
#ifndef wx_YIELD_DECLARED
#define wx_YIELD_DECLARED

// Yield to other apps/messages
WXDLLIMPEXP_BASE bool wxYield();

#endif // wx_YIELD_DECLARED

// Like wxYield, but fails silently if the yield is recursive.
WXDLLIMPEXP_BASE bool wxYieldIfNeeded();

// ----------------------------------------------------------------------------
// Error message functions used by wxWidgets (deprecated, use wxLog)
// ----------------------------------------------------------------------------

#endif
    // _WX_UTILSH__
