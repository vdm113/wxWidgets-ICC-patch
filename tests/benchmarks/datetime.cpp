/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        tests/benchmarks/datetime.cpp
// Purpose:     wxDateTime benchmarks
// Author:      Vadim Zeitlin
// Created:     2011-05-23
// Copyright:   (c) 2011 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/datetime.h"

#include "bench.h"

BENCHMARK_FUNC(ParseDate)
{
    wxDateTime dt;
    return dt.ParseDate("May 23, 2011") && dt.GetMonth() == wxDateTime::May;
}

