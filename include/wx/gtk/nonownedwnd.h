/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/nonownedwnd.h
// Purpose:     wxGTK-specific wxNonOwnedWindow declaration.
// Author:      Vadim Zeitlin
// Created:     2011-10-12
// Copyright:   (c) 2011 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GTK_NONOWNEDWND_H_
#define _WX_GTK_NONOWNEDWND_H_

class wxNonOwnedWindowShapeImpl;

// ----------------------------------------------------------------------------
// wxNonOwnedWindow contains code common to wx{Popup,TopLevel}Window in wxGTK.
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxNonOwnedWindow : public wxNonOwnedWindowBase
{
public:
    wxNonOwnedWindow() { m_shapeImpl = NULL; }
    virtual ~wxNonOwnedWindow();

    // Overridden to actually set the shape when the window becomes realized.
    virtual void GTKHandleRealized() wxOVERRIDE;

protected:
    virtual bool DoClearShape() wxOVERRIDE;
    virtual bool DoSetRegionShape(const wxRegion& region) wxOVERRIDE;
#if wxUSE_GRAPHICS_CONTEXT
    virtual bool DoSetPathShape(const wxGraphicsPath& path) wxOVERRIDE;
#endif // wxUSE_GRAPHICS_CONTEXT


private:
    // If non-NULL, contains information about custom window shape.
    wxNonOwnedWindowShapeImpl* m_shapeImpl;

    wxDECLARE_NO_COPY_CLASS(wxNonOwnedWindow);
};

#endif // _WX_GTK_NONOWNEDWND_H_
