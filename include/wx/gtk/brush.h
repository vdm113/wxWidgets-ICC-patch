/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/brush.h
// Purpose:
// Author:      Robert Roebling
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GTK_BRUSH_H_
#define _WX_GTK_BRUSH_H_

class WXDLLIMPEXP_FWD_CORE wxBitmap;
class WXDLLIMPEXP_FWD_CORE wxColour;

//-----------------------------------------------------------------------------
// wxBrush
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxBrush: public wxBrushBase
{
public:
    wxBrush() { }

    wxBrush( const wxColour &colour, wxBrushStyle style = wxBRUSHSTYLE_SOLID );
#if WXWIN_COMPATIBILITY_3_0
    wxDEPRECATED( wxBrush(const wxColour& col, int style) );
#endif
    wxBrush( const wxBitmap &stippleBitmap );
    virtual ~wxBrush();

    bool operator==(const wxBrush& brush) const;
    bool operator!=(const wxBrush& brush) const { return !(*this == brush); }

    wxBrushStyle GetStyle() const wxOVERRIDE;
    wxColour GetColour() const wxOVERRIDE;
    wxBitmap *GetStipple() const wxOVERRIDE;

    void SetColour( const wxColour& col ) wxOVERRIDE;
    void SetColour( unsigned char r, unsigned char g, unsigned char b ) wxOVERRIDE;
    void SetStyle( wxBrushStyle style ) wxOVERRIDE;
    void SetStipple( const wxBitmap& stipple ) wxOVERRIDE;

#if WXWIN_COMPATIBILITY_3_0
    wxDEPRECATED( void SetStyle(int style) )
        { SetStyle((wxBrushStyle)style); }
#endif

protected:
    virtual wxGDIRefData *CreateGDIRefData() const wxOVERRIDE;
    virtual wxGDIRefData *CloneGDIRefData(const wxGDIRefData *data) const wxOVERRIDE;

    DECLARE_DYNAMIC_CLASS(wxBrush)
};

#endif // _WX_GTK_BRUSH_H_
