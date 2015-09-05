/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_grid.h
// Purpose:     XML resource handler for wxGrid
// Author:      Agron Selimaj
// Created:     2005/08/11
// Copyright:   (c) 2005 Agron Selimaj, Freepour Controls Inc.
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_GRD_H_
#define _WX_XH_GRD_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_GRID

class WXDLLIMPEXP_XRC wxGridXmlHandler : public wxXmlResourceHandler
{
    wxDECLARE_DYNAMIC_CLASS(wxGridXmlHandler);

public:
    wxGridXmlHandler();

    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
};

#endif // wxUSE_XRC && wxUSE_GRID

#endif // _WX_XH_GRD_H_
