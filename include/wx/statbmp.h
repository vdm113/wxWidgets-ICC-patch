/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/statbmp.h
// Purpose:     wxStaticBitmap class interface
// Author:      Vadim Zeitlin
// Modified by:
// Created:     25.08.00
// Copyright:   (c) 2000 Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_STATBMP_H_BASE_
#define _WX_STATBMP_H_BASE_

#include "wx/defs.h"

#if wxUSE_STATBMP

#include "wx/control.h"
#include "wx/bitmap.h"
#include "wx/icon.h"

extern WXDLLIMPEXP_DATA_CORE(const char) wxStaticBitmapNameStr[];

// a control showing an icon or a bitmap
class WXDLLIMPEXP_CORE wxStaticBitmapBase : public wxControl
{
public:
    wxStaticBitmapBase() { }
    virtual ~wxStaticBitmapBase();

    // our interface
    virtual void SetIcon(const wxIcon& icon) = 0;
    virtual void SetBitmap(const wxBitmap& bitmap) = 0;
    virtual wxBitmap GetBitmap() const = 0;
    virtual wxIcon GetIcon() const /* = 0 -- should be pure virtual */
    {
        // stub it out here for now as not all ports implement it (but they
        // should)
        return wxIcon();
    }

    // overridden base class virtuals
    virtual bool AcceptsFocus() const wxOVERRIDE { return false; }
    virtual bool HasTransparentBackground() wxOVERRIDE { return true; }

protected:
    // choose the default border for this window
    virtual wxBorder GetDefaultBorder() const wxOVERRIDE { return wxBORDER_NONE; }

    virtual wxSize DoGetBestSize() const wxOVERRIDE;

    wxDECLARE_NO_COPY_CLASS(wxStaticBitmapBase);
};

#if defined(__WXUNIVERSAL__)
    #include "wx/univ/statbmp.h"
#elif defined(__WXMSW__)
    #include "wx/msw/statbmp.h"
#elif defined(__WXMOTIF__)
    #include "wx/motif/statbmp.h"
#elif defined(__WXGTK20__)
    #include "wx/gtk/statbmp.h"
#elif defined(__WXGTK__)
    #include "wx/gtk1/statbmp.h"
#elif defined(__WXMAC__)
    #include "wx/osx/statbmp.h"
#elif defined(__WXQT__)
    #include "wx/qt/statbmp.h"
#endif

#endif // wxUSE_STATBMP

#endif
    // _WX_STATBMP_H_BASE_
