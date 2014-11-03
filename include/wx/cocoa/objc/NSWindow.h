/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/objc/NSWindow.h
// Purpose:     WXNSWindow class
// Author:      David Elliott
// Modified by:
// Created:     2007/04/20 (move from NSWindow.mm)
// Copyright:   (c) 2003 David Elliott
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#import <AppKit/NSWindow.h>
#import <AppKit/NSPanel.h>

#include "wx/cocoa/objc/objc_uniquifying.h"

// ============================================================================
// @class WXNSWindow
// ============================================================================
@interface WXNSWindow : NSWindow
{
}

- (BOOL)canBecomeKeyWindow;
- (BOOL)canBecomeMainWindow;
@end // WXNSWindow
WX_DECLARE_GET_OBJC_CLASS(WXNSWindow,NSWindow)

// ============================================================================
// @class WXNSPanel
// ============================================================================
@interface WXNSPanel : NSPanel
{
}

- (BOOL)canBecomeKeyWindow;
- (BOOL)canBecomeMainWindow;
@end // WXNSPanel
WX_DECLARE_GET_OBJC_CLASS(WXNSPanel,NSPanel)

