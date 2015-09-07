/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/objc/NSMenu.h
// Purpose:     WXNSMenu class
// Author:      David Elliott
// Modified by:
// Created:     2007/04/20 (move from NSMenu.mm)
// Copyright:   (c) 2002 David Elliott
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WX_COCOA_OBJC_NSMENU_H__
#define __WX_COCOA_OBJC_NSMENU_H__

#include "wx/cocoa/objc/objc_uniquifying.h"

#import <AppKit/NSMenu.h>

// ============================================================================
// @class WXNSMenu
// ============================================================================
@interface WXNSMenu : NSMenu
{
}

- (void)dealloc;

@end // WXNSMenu
WX_DECLARE_GET_OBJC_CLASS(WXNSMenu,NSMenu)

#endif //ndef __WX_COCOA_OBJC_NSMENU_H__
