/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/fs_filter.h
// Purpose:     Filter file system handler
// Author:      Mike Wetherell
// Copyright:   (c) 2006 Mike Wetherell
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_FS_FILTER_H_
#define _WX_FS_FILTER_H_

#include "wx/defs.h"

#if wxUSE_FILESYSTEM

#include "wx/filesys.h"

//---------------------------------------------------------------------------
// wxFilterFSHandler
//---------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxFilterFSHandler : public wxFileSystemHandler
{
public:
    wxFilterFSHandler() : wxFileSystemHandler() { }
    virtual ~wxFilterFSHandler() { }

    virtual bool CanOpen(const wxString& location) wxOVERRIDE;
    virtual wxFSFile* OpenFile(wxFileSystem& fs, const wxString& location) wxOVERRIDE;

    virtual wxString FindFirst(const wxString& spec, int flags = 0) wxOVERRIDE;
    virtual wxString FindNext() wxOVERRIDE;

private:
    wxDECLARE_NO_COPY_CLASS(wxFilterFSHandler);
};

#endif // wxUSE_FILESYSTEM

#endif // _WX_FS_FILTER_H_
