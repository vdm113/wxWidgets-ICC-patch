/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/filtall.cpp
// Purpose:     Link all filter streams
// Author:      Mike Wetherell
// Copyright:   (c) 2006 Mike Wetherell
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_STREAMS

#if wxUSE_ZLIB
#include "wx/zstream.h"
#endif

// Reference filter classes to ensure they are linked into a statically
// linked program that uses Find or GetFirst to look for an filter handler.
// It is in its own file so that the user can override this behaviour by
// providing their own implementation.

void wxUseFilterClasses()
{
#if wxUSE_ZLIB
    wxZlibClassFactory();
    wxGzipClassFactory();
#endif
}

#endif // wxUSE_STREAMS
