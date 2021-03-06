/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/dcmemory.h
// Purpose:     wxMemoryDC base header
// Author:      Julian Smart
// Modified by:
// Created:
// Copyright:   (c) Julian Smart
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCMEMORY_H_BASE_
#define _WX_DCMEMORY_H_BASE_

#include "wx/dc.h"
#include "wx/bitmap.h"

//-----------------------------------------------------------------------------
// wxMemoryDC
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxMemoryDC: public wxDC
{
public:
    wxMemoryDC();
    wxMemoryDC( wxBitmap& bitmap );
    wxMemoryDC( wxDC *dc );

    // select the given bitmap to draw on it
    void SelectObject(wxBitmap& bmp);

    // select the given bitmap for read-only
    void SelectObjectAsSource(const wxBitmap& bmp);

    // get selected bitmap
    const wxBitmap& GetSelectedBitmap() const;
    wxBitmap& GetSelectedBitmap();

private:
    wxDECLARE_DYNAMIC_CLASS(wxMemoryDC);
};


#endif
    // _WX_DCMEMORY_H_BASE_
