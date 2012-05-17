/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif defined(__INTEL_COMPILER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

///////////////////////////////////////////////////////////////////////////////

// Name:        wx/univ/statusbr.h
// Purpose:     wxStatusBarUniv: wxStatusBar for wxUniversal declaration
// Author:      Vadim Zeitlin
// Modified by:
// Created:     14.10.01
// RCS-ID:      $Id$
// Copyright:   (c) 2001 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIV_STATUSBR_H_
#define _WX_UNIV_STATUSBR_H_

#include "wx/univ/inpcons.h"
#include "wx/arrstr.h"

// ----------------------------------------------------------------------------
// wxStatusBarUniv: a window near the bottom of the frame used for status info
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxStatusBarUniv : public wxStatusBarBase
{
public:
    wxStatusBarUniv() { Init(); }

    wxStatusBarUniv(wxWindow *parent,
                    wxWindowID id = wxID_ANY,
                    long style = wxSTB_DEFAULT_STYLE,
                    const wxString& name = wxPanelNameStr)
    {
        Init();

        (void)Create(parent, id, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id = wxID_ANY,
                long style = wxSTB_DEFAULT_STYLE,
                const wxString& name = wxPanelNameStr);

    // implement base class methods
    virtual void SetFieldsCount(int number = 1, const int *widths = NULL);
    virtual void SetStatusWidths(int n, const int widths[]);

    virtual bool GetFieldRect(int i, wxRect& rect) const;
    virtual void SetMinHeight(int height);

    virtual int GetBorderX() const;
    virtual int GetBorderY() const;

    // wxInputConsumer pure virtual
    virtual wxWindow *GetInputWindow() const
        { return const_cast<wxStatusBar*>(this); }

protected:
    virtual void DoUpdateStatusText(int i);

    // recalculate the field widths
    void OnSize(wxSizeEvent& event);

    // draw the statusbar
    virtual void DoDraw(wxControlRenderer *renderer);

    // tell them about our preferred height
    virtual wxSize DoGetBestSize() const;

    // override DoSetSize() to prevent the status bar height from changing
    virtual void DoSetSize(int x, int y,
                           int width, int height,
                           int sizeFlags = wxSIZE_AUTO);

    // get the (fixed) status bar height
    wxCoord GetHeight() const;

    // get the rectangle containing all the fields and the border between them
    //
    // also updates m_widthsAbs if necessary
    wxRect GetTotalFieldRect(wxCoord *borderBetweenFields);

    // get the rect for this field without ani side effects (see code)
    wxRect DoGetFieldRect(int n) const;

    // common part of all ctors
    void Init();

private:
    // the current status fields strings
    //wxArrayString m_statusText;

    // the absolute status fields widths
    wxArrayInt m_widthsAbs;

    DECLARE_DYNAMIC_CLASS(wxStatusBarUniv)
    DECLARE_EVENT_TABLE()
    WX_DECLARE_INPUT_CONSUMER()
};

#endif // _WX_UNIV_STATUSBR_H_

