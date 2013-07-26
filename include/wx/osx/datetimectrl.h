/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/osx/datetimectrl.h
// Purpose:     Declaration of wxOSX-specific wxDateTimePickerCtrl class.
// Author:      Vadim Zeitlin
// Created:     2011-12-18
// Copyright:   (c) 2011 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_OSX_DATETIMECTRL_H_
#define _WX_OSX_DATETIMECTRL_H_

class wxDateTimeWidgetImpl;

// ----------------------------------------------------------------------------
// wxDateTimePickerCtrl
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_ADV wxDateTimePickerCtrl : public wxDateTimePickerCtrlBase
{
public:
    // Implement the base class pure virtuals.
    virtual void SetValue(const wxDateTime& dt);
    virtual wxDateTime GetValue() const;

    // Implementation only.
    virtual void OSXGenerateEvent(const wxDateTime& dt) = 0;

protected:
    wxDateTimeWidgetImpl* GetDateTimePeer() const;
};

#endif // _WX_OSX_DATETIMECTRL_H_
