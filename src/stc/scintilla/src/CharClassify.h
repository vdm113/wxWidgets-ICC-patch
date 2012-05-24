/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

// Scintilla source code edit control
/** @file CharClassify.h
 ** Character classifications used by Document and RESearch.
 **/
// Copyright 2006-2009 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef CHARCLASSIFY_H
#define CHARCLASSIFY_H

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif

class CharClassify {
public:
	CharClassify();

	enum cc { ccSpace, ccNewLine, ccWord, ccPunctuation };
	void SetDefaultCharClasses(bool includeWordClass);
	void SetCharClasses(const unsigned char *chars, cc newCharClass);
	int GetCharsOfClass(cc charClass, unsigned char *buffer);
	cc GetClass(unsigned char ch) const { return static_cast<cc>(charClass[ch]);}
	bool IsWord(unsigned char ch) const { return static_cast<cc>(charClass[ch]) == ccWord;}

private:
	enum { maxChar=256 };
	unsigned char charClass[maxChar];    // not type cc to save space
};

#ifdef SCI_NAMESPACE
}
#endif

#endif
