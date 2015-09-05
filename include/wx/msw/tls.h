/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/tls.h
// Purpose:     Win32 implementation of wxTlsValue<>
// Author:      Vadim Zeitlin
// Created:     2008-08-08
// Copyright:   (c) 2008 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MSW_TLS_H_
#define _WX_MSW_TLS_H_

#include "wx/msw/wrapwin.h"
#include "wx/thread.h"
#include "wx/vector.h"

// ----------------------------------------------------------------------------
// wxTlsKey is a helper class encapsulating a TLS slot
// ----------------------------------------------------------------------------

class wxTlsKey
{
public:
    // ctor allocates a new key
    wxTlsKey(wxTlsDestructorFunction destructor)
    {
        m_destructor = destructor;
        m_slot = ::TlsAlloc();
    }

    // return true if the key was successfully allocated
    bool IsOk() const { return m_slot != TLS_OUT_OF_INDEXES; }

    // get the key value, there is no error return
    void *Get() const
    {
        return ::TlsGetValue(m_slot);
    }

    // change the key value, return true if ok
    bool Set(void *value)
    {
        void *old = Get();

        if ( ::TlsSetValue(m_slot, value) == 0 )
            return false;

        if ( old )
            m_destructor(old);

        // update m_allValues list of all values - remove old, add new
        wxCriticalSectionLocker lock(m_csAllValues);
        if ( old )
        {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            for ( wxVector<void*>::iterator i = m_allValues.begin();
                  i != m_allValues.end();
                  ++i )
            {
                if ( *i == old )
                {
                    if ( value )
                        *i = value;
                    else
                        m_allValues.erase(i);
                    return true;
                }
            }
            wxFAIL_MSG( "previous wxTlsKey value not recorded in m_allValues" );
        }

        if ( value )
            m_allValues.push_back(value);

        return true;
    }

    // free the key
    ~wxTlsKey()
    {
        if ( !IsOk() )
            return;

        // Win32 API doesn't have the equivalent of pthread's destructor, so we
        // have to keep track of all allocated values and destroy them manually;
        // ideally we'd do that at thread exit time, but since we could only
        // do that with wxThread and not otherwise created threads, we do it
        // here.
        //
        // TODO: We should still call destructors for wxTlsKey used in the
        //       thread from wxThread's thread shutdown code, *in addition*
        //       to doing it in ~wxTlsKey.
        //
        // NB: No need to lock m_csAllValues, by the time this code is called,
        //     no other thread can be using this key.
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
        for ( wxVector<void*>::iterator i = m_allValues.begin();
              i != m_allValues.end();
              ++i )
        {
            m_destructor(*i);
        }

        ::TlsFree(m_slot);
    }

private:
    wxTlsDestructorFunction m_destructor;
    DWORD m_slot;

    wxVector<void*> m_allValues;
    wxCriticalSection m_csAllValues;

    wxDECLARE_NO_COPY_CLASS(wxTlsKey);
};

#endif // _WX_MSW_TLS_H_

