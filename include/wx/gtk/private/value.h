/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/private/value.h
// Purpose:     Helper wrapper for working with GValue.
// Author:      Vadim Zeitlin
// Created:     2015-03-05
// Copyright:   (c) 2015 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GTK_PRIVATE_VALUE_H_
#define _WX_GTK_PRIVATE_VALUE_H_

// ----------------------------------------------------------------------------
// wxGtkValue: RAII wrapper for GValue
// ----------------------------------------------------------------------------

class wxGtkValue
{
public:
    // Initialize the value of the specified type.
    explicit wxGtkValue(GType gtype)
        : m_val(G_VALUE_INIT)
    {
        g_value_init(&m_val, gtype);
    }

    ~wxGtkValue()
    {
        g_value_unset(&m_val);
    }

    // Unsafe but convenient access to the real value for GTK+ functions.
    operator GValue*() { return &m_val; }

private:
    GValue m_val;

    // For now we just don't support copying at all for simplicity, it could be
    // implemented later if needed.
    wxDECLARE_NO_COPY_CLASS(wxGtkValue);
};

#endif // _WX_GTK_PRIVATE_VALUE_H_
