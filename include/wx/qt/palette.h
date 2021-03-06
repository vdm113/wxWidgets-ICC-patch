/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/palette.h
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_PALETTE_H_
#define _WX_QT_PALETTE_H_

class WXDLLIMPEXP_CORE wxPalette : public wxPaletteBase
{
public:
    wxPalette();
    wxPalette(int n, unsigned char *red, unsigned char *green, unsigned char *blue);
    
    bool Create(int n, unsigned char *red, unsigned char *green, unsigned char *blue);
    
    bool GetRGB(int pixel, unsigned char *red, unsigned char *green, unsigned char *blue) const;
    int GetPixel(unsigned char red, unsigned char green, unsigned char blue) const;

protected:
    virtual wxGDIRefData *CreateGDIRefData() const;
    virtual wxGDIRefData *CloneGDIRefData(const wxGDIRefData *data) const;

};

#endif // _WX_QT_PALETTE_H_
