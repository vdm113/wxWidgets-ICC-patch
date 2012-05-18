/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif defined(__INTEL_COMPILER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////

// Name:        src/cocoa/dcscreen.cpp
// Purpose:     wxScreenDCImpl class
// Author:      David Elliott
// Modified by:
// Created:     2003/03/16
// RCS-ID:      $Id$
// Copyright:   (c) 2002 David Elliott
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#include "wx/cocoa/dcscreen.h"

#ifndef WX_PRECOMP
#endif //WX_PRECOMP

//-----------------------------------------------------------------------------
// wxMemoryDC
//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxScreenDCImpl,wxCocoaDCImpl)

wxScreenDCImpl::wxScreenDCImpl(wxScreenDC *owner)
:   wxCocoaDCImpl(owner)
{
    m_ok = false;
}

wxScreenDCImpl::wxScreenDCImpl(wxScreenDC *owner, wxDC *WXUNUSED(dc) )
:   wxCocoaDCImpl(owner)
{
    m_ok = false;
}

wxScreenDCImpl::~wxScreenDCImpl(void)
{
}
