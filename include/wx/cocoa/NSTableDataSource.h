/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/NSTableDataSource.h
// Purpose:     wxCocoaNSTableDataSource Objective-C class
// Author:      David Elliott
// Modified by:
// Created:     2003/08/05
// Copyright:   (c) 2003 David Elliott
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WX_COCOA_NSTABLEDATASOURCE_H__
#define __WX_COCOA_NSTABLEDATASOURCE_H__

#include "wx/cocoa/objc/objc_uniquifying.h"

#import <Foundation/NSObject.h>

// ============================================================================
// @class wxCocoaNSTableDataSource
// ============================================================================
@interface wxCocoaNSTableDataSource : NSObject
{
}

// NSTableDataSource is a loosely defined protocol consisting of the
// following two message implementations
- (int)numberOfRowsInTableView: (NSTableView *)tableView;
- (id)tableView:(NSTableView *)tableView
    objectValueForTableColumn: (NSTableColumn *)tableColumn
    row: (int)rowIndex;
@end // wxCocoaNSTableDataSource
WX_DECLARE_GET_OBJC_CLASS(wxCocoaNSTableDataSource,NSObject)

#endif // _WX_COCOA_NSTABLEDATASOURCE_H_
