/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/sckfile.cpp
// Purpose:     File protocol
// Author:      Guilhem Lavaux
// Modified by:
// Created:     20/07/97
// Copyright:   (c) 1997, 1998 Guilhem Lavaux
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_STREAMS && wxUSE_PROTOCOL_FILE

#ifndef WX_PRECOMP
#endif

#include "wx/uri.h"
#include "wx/wfstream.h"
#include "wx/protocol/file.h"


// ----------------------------------------------------------------------------
// wxFileProto
// ----------------------------------------------------------------------------

wxIMPLEMENT_DYNAMIC_CLASS(wxFileProto, wxProtocol);
IMPLEMENT_PROTOCOL(wxFileProto, wxT("file"), NULL, false)

wxFileProto::wxFileProto()
           : wxProtocol()
{
}

wxFileProto::~wxFileProto()
{
}

wxInputStream *wxFileProto::GetInputStream(const wxString& path)
{
    wxFileInputStream *retval = new wxFileInputStream(wxURI::Unescape(path));
    if ( retval->IsOk() )
    {
        m_lastError = wxPROTO_NOERR;
        return retval;
    }

    m_lastError = wxPROTO_NOFILE;
    delete retval;

    return NULL;
}

#endif // wxUSE_STREAMS && wxUSE_PROTOCOL_FILE
