/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif defined(__INTEL_COMPILER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

///////////////////////////////////////////////////////////////////////////////

// Name:        wx/cocoa/objc/NSWindow.h
// Purpose:     WXNSWindow class
// Author:      David Elliott
// Modified by:
// Created:     2007/04/20 (move from NSWindow.mm)
// RCS-ID:      $Id$
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

