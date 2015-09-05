/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

// Scintilla source code edit control
/** @file KeyWords.cxx
 ** Colourise for particular languages.
 **/
// Copyright 1998-2002 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

#include <algorithm>

#include "StringCopy.h"
#include "WordList.h"

#ifdef SCI_NAMESPACE
using namespace Scintilla;
#endif

/**
 * Creates an array that points into each word in the string and puts \0 terminators
 * after each word.
 */
static char **ArrayFromWordList(char *wordlist, int *len, bool onlyLineEnds = false) {
	int prev = '\n';
	int words = 0;
	// For rapid determination of whether a character is a separator, build
	// a look up table.
	bool wordSeparator[256];
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
	for (int i=0; i<256; i++) {
		wordSeparator[i] = false;
	}
	wordSeparator[static_cast<unsigned int>('\r')] = true;
	wordSeparator[static_cast<unsigned int>('\n')] = true;
	if (!onlyLineEnds) {
		wordSeparator[static_cast<unsigned int>(' ')] = true;
		wordSeparator[static_cast<unsigned int>('\t')] = true;
	}
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
	for (int j = 0; wordlist[j]; j++) {
		int curr = static_cast<unsigned char>(wordlist[j]);
		if (!wordSeparator[curr] && wordSeparator[prev])
			words++;
		prev = curr;
	}
	char **keywords = new char *[words + 1];
	int wordsStore = 0;
	const size_t slen = strlen(wordlist);
	if (words) {
		prev = '\0';
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
		for (size_t k = 0; k < slen; k++) {
			if (!wordSeparator[static_cast<unsigned char>(wordlist[k])]) {
				if (!prev) {
					keywords[wordsStore] = &wordlist[k];
					wordsStore++;
				}
			} else {
				wordlist[k] = '\0';
			}
			prev = wordlist[k];
		}
	}
	keywords[wordsStore] = &wordlist[slen];
	*len = wordsStore;
	return keywords;
}

WordList::WordList(bool onlyLineEnds_) :
	words(0), list(0), len(0), onlyLineEnds(onlyLineEnds_) {
	// Prevent warnings by static analyzers about uninitialized starts.
	starts[0] = -1;
}

WordList::~WordList() {
	Clear();
}

WordList::operator bool() const {
	return len ? true : false;
}

bool WordList::operator!=(const WordList &other) const {
	if (len != other.len)
		return true;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
	for (int i=0; i<len; i++) {
		if (strcmp(words[i], other.words[i]) != 0)
			return true;
	}
	return false;
}

int WordList::Length() const {
	return len;
}

void WordList::Clear() {
	if (words) {
		delete []list;
		delete []words;
	}
	words = 0;
	list = 0;
	len = 0;
}

#ifdef _MSC_VER

static bool cmpWords(const char *a, const char *b) {
	return strcmp(a, b) < 0;
}

#else

static int cmpWords(const void *a, const void *b) {
	return strcmp(*static_cast<const char * const *>(a), *static_cast<const char * const *>(b));
}

static void SortWordList(char **words, unsigned int len) {
	qsort(reinterpret_cast<void *>(words), len, sizeof(*words), cmpWords);
}

#endif

void WordList::Set(const char *s) {
	Clear();
	const size_t lenS = strlen(s) + 1;
	list = new char[lenS];
	memcpy(list, s, lenS);
	words = ArrayFromWordList(list, &len, onlyLineEnds);
#ifdef _MSC_VER
	std::sort(words, words + len, cmpWords);
#else
	SortWordList(words, len);
#endif
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
	for (unsigned int k = 0; k < ELEMENTS(starts); k++)
		starts[k] = -1;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
	for (int l = len - 1; l >= 0; l--) {
		unsigned char indexChar = words[l][0];
		starts[indexChar] = l;
	}
}

/** Check whether a string is in the list.
 * List elements are either exact matches or prefixes.
 * Prefix elements start with '^' and match all strings that start with the rest of the element
 * so '^GTK_' matches 'GTK_X', 'GTK_MAJOR_VERSION', and 'GTK_'.
 */
bool WordList::InList(const char *s) const {
	if (0 == words)
		return false;
	unsigned char firstChar = s[0];
	int j = starts[firstChar];
	if (j >= 0) {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
		while (static_cast<unsigned char>(words[j][0]) == firstChar) {
			if (s[1] == words[j][1]) {
				const char *a = words[j] + 1;
				const char *b = s + 1;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
				while (*a && *a == *b) {
					a++;
					b++;
				}
				if (!*a && !*b)
					return true;
			}
			j++;
		}
	}
	j = starts[static_cast<unsigned int>('^')];
	if (j >= 0) {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
		while (words[j][0] == '^') {
			const char *a = words[j] + 1;
			const char *b = s;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
			while (*a && *a == *b) {
				a++;
				b++;
			}
			if (!*a)
				return true;
			j++;
		}
	}
	return false;
}

/** similar to InList, but word s can be a substring of keyword.
 * eg. the keyword define is defined as def~ine. This means the word must start
 * with def to be a keyword, but also defi, defin and define are valid.
 * The marker is ~ in this case.
 */
bool WordList::InListAbbreviated(const char *s, const char marker) const {
	if (0 == words)
		return false;
	unsigned char firstChar = s[0];
	int j = starts[firstChar];
	if (j >= 0) {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
		while (static_cast<unsigned char>(words[j][0]) == firstChar) {
			bool isSubword = false;
			int start = 1;
			if (words[j][1] == marker) {
				isSubword = true;
				start++;
			}
			if (s[1] == words[j][start]) {
				const char *a = words[j] + start;
				const char *b = s + 1;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
				while (*a && *a == *b) {
					a++;
					if (*a == marker) {
						isSubword = true;
						a++;
					}
					b++;
				}
				if ((!*a || isSubword) && !*b)
					return true;
			}
			j++;
		}
	}
	j = starts[static_cast<unsigned int>('^')];
	if (j >= 0) {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
		while (words[j][0] == '^') {
			const char *a = words[j] + 1;
			const char *b = s;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
			while (*a && *a == *b) {
				a++;
				b++;
			}
			if (!*a)
				return true;
			j++;
		}
	}
	return false;
}

const char *WordList::WordAt(int n) const {
	return words[n];
}

