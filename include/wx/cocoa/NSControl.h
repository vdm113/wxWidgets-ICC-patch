/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/NSControl.h
// Purpose:     wxCocoaNSControl class
// Author:      David Elliott
// Modified by:
// Created:     2003/02/15
// RCS-ID:      $Id$
// Copyright:   (c) 2003 David Elliott
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WX_COCOA_NSCONTROL_H__
#define __WX_COCOA_NSCONTROL_H__

#include "wx/hashmap.h"
#include "wx/cocoa/ObjcAssociate.h"

WX_DECLARE_OBJC_HASHMAP(NSControl);
class wxCocoaNSControl
{
    WX_DECLARE_OBJC_INTERFACE(NSControl)
public:
    virtual void CocoaTarget_action() {}
//    virtual void Cocoa_didChangeText(void) = 0;

    virtual ~wxCocoaNSControl() { }

protected:
    static struct objc_object *sm_cocoaTarget;
};

#endif // _WX_COCOA_NSCONTROL_H_
