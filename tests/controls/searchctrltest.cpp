/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


///////////////////////////////////////////////////////////////////////////////
// Name:        tests/controls/searchctrltest.cpp
// Purpose:     wxSearchCtrl unit test
// Author:      Vadim Zeitlin
// Created:     2013-01-20
// Copyright:   (c) 2013 Vadim Zeitlin <vadim@wxwidgets.org>
///////////////////////////////////////////////////////////////////////////////

#include "testprec.h"

#if wxUSE_SEARCHCTRL

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/app.h"
#endif // WX_PRECOMP

#include "wx/srchctrl.h"

class SearchCtrlTestCase : public CppUnit::TestCase
{
public:
    SearchCtrlTestCase() { }

    virtual void setUp();
    virtual void tearDown();

private:
    CPPUNIT_TEST_SUITE( SearchCtrlTestCase );
        CPPUNIT_TEST( Focus );
    CPPUNIT_TEST_SUITE_END();

    void Focus();

    wxSearchCtrl* m_search;

    wxDECLARE_NO_COPY_CLASS(SearchCtrlTestCase);
};

// register in the unnamed registry so that these tests are run by default
CPPUNIT_TEST_SUITE_REGISTRATION( SearchCtrlTestCase );

// also include in its own registry so that these tests can be run alone
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SearchCtrlTestCase, "SearchCtrlTestCase" );

void SearchCtrlTestCase::setUp()
{
    m_search = new wxSearchCtrl(wxTheApp->GetTopWindow(), wxID_ANY);
}

void SearchCtrlTestCase::tearDown()
{
    delete m_search;
    m_search = NULL;
}

void SearchCtrlTestCase::Focus()
{
    // TODO OS X test only passes when run solo ...
#ifndef __WXOSX__
    m_search->SetFocus();
    CPPUNIT_ASSERT( m_search->HasFocus() );
#endif
}

#endif // wxUSE_SEARCHCTRL
