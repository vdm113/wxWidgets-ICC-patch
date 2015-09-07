/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/os2/radiobut.h
// Purpose:     wxRadioButton class
// Author:      David Webster
// Modified by:
// Created:     10/12/99
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_RADIOBUT_H_
#define _WX_RADIOBUT_H_

#include "wx/control.h"

class WXDLLIMPEXP_CORE wxRadioButton: public wxControl
{
public:
    inline wxRadioButton() { Init(); }
    inline wxRadioButton( wxWindow*          pParent
                         ,wxWindowID         vId
                         ,const wxString&    rsLabel
                         ,const wxPoint&     rPos = wxDefaultPosition
                         ,const wxSize&      rSize = wxDefaultSize
                         ,long               lStyle = 0
                         ,const wxValidator& rValidator = wxDefaultValidator
                         ,const wxString&    rsName = wxRadioButtonNameStr
                         )
    {
        Init();

        Create( pParent
               ,vId
               ,rsLabel
               ,rPos
               ,rSize
               ,lStyle
               ,rValidator
               ,rsName
              );
    }

    bool Create( wxWindow* pParent
                ,wxWindowID         vId
                ,const wxString&    rsLabel
                ,const wxPoint&     rPos = wxDefaultPosition
                ,const wxSize&      rSize = wxDefaultSize
                ,long               lStyle = 0
                ,const wxValidator& rValidator = wxDefaultValidator
                ,const wxString&    rsName = wxRadioButtonNameStr
               );

    virtual void SetLabel(const wxString& rsLabel);
    virtual void SetValue(bool bVal);
    virtual bool GetValue(void) const ;

    bool            OS2Command( WXUINT wParam
                               ,WXWORD wId
                              );
    void            Command(wxCommandEvent& rEvent);
    virtual MRESULT OS2WindowProc( WXUINT   uMsg
                                  ,WXWPARAM wParam
                                  ,WXLPARAM lParam
                                 );
    virtual void    SetFocus(void);

protected:
    virtual wxBorder GetDefaultBorder() const { return wxBORDER_NONE; }
    virtual wxSize DoGetBestSize() const;

private:
    void Init(void);

    bool                            m_bFocusJustSet;

    DECLARE_DYNAMIC_CLASS(wxRadioButton)
}; // end of wxRadioButton

#endif
    // _WX_RADIOBUT_H_
