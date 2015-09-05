/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/arcall.cpp
// Purpose:     wxArchive link all archive streams
// Author:      Mike Wetherell
// Copyright:   (c) 2006 Mike Wetherell
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_ARCHIVE_STREAMS

#if wxUSE_ZIPSTREAM
#include "wx/zipstrm.h"
#endif
#if wxUSE_TARSTREAM
#include "wx/tarstrm.h"
#endif

// Reference archive classes to ensure they are linked into a statically
// linked program that uses Find or GetFirst to look for an archive handler.
// It is in its own file so that the user can override this behaviour by
// providing their own implementation.

void wxUseArchiveClasses()
{
#if wxUSE_ZIPSTREAM
    wxZipClassFactory();
#endif
#if wxUSE_TARSTREAM
    wxTarClassFactory();
#endif
}

#endif // wxUSE_ARCHIVE_STREAMS
