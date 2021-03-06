/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/cocoa/dcscreen.cpp
// Purpose:     wxScreenDCImpl class
// Author:      David Elliott
// Modified by:
// Created:     2003/03/16
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
