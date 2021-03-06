/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/private/fontfactory.h
// Purpose:     wxFontFactory class
// Author:      David Elliott
// Modified by:
// Created:     2007-10-13
// Copyright:   2007 David Elliott
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_COCOA_FONTFACTORY_H_
#define _WX_COCOA_FONTFACTORY_H_

/*! @discussion
    This class exists so that wxFont need not know about wxSettings
    The implementation, however, is in the font.mm file because the
    implementation needs to have knowledge of wxFontRefData.
 */
class wxCocoaFontFactory
{
public:
    /*! @abstract   Create an instance with the specified NSFont and underline flag.
     */
    static wxFont InstanceForNSFont(WX_NSFont cocoaFont, bool underlined = false);
};

#endif  //_WX_COCOA_FONTFACTORY_H_
