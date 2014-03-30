/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/fs_inet.h
// Purpose:     HTTP and FTP file system
// Author:      Vaclav Slavik
// Copyright:   (c) 1999 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_FS_INET_H_
#define _WX_FS_INET_H_

#include "wx/defs.h"

#if wxUSE_FILESYSTEM && wxUSE_FS_INET && wxUSE_STREAMS && wxUSE_SOCKETS

#include "wx/filesys.h"

// ----------------------------------------------------------------------------
// wxInternetFSHandler
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_NET wxInternetFSHandler : public wxFileSystemHandler
{
    public:
        virtual bool CanOpen(const wxString& location) wxOVERRIDE;
        virtual wxFSFile* OpenFile(wxFileSystem& fs, const wxString& location) wxOVERRIDE;
};

#endif
  // wxUSE_FILESYSTEM && wxUSE_FS_INET && wxUSE_STREAMS && wxUSE_SOCKETS

#endif // _WX_FS_INET_H_

