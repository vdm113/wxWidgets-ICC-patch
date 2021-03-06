/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        dialogs.h
// Purpose:     Life! dialogs
// Author:      Guillermo Rodriguez Garcia, <guille@iies.es>
// Modified by:
// Created:     Jan/2000
// Copyright:   (c) 2000, Guillermo Rodriguez Garcia
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _LIFE_DIALOGS_H_
#define _LIFE_DIALOGS_H_

#include "life.h"
#include "game.h"


// --------------------------------------------------------------------------
// LifeSamplesDialog
// --------------------------------------------------------------------------

class LifeSamplesDialog : public wxDialog
{
public:
    // ctor and dtor
    LifeSamplesDialog(wxWindow *parent);
    virtual ~LifeSamplesDialog();

    // members
    const LifePattern& GetPattern();

    // event handlers
    void OnListBox(wxCommandEvent &event);

private:
    // any class wishing to process wxWidgets events must use this macro
    wxDECLARE_EVENT_TABLE();

    int         m_value;
    wxListBox  *m_list;
    wxTextCtrl *m_text;
    LifeCanvas *m_canvas;
    Life       *m_life;
};

// --------------------------------------------------------------------------
// LifeAboutDialog
// --------------------------------------------------------------------------

class LifeAboutDialog : public wxDialog
{
public:
    // ctor
    LifeAboutDialog(wxWindow *parent);
};


#endif  // _LIFE_DIALOGS_H_
