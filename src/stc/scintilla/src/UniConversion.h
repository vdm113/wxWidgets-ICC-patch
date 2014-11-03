/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

// Scintilla source code edit control
/** @file UniConversion.h
 ** Functions to handle UTF-8 and UTF-16 strings.
 **/
// Copyright 1998-2001 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef UNICONVERSION_H
#define UNICONVERSION_H

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif

const int UTF8MaxBytes = 4;

unsigned int UTF8Length(const wchar_t *uptr, unsigned int tlen);
void UTF8FromUTF16(const wchar_t *uptr, unsigned int tlen, char *putf, unsigned int len);
unsigned int UTF8CharLength(unsigned char ch);
unsigned int UTF16Length(const char *s, unsigned int len);
unsigned int UTF16FromUTF8(const char *s, unsigned int len, wchar_t *tbuf, unsigned int tlen);

extern int UTF8BytesOfLead[256];
void UTF8BytesOfLeadInitialise();

inline bool UTF8IsTrailByte(int ch) {
	return (ch >= 0x80) && (ch < 0xc0);
}

inline bool UTF8IsAscii(int ch) {
	return ch < 0x80;
}

enum { UTF8MaskWidth=0x7, UTF8MaskInvalid=0x8 };
int UTF8Classify(const unsigned char *us, int len);

// Similar to UTF8Classify but returns a length of 1 for invalid bytes
// instead of setting the invalid flag
int UTF8DrawBytes(const unsigned char *us, int len);

// Line separator is U+2028 \xe2\x80\xa8
// Paragraph separator is U+2029 \xe2\x80\xa9
const int UTF8SeparatorLength = 3;
inline bool UTF8IsSeparator(const unsigned char *us) {
	return (us[0] == 0xe2) && (us[1] == 0x80) && ((us[2] == 0xa8) || (us[2] == 0xa9));
}

// NEL is U+0085 \xc2\x85
const int UTF8NELLength = 2;
inline bool UTF8IsNEL(const unsigned char *us) {
	return (us[0] == 0xc2) && (us[1] == 0x85);
}

#ifdef SCI_NAMESPACE
}
#endif

#endif
