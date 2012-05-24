/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/joystick.h
// Purpose:     wxJoystick base header
// Author:      wxWidgets Team
// Modified by:
// Created:
// Copyright:   (c) wxWidgets Team
// RCS-ID:      $Id$
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_JOYSTICK_H_BASE_
#define _WX_JOYSTICK_H_BASE_

#include "wx/defs.h"

#if wxUSE_JOYSTICK

#if defined(__WINDOWS__)
#include "wx/msw/joystick.h"
#elif defined(__WXMOTIF__)
#include "wx/unix/joystick.h"
#elif defined(__WXGTK__)
#include "wx/unix/joystick.h"
#elif defined(__WXX11__)
#include "wx/unix/joystick.h"
#elif defined(__DARWIN__)
#include "wx/osx/core/joystick.h"
#elif defined(__WXMAC__)
#include "wx/osx/joystick.h"
#elif defined(__WXPM__)
#include "wx/os2/joystick.h"
#endif

#endif // wxUSE_JOYSTICK

#endif
    // _WX_JOYSTICK_H_BASE_
