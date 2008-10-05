/////////////////////////////////////////////////////////////////////////////
// Name:        samples/propgrid/propgrid_minimal.cpp
// Purpose:     Minimal portion of wxPropertyGrid sample
// Author:      Jaakko Salli
// Modified by:
// Created:     2008-08-23
// RCS-ID:      $Id$
// Copyright:   (c) Jaakko Salli
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#include "wx/wx.h"
#include "wx/propgrid/propgrid.h"

class MyFrame : public wxFrame 
{ 
public: 
    MyFrame(wxWindow* parent); 
 
    void OnAction(wxCommandEvent& event); 
    void OnPropertyGridChange(wxPropertyGridEvent& event); 

private:
    wxPropertyGrid* m_pg;
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_HIGHEST+1, MyFrame::OnAction)
    EVT_PG_CHANGED( -1, MyFrame::OnPropertyGridChange )
END_EVENT_TABLE()

MyFrame::MyFrame(wxWindow* parent)
    : wxFrame(parent, wxID_ANY, wxT("PropertyGrid Test"))
{
    wxMenu *Menu = new wxMenu; 
    Menu->Append(wxID_HIGHEST+1, wxT("Action")); 
    wxMenuBar *MenuBar = new wxMenuBar(); 
    MenuBar->Append(Menu, wxT("Action")); 
    SetMenuBar(MenuBar); 

    wxPropertyGrid *pg = new wxPropertyGrid(this,-1,wxDefaultPosition,wxSize(400,400),
                        wxPG_SPLITTER_AUTO_CENTER |
                        wxPG_BOLD_MODIFIED );
    m_pg = pg;

    pg->Append( new wxStringProperty("String Property", wxPG_LABEL) );
    pg->Append( new wxIntProperty("Int Property", wxPG_LABEL) );
    pg->Append( new wxBoolProperty("Bool Property", wxPG_LABEL) );

    SetSize(400, 600);
}

void MyFrame::OnPropertyGridChange(wxPropertyGridEvent &event)
{
    wxPGProperty* p = event.GetProperty();

    if ( p )
        wxLogDebug("OnPropertyGridChange(%s)", p->GetName().c_str());
    else
        wxLogDebug("OnPropertyGridChange(NULL)");
}

void MyFrame::OnAction(wxCommandEvent &) 
{
}

// Called from propgridsample.cpp
//
void DisplayMinimalFrame(wxWindow* parent)
{
    MyFrame *frame = new MyFrame(parent);
    frame->Show(true);
}
