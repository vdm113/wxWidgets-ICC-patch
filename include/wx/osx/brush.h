/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/osx/brush.h
// Purpose:     wxBrush class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_BRUSH_H_
#define _WX_BRUSH_H_

#include "wx/gdicmn.h"
#include "wx/gdiobj.h"
#include "wx/bitmap.h"

class WXDLLIMPEXP_FWD_CORE wxBrush;

// Brush
class WXDLLIMPEXP_CORE wxBrush: public wxBrushBase
{
public:
    wxBrush();
    wxBrush(const wxColour& col, wxBrushStyle style = wxBRUSHSTYLE_SOLID);
    wxBrush(const wxBitmap& stipple);
    virtual ~wxBrush();

    virtual void SetColour(const wxColour& col) ;
    virtual void SetColour(unsigned char r, unsigned char g, unsigned char b) ;
    virtual void SetStyle(wxBrushStyle style)  ;
    virtual void SetStipple(const wxBitmap& stipple)  ;

    bool operator==(const wxBrush& brush) const;
    bool operator!=(const wxBrush& brush) const { return !(*this == brush); }

    wxColour GetColour() const;
    wxBrushStyle GetStyle() const ;
    wxBitmap *GetStipple() const ;


    wxDEPRECATED_MSG("use wxBRUSHSTYLE_XXX constants")
    wxBrush(const wxColour& col, int style);

    wxDEPRECATED_MSG("use wxBRUSHSTYLE_XXX constants")
    void SetStyle(int style) { SetStyle((wxBrushStyle)style); }

protected:
    virtual wxGDIRefData *CreateGDIRefData() const;
    virtual wxGDIRefData *CloneGDIRefData(const wxGDIRefData *data) const;

private:
    wxDECLARE_DYNAMIC_CLASS(wxBrush);
};

#endif // _WX_BRUSH_H_
