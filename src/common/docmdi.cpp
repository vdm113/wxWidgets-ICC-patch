/////////////////////////////////////////////////////////////////////////////
// Name:        docmdi.cpp
// Purpose:     Frame classes for MDI document/view applications
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "docmdi.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/defs.h"
#endif

#if USE_DOC_VIEW_ARCHITECTURE

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/docmdi.h"

/*
 * Docview MDI parent frame
 */

IMPLEMENT_CLASS(wxDocMDIParentFrame, wxMDIParentFrame)

BEGIN_EVENT_TABLE(wxDocMDIParentFrame, wxMDIParentFrame)
    EVT_MENU(wxID_EXIT, wxDocMDIParentFrame::OnExit)
    EVT_MENU_RANGE(wxID_FILE1, wxID_FILE2, wxDocMDIParentFrame::OnMRUFile)
END_EVENT_TABLE()

wxDocMDIParentFrame::wxDocMDIParentFrame(wxDocManager *manager, wxFrame *frame, wxWindowID id, const wxString& title,
  const wxPoint& pos, const wxSize& size, long style, const wxString& name):
  wxMDIParentFrame(frame, id, title, pos, size, style, name)
{
  m_docManager = manager;
}

void wxDocMDIParentFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
    Close();
}

void wxDocMDIParentFrame::OnMRUFile(wxCommandEvent& event)
{
      wxString f(m_docManager->GetHistoryFile(event.GetSelection() - wxID_FILE1));
      if (f != "")
        (void)m_docManager->CreateDocument(f, wxDOC_SILENT);
}

// Extend event processing to search the view's event table
bool wxDocMDIParentFrame::ProcessEvent(wxEvent& event)
{
    // Try the document manager, then do default processing
    if (!m_docManager || !m_docManager->ProcessEvent(event))
        return wxEvtHandler::ProcessEvent(event);
    else
        return TRUE;
}

// Define the behaviour for the frame closing
// - must delete all frames except for the main one.
bool wxDocMDIParentFrame::OnClose(void)
{
  return m_docManager->Clear(FALSE);
}


/*
 * Default document child frame for MDI children
 */

IMPLEMENT_CLASS(wxDocMDIChildFrame, wxMDIChildFrame)

BEGIN_EVENT_TABLE(wxDocMDIChildFrame, wxMDIChildFrame)
    EVT_ACTIVATE(wxDocMDIChildFrame::OnActivate)
END_EVENT_TABLE()

wxDocMDIChildFrame::wxDocMDIChildFrame(wxDocument *doc, wxView *view, wxMDIParentFrame *frame, wxWindowID  id,
  const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name):
    wxMDIChildFrame(frame, id, title, pos, size, style, name)
{
  m_childDocument = doc;
  m_childView = view;
  if (view)
    view->SetFrame(this);
}

wxDocMDIChildFrame::~wxDocMDIChildFrame(void)
{
}

// Extend event processing to search the view's event table
bool wxDocMDIChildFrame::ProcessEvent(wxEvent& event)
{
    if (m_childView)
        m_childView->Activate(TRUE);

	if ( !m_childView || ! m_childView->ProcessEvent(event) )
    {
        // Only hand up to the parent if it's a menu command
        if (!event.IsKindOf(CLASSINFO(wxCommandEvent)) || !GetParent() || !GetParent()->ProcessEvent(event))
		    return wxEvtHandler::ProcessEvent(event);
        else
            return TRUE;
    }
	else
		return TRUE;
}

void wxDocMDIChildFrame::OnActivate(wxActivateEvent& event)
{
  wxMDIChildFrame::OnActivate(event);

  if (m_childView)
    m_childView->Activate(event.GetActive());
}

bool wxDocMDIChildFrame::OnClose(void)
{
  // Close view but don't delete the frame while doing so!
  // ...since it will be deleted by wxWindows if we return TRUE.
  if (m_childView)
  {
    bool ans = m_childView->Close(FALSE); // FALSE means don't delete associated window
    if (ans)
    {
      m_childView->Activate(FALSE);
      delete m_childView;
      m_childView = NULL;
      m_childDocument = NULL;
    }
    
    return ans;
  }
  else return TRUE;
}

#endif
    // USE_DOC_VIEW_ARCHITECTURE

