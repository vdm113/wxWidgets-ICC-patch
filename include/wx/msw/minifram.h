/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif defined(__INTEL_COMPILER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////

// Name:        wx/msw/minifram.h
// Purpose:     wxMiniFrame class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MINIFRAM_H_
#define _WX_MINIFRAM_H_

#include "wx/frame.h"

class WXDLLIMPEXP_CORE wxMiniFrame : public wxFrame
{
public:
  wxMiniFrame() { }

  bool Create(wxWindow *parent,
              wxWindowID id,
              const wxString& title,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style = wxCAPTION | wxCLIP_CHILDREN | wxRESIZE_BORDER,
              const wxString& name = wxFrameNameStr)
  {
      return wxFrame::Create(parent, id, title, pos, size,
                             style |
                             wxFRAME_TOOL_WINDOW |
                             (parent ? wxFRAME_FLOAT_ON_PARENT : 0), name);
  }

  wxMiniFrame(wxWindow *parent,
              wxWindowID id,
              const wxString& title,
              const wxPoint& pos = wxDefaultPosition,
              const wxSize& size = wxDefaultSize,
              long style = wxCAPTION | wxCLIP_CHILDREN | wxRESIZE_BORDER,
              const wxString& name = wxFrameNameStr)
  {
      Create(parent, id, title, pos, size, style, name);
  }

protected:
  DECLARE_DYNAMIC_CLASS_NO_COPY(wxMiniFrame)
};

#endif
    // _WX_MINIFRAM_H_
