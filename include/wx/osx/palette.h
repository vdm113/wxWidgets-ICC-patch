/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/osx/palette.h
// Purpose:     wxPalette class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PALETTE_H_
#define _WX_PALETTE_H_

#include "wx/gdiobj.h"

#define M_PALETTEDATA ((wxPaletteRefData *)m_refData)

class WXDLLIMPEXP_CORE wxPalette : public wxPaletteBase
{
public:
    wxPalette();

    wxPalette(int n, const unsigned char *red, const unsigned char *green, const unsigned char *blue);
    virtual ~wxPalette();
    bool Create(int n, const unsigned char *red, const unsigned char *green, const unsigned char *blue);

    int GetPixel(unsigned char red, unsigned char green, unsigned char blue) const;
    bool GetRGB(int pixel, unsigned char *red, unsigned char *green, unsigned char *blue) const;

    virtual int GetColoursCount() const;

protected:
    virtual wxGDIRefData *CreateGDIRefData() const;
    virtual wxGDIRefData *CloneGDIRefData(const wxGDIRefData *data) const;

private:
    wxDECLARE_DYNAMIC_CLASS(wxPalette);
};

#endif // _WX_PALETTE_H_
