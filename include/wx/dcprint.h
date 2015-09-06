/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/dcprint.h
// Purpose:     wxPrinterDC base header
// Author:      Julian Smart
// Modified by:
// Created:
// Copyright:   (c) Julian Smart
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCPRINT_H_BASE_
#define _WX_DCPRINT_H_BASE_

#include "wx/defs.h"

#if wxUSE_PRINTING_ARCHITECTURE

#include "wx/dc.h"

//-----------------------------------------------------------------------------
// wxPrinterDC
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxPrinterDC : public wxDC
{
public:
    wxPrinterDC();
    wxPrinterDC(const wxPrintData& data);

    wxRect GetPaperRect() const;
    int GetResolution() const wxOVERRIDE;

protected:
    wxPrinterDC(wxDCImpl *impl) : wxDC(impl) { }

private:
    wxDECLARE_DYNAMIC_CLASS(wxPrinterDC);
};

#endif // wxUSE_PRINTING_ARCHITECTURE

#endif // _WX_DCPRINT_H_BASE_
