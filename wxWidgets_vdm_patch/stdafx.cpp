/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wxWidgets_vdm_patch/stdafx.cpp
// Purpose:     patcher for ICL compiler
// Author:      Marian 'VooDooMan' Meravy
// RCS-ID:      $Id$
// Copyright:   (c) Marian 'VooDooMan' Meravy
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// stdafx.cpp : source file that includes just the standard includes
// wxWidgets_vdm_patch.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
