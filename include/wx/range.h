/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/////////////////////////////////////////////////////////////////////////////
// Name:        wx/range.h
// Purpose:     Range Value Class
// Author:      Stefan Csomor
// Modified by:
// Created:     2011-01-07
// Copyright:   (c) 2011 Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_RANGE_H_
#define _WX_RANGE_H_

#include "wx/defs.h"

class wxRange
{
public :
    wxRange(): m_minVal(0), m_maxVal(0) {}
    wxRange( int minVal, int maxVal) : m_minVal(minVal), m_maxVal(maxVal) {}
    int GetMin() const { return m_minVal; }
    int GetMax() const { return m_maxVal; }
private :
    int m_minVal;
    int m_maxVal;
};

#endif // _WX_RANGE_H_
