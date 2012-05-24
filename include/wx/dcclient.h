/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/dcclient.h
// Purpose:     wxClientDC base header
// Author:      Julian Smart
// Copyright:   (c) Julian Smart
// RCS-ID:      $Id$
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCCLIENT_H_BASE_
#define _WX_DCCLIENT_H_BASE_

#include "wx/dc.h"

//-----------------------------------------------------------------------------
// wxWindowDC
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxWindowDC : public wxDC
{
public:
    wxWindowDC(wxWindow *win);

protected:
    wxWindowDC(wxDCImpl *impl) : wxDC(impl) { }

private:
    DECLARE_ABSTRACT_CLASS(wxWindowDC)
};

//-----------------------------------------------------------------------------
// wxClientDC
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxClientDC : public wxWindowDC
{
public:
    wxClientDC(wxWindow *win);

protected:
    wxClientDC(wxDCImpl *impl) : wxWindowDC(impl) { }

private:
    DECLARE_ABSTRACT_CLASS(wxClientDC)
};

//-----------------------------------------------------------------------------
// wxPaintDC
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxPaintDC : public wxClientDC
{
public:
    wxPaintDC(wxWindow *win);

protected:
    wxPaintDC(wxDCImpl *impl) : wxClientDC(impl) { }

private:
    DECLARE_ABSTRACT_CLASS(wxPaintDC)
};

#endif // _WX_DCCLIENT_H_BASE_
