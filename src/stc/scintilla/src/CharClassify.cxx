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
/** @file CharClassify.cxx
 ** Character classifications used by Document and RESearch.
 **/
// Copyright 2006 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <ctype.h>

#include "CharClassify.h"

#ifdef SCI_NAMESPACE
using namespace Scintilla;
#endif

// Shut up annoying Visual C++ warnings:
#ifdef _MSC_VER
#pragma warning(disable: 4514)
#endif

CharClassify::CharClassify() {
	SetDefaultCharClasses(true);
}

void CharClassify::SetDefaultCharClasses(bool includeWordClass) {
	// Initialize all char classes to default values
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	for (int ch = 0; ch < 256; ch++) {
		if (ch == '\r' || ch == '\n')
			charClass[ch] = ccNewLine;
		else if (ch < 0x20 || ch == ' ')
			charClass[ch] = ccSpace;
		else if (includeWordClass && (ch >= 0x80 || isalnum(ch) || ch == '_'))
			charClass[ch] = ccWord;
		else
			charClass[ch] = ccPunctuation;
	}
}

void CharClassify::SetCharClasses(const unsigned char *chars, cc newCharClass) {
	// Apply the newCharClass to the specifed chars
	if (chars) {
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
		while (*chars) {
			charClass[*chars] = static_cast<unsigned char>(newCharClass);
			chars++;
		}
	}
}

int CharClassify::GetCharsOfClass(cc characterClass, unsigned char *buffer) {
	// Get characters belonging to the given char class; return the number
	// of characters (if the buffer is NULL, don't write to it).
	int count = 0;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	for (int ch = maxChar - 1; ch >= 0; --ch) {
		if (charClass[ch] == characterClass) {
			++count;
			if (buffer) {
				*buffer = static_cast<unsigned char>(ch);
				buffer++;
			}
		}
	}
	return count;
}
