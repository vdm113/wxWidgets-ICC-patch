/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        playerdg.h
// Purpose:     Forty Thieves patience game
// Author:      Chris Breeze
// Modified by:
// Created:     21/07/97
// Copyright:   (c) 1993-1998 Chris Breeze
// Licence:     wxWindows licence
//---------------------------------------------------------------------------
// Last modified: 22nd July 1998 - ported to wxWidgets 2.0
/////////////////////////////////////////////////////////////////////////////
#ifndef _PLAYERDG_H_
#define _PLAYERDG_H_

class PlayerSelectionDialog : public wxDialog
{
public:
    PlayerSelectionDialog(wxWindow* parent, ScoreFile* file);
    virtual ~PlayerSelectionDialog(){};

    const wxString& GetPlayersName();
    void ButtonCallback(wxCommandEvent& event);
    void SelectCallback(wxCommandEvent& event);
    void OnSize(wxSizeEvent& event);

    DECLARE_EVENT_TABLE()

protected:
    friend void SelectCallback(wxListBox&, wxCommandEvent&);
    void OnCloseWindow(wxCloseEvent& event);

private:
    ScoreFile* m_scoreFile;
    wxString m_player;
    wxButton* m_OK;
    wxButton* m_cancel;
    wxTextCtrl* m_textField;
};

#endif
