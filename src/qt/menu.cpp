/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/menu.cpp
// Author:      Peter Most, Mariano Reingart
// Copyright:   (c) 2010 wxWidgets dev team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/menu.h"
#include "wx/qt/private/utils.h"
#include "wx/qt/private/converter.h"


static void ApplyStyle( QMenu *qtMenu, long style )
{
    if ( style & wxMENU_TEAROFF )
        qtMenu->setTearOffEnabled( true );
}

wxMenu::wxMenu(long style)
    : wxMenuBase( style )
{
    m_qtMenu = new QMenu();

    ApplyStyle( m_qtMenu, style );
}

wxMenu::wxMenu(const wxString& title, long style)
    : wxMenuBase( title, style )
{
    m_qtMenu = new QMenu( wxQtConvertString( title ));

    ApplyStyle( m_qtMenu, style );
}



static wxMenuItem *GetMenuItemAt( const wxMenu *menu, size_t position )
{
    // FindItemByPosition() is doing the same test, but we want to prevent
    // the warning message it prints when an illegal index is used.

    if ( position < menu->GetMenuItemCount() )
        return menu->FindItemByPosition( position );
    else
        return NULL;
}


static void InsertMenuItemAction( const wxMenu *menu, const wxMenuItem *previousItem,
    const wxMenuItem *item, const wxMenuItem *successiveItem )
{
    QMenu *qtMenu = menu->GetHandle();
    QAction *itemAction = item->GetHandle();
    if ( item->GetKind() == wxITEM_RADIO )
    {
        // If the previous menu item is a radio item then add this item to the
        // same action group, otherwise start a new group:

        if ( previousItem != NULL && previousItem->GetKind() == wxITEM_RADIO )
        {
            QAction *previousItemAction = previousItem->GetHandle();
            QActionGroup *previousItemActionGroup = previousItemAction->actionGroup();
            wxASSERT_MSG( previousItemActionGroup != NULL, "An action group should have been setup" );
            previousItemActionGroup->addAction( itemAction );
        }
        else
        {
            QActionGroup *actionGroup = new QActionGroup( qtMenu );
            actionGroup->addAction( itemAction );
            wxASSERT_MSG( itemAction->actionGroup() == actionGroup, "Must be the same action group" );
        }
    }
    // Insert the action into the actual menu:
    QAction *successiveItemAction = ( successiveItem != NULL ) ? successiveItem->GetHandle() : NULL;
    qtMenu->insertAction( successiveItemAction, itemAction );
}

wxMenuItem *wxMenu::DoAppend(wxMenuItem *item)
{
    // Get the previous/successive items *before* we call the base class methods,
    // because afterwards it is less clear where these items end up:

    wxMenuItem *previousItem = GetMenuItemAt( this, GetMenuItemCount() - 1 );
    wxMenuItem *successiveItem = GetMenuItemAt( this, GetMenuItemCount() );

    if ( wxMenuBase::DoAppend( item ) == NULL )
        return NULL;

    InsertMenuItemAction( this, previousItem, item, successiveItem );

    return item;
}


wxMenuItem *wxMenu::DoInsert(size_t insertPosition, wxMenuItem *item)
{
    // Get the previous/successive items *before* we call the base class methods,
    // because afterwards it is less clear where these items end up:

    wxMenuItem *previousItem = GetMenuItemAt( this, insertPosition - 1 );
    wxMenuItem *successiveItem = GetMenuItemAt( this, insertPosition );

    if ( wxMenuBase::DoInsert( insertPosition, item ) == NULL )
        return NULL;

    InsertMenuItemAction( this, previousItem, item, successiveItem );

    return item;
}


wxMenuItem *wxMenu::DoRemove(wxMenuItem *item)
{
    if ( wxMenuBase::DoRemove( item ) == NULL )
        return NULL;

    m_qtMenu->removeAction( item->GetHandle() );

    return item;
}


QMenu *wxMenu::GetHandle() const
{
    return m_qtMenu;
}


//##############################################################################

wxMenuBar::wxMenuBar()
{
    m_qtMenuBar  = new QMenuBar();
    PostCreation();
}

wxMenuBar::wxMenuBar( long WXUNUSED( style ))
{
    m_qtMenuBar = new QMenuBar();
    PostCreation();
}

wxMenuBar::wxMenuBar(size_t count, wxMenu *menus[], const wxString titles[], long WXUNUSED( style ))
{
    m_qtMenuBar = new QMenuBar();

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   pragma prefetch
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
    for ( size_t i = 0; i < count; ++i )
        Append( menus[ i ], titles[ i ] );

    PostCreation();
}


static QMenu *SetTitle( wxMenu *menu, const wxString &title )
{
    QMenu *qtMenu = menu->GetHandle();
    qtMenu->setTitle( wxQtConvertString( title ));

    return qtMenu;
}


bool wxMenuBar::Append( wxMenu *menu, const wxString& title )
{
    if ( !wxMenuBarBase::Append( menu, title ))
        return false;

    // Override the stored menu title with the given one:

    QMenu *qtMenu = SetTitle( menu, title );
    m_qtMenuBar->addMenu( qtMenu );
    
    return true;
}


static QAction *GetActionAt( const QWidget *qtWidget, size_t pos )
{
    QList< QAction * > actions = qtWidget->actions();
    return pos < static_cast< unsigned >( actions.size() ) ? actions.at( pos ) : NULL;
}


bool wxMenuBar::Insert(size_t pos, wxMenu *menu, const wxString& title)
{
    if ( !wxMenuBarBase::Insert( pos, menu, title ))
        return false;

    // Override the stored menu title with the given one:

    QMenu *qtMenu = SetTitle( menu, title );
    QAction *qtAction = GetActionAt( m_qtMenuBar, pos );
    m_qtMenuBar->insertMenu( qtAction, qtMenu );

    return true;
}

wxMenu *wxMenuBar::Remove(size_t pos)
{
    wxMenu *menu;

    if (( menu = wxMenuBarBase::Remove( pos )) == NULL )
        return NULL;

    m_qtMenuBar->removeAction( GetActionAt( m_qtMenuBar, pos ));
    return menu;
}

void wxMenuBar::EnableTop(size_t pos, bool enable)
{
    QAction *qtAction = GetActionAt( m_qtMenuBar, pos );
    qtAction->setEnabled( enable );
}


void wxMenuBar::SetMenuLabel(size_t pos, const wxString& label)
{
    QAction *qtAction = GetActionAt( m_qtMenuBar, pos );
    QMenu *qtMenu = qtAction->menu();
    qtMenu->setTitle( wxQtConvertString( label ));
}

wxString wxMenuBar::GetMenuLabel(size_t pos) const
{
    QAction *qtAction = GetActionAt( m_qtMenuBar, pos );
    QMenu *qtMenu = qtAction->menu();

    return wxQtConvertString( qtMenu->title() );
}

void wxMenuBar::Attach(wxFrame *frame)
{
    // sanity check as setMenuBar takes ownership
    wxCHECK_RET( m_qtMenuBar, "Menu bar has been previously deleted by Qt");
    wxMenuBarBase::Attach(frame);
}

void wxMenuBar::Detach()
{
    // the QMenuBar probably was deleted by Qt as setMenuBar takes ownership
    m_qtMenuBar = NULL;
    wxMenuBarBase::Detach();
}

QMenuBar *wxMenuBar::GetHandle() const
{
    return m_qtMenuBar;
}
