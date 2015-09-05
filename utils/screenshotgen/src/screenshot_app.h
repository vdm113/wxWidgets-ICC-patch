/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        screenshot_app.h
// Purpose:     Defines the Application Class
// Author:      Utensil Candel (UtensilCandel@@gmail.com)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _SCREENSHOT_APP_H_
#define _SCREENSHOT_APP_H_

#include "wx/app.h"

class ScreenshotApp : public wxApp
{
public:
    virtual bool OnInit();
};

#endif // _SCREENSHOT_APP_H_
