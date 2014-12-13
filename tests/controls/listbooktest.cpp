/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        tests/controls/listbooktest.cpp
// Purpose:     wxListbook unit test
// Author:      Steven Lamerton
// Created:     2010-07-02
// Copyright:   (c) 2010 Steven Lamerton
///////////////////////////////////////////////////////////////////////////////

#include "testprec.h"

#if wxUSE_LISTBOOK

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/panel.h"
#endif // WX_PRECOMP

#include "wx/listbook.h"
#include "wx/listctrl.h"
#include "bookctrlbasetest.h"

class ListbookTestCase : public BookCtrlBaseTestCase, public CppUnit::TestCase
{
public:
    ListbookTestCase() { }

    virtual void setUp();
    virtual void tearDown();

private:
    virtual wxBookCtrlBase *GetBase() const { return m_listbook; }

    virtual wxEventType GetChangedEvent() const
    { return wxEVT_LISTBOOK_PAGE_CHANGED; }

    virtual wxEventType GetChangingEvent() const
    { return wxEVT_LISTBOOK_PAGE_CHANGING; }

    CPPUNIT_TEST_SUITE( ListbookTestCase );
        wxBOOK_CTRL_BASE_TESTS();
        CPPUNIT_TEST( ListView );
    CPPUNIT_TEST_SUITE_END();

    void ListView();

    wxListbook *m_listbook;

    DECLARE_NO_COPY_CLASS(ListbookTestCase)
};

// register in the unnamed registry so that these tests are run by default
CPPUNIT_TEST_SUITE_REGISTRATION( ListbookTestCase );

// also include in its own registry so that these tests can be run alone
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ListbookTestCase, "ListbookTestCase" );

void ListbookTestCase::setUp()
{
    m_listbook = new wxListbook(wxTheApp->GetTopWindow(), wxID_ANY,
                                wxDefaultPosition, wxSize(400, 300));
    AddPanels();
}

void ListbookTestCase::tearDown()
{
    wxDELETE(m_listbook);
}

void ListbookTestCase::ListView()
{
    wxListView* listview = m_listbook->GetListView();

    CPPUNIT_ASSERT(listview);
    CPPUNIT_ASSERT_EQUAL(3, listview->GetItemCount());
    CPPUNIT_ASSERT_EQUAL("Panel 1", listview->GetItemText(0));
}

#endif //wxUSE_LISTBOOK
