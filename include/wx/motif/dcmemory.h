/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/motif/dcmemory.h
// Purpose:     wxMemoryDCImpl class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCMEMORY_H_
#define _WX_DCMEMORY_H_

#include "wx/motif/dcclient.h"

class WXDLLIMPEXP_CORE wxMemoryDCImpl : public wxWindowDCImpl
{
public:
    wxMemoryDCImpl(wxMemoryDC *owner) : wxWindowDCImpl(owner) { Init(); }
    wxMemoryDCImpl(wxMemoryDC *owner, wxBitmap& bitmap)
        : wxWindowDCImpl(owner)
    {
        Init();
        DoSelect(bitmap);
    }

    wxMemoryDCImpl(wxMemoryDC *owner, wxDC *dc);
    virtual ~wxMemoryDCImpl();

    virtual void DoGetSize( int *width, int *height ) const;
    virtual void DoSelect(const wxBitmap& bitmap);

private:
    friend class wxPaintDC;

    void Init();

    wxBitmap m_bitmap;

    wxDECLARE_DYNAMIC_CLASS(wxMemoryDCImpl);
};

#endif
// _WX_DCMEMORY_H_
