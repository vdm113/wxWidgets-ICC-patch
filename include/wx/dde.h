/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/dde.h
// Purpose:     DDE base header
// Author:      Julian Smart
// Modified by:
// Created:
// Copyright:   (c) Julian Smart
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DDE_H_BASE_
#define _WX_DDE_H_BASE_

#include "wx/list.h"

class WXDLLIMPEXP_FWD_BASE wxDDEClient;
class WXDLLIMPEXP_FWD_BASE wxDDEServer;
class WXDLLIMPEXP_FWD_BASE wxDDEConnection;

WX_DECLARE_USER_EXPORTED_LIST(wxDDEClient, wxDDEClientList, WXDLLIMPEXP_BASE);
WX_DECLARE_USER_EXPORTED_LIST(wxDDEServer, wxDDEServerList, WXDLLIMPEXP_BASE);
WX_DECLARE_USER_EXPORTED_LIST(wxDDEConnection, wxDDEConnectionList, WXDLLIMPEXP_BASE);

#if defined(__WINDOWS__)
    #include "wx/msw/dde.h"
#else
    #error DDE is only supported under Windows
#endif

#endif
    // _WX_DDE_H_BASE_
