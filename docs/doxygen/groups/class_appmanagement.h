/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/////////////////////////////////////////////////////////////////////////////
// Name:        class_appmanagement.h
// Purpose:     Application and Process Management classes group docs
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@defgroup group_class_appmanagement Application and Process Management
@ingroup group_class

The classes in this section represent the application (see wxApp) or parts of it
(e.g. wxEventLoopBase, wxModule).
They can be used for initialization/shutdown of the application itself.

Related macros/global-functions group: @ref group_funcmacro_appinitterm.

*/

