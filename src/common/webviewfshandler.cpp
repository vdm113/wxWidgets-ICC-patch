/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        webviewfshandler.cpp
// Purpose:     Custom webview handler for virtual file system
// Author:      Nick Matthews
// Id:          $Id$
// Copyright:   (c) 2012 Steven Lamerton
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if wxUSE_WEBVIEW

#if defined(__BORLANDC__)
    #pragma hdrstop
#endif

#include "wx/webviewfshandler.h"
#include "wx/filesys.h"

wxWebViewFSHandler::wxWebViewFSHandler(const wxString& scheme) :
                         wxWebViewHandler(scheme)
{
    m_fileSystem = new wxFileSystem();
}

wxWebViewFSHandler::~wxWebViewFSHandler()
{
    wxDELETE(m_fileSystem);
}

wxFSFile* wxWebViewFSHandler::GetFile(const wxString &uri)
{
    return m_fileSystem->OpenFile(uri);
}

#endif // wxUSE_WEBVIEW
