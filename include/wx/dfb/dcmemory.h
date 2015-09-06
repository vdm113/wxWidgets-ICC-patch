/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/dfb/dcmemory.h
// Purpose:     wxMemoryDC class declaration
// Created:     2006-08-10
// Author:      Vaclav Slavik
// Copyright:   (c) 2006 REA Elektronik GmbH
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DFB_DCMEMORY_H_
#define _WX_DFB_DCMEMORY_H_

#include "wx/dfb/dc.h"
#include "wx/bitmap.h"

class WXDLLIMPEXP_CORE wxMemoryDCImpl : public wxDFBDCImpl
{
public:
    wxMemoryDCImpl(wxMemoryDC *owner);
    wxMemoryDCImpl(wxMemoryDC *owner, wxBitmap& bitmap);
    wxMemoryDCImpl(wxMemoryDC *owner, wxDC *dc); // create compatible DC

    // override wxMemoryDC-specific base class virtual methods
    virtual const wxBitmap& GetSelectedBitmap() const { return m_bmp; }
    virtual wxBitmap& GetSelectedBitmap() { return m_bmp; }
    virtual void DoSelect(const wxBitmap& bitmap);

private:
    void Init();

    wxBitmap m_bmp;

    wxDECLARE_DYNAMIC_CLASS(wxMemoryDCImpl);
};

#endif // _WX_DFB_DCMEMORY_H_

