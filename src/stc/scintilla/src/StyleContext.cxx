/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif defined(__INTEL_COMPILER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

// Scintilla source code edit control

/** @file StyleContext.cxx
 ** Lexer infrastructure.
 **/
// Copyright 1998-2004 by Neil Hodgson <neilh@scintilla.org>
// This file is in the public domain.

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "Platform.h"

#include "PropSet.h"
#include "Accessor.h"
#include "StyleContext.h"

#ifdef SCI_NAMESPACE
using namespace Scintilla;
#endif

static void getRange(unsigned int start,
		unsigned int end,
		Accessor &styler,
		char *s,
		unsigned int len) {
	unsigned int i = 0;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	while ((i < end - start + 1) && (i < len-1)) {
		s[i] = styler[start + i];
		i++;
	}
	s[i] = '\0';
}

void StyleContext::GetCurrent(char *s, unsigned int len) {
	getRange(styler.GetStartSegment(), currentPos - 1, styler, s, len);
}

static void getRangeLowered(unsigned int start,
		unsigned int end,
		Accessor &styler,
		char *s,
		unsigned int len) {
	unsigned int i = 0;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	while ((i < end - start + 1) && (i < len-1)) {
		s[i] = static_cast<char>(tolower(styler[start + i]));
		i++;
	}
	s[i] = '\0';
}

void StyleContext::GetCurrentLowered(char *s, unsigned int len) {
	getRangeLowered(styler.GetStartSegment(), currentPos - 1, styler, s, len);
}
