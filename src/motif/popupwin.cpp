/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/////////////////////////////////////////////////////////////////////////////
// Name:        src/motif/popupwin.cpp
// Purpose:     wxPopupWindow implementation
// Author:      Mattia barbon
// Modified by:
// Created:     28.08.03
// Copyright:   (c) Mattia barbon
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/popupwin.h"

#ifndef WX_PRECOMP
    #include "wx/app.h"
#endif

#ifdef __VMS__
#pragma message disable nosimpint
#endif
#include <Xm/Xm.h>
#ifdef __VMS__
#pragma message enable nosimpint
#endif

#include "wx/motif/private.h"

bool wxPopupWindow::Create( wxWindow *parent, int flags )
{
    if( !wxPopupWindowBase::Create( parent, flags ) )
        return false;

    SetParent( parent );
    if( parent )
        parent->AddChild( this );

    Widget popup = XtVaCreatePopupShell( "shell",
                                         overrideShellWidgetClass,
                                         (Widget)wxTheApp->GetTopLevelWidget(),
                                         NULL );

    m_mainWidget = (WXWidget)popup;

    wxAddWindowToTable( (Widget) m_mainWidget, this );

    DoSetSizeIntr( -1, -1, 100, 100, 0, true );

    XtSetMappedWhenManaged( popup, False );
    XtRealizeWidget( popup );
    XtManageChild ( popup );
/*
    XtTranslations ptr;
    XtOverrideTranslations (popup,
        ptr = XtParseTranslationTable ("<Configure>: resize()"));
    XtFree ((char *) ptr);
*/
    return true;
}

bool wxPopupWindow::Show( bool show )
{
    if( !wxWindowBase::Show( show ) )
        return false;

    if( show )
    {
        XtPopup( (Widget)GetMainWidget(), XtGrabNonexclusive );
    }
    else
    {
        XtPopdown( (Widget)GetMainWidget() );
    }

    return true;
}
