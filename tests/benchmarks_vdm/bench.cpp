/////////////////////////////////////////////////////////////////////////////
// Name:        tests/benchmarks/bench.cpp
// Purpose:     Main file of the benchmarking suite
// Author:      Vadim Zeitlin
// Created:     2008-07-19
// Copyright:   (c) 2008 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/app.h"
#include "wx/cmdline.h"
#include "wx/stopwatch.h"

#if wxUSE_GUI
    #include "wx/frame.h"
#endif

#include "bench.h"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

static const char OPTION_LIST = 'l';
static const char OPTION_SINGLE = '1';

static const char OPTION_AVG_COUNT = 'a';
static const char OPTION_NUM_RUNS = 'n';
static const char OPTION_NUMERIC_PARAM = 'p';
static const char OPTION_STRING_PARAM = 's';

char tmp[4096];

// ----------------------------------------------------------------------------
// BenchApp declaration
// ----------------------------------------------------------------------------

#if wxUSE_GUI
    typedef wxApp BenchAppBase;
#else
    typedef wxAppConsole BenchAppBase;
#endif

class BenchApp : public BenchAppBase
{
public:
    BenchApp();

    // standard overrides
    virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
    virtual bool OnInit();
    virtual int  OnRun();
    virtual int  OnExit();

    // accessors
    int GetNumericParameter() const { return m_numParam; }
    const wxString& GetStringParameter() const { return m_strParam; }

private:
    // list all registered benchmarks
    void ListBenchmarks();

    // command lines options/parameters
    wxSortedArrayString m_toRun;
    long m_numRuns,
         m_avgCount,
         m_numParam;
    wxString m_strParam;
};

IMPLEMENT_APP(BenchApp)

// ============================================================================
// Bench namespace symbols implementation
// ============================================================================

Bench::Function *Bench::Function::ms_head = NULL;

long Bench::GetNumericParameter()
{
    return wxGetApp().GetNumericParameter();
}

wxString Bench::GetStringParameter()
{
    return wxGetApp().GetStringParameter();
}

// ============================================================================
// BenchApp implementation
// ============================================================================

BenchApp::BenchApp()
{
    m_avgCount = 10;
    m_numRuns = 10000; // just some default (TODO: switch to time-based one)
    m_numParam = 0;
}

bool BenchApp::OnInit()
{
    if ( !BenchAppBase::OnInit() )
        return false;

    sprintf(tmp,"wxWidgets benchmarking program\n"
             "Build: %s\n", WX_BUILD_OPTIONS_SIGNATURE);
    wxLogMessage(tmp);

#if wxUSE_GUI
    // create a hidden parent window to be used as parent for the GUI controls
    new wxFrame(NULL, wxID_ANY, "Hidden wx benchmark frame");
#endif // wxUSE_GUI

    return true;
}

void BenchApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    BenchAppBase::OnInitCmdLine(parser);

    parser.AddSwitch(OPTION_LIST,
                     "list",
                     "list all the existing benchmarks");

    parser.AddSwitch(OPTION_SINGLE,
                     "single",
                     "run the benchmark once only");

    parser.AddOption(OPTION_AVG_COUNT,
                     "avg-count",
                     wxString::Format
                     (
                        "number of times to run benchmarking loop (default: %ld)",
                        m_avgCount
                     ),
                     wxCMD_LINE_VAL_NUMBER);
    parser.AddOption(OPTION_NUM_RUNS,
                     "num-runs",
                     wxString::Format
                     (
                         "number of times to run each benchmark in a loop "
                         "(default: %ld)",
                         m_numRuns
                     ),
                     wxCMD_LINE_VAL_NUMBER);
    parser.AddOption(OPTION_NUMERIC_PARAM,
                     "num-param",
                     wxString::Format
                     (
                         "numeric parameter used by some benchmark functions "
                         "(default: %ld)",
                         m_numParam
                     ),
                     wxCMD_LINE_VAL_NUMBER);
    parser.AddOption(OPTION_STRING_PARAM,
                     "str-param",
                     "string parameter used by some benchmark functions "
                     "(default: empty)",
                     wxCMD_LINE_VAL_STRING);

    parser.AddParam("benchmark name",
                    wxCMD_LINE_VAL_STRING,
                    wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_PARAM_MULTIPLE);
}

bool BenchApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    if ( parser.Found(OPTION_LIST) )
    {
        ListBenchmarks();

        return false;
    }

    const size_t count = parser.GetParamCount();
    if ( !count )
    {
        parser.Usage();

        ListBenchmarks();

        return false;
    }

    bool numRunsSpecified = false;
    if ( parser.Found(OPTION_AVG_COUNT, &m_avgCount) )
        numRunsSpecified = true;
    if ( parser.Found(OPTION_NUM_RUNS, &m_numRuns) )
        numRunsSpecified = true;
    parser.Found(OPTION_NUMERIC_PARAM, &m_numParam);
    parser.Found(OPTION_STRING_PARAM, &m_strParam);
    if ( parser.Found(OPTION_SINGLE) )
    {
        if ( numRunsSpecified )
        {
            wxFprintf(stderr, "Incompatible options specified.\n");

            return false;
        }

        m_avgCount =
        m_numRuns = 1;
    }

    // construct sorted array for quick verification of benchmark names
    wxSortedArrayString benchmarks;
#if defined(__INTEL_COMPILER) // VDM auto patch
#   pragma ivdep
#endif
    for ( Bench::Function *func = Bench::Function::GetFirst();
          func;
          func = func->GetNext() )
    {
        benchmarks.push_back(func->GetName());
    }

#if defined(__INTEL_COMPILER) // VDM auto patch
#   pragma ivdep
#endif
    for ( size_t n = 0; n < count; n++ )
    {
        const wxString name = parser.GetParam(n);
        if ( benchmarks.Index(name) == wxNOT_FOUND )
        {
            wxFprintf(stderr, "No benchmark named \"%s\".\n", name);
            return false;
        }

        m_toRun.push_back(name);
    }

    return BenchAppBase::OnCmdLineParsed(parser);
}

int BenchApp::OnRun()
{
    int rc = EXIT_SUCCESS;
#if defined(__INTEL_COMPILER) // VDM auto patch
#   pragma ivdep
#endif
    for ( Bench::Function *func = Bench::Function::GetFirst();
          func;
          func = func->GetNext() )
    {
        if ( m_toRun.Index(func->GetName()) == wxNOT_FOUND )
            continue;

        wxString params;
        if ( m_numParam )
            params += wxString::Format(" with N=%ld", m_numParam);
        if ( !m_strParam.empty() )
        {
            if ( !params.empty() )
                params += " and";
            params += wxString::Format(" with s=\"%s\"", m_strParam);
        }

        sprintf(tmp,"Benchmarking %s%s: ", func->GetName(), params);
        wxLogMessage(tmp);

        long timeMin = LONG_MAX,
             timeMax = 0,
             timeTotal = 0;
        bool ok = func->Init();
#if defined(__INTEL_COMPILER) // VDM auto patch
#   pragma ivdep
#endif
        for ( long a = 0; ok && a < m_avgCount; a++ )
        {
            wxStopWatch sw;
#if defined(__INTEL_COMPILER) // VDM auto patch
#   pragma ivdep
#endif
            for ( long n = 0; n < m_numRuns && ok; n++ )
            {
                ok = func->Run();
            }

            sw.Pause();

            const long t = sw.Time();
            if ( t < timeMin )
                timeMin = t;
            if ( t > timeMax )
                timeMax = t;
            timeTotal += t;
        }

        func->Done();

        if ( !ok )
        {
            sprintf(tmp,"ERROR\n");
            wxLogMessage(tmp);
            rc = EXIT_FAILURE;
        }
        else
        {
            sprintf(tmp,"%ldms total, ", timeTotal);
            wxLogMessage(tmp);

            long times = m_avgCount;
            if ( m_avgCount > 2 )
            {
                timeTotal -= timeMin + timeMax;
                times -= 2;
            }

            sprintf(tmp,"%.2f avg (min=%ld, max=%ld)\n",
                     (float)timeTotal / times, timeMin, timeMax);
            wxLogMessage(tmp);
        }

        fflush(stdout);
    }

    return rc;
}

int BenchApp::OnExit()
{
#if wxUSE_GUI
    delete GetTopWindow();
#endif // wxUSE_GUI

    return 0;
}

/* static */
void BenchApp::ListBenchmarks()
{
    sprintf(tmp,"Available benchmarks:\n");
    wxLogMessage(tmp);
#if defined(__INTEL_COMPILER) // VDM auto patch
#   pragma ivdep
#endif
    for ( Bench::Function *func = Bench::Function::GetFirst();
          func;
          func = func->GetNext() )
    {
        sprintf(tmp,"\t%s\n", func->GetName());
        wxLogMessage(tmp);
    }
}
