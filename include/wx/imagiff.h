/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/imagiff.h
// Purpose:     wxImage handler for Amiga IFF images
// Author:      Steffen Gutmann
// Copyright:   (c) Steffen Gutmann, 2002
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_IMAGE_IFF_H_
#define _WX_IMAGE_IFF_H_

#include "wx/image.h"

//-----------------------------------------------------------------------------
// wxIFFHandler
//-----------------------------------------------------------------------------

#if wxUSE_IMAGE && wxUSE_IFF

class WXDLLIMPEXP_CORE wxIFFHandler : public wxImageHandler
{
public:
    wxIFFHandler()
    {
        m_name = wxT("IFF file");
        m_extension = wxT("iff");
        m_type = wxBITMAP_TYPE_IFF;
        m_mime = wxT("image/x-iff");
    }

#if wxUSE_STREAMS
    virtual bool LoadFile(wxImage *image, wxInputStream& stream, bool verbose=true, int index=-1) wxOVERRIDE;
    virtual bool SaveFile(wxImage *image, wxOutputStream& stream, bool verbose=true) wxOVERRIDE;
protected:
    virtual bool DoCanRead(wxInputStream& stream) wxOVERRIDE;
#endif

    wxDECLARE_DYNAMIC_CLASS(wxIFFHandler);
};

#endif // wxUSE_IMAGE && wxUSE_IFF

#endif // _WX_IMAGE_IFF_H_
