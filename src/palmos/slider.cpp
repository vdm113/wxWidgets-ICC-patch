/////////////////////////////////////////////////////////////////////////////
// Name:        src/palmos/slider.cpp
// Purpose:     wxSlider
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by: Wlodzimierz ABX Skiba - native implementation
// Created:     10/13/04
// RCS-ID:      $Id$
// Copyright:   (c) William Osborne, Wlodzimierz Skiba
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "slider.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#if wxUSE_SLIDER

#ifndef WX_PRECOMP
#include "wx/utils.h"
#include "wx/brush.h"
#include "wx/slider.h"
#endif

#include "wx/toplevel.h"

#if wxUSE_EXTENDED_RTTI
WX_DEFINE_FLAGS( wxSliderStyle )

wxBEGIN_FLAGS( wxSliderStyle )
    // new style border flags, we put them first to
    // use them for streaming out
    wxFLAGS_MEMBER(wxBORDER_SIMPLE)
    wxFLAGS_MEMBER(wxBORDER_SUNKEN)
    wxFLAGS_MEMBER(wxBORDER_DOUBLE)
    wxFLAGS_MEMBER(wxBORDER_RAISED)
    wxFLAGS_MEMBER(wxBORDER_STATIC)
    wxFLAGS_MEMBER(wxBORDER_NONE)

    // old style border flags
    wxFLAGS_MEMBER(wxSIMPLE_BORDER)
    wxFLAGS_MEMBER(wxSUNKEN_BORDER)
    wxFLAGS_MEMBER(wxDOUBLE_BORDER)
    wxFLAGS_MEMBER(wxRAISED_BORDER)
    wxFLAGS_MEMBER(wxSTATIC_BORDER)
    wxFLAGS_MEMBER(wxBORDER)

    // standard window styles
    wxFLAGS_MEMBER(wxTAB_TRAVERSAL)
    wxFLAGS_MEMBER(wxCLIP_CHILDREN)
    wxFLAGS_MEMBER(wxTRANSPARENT_WINDOW)
    wxFLAGS_MEMBER(wxWANTS_CHARS)
    wxFLAGS_MEMBER(wxFULL_REPAINT_ON_RESIZE)
    wxFLAGS_MEMBER(wxALWAYS_SHOW_SB )
    wxFLAGS_MEMBER(wxVSCROLL)
    wxFLAGS_MEMBER(wxHSCROLL)

    wxFLAGS_MEMBER(wxSL_HORIZONTAL)
    wxFLAGS_MEMBER(wxSL_VERTICAL)
    wxFLAGS_MEMBER(wxSL_AUTOTICKS)
    wxFLAGS_MEMBER(wxSL_LABELS)
    wxFLAGS_MEMBER(wxSL_LEFT)
    wxFLAGS_MEMBER(wxSL_TOP)
    wxFLAGS_MEMBER(wxSL_RIGHT)
    wxFLAGS_MEMBER(wxSL_BOTTOM)
    wxFLAGS_MEMBER(wxSL_BOTH)
    wxFLAGS_MEMBER(wxSL_SELRANGE)

wxEND_FLAGS( wxSliderStyle )

IMPLEMENT_DYNAMIC_CLASS_XTI(wxSlider, wxControl,"wx/scrolbar.h")

wxBEGIN_PROPERTIES_TABLE(wxSlider)
    wxEVENT_RANGE_PROPERTY( Scroll , wxEVT_SCROLL_TOP , wxEVT_SCROLL_ENDSCROLL , wxScrollEvent )
    wxEVENT_PROPERTY( Updated , wxEVT_COMMAND_SLIDER_UPDATED , wxCommandEvent )

    wxPROPERTY( Value , int , SetValue, GetValue , 0, 0 /*flags*/ , wxT("Helpstring") , wxT("group"))
    wxPROPERTY( Minimum , int , SetMin, GetMin, 0 , 0 /*flags*/ , wxT("Helpstring") , wxT("group"))
    wxPROPERTY( Maximum , int , SetMax, GetMax, 0 , 0 /*flags*/ , wxT("Helpstring") , wxT("group"))
    wxPROPERTY( PageSize , int , SetPageSize, GetLineSize, 1 , 0 /*flags*/ , wxT("Helpstring") , wxT("group"))
    wxPROPERTY( LineSize , int , SetLineSize, GetLineSize, 1 , 0 /*flags*/ , wxT("Helpstring") , wxT("group"))
    wxPROPERTY( ThumbLength , int , SetThumbLength, GetThumbLength, 1 , 0 /*flags*/ , wxT("Helpstring") , wxT("group"))
    wxPROPERTY_FLAGS( WindowStyle , wxSliderStyle , long , SetWindowStyleFlag , GetWindowStyleFlag , EMPTY_MACROVALUE , 0 /*flags*/ , wxT("Helpstring") , wxT("group")) // style
wxEND_PROPERTIES_TABLE()

wxBEGIN_HANDLERS_TABLE(wxSlider)
wxEND_HANDLERS_TABLE()

wxCONSTRUCTOR_8( wxSlider , wxWindow* , Parent , wxWindowID , Id , int , Value , int , Minimum , int , Maximum , wxPoint , Position , wxSize , Size , long , WindowStyle )
#else
IMPLEMENT_DYNAMIC_CLASS(wxSlider, wxControl)
#endif

// Slider
wxSlider::wxSlider()
{
}

bool wxSlider::Create(wxWindow *parent, wxWindowID id,
           int value, int minValue, int maxValue,
           const wxPoint& pos,
           const wxSize& size, long style,
           const wxValidator& validator,
           const wxString& name)
{
    if(!wxControl::Create(parent, id, pos, size, style, validator, name))
        return false;

    FormType* form = GetParentForm();
    if(form==NULL)
        return false;

    SliderControlType *slider = CtlNewSliderControl (
                                   (void **)&form,
                                   GetId(),
                                   feedbackSliderCtl,
                                   NULL,
                                   0,
                                   0,
                                   pos.x,
                                   pos.y,
                                   size.x,
                                   size.y,
                                   minValue,
                                   maxValue,
                                   1,
                                   value
                              );

    if(slider==NULL)
        return false;

    Show();
    return true;
}

wxSlider::~wxSlider()
{
}

int wxSlider::GetMin() const
{
    ControlType *control = (ControlType *)GetObjectPtr();
    if(control==NULL)
        return 0;
    uint16_t ret;
    CtlGetSliderValues(control, &ret, NULL, NULL, NULL);
    return ret;
}

int wxSlider::GetMax() const
{
    ControlType *control = (ControlType *)GetObjectPtr();
    if(control==NULL)
        return 0;
    uint16_t ret;
    CtlGetSliderValues(control, NULL, &ret, NULL, NULL);
    return ret;
}

int wxSlider::GetPageSize() const
{
    ControlType *control = (ControlType *)GetObjectPtr();
    if(control==NULL)
        return 0;
    uint16_t ret;
    CtlGetSliderValues(control, NULL, NULL, &ret, NULL);
    return ret;
}

int wxSlider::GetValue() const
{
    ControlType *control = (ControlType *)GetObjectPtr();
    if(control==NULL)
        return 0;
    uint16_t ret;
    CtlGetSliderValues(control, NULL, NULL, NULL, &ret);
    return ret;
}

void wxSlider::SetValue(int value)
{
    SetIntValue(value);
}

wxSize wxSlider::DoGetBestSize() const
{
    return wxSize(0,0);
}


void wxSlider::SetRange(int minValue, int maxValue)
{
}

void wxSlider::SetTickFreq(int n, int pos)
{
}

void wxSlider::SetPageSize(int pageSize)
{
}

void wxSlider::ClearSel()
{
}

void wxSlider::ClearTicks()
{
}

void wxSlider::SetLineSize(int lineSize)
{
}

int wxSlider::GetLineSize() const
{
    return 0;
}

int wxSlider::GetSelEnd() const
{
    return 0;
}

int wxSlider::GetSelStart() const
{
    return 0;
}

void wxSlider::SetSelection(int minPos, int maxPos)
{
}

void wxSlider::SetThumbLength(int len)
{
}

int wxSlider::GetThumbLength() const
{
    return 0;
}

void wxSlider::SetTick(int tickPos)
{
}

// ----------------------------------------------------------------------------
// helpers
// ----------------------------------------------------------------------------

bool wxSlider::SendUpdatedEvent()
{
    wxCommandEvent event(wxEVT_COMMAND_SLIDER_UPDATED, GetId());
    event.SetEventObject(this);
    event.SetInt(GetValue());
    return ProcessCommand(event);
}

void wxSlider::Command (wxCommandEvent & event)
{
}

#endif // wxUSE_SLIDER
