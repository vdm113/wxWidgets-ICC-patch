/////////////////////////////////////////////////////////////////////////////
// Name:        utils.h
// Purpose:     interface of wxWindowDisabler
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxWindowDisabler
    @wxheader{utils.h}

    This class disables all windows of the application (may be with the exception
    of one of them) in its constructor and enables them back in its destructor.

    This is useful when you want to indicate to the user that the application
    is currently busy and cannot respond to user input.

    @library{wxcore}
    @category{FIXME}

    @see wxBusyCursor
*/
class wxWindowDisabler
{
public:
    /**
        Disables all top level windows of the applications.

        If @a disable is @c false nothing is done. This can be convenient if
        the windows should be disabled depending on some condition.

        @since 2.9.0
    */
    wxWindowDisabler(bool disable = true);

    /**
        Disables all top level windows of the applications with the exception of
        @a winToSkip if it is not @NULL.
    */
    wxWindowDisabler(wxWindow* winToSkip);

    /**
        Reenables back the windows disabled by the constructor.
    */
    ~wxWindowDisabler();
};



/**
    @class wxBusyCursor
    @wxheader{utils.h}

    This class makes it easy to tell your user that the program is temporarily busy.
    Just create a wxBusyCursor object on the stack, and within the current scope,
    the hourglass will be shown.

    For example:

    @code
    wxBusyCursor wait;

      for (int i = 0; i  100000; i++)
        DoACalculation();
    @endcode

    It works by calling wxBeginBusyCursor() in the constructor,
    and wxEndBusyCursor() in the destructor.

    @library{wxcore}
    @category{FIXME}

    @see wxBeginBusyCursor(), wxEndBusyCursor(), wxWindowDisabler
*/
class wxBusyCursor
{
public:
    /**
        Constructs a busy cursor object, calling wxBeginBusyCursor().
    */
    wxBusyCursor(wxCursor* cursor = wxHOURGLASS_CURSOR);

    /**
        Destroys the busy cursor object, calling wxEndBusyCursor().
    */
    ~wxBusyCursor();
};



// ============================================================================
// Global functions/macros
// ============================================================================


/** @ingroup group_funcmacro_dialog */
//@{

/**
    Changes the cursor to the given cursor for all windows in the application.
    Use wxEndBusyCursor() to revert the cursor back to its previous state.
    These two calls can be nested, and a counter ensures that only the outer
    calls take effect.

    @see wxIsBusy(), wxBusyCursor

    @header{wx/utils.h}
*/
void wxBeginBusyCursor(wxCursor* cursor = wxHOURGLASS_CURSOR);

/**
    Changes the cursor back to the original cursor, for all windows in the
    application. Use with wxBeginBusyCursor().

    @see wxIsBusy(), wxBusyCursor

    @header{wx/utils.h}
*/
void wxEndBusyCursor();

/**
    Returns @true if between two wxBeginBusyCursor() and wxEndBusyCursor()
    calls.

    @see wxBusyCursor.

    @header{wx/utils.h}
*/
bool wxIsBusy();

/**
    Ring the system bell.

    @note This function is categorized as a GUI one and so is not thread-safe.

    @header{wx/utils.h}
*/
void wxBell();

/**
    Shows a message box with the information about the wxWidgets build used,
    including its version, most important build parameters and the version of
    the underlying GUI toolkit. This is mainly used for diagnostic purposes
    and can be invoked by Ctrl-Alt-middle clicking on any wxWindow which
    doesn't otherwise handle this event.

    @wxsince{2.9.0}

    @header{wx/utils.h}
*/
void wxInfoMessageBox(wxWindow parent = NULL);

//@}



/** @ingroup group_funcmacro_env */
//@{

/**
    This is a macro defined as @c getenv() or its wide char version in Unicode
    mode.

    Note that under Win32 it may not return correct value for the variables set
    with wxSetEnv(), use wxGetEnv() function instead.

    @header{wx/utils.h}
*/
wxChar* wxGetenv(const wxString& var);

/**
    Returns the current value of the environment variable @c var in @c value.
    @c value may be @NULL if you just want to know if the variable exists and
    are not interested in its value.

    Returns @true if the variable exists, @false otherwise.

    @header{wx/utils.h}
*/
bool wxGetEnv(const wxString& var, wxString* value);

/**
    Sets the value of the environment variable @c var (adding it if necessary)
    to @c value.

    Returns @true on success.

    @see wxUnsetEnv()

    @header{wx/utils.h}
*/
bool wxSetEnv(const wxString& var, const wxString& value);

/**
    Removes the variable @c var from the environment. wxGetEnv() will return
    @NULL after the call to this function.

    Returns @true on success.

    @header{wx/utils.h}
*/
bool wxUnsetEnv(const wxString& var);

//@}



/** @ingroup group_funcmacro_misc */
//@{

/**
    Returns battery state as one of @c wxBATTERY_NORMAL_STATE,
    @c wxBATTERY_LOW_STATE, @c wxBATTERY_CRITICAL_STATE,
    @c wxBATTERY_SHUTDOWN_STATE or @c wxBATTERY_UNKNOWN_STATE.
    @c wxBATTERY_UNKNOWN_STATE is also the default on platforms where this
    feature is not implemented (currently everywhere but MS Windows).

    @header{wx/utils.h}
*/
wxBatteryState wxGetBatteryState();

/**
    Returns the type of power source as one of @c wxPOWER_SOCKET,
    @c wxPOWER_BATTERY or @c wxPOWER_UNKNOWN. @c wxPOWER_UNKNOWN is also the
    default on platforms where this feature is not implemented (currently
    everywhere but MS Windows).

    @header{wx/utils.h}
*/
wxPowerType wxGetPowerType();

/**
    Under X only, returns the current display name.

    @see wxSetDisplayName()

    @header{wx/utils.h}
*/
wxString wxGetDisplayName();

/**
    For normal keys, returns @true if the specified key is currently down.

    For togglable keys (Caps Lock, Num Lock and Scroll Lock), returns @true if
    the key is toggled such that its LED indicator is lit. There is currently
    no way to test whether togglable keys are up or down.

    Even though there are virtual key codes defined for mouse buttons, they
    cannot be used with this function currently.

    @header{wx/utils.h}
*/
bool wxGetKeyState(wxKeyCode key);

/**
    Returns the mouse position in screen coordinates.

    @header{wx/utils.h}
*/
wxPoint wxGetMousePosition();

/**
    Returns the current state of the mouse.  Returns a wxMouseState instance
    that contains the current position of the mouse pointer in screen
    coordinates, as well as boolean values indicating the up/down status of the
    mouse buttons and the modifier keys.

    @header{wx/utils.h}
*/
wxMouseState wxGetMouseState();

/**
    This function enables or disables all top level windows. It is used by
    wxSafeYield().

    @header{wx/utils.h}
*/
void wxEnableTopLevelWindows(bool enable = true);

/**
    Find the deepest window at the given mouse position in screen coordinates,
    returning the window if found, or @NULL if not.

    @header{wx/utils.h}
*/
wxWindow* wxFindWindowAtPoint(const wxPoint& pt);

/**
    @deprecated Replaced by wxWindow::FindWindowByLabel().

    Find a window by its label. Depending on the type of window, the label may
    be a window title or panel item label. If @a parent is @NULL, the search
    will start from all top-level frames and dialog boxes; if non-@NULL, the
    search will be limited to the given window hierarchy. The search is
    recursive in both cases.

    @header{wx/utils.h}
*/
wxWindow* wxFindWindowByLabel(const wxString& label,
                              wxWindow* parent = NULL);

/**
    @deprecated Replaced by wxWindow::FindWindowByName().

    Find a window by its name (as given in a window constructor or @e Create
    function call). If @a parent is @NULL, the search will start from all
    top-level frames and dialog boxes; if non-@NULL, the search will be limited
    to the given window hierarchy. The search is recursive in both cases.

    If no such named window is found, wxFindWindowByLabel() is called.

    @header{wx/utils.h}
*/
wxWindow* wxFindWindowByName(const wxString& name, wxWindow* parent = NULL);

/**
    Find a menu item identifier associated with the given frame's menu bar.

    @header{wx/utils.h}
*/
int wxFindMenuItemId(wxFrame* frame, const wxString& menuString,
                     const wxString& itemString);

/**
    @deprecated Ids generated by it can conflict with the Ids defined by the
                user code, use @c wxID_ANY to assign ids which are guaranteed
                to not conflict with the user-defined ids for the controls and
                menu items you create instead of using this function.

    Generates an integer identifier unique to this run of the program.

    @header{wx/utils.h}
*/
long wxNewId();

/**
    Ensures that Ids subsequently generated by wxNewId() do not clash with the
    given @a id.

    @header{wx/utils.h}
*/
void wxRegisterId(long id);

/**
    Opens the @a url in user's default browser. If the @a flags parameter
    contains @c wxBROWSER_NEW_WINDOW flag, a new window is opened for the URL
    (currently this is only supported under Windows). The @a url may also be a
    local file path (with or without the "file://" prefix), if it doesn't
    correspond to an existing file and the URL has no scheme "http://" is
    prepended to it by default.

    Returns @true if the application was successfully launched.

    @note For some configurations of the running user, the application which is
          launched to open the given URL may be URL-dependent (e.g. a browser
          may be used for local URLs while another one may be used for remote
          URLs).

    @header{wx/utils.h}
*/
bool wxLaunchDefaultBrowser(const wxString& url, int flags = 0);

/**
    Loads a user-defined Windows resource as a string. If the resource is
    found, the function creates a new character array and copies the data into
    it. A pointer to this data is returned. If unsuccessful, @NULL is returned.

    The resource must be defined in the @c .rc file using the following syntax:

    @code
    myResource TEXT file.ext
    @endcode

    Where @c file.ext is a file that the resource compiler can find.

    This function is available under Windows only.

    @header{wx/utils.h}
*/
wxString wxLoadUserResource(const wxString& resourceName,
                            const wxString& resourceType = "TEXT");

/**
    @deprecated Replaced by wxWindow::Close(). See the
                @ref overview_windowdeletion "window deletion overview".

    Tells the system to delete the specified object when all other events have
    been processed. In some environments, it is necessary to use this instead
    of deleting a frame directly with the delete operator, because some GUIs
    will still send events to a deleted window.

    @header{wx/utils.h}
*/
void wxPostDelete(wxObject* object);

/**
    Under X only, sets the current display name. This is the X host and display
    name such as "colonsay:0.0", and the function indicates which display
    should be used for creating windows from this point on. Setting the display
    within an application allows multiple displays to be used.

    @see wxGetDisplayName()

    @header{wx/utils.h}
*/
void wxSetDisplayName(const wxString& displayName);

/**
    Strips any menu codes from @a str and returns the result.

    By default, the functions strips both the mnemonics character (@c '&')
    which is used to indicate a keyboard shortkey, and the accelerators, which
    are used only in the menu items and are separated from the main text by the
    @c \t (TAB) character. By using @a flags of @c wxStrip_Mnemonics or
    @c wxStrip_Accel to strip only the former or the latter part, respectively.

    Notice that in most cases wxMenuItem::GetLabelFromText() or
    wxControl::GetLabelText() can be used instead.

    @header{wx/utils.h}
*/
wxString wxStripMenuCodes(const wxString& str, int flags = wxStrip_All);

//@}






/**
    This function returns the "user id" also known as "login name" under Unix
    i.e.  something like "jsmith". It uniquely identifies the current user (on
    this system).  Under Windows or NT, this function first looks in the
    environment variables USER and LOGNAME; if neither of these is found, the
    entry @b UserId in the @b wxWidgets section of the WIN.INI file is tried.

    @return Returns the login name if successful or an empty string otherwise.

    @see wxGetUserName()

    @header{wx/utils.h}
*/
wxString wxGetUserId();

/**
    @deprecated
    This form is deprecated, use wxGetUserId() version that returns wxString.

    @param buf Buffer to store login name in.
    @param sz  Size of the buffer.

    @return Returns @true if successful, @false otherwise.
 */
bool wxGetUserId(char* buf, int sz);

/**
    Returns the string containing the description of the current platform in a
    user-readable form. For example, this function may return strings like
    @c Windows NT Version 4.0 or @c Linux 2.2.2 i386.

    @see ::wxGetOsVersion

    @header{wx/utils.h}
*/
wxString wxGetOsDescription();

/**
    Return the (current) user's home directory.

    @see wxGetUserHome(), wxStandardPaths

    @header{wx/utils.h}
*/
wxString wxGetHomeDir();

/**
    Sleeps for the specified number of milliseconds. Notice that usage of this
    function is encouraged instead of calling usleep(3) directly because the
    standard usleep() function is not MT safe.

    @header{wx/utils.h}
*/
void wxMilliSleep(unsigned long milliseconds);

/**
    Sleeps for the specified number of microseconds. The microsecond resolution may
    not, in fact, be available on all platforms (currently only Unix platforms with
    nanosleep(2) may provide it) in which case this is the same as
    wxMilliSleep()(@e microseconds/1000).

    @header{wx/utils.h}
*/
void wxMicroSleep(unsigned long microseconds);

/**
    Executes a command in an interactive shell window. If no command is
    specified, then just the shell is spawned.
    See also wxExecute(), @ref overview_sampleexec "Exec sample".

    @header{wx/utils.h}
*/
bool wxShell(const wxString& command = NULL);

/**
    Gets the version and the operating system ID for currently running OS.
    See wxPlatformInfo for more details about wxOperatingSystemId.

    @see ::wxGetOsDescription, wxPlatformInfo

    @header{wx/utils.h}
*/
wxOperatingSystemId wxGetOsVersion(int* major = NULL,
                                    int* minor = NULL);

/**
    Returns the FQDN (fully qualified domain host name) or an empty string on
    error.

    @see wxGetHostName()

    @header{wx/utils.h}
*/
wxString wxGetFullHostName();

/**
    Returns the amount of free memory in bytes under environments which
    support it, and -1 if not supported or failed to perform measurement.
*/
wxMemorySize wxGetFreeMemory();

//@{
/**
    Copies the current host machine's name into the supplied buffer. Please note
    that the returned name is @e not fully qualified, i.e. it does not include
    the domain name.
    Under Windows or NT, this function first looks in the environment
    variable SYSTEM_NAME; if this is not found, the entry @b HostName
    in the @b wxWidgets section of the WIN.INI file is tried.
    The first variant of this function returns the hostname if successful or an
    empty string otherwise. The second (deprecated) function returns @true
    if successful, @false otherwise.

    @see wxGetFullHostName()

    @header{wx/utils.h}
*/
wxString wxGetHostName();
bool wxGetHostName(char* buf, int sz);
//@}

/**
    Returns the home directory for the given user. If the @a user is empty
    (default value), this function behaves like
    wxGetHomeDir() i.e. returns the current user home
    directory.
    If the home directory couldn't be determined, an empty string is returned.

    @header{wx/utils.h}
*/
wxString wxGetUserHome(const wxString& user = "");

//@{
/**
    @b wxPerl note: In wxPerl this function is called @c Wx::ExecuteStdoutStderr
    and it only takes the @c command argument,
    and returns a 3-element list @c ( status, output, errors ), where
    @c output and @c errors are array references.
    Executes another program in Unix or Windows.
    The first form takes a command string, such as @c "emacs file.txt".
    The second form takes an array of values: a command, any number of
    arguments, terminated by @NULL.
    The semantics of the third and fourth versions is different from the first two
    and is described in more details below.
    If @a flags parameter contains @c wxEXEC_ASYNC flag (the default), flow
    of control immediately returns. If it contains @c wxEXEC_SYNC, the current
    application waits until the other program has terminated.
    In the case of synchronous execution, the return value is the exit code of
    the process (which terminates by the moment the function returns) and will be
    -1 if the process couldn't be started and typically 0 if the process
    terminated successfully. Also, while waiting for the process to
    terminate, wxExecute will call wxYield(). Because of this, by
    default this function disables all application windows to avoid unexpected
    reentrancies which could result from the users interaction with the program
    while the child process is running. If you are sure that it is safe to not
    disable the program windows, you may pass @c wxEXEC_NODISABLE flag to
    prevent this automatic disabling from happening.
    For asynchronous execution, however, the return value is the process id and
    zero value indicates that the command could not be executed. As an added
    complication, the return value of -1 in this case indicates that we didn't
    launch a new process, but connected to the running one (this can only happen in
    case of using DDE under Windows for command execution). In particular, in this,
    and only this, case the calling code will not get the notification about
    process termination.
    If callback isn't @NULL and if execution is asynchronous,
    wxProcess::OnTerminate will be called when
    the process finishes. Specifying this parameter also allows you to redirect the
    standard input and/or output of the process being launched by calling
    wxProcess::Redirect. If the child process IO is redirected,
    under Windows the process window is not shown by default (this avoids having to
    flush an unnecessary console for the processes which don't create any windows
    anyhow) but a @c wxEXEC_NOHIDE flag can be used to prevent this from
    happening, i.e. with this flag the child process window will be shown normally.
    Under Unix the flag @c wxEXEC_MAKE_GROUP_LEADER may be used to ensure
    that the new process is a group leader (this will create a new session if
    needed). Calling wxKill() passing wxKILL_CHILDREN will
    kill this process as well as all of its children (except those which have
    started their own session).
    The @c wxEXEC_NOEVENTS flag prevents processing of any events from taking
    place while the child process is running. It should be only used for very
    short-lived processes as otherwise the application windows risk becoming
    unresponsive from the users point of view. As this flag only makes sense with
    @c wxEXEC_SYNC, @c wxEXEC_BLOCK equal to the sum of both of these flags
    is provided as a convenience.
    Finally, you may use the third overloaded version of this function to execute
    a process (always synchronously, the contents of @a flags is or'd with
    @c wxEXEC_SYNC) and capture its output in the array @e output. The
    fourth version adds the possibility to additionally capture the messages from
    standard error output in the @a errors array.
    @b NB: Currently wxExecute() can only be used from the main thread, calling
    this function from another thread will result in an assert failure in debug
    build and won't work.

    @param command
        The command to execute and any parameters to pass to it as a
        single string.
    @param argv
        The command to execute should be the first element of this
        array, any additional ones are the command parameters and the array must be
        terminated with a @NULL pointer.
    @param flags
        Must include either wxEXEC_ASYNC or wxEXEC_SYNC and can also include
        wxEXEC_NOHIDE, wxEXEC_MAKE_GROUP_LEADER (in either case) or
        wxEXEC_NODISABLE and wxEXEC_NOEVENTS or wxEXEC_BLOCK, which is equal to
        their combination, in wxEXEC_SYNC case.
    @param callback
        An optional pointer to wxProcess

    @see wxShell(), wxProcess, @ref overview_sampleexec "Exec sample".

    @header{wx/utils.h}
*/
long wxExecute(const wxString& command,
               int sync = wxEXEC_ASYNC,
               wxProcess* callback = NULL);
long wxExecute(char** argv,
               int flags = wxEXEC_ASYNC,
               wxProcess* callback = NULL);
long wxExecute(wchar_t** argv,
               int flags = wxEXEC_ASYNC,
               wxProcess* callback = NULL);
long wxExecute(const wxString& command,
               wxArrayString& output,
               int flags = 0);
long wxExecute(const wxString& command,
               wxArrayString& output,
               wxArrayString& errors,
               int flags = 0);
//@}

/**
    Returns a string representing the current date and time.

    @header{wx/utils.h}
*/
wxString wxNow();

/**
    Returns @true if the operating system the program is running under is 64 bit.
    The check is performed at run-time and may differ from the value available at
    compile-time (at compile-time you can just check if @c sizeof(void*)==8)
    since the program could be running in emulation mode or in a mixed 32/64 bit
    system
    (bi-architecture operating system).
    Very important: this function is not 100% reliable on some systems given the
    fact
    that there isn't always a standard way to do a reliable check on the OS
    architecture.

    @header{wx/utils.h}
*/
bool wxIsPlatform64Bit();

/**
    Returns the number uniquely identifying the current process in the system.
    If an error occurs, 0 is returned.

    @header{wx/utils.h}
*/
unsigned long wxGetProcessId();

/**
    Equivalent to the Unix kill function: send the given signal @a sig to the
    process with PID @e pid. The valid signal values are

    @code
    enum wxSignal
    {
        wxSIGNONE = 0,  // verify if the process exists under Unix
        wxSIGHUP,
        wxSIGINT,
        wxSIGQUIT,
        wxSIGILL,
        wxSIGTRAP,
        wxSIGABRT,
        wxSIGEMT,
        wxSIGFPE,
        wxSIGKILL,      // forcefully kill, dangerous!
        wxSIGBUS,
        wxSIGSEGV,
        wxSIGSYS,
        wxSIGPIPE,
        wxSIGALRM,
        wxSIGTERM       // terminate the process gently
    };
    @endcode

    @c wxSIGNONE, @c wxSIGKILL and @c wxSIGTERM have the same meaning
    under both Unix and Windows but all the other signals are equivalent to
    @c wxSIGTERM under Windows.
    Returns 0 on success, -1 on failure. If @a rc parameter is not @NULL, it will
    be filled with an element of @c wxKillError enum:

    @code
    enum wxKillError
    {
        wxKILL_OK,              // no error
        wxKILL_BAD_SIGNAL,      // no such signal
        wxKILL_ACCESS_DENIED,   // permission denied
        wxKILL_NO_PROCESS,      // no such process
        wxKILL_ERROR            // another, unspecified error
    };
    @endcode

    The @a flags parameter can be wxKILL_NOCHILDREN (the default),
    or wxKILL_CHILDREN, in which case the child processes of this
    process will be killed too. Note that under Unix, for wxKILL_CHILDREN
    to work you should have created the process by passing wxEXEC_MAKE_GROUP_LEADER
    to wxExecute.

    @see wxProcess::Kill, wxProcess::Exists, @ref overview_sampleexec "Exec sample"

    @header{wx/utils.h}
*/
int wxKill(long pid, int sig = wxSIGTERM, wxKillError rc = NULL,
            int flags = 0);

//@{
/**
    Copies the user's email address into the supplied buffer, by
    concatenating the values returned by wxGetFullHostName()
    and wxGetUserId().
    Returns @true if successful, @false otherwise.

    @header{wx/utils.h}
*/
wxString wxGetEmailAddress();
bool wxGetEmailAddress(char* buf, int sz);
//@}

/**
    Sleeps for the specified number of seconds.

    @header{wx/utils.h}
*/
void wxSleep(int secs);

/**
    Returns @true if the current platform is little endian (instead of big
    endian).
    The check is performed at run-time.

    @see @ref overview_byteordermacros "Byte order macros"

    @header{wx/utils.h}
*/
bool wxIsPlatformLittleEndian();

