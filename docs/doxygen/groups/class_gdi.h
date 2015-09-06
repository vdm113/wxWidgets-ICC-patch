/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        class_gdi.h
// Purpose:     Graphics Device Interface classes group docs
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@defgroup group_class_gdi Graphics Device Interface (GDI)
@ingroup group_class

The following are classes related to GDI (Graphics Device Interface) access.
They provide an API for drawing on device contexts, windows, and printing.

Related Overviews: @ref overview_dc, @ref overview_bitmap

Related macros/global-functions group: @ref group_funcmacro_gdi


*/

