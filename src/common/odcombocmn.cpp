/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/odcombocmn.cpp
// Purpose:     wxOwnerDrawnComboBox common code
// Author:      Jaakko Salli
// Modified by:
// Created:     Apr-30-2006
// Copyright:   (c) 2005 Jaakko Salli
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_ODCOMBOBOX

#include "wx/odcombo.h"

#ifndef WX_PRECOMP
    #include "wx/log.h"
    #include "wx/combobox.h"
    #include "wx/dcclient.h"
    #include "wx/settings.h"
    #include "wx/dialog.h"
#endif

#include "wx/combo.h"

// ----------------------------------------------------------------------------
// XTI
// ----------------------------------------------------------------------------

wxIMPLEMENT_DYNAMIC_CLASS2_XTI(wxOwnerDrawnComboBox, wxComboCtrl, \
                               wxControlWithItems, "wx/odcombo.h")

wxBEGIN_PROPERTIES_TABLE(wxOwnerDrawnComboBox)
wxEND_PROPERTIES_TABLE()

wxEMPTY_HANDLERS_TABLE(wxOwnerDrawnComboBox)

wxCONSTRUCTOR_5( wxOwnerDrawnComboBox , wxWindow* , Parent , wxWindowID , \
                 Id , wxString , Value , wxPoint , Position , wxSize , Size )

#endif // wxUSE_ODCOMBOBOX
