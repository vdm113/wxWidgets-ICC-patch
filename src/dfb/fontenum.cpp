/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/dfb/fontenum.cpp
// Purpose:     wxFontEnumerator class
// Author:      Vaclav Slavik
// Created:     2006-08-10
// Copyright:   (c) 2006 REA Elektronik GmbH
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/fontenum.h"
#include "wx/private/fontmgr.h"

#if wxUSE_FONTENUM

// ----------------------------------------------------------------------------
// wxFontEnumerator
// ----------------------------------------------------------------------------

bool wxFontEnumerator::EnumerateFacenames(wxFontEncoding encoding,
                                          bool fixedWidthOnly)
{
    // we only support UTF-8 and system (which means "use any"):
    if ( encoding != wxFONTENCODING_SYSTEM && encoding != wxFONTENCODING_UTF8 )
        return false;

    bool found = false;
    const wxFontBundleList& list = wxFontsManager::Get()->GetBundles();

#if defined(__INTEL_COMPILER) // VDM auto patch
#   pragma ivdep
#endif
    for ( wxFontBundleList::const_iterator f = list.begin(); f != list.end(); ++f )
    {
        if ( fixedWidthOnly && !(*f)->IsFixed() )
            continue;

        found = true;
        if ( !OnFacename((*f)->GetName()) )
            break; // OnFacename() requests us to stop enumeration
    }

    return found;
}

bool wxFontEnumerator::EnumerateEncodings(const wxString& facename)
{
    return EnumerateEncodingsUTF8(facename);
}

#endif // wxUSE_FONTENUM
