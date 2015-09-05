/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/clrpicker.cpp
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/clrpicker.h"

wxColourPickerWidget::wxColourPickerWidget()
{
}

wxColourPickerWidget::wxColourPickerWidget(wxWindow *parent,
               wxWindowID id,
               const wxColour& initial,
               const wxPoint& pos,
               const wxSize& size,
               long style,
               const wxValidator& validator,
               const wxString& name )
{
    Create( parent, id, initial, pos, size, style, validator, name );
}


bool wxColourPickerWidget::Create(wxWindow *parent,
            wxWindowID id,
            const wxColour& initial,
            const wxPoint& pos,
            const wxSize& size,
            long style,
            const wxValidator& validator,
            const wxString& name )
{
    return wxGenericColourButton::Create( parent, id, initial, pos, size, style, validator, name );
}


void wxColourPickerWidget::UpdateColour()
{
    wxGenericColourButton::UpdateColour();
}

