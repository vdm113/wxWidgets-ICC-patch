/////////////////////////////////////////////////////////////////////////////
// Name:        wx/mac/carbon/colour.h
// Purpose:     wxColour class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id$
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_COLOUR_H_
#define _WX_COLOUR_H_

#include "wx/object.h"
#include "wx/string.h"

#include "wx/osx/core/cfref.h"

struct RGBColor;

// Colour
class WXDLLIMPEXP_CORE wxColour: public wxColourBase
{
public:
    // constructors
    // ------------
    DEFINE_STD_WXCOLOUR_CONSTRUCTORS

    // default copy ctor and dtor are ok

    // accessors
    virtual bool IsOk() const { return m_cgColour; }

    ChannelType Red() const { return m_red; }
    ChannelType Green() const { return m_green; }
    ChannelType Blue() const { return m_blue; }
    ChannelType Alpha() const { return m_alpha; }

    // comparison
    bool operator == (const wxColour& colour) const;

    bool operator != (const wxColour& colour) const { return !(*this == colour); }

    CGColorRef GetPixel() const { return m_cgColour; };

    CGColorRef GetCGColor() const { return m_cgColour; };
    CGColorRef CreateCGColor() const { return wxCFRetain( (CGColorRef)m_cgColour ); };

#if wxOSX_USE_CARBON
    void GetRGBColor( RGBColor *col ) const;
#endif

    // Mac-specific ctor and assignment operator from the native colour
    // assumes ownership of CGColorRef
    wxColour( CGColorRef col );
#if wxOSX_USE_CARBON
    wxColour(const RGBColor& col);
    wxColour& operator=(const RGBColor& col);
#endif
    wxColour& operator=(CGColorRef col);
    wxColour& operator=(const wxColour& col);

protected :
    virtual void
    InitRGBA(ChannelType r, ChannelType g, ChannelType b, ChannelType a);
#if wxOSX_USE_CARBON
    void InitRGBColor( const RGBColor& col );
#endif
    void InitCGColorRef( CGColorRef col );
private:
    wxCFRef<CGColorRef>     m_cgColour;

    ChannelType             m_red;
    ChannelType             m_blue;
    ChannelType             m_green;
    ChannelType             m_alpha;

    DECLARE_DYNAMIC_CLASS(wxColour)
};

#endif
  // _WX_COLOUR_H_
