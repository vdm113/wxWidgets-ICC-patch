/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif defined(__INTEL_COMPILER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

// Scintilla source code edit control

/** @file KeyMap.h
 ** Defines a mapping between keystrokes and commands.
 **/
// Copyright 1998-2001 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef KEYTOCOMMAND_H
#define KEYTOCOMMAND_H

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif

#define SCI_NORM 0
#define SCI_SHIFT SCMOD_SHIFT
#define SCI_CTRL SCMOD_CTRL
#define SCI_ALT SCMOD_ALT
#define SCI_CSHIFT (SCI_CTRL | SCI_SHIFT)
#define SCI_ASHIFT (SCI_ALT | SCI_SHIFT)

/**
 */
class KeyToCommand {
public:
	int key;
	int modifiers;
	unsigned int msg;
};

/**
 */
class KeyMap {
	KeyToCommand *kmap;
	int len;
	int alloc;
	static const KeyToCommand MapDefault[];

public:
	KeyMap();
	~KeyMap();
	void Clear();
	void AssignCmdKey(int key, int modifiers, unsigned int msg);
	unsigned int Find(int key, int modifiers);	// 0 returned on failure
};

#ifdef SCI_NAMESPACE
}
#endif

#endif
