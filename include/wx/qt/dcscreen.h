/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/dcscreen.h
// Author:      Sean D'Epagnier
// Copyright:   (c) Sean D'Epagnier
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_DCSCREEN_H_
#define _WX_QT_DCSCREEN_H_

#include "wx/qt/dcclient.h"

class WXDLLIMPEXP_CORE wxScreenDCImpl : public wxWindowDCImpl
{
public:
    wxScreenDCImpl( wxScreenDC *owner );

    ~wxScreenDCImpl();

    virtual void DoGetSize(int *width, int *height) const wxOVERRIDE;

private:

    wxDECLARE_ABSTRACT_CLASS(wxScreenDCImpl);
};

#endif // _WX_QT_DCSCREEN_H_
