/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xpmdecod.h
// Purpose:     wxXPMDecoder, XPM reader for wxImage and wxBitmap
// Author:      Vaclav Slavik
// Copyright:   (c) 2001 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XPMDECOD_H_
#define _WX_XPMDECOD_H_

#include "wx/defs.h"

#if wxUSE_IMAGE && wxUSE_XPM

class WXDLLIMPEXP_FWD_CORE wxImage;
class WXDLLIMPEXP_FWD_BASE wxInputStream;

// --------------------------------------------------------------------------
// wxXPMDecoder class
// --------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxXPMDecoder
{
public:
    // constructor, destructor, etc.
    wxXPMDecoder() {}
    ~wxXPMDecoder() {}

#if wxUSE_STREAMS
    // Is the stream XPM file?
    // NOTE: this function modifies the current stream position
    bool CanRead(wxInputStream& stream);

    // Read XPM file from the stream, parse it and create image from it
    wxImage ReadFile(wxInputStream& stream);
#endif

    // Read directly from XPM data (as passed to wxBitmap ctor):
    wxImage ReadData(const char* const* xpm_data);

#ifdef __BORLANDC__
    // needed for Borland 5.5
    wxImage ReadData(char** xpm_data)
        { return ReadData(const_cast<const char* const*>(xpm_data)); }
#endif
};

#endif // wxUSE_IMAGE && wxUSE_XPM

#endif  // _WX_XPM_H_
