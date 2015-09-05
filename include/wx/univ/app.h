/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


///////////////////////////////////////////////////////////////////////////////
// Name:        wx/univ/app.h
// Purpose:     wxUniversalApp class extends wxApp for wxUniv port
// Author:      Vadim Zeitlin
// Modified by:
// Created:     06.08.00
// Copyright:   (c) 2000 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIX_APP_H_
#define _WX_UNIX_APP_H_

class WXDLLIMPEXP_CORE wxUniversalApp : public wxApp
{
public:
};

#endif // _WX_UNIX_APP_H_

