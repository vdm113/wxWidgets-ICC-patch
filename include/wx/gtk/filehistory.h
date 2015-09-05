/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/filehistory.h
// Purpose:     GTK+ bits for wxFileHistory
// Author:      Vaclav Slavik
// Created:     2010-05-06
// Copyright:   (c) 2010 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GTK_FILEHISTORY_H_
#define _WX_GTK_FILEHISTORY_H_

class WXDLLIMPEXP_CORE wxFileHistory : public wxFileHistoryBase
{
public:
    wxFileHistory(size_t maxFiles = 9, wxWindowID idBase = wxID_FILE1)
        : wxFileHistoryBase(maxFiles, idBase) {}

    virtual void AddFileToHistory(const wxString& file) wxOVERRIDE;

    wxDECLARE_DYNAMIC_CLASS(wxFileHistory);
};

#endif // _WX_GTK_FILEHISTORY_H_
