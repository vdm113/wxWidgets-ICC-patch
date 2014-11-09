/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/utils.h
// Purpose:     utility classes and/or functions
// Author:      Peter Most, Javier Torres
// Created:     15/05/10
// Copyright:   (c) Peter Most, Javier Torres
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_UTILS_H_
#define _WX_QT_UTILS_H_

#include "wx/mousestate.h"
#include <QtCore/Qt>

void wxQtFillMouseButtons( Qt::MouseButtons buttons, wxMouseState *state );

void wxMissingImplementation( const char fileName[], unsigned lineNumber,
    const char feature[] );

#define wxMISSING_IMPLEMENTATION( feature )\
    wxMissingImplementation( __FILE__, __LINE__, feature )

#define wxMISSING_FUNCTION() \
    wxMISSING_IMPLEMENTATION( __WXFUNCTION__ )

// global function handle Qt objects destruction (just for debugging now)
void wxQtHandleDestroyedSignal(QObject *qobj = 0);

#endif // _WX_QT_UTILS_H_
