/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif defined(__INTEL_COMPILER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

///////////////////////////////////////////////////////////////////////////////

// Name:        wx/univ/custombgwin.h
// Purpose:     wxUniv implementation of wxCustomBackgroundWindow.
// Author:      Vadim Zeitlin
// Created:     2011-10-10
// RCS-ID:      $Id: wxhead.h,v 1.12 2010-04-22 12:44:51 zeitlin Exp $
// Copyright:   (c) 2011 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIV_CUSTOMBGWIN_H_
#define _WX_UNIV_CUSTOMBGWIN_H_

// ----------------------------------------------------------------------------
// wxCustomBackgroundWindow
// ----------------------------------------------------------------------------

template <class W>
class wxCustomBackgroundWindow : public W,
                                 public wxCustomBackgroundWindowBase
{
public:
    typedef W BaseWindowClass;

    wxCustomBackgroundWindow() { }

protected:
    virtual void DoSetBackgroundBitmap(const wxBitmap& bmp)
    {
        // We have support for background bitmap even at the base class level.
        BaseWindowClass::SetBackground(bmp, wxALIGN_NOT, wxTILE);
    }

    wxDECLARE_NO_COPY_TEMPLATE_CLASS(wxCustomBackgroundWindow, W);
};

#endif // _WX_UNIV_CUSTOMBGWIN_H_
