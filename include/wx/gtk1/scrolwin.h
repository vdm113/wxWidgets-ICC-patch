/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk1/scrolwin.h
// Purpose:     wxScrolledWindow class
// Author:      Robert Roebling
// Modified by: Vadim Zeitlin (2005-10-10): wxScrolledWindow is now common
// Created:     01/02/97
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GTK_SCROLLWIN_H_
#define _WX_GTK_SCROLLWIN_H_

// ----------------------------------------------------------------------------
// wxScrolledWindow
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxScrollHelper : public wxScrollHelperBase
{
public:
    wxScrollHelper(wxWindow *win) : wxScrollHelperBase(win) { }

    // implement base class pure virtuals
    virtual void SetScrollbars(int pixelsPerUnitX, int pixelsPerUnitY,
                               int noUnitsX, int noUnitsY,
                               int xPos = 0, int yPos = 0,
                               bool noRefresh = false);
    virtual bool IsScrollbarShown(int orient) const;
    virtual void AdjustScrollbars();

protected:
    virtual void DoScroll(int x, int y);
    virtual void DoShowScrollbars(wxScrollbarVisibility horz,
                                  wxScrollbarVisibility vert);

    // this does (each) half of AdjustScrollbars() work
    void DoAdjustScrollbar(GtkAdjustment *adj,
                           int pixelsPerLine,
                           int winSize,
                           int virtSize,
                           int *pos,
                           int *lines,
                           int *linesPerPage);

    // and this does the same for Scroll()
    void DoScrollOneDir(int orient,
                        GtkAdjustment *adj,
                        int pos,
                        int pixelsPerLine,
                        int *posOld);

private:
    wxDECLARE_NO_COPY_CLASS(wxScrollHelper);
};

#endif // _WX_GTK_SCROLLWIN_H_

