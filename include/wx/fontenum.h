/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/fontenum.h
// Purpose:     wxFontEnumerator class for getting available fonts
// Author:      Julian Smart, Vadim Zeitlin
// Modified by: extended to enumerate more than just font facenames and works
//              not only on Windows now (VZ)
// Created:     04/01/98
// Copyright:   (c) Julian Smart, Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_FONTENUM_H_
#define _WX_FONTENUM_H_

#include "wx/defs.h"

#if wxUSE_FONTENUM

#include "wx/fontenc.h"
#include "wx/arrstr.h"

// ----------------------------------------------------------------------------
// wxFontEnumerator enumerates all available fonts on the system or only the
// fonts with given attributes
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxFontEnumerator
{
public:
    wxFontEnumerator() {}

    // virtual dtor for the base class
    virtual ~wxFontEnumerator() {}

    // start enumerating font facenames (either all of them or those which
    // support the given encoding) - will result in OnFacename() being
    // called for each available facename (until they are exhausted or
    // OnFacename returns false)
    virtual bool EnumerateFacenames
                 (
                    wxFontEncoding encoding = wxFONTENCODING_SYSTEM, // all
                    bool fixedWidthOnly = false
                 );

    // enumerate the different encodings either for given font facename or for
    // all facenames - will result in OnFontEncoding() being called for each
    // available (facename, encoding) couple
    virtual bool EnumerateEncodings(const wxString& facename = wxEmptyString);

    // callbacks which are called after one of EnumerateXXX() functions from
    // above is invoked - all of them may return false to stop enumeration or
    // true to continue with it

    // called by EnumerateFacenames
    virtual bool OnFacename(const wxString& WXUNUSED(facename))
        { return true; }

    // called by EnumerateEncodings
    virtual bool OnFontEncoding(const wxString& WXUNUSED(facename),
                                const wxString& WXUNUSED(encoding))
        { return true; }



    // convenience function that returns array of facenames.
    static wxArrayString
    GetFacenames(wxFontEncoding encoding = wxFONTENCODING_SYSTEM, // all
                 bool fixedWidthOnly = false);

    // convenience function that returns array of all available encodings.
    static wxArrayString GetEncodings(const wxString& facename = wxEmptyString);

    // convenience function that returns true if the given face name exist
    // in the user's system
    static bool IsValidFacename(const wxString &str);

private:
#ifdef wxHAS_UTF8_FONTS
    // helper for ports that only use UTF-8 encoding natively
    bool EnumerateEncodingsUTF8(const wxString& facename);
#endif

    wxDECLARE_NO_COPY_CLASS(wxFontEnumerator);
};

#endif // wxUSE_FONTENUM

#endif // _WX_FONTENUM_H_
