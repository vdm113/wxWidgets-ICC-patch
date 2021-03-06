/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        tests/testdate.h
// Purpose:     Unit test helpers for dealing with wxDateTime.
// Author:      Vadim Zeitlin
// Copyright:   (c) 2011 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_TESTS_TESTDATE_H_
#define _WX_TESTS_TESTDATE_H_

#include "wx/datetime.h"

// need this to be able to use CPPUNIT_ASSERT_EQUAL with wxDateTime objects
inline std::ostream& operator<<(std::ostream& ostr, const wxDateTime& dt)
{
    ostr << dt.FormatISOCombined(' ');

    return ostr;
}

// need this to be able to use CPPUNIT_ASSERT_EQUAL with wxDateSpan objects
inline std::ostream& operator<<(std::ostream& ostr, const wxDateSpan& span)
{
    ostr << span.GetYears() << "Y, "
         << span.GetMonths() << "M, "
         << span.GetWeeks() << "W, "
         << span.GetDays() << "D";

    return ostr;
}

WX_CPPUNIT_ALLOW_EQUALS_TO_INT(wxDateTime::wxDateTime_t)

#endif // _WX_TESTS_TESTDATE_H_
