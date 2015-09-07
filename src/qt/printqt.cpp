/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/printqt.cpp
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/print.h"

wxQtPrinter::wxQtPrinter( wxPrintDialogData *WXUNUSED(data))
{
}

bool wxQtPrinter::Setup(wxWindow *WXUNUSED(parent))
{
    return false;
}

bool wxQtPrinter::Print(wxWindow *WXUNUSED(parent), wxPrintout *WXUNUSED(printout), bool WXUNUSED(prompt))
{
    return false;
}

wxDC* wxQtPrinter::PrintDialog(wxWindow *WXUNUSED(parent))
{
    return NULL;
}

//##############################################################################

wxQtPrintPreview::wxQtPrintPreview(wxPrintout *printout,
                      wxPrintout *printoutForPrinting,
                      wxPrintDialogData *data)
    : wxPrintPreviewBase( printout, printoutForPrinting, data )
{
}

wxQtPrintPreview::wxQtPrintPreview(wxPrintout *printout,
                      wxPrintout *printoutForPrinting,
                      wxPrintData *data)
    : wxPrintPreviewBase( printout, printoutForPrinting, data )
{
}

bool wxQtPrintPreview::Print(bool WXUNUSED(interactive))
{
    return false;
}

void wxQtPrintPreview::DetermineScaling()
{
}

