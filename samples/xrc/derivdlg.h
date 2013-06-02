/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

//-----------------------------------------------------------------------------
// Name:        derivdlg.h
// Purpose:     XML resources sample: A derived dialog
// Author:      Robert O'Connor (rob@medicalmnemonics.com), Vaclav Slavik
// RCS-ID:      $Id$
// Copyright:   (c) Robert O'Connor and Vaclav Slavik
// Licence:     wxWindows licence
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Begin single inclusion of this .h file condition
//-----------------------------------------------------------------------------

#ifndef _DERIVDLG_H_
#define _DERIVDLG_H_

//-----------------------------------------------------------------------------
// Headers
//-----------------------------------------------------------------------------

#include "wx/dialog.h"

//-----------------------------------------------------------------------------
// Class definition: PreferencesDialog
//-----------------------------------------------------------------------------

// A derived dialog.
class PreferencesDialog : public wxDialog
{

public:

    // Constructor.
    /*
       \param parent The parent window. Simple constructor.
     */
    PreferencesDialog( wxWindow* parent );

    // Destructor.
    ~PreferencesDialog(){};

private:

    // Stuff to do when "My Button" gets clicked
    void OnMyButtonClicked( wxCommandEvent &event );

    // Stuff to do when a "My Checkbox" gets updated
    // (drawn, or it changes its value)
    void OnUpdateUIMyCheckbox( wxUpdateUIEvent &event );

    // Override base class functions of a wxDialog.
    void OnOK( wxCommandEvent &event );

    // Any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()

};

//-----------------------------------------------------------------------------
// End single inclusion of this .h file condition
//-----------------------------------------------------------------------------

#endif  //_DERIVDLG_H_
