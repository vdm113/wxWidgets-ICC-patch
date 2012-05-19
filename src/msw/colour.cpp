/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////


// Name:        src/msw/colour.cpp
// Purpose:     wxColour class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/colour.h"

#ifndef WX_PRECOMP
    #include "wx/gdicmn.h"
#endif

#include "wx/msw/private.h"

#include <string.h>

// Colour

void wxColour::Init()
{
    m_isInit = false;
    m_pixel = 0;
    m_alpha =
    m_red =
    m_blue =
    m_green = 0;
}

void wxColour::InitRGBA(unsigned char r, unsigned char g, unsigned char b,
                        unsigned char a)
{
    m_red = r;
    m_green = g;
    m_blue = b;
    m_alpha = a;
    m_isInit = true;
    m_pixel = PALETTERGB(m_red, m_green, m_blue);
}
