/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/x11/brush.h
// Purpose:     wxBrush class
// Author:      Julian Smart, Robert Roebling
// Modified by:
// Created:     17/09/98
// Copyright:   (c) Julian Smart, Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_BRUSH_H_
#define _WX_BRUSH_H_

#include "wx/gdiobj.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_FWD_CORE wxBrush;
class WXDLLIMPEXP_FWD_CORE wxColour;
class WXDLLIMPEXP_FWD_CORE wxBitmap;

//-----------------------------------------------------------------------------
// wxBrush
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxBrush : public wxBrushBase
{
public:
    wxBrush() { }

    wxBrush( const wxColour &colour, wxBrushStyle style = wxBRUSHSTYLE_SOLID );
    wxBrush( const wxBitmap &stippleBitmap );
    virtual ~wxBrush();

    bool operator==(const wxBrush& brush) const;
    bool operator!=(const wxBrush& brush) const { return !(*this == brush); }

    wxBrushStyle GetStyle() const;
    wxColour GetColour() const;
    wxBitmap *GetStipple() const;

    void SetColour( const wxColour& col );
    void SetColour( unsigned char r, unsigned char g, unsigned char b );
    void SetStyle( wxBrushStyle style );
    void SetStipple( const wxBitmap& stipple );


    wxDEPRECATED_MSG("use wxBRUSHSTYLE_XXX constants")
    wxBrush(const wxColour& col, int style);

    wxDEPRECATED_MSG("use wxBRUSHSTYLE_XXX constants")
    void SetStyle(int style) { SetStyle((wxBrushStyle)style); }

protected:
    virtual wxGDIRefData *CreateGDIRefData() const;
    virtual wxGDIRefData *CloneGDIRefData(const wxGDIRefData *data) const;

    wxDECLARE_DYNAMIC_CLASS(wxBrush);
};

#endif // _WX_BRUSH_H_
