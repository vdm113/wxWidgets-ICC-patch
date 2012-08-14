/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        helpview.cpp
// Purpose:     wxHtml sample: help browser
// Author:      ?
// Modified by:
// Created:     ?
// RCS-ID:      $Id$
// Copyright:   (c) wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// Please note: see utils/helpview for a more fully-featured
// standalone help browser.

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/image.h"
#include "wx/wxhtml.h"
#include "wx/fs_zip.h"
#include "wx/log.h"
#include "wx/filedlg.h"


// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)

    virtual bool OnInit();
    virtual int OnExit();

private:
    wxHtmlHelpController *help;
};


IMPLEMENT_APP(MyApp)


bool MyApp::OnInit()
{
#ifdef __WXMOTIF__
    delete wxLog::SetActiveTarget(new wxLogStderr); // So dialog boxes aren't used
#endif

    wxInitAllImageHandlers();
    wxFileSystem::AddHandler(new wxZipFSHandler);

    SetVendorName(wxT("wxWidgets"));
    SetAppName(wxT("wxHTMLHelp"));
    wxConfig::Get(); // create an instance

    help = new wxHtmlHelpController;

    if (argc < 2) {
        wxLogError(wxT("Usage : helpview <helpfile> [<more helpfiles>]"));
        wxLogError(wxT("  helpfile may be .hhp, .zip or .htb"));
        return false;
    }

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
    for (int i = 1; i < argc; i++)
        help->AddBook(wxFileName(argv[i]));

#ifdef __WXMOTIF__
    delete wxLog::SetActiveTarget(new wxLogGui);
#endif

    help->SetShouldPreventAppExit(true);

    help -> DisplayContents();

    return true;
}

int MyApp::OnExit()
{
    delete help;
    delete wxConfig::Set(NULL);

    return 0;
}

