/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif defined(__INTEL_COMPILER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/* Copyright (c) 1998, 1999 Thai Open Source Software Center Ltd

   See the file COPYING for copying permission.
*/

/* 0x80 */ BT_OTHER, BT_OTHER, BT_OTHER, BT_OTHER,
/* 0x84 */ BT_OTHER, BT_OTHER, BT_OTHER, BT_OTHER,
/* 0x88 */ BT_OTHER, BT_OTHER, BT_OTHER, BT_OTHER,
/* 0x8C */ BT_OTHER, BT_OTHER, BT_OTHER, BT_OTHER,
/* 0x90 */ BT_OTHER, BT_OTHER, BT_OTHER, BT_OTHER,
/* 0x94 */ BT_OTHER, BT_OTHER, BT_OTHER, BT_OTHER,
/* 0x98 */ BT_OTHER, BT_OTHER, BT_OTHER, BT_OTHER,
/* 0x9C */ BT_OTHER, BT_OTHER, BT_OTHER, BT_OTHER,
/* 0xA0 */ BT_OTHER, BT_OTHER, BT_OTHER, BT_OTHER,
/* 0xA4 */ BT_OTHER, BT_OTHER, BT_OTHER, BT_OTHER,
/* 0xA8 */ BT_OTHER, BT_OTHER, BT_NMSTRT, BT_OTHER,
/* 0xAC */ BT_OTHER, BT_OTHER, BT_OTHER, BT_OTHER,
/* 0xB0 */ BT_OTHER, BT_OTHER, BT_OTHER, BT_OTHER,
/* 0xB4 */ BT_OTHER, BT_NMSTRT, BT_OTHER, BT_NAME,
/* 0xB8 */ BT_OTHER, BT_OTHER, BT_NMSTRT, BT_OTHER,
/* 0xBC */ BT_OTHER, BT_OTHER, BT_OTHER, BT_OTHER,
/* 0xC0 */ BT_NMSTRT, BT_NMSTRT, BT_NMSTRT, BT_NMSTRT,
/* 0xC4 */ BT_NMSTRT, BT_NMSTRT, BT_NMSTRT, BT_NMSTRT,
/* 0xC8 */ BT_NMSTRT, BT_NMSTRT, BT_NMSTRT, BT_NMSTRT,
/* 0xCC */ BT_NMSTRT, BT_NMSTRT, BT_NMSTRT, BT_NMSTRT,
/* 0xD0 */ BT_NMSTRT, BT_NMSTRT, BT_NMSTRT, BT_NMSTRT,
/* 0xD4 */ BT_NMSTRT, BT_NMSTRT, BT_NMSTRT, BT_OTHER,
/* 0xD8 */ BT_NMSTRT, BT_NMSTRT, BT_NMSTRT, BT_NMSTRT,
/* 0xDC */ BT_NMSTRT, BT_NMSTRT, BT_NMSTRT, BT_NMSTRT,
/* 0xE0 */ BT_NMSTRT, BT_NMSTRT, BT_NMSTRT, BT_NMSTRT,
/* 0xE4 */ BT_NMSTRT, BT_NMSTRT, BT_NMSTRT, BT_NMSTRT,
/* 0xE8 */ BT_NMSTRT, BT_NMSTRT, BT_NMSTRT, BT_NMSTRT,
/* 0xEC */ BT_NMSTRT, BT_NMSTRT, BT_NMSTRT, BT_NMSTRT,
/* 0xF0 */ BT_NMSTRT, BT_NMSTRT, BT_NMSTRT, BT_NMSTRT,
/* 0xF4 */ BT_NMSTRT, BT_NMSTRT, BT_NMSTRT, BT_OTHER,
/* 0xF8 */ BT_NMSTRT, BT_NMSTRT, BT_NMSTRT, BT_NMSTRT,
/* 0xFC */ BT_NMSTRT, BT_NMSTRT, BT_NMSTRT, BT_NMSTRT,
