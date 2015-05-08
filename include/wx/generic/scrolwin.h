/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/scrolwin.h
// Purpose:     generic wxScrollHelper
// Author:      Vadim Zeitlin
// Created:     2008-12-24 (replacing old file with the same name)
// Copyright:   (c) 2008 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GENERIC_SCROLLWIN_H_
#define _WX_GENERIC_SCROLLWIN_H_

#include "wx/recguard.h"

// ----------------------------------------------------------------------------
// generic wxScrollHelper implementation
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxScrollHelper : public wxScrollHelperBase
{
public:
    wxScrollHelper(wxWindow *winToScroll);

    // implement base class pure virtuals
    virtual void AdjustScrollbars();
    virtual bool IsScrollbarShown(int orient) const;

protected:
    virtual void DoScroll(int x, int y);
    virtual void DoShowScrollbars(wxScrollbarVisibility horz,
                                  wxScrollbarVisibility vert);

private:
    // helper of AdjustScrollbars(): does the work for the single scrollbar
    //
    // notice that the parameters passed by non-const references are modified
    // by this function
    void DoAdjustScrollbar(int orient,
                           int clientSize,
                           int virtSize,
                           int pixelsPerUnit,
                           int& scrollUnits,
                           int& scrollPosition,
                           int& scrollLinesPerPage,
                           wxScrollbarVisibility visibility);


    wxScrollbarVisibility m_xVisibility,
                          m_yVisibility;
    wxRecursionGuardFlag m_adjustScrollFlagReentrancy;

    wxDECLARE_NO_COPY_CLASS(wxScrollHelper);
};

#endif // _WX_GENERIC_SCROLLWIN_H_

