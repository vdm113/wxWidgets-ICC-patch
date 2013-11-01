/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Registered charset names are at most 40 characters long. */

#define CHARSET_MAX 41

/* Figure out the charset to use from the ContentType.
   buf contains the body of the header field (the part after "Content-Type:").
   charset gets the charset to use.  It must be at least CHARSET_MAX chars
   long.  charset will be empty if the default charset should be used.
*/

void getXMLCharset(const char *buf, char *charset);

#ifdef __cplusplus
}
#endif
