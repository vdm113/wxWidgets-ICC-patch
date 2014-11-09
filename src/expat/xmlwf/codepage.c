/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/* Copyright (c) 1998, 1999 Thai Open Source Software Center Ltd
   See the file COPYING for copying permission.
*/

#include "codepage.h"

#if (defined(WIN32) || (defined(__WATCOMC__) && defined(__NT__)))
#define STRICT 1
#define WIN32_LEAN_AND_MEAN 1

#include <windows.h>

int
codepageMap(int cp, int *map)
{
  int i;
  CPINFO info;
  if (!GetCPInfo(cp, &info) || info.MaxCharSize > 2)
    return 0;
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif
  for (i = 0; i < 256; i++)
    map[i] = -1;
  if (info.MaxCharSize > 1) {
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif
    for (i = 0; i < MAX_LEADBYTES; i+=2) {
      int j, lim;
      if (info.LeadByte[i] == 0 && info.LeadByte[i + 1] == 0)
        break;
      lim = info.LeadByte[i + 1];
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif
      for (j = info.LeadByte[i]; j <= lim; j++)
        map[j] = -2;
    }
  }
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif
  for (i = 0; i < 256; i++) {
   if (map[i] == -1) {
     char c = (char)i;
     unsigned short n;
     if (MultiByteToWideChar(cp, MB_PRECOMPOSED|MB_ERR_INVALID_CHARS,
                             &c, 1, &n, 1) == 1)
       map[i] = n;
   }
  }
  return 1;
}

int
codepageConvert(int cp, const char *p)
{
  unsigned short c;
  if (MultiByteToWideChar(cp, MB_PRECOMPOSED|MB_ERR_INVALID_CHARS,
                          p, 2, &c, 1) == 1)
    return c;
  return -1;
}

#else /* not WIN32 */

int
codepageMap(int cp, int *map)
{
  return 0;
}

int
codepageConvert(int cp, const char *p)
{
  return -1;
}

#endif /* not WIN32 */
