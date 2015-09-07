/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/NSScroller.h
// Purpose:     wxCocoaNSScroller class
// Author:      David Elliott
// Modified by:
// Created:     2004/04/27
// Copyright:   (c) 2004 David Elliott
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_COCOA_NSSCROLLER_H__
#define _WX_COCOA_NSSCROLLER_H__

#include "wx/hashmap.h"
#include "wx/cocoa/ObjcAssociate.h"
#include "wx/cocoa/ObjcRef.h"

DECLARE_WXCOCOA_OBJC_CLASS(NSScroller);

WX_DECLARE_OBJC_HASHMAP(NSScroller);

class wxCocoaNSScroller
{
    WX_DECLARE_OBJC_INTERFACE_HASHMAP(NSScroller);
public:
    void AssociateNSScroller(WX_NSScroller cocoaNSScroller);
    void DisassociateNSScroller(WX_NSScroller cocoaNSScroller)
    {
        if(cocoaNSScroller)
            sm_cocoaHash.erase(cocoaNSScroller);
    }

    virtual void Cocoa_wxNSScrollerAction(void) = 0;
    virtual ~wxCocoaNSScroller() { }

protected:
    static const wxObjcAutoRefFromAlloc<struct objc_object*> sm_cocoaTarget;
};

#endif // _WX_COCOA_NSSCROLLER_H__
