/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

// Scintilla source code edit control
/** @file LexSpice.cxx
 ** Lexer for Spice
 **/
// Copyright 2006 by Fabien Proriol
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

#include "WordList.h"
#include "LexAccessor.h"
#include "Accessor.h"
#include "StyleContext.h"
#include "CharacterSet.h"
#include "LexerModule.h"

#ifdef SCI_NAMESPACE
using namespace Scintilla;
#endif

/*
 * Interface
 */

static void ColouriseDocument(
    unsigned int startPos,
    int length,
    int initStyle,
    WordList *keywordlists[],
    Accessor &styler);

static const char * const spiceWordListDesc[] = {
    "Keywords",        // SPICE command
    "Keywords2",    // SPICE functions
    "Keywords3",    // SPICE params
    0
};

LexerModule lmSpice(SCLEX_SPICE, ColouriseDocument, "spice", NULL, spiceWordListDesc);

/*
 * Implementation
 */

static void ColouriseComment(StyleContext& sc, bool& apostropheStartsAttribute);
static void ColouriseDelimiter(StyleContext& sc, bool& apostropheStartsAttribute);
static void ColouriseNumber(StyleContext& sc, bool& apostropheStartsAttribute);
static void ColouriseWhiteSpace(StyleContext& sc, bool& apostropheStartsAttribute);
static void ColouriseWord(StyleContext& sc, WordList& keywords, WordList& keywords2, WordList& keywords3, bool& apostropheStartsAttribute);

static inline bool IsDelimiterCharacter(int ch);
static inline bool IsSeparatorOrDelimiterCharacter(int ch);

static void ColouriseComment(StyleContext& sc, bool&) {
    sc.SetState(SCE_SPICE_COMMENTLINE);
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
    while (!sc.atLineEnd) {
        sc.Forward();
    }
}

static void ColouriseDelimiter(StyleContext& sc, bool& apostropheStartsAttribute) {
    apostropheStartsAttribute = sc.Match (')');
    sc.SetState(SCE_SPICE_DELIMITER);
    sc.ForwardSetState(SCE_SPICE_DEFAULT);
}

static void ColouriseNumber(StyleContext& sc, bool& apostropheStartsAttribute) {
    apostropheStartsAttribute = true;
    std::string number;
    sc.SetState(SCE_SPICE_NUMBER);
    // Get all characters up to a delimiter or a separator, including points, but excluding
    // double points (ranges).
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
    while (!IsSeparatorOrDelimiterCharacter(sc.ch) || (sc.ch == '.' && sc.chNext != '.')) {
        number += static_cast<char>(sc.ch);
        sc.Forward();
    }
    // Special case: exponent with sign
    if ((sc.chPrev == 'e' || sc.chPrev == 'E') &&
            (sc.ch == '+' || sc.ch == '-')) {
        number += static_cast<char>(sc.ch);
        sc.Forward ();
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
        while (!IsSeparatorOrDelimiterCharacter(sc.ch)) {
            number += static_cast<char>(sc.ch);
            sc.Forward();
        }
    }
    sc.SetState(SCE_SPICE_DEFAULT);
}

static void ColouriseWhiteSpace(StyleContext& sc, bool& ) {
    sc.SetState(SCE_SPICE_DEFAULT);
    sc.ForwardSetState(SCE_SPICE_DEFAULT);
}

static void ColouriseWord(StyleContext& sc, WordList& keywords, WordList& keywords2, WordList& keywords3, bool& apostropheStartsAttribute) {
    apostropheStartsAttribute = true;
    sc.SetState(SCE_SPICE_IDENTIFIER);
    std::string word;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
    while (!sc.atLineEnd && !IsSeparatorOrDelimiterCharacter(sc.ch)) {
        word += static_cast<char>(tolower(sc.ch));
        sc.Forward();
    }
    if (keywords.InList(word.c_str())) {
        sc.ChangeState(SCE_SPICE_KEYWORD);
        if (word != "all") {
            apostropheStartsAttribute = false;
        }
    }
    else if (keywords2.InList(word.c_str())) {
        sc.ChangeState(SCE_SPICE_KEYWORD2);
        if (word != "all") {
            apostropheStartsAttribute = false;
        }
    }
    else if (keywords3.InList(word.c_str())) {
        sc.ChangeState(SCE_SPICE_KEYWORD3);
        if (word != "all") {
            apostropheStartsAttribute = false;
        }
    }
    sc.SetState(SCE_SPICE_DEFAULT);
}

//
// ColouriseDocument
//
static void ColouriseDocument(
    unsigned int startPos,
    int length,
    int initStyle,
    WordList *keywordlists[],
    Accessor &styler) {
    WordList &keywords = *keywordlists[0];
    WordList &keywords2 = *keywordlists[1];
    WordList &keywords3 = *keywordlists[2];
    StyleContext sc(startPos, length, initStyle, styler);
    int lineCurrent = styler.GetLine(startPos);
    bool apostropheStartsAttribute = (styler.GetLineState(lineCurrent) & 1) != 0;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
    while (sc.More()) {
        if (sc.atLineEnd) {
            // Go to the next line
            sc.Forward();
            lineCurrent++;
            // Remember the line state for future incremental lexing
            styler.SetLineState(lineCurrent, apostropheStartsAttribute);
            // Don't continue any styles on the next line
            sc.SetState(SCE_SPICE_DEFAULT);
        }
        // Comments
        if ((sc.Match('*') && sc.atLineStart) || sc.Match('*','~')) {
            ColouriseComment(sc, apostropheStartsAttribute);
        // Whitespace
        } else if (IsASpace(sc.ch)) {
            ColouriseWhiteSpace(sc, apostropheStartsAttribute);
        // Delimiters
        } else if (IsDelimiterCharacter(sc.ch)) {
            ColouriseDelimiter(sc, apostropheStartsAttribute);
        // Numbers
        } else if (IsADigit(sc.ch) || sc.ch == '#') {
            ColouriseNumber(sc, apostropheStartsAttribute);
        // Keywords or identifiers
        } else {
            ColouriseWord(sc, keywords, keywords2, keywords3, apostropheStartsAttribute);
        }
    }
    sc.Complete();
}

static inline bool IsDelimiterCharacter(int ch) {
    switch (ch) {
    case '&':
    case '\'':
    case '(':
    case ')':
    case '*':
    case '+':
    case ',':
    case '-':
    case '.':
    case '/':
    case ':':
    case ';':
    case '<':
    case '=':
    case '>':
    case '|':
        return true;
    default:
        return false;
    }
}

static inline bool IsSeparatorOrDelimiterCharacter(int ch) {
    return IsASpace(ch) || IsDelimiterCharacter(ch);
}
