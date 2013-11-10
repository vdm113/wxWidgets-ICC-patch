/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        main.h
// Purpose:     Main page of the Doxygen manual
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**

@mainpage Documentation

@author Julian Smart, Vadim Zeitlin, Robin Dunn, Stefan Csomor,
        Bryan Petty, Francesco Montorsi, Robert Roebling et al

@date October, 2013

@image html main_wxlogo.png

@section manual_user User Manual

@li @subpage page_introduction
@li @subpage page_copyright
@li @subpage page_utils
@li @subpage page_samples
@li @subpage page_libs
@li @subpage page_class_cat
@li @subpage page_topics
@li @subpage page_multiplatform
@li @subpage page_port

@section manual_reference Reference Manual

@li @subpage page_constants
@li @subpage page_translations
@li @subpage group_class
@li @subpage group_funcmacro

*/
