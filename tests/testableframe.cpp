/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        testableframe.cpp
// Purpose:     An improved wxFrame for unit-testing
// Author:      Steven Lamerton
// Copyright:   (c) 2010 Steven Lamerton
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "testprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/app.h"
#include "testableframe.h"

wxTestableFrame::wxTestableFrame() : wxFrame(NULL, wxID_ANY, "Test Frame")
{
    // Use fixed position to facilitate debugging.
    Move(200, 200);
}

void wxTestableFrame::OnEvent(wxEvent& evt)
{
    m_count[evt.GetEventType()]++;

    if(! evt.IsCommandEvent() )
        evt.Skip();
}

int wxTestableFrame::GetEventCount(wxEventType type)
{
    return m_count[type];
}

void wxTestableFrame::ClearEventCount(wxEventType type)
{
    m_count[type] = 0;
}

EventCounter::EventCounter(wxWindow* win, wxEventType type) : m_type(type),
                                                              m_win(win)

{
    m_frame = wxStaticCast(wxTheApp->GetTopWindow(), wxTestableFrame);

    m_win->Connect(m_type, wxEventHandler(wxTestableFrame::OnEvent),
                   NULL, m_frame);
}

EventCounter::~EventCounter()
{
    m_win->Disconnect(m_type, wxEventHandler(wxTestableFrame::OnEvent),
                      NULL, m_frame);

    //This stops spurious counts from previous tests
    Clear();

    m_frame = NULL;
    m_win = NULL;
}
