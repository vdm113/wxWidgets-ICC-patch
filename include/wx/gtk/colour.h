/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/colour.h
// Purpose:
// Author:      Robert Roebling
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GTK_COLOUR_H_
#define _WX_GTK_COLOUR_H_

#ifdef __WXGTK3__
typedef struct _GdkRGBA GdkRGBA;
#endif

//-----------------------------------------------------------------------------
// wxColour
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxColour : public wxColourBase
{
public:
    // constructors
    // ------------
    DEFINE_STD_WXCOLOUR_CONSTRUCTORS
    wxColour(const GdkColor& gdkColor);
#ifdef __WXGTK3__
    wxColour(const GdkRGBA& gdkRGBA);
#endif

    virtual ~wxColour();

    bool operator==(const wxColour& col) const;
    bool operator!=(const wxColour& col) const { return !(*this == col); }

    unsigned char Red() const wxOVERRIDE;
    unsigned char Green() const wxOVERRIDE;
    unsigned char Blue() const wxOVERRIDE;
    unsigned char Alpha() const wxOVERRIDE;

    // Implementation part
#ifdef __WXGTK3__
    operator const GdkRGBA*() const;
#else
    void CalcPixel( GdkColormap *cmap );
    int GetPixel() const;
#endif
    const GdkColor *GetColor() const;

protected:
    virtual void
    InitRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) wxOVERRIDE;

    virtual bool FromString(const wxString& str) wxOVERRIDE;

    DECLARE_DYNAMIC_CLASS(wxColour)
};

#endif // _WX_GTK_COLOUR_H_
