/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif defined(__INTEL_COMPILER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////

// Name:        wx/x11/dcmemory.h
// Purpose:     wxMemoryDC class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DCMEMORY_H_
#define _WX_DCMEMORY_H_

#include "wx/dc.h"
#include "wx/dcmemory.h"
#include "wx/x11/dcclient.h"

class WXDLLIMPEXP_CORE wxMemoryDCImpl : public wxWindowDCImpl
{
public:
    wxMemoryDCImpl( wxDC* owner );
    wxMemoryDCImpl( wxDC* owner, wxBitmap& bitmap);
    wxMemoryDCImpl( wxDC* owner, wxDC *dc );
    virtual ~wxMemoryDCImpl();

    virtual const wxBitmap& GetSelectedBitmap() const;
    virtual wxBitmap& GetSelectedBitmap();

    // implementation
    wxBitmap  m_selected;

protected:
    virtual void DoGetSize( int *width, int *height ) const;
    virtual void DoSelect(const wxBitmap& bitmap);

private:
    void Init();

private:
    DECLARE_CLASS(wxMemoryDCImpl)
};

#endif
// _WX_DCMEMORY_H_
