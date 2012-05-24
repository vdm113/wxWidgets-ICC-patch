/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

// Scintilla source code edit control
/** @file PropSet.h
 ** An interface to the methods needed for access to property sets inside lexers.
 **/
// Copyright 1998-2009 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef PROPSET_H
#define PROPSET_H

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif

class PropertyGet {
public:
	virtual char *ToString() const=0;	// Caller must delete[] the return value
	virtual int GetInt(const char *key, int defaultValue=0) const=0;
	virtual ~PropertyGet() {}
};

#ifdef SCI_NAMESPACE
}
#endif

#endif
