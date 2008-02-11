/////////////////////////////////////////////////////////////////////////////
// Name:        src/generic/collpaneg.cpp
// Purpose:     wxGenericCollapsiblePane
// Author:      Francesco Montorsi
// Modified By:
// Created:     8/10/2006
// Id:          $Id$
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"
#include "wx/defs.h"

#if wxUSE_COLLPANE && wxUSE_BUTTON && wxUSE_STATLINE

#include "wx/collpane.h"

#ifndef WX_PRECOMP
    #include "wx/toplevel.h"
    #include "wx/button.h"
    #include "wx/sizer.h"
    #include "wx/panel.h"
#endif // !WX_PRECOMP

#include "wx/statline.h"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// ============================================================================
// implementation
// ============================================================================

const wxChar wxCollapsiblePaneNameStr[] = wxT("collapsiblePane");

//-----------------------------------------------------------------------------
// wxGenericCollapsiblePane
//-----------------------------------------------------------------------------

DEFINE_EVENT_TYPE(wxEVT_COMMAND_COLLPANE_CHANGED)
IMPLEMENT_DYNAMIC_CLASS(wxGenericCollapsiblePane, wxControl)
IMPLEMENT_DYNAMIC_CLASS(wxCollapsiblePaneEvent, wxCommandEvent)

BEGIN_EVENT_TABLE(wxGenericCollapsiblePane, wxControl)
    EVT_BUTTON(wxID_ANY, wxGenericCollapsiblePane::OnButton)
    EVT_SIZE(wxGenericCollapsiblePane::OnSize)

    WX_EVENT_TABLE_CONTROL_CONTAINER(wxGenericCollapsiblePane)
END_EVENT_TABLE()

WX_DELEGATE_TO_CONTROL_CONTAINER(wxGenericCollapsiblePane, wxControl)

void wxGenericCollapsiblePane::Init()
{
    WX_INIT_CONTROL_CONTAINER();

    m_pButton = NULL;
    m_pPane = NULL;
    m_pStaticLine = NULL;
    m_sz = NULL;
}

bool wxGenericCollapsiblePane::Create(wxWindow *parent,
                                      wxWindowID id,
                                      const wxString& label,
                                      const wxPoint& pos,
                                      const wxSize& size,
                                      long style,
                                      const wxValidator& val,
                                      const wxString& name)
{
    if ( !wxControl::Create(parent, id, pos, size, style, val, name) )
        return false;

    m_strLabel = label;

#ifdef __WXMAC__
    // on Mac we use the disclosure triangle
    // we need a light gray line above and below, lets approximate with the frame
    m_pStaticLine = NULL;
    m_pButton = new wxDisclosureTriangle( this, wxID_ANY, GetBtnLabel(), 
                                         wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER );
    m_pButton->SetBackgroundColour( wxColour( 221, 226, 239 ) );
    m_sz = new wxBoxSizer(wxHORIZONTAL);
    // m_sz->Add(4,4); where shall we put it?
    m_sz->Add( m_pButton, 1);
#else
    // create children and lay them out using a wxBoxSizer
    // (so that we automatically get RTL features)
    m_pButton = new wxButton(this, wxID_ANY, GetBtnLabel(), wxPoint(0, 0),
                             wxDefaultSize, wxBU_EXACTFIT);
    m_pStaticLine = new wxStaticLine(this, wxID_ANY);
    // on other platforms we put the static line and the button horizontally
    m_sz = new wxBoxSizer(wxHORIZONTAL);
    m_sz->Add(m_pButton, 0, wxLEFT|wxTOP|wxBOTTOM, GetBorder());
    m_sz->Add(m_pStaticLine, 1, wxALIGN_CENTER|wxLEFT|wxRIGHT, GetBorder());
#endif

    // FIXME: at least under wxCE and wxGTK1 the background is black if we don't do
    //        this, no idea why...
#if defined(__WXWINCE__) || defined(__WXGTK__)
    SetBackgroundColour(parent->GetBackgroundColour());
#endif

    // do not set sz as our sizers since we handle the pane window without using sizers
    m_pPane = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                          wxTAB_TRAVERSAL|wxNO_BORDER);

    // start as collapsed:
    m_pPane->Hide();

    return true;
}

wxGenericCollapsiblePane::~wxGenericCollapsiblePane()
{
    if (m_pButton)
        m_pButton->SetContainingSizer(NULL);
    
    if (m_pStaticLine)
        m_pStaticLine->SetContainingSizer(NULL);
    
    // our sizer is not deleted automatically since we didn't use SetSizer()!
    wxDELETE(m_sz);
}

wxSize wxGenericCollapsiblePane::DoGetBestSize() const
{
    // NB: do not use GetSize() but rather GetMinSize()
    wxSize sz = m_sz->GetMinSize();

    // when expanded, we need more vertical space
    if ( IsExpanded() )
    {
        sz.SetWidth(wxMax( sz.GetWidth(), m_pPane->GetBestSize().x ));
        sz.SetHeight(sz.y + GetBorder() + m_pPane->GetBestSize().y);
    }

    return sz;
}

wxString wxGenericCollapsiblePane::GetBtnLabel() const
{
    // on mac the triangle indicates the state, no string change
#ifdef __WXMAC__
    return m_strLabel;
#else
    return m_strLabel + (IsCollapsed() ? wxT(" >>") : wxT(" <<"));
#endif
}

void wxGenericCollapsiblePane::OnStateChange(const wxSize& sz)
{
    // minimal size has priority over the best size so set here our min size
    SetMinSize(sz);
    SetSize(sz);

    if (this->HasFlag(wxCP_NO_TLW_RESIZE))
    {
        // the user asked to explicitely handle the resizing itself...
        return;
    }


    //
    // NB: the following block of code has been accurately designed to
    //     as much flicker-free as possible; be careful when modifying it!
    //

    wxTopLevelWindow *
        top = wxDynamicCast(wxGetTopLevelParent(this), wxTopLevelWindow);
    if ( top )
    {
        // NB: don't Layout() the 'top' window as its size has not been correctly
        //     updated yet and we don't want to do an initial Layout() with the old
        //     size immediately followed by a SetClientSize/Fit call for the new
        //     size; that would provoke flickering!

        if (top->GetSizer())
#ifdef __WXGTK__
        // FIXME: the SetSizeHints() call would be required also for GTK+ for
        //        the expanded->collapsed transition. Unfortunately if we
        //        enable this line, then the GTK+ top window won't always be
        //        resized by the SetClientSize() call below! As a side effect
        //        of this dirty fix, the minimal size for the pane window is
        //        not set in GTK+ and the user can hide it shrinking the "top"
        //        window...
        if (IsCollapsed())
#endif
            top->GetSizer()->SetSizeHints(top);


        // we shouldn't attempt to resize a maximized window, whatever happens
        if ( !top->IsMaximized() )
        {
            if ( IsCollapsed() )
            {
                // expanded -> collapsed transition
                if (top->GetSizer())
                {
                    // we have just set the size hints...
                    wxSize szClient = top->GetSizer()->CalcMin();

                    // use SetClientSize() and not SetSize() otherwise the size for
                    // e.g. a wxFrame with a menubar wouldn't be correctly set
                    top->SetClientSize(szClient);
                }
                else
                    top->Layout();
            }
            else
            {
                // collapsed -> expanded transition

                // force our parent to "fit", i.e. expand so that it can honour
                // our minimal size
                top->Fit();
            }
        }
    }
}

void wxGenericCollapsiblePane::Collapse(bool collapse)
{
    // optimization
    if ( IsCollapsed() == collapse )
        return;

    // update our state
    m_pPane->Show(!collapse);

    // update button label
#ifdef __WXMAC__
    m_pButton->SetOpen( !collapse );
#else
    // NB: this must be done after updating our "state"
    m_pButton->SetLabel(GetBtnLabel());
#endif


    OnStateChange(GetBestSize());
}

void wxGenericCollapsiblePane::SetLabel(const wxString &label)
{
    m_strLabel = label;
#ifdef __WXMAC__
    m_pButton->SetLabel(GetBtnLabel());
#else
    m_pButton->SetLabel(GetBtnLabel());
    m_pButton->SetInitialSize();
#endif

    Layout();
}

bool wxGenericCollapsiblePane::Layout()
{
#ifdef __WXMAC__
    if (!m_pButton || !m_pPane || !m_sz)
        return false;     // we need to complete the creation first!
#else
    if (!m_pButton || !m_pStaticLine || !m_pPane || !m_sz)
        return false;     // we need to complete the creation first!
#endif

    wxSize oursz(GetSize());

    // move & resize the button and the static line
    m_sz->SetDimension(0, 0, oursz.GetWidth(), m_sz->GetMinSize().GetHeight());
    m_sz->Layout();

    if ( IsExpanded() )
    {
        // move & resize the container window
        int yoffset = m_sz->GetSize().GetHeight() + GetBorder();
        m_pPane->SetSize(0, yoffset,
                        oursz.x, oursz.y - yoffset);

        // this is very important to make the pane window layout show correctly
        m_pPane->Layout();
    }

    return true;
}

int wxGenericCollapsiblePane::GetBorder() const
{
#if defined( __WXMAC__ )
    return 6;
#elif defined(__WXMSW__)
    wxASSERT(m_pButton);
    return m_pButton->ConvertDialogToPixels(wxSize(2, 0)).x;
#else
    return 5;
#endif
}



//-----------------------------------------------------------------------------
// wxGenericCollapsiblePane - event handlers
//-----------------------------------------------------------------------------

void wxGenericCollapsiblePane::OnButton(wxCommandEvent& event)
{
    if ( event.GetEventObject() != m_pButton )
    {
        event.Skip();
        return;
    }

    Collapse(!IsCollapsed());

    // this change was generated by the user - send the event
    wxCollapsiblePaneEvent ev(this, GetId(), IsCollapsed());
    GetEventHandler()->ProcessEvent(ev);
}

void wxGenericCollapsiblePane::OnSize(wxSizeEvent& WXUNUSED(event))
{
#if 0       // for debug only
    wxClientDC dc(this);
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawRectangle(wxPoint(0,0), GetSize());
    dc.SetPen(*wxRED_PEN);
    dc.DrawRectangle(wxPoint(0,0), GetBestSize());
#endif

    Layout();
}

#endif // wxUSE_COLLPANE && wxUSE_BUTTON && wxUSE_STATLINE
