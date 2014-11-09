/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

// Scintilla source code edit control
/** @file StringCopy.h
 ** Safe string copy function which always NUL terminates.
 ** ELEMENTS macro for determining array sizes.
 **/
// Copyright 2013 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef STRINGCOPY_H
#define STRINGCOPY_H

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif

// Safer version of string copy functions like strcpy, wcsncpy, etc.
// Instantiate over fixed length strings of both char and wchar_t.
// May truncate if source doesn't fit into dest with room for NUL.

template <typename T, size_t count>
void StringCopy(T (&dest)[count], const T* source) {
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif
	for (size_t i=0; i<count; i++) {
		dest[i] = source[i];
		if (!source[i])
			break;
	}
	dest[count-1] = 0;
}

#define ELEMENTS(a) (sizeof(a) / sizeof(a[0]))

#ifdef SCI_NAMESPACE
}
#endif

#endif
