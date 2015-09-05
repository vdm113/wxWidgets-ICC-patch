/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/////////////////////////////////////////////////////////////////////////////
// Name:        wx/dfb/private.h
// Purpose:     private helpers for wxDFB implementation
// Author:      Vaclav Slavik
// Created:     2006-08-09
// Copyright:   (c) 2006 REA Elektronik GmbH
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DFB_PRIVATE_H_
#define _WX_DFB_PRIVATE_H_

#include "wx/intl.h"
#include "wx/log.h"

#include "wx/dfb/wrapdfb.h"
#include <directfb_version.h>

//-----------------------------------------------------------------------------
// misc helpers
//-----------------------------------------------------------------------------

/// Convert DirectFB timestamp to wxEvent one:
#define wxDFB_EVENT_TIMESTAMP(event) \
        ((event).timestamp.tv_sec * 1000 + (event).timestamp.tv_usec / 1000)

/**
    Check if DirectFB library version is at least @a major.@a minor.@a release.

    @see wxCHECK_VERSION
 */
#define wxCHECK_DFB_VERSION(major,minor,release) \
    (DIRECTFB_MAJOR_VERSION > (major) || \
    (DIRECTFB_MAJOR_VERSION == (major) && \
        DIRECTFB_MINOR_VERSION > (minor)) || \
    (DIRECTFB_MAJOR_VERSION == (major) && \
        DIRECTFB_MINOR_VERSION == (minor) && \
            DIRECTFB_MICRO_VERSION >= (release)))

#endif // _WX_DFB_PRIVATE_H_
