/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        screenshot_app.cpp
// Purpose:     Implement Application Class
// Author:      Utensil Candel (UtensilCandel@@gmail.com)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers wxWidgets headers)
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/richtext/richtextxml.h"

#include "screenshot_app.h"
#include "screenshot_main.h"


// ----------------------------------------------------------------------------
// ScreenshotApp
// ----------------------------------------------------------------------------

wxIMPLEMENT_APP(ScreenshotApp);

bool ScreenshotApp::OnInit()
{
    // Init all Image handlers
    wxInitAllImageHandlers();

    // Add richtext extra handlers (plain text is automatically added)
    wxRichTextBuffer::AddHandler(new wxRichTextXMLHandler);

    ScreenshotFrame* frame = new ScreenshotFrame(0L);
    frame->Show();

    return true;
}
