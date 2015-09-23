/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/msw/systhemectrl.cpp
// Purpose:     wxMSW implementation of wxSystemThemedControl
// Author:      Tobias Taschner
// Created:     2015-09-15
// Copyright:   (c) 2015 wxWidgets development team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/systhemectrl.h"

#include "wx/msw/private.h"
#include "wx/msw/uxtheme.h"

#ifdef wxHAS_SYSTEM_THEMED_CONTROL

void wxSystemThemedControlBase::DoEnableSystemTheme(bool enable, wxWindow* window)
{
    if ( wxGetWinVersion() >= wxWinVersion_Vista )
    {
        if ( wxUxThemeEngine *te = wxUxThemeEngine::GetIfActive() )
        {
            const wchar_t* const sysThemeId = enable ? L"EXPLORER" : NULL;
            te->SetWindowTheme(GetHwndOf(window), sysThemeId, NULL);
        }
    }
}

#endif // wxHAS_SYSTEM_THEMED_CONTROL
