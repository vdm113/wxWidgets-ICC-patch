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

#include <string>

#include "ILexer.h"
#include "Scintilla.h"
#include "SciLexer.h"

#include "PropSetSimple.h"
#include "WordList.h"
#include "LexAccessor.h"
#include "Accessor.h"
#include "LexerModule.h"
#include "LexerBase.h"
#include "LexerSimple.h"

#ifdef SCI_NAMESPACE
using namespace Scintilla;
#endif

LexerSimple::LexerSimple(const LexerModule *module_) : module(module_) {
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	for (int wl = 0; wl < module->GetNumWordLists(); wl++) {
		if (!wordLists.empty())
			wordLists += "\n";
		wordLists += module->GetWordListDescription(wl);
	}
}

const char * SCI_METHOD LexerSimple::DescribeWordListSets() {
	return wordLists.c_str();
}

void SCI_METHOD LexerSimple::Lex(unsigned int startPos, int lengthDoc, int initStyle, IDocument *pAccess) {
	Accessor astyler(pAccess, &props);
	module->Lex(startPos, lengthDoc, initStyle, keyWordLists, astyler);
	astyler.Flush();
}

void SCI_METHOD LexerSimple::Fold(unsigned int startPos, int lengthDoc, int initStyle, IDocument *pAccess) {
	if (props.GetInt("fold")) {
		Accessor astyler(pAccess, &props);
		module->Fold(startPos, lengthDoc, initStyle, keyWordLists, astyler);
		astyler.Flush();
	}
}
