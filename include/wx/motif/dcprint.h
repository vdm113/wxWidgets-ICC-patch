/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif defined(__INTEL_COMPILER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////

// Name:        wx/motif/dcprint.h
// Purpose:     wxPrinterDC class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCPRINT_H_
#define _WX_DCPRINT_H_

#include "wx/motif/dc.h"

class WXDLLIMPEXP_CORE wxPrinterDC : public wxMotifDCImpl
{
public:
    // Create a printer DC
    wxPrinterDCImpl(const wxString& driver, const wxString& device,
                    const wxString& output,
                    bool interactive = true,
                    wxPrintOrientation orientation = wxPORTRAIT);
    virtual ~wxPrinterDC();

    wxRect GetPaperRect() const;

    DECLARE_CLASS(wxPrinterDCImpl)
};

#endif // _WX_DCPRINT_H_
