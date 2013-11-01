/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/checkeddelete.h
// Purpose:     wxCHECKED_DELETE() macro
// Author:      Vadim Zeitlin
// Created:     2009-02-03
// Copyright:   (c) 2002-2009 wxWidgets dev team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CHECKEDDELETE_H_
#define _WX_CHECKEDDELETE_H_

#include "wx/cpp.h"

// TODO: provide wxCheckedDelete[Array]() template functions too

// ----------------------------------------------------------------------------
// wxCHECKED_DELETE and wxCHECKED_DELETE_ARRAY macros
// ----------------------------------------------------------------------------

/*
   checked deleters are used to make sure that the type being deleted is really
   a complete type.: otherwise sizeof() would result in a compile-time error

#if defined(__INTEL_COMPILER) // VDM auto patch
#   pragma ivdep
#endif
   do { ... } while ( 0 ) construct is used to have an anonymous scope
   (otherwise we could have name clashes between different "complete"s) but
   still force a semicolon after the macro
*/

#define wxCHECKED_DELETE(ptr)                                                 \
    wxSTATEMENT_MACRO_BEGIN                                                   \
        typedef char complete[sizeof(*ptr)] WX_ATTRIBUTE_UNUSED;              \
        delete ptr;                                                           \
    wxSTATEMENT_MACRO_END

#define wxCHECKED_DELETE_ARRAY(ptr)                                           \
    wxSTATEMENT_MACRO_BEGIN                                                   \
        typedef char complete[sizeof(*ptr)] WX_ATTRIBUTE_UNUSED;              \
        delete [] ptr;                                                        \
    wxSTATEMENT_MACRO_END


#endif // _WX_CHECKEDDELETE_H_

