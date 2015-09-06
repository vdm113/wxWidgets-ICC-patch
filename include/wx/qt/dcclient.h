/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/dcclient.h
// Author:      Peter Most, Javier Torres, Mariano Reingart
// Copyright:   (c) 2010 wxWidgets dev team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_DCCLIENT_H_
#define _WX_QT_DCCLIENT_H_

#include "wx/qt/dc.h"

class WXDLLIMPEXP_CORE wxWindowDCImpl : public wxQtDCImpl
{
public:
    wxWindowDCImpl( wxDC *owner );
    wxWindowDCImpl( wxDC *owner, wxWindow *win );

    ~wxWindowDCImpl();

protected:
    wxWindow *m_window;

private:

};


class WXDLLIMPEXP_CORE wxClientDCImpl : public wxWindowDCImpl
{
public:
    wxClientDCImpl( wxDC *owner );
    wxClientDCImpl( wxDC *owner, wxWindow *win );

    ~wxClientDCImpl();
};


class WXDLLIMPEXP_CORE wxPaintDCImpl : public wxWindowDCImpl
{
public:
    wxPaintDCImpl( wxDC *owner );
    wxPaintDCImpl( wxDC *owner, wxWindow *win );
};

#endif // _WX_QT_DCCLIENT_H_
