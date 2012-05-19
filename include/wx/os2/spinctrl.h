/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////


// Name:        wx/os2/spinctrl.h
// Purpose:     wxSpinCtrl class declaration for Win32
// Author:      David Webster
// Modified by:
// Created:     10/15/99
// RCS-ID:      $Id$
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MSW_SPINCTRL_H_
#define _WX_MSW_SPINCTRL_H_

#include "wx/spinbutt.h"    // the base class
#include "wx/dynarray.h"
class WXDLLIMPEXP_FWD_CORE wxSpinCtrl;
WX_DEFINE_EXPORTED_ARRAY_PTR(wxSpinCtrl *, wxArraySpins);

// ----------------------------------------------------------------------------
// Under Win32 and OS2 PM, wxSpinCtrl is a wxSpinButton with a buddy
// text window whose contents is automatically updated when the spin
// control is clicked.
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxSpinCtrl : public wxSpinButton
{
public:
    wxSpinCtrl() { }
    wxSpinCtrl( wxWindow*       pParent
               ,wxWindowID      vId = wxID_ANY
               ,const wxString& rsValue = wxEmptyString
               ,const wxPoint&  rPos = wxDefaultPosition
               ,const wxSize&   rSize = wxDefaultSize
               ,long            lStyle = wxSP_ARROW_KEYS
               ,int             nMin = 0
               ,int             nMax = 100
               ,int             nInitial = 0
               ,const wxString& rsName = wxT("wxSpinCtrl")
              )
    {
        Create(pParent, vId, rsValue, rPos, rSize, lStyle, nMin, nMax, nInitial, rsName);
    }
    virtual ~wxSpinCtrl();

    bool Create(wxWindow*       pParent
               ,wxWindowID      vId = wxID_ANY
               ,const wxString& rsValue = wxEmptyString
               ,const wxPoint&  rPos = wxDefaultPosition
               ,const wxSize&   rSize = wxDefaultSize
               ,long            lStyle = wxSP_ARROW_KEYS
               ,int             nMin = 0
               ,int             nMax = 100
               ,int             nInitial = 0
               ,const wxString& rsName = wxT("wxSpinCtrl")
              );

    //
    // A wxTextCtrl-like method (but we can't have GetValue returning wxString
    // because the base class already has one returning int!)
    //
    void SetValue(const wxString& rsText);

    //
    // implementation only from now on
    // -------------------------------
    //
    virtual bool Enable(bool bEnable = true);

    virtual int  GetValue(void) const;

    virtual bool SetFont(const wxFont &rFont);
    virtual void SetFocus(void);

    inline virtual void SetValue(int nVal) { wxSpinButton::SetValue(nVal); }

    void SetSelection(long lFrom, long lTo);

    virtual bool Show(bool bShow = true);

    //
    // wxSpinButton doesn't accept focus, but we do
    //
    inline virtual bool AcceptsFocus(void) const { return false; }

    //
    // Return the spinctrl object whose buddy is the given window or NULL
    // Doesn't really do much under OS/2
    //
    static wxSpinCtrl*  GetSpinForTextCtrl(WXHWND hWndBuddy);

    //
    // Process a WM_COMMAND generated by the buddy text control
    //
    bool ProcessTextCommand( WXWORD wCmd
                            ,WXWORD wId
                           );

protected:
    virtual void          DoGetPosition( int* nlX
                                        ,int* nlY
                                       ) const;
    void                  DoMoveWindow( int nX
                                       ,int nY
                                       ,int nWidth
                                       ,int nHeight
                                      );
    virtual wxSize        DoGetBestSize(void) const;
    virtual void          DoGetSize( int* pnWidth
                                    ,int* pnHeight
                                   ) const;

    //
    // The handler for wxSpinButton events
    //
    void OnSpinChange(wxSpinEvent& rEvent);
    void OnChar(wxKeyEvent& rEvent);
    void OnSetFocus(wxFocusEvent& rEvent);

    WXHWND                          m_hWndBuddy;
    static wxArraySpins             m_svAllSpins;

private:
    DECLARE_DYNAMIC_CLASS(wxSpinCtrl)
    DECLARE_EVENT_TABLE()
}; // end of CLASS wxSpinCtrl

#endif // _WX_MSW_SPINCTRL_H_
