/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
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

struct StyleAndColour {
	int style;
	ColourDesired fore;
	StyleAndColour() : style(INDIC_PLAIN), fore(0, 0, 0) {
	}
	StyleAndColour(int style_, ColourDesired fore_ = ColourDesired(0, 0, 0)) : style(style_), fore(fore_) {
	}
	bool operator==(const StyleAndColour &other) const {
		return (style == other.style) && (fore == other.fore);
	}
};

/**
 */
class Indicator {
public:
	int style;
	ColourDesired fore;
	bool under;
	int fillAlpha;
	int outlineAlpha;
	Indicator() : style(INDIC_PLAIN), fore(ColourDesired(0,0,0)), under(false), fillAlpha(30), outlineAlpha(50) {
	}
	Indicator(int style_, ColourDesired fore_=ColourDesired(0,0,0), bool under_=false, int fillAlpha_=30, int outlineAlpha_=50) :
		style(style_), fore(fore_), under(under_), fillAlpha(fillAlpha_), outlineAlpha(outlineAlpha_) {
	}
	void SetFlags(int attributes_);
};

#ifdef SCI_NAMESPACE
}
#endif

#endif
