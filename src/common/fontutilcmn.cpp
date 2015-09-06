/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/fontutilcmn.cpp
// Purpose:     Font helper functions common for all ports
// Author:      Vaclav Slavik
// Modified by:
// Created:     2006-12-20
// Copyright:   (c) Vadim Zeitlin, Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/fontutil.h"
#include "wx/encinfo.h"

// ============================================================================
// implementation
// ============================================================================

#ifdef wxHAS_UTF8_FONTS

// ----------------------------------------------------------------------------
// wxNativeEncodingInfo
// ----------------------------------------------------------------------------

bool wxNativeEncodingInfo::FromString(const wxString& WXUNUSED(s))
{
    return false;
}

wxString wxNativeEncodingInfo::ToString() const
{
    return wxEmptyString;
}

bool wxTestFontEncoding(const wxNativeEncodingInfo& WXUNUSED(info))
{
    return true;
}

bool wxGetNativeFontEncoding(wxFontEncoding encoding,
                             wxNativeEncodingInfo *info)
{
    // all encodings are available because we translate text in any encoding to
    // UTF-8 internally anyhow
    info->facename.clear();
    info->encoding = encoding;

    return true;
}

#endif // wxHAS_UTF8_FONTS
