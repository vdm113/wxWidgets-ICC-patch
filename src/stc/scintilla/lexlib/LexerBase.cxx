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

// Scintilla source code edit control
/** @file LexerSimple.cxx
 ** A simple lexer with no state.
 **/
// Copyright 1998-2010 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <ctype.h>

#include "ILexer.h"
#include "Scintilla.h"
#include "SciLexer.h"

#include "PropSetSimple.h"
#include "WordList.h"
#include "LexAccessor.h"
#include "Accessor.h"
#include "LexerModule.h"
#include "LexerBase.h"

#ifdef SCI_NAMESPACE
using namespace Scintilla;
#endif

LexerBase::LexerBase() {
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	for (int wl = 0; wl < numWordLists; wl++)
		keyWordLists[wl] = new WordList;
	keyWordLists[numWordLists] = 0;
}

LexerBase::~LexerBase() {
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	for (int wl = 0; wl < numWordLists; wl++) {
		delete keyWordLists[wl];
		keyWordLists[wl] = 0;
	}
	keyWordLists[numWordLists] = 0;
}

void SCI_METHOD LexerBase::Release() {
	delete this;
}

int SCI_METHOD LexerBase::Version() const {
	return lvOriginal;
}

const char * SCI_METHOD LexerBase::PropertyNames() {
	return "";
}

int SCI_METHOD LexerBase::PropertyType(const char *) {
	return SC_TYPE_BOOLEAN;
}

const char * SCI_METHOD LexerBase::DescribeProperty(const char *) {
	return "";
}

int SCI_METHOD LexerBase::PropertySet(const char *key, const char *val) {
	const char *valOld = props.Get(key);
	if (strcmp(val, valOld) != 0) {
		props.Set(key, val);
		return 0;
	} else {
		return -1;
	}
}

const char * SCI_METHOD LexerBase::DescribeWordListSets() {
	return "";
}

int SCI_METHOD LexerBase::WordListSet(int n, const char *wl) {
	if (n < numWordLists) {
		WordList wlNew;
		wlNew.Set(wl);
		if (*keyWordLists[n] != wlNew) {
			keyWordLists[n]->Set(wl);
			return 0;
		}
	}
	return -1;
}

void * SCI_METHOD LexerBase::PrivateCall(int, void *) {
	return 0;
}
