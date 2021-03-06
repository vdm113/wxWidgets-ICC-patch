/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        src/stc/stc_i18n.cpp
// Purpose:     Dummy file containing translatable strings from Scintilla
// Author:      Vadim Zeitlin
// Created:     2010-06-09
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

