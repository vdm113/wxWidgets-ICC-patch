/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/os2/brush.h
// Purpose:     wxBrush class
// Author:      David Webster
// Modified by:
// Created:     10/13/99
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_BRUSH_H_
#define _WX_BRUSH_H_

#include "wx/bitmap.h"

// Brush
class WXDLLIMPEXP_CORE wxBrush: public wxBrushBase
{
public:
    wxBrush();
    wxBrush(const wxColour& rCol, wxBrushStyle nStyle = wxBRUSHSTYLE_SOLID);
#if WXWIN_COMPATIBILITY_3_0
    wxDEPRECATED( wxBrush(const wxColour& col, int style) );
#endif
    wxBrush(const wxBitmap& rStipple);
    virtual ~wxBrush();

    bool operator == (const wxBrush& rBrush) const;
    inline bool operator != (const wxBrush& rBrush) const { return !(*this == rBrush); }

    virtual void SetColour(const wxColour& rColour);
    virtual void SetColour(unsigned char cRed, unsigned char cGreen, unsigned char cBrush);
    virtual void SetPS(HPS hPS);
    virtual void SetStyle(wxBrushStyle nStyle);
    virtual void SetStipple(const wxBitmap& rStipple);

    wxColour GetColour(void) const;
    wxBrushStyle GetStyle(void) const;
    wxBitmap* GetStipple(void) const;
    int       GetPS(void) const;

#if WXWIN_COMPATIBILITY_3_0
    wxDEPRECATED( void SetStyle(int style) )
        { SetStyle((wxBrushStyle)style); }
#endif

    //
    // Implementation
    //

    //
    // Useful helper: create the brush resource
    //
    bool     RealizeResource(void);
    virtual WXHANDLE GetResourceHandle(void) const;
    bool     FreeResource(bool bForce = false);
    bool     IsFree(void) const;

protected:
    virtual wxGDIRefData *CreateGDIRefData() const;
    virtual wxGDIRefData *CloneGDIRefData(const wxGDIRefData *data) const;

private:
    DECLARE_DYNAMIC_CLASS(wxBrush)
}; // end of CLASS wxBrush

#endif
    // _WX_BRUSH_H_
