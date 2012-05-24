/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/imagpcx.h
// Purpose:     wxImage PCX handler
// Author:      Guillermo Rodriguez Garcia <guille@iies.es>
// RCS-ID:      $Id$
// Copyright:   (c) 1999 Guillermo Rodriguez Garcia
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_IMAGPCX_H_
#define _WX_IMAGPCX_H_

#include "wx/image.h"


//-----------------------------------------------------------------------------
// wxPCXHandler
//-----------------------------------------------------------------------------

#if wxUSE_PCX
class WXDLLIMPEXP_CORE wxPCXHandler : public wxImageHandler
{
public:
    inline wxPCXHandler()
    {
        m_name = wxT("PCX file");
        m_extension = wxT("pcx");
        m_type = wxBITMAP_TYPE_PCX;
        m_mime = wxT("image/pcx");
    }

#if wxUSE_STREAMS
    virtual bool LoadFile( wxImage *image, wxInputStream& stream, bool verbose=true, int index=-1 );
    virtual bool SaveFile( wxImage *image, wxOutputStream& stream, bool verbose=true );
protected:
    virtual bool DoCanRead( wxInputStream& stream );
#endif // wxUSE_STREAMS

private:
    DECLARE_DYNAMIC_CLASS(wxPCXHandler)
};
#endif // wxUSE_PCX


#endif
  // _WX_IMAGPCX_H_

