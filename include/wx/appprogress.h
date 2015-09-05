/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/appprogress.h
// Purpose:     wxAppProgressIndicator interface.
// Author:      Chaobin Zhang <zhchbin@gmail.com>
// Created:     2014-09-05
// Copyright:   (c) 2014 wxWidgets development team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_APPPROG_H_
#define _WX_APPPROG_H_

#include "wx/defs.h"

class WXDLLIMPEXP_CORE wxAppProgressIndicatorBase
{
public:
    wxAppProgressIndicatorBase() {}
    virtual ~wxAppProgressIndicatorBase() {}

    virtual bool IsAvailable() const = 0;

    virtual void SetValue(int value) = 0;
    virtual void SetRange(int range) = 0;
    virtual void Pulse() = 0;
    virtual void Reset() = 0;

private:
    wxDECLARE_NO_COPY_CLASS(wxAppProgressIndicatorBase);
};

#if defined(__WXMSW__) && wxUSE_TASKBARBUTTON
    #include "wx/msw/appprogress.h"
#elif defined(__WXOSX_COCOA__)
    #include "wx/osx/appprogress.h"
#else
    class wxAppProgressIndicator : public wxAppProgressIndicatorBase
    {
    public:
        wxAppProgressIndicator(wxWindow* WXUNUSED(parent) = NULL,
                               int WXUNUSED(maxValue) = 100)
        {
        }

        virtual bool IsAvailable() const wxOVERRIDE { return false; }

        virtual void SetValue(int WXUNUSED(value)) wxOVERRIDE { }
        virtual void SetRange(int WXUNUSED(range)) wxOVERRIDE { }
        virtual void Pulse() wxOVERRIDE { }
        virtual void Reset() wxOVERRIDE { }
    };
#endif

#endif  // _WX_APPPROG_H_
