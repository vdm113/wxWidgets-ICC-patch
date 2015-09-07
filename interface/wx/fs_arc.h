/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/fs_arc.h
// Purpose:     Archive file system
// Author:      Vaclav Slavik, Mike Wetherell
// Copyright:   (c) 1999 Vaclav Slavik, (c) 2006 Mike Wetherell
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxArchiveFSHandler

    A file system handler for accessing files inside of archives.
*/
class wxArchiveFSHandler : public wxFileSystemHandler
{
public:
    wxArchiveFSHandler();
    virtual ~wxArchiveFSHandler();
    void Cleanup();
};


typedef wxArchiveFSHandler wxZipFSHandler;
