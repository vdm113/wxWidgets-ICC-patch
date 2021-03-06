/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/settcmn.cpp
// Purpose:     common (to all ports) wxWindow functions
// Author:      Robert Roebling
// Copyright:   (c) wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/settings.h"

#ifndef WX_PRECOMP
    #include "wx/utils.h"
#endif //WX_PRECOMP

// ----------------------------------------------------------------------------
// static data
// ----------------------------------------------------------------------------

wxSystemScreenType wxSystemSettings::ms_screen = wxSYS_SCREEN_NONE;

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

wxSystemScreenType wxSystemSettings::GetScreenType()
{
    if (ms_screen == wxSYS_SCREEN_NONE)
    {
        // wxUniv will be used on small devices, too.
        int x = GetMetric( wxSYS_SCREEN_X );

        ms_screen = wxSYS_SCREEN_DESKTOP;

        if (x < 800)
            ms_screen = wxSYS_SCREEN_SMALL;

        if (x < 640)
            ms_screen = wxSYS_SCREEN_PDA;

        if (x < 200)
            ms_screen = wxSYS_SCREEN_TINY;

        // This is probably a bug, but VNC seems to report 0
        if (x < 10)
            ms_screen = wxSYS_SCREEN_DESKTOP;
    }

    return ms_screen;
}

void wxSystemSettings::SetScreenType( wxSystemScreenType screen )
{
    ms_screen = screen;
}
