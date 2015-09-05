/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/////////////////////////////////////////////////////////////////////////////
// Name:        wx/imagpng.h
// Purpose:     wxImage PNG handler
// Author:      Robert Roebling
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_IMAGPNG_H_
#define _WX_IMAGPNG_H_

#include "wx/defs.h"

//-----------------------------------------------------------------------------
// wxPNGHandler
//-----------------------------------------------------------------------------

#if wxUSE_LIBPNG

#include "wx/image.h"
#include "wx/versioninfo.h"

#define wxIMAGE_OPTION_PNG_FORMAT    wxT("PngFormat")
#define wxIMAGE_OPTION_PNG_BITDEPTH  wxT("PngBitDepth")
#define wxIMAGE_OPTION_PNG_FILTER    wxT("PngF")
#define wxIMAGE_OPTION_PNG_COMPRESSION_LEVEL       wxT("PngZL")
#define wxIMAGE_OPTION_PNG_COMPRESSION_MEM_LEVEL   wxT("PngZM")
#define wxIMAGE_OPTION_PNG_COMPRESSION_STRATEGY    wxT("PngZS")
#define wxIMAGE_OPTION_PNG_COMPRESSION_BUFFER_SIZE wxT("PngZB")

enum
{
    wxPNG_TYPE_COLOUR = 0,
    wxPNG_TYPE_GREY = 2,
    wxPNG_TYPE_GREY_RED = 3,
    wxPNG_TYPE_PALETTE = 4
};

class WXDLLIMPEXP_CORE wxPNGHandler: public wxImageHandler
{
public:
    inline wxPNGHandler()
    {
        m_name = wxT("PNG file");
        m_extension = wxT("png");
        m_type = wxBITMAP_TYPE_PNG;
        m_mime = wxT("image/png");
    }

    static wxVersionInfo GetLibraryVersionInfo();

#if wxUSE_STREAMS
    virtual bool LoadFile( wxImage *image, wxInputStream& stream, bool verbose=true, int index=-1 ) wxOVERRIDE;
    virtual bool SaveFile( wxImage *image, wxOutputStream& stream, bool verbose=true ) wxOVERRIDE;
protected:
    virtual bool DoCanRead( wxInputStream& stream ) wxOVERRIDE;
#endif

private:
    wxDECLARE_DYNAMIC_CLASS(wxPNGHandler);
};

#endif
  // wxUSE_LIBPNG

#endif
  // _WX_IMAGPNG_H_

