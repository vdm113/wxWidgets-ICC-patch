/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/busyinfo.h
// Purpose:     Information window (when app is busy)
// Author:      Vaclav Slavik
// Copyright:   (c) 1999 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_BUSYINFO_H_
#define _WX_BUSYINFO_H_

#include "wx/defs.h"

#if wxUSE_BUSYINFO

#include "wx/object.h"

class WXDLLIMPEXP_FWD_CORE wxFrame;
class WXDLLIMPEXP_FWD_CORE wxWindow;

//--------------------------------------------------------------------------------
// wxBusyInfo
//                  Displays progress information
//                  Can be used in exactly same way as wxBusyCursor
//--------------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxBusyInfo : public wxObject
{
public:
    wxBusyInfo(const wxBusyInfoFlags& flags)
    {
        Init(flags);
    }

    wxBusyInfo(const wxString& message, wxWindow *parent = NULL)
    {
        Init(wxBusyInfoFlags().Parent(parent).Label(message));
    }

    virtual ~wxBusyInfo();

private:
    void Init(const wxBusyInfoFlags& flags);

    wxFrame *m_InfoFrame;

    wxDECLARE_NO_COPY_CLASS(wxBusyInfo);
};

#endif // wxUSE_BUSYINFO
#endif // _WX_BUSYINFO_H_
