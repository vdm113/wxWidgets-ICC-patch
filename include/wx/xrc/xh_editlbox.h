/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_editlbox.h
// Purpose:     declaration of wxEditableListBox XRC handler
// Author:      Vadim Zeitlin
// Created:     2009-06-04
// Copyright:   (c) 2009 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XRC_XH_EDITLBOX_H_
#define _WX_XRC_XH_EDITLBOX_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_XRC && wxUSE_EDITABLELISTBOX

// ----------------------------------------------------------------------------
// wxEditableListBoxXmlHandler: XRC handler for wxEditableListBox
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_XRC wxEditableListBoxXmlHandler : public wxXmlResourceHandler
{
public:
    wxEditableListBoxXmlHandler();

    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

private:
    bool m_insideBox;
    wxArrayString m_items;

    wxDECLARE_DYNAMIC_CLASS(wxEditableListBoxXmlHandler);
};

#endif // wxUSE_XRC && wxUSE_EDITABLELISTBOX

#endif // _WX_XRC_XH_EDITLBOX_H_

