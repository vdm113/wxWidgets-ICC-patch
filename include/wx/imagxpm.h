/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/imagxpm.h
// Purpose:     wxImage XPM handler
// Author:      Vaclav Slavik
// Copyright:   (c) 2001 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_IMAGXPM_H_
#define _WX_IMAGXPM_H_

#include "wx/image.h"

#if wxUSE_XPM

//-----------------------------------------------------------------------------
// wxXPMHandler
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxXPMHandler : public wxImageHandler
{
public:
    inline wxXPMHandler()
    {
        m_name = wxT("XPM file");
        m_extension = wxT("xpm");
        m_type = wxBITMAP_TYPE_XPM;
        m_mime = wxT("image/xpm");
    }

#if wxUSE_STREAMS
    virtual bool LoadFile( wxImage *image, wxInputStream& stream, bool verbose=true, int index=-1 ) wxOVERRIDE;
    virtual bool SaveFile( wxImage *image, wxOutputStream& stream, bool verbose=true ) wxOVERRIDE;
protected:
    virtual bool DoCanRead( wxInputStream& stream ) wxOVERRIDE;
#endif

private:
    wxDECLARE_DYNAMIC_CLASS(wxXPMHandler);
};

#endif // wxUSE_XPM

#endif // _WX_IMAGXPM_H_
