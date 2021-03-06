/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

// Scintilla source code edit control
/** @file CaseFolder.h
 ** Classes for case folding.
 **/
// Copyright 1998-2013 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef CASEFOLDER_H
#define CASEFOLDER_H

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif

class CaseFolder {
public:
	virtual ~CaseFolder();
	virtual size_t Fold(char *folded, size_t sizeFolded, const char *mixed, size_t lenMixed) = 0;
};

class CaseFolderTable : public CaseFolder {
protected:
	char mapping[256];
public:
	CaseFolderTable();
	virtual ~CaseFolderTable();
	virtual size_t Fold(char *folded, size_t sizeFolded, const char *mixed, size_t lenMixed);
	void SetTranslation(char ch, char chTranslation);
	void StandardASCII();
};

class ICaseConverter;

class CaseFolderUnicode : public CaseFolderTable {
	ICaseConverter *converter;
public:
	CaseFolderUnicode();
	virtual size_t Fold(char *folded, size_t sizeFolded, const char *mixed, size_t lenMixed);
};

#ifdef SCI_NAMESPACE
}
#endif

#endif
