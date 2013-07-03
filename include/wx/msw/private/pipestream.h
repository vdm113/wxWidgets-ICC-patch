/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/private/pipestream.h
// Purpose:     MSW wxPipeInputStream and wxPipeOutputStream declarations
// Author:      Vadim Zeitlin
// Created:     2013-06-08 (extracted from src/msw/utilsexc.cpp)
// RCS-ID:      $Id$
// Copyright:   (c) 2013 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MSW_PRIVATE_PIPESTREAM_H_
#define _WX_MSW_PRIVATE_PIPESTREAM_H_

class wxPipeInputStream : public wxInputStream
{
public:
    wxEXPLICIT wxPipeInputStream(HANDLE hInput);
    virtual ~wxPipeInputStream();

    // returns true if the pipe is still opened
    bool IsOpened() const { return m_hInput != INVALID_HANDLE_VALUE; }

    // returns true if there is any data to be read from the pipe
    virtual bool CanRead() const;

protected:
    virtual size_t OnSysRead(void *buffer, size_t len);

protected:
    HANDLE m_hInput;

    wxDECLARE_NO_COPY_CLASS(wxPipeInputStream);
};

class wxPipeOutputStream: public wxOutputStream
{
public:
    wxEXPLICIT wxPipeOutputStream(HANDLE hOutput);
    virtual ~wxPipeOutputStream() { Close(); }
    bool Close();

protected:
    size_t OnSysWrite(const void *buffer, size_t len);

protected:
    HANDLE m_hOutput;

    wxDECLARE_NO_COPY_CLASS(wxPipeOutputStream);
};

#endif // _WX_MSW_PRIVATE_PIPESTREAM_H_
