/////////////////////////////////////////////////////////////////////////////
// Name:        cocoa/choice.mm
// Purpose:     wxChoice
// Author:      David Elliott
// Modified by:
// Created:     2003/03/16
// RCS-ID:      $Id: 
// Copyright:   (c) 2003 David Elliott
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include "wx/log.h"
    #include "wx/app.h"
    #include "wx/choice.h"
    #include "wx/arrstr.h"
#endif //WX_PRECOMP

#include "wx/cocoa/string.h"

#import <AppKit/NSPopUpButton.h>
#import <AppKit/NSMenu.h>
#import <Foundation/NSNotification.h>
#import <Foundation/NSDictionary.h>

IMPLEMENT_DYNAMIC_CLASS(wxChoice, wxControl)
BEGIN_EVENT_TABLE(wxChoice, wxChoiceBase)
END_EVENT_TABLE()
// WX_IMPLEMENT_COCOA_OWNER(wxChoice,NSButton,NSControl,NSView)

void wxChoice::Init()
{
    m_sortedStrings = NULL;
}

bool wxChoice::Create(wxWindow *parent, wxWindowID winid,
            const wxPoint& pos,
            const wxSize& size,
            const wxArrayString& choices,
            long style,
            const wxValidator& validator,
            const wxString& name)
{
    wxCArrayString chs(choices);

    return Create(parent, winid, pos, size, chs.GetCount(), chs.GetStrings(),
                  style, validator, name);
}

bool wxChoice::Create(wxWindow *parent, wxWindowID winid,
            const wxPoint& pos,
            const wxSize& size,
            int n, const wxString choices[],
            long style,
            const wxValidator& validator,
            const wxString& name)
{
    if(!CreateControl(parent,winid,pos,size,style,validator,name))
        return false;

    SetNSView([[NSPopUpButton alloc] initWithFrame:MakeDefaultNSRect(size)
        pullsDown: NO]);
    [m_cocoaNSView release];

    NSMenu *nsmenu = [(NSPopUpButton*)m_cocoaNSView menu];
    AssociateNSMenu(nsmenu, OBSERVE_DidSendAction);

    if(style&wxCB_SORT)
    {
        m_sortedStrings = new wxSortedArrayString;
        for(int i=0; i<n; i++)
        {
            m_sortedStrings->Add(choices[i]);
        }
        for(size_t i=0; i < m_sortedStrings->GetCount(); i++)
        {
            [nsmenu addItemWithTitle:wxNSStringWithWxString(
                    m_sortedStrings->Item(i))
                action: nil keyEquivalent:@""];
        }
    }
    else
    {
        for(int i=0; i<n; i++)
        {
            [nsmenu addItemWithTitle:wxNSStringWithWxString(choices[i])
                action: nil keyEquivalent:@""];
        }
    }
    m_itemsClientData.SetCount(n);

    [(NSPopUpButton*)m_cocoaNSView sizeToFit];
    if(m_parent)
        m_parent->CocoaAddChild(this);
    SetInitialFrameRect(pos,size);

    return true;
}

wxChoice::~wxChoice()
{
    DisassociateNSMenu([(NSPopUpButton*)m_cocoaNSView menu]);

    if(m_sortedStrings)
        m_sortedStrings->Clear();
    delete m_sortedStrings;

    if(HasClientObjectData())
    {
        for(size_t i=0; i < m_itemsClientData.GetCount(); i++)
            delete (wxClientData*)m_itemsClientData.Item(i);
    }
    m_itemsClientData.Clear();

    CocoaRemoveFromParent();
}

void wxChoice::CocoaNotification_menuDidSendAction(WX_NSNotification notification)
{
    NSDictionary *userInfo = [notification userInfo];
    NSMenuItem *menuitem = [userInfo objectForKey:@"MenuItem"];
    int index = [[(NSPopUpButton*)m_cocoaNSView menu] indexOfItem: menuitem];
    int selectedItem = [(NSPopUpButton*)m_cocoaNSView indexOfSelectedItem];
    wxLogDebug(wxT("menuDidSendAction, index=%d, selectedItem=%d"), index, selectedItem);
    wxCommandEvent event(wxEVT_COMMAND_CHOICE_SELECTED, m_windowId);
    event.SetInt(index);
    event.SetEventObject(this);
    event.SetString(GetStringSelection());
    GetEventHandler()->ProcessEvent(event);
}

void wxChoice::Clear()
{
    if(m_sortedStrings)
        m_sortedStrings->Clear();
    if(HasClientObjectData())
    {
        for(size_t i=0; i < m_itemsClientData.GetCount(); i++)
            delete (wxClientData*)m_itemsClientData.Item(i);
    }
    m_itemsClientData.Clear();
    [(NSPopUpButton*)m_cocoaNSView removeAllItems];
}

void wxChoice::Delete(int n)
{
    if(m_sortedStrings)
        m_sortedStrings->RemoveAt(n);
    if(HasClientObjectData())
        delete (wxClientData*)m_itemsClientData.Item(n);
    m_itemsClientData.RemoveAt(n);
    [(NSPopUpButton*)m_cocoaNSView removeItemAtIndex:n];
}

int wxChoice::GetCount() const
{
    return [(NSPopUpButton*)m_cocoaNSView numberOfItems];
}

wxString wxChoice::GetString(int n) const
{
    return wxStringWithNSString([(NSPopUpButton*)m_cocoaNSView itemTitleAtIndex:n]);
}

void wxChoice::SetString(int n, const wxString& title)
{
    NSMenuItem *item = [(NSPopUpButton*)m_cocoaNSView itemAtIndex:n];
    [item setTitle:wxNSStringWithWxString(title)];
}

int wxChoice::FindString(const wxString& title) const
{
    return [(NSPopUpButton*)m_cocoaNSView indexOfItemWithTitle:
        wxNSStringWithWxString(title)];
}

int wxChoice::GetSelection() const
{
    return [(NSPopUpButton*)m_cocoaNSView indexOfSelectedItem];
}

int wxChoice::DoAppend(const wxString& title)
{
    NSMenu *nsmenu = [(NSPopUpButton*)m_cocoaNSView menu];
    NSMenuItem *item;
    if(m_sortedStrings)
    {
        int sortedIndex = m_sortedStrings->Add(title);
        item = [nsmenu insertItemWithTitle:
                wxNSStringWithWxString(title)
            action: nil keyEquivalent:@"" atIndex:sortedIndex];
        m_itemsClientData.Insert(NULL, sortedIndex);
    }
    else
    {
        item = [nsmenu addItemWithTitle:wxNSStringWithWxString(title)
            action: nil keyEquivalent:@""];
        m_itemsClientData.Add(NULL);
    }
    return [nsmenu indexOfItem:item];
}

int wxChoice::DoInsert(const wxString& title, int pos)
{
    if(m_sortedStrings)
        return DoAppend(title);
    NSMenu *nsmenu = [(NSPopUpButton*)m_cocoaNSView menu];
    NSMenuItem *item = [nsmenu insertItemWithTitle:wxNSStringWithWxString(title)
        action: nil keyEquivalent:@"" atIndex:pos];
    m_itemsClientData.Insert(NULL, pos);
    return [nsmenu indexOfItem:item];
}

void wxChoice::DoSetItemClientData(int n, void *data)
{
    m_itemsClientData.Item(n) = data;
}

void* wxChoice::DoGetItemClientData(int n) const
{
    return m_itemsClientData.Item(n);
}

void wxChoice::DoSetItemClientObject(int n, wxClientData *data)
{
    m_itemsClientData.Item(n) = data;
}

wxClientData* wxChoice::DoGetItemClientObject(int n) const
{
    return (wxClientData*)m_itemsClientData.Item(n);
}

void wxChoice::SetSelection(int n)
{
    [(NSPopUpButton*)m_cocoaNSView selectItemAtIndex:n];
}

