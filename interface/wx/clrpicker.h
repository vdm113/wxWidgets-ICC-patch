/////////////////////////////////////////////////////////////////////////////
// Name:        clrpicker.h
// Purpose:     interface of wxColourPickerCtrl
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#define wxCLRP_USE_TEXTCTRL       (wxPB_USE_TEXTCTRL)
#define wxCLRP_DEFAULT_STYLE      0
#define wxCLRP_SHOW_LABEL         0x0008

wxEventType wxEVT_COMMAND_COLOURPICKER_CHANGED;


/**
    @class wxColourPickerCtrl

    This control allows the user to select a colour. The generic implementation
    is a button which brings up a wxColourDialog when clicked. Native
    implementation may differ but this is usually a (small) widget which give
    access to the colour-chooser dialog. It is only available if
    @c wxUSE_COLOURPICKERCTRL is set to 1 (the default).

    @beginStyleTable
    @style{wxCLRP_DEFAULT_STYLE}
           The default style: 0.
    @style{wxCLRP_USE_TEXTCTRL}
           Creates a text control to the left of the picker button which is
           completely managed by the wxColourPickerCtrl and which can be used
           by the user to specify a colour (see SetColour). The text control
           is automatically synchronized with button's value. Use functions
           defined in wxPickerBase to modify the text control.
    @style{wxCLRP_SHOW_LABEL}
           Shows the colour in HTML form (AABBCC) as colour button label
           (instead of no label at all).
    @endStyleTable

    @beginEventEmissionTable{wxColourPickerEvent}
    @event{EVT_COLOURPICKER_CHANGED(id, func)}
           The user changed the colour selected in the control either using the
           button or using text control (see @c wxCLRP_USE_TEXTCTRL; note that
           in this case the event is fired only if the user’s input is valid,
           i.e. recognizable).
    @endEventTable

    @library{wxcore}
    @category{pickers}
    @appearance{colourpickerctrl.png}

    @see wxColourDialog, wxColourPickerEvent
*/
class wxColourPickerCtrl : public wxPickerBase
{
public:
    wxColourPickerCtrl();
    
    /**
        Initializes the object and calls Create() with all the parameters.
    */
    wxColourPickerCtrl(wxWindow* parent, wxWindowID id,
                       const wxColour& colour = *wxBLACK,
                       const wxPoint& pos = wxDefaultPosition,
                       const wxSize& size = wxDefaultSize,
                       long style = wxCLRP_DEFAULT_STYLE,
                       const wxValidator& validator = wxDefaultValidator,
                       const wxString& name = wxColourPickerCtrlNameStr);

    /**
        Creates a colour picker with the given arguments.

        @param parent
            Parent window, must not be non-@NULL.
        @param id
            The identifier for the control.
        @param colour
            The initial colour shown in the control.
        @param pos
            Initial position.
        @param size
            Initial size.
        @param style
            The window style, see wxCRLP_* flags.
        @param validator
            Validator which can be used for additional date checks.
        @param name
            Control name.

        @return @true if the control was successfully created or @false if
                creation failed.
    */
    bool Create(wxWindow* parent, wxWindowID id,
                const wxColour& colour = *wxBLACK,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxCLRP_DEFAULT_STYLE,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxColourPickerCtrlNameStr);

    /**
        Returns the currently selected colour.
    */
    wxColour GetColour() const;

    //@{
    /**
        Sets the currently selected colour. See wxColour::Set().
    */
    void SetColour(const wxColour& col);
    void SetColour(const wxString& colname);
    //@}
};



/**
    @class wxColourPickerEvent

    This event class is used for the events generated by wxColourPickerCtrl.

    @beginEventTable{wxColourPickerEvent}
    @event{EVT_COLOURPICKER_CHANGED(id, func)}
           Generated whenever the selected colour changes.
    @endEventTable

    @library{wxcore}
    @category{events}

    @see wxColourPickerCtrl
*/
class wxColourPickerEvent : public wxCommandEvent
{
public:
    wxColourPickerEvent();

    /**
        The constructor is not normally used by the user code.
    */
    wxColourPickerEvent(wxObject* generator, int id,
                        const wxColour& colour);

    /**
        Retrieve the colour the user has just selected.
    */
    wxColour GetColour() const;

    /**
        Set the colour associated with the event.
    */
    void SetColour(const wxColour& pos);
};

