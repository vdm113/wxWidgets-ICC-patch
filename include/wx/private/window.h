/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/private/window.h
// Purpose:     misc wxWindow helpers
// Author:      Vaclav Slavik
// Created:     2010-01-21
// Copyright:   (c) 2010 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PRIVATE_WINDOW_H_
#define _WX_PRIVATE_WINDOW_H_

#include "wx/gdicmn.h"

namespace wxPrivate
{

// Windows' computes dialog units using average character width over upper-
// and lower-case ASCII alphabet and not using the average character width
// metadata stored in the font; see
// http://support.microsoft.com/default.aspx/kb/145994 for detailed discussion.
//
// This helper function computes font dimensions in the same way. It works with
// either wxDC or wxWindow argument.
template<typename T>
inline wxSize GetAverageASCIILetterSize(const T& of_what)
{
    const wxStringCharType *TEXT_TO_MEASURE =
        wxS("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

    wxSize s = of_what.GetTextExtent(TEXT_TO_MEASURE);
    s.x = (s.x / 26 + 1) / 2;
    return s;
}

} // namespace wxPrivate

#endif // _WX_PRIVATE_WINDOW_H_
