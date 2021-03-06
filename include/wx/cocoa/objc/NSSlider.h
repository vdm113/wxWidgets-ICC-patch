/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/objc/NSSlider.h
// Purpose:     WXNSSlider class
// Author:      David Elliott
// Modified by:
// Created:     2007/08/10 (move from NSSlider.mm)
// Copyright:   (c) 2007 Software 2000 Ltd.
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WX_COCOA_OBJC_NSSLIDER_H__
#define __WX_COCOA_OBJC_NSSLIDER_H__

#include "wx/cocoa/objc/objc_uniquifying.h"

#import <AppKit/NSSlider.h>

// ============================================================================
// @class WXNSSlider
// ============================================================================

@interface WXNSSlider : NSSlider
@end

WX_DECLARE_GET_OBJC_CLASS(WXNSSlider,NSSlider)

// ============================================================================
// @class WXNSSliderCell
// ============================================================================

@interface WXNSSliderCell : NSSliderCell
@end

WX_DECLARE_GET_OBJC_CLASS(WXNSSliderCell,NSSliderCell)

#endif //ndef __WX_COCOA_OBJC_NSSLIDER_H__
