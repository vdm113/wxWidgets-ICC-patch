/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/palette.h
// Purpose:     Common header and base class for wxPalette
// Author:      Julian Smart
// Modified by:
// Created:
// Copyright:   (c) Julian Smart
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PALETTE_H_BASE_
#define _WX_PALETTE_H_BASE_

#include "wx/defs.h"

#if wxUSE_PALETTE

#include "wx/object.h"
#include "wx/gdiobj.h"

// wxPaletteBase
class WXDLLIMPEXP_CORE wxPaletteBase: public wxGDIObject
{
public:
    virtual ~wxPaletteBase() { }

    virtual int GetColoursCount() const { wxFAIL_MSG( wxT("not implemented") ); return 0; }
};

#if defined(__WXMSW__)
    #include "wx/msw/palette.h"
#elif defined(__WXX11__) || defined(__WXMOTIF__)
    #include "wx/x11/palette.h"
#elif defined(__WXGTK__)
    #include "wx/generic/paletteg.h"
#elif defined(__WXMAC__)
    #include "wx/osx/palette.h"
#elif defined(__WXQT__)
    #include "wx/qt/palette.h"
#endif

#endif // wxUSE_PALETTE

#endif // _WX_PALETTE_H_BASE_
