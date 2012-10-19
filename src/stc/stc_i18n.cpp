/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/* VDM: BEGIN */
#if defined(_MSC_VER) && defined(MY_MSC_NO_ITERATOR_DEBUGGING) && MY_MSC_NO_ITERATOR_DEBUGGING==1
#   if defined(_SECURE_SCL)
#       undef _SECURE_SCL
#   endif
#   define _SECURE_SCL 0

#   if defined(_HAS_ITERATOR_DEBUGGING)
#       undef _HAS_ITERATOR_DEBUGGING
#   endif
#   define _HAS_ITERATOR_DEBUGGING 0
#endif
/* VDM: END   */

///////////////////////////////////////////////////////////////////////////////
// Name:        src/stc/stc_i18n.cpp
// Purpose:     Dummy file containing translatable strings from Scintilla
// Author:      Vadim Zeitlin
// Created:     2010-06-09
// RCS-ID:      $Id: $
// Copyright:   (c) 2010 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// This is just a collection of translatable strings defined in Scintilla
// sources. It is used by locale/Makefile only and not supposed to be compiled.
#error "This file is not supposed to be compiled."

// These strings were manually extracted from ScintillaBase::ContextMenu().
_("Undo")
_("Redo")
_("Cut")
_("Copy")
_("Paste")
_("Delete")
_("Select All")

