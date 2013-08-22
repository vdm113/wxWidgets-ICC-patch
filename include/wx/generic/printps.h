/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/printps.h
// Purpose:     wxPostScriptPrinter, wxPostScriptPrintPreview
//              wxGenericPageSetupDialog
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __PRINTPSH__
#define __PRINTPSH__

#include "wx/prntbase.h"

#if wxUSE_PRINTING_ARCHITECTURE && wxUSE_POSTSCRIPT

// ----------------------------------------------------------------------------
// Represents the printer: manages printing a wxPrintout object
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxPostScriptPrinter : public wxPrinterBase
{
public:
    wxPostScriptPrinter(wxPrintDialogData *data = NULL);
    virtual ~wxPostScriptPrinter();

    virtual bool Print(wxWindow *parent, wxPrintout *printout, bool prompt = true);
    virtual wxDC* PrintDialog(wxWindow *parent);
    virtual bool Setup(wxWindow *parent);

private:
    DECLARE_DYNAMIC_CLASS(wxPostScriptPrinter)
};

// ----------------------------------------------------------------------------
// wxPrintPreview: programmer creates an object of this class to preview a
// wxPrintout.
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxPostScriptPrintPreview : public wxPrintPreviewBase
{
public:
    wxPostScriptPrintPreview(wxPrintout *printout,
                             wxPrintout *printoutForPrinting = NULL,
                             wxPrintDialogData *data = NULL);
    wxPostScriptPrintPreview(wxPrintout *printout,
                             wxPrintout *printoutForPrinting,
                             wxPrintData *data);

    virtual ~wxPostScriptPrintPreview();

    virtual bool Print(bool interactive);
    virtual void DetermineScaling();

private:
    void Init(wxPrintout *printout, wxPrintout *printoutForPrinting);

private:
    DECLARE_CLASS(wxPostScriptPrintPreview)
};

#endif

#endif
// __PRINTPSH__
