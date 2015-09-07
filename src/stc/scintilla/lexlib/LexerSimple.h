/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

// Scintilla source code edit control
/** @file LexerSimple.h
 ** A simple lexer with no state.
 **/
// Copyright 1998-2010 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef LEXERSIMPLE_H
#define LEXERSIMPLE_H

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif

// A simple lexer with no state
class LexerSimple : public LexerBase {
	const LexerModule *module;
	std::string wordLists;
public:
	explicit LexerSimple(const LexerModule *module_);
	const char * SCI_METHOD DescribeWordListSets();
	void SCI_METHOD Lex(unsigned int startPos, int lengthDoc, int initStyle, IDocument *pAccess);
	void SCI_METHOD Fold(unsigned int startPos, int lengthDoc, int initStyle, IDocument *pAccess);
};

#ifdef SCI_NAMESPACE
}
#endif

#endif
