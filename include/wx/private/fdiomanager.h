/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/private/fdiomanager.h
// Purpose:     declaration of wxFDIOManager
// Author:      Vadim Zeitlin
// Created:     2009-08-17
// Copyright:   (c) 2009 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PRIVATE_FDIOMANAGER_H_
#define _WX_PRIVATE_FDIOMANAGER_H_

#include "wx/private/fdiohandler.h"

// ----------------------------------------------------------------------------
// wxFDIOManager: register or unregister wxFDIOHandlers
// ----------------------------------------------------------------------------

// currently only used in wxGTK and wxMotif, see wx/unix/apptrait.h

class wxFDIOManager
{
public:
    // identifies either input or output direction
    //
    // NB: the values of this enum shouldn't change
    enum Direction
    {
        INPUT,
        OUTPUT
    };

    // start or stop monitoring the events on the given file descriptor
    virtual int AddInput(wxFDIOHandler *handler, int fd, Direction d) = 0;
    virtual void RemoveInput(wxFDIOHandler *handler, int fd, Direction d) = 0;

    // empty but virtual dtor for the base class
    virtual ~wxFDIOManager() { }
};

#endif // _WX_PRIVATE_FDIOMANAGER_H_

