/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/os2/pnghand.h
// Purpose:     PNG bitmap handler
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// Copyright:   (c) Microsoft, Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PNGHAND_H_
#define _WX_PNGHAND_H_

class WXDLLIMPEXP_CORE wxPNGFileHandler: public wxBitmapHandler
{
    DECLARE_DYNAMIC_CLASS(wxPNGFileHandler)
public:
    inline wxPNGFileHandler(void)
    {
        m_sName = "PNG bitmap file";
        m_sExtension = "bmp";
        m_lType = wxBITMAP_TYPE_PNG;
    }

    virtual bool LoadFile( wxBitmap*       pBitmap
                          ,const wxString& rName
                          ,HPS             hPs
                          ,long            lFlags
                          ,int             nDesiredWidth
                          ,int             nDesiredHeight
                         );
    virtual bool SaveFile( wxBitmap*        pBitmap
                          ,const wxString&  rName
                          ,int              nType
                          ,const wxPalette* pPalette = NULL
                         );
};

#endif
    // _WX_PNGHAND_H_
