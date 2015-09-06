/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        src/osx/carbon/menuitem.cpp
// Purpose:     wxMenuItem implementation
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#include "wx/menuitem.h"
#include "wx/stockitem.h"

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/menu.h"
#endif // WX_PRECOMP

#include "wx/osx/private.h"

// because on mac carbon everything is done through MenuRef APIs both implementation
// classes are in menu.cpp
