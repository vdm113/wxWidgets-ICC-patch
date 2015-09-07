/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/iosfwrap.h
// Purpose:     includes the correct stream-related forward declarations
// Author:      Jan van Dijk <jan@etpmod.phys.tue.nl>
// Modified by:
// Created:     18.12.2002
// Copyright:   wxWidgets team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#if wxUSE_STD_IOSTREAM

#if wxUSE_IOSTREAMH
    // There is no pre-ANSI iosfwd header so we include the full declarations.
#   include <iostream.h>
#else
#   include <iosfwd>
#endif

#ifdef __WINDOWS__
#   include "wx/msw/winundef.h"
#endif

#endif // wxUSE_STD_IOSTREAM

