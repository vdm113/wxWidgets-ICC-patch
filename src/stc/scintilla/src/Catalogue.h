/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

// Scintilla source code edit control
/** @file Catalogue.h
 ** Lexer infrastructure.
 **/
// Copyright 1998-2010 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef CATALOGUE_H
#define CATALOGUE_H

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif

class Catalogue {
public:
	static const LexerModule *Find(int language);
	static const LexerModule *Find(const char *languageName);
	static void AddLexerModule(LexerModule *plm);
};

#ifdef SCI_NAMESPACE
}
#endif

#endif
