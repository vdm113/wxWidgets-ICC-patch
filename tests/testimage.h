/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


///////////////////////////////////////////////////////////////////////////////
// Name:        tests/testimage.h
// Purpose:     Unit test helpers for dealing with wxImage.
// Author:      Vadim Zeitlin
// Copyright:   (c) 2011 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_TESTS_TESTIMAGE_H_
#define _WX_TESTS_TESTIMAGE_H_

#include "wx/image.h"

CPPUNIT_NS_BEGIN

template <>
struct assertion_traits<wxImage>
{
    static bool equal(const wxImage& i1, const wxImage& i2)
    {
        if ( i1.GetWidth() != i2.GetWidth() )
            return false;

        if ( i1.GetHeight() != i2.GetHeight() )
            return false;

        return memcmp(i1.GetData(), i2.GetData(),
                      i1.GetWidth()*i1.GetHeight()*3) == 0;
    }

    static std::string toString(const wxImage& image)
    {
        return wxString::Format("image of size %d*%d with%s alpha",
                                image.GetWidth(),
                                image.GetHeight(),
                                image.HasAlpha() ? "" : "out")
                .ToStdString();
    }
};

CPPUNIT_NS_END

#endif // _WX_TESTS_TESTIMAGE_H_
