/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/nonownedwnd.h
// Purpose:     wxNonOwnedWindow declaration for wxMSW.
// Author:      Vadim Zeitlin
// Created:     2011-10-09
// Copyright:   (c) 2011 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MSW_NONOWNEDWND_H_
#define _WX_MSW_NONOWNEDWND_H_

class wxNonOwnedWindowShapeImpl;

// ----------------------------------------------------------------------------
// wxNonOwnedWindow
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxNonOwnedWindow : public wxNonOwnedWindowBase
{
public:
    wxNonOwnedWindow();
    virtual ~wxNonOwnedWindow();

protected:
    virtual bool DoClearShape();
    virtual bool DoSetRegionShape(const wxRegion& region);
#if wxUSE_GRAPHICS_CONTEXT
    virtual bool DoSetPathShape(const wxGraphicsPath& path);

private:
    wxNonOwnedWindowShapeImpl* m_shapeImpl;
#endif // wxUSE_GRAPHICS_CONTEXT

    wxDECLARE_NO_COPY_CLASS(wxNonOwnedWindow);
};

#endif // _WX_MSW_NONOWNEDWND_H_
