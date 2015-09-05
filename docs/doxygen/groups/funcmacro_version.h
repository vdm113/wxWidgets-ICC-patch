/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        funcmacro_version.h
// Purpose:     Versioning function and macro group docs
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@defgroup group_funcmacro_version Versioning
@ingroup group_funcmacro

The following constants are defined in wxWidgets:

@beginDefList
@itemdef{ wxMAJOR_VERSION, The major version of wxWidgets }
@itemdef{ wxMINOR_VERSION, The minor version of wxWidgets }
@itemdef{ wxRELEASE_NUMBER, The release number }
@itemdef{ wxSUBRELEASE_NUMBER, The subrelease number which is 0 for all
    official releases }
@endDefList

For example, the values or these constants for wxWidgets 2.8.7
are 2, 8, 7 and 0.

Additionally, wxVERSION_STRING is a user-readable string containing the full
wxWidgets version and wxVERSION_NUMBER is a combination of the three version
numbers above: for 2.1.15, it is 2115 and it is 2200 for wxWidgets 2.2.

The subrelease number is only used for the sources in between official releases
and so normally is not useful.

@header{wx/version.h}

*/

