/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/palette.cpp
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/palette.h"

wxPalette::wxPalette()
{
}

wxPalette::wxPalette(int WXUNUSED(n), unsigned char *WXUNUSED(red), unsigned char *WXUNUSED(green), unsigned char *WXUNUSED(blue))
{
}

bool wxPalette::Create(int WXUNUSED(n), unsigned char *WXUNUSED(red), unsigned char *WXUNUSED(green), unsigned char *WXUNUSED(blue))
{
    return false;
}

bool wxPalette::GetRGB(int WXUNUSED(pixel), unsigned char *WXUNUSED(red), unsigned char *WXUNUSED(green), unsigned char *WXUNUSED(blue)) const
{
    return false;
}

int wxPalette::GetPixel(unsigned char WXUNUSED(red), unsigned char WXUNUSED(green), unsigned char WXUNUSED(blue)) const
{
    return false;
}


wxGDIRefData *wxPalette::CreateGDIRefData() const
{
    return NULL;
}

wxGDIRefData *wxPalette::CloneGDIRefData(const wxGDIRefData *WXUNUSED(data)) const
{
    return NULL;
}

