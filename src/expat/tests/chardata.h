/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/* chardata.h

   Interface to some helper routines used to accumulate and check text
   and attribute content.
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef XML_CHARDATA_H
#define XML_CHARDATA_H 1

#ifndef XML_VERSION
#include "expat.h"                      /* need XML_Char */
#endif


typedef struct {
    int count;                          /* # of chars, < 0 if not set */
    XML_Char data[1024];
} CharData;


void CharData_Init(CharData *storage);

void CharData_AppendString(CharData *storage, const char *s);

void CharData_AppendXMLChars(CharData *storage, const XML_Char *s, int len);

int CharData_CheckString(CharData *storage, const char *s);

int CharData_CheckXMLChars(CharData *storage, const XML_Char *s);


#endif  /* XML_CHARDATA_H */

#ifdef __cplusplus
}
#endif
