/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/x11/dcscreen.h
// Purpose:     wxScreenDC class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCSCREEN_H_
#define _WX_DCSCREEN_H_

#include "wx/dcclient.h"
#include "wx/x11/dcclient.h"

//-----------------------------------------------------------------------------
// wxScreenDC
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxScreenDCImpl : public wxPaintDCImpl
{
public:
    wxScreenDCImpl( wxDC *owner);
    virtual ~wxScreenDCImpl();

protected:
    virtual void DoGetSize(int *width, int *height) const;

private:
    wxDECLARE_CLASS(wxScreenDCImpl);
};


#endif
    // _WX_DCSCREEN_H_
