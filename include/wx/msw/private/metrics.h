/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/private/metrics.h
// Purpose:     various helper functions to retrieve system metrics
// Author:      Vadim Zeitlin
// Created:     2008-09-05
// Copyright:   (c) 2008 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MSW_PRIVATE_METRICS_H_
#define _WX_MSW_PRIVATE_METRICS_H_

namespace wxMSWImpl
{

// return NONCLIENTMETRICS as retrieved by SystemParametersInfo()
//
// currently this is not cached as the values may change when system settings
// do and we don't react to this to invalidate the cache but it could be done
// in the future
//
// MT-safety: this function is only meant to be called from the main thread
inline const NONCLIENTMETRICS& GetNonClientMetrics()
{
    static WinStruct<NONCLIENTMETRICS> nm;
    if ( !::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &nm, 0) )
    {
#if WINVER >= 0x0600
        // a new field has been added to NONCLIENTMETRICS under Vista, so
        // the call to SystemParametersInfo() fails if we use the struct
        // size incorporating this new value on an older system -- retry
        // without it
        nm.cbSize -= sizeof(int);
        if ( !::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &nm, 0) )
#endif // WINVER >= 0x0600
        {
            // maybe we should initialize the struct with some defaults?
            wxLogLastError(wxT("SystemParametersInfo(SPI_GETNONCLIENTMETRICS)"));
        }
    }

    return nm;
}

} // namespace wxMSWImpl

#endif // _WX_MSW_PRIVATE_METRICS_H_

