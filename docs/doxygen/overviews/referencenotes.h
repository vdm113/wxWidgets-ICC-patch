/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        referencenotes.h
// Purpose:     topic overview
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@page overview_referencenotes Notes on Using this Reference Manual

@tableofcontents

In the descriptions of the wxWidgets classes and their member functions, note
that descriptions of inherited member functions are not duplicated in derived
classes unless their behaviour is different. So in using a class such as
wxScrolledWindow, be aware that wxWindow functions may be relevant.

Where not explicitly stated, size and position arguments may usually be given a
value of ::wxDefaultSize and ::wxDefaultPosition, in which case wxWidgets will
choose suitable values.

*/
