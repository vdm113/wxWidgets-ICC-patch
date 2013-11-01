/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wxWidgets_vdm_patch/stdafx.cpp
// Purpose:     patcher for ICL compiler
// Author:      Marian 'VooDooMan' Meravy
// RCS-ID:      $Id$
// Copyright:   (c) Marian 'VooDooMan' Meravy
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#pragma once

// Including SDKDDKVer.h defines the highest available Windows platform.

// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.

#include <SDKDDKVer.h>
