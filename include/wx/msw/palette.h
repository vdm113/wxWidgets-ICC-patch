/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/palette.h
// Purpose:     wxPalette class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PALETTE_H_
#define _WX_PALETTE_H_

#include "wx/gdiobj.h"

class WXDLLIMPEXP_CORE wxPalette : public wxPaletteBase
{
public:
    wxPalette() { }
    wxPalette(int n,
              const unsigned char *red, const unsigned char *green, const unsigned char *blue)
    {
        Create(n, red, green, blue);
    }

    bool Create(int n,
                const unsigned char *red, const unsigned char *green, const unsigned char *blue);

    virtual int GetColoursCount() const;

    int
    GetPixel(unsigned char red, unsigned char green, unsigned char blue) const;

    bool
    GetRGB(int pixel,
           unsigned char *red, unsigned char *green, unsigned char *blue) const;

    // implementation
    WXHPALETTE GetHPALETTE() const;
    void SetHPALETTE(WXHPALETTE pal);

protected:
    virtual wxGDIRefData *CreateGDIRefData() const;
    virtual wxGDIRefData *CloneGDIRefData(const wxGDIRefData *data) const;

private:
    wxDECLARE_DYNAMIC_CLASS(wxPalette);
};

#endif // _WX_PALETTE_H_
