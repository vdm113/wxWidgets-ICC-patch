/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////
// File:        wx/mac/taskbarosx.h
// Purpose:     Defines wxTaskBarIcon class for OSX
// Author:      Ryan Norton
// Modified by:
// Created:     04/04/2003
// Copyright:   (c) Ryan Norton, 2003
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////

#ifndef _TASKBAR_H_
#define _TASKBAR_H_

class WXDLLIMPEXP_FWD_CORE wxIcon;
class WXDLLIMPEXP_FWD_CORE wxMenu;

class WXDLLIMPEXP_ADV wxTaskBarIcon : public wxTaskBarIconBase
{
    wxDECLARE_DYNAMIC_CLASS_NO_COPY(wxTaskBarIcon);
public:
    wxTaskBarIcon(wxTaskBarIconType iconType = wxTBI_DEFAULT_TYPE);
    virtual ~wxTaskBarIcon();

    // returns true if the taskbaricon is in the global menubar
#if wxOSX_USE_COCOA
    bool OSXIsStatusItem();
#else
    bool OSXIsStatusItem() { return false; }
#endif
    bool IsOk() const { return true; }

    bool IsIconInstalled() const;
    bool SetIcon(const wxIcon& icon, const wxString& tooltip = wxEmptyString);
    bool RemoveIcon();
    bool PopupMenu(wxMenu *menu);

protected:
    wxTaskBarIconType m_type;
    class wxTaskBarIconImpl* m_impl;
    friend class wxTaskBarIconImpl;
};
#endif
    // _TASKBAR_H_
