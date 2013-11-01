/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/gnome/gvfs.h
// Author:      Robert Roebling
// Purpose:     GNOME VFS support
// Created:     17/03/06
// Copyright:   Robert Roebling
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GTK_GVFS_H_
#define _WX_GTK_GVFS_H_

#include "wx/defs.h"

#if wxUSE_MIMETYPE && wxUSE_LIBGNOMEVFS

#include "wx/string.h"
#include "wx/unix/mimetype.h"

//----------------------------------------------------------------------------
// wxGnomeVFSMimeTypesManagerImpl
//----------------------------------------------------------------------------

class wxGnomeVFSMimeTypesManagerImpl: public wxMimeTypesManagerImpl
{
public:
    wxGnomeVFSMimeTypesManagerImpl() { }

protected:
    virtual bool DoAssociation(const wxString& strType,
                       const wxString& strIcon,
                       wxMimeTypeCommands *entry,
                       const wxArrayString& strExtensions,
                       const wxString& strDesc);
};

//----------------------------------------------------------------------------
// wxGnomeVFSMimeTypesManagerFactory
//----------------------------------------------------------------------------

class wxGnomeVFSMimeTypesManagerFactory: public wxMimeTypesManagerFactory
{
public:
    wxGnomeVFSMimeTypesManagerFactory() {}

    virtual wxMimeTypesManagerImpl *CreateMimeTypesManagerImpl();
};

#endif
  // wxUSE_MIMETYPE

#endif
