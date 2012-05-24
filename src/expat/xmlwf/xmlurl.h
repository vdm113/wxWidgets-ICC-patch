/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

#ifdef __cplusplus
extern "C" {
#endif

int XML_URLInit();
void XML_URLUninit();
int XML_ProcessURL(XML_Parser parser,
                   const XML_Char *url,
                   unsigned flags);

#ifdef __cplusplus
}
#endif
