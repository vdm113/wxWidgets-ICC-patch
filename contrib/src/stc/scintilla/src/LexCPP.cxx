// Scintilla source code edit control
/** @file LexCPP.cxx
 ** Lexer for C++, C, Java, and Javascript.
 **/
// Copyright 1998-2002 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>

#include "Platform.h"

#include "PropSet.h"
#include "Accessor.h"
#include "StyleContext.h"
#include "KeyWords.h"
#include "Scintilla.h"
#include "SciLexer.h"

#define KEYWORD_BOXHEADER 1
#define KEYWORD_FOLDCONTRACTED 2

static bool IsOKBeforeRE(const int ch) {
	return (ch == '(') || (ch == '=') || (ch == ',');
}

static inline bool IsAWordChar(const int ch) {
	return (ch < 0x80) && (isalnum(ch) || ch == '.' || ch == '_');
}

static inline bool IsAWordStart(const int ch) {
	return (ch < 0x80) && (isalnum(ch) || ch == '_');
}

static inline bool IsADoxygenChar(const int ch) {
	return (islower(ch) || ch == '$' || ch == '@' ||
	        ch == '\\' || ch == '&' || ch == '<' ||
	        ch == '>' || ch == '#' || ch == '{' ||
	        ch == '}' || ch == '[' || ch == ']');
}

static inline bool IsStateComment(const int state) {
	return ((state == SCE_C_COMMENT) ||
	        (state == SCE_C_COMMENTLINE) ||
	        (state == SCE_C_COMMENTDOC) ||
	        (state == SCE_C_COMMENTDOCKEYWORD) ||
	        (state == SCE_C_COMMENTDOCKEYWORDERROR));
}

static inline bool IsStateString(const int state) {
	return ((state == SCE_C_STRING) || (state == SCE_C_VERBATIM));
}

static void ColouriseCppDoc(unsigned int startPos, int length, int initStyle, WordList *keywordlists[],
                            Accessor &styler, bool caseSensitive) {

	WordList &keywords = *keywordlists[0];
	WordList &keywords2 = *keywordlists[1];
	WordList &keywords3 = *keywordlists[2];
	WordList &keywords4 = *keywordlists[3];

	bool stylingWithinPreprocessor = styler.GetPropertyInt("styling.within.preprocessor") != 0;

	// Do not leak onto next line
	if (initStyle == SCE_C_STRINGEOL)
		initStyle = SCE_C_DEFAULT;

	int chPrevNonWhite = ' ';
	int visibleChars = 0;
	bool lastWordWasUUID = false;

	StyleContext sc(startPos, length, initStyle, styler);

	for (; sc.More(); sc.Forward()) {

		if (sc.atLineStart && (sc.state == SCE_C_STRING)) {
			// Prevent SCE_C_STRINGEOL from leaking back to previous line
			sc.SetState(SCE_C_STRING);
		}

		// Handle line continuation generically.
		if (sc.ch == '\\') {
			if (sc.chNext == '\n' || sc.chNext == '\r') {
				sc.Forward();
				if (sc.ch == '\r' && sc.chNext == '\n') {
					sc.Forward();
				}
				continue;
			}
		}

		// Determine if the current state should terminate.
		if (sc.state == SCE_C_OPERATOR) {
			sc.SetState(SCE_C_DEFAULT);
		} else if (sc.state == SCE_C_NUMBER) {
			if (!IsAWordChar(sc.ch)) {
				sc.SetState(SCE_C_DEFAULT);
			}
		} else if (sc.state == SCE_C_IDENTIFIER) {
			if (!IsAWordChar(sc.ch) || (sc.ch == '.')) {
				char s[100];
				if (caseSensitive) {
					sc.GetCurrent(s, sizeof(s));
				} else {
					sc.GetCurrentLowered(s, sizeof(s));
				}
				if (keywords.InList(s)) {
					lastWordWasUUID = strcmp(s, "uuid") == 0;
					sc.ChangeState(SCE_C_WORD);
				} else if (keywords2.InList(s)) {
					sc.ChangeState(SCE_C_WORD2);
				} else if (keywords4.InList(s)) {
					sc.ChangeState(SCE_C_GLOBALCLASS);
				}
				sc.SetState(SCE_C_DEFAULT);
			}
		} else if (sc.state == SCE_C_PREPROCESSOR) {
			if (stylingWithinPreprocessor) {
				if (IsASpace(sc.ch)) {
					sc.SetState(SCE_C_DEFAULT);
				}
			} else {
				if ((sc.ch == '\r') || (sc.ch == '\n') || (sc.Match('/', '*')) || (sc.Match('/', '/'))) {
					sc.SetState(SCE_C_DEFAULT);
				}
			}
		} else if (sc.state == SCE_C_COMMENT) {
			if (sc.Match('*', '/')) {
				sc.Forward();
				sc.ForwardSetState(SCE_C_DEFAULT);
			}
		} else if (sc.state == SCE_C_COMMENTDOC) {
			if (sc.Match('*', '/')) {
				sc.Forward();
				sc.ForwardSetState(SCE_C_DEFAULT);
			} else if (sc.ch == '@' || sc.ch == '\\') {
				sc.SetState(SCE_C_COMMENTDOCKEYWORD);
			}
		} else if (sc.state == SCE_C_COMMENTLINE || sc.state == SCE_C_COMMENTLINEDOC) {
			if (sc.ch == '\r' || sc.ch == '\n') {
				sc.SetState(SCE_C_DEFAULT);
				visibleChars = 0;
			}
		} else if (sc.state == SCE_C_COMMENTDOCKEYWORD) {
			if (sc.Match('*', '/')) {
				sc.ChangeState(SCE_C_COMMENTDOCKEYWORDERROR);
				sc.Forward();
				sc.ForwardSetState(SCE_C_DEFAULT);
			} else if (!IsADoxygenChar(sc.ch)) {
				char s[100];
				if (caseSensitive) {
					sc.GetCurrent(s, sizeof(s));
				} else {
					sc.GetCurrentLowered(s, sizeof(s));
				}
				if (!isspace(sc.ch) || !keywords3.InList(s + 1)) {
					sc.ChangeState(SCE_C_COMMENTDOCKEYWORDERROR);
				}
				sc.SetState(SCE_C_COMMENTDOC);
			}
		} else if (sc.state == SCE_C_STRING) {
			if (sc.ch == '\\') {
				if (sc.chNext == '\"' || sc.chNext == '\'' || sc.chNext == '\\') {
					sc.Forward();
				}
			} else if (sc.ch == '\"') {
				sc.ForwardSetState(SCE_C_DEFAULT);
			} else if (sc.atLineEnd) {
				sc.ChangeState(SCE_C_STRINGEOL);
				sc.ForwardSetState(SCE_C_DEFAULT);
				visibleChars = 0;
			}
		} else if (sc.state == SCE_C_CHARACTER) {
			if (sc.atLineEnd) {
				sc.ChangeState(SCE_C_STRINGEOL);
				sc.ForwardSetState(SCE_C_DEFAULT);
				visibleChars = 0;
			} else if (sc.ch == '\\') {
				if (sc.chNext == '\"' || sc.chNext == '\'' || sc.chNext == '\\') {
					sc.Forward();
				}
			} else if (sc.ch == '\'') {
				sc.ForwardSetState(SCE_C_DEFAULT);
			}
		} else if (sc.state == SCE_C_REGEX) {
			if (sc.ch == '\r' || sc.ch == '\n' || sc.ch == '/') {
				sc.ForwardSetState(SCE_C_DEFAULT);
			} else if (sc.ch == '\\') {
				// Gobble up the quoted character
				if (sc.chNext == '\\' || sc.chNext == '/') {
					sc.Forward();
				}
			}
		} else if (sc.state == SCE_C_VERBATIM) {
			if (sc.ch == '\"') {
				if (sc.chNext == '\"') {
					sc.Forward();
				} else {
					sc.ForwardSetState(SCE_C_DEFAULT);
				}
			}
		} else if (sc.state == SCE_C_UUID) {
			if (sc.ch == '\r' || sc.ch == '\n' || sc.ch == ')') {
				sc.SetState(SCE_C_DEFAULT);
			}
		}

		// Determine if a new state should be entered.
		if (sc.state == SCE_C_DEFAULT) {
			if (sc.Match('@', '\"')) {
				sc.SetState(SCE_C_VERBATIM);
				sc.Forward();
			} else if (IsADigit(sc.ch) || (sc.ch == '.' && IsADigit(sc.chNext))) {
				if (lastWordWasUUID) {
					sc.SetState(SCE_C_UUID);
					lastWordWasUUID = false;
				} else {
					sc.SetState(SCE_C_NUMBER);
				}
			} else if (IsAWordStart(sc.ch) || (sc.ch == '@')) {
				if (lastWordWasUUID) {
					sc.SetState(SCE_C_UUID);
					lastWordWasUUID = false;
				} else {
					sc.SetState(SCE_C_IDENTIFIER);
				}
			} else if (sc.Match('/', '*')) {
				if (sc.Match("/**") || sc.Match("/*!")) {	// Support of Qt/Doxygen doc. style
					sc.SetState(SCE_C_COMMENTDOC);
				} else {
					sc.SetState(SCE_C_COMMENT);
				}
				sc.Forward();	// Eat the * so it isn't used for the end of the comment
			} else if (sc.Match('/', '/')) {
				if ((sc.Match("///") && !sc.Match("////")) || sc.Match("//!"))
					// Support of Qt/Doxygen doc. style
					sc.SetState(SCE_C_COMMENTLINEDOC);
				else
					sc.SetState(SCE_C_COMMENTLINE);
			} else if (sc.ch == '/' && IsOKBeforeRE(chPrevNonWhite)) {
				sc.SetState(SCE_C_REGEX);
			} else if (sc.ch == '\"') {
				sc.SetState(SCE_C_STRING);
			} else if (sc.ch == '\'') {
				sc.SetState(SCE_C_CHARACTER);
			} else if (sc.ch == '#' && visibleChars == 0) {
				// Preprocessor commands are alone on their line
				sc.SetState(SCE_C_PREPROCESSOR);
				// Skip whitespace between # and preprocessor word
				do {
					sc.Forward();
				} while ((sc.ch == ' ' || sc.ch == '\t') && sc.More());
				if (sc.ch == '\r' || sc.ch == '\n') {
					sc.SetState(SCE_C_DEFAULT);
				}
			} else if (isoperator(static_cast<char>(sc.ch))) {
				sc.SetState(SCE_C_OPERATOR);
			}
		}

		if (sc.atLineEnd) {
			// Reset states to begining of colourise so no surprises
			// if different sets of lines lexed.
			chPrevNonWhite = ' ';
			visibleChars = 0;
			lastWordWasUUID = false;
		}
		if (!IsASpace(sc.ch)) {
			chPrevNonWhite = sc.ch;
			visibleChars++;
		}
	}
	sc.Complete();
}

static bool IsStreamCommentStyle(int style) {
	return style == SCE_C_COMMENT ||
	       style == SCE_C_COMMENTDOC ||
	       style == SCE_C_COMMENTDOCKEYWORD ||
	       style == SCE_C_COMMENTDOCKEYWORDERROR;
}

// Store both the current line's fold level and the next lines in the
// level store to make it easy to pick up with each increment
// and to make it possible to fiddle the current level for "} else {".
static void FoldNoBoxCppDoc(unsigned int startPos, int length, int initStyle,
                            Accessor &styler) {
	bool foldComment = styler.GetPropertyInt("fold.comment") != 0;
	bool foldPreprocessor = styler.GetPropertyInt("fold.preprocessor") != 0;
	bool foldCompact = styler.GetPropertyInt("fold.compact", 1) != 0;
	bool foldAtElse = styler.GetPropertyInt("fold.at.else", 0) != 0;
	unsigned int endPos = startPos + length;
	int visibleChars = 0;
	int lineCurrent = styler.GetLine(startPos);
	int levelCurrent = SC_FOLDLEVELBASE;
	if (lineCurrent > 0)
		levelCurrent = styler.LevelAt(lineCurrent-1) >> 16;
	int levelMinCurrent = levelCurrent;
	int levelNext = levelCurrent;
	char chNext = styler[startPos];
	int styleNext = styler.StyleAt(startPos);
	int style = initStyle;
	for (unsigned int i = startPos; i < endPos; i++) {
		char ch = chNext;
		chNext = styler.SafeGetCharAt(i + 1);
		int stylePrev = style;
		style = styleNext;
		styleNext = styler.StyleAt(i + 1);
		bool atEOL = (ch == '\r' && chNext != '\n') || (ch == '\n');
		if (foldComment && IsStreamCommentStyle(style)) {
			if (!IsStreamCommentStyle(stylePrev)) {
				levelNext++;
			} else if (!IsStreamCommentStyle(styleNext) && !atEOL) {
				// Comments don't end at end of line and the next character may be unstyled.
				levelNext--;
			}
		}
		if (foldComment && (style == SCE_C_COMMENTLINE)) {
			if ((ch == '/') && (chNext == '/')) {
				char chNext2 = styler.SafeGetCharAt(i + 2);
				if (chNext2 == '{') {
					levelNext++;
				} else if (chNext2 == '}') {
					levelNext--;
				}
			}
		}
		if (foldPreprocessor && (style == SCE_C_PREPROCESSOR)) {
			if (ch == '#') {
				unsigned int j = i + 1;
				while ((j < endPos) && IsASpaceOrTab(styler.SafeGetCharAt(j))) {
					j++;
				}
				if (styler.Match(j, "region") || styler.Match(j, "if")) {
					levelNext++;
				} else if (styler.Match(j, "end")) {
					levelNext--;
				}
			}
		}
		if (style == SCE_C_OPERATOR) {
			if (ch == '{') {
				// Measure the minimum before a '{' to allow
				// folding on "} else {"
				if (levelMinCurrent > levelNext) {
					levelMinCurrent = levelNext;
				}
				levelNext++;
			} else if (ch == '}') {
				levelNext--;
			}
		}
		if (atEOL) {
			int levelUse = levelCurrent;
			if (foldAtElse) {
				levelUse = levelMinCurrent;
			}
			int lev = levelUse | levelNext << 16;
			if (visibleChars == 0 && foldCompact)
				lev |= SC_FOLDLEVELWHITEFLAG;
			if (levelUse < levelNext)
				lev |= SC_FOLDLEVELHEADERFLAG;
			if (lev != styler.LevelAt(lineCurrent)) {
				styler.SetLevel(lineCurrent, lev);
			}
			lineCurrent++;
			levelCurrent = levelNext;
			levelMinCurrent = levelCurrent;
			visibleChars = 0;
		}
		if (!isspacechar(ch))
			visibleChars++;
	}
}

static void FoldCppDoc(unsigned int startPos, int length, int initStyle, WordList *[],
                       Accessor &styler) {
	FoldNoBoxCppDoc(startPos, length, initStyle, styler);
}

static const char * const cppWordLists[] = {
            "Primary keywords and identifiers",
            "Secondary keywords and identifiers",
            "Documentation comment keywords",
            "Unused",
            "Global classes and typedefs",
            0,
        };

static void ColouriseCppDocSensitive(unsigned int startPos, int length, int initStyle, WordList *keywordlists[],
                                     Accessor &styler) {
	ColouriseCppDoc(startPos, length, initStyle, keywordlists, styler, true);
}

static void ColouriseCppDocInsensitive(unsigned int startPos, int length, int initStyle, WordList *keywordlists[],
                                       Accessor &styler) {
	ColouriseCppDoc(startPos, length, initStyle, keywordlists, styler, false);
}

LexerModule lmCPP(SCLEX_CPP, ColouriseCppDocSensitive, "cpp", FoldCppDoc, cppWordLists);
LexerModule lmCPPNoCase(SCLEX_CPPNOCASE, ColouriseCppDocInsensitive, "cppnocase", FoldCppDoc, cppWordLists);
LexerModule lmTCL(SCLEX_TCL, ColouriseCppDocSensitive, "tcl", FoldCppDoc, cppWordLists);
