/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/time.h
// Purpose:     Time-related functions.
// Author:      Vadim Zeitlin
// Created:     2011-11-27
// Copyright:   (c) 2011 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/** @addtogroup group_funcmacro_time */
//@{

/**
    Returns the difference between UTC and local time in seconds.

    @header{wx/time.h}
 */
int wxGetTimeZone();

/**
    Returns the number of seconds since local time 00:00:00 Jan 1st 1970.

    @see wxDateTime::Now()

    @header{wx/time.h}
*/
long wxGetLocalTime();

/**
    Returns the number of milliseconds since local time 00:00:00 Jan 1st 1970.

    The use of wxGetUTCTimeMillis() is preferred as it provides a usually
    (except for changes to the system time) monotonic clock which the local
    time also changes whenever DST begins or ends.

    @see wxDateTime::Now(), wxGetUTCTimeMillis(), wxGetUTCTimeUSec()

    @header{wx/time.h}
*/
wxLongLong wxGetLocalTimeMillis();

/**
    Returns the number of seconds since GMT 00:00:00 Jan 1st 1970.

    @see wxDateTime::Now()

    @header{wx/time.h}
*/
long wxGetUTCTime();

/**
    Returns the number of milliseconds since GMT 00:00:00 Jan 1st 1970.

    @header{wx/time.h}

    @since 2.9.3
 */
wxLongLong wxGetUTCTimeMillis();

/**
    Returns the number of microseconds since GMT 00:00:00 Jan 1st 1970.

    @header{wx/time.h}

    @since 2.9.3
 */
wxLongLong wxGetUTCTimeUSec();

//@}
