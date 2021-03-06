/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/toplevel.cpp
// Author:      Peter Most, Javier Torres, Sean D'Epagnier, Mariano Reingart
// Copyright:   (c) 2010 wxWidgets dev team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/toplevel.h"
#include "wx/qt/private/converter.h"
#include <QtGui/QIcon>

wxTopLevelWindowQt::wxTopLevelWindowQt()
{
}

wxTopLevelWindowQt::wxTopLevelWindowQt(wxWindow *parent,
           wxWindowID winId,
           const wxString &title,
           const wxPoint &pos,
           const wxSize &size,
           long style,
           const wxString &name )
{
    Create( parent, winId, title, pos, size, style, name );
}

bool wxTopLevelWindowQt::Create( wxWindow *parent, wxWindowID winId,
    const wxString &title, const wxPoint &pos, const wxSize &sizeOrig,
    long style, const wxString &name )
{
    wxSize size(sizeOrig);
    if ( !size.IsFullySpecified() )
        size.SetDefaults( GetDefaultSize() );

    wxTopLevelWindows.Append( this );

    if (!CreateBase( parent, winId, pos, size, style, wxDefaultValidator, name ))
    {
        wxFAIL_MSG( wxT("wxTopLevelWindowNative creation failed") );
        return false;
    }

    SetTitle( title );
    SetWindowStyleFlag( style );

    if (pos != wxDefaultPosition)
        m_qtWindow->move( pos.x, pos.y );

    m_qtWindow->resize( wxQtConvertSize( size ) );

    // Prevent automatic deletion of Qt main window on close
    // (this should be the default, but left just fo enforce it)
    GetHandle()->setAttribute(Qt::WA_DeleteOnClose, false);

    // not calling to wxWindow::Create, so do the rest of initialization:
    if (parent) parent->AddChild( this );

    return true;
}

void wxTopLevelWindowQt::Maximize(bool WXUNUSED(maximize)) 
{
}

void wxTopLevelWindowQt::Restore()
{
}

void wxTopLevelWindowQt::Iconize(bool WXUNUSED(iconize) )
{
}

bool wxTopLevelWindowQt::IsMaximized() const
{
    return false;
}

bool wxTopLevelWindowQt::IsIconized() const
{
    return false;
}


bool wxTopLevelWindowQt::ShowFullScreen(bool WXUNUSED(show), long WXUNUSED(style))
{
    return false;
}

bool wxTopLevelWindowQt::IsFullScreen() const
{
    return false;
}

void wxTopLevelWindowQt::SetTitle(const wxString& title)
{
    GetHandle()->setWindowTitle( wxQtConvertString( title ));
}

wxString wxTopLevelWindowQt::GetTitle() const
{
    return ( wxQtConvertString( GetHandle()->windowTitle() ));
}

void wxTopLevelWindowQt::SetIcons( const wxIconBundle& icons )
{
    wxTopLevelWindowBase::SetIcons( icons );
    
    QIcon qtIcons;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   pragma prefetch
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
    for ( size_t i = 0; i < icons.GetIconCount(); i++ )
    {
        qtIcons.addPixmap( *icons.GetIconByIndex( i ).GetHandle() );
    }
    GetHandle()->setWindowIcon( qtIcons );
}

void wxTopLevelWindowQt::SetWindowStyleFlag( long style )
{
    wxWindow::SetWindowStyleFlag( style );
    
    Qt::WindowFlags qtFlags = GetHandle()->windowFlags();
    
    if ( HasFlag( wxSTAY_ON_TOP ) != qtFlags.testFlag( Qt::WindowStaysOnTopHint ) )
        qtFlags ^= Qt::WindowStaysOnTopHint;

    if ( HasFlag( wxCAPTION ) )
    {
        // Only show buttons if window has caption
        if ( HasFlag( wxSYSTEM_MENU ) )
        {
            qtFlags |= Qt::WindowSystemMenuHint;
            if ( HasFlag( wxMINIMIZE_BOX ) )
                qtFlags |= Qt::WindowMinimizeButtonHint;
            else
                qtFlags &= ~Qt::WindowMinimizeButtonHint;
            
            if ( HasFlag( wxMAXIMIZE_BOX ) )
                qtFlags |= Qt::WindowMaximizeButtonHint;
            else
                qtFlags &= ~Qt::WindowMaximizeButtonHint;
            
            if ( HasFlag( wxCLOSE_BOX ) )
                qtFlags |= Qt::WindowCloseButtonHint;
            else
                qtFlags &= ~Qt::WindowCloseButtonHint;
        }
        else
        {
            qtFlags &= ~Qt::WindowSystemMenuHint;
            qtFlags &= ~Qt::WindowMinMaxButtonsHint;
            qtFlags &= ~Qt::WindowCloseButtonHint;
        }
    }
        
    GetHandle()->setWindowFlags( qtFlags );
    
    wxCHECK_RET( !( HasFlag( wxMAXIMIZE ) && HasFlag( wxMAXIMIZE ) ), "Window cannot be both maximized and minimized" );
    if ( HasFlag( wxMAXIMIZE ) )
        GetHandle()->setWindowState( Qt::WindowMaximized );
    else if ( HasFlag( wxMINIMIZE ) )
        GetHandle()->setWindowState( Qt::WindowMinimized );
    
    if ( HasFlag( wxRESIZE_BORDER ) )
        GetHandle()->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
    else
        GetHandle()->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    
    if ( HasFlag( wxCENTRE ) )
    {
        Centre();
    }
}

long wxTopLevelWindowQt::GetWindowStyleFlag() const
{
    // Update maximized/minimized state
    long winStyle = wxWindow::GetWindowStyleFlag();

    if (GetHandle())
    {
        switch ( GetHandle()->windowState() )
        {
        case Qt::WindowMaximized:
            winStyle &= ~wxMINIMIZE;
            winStyle |= wxMAXIMIZE;
            break;
        case Qt::WindowMinimized:
            winStyle &= ~wxMAXIMIZE;
            winStyle |= wxMINIMIZE;
            break;
        default:
            winStyle &= ~wxMINIMIZE;
            winStyle &= ~wxMAXIMIZE;
        }
    }

    return winStyle;
}
