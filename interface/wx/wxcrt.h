/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////


// Name:        wx/wxcrt.h
// Purpose:     interface of global wx wrappers for CRT functions
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


/** @addtogroup group_funcmacro_crt */
//@{

bool wxIsEmpty(const char *s);
bool wxIsEmpty(const wchar_t *s);
bool wxIsEmpty(const wxCharBuffer& s);
bool wxIsEmpty(const wxWCharBuffer& s);
bool wxIsEmpty(const wxString& s);
bool wxIsEmpty(const wxCStrData& s);
wxChar* wxTmemchr(const wxChar* s, wxChar c, size_t l);
int wxTmemcmp(const wxChar* sz1, const wxChar* sz2, size_t len);
wxChar* wxTmemcpy(wxChar* szOut, const wxChar* szIn, size_t len);
wxChar* wxTmemmove(wxChar* szOut, const wxChar* szIn, size_t len);
wxChar* wxTmemset(wxChar* szOut, const wxChar cIn, size_t len);
char* wxTmemchr(const char* s, char c, size_t len);
int wxTmemcmp(const char* sz1, const char* sz2, size_t len);
char* wxTmemcpy(char* szOut, const char* szIn, size_t len);
char* wxTmemmove(char* szOut, const char* szIn, size_t len);
char* wxTmemset(char* szOut, const char cIn, size_t len);
char* wxSetlocale(int category, const wxCharBuffer& locale);
char* wxSetlocale(int category, const wxString& locale);
char* wxSetlocale(int category, const wxCStrData& locale);
size_t wxStrlen(const wxCharBuffer& s);
size_t wxStrlen(const wxWCharBuffer& s);
size_t wxStrlen(const wxString& s);
size_t wxStrlen(const wxCStrData& s);
size_t wxStrnlen(const char *str, size_t maxlen);
size_t wxStrnlen(const char *str, size_t maxlen);
size_t wxStrnlen(const wchar_t *str, size_t maxlen);
size_t wxStrnlen(const wchar_t *str, size_t maxlen);
char* wxStrdup(const wxCharBuffer& s);
wchar_t* wxStrdup(const wxWCharBuffer& s);
char* wxStrdup(const wxString& s);
char* wxStrdup(const wxCStrData& s);
char *wxStrcpy(char *dest, const char *src);
wchar_t *wxStrcpy(wchar_t *dest, const wchar_t *src);
char *wxStrcpy(char *dest, const wxString& src);
char *wxStrcpy(char *dest, const wxCStrData& src);
char *wxStrcpy(char *dest, const wxCharBuffer& src);
wchar_t *wxStrcpy(wchar_t *dest, const wxString& src);
wchar_t *wxStrcpy(wchar_t *dest, const wxCStrData& src);
wchar_t *wxStrcpy(wchar_t *dest, const wxWCharBuffer& src);
char *wxStrcpy(char *dest, const wchar_t *src);
wchar_t *wxStrcpy(wchar_t *dest, const char *src);
char *wxStrncpy(char *dest, const char *src, size_t n);
wchar_t *wxStrncpy(wchar_t *dest, const wchar_t *src, size_t n);
char *wxStrncpy(char *dest, const wxString& src, size_t n);
char *wxStrncpy(char *dest, const wxCStrData& src, size_t n);
char *wxStrncpy(char *dest, const wxCharBuffer& src, size_t n);
wchar_t *wxStrncpy(wchar_t *dest, const wxString& src, size_t n);
wchar_t *wxStrncpy(wchar_t *dest, const wxCStrData& src, size_t n);
wchar_t *wxStrncpy(wchar_t *dest, const wxWCharBuffer& src, size_t n);
char *wxStrncpy(char *dest, const wchar_t *src, size_t n);
wchar_t *wxStrncpy(wchar_t *dest, const char *src, size_t n);
size_t wxStrlcpy(char *dest, const char *src, size_t n);
size_t wxStrlcpy(wchar_t *dest, const wchar_t *src, size_t n);
char *wxStrcat(char *dest, const char *src);
wchar_t *wxStrcat(wchar_t *dest, const wchar_t *src);
char *wxStrcat(char *dest, const wxString& src);
char *wxStrcat(char *dest, const wxCStrData& src);
char *wxStrcat(char *dest, const wxCharBuffer& src);
wchar_t *wxStrcat(wchar_t *dest, const wxString& src);
wchar_t *wxStrcat(wchar_t *dest, const wxCStrData& src);
wchar_t *wxStrcat(wchar_t *dest, const wxWCharBuffer& src);
char *wxStrcat(char *dest, const wchar_t *src);
wchar_t *wxStrcat(wchar_t *dest, const char *src);
char *wxStrncat(char *dest, const char *src, size_t n);
wchar_t *wxStrncat(wchar_t *dest, const wchar_t *src, size_t n);
char *wxStrncat(char *dest, const wxString& src, size_t n);
char *wxStrncat(char *dest, const wxCStrData& src, size_t n);
char *wxStrncat(char *dest, const wxCharBuffer& src, size_t n);
wchar_t *wxStrncat(wchar_t *dest, const wxString& src, size_t n);
wchar_t *wxStrncat(wchar_t *dest, const wxCStrData& src, size_t n);
wchar_t *wxStrncat(wchar_t *dest, const wxWCharBuffer& src, size_t n);
char *wxStrncat(char *dest, const wchar_t *src, size_t n);
wchar_t *wxStrncat(wchar_t *dest, const char *src, size_t n);
int wxStrcmp_String(const wxString& s1, const T& s2);
int wxStricmp_String(const wxString& s1, const T& s2);
int wxStrcoll_String(const wxString& s1, const T& s2);;
int wxStrcoll_String(const wxString& s1, const T& s2);
size_t wxStrspn_String(const wxString& s1, const T& s2);
size_t wxStrcspn_String(const wxString& s1, const T& s2);
int wxStrncmp_String(const wxString& s1, const T& s2, size_t n);
int wxStrnicmp_String(const wxString& s1, const T& s2, size_t n);
size_t wxStrxfrm(char *dest, const char *src, size_t n);
size_t wxStrxfrm(wchar_t *dest, const wchar_t *src, size_t n);
size_t wxStrxfrm(T *dest, const wxCharTypeBuffer<T>& src, size_t n);
size_t wxStrxfrm(char *dest, const wxString& src, size_t n);
size_t wxStrxfrm(wchar_t *dest, const wxString& src, size_t n);
size_t wxStrxfrm(char *dest, const wxCStrData& src, size_t n);
size_t wxStrxfrm(wchar_t *dest, const wxCStrData& src, size_t n);
char *wxStrtok(char *str, const char *delim, char **saveptr);
wchar_t *wxStrtok(wchar_t *str, const wchar_t *delim, wchar_t **saveptr);
char *wxStrtok(char *str, const wxCStrData& delim, char **saveptr);
wchar_t *wxStrtok(wchar_t *str, const wxCStrData& delim, wchar_t **saveptr);
char *wxStrtok(char *str, const wxString& delim, char **saveptr);
wchar_t *wxStrtok(wchar_t *str, const wxString& delim, wchar_t **saveptr);
const char *wxStrstr(const char *haystack, const char *needle);
const wchar_t *wxStrstr(const wchar_t *haystack, const wchar_t *needle);
const char *wxStrstr(const char *haystack, const wxString& needle);
const wchar_t *wxStrstr(const wchar_t *haystack, const wxString& needle);
const char *wxStrstr(const wxString& haystack, const wxString& needle);
const char *wxStrstr(const wxCStrData& haystack, const wxString& needle);
const char *wxStrstr(const wxCStrData& haystack, const wxCStrData& needle);
const char *wxStrstr(const wxString& haystack, const char *needle);
const char *wxStrstr(const wxCStrData& haystack, const char *needle);
const wchar_t *wxStrstr(const wxString& haystack, const wchar_t *needle);
const wchar_t *wxStrstr(const wxCStrData& haystack, const wchar_t *needle);
const char *wxStrchr(const char *s, char c);
const wchar_t *wxStrchr(const wchar_t *s, wchar_t c);
const char *wxStrrchr(const char *s, char c);
const wchar_t *wxStrrchr(const wchar_t *s, wchar_t c);
const char *wxStrchr(const char *s, const wxUniChar& c);
const wchar_t *wxStrchr(const wchar_t *s, const wxUniChar& c);
const char *wxStrrchr(const char *s, const wxUniChar& c);
const wchar_t *wxStrrchr(const wchar_t *s, const wxUniChar& c);
const char *wxStrchr(const char *s, const wxUniCharRef& c);
const wchar_t *wxStrchr(const wchar_t *s, const wxUniCharRef& c);
const char *wxStrrchr(const char *s, const wxUniCharRef& c);
const wchar_t *wxStrrchr(const wchar_t *s, const wxUniCharRef& c);
const T* wxStrchr(const wxCharTypeBuffer<T>& s, T c);
const T* wxStrrchr(const wxCharTypeBuffer<T>& s, T c);
const T* wxStrchr(const wxCharTypeBuffer<T>& s, const wxUniChar& c);
const T* wxStrrchr(const wxCharTypeBuffer<T>& s, const wxUniChar& c);
const T* wxStrchr(const wxCharTypeBuffer<T>& s, const wxUniCharRef& c);
const T* wxStrrchr(const wxCharTypeBuffer<T>& s, const wxUniCharRef& c);
const char* wxStrchr(const wxString& s, char c);
const char* wxStrrchr(const wxString& s, char c);
const char* wxStrchr(const wxString& s, int c);
const char* wxStrrchr(const wxString& s, int c);
const char* wxStrchr(const wxString& s, const wxUniChar& c);
const char* wxStrrchr(const wxString& s, const wxUniChar& c);
const char* wxStrchr(const wxString& s, const wxUniCharRef& c);
const char* wxStrrchr(const wxString& s, const wxUniCharRef& c);
const wchar_t* wxStrchr(const wxString& s, wchar_t c);
const wchar_t* wxStrrchr(const wxString& s, wchar_t c);
const char* wxStrchr(const wxCStrData& s, char c);
const char* wxStrrchr(const wxCStrData& s, char c);
const char* wxStrchr(const wxCStrData& s, int c);
const char* wxStrrchr(const wxCStrData& s, int c);
const char* wxStrchr(const wxCStrData& s, const wxUniChar& c);
const char* wxStrrchr(const wxCStrData& s, const wxUniChar& c);
const char* wxStrchr(const wxCStrData& s, const wxUniCharRef& c);
const char* wxStrrchr(const wxCStrData& s, const wxUniCharRef& c);
const wchar_t* wxStrchr(const wxCStrData& s, wchar_t c);
const wchar_t* wxStrrchr(const wxCStrData& s, wchar_t c);
const char *wxStrpbrk(const char *s, const char *accept);
const wchar_t *wxStrpbrk(const wchar_t *s, const wchar_t *accept);
const char *wxStrpbrk(const char *s, const wxString& accept);
const char *wxStrpbrk(const char *s, const wxCStrData& accept);
const wchar_t *wxStrpbrk(const wchar_t *s, const wxString& accept);
const wchar_t *wxStrpbrk(const wchar_t *s, const wxCStrData& accept);
const char *wxStrpbrk(const wxString& s, const wxString& accept);
const char *wxStrpbrk(const wxString& s, const char *accept);
const wchar_t *wxStrpbrk(const wxString& s, const wchar_t *accept);
const char *wxStrpbrk(const wxString& s, const wxCStrData& accept);
const char *wxStrpbrk(const wxCStrData& s, const wxString& accept);
const char *wxStrpbrk(const wxCStrData& s, const char *accept);
const wchar_t *wxStrpbrk(const wxCStrData& s, const wchar_t *accept);
const char *wxStrpbrk(const wxCStrData& s, const wxCStrData& accept);
const T *wxStrpbrk(const S& s, const wxCharTypeBuffer<T>& accept);
/* inlined non-const versions */;
char *wxStrstr(char *haystack, const char *needle);
wchar_t *wxStrstr(wchar_t *haystack, const wchar_t *needle);
char *wxStrstr(char *haystack, const wxString& needle);
wchar_t *wxStrstr(wchar_t *haystack, const wxString& needle);
char * wxStrchr(char *s, char c);
char * wxStrrchr(char *s, char c);
wchar_t * wxStrchr(wchar_t *s, wchar_t c);
wchar_t * wxStrrchr(wchar_t *s, wchar_t c);
char * wxStrpbrk(char *s, const char *accept);
wchar_t * wxStrpbrk(wchar_t *s, const wchar_t *accept);
char * wxStrpbrk(char *s, const wxString& accept);
wchar_t * wxStrpbrk(wchar_t *s, const wxString& accept);
FILE *wxFopen(const wxString& path, const wxString& mode);
FILE *wxFreopen(const wxString& path, const wxString& mode, FILE *stream);
int wxRemove(const wxString& path);
int wxRename(const wxString& oldpath, const wxString& newpath);
char *wxFgets(char *s, int size, FILE *stream);
int wxFgetc(FILE *stream);
int wxUngetc(int c, FILE *stream);
int wxAtoi(const wxString& str);
int wxAtoi(const wxString& str);
long wxAtol(const wxString& str);
long wxAtol(const wxString& str);
double wxAtof(const wxString& str);
double wxAtof(const wxString& str);
double wxStrtod(const char *nptr, char **endptr);
double wxStrtod(const wchar_t *nptr, wchar_t **endptr);
double wxStrtod(const wxCharTypeBuffer<T>& nptr, T **endptr);
double wxStrtod(const wxString& nptr, T endptr);
double wxStrtod(const wxCStrData& nptr, T endptr);
int wxSystem(const wxString& str);
int wxSystem(const wxString& str);
char* wxGetenv(const char *name);
wchar_t* wxGetenv(const wchar_t *name);
char* wxGetenv(const wxString& name);
char* wxGetenv(const wxCStrData& name);
char* wxGetenv(const wxCharBuffer& name);
wchar_t* wxGetenv(const wxWCharBuffer& name);
size_t wxStrftime(char *s, size_t max, size_t max, const wxString& format, const struct tm *tm);
size_t wxStrftime(wchar_t *s, size_t max, size_t max, const wxString& format, const struct tm *tm);
bool wxIsalnum(const wxUniChar& c) ;
bool wxIsalpha(const wxUniChar& c) ;
bool wxIscntrl(const wxUniChar& c) ;
bool wxIsdigit(const wxUniChar& c) ;
bool wxIsgraph(const wxUniChar& c) ;
bool wxIslower(const wxUniChar& c) ;
bool wxIsprint(const wxUniChar& c) ;
bool wxIspunct(const wxUniChar& c) ;
bool wxIsspace(const wxUniChar& c) ;
bool wxIsupper(const wxUniChar& c) ;
bool wxIsxdigit(const wxUniChar& c);
wxUniChar wxTolower(const wxUniChar& c);
wxUniChar wxToupper(const wxUniChar& c);
int wxIsctrl(const wxUniChar& c);

//@}
