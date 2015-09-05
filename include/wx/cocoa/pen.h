/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/////////////////////////////////////////////////////////////////////////////
// Name:        wx/cocoa/pen.h
// Purpose:     wxPen class
// Author:      David Elliott
// Modified by:
// Created:     2003/08/02 (stubs from 22.03.2003)
// Copyright:   (c) 2003 David Elliott
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_COCOA_PEN_H__
#define __WX_COCOA_PEN_H__

#include "wx/gdiobj.h"
#include "wx/gdicmn.h"

class WXDLLIMPEXP_FWD_CORE wxColour;
class WXDLLIMPEXP_FWD_CORE wxBitmap;

// ========================================================================
// wxPen
// ========================================================================
class WXDLLIMPEXP_CORE wxPen: public wxGDIObject
{
public:
    wxPen();
    wxPen(const wxColour& col, int width = 1, wxPenStyle style = wxPENSTYLE_SOLID);
    wxPen(const wxBitmap& stipple, int width);
    virtual ~wxPen();

    // FIXME: operator==() is wrong
    bool operator==(const wxPen& pen) const { return m_refData == pen.m_refData; }
    bool operator!=(const wxPen& pen) const { return !(*this == pen); }

    void SetColour(const wxColour& col) ;
    void SetColour(unsigned char r, unsigned char g, unsigned char b)  ;

    void SetWidth(int width);
    void SetStyle(wxPenStyle style);
    void SetStipple(const wxBitmap& stipple);
    void SetDashes(int nb_dashes, const wxDash *dash);
    void SetJoin(wxPenJoin join);
    void SetCap(wxPenCap cap);

    wxColour GetColour() const;
    int GetWidth() const;
    wxPenStyle GetStyle() const;
    wxPenJoin GetJoin() const;
    wxPenCap GetCap() const;
    int GetDashes(wxDash **ptr) const;
    wxBitmap *GetStipple() const;


    wxDEPRECATED_MSG("use wxPENSTYLE_XXX constants")
    wxPen(const wxColour& col, int width, int style);

    wxDEPRECATED_MSG("use wxPENSTYLE_XXX constants")
    void SetStyle(int style) { SetStyle((wxPenStyle)style); }

    // Cocoa-specific
    WX_NSColor GetNSColor();
    int GetCocoaLineDash(const CGFloat **pattern);

protected:
    wxGDIRefData *CreateGDIRefData() const;
    wxGDIRefData *CloneGDIRefData(const wxGDIRefData *data) const;

    DECLARE_DYNAMIC_CLASS(wxPen)
};

#endif // __WX_COCOA_PEN_H__
