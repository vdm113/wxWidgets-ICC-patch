/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif defined(__INTEL_COMPILER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

// Scintilla source code edit control

/** @file Indicator.h
 ** Defines the style of indicators which are text decorations such as underlining.
 **/
// Copyright 1998-2001 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef INDICATOR_H
#define INDICATOR_H

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif

/**
 */
class Indicator {
public:
	int style;
	bool under;
	ColourPair fore;
	int fillAlpha;
	Indicator() : style(INDIC_PLAIN), under(false), fore(ColourDesired(0,0,0)), fillAlpha(30) {
	}
	void Draw(Surface *surface, const PRectangle &rc, const PRectangle &rcLine);
};

#ifdef SCI_NAMESPACE
}
#endif

#endif
