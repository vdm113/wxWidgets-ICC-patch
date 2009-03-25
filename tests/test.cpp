///////////////////////////////////////////////////////////////////////////////
// Name:        test.cpp
// Purpose:     Test program for wxWidgets
// Author:      Mike Wetherell
// RCS-ID:      $Id$
// Copyright:   (c) 2004 Mike Wetherell
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h"
// and "wx/cppunit.h"
#include "testprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/beforestd.h"
#ifdef __VISUALC__
    #pragma warning(disable:4100)
#endif
#include <cppunit/TestListener.h>
#ifdef __VISUALC__
    #pragma warning(default:4100)
#endif
#include <cppunit/Protector.h>
#include <cppunit/Test.h>
#include <cppunit/TestResult.h>
#include "wx/afterstd.h"

#include "wx/cmdline.h"
#include <iostream>

#ifdef __WXMSW__
    #include "wx/msw/msvcrt.h"
#endif

using namespace std;

using CppUnit::Test;
using CppUnit::TestSuite;
using CppUnit::TestFactoryRegistry;

// exception class for MSVC debug CRT assertion failures
#ifdef wxUSE_VC_CRTDBG

struct CrtAssertFailure
{
    CrtAssertFailure(const char *message) : m_msg(message) { }

    const wxString m_msg;

    wxDECLARE_NO_ASSIGN_CLASS(CrtAssertFailure);
};

#endif // wxUSE_VC_CRTDBG

// this function should only be called from a catch clause
static string GetExceptionMessage()
{
    wxString msg;

    try
    {
        throw;
    }
#if wxDEBUG_LEVEL
    catch ( TestAssertFailure& e )
    {
        msg << "wxWidgets assert: " << e.m_cond << " failed "
               "at " << e.m_file << ":" << e.m_line << " in " << e.m_func
            << " with message " << e.m_msg;
    }
#endif // wxDEBUG_LEVEL
#ifdef wxUSE_VC_CRTDBG
    catch ( CrtAssertFailure& e )
    {
        msg << "CRT assert failure: " << e.m_msg;
    }
#endif // wxUSE_VC_CRTDBG
    catch ( std::exception& e )
    {
        msg << "std::exception: " << e.what();
    }
    catch ( ... )
    {
        msg = "Unknown exception caught.";
    }

    return string(msg.mb_str());
}

// Protector adding handling of wx-specific (this includes MSVC debug CRT in
// this context) exceptions
class wxUnitTestProtector : public CppUnit::Protector
{
public:
    virtual bool protect(const CppUnit::Functor &functor,
                         const CppUnit::ProtectorContext& context)
    {
        try
        {
            return functor();
        }
        catch ( std::exception& e )
        {
            // cppunit deals with the standard exceptions itself, let it do as
            // it output more details (especially for std::exception-derived
            // CppUnit::Exception) than we do
            throw;
        }
        catch ( ... )
        {
            reportError(context, CppUnit::Message("Uncaught exception",
                                                  GetExceptionMessage()));
        }

        return false;
    }
};

// Displays the test name before starting to execute it: this helps with
// diagnosing where exactly does a test crash or hang when/if it does.
class DetailListener : public CppUnit::TestListener
{
public:
    DetailListener(bool doTiming = false):
        CppUnit::TestListener(),
        m_timing(doTiming)
    {
    }

    virtual void startTest(CppUnit::Test *test)
    {
        std::cout << test->getName () << " ";
        m_watch.Start();
    }

    virtual void endTest(CppUnit::Test * WXUNUSED(test))
    {
        m_watch.Pause();
        if ( m_timing )
            std::cout << " (in "<< m_watch.Time() << " ms )";
        std::cout << "\n";
    }

protected :
    bool m_timing;
    wxStopWatch m_watch;
};

#if wxUSE_GUI
    typedef wxApp TestAppBase;
#else
    typedef wxAppConsole TestAppBase;
#endif

// The application class
//
class TestApp : public TestAppBase
{
public:
    TestApp();

    // standard overrides
    virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
    virtual bool OnInit();
    virtual int  OnRun();
    virtual int  OnExit();

    // used by events propagation test
    virtual int FilterEvent(wxEvent& event);
    virtual bool ProcessEvent(wxEvent& event);

    void SetFilterEventFunc(FilterEventFunc f) { m_filterEventFunc = f; }
    void SetProcessEventFunc(ProcessEventFunc f) { m_processEventFunc = f; }

private:
    void List(Test *test, const string& parent = "") const;

    // command lines options/parameters
    bool m_list;
    bool m_longlist;
    bool m_detail;
    bool m_timing;
    wxArrayString m_registries;

    // event handling hooks
    FilterEventFunc m_filterEventFunc;
    ProcessEventFunc m_processEventFunc;
};

IMPLEMENT_APP_NO_MAIN(TestApp)

#ifdef wxUSE_VC_CRTDBG

static int TestCrtReportHook(int reportType, char *message, int *)
{
    if ( reportType != _CRT_ASSERT )
        return FALSE;

    throw CrtAssertFailure(message);
}

#endif // wxUSE_VC_CRTDBG

#if wxDEBUG_LEVEL

static void TestAssertHandler(const wxString& file,
                              int line,
                              const wxString& func,
                              const wxString& cond,
                              const wxString& msg)
{
    throw TestAssertFailure(file, line, func, cond, msg);
}

#endif // wxDEBUG_LEVEL

int main(int argc, char **argv)
{
    // tests can be ran non-interactively so make sure we don't show any assert
    // dialog boxes -- neither our own nor from MSVC debug CRT -- which would
    // prevent them from completing

#if wxDEBUG_LEVEL
    wxSetAssertHandler(TestAssertHandler);
#endif // wxDEBUG_LEVEL

#ifdef wxUSE_VC_CRTDBG
    _CrtSetReportHook(TestCrtReportHook);
#endif // wxUSE_VC_CRTDBG

    try
    {
        return wxEntry(argc, argv);
    }
    catch ( ... )
    {
        cerr << "\n" << GetExceptionMessage() << endl;
    }

    return -1;
}

TestApp::TestApp()
  : m_list(false),
    m_longlist(false)
{
    m_filterEventFunc = NULL;
    m_processEventFunc = NULL;
}

// Init
//
bool TestApp::OnInit()
{
    if ( !TestAppBase::OnInit() )
        return false;

    cout << "Test program for wxWidgets\n"
         << "build: " << WX_BUILD_OPTIONS_SIGNATURE << std::endl;

#if wxUSE_GUI
    // create a hidden parent window to be used as parent for the GUI controls
    new wxFrame(NULL, wxID_ANY, "Hidden wx test frame");
#endif // wxUSE_GUI

    return true;
}

// The table of command line options
//
void TestApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    TestAppBase::OnInitCmdLine(parser);

    static const wxCmdLineEntryDesc cmdLineDesc[] = {
        { wxCMD_LINE_SWITCH, "l", "list",
            "list the test suites, do not run them",
            wxCMD_LINE_VAL_NONE, 0 },
        { wxCMD_LINE_SWITCH, "L", "longlist",
            "list the test cases, do not run them",
            wxCMD_LINE_VAL_NONE, 0 },
        { wxCMD_LINE_SWITCH, "d", "detail",
            "print the test case names, run them",
            wxCMD_LINE_VAL_NONE, 0 },
        { wxCMD_LINE_SWITCH, "t", "timing",
            "print names and mesure running time of individual test, run them",
            wxCMD_LINE_VAL_NONE, 0 },
        { wxCMD_LINE_PARAM, NULL, NULL, "REGISTRY", wxCMD_LINE_VAL_STRING,
            wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_PARAM_MULTIPLE },
        wxCMD_LINE_DESC_END
    };

    parser.SetDesc(cmdLineDesc);
}

// Handle command line options
//
bool TestApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    if (parser.GetParamCount())
        for (size_t i = 0; i < parser.GetParamCount(); i++)
            m_registries.push_back(parser.GetParam(i));
    else
        m_registries.push_back("");

    m_longlist = parser.Found(_T("longlist"));
    m_list = m_longlist || parser.Found(_T("list"));
    m_timing = parser.Found(_T("timing"));
    m_detail = !m_timing && parser.Found(_T("detail"));

    return TestAppBase::OnCmdLineParsed(parser);
}

// Event handling
int TestApp::FilterEvent(wxEvent& event)
{
    if ( m_filterEventFunc )
        return (*m_filterEventFunc)(event);

    return TestAppBase::FilterEvent(event);
}

bool TestApp::ProcessEvent(wxEvent& event)
{
    if ( m_processEventFunc )
        return (*m_processEventFunc)(event);

    return TestAppBase::ProcessEvent(event);
}

extern void SetFilterEventFunc(FilterEventFunc func)
{
    wxGetApp().SetFilterEventFunc(func);
}

extern void SetProcessEventFunc(ProcessEventFunc func)
{
    wxGetApp().SetProcessEventFunc(func);
}

// Run
//
int TestApp::OnRun()
{
    CppUnit::TextTestRunner runner;

    for (size_t i = 0; i < m_registries.size(); i++)
    {
        wxString reg = m_registries[i];
        if (!reg.empty() && !reg.EndsWith("TestCase"))
            reg += "TestCase";
        // allow the user to specify the name of the testcase "in short form"
        // (all wx test cases end with TestCase postfix)

        auto_ptr<Test> test(reg.empty() ?
            TestFactoryRegistry::getRegistry().makeTest() :
            TestFactoryRegistry::getRegistry(string(reg.mb_str())).makeTest());

        TestSuite *suite = dynamic_cast<TestSuite*>(test.get());

        if (suite && suite->countTestCases() == 0)
            wxLogError(_T("No such test suite: %s"), reg);
        else if (m_list)
            List(test.get());
        else
            runner.addTest(test.release());
    }

    if ( m_list )
        return EXIT_SUCCESS;

    runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(), cout));

#if wxUSE_LOG
    // Switch off logging unless --verbose
    bool verbose = wxLog::GetVerbose();
    wxLog::EnableLogging(verbose);
#else
    bool verbose = false;
#endif

    // there is a bug
    // (http://sf.net/tracker/index.php?func=detail&aid=1649369&group_id=11795&atid=111795)
    // in some versions of cppunit: they write progress dots to cout (and not
    // cerr) and don't flush it so all the dots appear at once at the end which
    // is not very useful so unbuffer cout to work around this
    cout.setf(ios::unitbuf);

    // add detail listener if needed
    DetailListener detailListener(m_timing);
    if ( m_detail || m_timing )
        runner.eventManager().addListener(&detailListener);

    // finally ensure that we report our own exceptions nicely instead of
    // giving "uncaught exception of unknown type" messages
    runner.eventManager().pushProtector(new wxUnitTestProtector);

    return runner.run("", false, true, !verbose) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int TestApp::OnExit()
{
#if wxUSE_GUI
    delete GetTopWindow();
#endif // wxUSE_GUI

    return 0;
}

// List the tests
//
void TestApp::List(Test *test, const string& parent /*=""*/) const
{
    TestSuite *suite = dynamic_cast<TestSuite*>(test);
    string name;

    if (suite) {
        // take the last component of the name and append to the parent
        name = test->getName();
        string::size_type i = name.find_last_of(".:");
        if (i != string::npos)
            name = name.substr(i + 1);
        name = parent + "." + name;

        // drop the 1st component from the display and indent
        if (parent != "") {
            string::size_type j = i = name.find('.', 1);
            while ((j = name.find('.', j + 1)) != string::npos)
                cout << "  ";
            cout << "  " << name.substr(i + 1) << "\n";
        }

        typedef vector<Test*> Tests;
        typedef Tests::const_iterator Iter;

        const Tests& tests = suite->getTests();

        for (Iter it = tests.begin(); it != tests.end(); ++it)
            List(*it, name);
    }
    else if (m_longlist) {
        string::size_type i = 0;
        while ((i = parent.find('.', i + 1)) != string::npos)
            cout << "  ";
        cout << "  " << test->getName() << "\n";
    }
}
