/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/NSPanel.h
// Purpose:     wxCocoaNSPanel class
// Author:      David Elliott
// Modified by:
// Created:     2003/03/16
// Copyright:   (c) 2003 David Elliott
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WX_COCOA_NSPANEL_H__
#define __WX_COCOA_NSPANEL_H__

#include "wx/hashmap.h"
#include "wx/cocoa/ObjcAssociate.h"

WX_DECLARE_OBJC_HASHMAP(NSPanel);

class wxCocoaNSPanel
{
    WX_DECLARE_OBJC_INTERFACE(NSPanel)
};

#endif // _WX_COCOA_NSPANEL_H_
