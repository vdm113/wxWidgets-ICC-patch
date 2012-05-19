/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

///////////////////////////////////////////////////////////////////////////////


// Name:        wx/msw/ownerdrw.h
// Purpose:     wxOwnerDrawn class
// Author:      Marcin Malich
// Modified by:
// Created:     2009-09-22
// RCS-ID:      $Id$
// Copyright:   (c) 2009 Marcin Malich <me@malcom.pl>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_OWNERDRW_H_
#define _WX_OWNERDRW_H_

#if wxUSE_OWNER_DRAWN

class WXDLLIMPEXP_CORE wxOwnerDrawn : public wxOwnerDrawnBase
{
public:
    wxOwnerDrawn() {}
    virtual ~wxOwnerDrawn() {}

    virtual bool OnDrawItem(wxDC& dc, const wxRect& rc,
                            wxODAction act, wxODStatus stat);
};

#endif // wxUSE_OWNER_DRAWN

#endif // _WX_OWNERDRW_H_
