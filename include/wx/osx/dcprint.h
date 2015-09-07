/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/osx/dcprint.h
// Purpose:     wxPrinterDC class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCPRINT_H_
#define _WX_DCPRINT_H_

#include "wx/dc.h"
#include "wx/dcgraph.h"
#include "wx/cmndata.h"

class wxNativePrinterDC ;

class WXDLLIMPEXP_CORE wxPrinterDCImpl: public wxGCDCImpl
{
public:
#if wxUSE_PRINTING_ARCHITECTURE

    wxPrinterDCImpl( wxPrinterDC *owner, const wxPrintData& printdata );
    virtual ~wxPrinterDCImpl();

    virtual bool StartDoc( const wxString& WXUNUSED(message) ) ;
    virtual void EndDoc(void) ;
    virtual void StartPage(void) ;
    virtual void EndPage(void) ;

    wxRect GetPaperRect() const;

    wxPrintData& GetPrintData() { return m_printData; }
    virtual wxSize GetPPI() const;

protected:
    virtual void DoGetSize( int *width, int *height ) const;

    wxPrintData        m_printData ;
    wxNativePrinterDC* m_nativePrinterDC ;

private:
    wxDECLARE_CLASS(wxPrinterDC);
#endif // wxUSE_PRINTING_ARCHITECTURE
};

#endif
    // _WX_DCPRINT_H_

