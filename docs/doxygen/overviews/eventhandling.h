/////////////////////////////////////////////////////////////////////////////
// Name:        eventhandling.h
// Purpose:     topic overview
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @page overview_events Events and Event Handling

    Related classes: wxEvtHandler, wxWindow, wxEvent

@li @ref overview_events_introduction
@li @ref overview_events_eventhandling
@li @ref overview_events_processing
@li @ref overview_events_custom
@li @ref overview_events_misc


<hr>


@section overview_events_introduction Introduction to Events

Like with all the other GUI frameworks, the control of flow in wxWidgets
applications is event-based: the program normally performs most of its actions
in response to the events generated by the user. These events can be triggered
by using the input devices (such as keyboard, mouse, joystick) directly or,
more commonly, by a standard control which synthesizes such input events into
higher level events: for example, a wxButton can generate a click event when
the user presses the left mouse button on it and then releases it without
pressing @c Esc in the meanwhile. There are also events which don't directly
correspond to the user actions, such as wxTimerEvent or wxSocketEvent.

But in all cases wxWidgets represents these events in a uniform way and allows
you to handle them in the same way wherever they originate from. And while the
events are normally generated by wxWidgets itself, you can also do this, which
is especially useful when using custom events (see @ref overview_events_custom).

To be more precise, each event is described by:
 - <em>Event type</em>: this is simply a value of type wxEventType which
 uniquely identifies the type of the event. For example, clicking on a button,
 selecting an item from a list box and pressing a key on the keyboard all
 generate events with different event types.
 - <em>Event class</em> carried by the event: each event has some information
 associated with it and this data is represented by an object of a class
 derived from wxEvent. Events of different types can use the same event class,
 for example both button click and listbox selection events use wxCommandEvent
 class (as do all the other simple control events), but the key press event
 uses wxKeyEvent as the information associated with it is different.
 - <em>Event source</em>: wxEvent stores the object which generated the event
 and, for windows, its identifier (see @ref overview_events_winid). As it is
 common to have more than one object generating events of the same type (e.g. a
 typical window contains several buttons, all generating the same button click
 event), checking the event source object or its id allows to distinguish
 between them.


@section overview_events_eventhandling Event Handling

There are two principal ways to handle events in wxWidgets. One of them uses
<em>event table</em> macros and allows you to define the connection between events
and their handlers only statically, i.e., during program compilation. The other
one uses wxEvtHandler::Connect() call and can be used to connect, and
disconnect, the handlers dynamically, i.e., during run-time depending on some
conditions. It also allows the direct connection of the events of one object to a
handler method in another object. The static event tables can only handle
events in the object where they are defined so using Connect() is more flexible
than using the event tables. On the other hand, event tables are more succinct
and centralize all event handlers connection in one place. You can either
choose a single approach that you find preferable or freely combine both
methods in your program in different classes or even in one and the same class,
although this is probably sufficiently confusing to be a bad idea.

But before you make this choice, let us discuss these two ways in more
detail. In the next section we provide a short introduction to handling the
events using the event tables. Please see @ref overview_events_connect
for the discussion of Connect().

@subsection overview_events_eventtables Event Handling with Event Tables

To use an <em>event table</em> you must first decide in which class you wish to
handle the events. The only requirement imposed by wxWidgets is that this class
must derive from wxEvtHandler and so, considering that wxWindow derives from
it, any classes representing windows can handle events. Simple events such as
menu commands are usually processed at the level of a top-level window
containing the menu, so let's suppose that you need to handle some events in @c
MyFrame class deriving from wxFrame.

First define one or more <em>event handlers</em>. They
are just simple (non-virtual) methods of the class that take as a parameter a
reference to an object of a wxEvent-derived class and have no return value (any
return information is passed via the argument, which is why it is non-const).
You also need to insert a macro

@code
DECLARE_EVENT_TABLE()
@endcode

somewhere in the class declaration. It doesn't matter where it appears but
it's customary to put it at the end because the macro changes the access
type internally so it's safest if nothing follows it. The
full class declaration might look like this:

@code
class MyFrame : public wxFrame
{
public:
    MyFrame(...) : wxFrame(...) { }

    ...

protected:
    int m_whatever;

private:
    // Notice that as the event handlers normally are not called from outside
    // the class, they normally are private. In particular they don't need
    // to be public.
    void OnExit(wxCommandEvent& event);
    void OnButton1(wxCommandEvent& event);
    void OnSize(wxSizeEvent& event);

    // it's common to call the event handlers OnSomething() but there is no
    // obligation to do that; this one is an event handler too:
    void DoTest(wxCommandEvent& event);

    DECLARE_EVENT_TABLE()
};
@endcode

Next the event table must be defined and, as with any definition, it must be
placed in an implementation file. The event table tells wxWidgets how to map
events to member functions and in our example it could look like this:

@code
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MyFrame::OnExit)
    EVT_MENU(DO_TEST, MyFrame::DoTest)
    EVT_SIZE(MyFrame::OnSize)
    EVT_BUTTON(BUTTON1, MyFrame::OnButton1)
END_EVENT_TABLE()
@endcode

Notice that you must mention a method you want to use for the event handling in
the event table definition; just defining it in MyFrame class is @e not enough.

Let us now look at the details of this definition: the first line means that we
are defining the event table for MyFrame class and that its base class is
wxFrame, so events not processed by MyFrame will, by default, be handled by
wxFrame. The next four lines define connections of individual events to their
handlers: the first two of them map menu commands from the items with the
identifiers specified as the first macro parameter to two different member
functions. In the next one, @c EVT_SIZE means that any changes in the size of
the frame will result in calling OnSize() method. Note that this macro doesn't
need a window identifier, since normally you are only interested in the current
window's size events.

The @c EVT_BUTTON macro demonstrates that the originating event does not have to
come from the window class implementing the event table -- if the event source
is a button within a panel within a frame, this will still work, because event
tables are searched up through the hierarchy of windows for the command events.
(But only command events, so you can't catch mouse move events in a child
control in the parent window in the same way because wxMouseEvent doesn't
derive from wxCommandEvent. See below for how you can do it.) In this case, the
button's event table will be searched, then the parent panel's, then the
frame's.

Finally, you need to implement the event handlers. As mentioned before, all
event handlers take a wxEvent-derived argument whose exact class differs
according to the type of event and the class of the originating window. For
size events, wxSizeEvent is used. For menu commands and most control commands
(such as button presses), wxCommandEvent is used. When controls get more
complicated, more specific wxCommandEvent-derived event classes providing
additional control-specific information can be used, such as wxTreeEvent for
events from wxTreeCtrl windows.

In the simplest possible case an event handler may not use the @c event
parameter at all. For example,

@code
void MyFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
    // when the user selects "Exit" from the menu we should close
    Close(true);
}
@endcode

In other cases you may need some information carried by the @c event argument,
as in:

@code
void MyFrame::OnSize(wxSizeEvent& event)
{
    wxSize size = event.GetSize();

    ... update the frame using the new size ...
}
@endcode

You will find the details about the event table macros and the corresponding
wxEvent-derived classes in the discussion of each control generating these
events.


@subsection overview_events_connect Dynamic Event Handling

As with the event tables, decide in which class you intend to
handle the events first and, as before, this class must derive from
wxEvtHandler (usually indirectly via wxWindow). See the declaration of MyFrame
in the previous section. However the similarities end here and both the syntax
and the possibilities of handling events in this way are rather different.

Let us start by looking at the syntax: the first obvious difference is that you
need not use DECLARE_EVENT_TABLE() nor BEGIN_EVENT_TABLE() and the
associated macros. Instead, in any place in your code, but usually in
the code of the class defining the handler itself (and definitely not in the
global scope as with the event tables), call its Connect() method like this:

@code
MyFrame::MyFrame(...)
{
      Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED,
                wxCommandEventHandler(MyFrame::OnExit));
}
@endcode

This class should be self-explanatory except for wxCommandEventHandler part:
this is a macro that ensures that the method is of the correct type by using
static_cast in the same way as the event table macros.

Now let us describe the semantic differences:
<ul>
    <li>
        Event handlers can be connected at any moment. For example, it's possible
        to do some initialization first and only connect the handlers if and when
        it succeeds. This can avoid the need to test that the object was properly
        initialized in the event handlers themselves. With Connect() they
        simply won't be called if it wasn't correctly initialized.
    </li>

    <li>
        As a slight extension of the above, the handlers can also be
        Disconnect()-ed at any time and maybe later reconnected. Of course,
        it's also possible to emulate this behaviour with the classic
        static (i.e., connected via event tables) handlers by using an internal
        flag indicating whether the handler is currently enabled and returning
        from it if it isn't, but using dynamically connected handlers requires
        less code and is also usually more clear.
    </li>

    <li>
        Also notice that you must derive a class inherited from, say,
        wxTextCtrl even if you don't want to modify the control behaviour at
        all but just want to handle some of its events. This is especially
        inconvenient when the control is loaded from the XRC. Connecting the
        event handler dynamically bypasses the need for this unwanted
        sub-classing.
    </li>

    <li>
        Last but very, very far from least is the possibility to connect an
        event of some object to a method of another object. This is impossible
        to do with event tables because it is not possible to specify the
        object to dispatch the event to so it necessarily needs to be sent to
        the same object which generated the event. Not so with Connect() which
        has an optional @c eventSink parameter that can be used to specify the
        object which will handle the event. Of course, in this case the method
        being connected must belong to the class that is the type of the
        @c eventSink object! To give a quick example, people often want to catch
        mouse movement events happening when the mouse is in one of the frame
        children in the frame itself. Doing it in a naive way doesn't work:
        <ul>
            <li>
                A @c EVT_LEAVE_WINDOW(MyFrame::OnMouseLeave) line in the frame
                event table has no effect as mouse move (including entering and
                leaving) events are not propagated up to the parent window
                (at least not by default).
            </li>

            <li>
                Putting the same line in a child event table will crash during
                run-time because the MyFrame method will be called on a wrong
                object -- it's easy to convince oneself that the only object
                that can be used here is the pointer to the child, as
                wxWidgets has nothing else. But calling a frame method with the
                child window pointer instead of the pointer to the frame is, of
                course, disastrous.
            </li>
        </ul>

        However writing
        @code
            MyFrame::MyFrame(...)
            {
              m_child->Connect(wxID_ANY, wxEVT_LEAVE_WINDOW,
                               wxMouseEventHandler(MyFrame::OnMouseLeave),
                               NULL,  // unused extra data parameter
                               this); // this indicates the object to connect to
            }
        @endcode
        will work exactly as expected. Note that you can get the object that
        generated the event -- and that is not the same as the frame -- via
        wxEvent::GetEventObject() method of @c event argument passed to the
        event handler.
    </li>
</ul>

To summarize, using Connect() requires slightly more typing but is much more
flexible than using static event tables so don't hesitate to use it when you
need this extra power. On the other hand, event tables are still perfectly fine
in simple situations where this extra flexibility is not needed.


@section overview_events_processing How Events are Processed

The previous sections explain how to define event handlers but don't address
the question of how exactly wxWidgets finds the handler to call for the
given event. This section describes the algorithm used in detail.

When an event is received from the windowing system, wxWidgets calls
wxEvtHandler::ProcessEvent() on the first event handler object belonging to the
window generating the event. The normal order of event table searching by
ProcessEvent() is as follows, with the event processing stopping as soon as a
handler is found (unless the handler calls wxEvent::Skip() in which case it
doesn't count as having handled the event and the search continues):
<ol>
    <li value="0">
    Before anything else happens, wxApp::FilterEvent() is called. If it returns
    anything but -1 (default), the event handling stops immediately.
    </li>

    <li value="1">
    If this event handler is disabled via a call to
    wxEvtHandler::SetEvtHandlerEnabled() the next three steps are skipped and
    the event handler resumes at step (5).
    </li>

    <li value="2">
    If the object is a wxWindow and has an associated validator, wxValidator
    gets a chance to process the event.
    </li>

    <li value="3">
    The list of dynamically connected event handlers, i.e., those for which
    Connect() was called, is consulted. Notice that this is done before
    checking the static event table entries, so if both a dynamic and a static
    event handler match the same event, the static one is never going to be
    used.
    </li>

    <li value="4">
    The event table containing all the handlers defined using the event table
    macros in this class and its base classes is examined. Notice that this
    means that any event handler defined in a base class will be executed at
    this step.
    </li>

    <li value="5">
    The event is passed to the next event handler, if any, in the event handler
    chain, i.e., the steps (1) to (4) are done for it. This chain can be formed
    using wxEvtHandler::SetNextHandler():
        @image html overview_events_chain.png
    (referring to the image, if @c A->ProcessEvent is called and it doesn't handle
     the event, @c B->ProcessEvent will be called and so on...).
    In the case of wxWindow you can build a stack (implemented using wxEvtHandler
    double-linked list) using wxWindow::PushEventHandler():
        @image html overview_events_winstack.png
    (referring to the image, if @c W->ProcessEvent is called, it immediately calls
     @c A->ProcessEvent; if nor @c A nor @c B handle the event, then the wxWindow
     itself is used - i.e. the dynamically connected event handlers and static
     event table entries of wxWindow are looked as the last possibility, after
     all pushed event handlers were tested).
    Note however that usually there are no wxEvtHandler chains nor wxWindows stacks
    so this step will usually do anything.
    </li>

    <li value="6">
    If the object is a wxWindow and the event is set to propagate (by default
    only wxCommandEvent-derived events are set to propagate), then the
    processing restarts from the step (1) (and excluding the step (7)) for the
    parent window. If this object is not a window but the next handler exists,
    the event is passed to its parent if it is a window. This ensures that in a
    common case of (possibly several) non-window event handlers pushed on top
    of a window, the event eventually reaches the window parent.
    </li>

    <li value="7">
    Finally, i.e., if the event is still not processed, the wxApp object itself
    (which derives from wxEvtHandler) gets a last chance to process it.
    </li>
</ol>

<em>Please pay close attention to step 6!</em> People often overlook or get
confused by this powerful feature of the wxWidgets event processing system. The
details of event propagation up the window hierarchy are described in the
next section.

Also please notice that there are additional steps in the event handling for
the windows-making part of wxWidgets document-view framework, i.e.,
wxDocParentFrame, wxDocChildFrame and their MDI equivalents wxDocMDIParentFrame
and wxDocMDIChildFrame. The parent frame classes modify step (2) above to
send the events received by them to wxDocManager object first. This object, in
turn, sends the event to the current view and the view itself lets its
associated document process the event first. The child frame classes send
the event directly to the associated view which still forwards it to its
document object. Notice that to avoid remembering the exact order in which the
events are processed in the document-view frame, the simplest, and recommended,
solution is to only handle the events at the view classes level, and not in the
document or document manager classes


@subsection overview_events_propagation How Events Propagate Upwards

As mentioned above, the events of the classes deriving from wxCommandEvent are
propagated by default to the parent window if they are not processed in this
window itself. But although by default only the command events are propagated
like this, other events can be propagated as well because the event handling
code uses wxEvent::ShouldPropagate() to check whether an event should be
propagated. It is also possible to propagate the event only a limited number of
times and not until it is processed (or a top level parent window is reached).

Finally, there is another additional complication (which, in fact, simplifies
life of wxWidgets programmers significantly): when propagating the command
events up to the parent window, the event propagation stops when it
reaches the parent dialog, if any. This means that you don't risk getting
unexpected events from the dialog controls (which might be left unprocessed by
the dialog itself because it doesn't care about them) when a modal dialog is
popped up. The events do propagate beyond the frames, however. The rationale
for this choice is that there are only a few frames in a typical application
and their parent-child relation are well understood by the programmer while it
may be difficult, if not impossible, to track down all the dialogs that
may be popped up in a complex program (remember that some are created
automatically by wxWidgets). If you need to specify a different behaviour for
some reason, you can use wxWindow::SetExtraStyle(wxWS_EX_BLOCK_EVENTS)
explicitly to prevent the events from being propagated beyond the given window
or unset this flag for the dialogs that have it on by default.

Typically events that deal with a window as a window (size, motion,
paint, mouse, keyboard, etc.) are sent only to the window.  Events
that have a higher level of meaning or are generated by the window
itself, (button click, menu select, tree expand, etc.) are command
events and are sent up to the parent to see if it is interested in the event.

As mentioned above, only command events are recursively applied to the parents
event handler in the library itself. As this quite often causes confusion for
users, here is a list of system events that will @em not get sent to the
parent's event handler:

@li wxEvent: The event base class
@li wxActivateEvent: A window or application activation event
@li wxCloseEvent: A close window or end session event
@li wxEraseEvent: An erase background event
@li wxFocusEvent: A window focus event
@li wxKeyEvent: A keypress event
@li wxIdleEvent: An idle event
@li wxInitDialogEvent: A dialog initialisation event
@li wxJoystickEvent: A joystick event
@li wxMenuEvent: A menu event
@li wxMouseEvent: A mouse event
@li wxMoveEvent: A move event
@li wxPaintEvent: A paint event
@li wxQueryLayoutInfoEvent: Used to query layout information
@li wxSetCursorEvent: Used for special cursor processing based on current mouse position
@li wxSizeEvent: A size event
@li wxScrollWinEvent: A scroll event sent by a scrolled window (not a scroll bar)
@li wxSysColourChangedEvent: A system colour change event

In some cases, it might be desired by the programmer to get a certain number
of system events in a parent window, for example all key events sent to, but not
used by, the native controls in a dialog. In this case, a special event handler
will have to be written that will override ProcessEvent() in order to pass
all events (or any selection of them) to the parent window.


@section overview_events_custom Custom Event Summary

@subsection overview_events_custom_general General approach

As each event is uniquely defined by its event type, defining a custom event
starts with defining a new event type for it. This is done using
wxDEFINE_EVENT() macro. As an event type is a variable, it can also be
declared using wxDECLARE_EVENT() if necessary.

The next thing to do is to decide whether you need to define a custom event
class for events of this type or if you can reuse an existing class, typically
either wxEvent (which doesn't provide any extra information) or wxCommandEvent
(which contains several extra fields and also propagates upwards by default).
Both strategies are described in details below. See also the @ref
page_samples_event for a complete example of code defining and working with the
custom event types.


@subsection overview_events_custom_existing Using Existing Event Classes

If you just want to use a wxCommandEvent with a new event type, use one of the
generic event table macros listed below, without having to define a new event
class yourself.

Example:

@code
// this is typically in a header: it just declares MY_EVENT event type
wxDECLARE_EVENT(MY_EVENT, wxCommandEvent);

// this is a definition so can't be in a header
wxDEFINE_EVENT(MY_EVENT, wxCommandEvent);

// example of code handling the event with event tables
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU    (wxID_EXIT, MyFrame::OnExit)
    ...
    EVT_COMMAND (ID_MY_WINDOW, MY_EVENT, MyFrame::OnMyEvent)
END_EVENT_TABLE()

void MyFrame::OnMyEvent(wxCommandEvent& event)
{
    // do something
    wxString text = event.GetText();
}

// example of code handling the event with Connect():
MyFrame::MyFrame()
{
    Connect(ID_MY_WINDOW, MY_EVENT, &MyFrame::OnMyEvent);
}

// example of code generating the event
void MyWindow::SendEvent()
{
    wxCommandEvent event(MY_EVENT, GetId());
    event.SetEventObject(this);

    // Give it some contents
    event.SetText("Hello");

    // Do send it
    ProcessWindowEvent(event);
}
@endcode


@subsection overview_events_custom_ownclass Defining Your Own Event Class

Under certain circumstances, you must define your own event class e.g., for
sending more complex data from one place to another. Apart from defining your
event class, you also need to define your own event table macro if you want to
use event tables for handling events of this type.

Here is an example:

@code
// define a new event class
class MyPlotEvent: public wxEvent
{
public:
    MyPlotEvent(wxEventType eventType, int winid, const wxPoint& pos)
        : wxEvent(winid, eventType),
          m_pos(pos)
    {
    }

    // accessors
    wxPoint GetPoint() const { return m_pos; }

    // implement the base class pure virtual
    virtual wxEvent *Clone() const { return new MyPlotEvent(*this); }

private:
    const wxPoint m_pos;
};

// we define a single MY_PLOT_CLICKED event type associated with the class
// above but typically you are going to have more than one event type, e.g. you
// could also have MY_PLOT_ZOOMED or MY_PLOT_PANNED &c -- in which case you
// would just add more similar lines here
wxDEFINE_EVENT(MY_PLOT_CLICKED, MyPlotEvent);


// if you want to support old compilers you need to use some ugly macros:
typedef void (wxEvtHandler::*MyPlotEventFunction)(MyPlotEvent&);
#define MyPlotEventHandler(func) wxEVENT_HANDLER_CAST(MyPlotEventFunction, func)

// if your code is only built sing reasonably modern compilers, you could just
// do this instead:
#define MyPlotEventHandler(func) (&func)

// finally define a macro for creating the event table entries for the new
// event type
//
// remember that you don't need this at all if you only use Connect() and that
// you can replace MyPlotEventHandler(func) with just &func unless you use a
// really old compiler
#define MY_EVT_PLOT_CLICK(id, func) \
    wx__DECLARE_EVT1(MY_PLOT_CLICKED, id, MyPlotEventHandler(func))


// example of code handling the event (you will use one of these methods, not
// both, of course):
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_PLOT(ID_MY_WINDOW, MyFrame::OnPlot)
END_EVENT_TABLE()

MyFrame::MyFrame()
{
    Connect(ID_MY_WINDOW, MY_PLOT_CLICKED, &MyFrame::OnPlot);
}

void MyFrame::OnPlot(MyPlotEvent& event)
{
    ... do something with event.GetPoint() ...
}


// example of code generating the event:
void MyWindow::SendEvent()
{
    MyPlotEvent event(MY_PLOT_CLICKED, GetId(), wxPoint(...));
    event.SetEventObject(this);
    ProcessWindowEvent(event);
}
@endcode



@section overview_events_misc Miscellaneous Notes

@subsection overview_events_virtual Event Handlers vs Virtual Methods

It may be noted that wxWidgets' event processing system implements something
close to virtual methods in normal C++ in spirit: both of these mechanisms
allow you to alter the behaviour of the base class by defining the event handling
functions in the derived classes.

There is however an important difference between the two mechanisms when you
want to invoke the default behaviour, as implemented by the base class, from a
derived class handler. With the virtual functions, you need to call the base
class function directly and you can do it either in the beginning of the
derived class handler function (to post-process the event) or at its end (to
pre-process the event). With the event handlers, you only have the option of
pre-processing the events and in order to still let the default behaviour
happen you must call wxEvent::Skip() and @em not call the base class event
handler directly. In fact, the event handler probably doesn't even exist in the
base class as the default behaviour is often implemented in platform-specific
code by the underlying toolkit or OS itself. But even if it does exist at
wxWidgets level, it should never be called directly as the event handlers are
not part of wxWidgets API and should never be called directly.

Finally, please notice that the event handlers themselves shouldn't be virtual.
They should always be non-virtual and usually private (as there is no need to
make them public) methods of a wxEvtHandler-derived class.


@subsection overview_events_prog User Generated Events vs Programmatically Generated Events

While generically wxEvents can be generated both by user
actions (e.g., resize of a wxWindow) and by calls to functions
(e.g., wxWindow::SetSize), wxWidgets controls normally send wxCommandEvent-derived
events only for the user-generated events. The only @b exceptions to this rule are:

@li wxNotebook::AddPage: No event-free alternatives
@li wxNotebook::AdvanceSelection: No event-free alternatives
@li wxNotebook::DeletePage: No event-free alternatives
@li wxNotebook::SetSelection: Use wxNotebook::ChangeSelection instead, as
    wxNotebook::SetSelection is deprecated
@li wxTreeCtrl::Delete: No event-free alternatives
@li wxTreeCtrl::DeleteAllItems: No event-free alternatives
@li wxTreeCtrl::EditLabel: No event-free alternatives
@li All wxTextCtrl methods

wxTextCtrl::ChangeValue can be used instead of wxTextCtrl::SetValue but the other
functions, such as wxTextCtrl::Replace or wxTextCtrl::WriteText don't have event-free
equivalents.



@subsection overview_events_pluggable Pluggable Event Handlers

<em>TODO: Probably deprecated, Connect() provides a better way to do this</em>

In fact, you don't have to derive a new class from a window class
if you don't want to. You can derive a new class from wxEvtHandler instead,
defining the appropriate event table, and then call wxWindow::SetEventHandler
(or, preferably, wxWindow::PushEventHandler) to make this
event handler the object that responds to events. This way, you can avoid
a lot of class derivation, and use instances of the same event handler class (but different
objects as the same event handler object shouldn't be used more than once) to
handle events from instances of different widget classes.

If you ever have to call a window's event handler
manually, use the GetEventHandler function to retrieve the window's event handler and use that
to call the member function. By default, GetEventHandler returns a pointer to the window itself
unless an application has redirected event handling using SetEventHandler or PushEventHandler.

One use of PushEventHandler is to temporarily or permanently change the
behaviour of the GUI. For example, you might want to invoke a dialog editor
in your application that changes aspects of dialog boxes. You can
grab all the input for an existing dialog box, and edit it 'in situ',
before restoring its behaviour to normal. So even if the application
has derived new classes to customize behaviour, your utility can indulge
in a spot of body-snatching. It could be a useful technique for on-line
tutorials, too, where you take a user through a serious of steps and
don't want them to diverge from the lesson. Here, you can examine the events
coming from buttons and windows, and if acceptable, pass them through to
the original event handler. Use PushEventHandler/PopEventHandler
to form a chain of event handlers, where each handler processes a different
range of events independently from the other handlers.



@subsection overview_events_winid Window Identifiers

Window identifiers are integers, and are used to
uniquely determine window identity in the event system (though you can use it
for other purposes). In fact, identifiers do not need to be unique
across your entire application as long they are unique within the
particular context you're interested in, such as a frame and its children. You
may use the @c wxID_OK identifier, for example, on any number of dialogs
as long as you don't have several within the same dialog.

If you pass @c wxID_ANY to a window constructor, an identifier will be
generated for you automatically by wxWidgets. This is useful when you don't
care about the exact identifier either because you're not going to process the
events from the control being created or because you process the events
from all controls in one place (in which case you should specify @c wxID_ANY
in the event table or wxEvtHandler::Connect call
as well). The automatically generated identifiers are always negative and so
will never conflict with the user-specified identifiers which must be always
positive.

See @ref page_stdevtid for the list of standard identifiers available.
You can use wxID_HIGHEST to determine the number above which it is safe to
define your own identifiers. Or, you can use identifiers below wxID_LOWEST.
Finally, you can allocate identifiers dynamically using wxNewId() function too.
If you use wxNewId() consistently in your application, you can be sure that
your identifiers don't conflict accidentally.


@subsection overview_events_custom_generic Generic Event Table Macros

@beginTable
@row2col{EVT_CUSTOM(event\, id\, func),
        Allows you to add a custom event table
        entry by specifying the event identifier (such as wxEVT_SIZE),
        the window identifier, and a member function to call.}
@row2col{EVT_CUSTOM_RANGE(event\, id1\, id2\, func),
        The same as EVT_CUSTOM, but responds to a range of window identifiers.}
@row2col{EVT_COMMAND(id\, event\, func),
        The same as EVT_CUSTOM, but expects a member function with a
        wxCommandEvent argument.}
@row2col{EVT_COMMAND_RANGE(id1\, id2\, event\, func),
        The same as EVT_CUSTOM_RANGE, but
        expects a member function with a wxCommandEvent argument.}
@row2col{EVT_NOTIFY(event\, id\, func),
        The same as EVT_CUSTOM, but
        expects a member function with a wxNotifyEvent argument.}
@row2col{EVT_NOTIFY_RANGE(event\, id1\, id2\, func),
        The same as EVT_CUSTOM_RANGE, but
        expects a member function with a wxNotifyEvent argument.}
@endTable



@subsection overview_events_macros Event Handling Summary

For the full list of event classes, please see the
@ref group_class_events "event classes group page".


*/

