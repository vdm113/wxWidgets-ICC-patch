/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/autorelease.h
// Purpose:     Automatic NSAutoreleasePool functionality
// Author:      David Elliott
// Modified by:
// Created:     2003/07/11
// Copyright:   (c) 2003 David Elliott <dfe@cox.net>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_COCOA_AUTORELEASE_H__
#define __WX_COCOA_AUTORELEASE_H__

#import <Foundation/NSAutoreleasePool.h>

class wxAutoNSAutoreleasePool
{
public:
    wxAutoNSAutoreleasePool()
    {
        m_pool = [[NSAutoreleasePool alloc] init];
    }
    ~wxAutoNSAutoreleasePool()
    {
        [m_pool release];
    }
protected:
    NSAutoreleasePool *m_pool;
};

#endif //__WX_COCOA_AUTORELEASE_H__
