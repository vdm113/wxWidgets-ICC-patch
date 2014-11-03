/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/infobar.h
// Purpose:     interface of wxInfoBar
// Author:      Vadim Zeitlin
// Copyright:   (c) 2009 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
    An info bar is a transient window shown at top or bottom of its parent
    window to display non-critical information to the user.

    This class provides another way to show messages to the user, intermediate
    between message boxes and status bar messages. The message boxes are modal
    and thus interrupt the users work flow and should be used sparingly for
    this reason. However status bar messages are often too easy not to notice
    at all. An info bar provides a way to present the messages which has a much
    higher chance to be noticed by the user but without being annoying.

    Info bar may show an icon (on the left), text message and, optionally,
    buttons allowing the user to react to the information presented. It always
    has a close button at the right allowing the user to dismiss it so it isn't
    necessary to provide a button just to close it.

    wxInfoBar calls its parent wxWindow::Layout() method and assumes that it
    will change the parent layout appropriately depending on whether the info
    bar itself is shown or hidden. Usually this is achieved by simply using a
    sizer for the parent window layout and adding wxInfoBar to this sizer as
    one of the items. Considering the usual placement of the info bars,
    normally this sizer should be a vertical wxBoxSizer and the bar its first
    or last element so the simplest possible example of using this class would
    be:
    @code
    class MyFrame : public wxFrame
    {
        ...

        wxInfoBar *m_infoBar;
    };

    MyFrame::MyFrame()
    {
        ...
        m_infoBar = new wxInfoBar(this);

        wxSizer *sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(m_infoBar, wxSizerFlags().Expand());
        ... add other frame controls to the sizer ...
        SetSizer(sizer);
    }

    void MyFrame::SomeMethod()
    {
        m_infoBar->ShowMessage("Something happened", wxICON_INFORMATION);
    }
    @endcode

    See the dialogs sample for more sophisticated examples.


    Currently this class is implemented generically (i.e. in the same
    platform-independent way for all ports) and also natively in wxGTK but the
    native implementation requires a recent -- as of this writing -- GTK+ 2.18
    version.

    @library{wxcore}
    @category{miscwnd}

    @see wxStatusBar, wxMessageDialog

    @since 2.9.1
*/
class wxInfoBar : public wxControl
{
public:
    /**
        Default constructor.

        Use Create() for the objects created using this constructor.
     */
    wxInfoBar();

    /**
        Constructor creating the info bar window.

        @see Create()
     */
    wxInfoBar(wxWindow *parent, wxWindowID winid = wxID_ANY);

    /**
        Create the info bar window.

        Notice that unlike most of the other wxWindow-derived classes,
        wxInfoBar is created hidden and is only shown when ShowMessage() is
        called. This is more convenient as usually the info bar is created to
        be shown at some later time and not immediately and so creating it
        hidden avoids the need to call Hide() explicitly from the code using
        it.

        This should be only called if the object was created using its default
        constructor.

        @param parent
            A valid parent window pointer.
        @param winid
            The id of the info bar window, usually unused as currently no
            events are generated by this class.
     */
    bool Create(wxWindow *parent, wxWindowID winid = wxID_ANY);

    /**
        Add a button to be shown in the info bar.

        The button added by this method will be shown to the right of the text
        (in LTR layout), with each successive button being added to the right
        of the previous one. If any buttons are added to the info bar using
        this method, the default "Close" button is not shown as it is assumed
        that the extra buttons already allow the user to close it.

        Clicking the button will generate a normal EVT_COMMAND_BUTTON_CLICKED
        event which can be handled as usual. The default handler in wxInfoBar
        itself closes the window whenever a button in it is clicked so if you
        wish the info bar to be hidden when the button is clicked, simply call
        @c event.Skip() in the button handler to let the base class handler do
        it (calling Dismiss() explicitly works too, of course). On the other
        hand, if you don't skip the event, the info bar will remain opened so
        make sure to do it for at least some buttons to allow the user to close
        it.

        Notice that the generic wxInfoBar implementation handles the button
        events itself and so they are not propagated to the info bar parent and
        you need to either inherit from wxInfoBar and handle them in your
        derived class or use wxEvtHandler::Connect(), as is done in the dialogs
        sample, to handle the button events in the parent frame.

        @param btnid
            Id of the button. It will be used in the button message clicking
            this button will generate.
        @param label
            The label of the button. It may only be empty if @a btnid is one of
            the stock ids in which case the corresponding stock label (see
            wxGetStockLabel()) will be used.
     */
    void AddButton(wxWindowID btnid, const wxString& label = wxString());

    /**
        Hide the info bar window.

        This method hides the window and lays out the parent window to account
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif
        for its disappearance (unlike a simple Hide()).
     */
    virtual void Dismiss();

    /**
        Remove a button previously added by AddButton().

        @param btnid
            Id of the button to remove. If more than one button with the same
            id is used in the info bar (which is in any case not recommended),
            the last, i.e. most recently added, button with this id is removed.
     */
    void RemoveButton(wxWindowID btnid);

    /**
        Show a message in the bar.

        If the bar is currently hidden, it will be shown. Otherwise its message
        will be updated in place.

        @param msg
            The text of the message.
        @param flags
            One of wxICON_NONE, wxICON_INFORMATION (default), wxICON_QUESTION,
            wxICON_WARNING or wxICON_ERROR values. These flags have the same
            meaning as in wxMessageDialog for the generic version, i.e. show
            (or not, in case of wxICON_NONE) the corresponding icon in the bar
            but can be interpreted by the native versions. For example, the
            GTK+ native implementation doesn't show icons at all but uses this
            parameter to select the appropriate background colour for the
            notification.
     */
    void ShowMessage(const wxString& msg, int flags = wxICON_NONE);

    /**
        Returns the number of currently shown buttons.

        This is simply the number of calls to AddButton() minus the number
        of calls to RemoveButton() so far.

        @return The number of currently shown buttons, possibly 0.

        @since 3.1.0
     */
    virtual size_t GetButtonCount() const;

    /**
        Returns the ID of the button at the given position.

        The positions of the buttons are counted in order of their addition.

        @param idx
            The position of the button in 0 to GetButtonCount() range.
        @return
            The ID of the button at the given position or wxID_NONE if it
            is out of range (this also results in an assertion failure).

        @since 3.1.0
     */
    virtual wxWindowID GetButtonId(size_t idx) const;

    /**
        Returns whether a button with the given ID is currently shown.

        @param btnid
            ID of the button to check for.
        @return
            \true if the button with this ID is currently shown.

        @since 3.1.0
     */
    virtual bool HasButtonId(wxWindowID btnid) const;

    /**
        @name Generic version customization methods.

        All these methods exist in the generic version of the class only.

        The generic version uses wxWindow::ShowWithEffect() function to
        progressively show it on the platforms which support it (currently only
        wxMSW). The methods here allow to change the default effect used (or
        disable it entirely) and change its duration.
     */
    //@{

    /**
        Set the effects to use when showing and hiding the bar.

        Either or both of the parameters can be set to wxSHOW_EFFECT_NONE to
        disable using effects entirely.

        By default, the info bar uses wxSHOW_EFFECT_SLIDE_TO_BOTTOM effect for
        showing itself and wxSHOW_EFFECT_SLIDE_TO_TOP for hiding if it is the
        first element of the containing sizer and reverse effects if it's the
        last one. If it is neither the first nor the last element, no effect is
        used to avoid the use of an inappropriate one and this function must be
        called if an effect is desired.

        @param showEffect
            The effect to use when showing the bar.
        @param hideEffect
            The effect to use when hiding the bar.
     */
    void SetShowHideEffects(wxShowEffect showEffect, wxShowEffect hideEffect);

    /// Return the effect currently used for showing the bar.
    wxShowEffect GetShowEffect() const;

    /// Return the effect currently used for hiding the bar.
    wxShowEffect GetHideEffect() const;

    /**
        Set the duration of the animation used when showing or hiding the bar.

        By default, 500ms duration is used.

        @param duration
            Duration of the animation, in milliseconds.
     */
    void SetEffectDuration(int duration);

    /// Return the effect animation duration currently used.
    int GetEffectDuration() const;

    /**
        Overridden base class methods changes the font of the text message.

        wxInfoBar overrides this method to use the font passed to it for its
        text message part. By default a larger and bold version of the standard
        font is used.

        This method is generic-only.
     */
    virtual bool SetFont(const wxFont& font);

    //@}
};
