/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif defined(__INTEL_COMPILER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

///////////////////////////////////////////////////////////////////////////////

// Name:        wx/cocoa/objc/NSMenu.h
// Purpose:     WXNSMenu class
// Author:      David Elliott
// Modified by:
// Created:     2007/04/20 (move from NSMenu.mm)
// RCS-ID:      $Id$
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
