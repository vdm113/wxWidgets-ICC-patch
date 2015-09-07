/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        scoredg.h
// Purpose:     Forty Thieves patience game
// Author:      Chris Breeze
// Modified by:
// Created:     21/07/97
// Copyright:   (c) 1993-1998 Chris Breeze
// Licence:     wxWindows licence
//---------------------------------------------------------------------------
// Last modified: 22nd July 1998 - ported to wxWidgets 2.0
/////////////////////////////////////////////////////////////////////////////
#ifndef _SCOREDG_H_
#define _SCOREDG_H_

class ScoreDialog : public wxDialog
{
public:
    ScoreDialog(wxWindow* parent, ScoreFile* file);
    virtual ~ScoreDialog(){};

    void Display();

protected:
    void OnCloseWindow(wxCloseEvent& event);

private:
    ScoreFile* m_scoreFile;
    wxButton* m_OK;

	wxDECLARE_EVENT_TABLE();
};

#endif
