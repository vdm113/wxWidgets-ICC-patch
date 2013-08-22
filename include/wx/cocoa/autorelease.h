/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
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
