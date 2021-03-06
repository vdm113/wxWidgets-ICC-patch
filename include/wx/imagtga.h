/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/imagtga.h
// Purpose:     wxImage TGA handler
// Author:      Seth Jackson
// Copyright:   (c) 2005 Seth Jackson
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_IMAGTGA_H_
#define _WX_IMAGTGA_H_

#include "wx/image.h"

//-----------------------------------------------------------------------------
// wxTGAHandler
//-----------------------------------------------------------------------------

#if wxUSE_TGA

class WXDLLIMPEXP_CORE wxTGAHandler : public wxImageHandler
{
public:
    wxTGAHandler()
    {
        m_name = wxT("TGA file");
        m_extension = wxT("tga");
        m_altExtensions.Add(wxT("tpic"));
        m_type = wxBITMAP_TYPE_TGA;
        m_mime = wxT("image/tga");
    }

#if wxUSE_STREAMS
    virtual bool LoadFile(wxImage* image, wxInputStream& stream,
                            bool verbose = true, int index = -1) wxOVERRIDE;
    virtual bool SaveFile(wxImage* image, wxOutputStream& stream,
                             bool verbose = true) wxOVERRIDE;
protected:
    virtual bool DoCanRead(wxInputStream& stream) wxOVERRIDE;
#endif // wxUSE_STREAMS

    wxDECLARE_DYNAMIC_CLASS(wxTGAHandler);
};

#endif // wxUSE_TGA

#endif // _WX_IMAGTGA_H_
