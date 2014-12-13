/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/config.h
// Purpose:     wxConfig base header
// Author:      Julian Smart
// Modified by:
// Created:
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CONFIG_H_BASE_
#define _WX_CONFIG_H_BASE_

#include "wx/confbase.h"

#if wxUSE_CONFIG

// ----------------------------------------------------------------------------
// define the native wxConfigBase implementation
// ----------------------------------------------------------------------------

// under Windows we prefer to use the native implementation but can be forced
// to use the file-based one
#if defined(__WINDOWS__) && wxUSE_CONFIG_NATIVE
    #include "wx/msw/regconf.h"
    #define wxConfig  wxRegConfig
#else // either we're under Unix or wish to always use config files
    #include "wx/fileconf.h"
    #define wxConfig wxFileConfig
#endif

#endif // wxUSE_CONFIG

#endif // _WX_CONFIG_H_BASE_
