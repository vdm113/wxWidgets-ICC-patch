/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/meta/convertible.h
// Purpose:     Test if types are convertible
// Author:      Arne Steinarson
// Created:     2008-01-10
// Copyright:   (c) 2008 Arne Steinarson
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_META_CONVERTIBLE_H_
#define _WX_META_CONVERTIBLE_H_

//
// Introduce an extra class to make this header compilable with g++3.2
//
template <class D, class B>
struct wxConvertibleTo_SizeHelper
{
    static char Match(B* pb);
    static int  Match(...);
};

// Helper to decide if an object of type D is convertible to type B (the test
// succeeds in particular when D derives from B)
template <class D, class B>
struct wxConvertibleTo
{
    enum
    {
        value =
            sizeof(wxConvertibleTo_SizeHelper<D,B>::Match(static_cast<D*>(NULL)))
            ==
            sizeof(char)
    };
};

#endif // _WX_META_CONVERTIBLE_H_

