/////////////////////////////////////////////////////////////////////////////
// Name:        wxchar.cpp
// Purpose:     wxChar implementation
// Author:      Ove K�ven
// Modified by:
// Created:     09/04/99
// RCS-ID:      $Id$
// Copyright:   (c) wxWindows copyright
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
  #pragma implementation "wxchar.h"
#endif

// ===========================================================================
// headers, declarations, constants
// ===========================================================================

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#define _ISOC9X_SOURCE 1 // to get vsscanf()
#define _BSD_SOURCE    1 // to still get strdup()

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#ifndef WX_PRECOMP
  #include "wx/defs.h"
  #include "wx/wxchar.h"
  #include "wx/string.h"
  #include "wx/hash.h"
#endif

#if defined(__WIN32__) && defined(wxNEED_WX_CTYPE_H)
  #include <windef.h>
  #include <winbase.h>
  #include <winnls.h>
  #include <winnt.h>
#endif

#if wxUSE_WCHAR_T
size_t WXDLLEXPORT wxMB2WC(wchar_t *buf, const char *psz, size_t n)
{
  if (buf) {
    if (!n || !*psz) {
      if (n) *buf = wxT('\0');
      return 0;
    }
    return mbstowcs(buf, psz, n);
  }

  // assume that we have mbsrtowcs() too if we have wcsrtombs()
#ifdef HAVE_WCSRTOMBS
  mbstate_t mbstate;
  return mbsrtowcs((wchar_t *) NULL, &psz, 0, &mbstate);
#else  // !GNU libc
  return mbstowcs((wchar_t *) NULL, psz, 0);
#endif // GNU
}

size_t WXDLLEXPORT wxWC2MB(char *buf, const wchar_t *pwz, size_t n)
{
  if (buf) {
    if (!n || !*pwz) {
      // glibc2.1 chokes on null input
      if (n) *buf = '\0';
      return 0;
    }
    return wcstombs(buf, pwz, n);
  }

#if HAVE_WCSRTOMBS
  mbstate_t mbstate;
  return wcsrtombs((char *) NULL, &pwz, 0, &mbstate);
#else  // !GNU libc
  return wcstombs((char *) NULL, pwz, 0);
#endif // GNU
}
#endif // wxUSE_WCHAR_T

bool WXDLLEXPORT wxOKlibc()
{
#if wxUSE_WCHAR_T && defined(__UNIX__) && defined(__GLIBC__)
  // glibc 2.0 uses UTF-8 even when it shouldn't
  wchar_t res = 0;
  if ((MB_CUR_MAX == 2) &&
      (wxMB2WC(&res, "\xdd\xa5", 1) == 1) &&
      (res==0x765)) {
    // this is UTF-8 allright, check whether that's what we want
    char *cur_locale = setlocale(LC_CTYPE, NULL);
    if ((strlen(cur_locale) < 4) ||
            (strcasecmp(cur_locale + strlen(cur_locale) - 4, "utf8")) ||
            (strcasecmp(cur_locale + strlen(cur_locale) - 5, "utf-8"))) {
      // nope, don't use libc conversion
      return FALSE;
    }
  }
#endif
  return TRUE;
}

#ifndef HAVE_WCSLEN
size_t   WXDLLEXPORT wcslen(const wchar_t *s)
{
  size_t len = 0;
  while (s[len]) len++;
  return len;
}
#endif

// ============================================================================
// printf() functions business
// ============================================================================

// ----------------------------------------------------------------------------
// implement [v]snprintf() if the system doesn't provide a safe one
// ----------------------------------------------------------------------------

#if !defined(wxVsnprintf_)
int WXDLLEXPORT wxVsnprintf_(wxChar *buf, size_t lenMax,
                             const wxChar *format, va_list argptr)
{
    // buffer to avoid dynamic memory allocation each time for small strings
    char szScratch[1024];

    // number of characters in the buffer so far, must be less than lenMax
    size_t lenCur = 0;

    for (size_t n = 0; format[n]; n++)
    {
        if (format[n] == wxT('%')) {
            static char s_szFlags[256] = "%";
            size_t flagofs = 1;
            bool adj_left = FALSE,
                 in_prec = FALSE,
                 prec_dot = FALSE,
                 done = FALSE;
            int ilen = 0;
            size_t min_width = 0,
                   max_width = wxSTRING_MAXLEN;
            do {

#define CHECK_PREC \
                if (in_prec && !prec_dot) \
                { \
                    s_szFlags[flagofs++] = '.'; \
                    prec_dot = TRUE; \
                }

#define APPEND_CH(ch) \
                if ( lenCur == lenMax ) \
                    return -1; \
                \
                buf[lenCur++] = ch

#define APPEND_STR(s) \
                for ( const char *p = s; *p; p++ ) \
                { \
                    APPEND_CH((wchar_t)(*p)); \
                }

#define APPEND_WSTR(s) \
                for ( const wchar_t *p = s; *p; p++ ) \
                { \
                    APPEND_CH(*p); \
                }

                switch (format[++n]) {
                    case wxT('\0'):
                        APPEND_CH(_T('\0'));

                        done = TRUE;
                        break;

                    case wxT('%'):
                        APPEND_CH(_T('%'));
                        done = TRUE;
                        break;

                    case wxT('#'):
                    case wxT('0'):
                    case wxT(' '):
                    case wxT('+'):
                    case wxT('\''):
                        CHECK_PREC
                        s_szFlags[flagofs++] = format[n];
                        break;

                    case wxT('-'):
                        CHECK_PREC
                        adj_left = TRUE;
                        s_szFlags[flagofs++] = format[n];
                        break;

                    case wxT('.'):
                        CHECK_PREC
                            in_prec = TRUE;
                        prec_dot = FALSE;
                        max_width = 0;
                        // dot will be auto-added to s_szFlags if non-negative number follows
                        break;

                    case wxT('h'):
                        ilen = -1;
                        CHECK_PREC
                        s_szFlags[flagofs++] = format[n];
                        break;

                    case wxT('l'):
                        ilen = 1;
                        CHECK_PREC
                        s_szFlags[flagofs++] = format[n];
                        break;

                    case wxT('q'):
                    case wxT('L'):
                        ilen = 2;
                        CHECK_PREC
                        s_szFlags[flagofs++] = format[n];
                        break;

                    case wxT('Z'):
                        ilen = 3;
                        CHECK_PREC
                        s_szFlags[flagofs++] = format[n];
                        break;

                    case wxT('*'):
                        {
                            int len = va_arg(argptr, int);
                            if (in_prec) {
                                if (len<0) break;
                                CHECK_PREC
                                    max_width = len;
                            } else {
                                if (len<0) {
                                    adj_left = !adj_left;
                                    s_szFlags[flagofs++] = '-';
                                    len = -len;
                                }
                                min_width = len;
                            }
                            flagofs += ::sprintf(s_szFlags+flagofs,"%d",len);
                        }
                        break;

                    case wxT('1'): case wxT('2'): case wxT('3'):
                    case wxT('4'): case wxT('5'): case wxT('6'):
                    case wxT('7'): case wxT('8'): case wxT('9'):
                        {
                            int len = 0;
                            CHECK_PREC
                                while ((format[n]>=wxT('0')) && (format[n]<=wxT('9'))) {
                                    s_szFlags[flagofs++] = format[n];
                                    len = len*10 + (format[n] - wxT('0'));
                                    n++;
                                }
                            if (in_prec) max_width = len;
                            else min_width = len;
                            n--; // the main loop pre-increments n again
                        }
                        break;

                    case wxT('d'):
                    case wxT('i'):
                    case wxT('o'):
                    case wxT('u'):
                    case wxT('x'):
                    case wxT('X'):
                        CHECK_PREC
                        s_szFlags[flagofs++] = format[n];
                        s_szFlags[flagofs] = '\0';
                        if (ilen == 0 ) {
                            int val = va_arg(argptr, int);
                            ::sprintf(szScratch, s_szFlags, val);
                        }
                        else if (ilen == -1) {
                            short int val = va_arg(argptr, short int);
                            ::sprintf(szScratch, s_szFlags, val);
                        }
                        else if (ilen == 1) {
                            long int val = va_arg(argptr, long int);
                            ::sprintf(szScratch, s_szFlags, val);
                        }
                        else if (ilen == 2) {
#if SIZEOF_LONG_LONG
                            long long int val = va_arg(argptr, long long int);
                            ::sprintf(szScratch, s_szFlags, val);
#else
                            long int val = va_arg(argptr, long int);
                            ::sprintf(szScratch, s_szFlags, val);
#endif
                        }
                        else if (ilen == 3) {
                            size_t val = va_arg(argptr, size_t);
                            ::sprintf(szScratch, s_szFlags, val);
                        }

                        APPEND_STR(szScratch);

                        done = TRUE;
                        break;

                    case wxT('e'):
                    case wxT('E'):
                    case wxT('f'):
                    case wxT('g'):
                    case wxT('G'):
                        CHECK_PREC
                        s_szFlags[flagofs++] = format[n];
                        s_szFlags[flagofs] = '\0';
                        if (ilen == 2) {
                            long double val = va_arg(argptr, long double);
                            ::sprintf(szScratch, s_szFlags, val);
                        } else {
                            double val = va_arg(argptr, double);
                            ::sprintf(szScratch, s_szFlags, val);
                        }

                        APPEND_STR(szScratch);

                        done = TRUE;
                        break;

                    case wxT('p'):
                        {
                            void *val = va_arg(argptr, void *);
                            CHECK_PREC
                                s_szFlags[flagofs++] = format[n];
                            s_szFlags[flagofs] = '\0';
                            ::sprintf(szScratch, s_szFlags, val);

                            APPEND_STR(szScratch);

                            done = TRUE;
                        }
                        break;

                    case wxT('c'):
                        {
                            wxChar val = va_arg(argptr, int);
                            // we don't need to honor padding here, do we?
                            APPEND_CH(val);

                            done = TRUE;
                        }
                        break;

                    case wxT('s'):
                        if (ilen == -1) {
                            // wx extension: we'll let %hs mean non-Unicode strings
                            char *val = va_arg(argptr, char *);
#if wxUSE_UNICODE
                            // ASCII->Unicode constructor handles max_width right
                            wxString s(val, wxConvLibc, max_width);
#else
                            size_t len = wxSTRING_MAXLEN;
                            if (val) {
                                for (len = 0; val[len] && (len<max_width); len++);
                            } else val = wxT("(null)");
                            wxString s(val, len);
#endif
                            if (s.Len() < min_width)
                                s.Pad(min_width - s.Len(), wxT(' '), adj_left);

                            APPEND_WSTR(s);
                        } else {
                            wxChar *val = va_arg(argptr, wxChar *);
                            size_t len = wxSTRING_MAXLEN;
                            if (val) {
                                for (len = 0; val[len] && (len<max_width); len++);
                            } else val = wxT("(null)");
                            wxString s(val, len);
                            if (s.Len() < min_width)
                                s.Pad(min_width - s.Len(), wxT(' '), adj_left);

                            APPEND_WSTR(s);
                        }
                        done = TRUE;
                        break;

                    case wxT('n'):
                        if (ilen == 0) {
                            int *val = va_arg(argptr, int *);
                            *val = lenCur;
                        }
                        else if (ilen == -1) {
                            short int *val = va_arg(argptr, short int *);
                            *val = lenCur;
                        }
                        else if (ilen >= 1) {
                            long int *val = va_arg(argptr, long int *);
                            *val = lenCur;
                        }
                        done = TRUE;
                        break;

                    default:
                        if (wxIsalpha(format[n]))
                            // probably some flag not taken care of here yet
                            s_szFlags[flagofs++] = format[n];
                        else {
                            // bad format
                            APPEND_CH(_T('%')); // just to pass the glibc tst-printf.c
                            n--;
                            done = TRUE;
                        }
                        break;
                }
            } while (!done);
        }
        else
        {
            APPEND_CH(format[n]);
        }
    }

    return lenCur;
}

#undef APPEND_CH
#undef APPEND_STR
#undef CHECK_PREC

#endif // !wxVsnprintfA

#if !defined(wxSnprintf_)
int WXDLLEXPORT wxSnprintf_(wxChar *buf, size_t len, const wxChar *format, ...)
{
    va_list argptr;
    va_start(argptr, format);

    int iLen = wxVsnprintf_(buf, len, format, argptr);

    va_end(argptr);

    return iLen;
}
#endif // wxSnprintf_

// ----------------------------------------------------------------------------
// implement the standard IO functions for wide char if libc doesn't have them
// ----------------------------------------------------------------------------

#ifndef HAVE_FPUTWC

int wxFputs(const wchar_t *ws, FILE *stream)
{
    // counting the number of wide characters written isn't worth the trouble,
    // simply distinguish between ok and error
    return fputs(wxConvLibc.cWC2MB(ws), stream) == -1 ? -1 : 0;
}

int /* not wint_t */ wxPutc(wchar_t wc, FILE *stream)
{
    wchar_t ws[2] = { wc, L'\0' };

    return wxFputs(ws, stream);
}

#endif // HAVE_FPUTWC

// NB: we only implement va_list functions here, the ones taking ... are
//     defined below for wxNEED_PRINTF_CONVERSION case anyhow and we reuse
//     the definitions there to avoid duplicating them here
#ifdef wxNEED_WPRINTF

// TODO: implement the scanf() functions
int vwscanf(const wchar_t *format, va_list argptr)
{
    wxFAIL_MSG( _T("TODO") );

    return -1;
}

int vswscanf(const wchar_t *ws, const wchar_t *format, va_list argptr)
{
    wxFAIL_MSG( _T("TODO") );

    return -1;
}

int vfwscanf(FILE *stream, const wchar_t *format, va_list argptr)
{
    wxFAIL_MSG( _T("TODO") );

    return -1;
}

#define vswprintf wxVsnprintf_

int vfwprintf(FILE *stream, const wchar_t *format, va_list argptr)
{
    wxString s;
    int rc = s.PrintfV(format, argptr);

    if ( rc != -1 )
    {
        // we can't do much better without Unicode support in libc...
        if ( fprintf(stream, s.mb_str()) == -1 )
            return -1;
    }

    return rc;
}

int vwprintf(const wchar_t *format, va_list argptr)
{
    return wxVfprintf(stdout, format, argptr);
}

#endif // wxNEED_WPRINTF

#ifdef wxNEED_PRINTF_CONVERSION

// ----------------------------------------------------------------------------
// wxFormatConverter: class doing the "%s" -> "%ls" conversion
// ----------------------------------------------------------------------------

/*
   Here are the gory details. We want to follow the Windows/MS conventions,
   that is to have

   In ANSI mode:

   format specifier         results in
   -----------------------------------
   %c, %hc, %hC             char
   %lc, %C, %lC             wchar_t

   In Unicode mode:

   format specifier         results in
   -----------------------------------
   %hc, %C, %hC             char
   %c, %lc, %lC             wchar_t


   while on POSIX systems we have %C identical to %lc and %c always means char
   (in any mode) while %lc always means wchar_t,

   So to use native functions in order to get our semantics we must do the
   following translations in Unicode mode (nothing to do in ANSI mode):

   wxWindows specifier      POSIX specifier
   ----------------------------------------

   %hc, %C, %hC             %c
   %c                       %lc


   And, of course, the same should be done for %s as well.
*/

class wxFormatConverter
{
public:
    wxFormatConverter(const wxChar *format);

    operator const wxChar *() const { return m_fmt.c_str(); }

private:
    // copy another character to the translated format: this function does the
    // copy if we are translating but doesn't do anything at all if we don't,
    // so we don't create the translated format string at all unless we really
    // need to (i.e. InsertFmtChar() is called)
    wxChar CopyFmtChar(wxChar ch)
    {
        if ( !m_fmtOrig )
        {
            // we're translating, d copy
            m_fmt += ch;
        }
        else
        {
            // simply increase the count which should be copied by
            // CopyAllBefore() later if needed
            m_nCopied++;
        }

        return ch;
    }

    // insert an extra character
    void InsertFmtChar(wxChar ch)
    {
        if ( m_fmtOrig )
        {
            // so far we haven't translated anything yet
            CopyAllBefore();
        }

        m_fmt += ch;
    }

    void CopyAllBefore()
    {
        wxASSERT_MSG( m_fmtOrig && m_fmt.empty(), _T("logic error") );

        m_fmt = wxString(m_fmtOrig, m_nCopied);

        // we won't need it any longer
        m_fmtOrig = NULL;
    }

    static bool IsFlagChar(wxChar ch)
    {
        return ch == _T('-') || ch == _T('+') ||
               ch == _T('0') || ch == _T(' ') || ch == _T('#');
    }

    void SkipDigits(const wxChar **ppc)
    {
        while ( **ppc >= _T('0') && **ppc <= _T('9') )
            CopyFmtChar(*(*ppc)++);
    }

    // the translated format
    wxString m_fmt;

    // the original format
    const wxChar *m_fmtOrig;

    // the number of characters already copied
    size_t m_nCopied;
};

wxFormatConverter::wxFormatConverter(const wxChar *format)
{
    m_fmtOrig = format;
    m_nCopied = 0;

    while ( *format )
    {
        if ( CopyFmtChar(*format++) == _T('%') )
        {
            // skip any flags
            while ( IsFlagChar(*format) )
                CopyFmtChar(*format++);

            // and possible width
            if ( *format == _T('*') )
                CopyFmtChar(*format++);
            else
                SkipDigits(&format);

            // precision?
            if ( *format == _T('.') )
            {
                SkipDigits(&format);
            }

            // next we can have a size modifier
            enum
            {
                Default,
                Short,
                Long
            } size;

            switch ( *format )
            {
                case _T('h'):
                    size = Short;
                    format++;
                    break;

                case _T('l'):
                    // "ll" has a different meaning!
                    if ( format[1] != _T('l') )
                    {
                        size = Long;
                        format++;
                        break;
                    }
                    //else: fall through

                default:
                    size = Default;
            }

            // and finally we should have the type
            switch ( *format )
            {
                case _T('C'):
                case _T('S'):
                    // %C and %hC -> %c and %lC -> %lc
                    if ( size == Long )
                        CopyFmtChar(_T('l'));

                    InsertFmtChar(*format++ == _T('C') ? _T('c') : _T('s'));
                    break;

                case _T('c'):
                case _T('s'):
                    // %c -> %lc but %hc stays %hc and %lc is still %lc
                    switch ( size )
                    {
                        case Default:
                            InsertFmtChar(_T('l'));
                            break;

                        case Short:
                            CopyFmtChar(_T('h'));
                            break;

                        case Long:
                            ;
                    }
                    // fall through

                default:
                    // nothing special to do
                    CopyFmtChar(*format++);
            }
        }
    }
}

#else // !wxNEED_PRINTF_CONVERSION
    // no conversion necessary
    #define wxFormatConverter(x) (x)
#endif // wxNEED_PRINTF_CONVERSION/!wxNEED_PRINTF_CONVERSION

// ----------------------------------------------------------------------------
// wxPrintf(), wxScanf() and relatives
// ----------------------------------------------------------------------------

#if defined(wxNEED_PRINTF_CONVERSION) || defined(wxNEED_WPRINTF)

int wxScanf( const wxChar *format, ... ) ATTRIBUTE_PRINTF_2
{
    va_list argptr;
    va_start(argptr, format);

    int ret = vwscanf(wxFormatConverter(format), argptr );

    va_end(argptr);

    return ret;
}

int wxSscanf( const wxChar *str, const wxChar *format, ... ) ATTRIBUTE_PRINTF_3
{
    va_list argptr;
    va_start(argptr, format);

    int ret = vswscanf( str, wxFormatConverter(format), argptr );

    va_end(argptr);

    return ret;
}

int wxFscanf( FILE *stream, const wxChar *format, ... ) ATTRIBUTE_PRINTF_3
{
    va_list argptr;
    va_start(argptr, format);

    int ret = vfwscanf(stream, wxFormatConverter(format), argptr);

    va_end(argptr);

    return ret;
}

int wxPrintf( const wxChar *format, ... ) ATTRIBUTE_PRINTF_2
{
    va_list argptr;
    va_start(argptr, format);

    int ret = vwprintf( wxFormatConverter(format), argptr );

    va_end(argptr);

    return ret;
}

#ifndef wxSnprintf
int wxSnprintf( wxChar *str, size_t size, const wxChar *format, ... ) ATTRIBUTE_PRINTF_4
{
    va_list argptr;
    va_start(argptr, format);

    int ret = vswprintf( str, size, wxFormatConverter(format), argptr );

    va_end(argptr);

    return ret;
}
#endif // wxSnprintf

int wxSprintf( wxChar *str, const wxChar *format, ... ) ATTRIBUTE_PRINTF_3
{
    va_list argptr;
    va_start(argptr, format);

    // callers of wxSprintf() deserve what they get
    int ret = vswprintf( str, UINT_MAX, wxFormatConverter(format), argptr );

    va_end(argptr);

    return ret;
}

int wxFprintf( FILE *stream, const wxChar *format, ... ) ATTRIBUTE_PRINTF_3
{
    va_list argptr;
    va_start( argptr, format );

    int ret = vfwprintf( stream, wxFormatConverter(format), argptr );

    va_end(argptr);

    return ret;
}

int wxVsscanf( const wxChar *str, const wxChar *format, va_list argptr )
{
    return vswscanf( str, wxFormatConverter(format), argptr );
}

int wxVfprintf( FILE *stream, const wxChar *format, va_list argptr )
{
    return vfwprintf( stream, wxFormatConverter(format), argptr );
}

int wxVprintf( const wxChar *format, va_list argptr )
{
    return vwprintf( wxFormatConverter(format), argptr );
}

#ifndef wxVsnprintf
int wxVsnprintf( wxChar *str, size_t size, const wxChar *format, va_list argptr )
{
    return vswprintf( str, size, wxFormatConverter(format), argptr );
}
#endif // wxVsnprintf

int wxVsprintf( wxChar *str, const wxChar *format, va_list argptr )
{
    // same as for wxSprintf()
    return vswprintf(str, UINT_MAX, wxFormatConverter(format), argptr);
}

#endif // wxNEED_PRINTF_CONVERSION

// ----------------------------------------------------------------------------
// ctype.h stuff (currently unused)
// ----------------------------------------------------------------------------

#if defined(__WIN32__) && defined(wxNEED_WX_CTYPE_H)
inline WORD wxMSW_ctype(wxChar ch)
{
  WORD ret;
  GetStringTypeEx(LOCALE_USER_DEFAULT, CT_CTYPE1, &ch, 1, &ret);
  return ret;
}

WXDLLEXPORT int wxIsalnum(wxChar ch) { return IsCharAlphaNumeric(ch); }
WXDLLEXPORT int wxIsalpha(wxChar ch) { return IsCharAlpha(ch); }
WXDLLEXPORT int wxIsctrl(wxChar ch) { return wxMSW_ctype(ch) & C1_CNTRL; }
WXDLLEXPORT int wxIsdigit(wxChar ch) { return wxMSW_ctype(ch) & C1_DIGIT; }
WXDLLEXPORT int wxIsgraph(wxChar ch) { return wxMSW_ctype(ch) & (C1_DIGIT|C1_PUNCT|C1_ALPHA); }
WXDLLEXPORT int wxIslower(wxChar ch) { return IsCharLower(ch); }
WXDLLEXPORT int wxIsprint(wxChar ch) { return wxMSW_ctype(ch) & (C1_DIGIT|C1_SPACE|C1_PUNCT|C1_ALPHA); }
WXDLLEXPORT int wxIspunct(wxChar ch) { return wxMSW_ctype(ch) & C1_PUNCT; }
WXDLLEXPORT int wxIsspace(wxChar ch) { return wxMSW_ctype(ch) & C1_SPACE; }
WXDLLEXPORT int wxIsupper(wxChar ch) { return IsCharUpper(ch); }
WXDLLEXPORT int wxIsxdigit(wxChar ch) { return wxMSW_ctype(ch) & C1_XDIGIT; }
WXDLLEXPORT int wxTolower(wxChar ch) { return (wxChar)CharLower((LPTSTR)(ch)); }
WXDLLEXPORT int wxToupper(wxChar ch) { return (wxChar)CharUpper((LPTSTR)(ch)); }
#endif

#ifndef wxStrdup
WXDLLEXPORT wxChar * wxStrdup(const wxChar *psz)
{
  size_t size = (wxStrlen(psz) + 1) * sizeof(wxChar);
  wxChar *ret = (wxChar *) malloc(size);
  memcpy(ret, psz, size);
  return ret;
}
#endif

#ifndef wxStricmp
int WXDLLEXPORT wxStricmp(const wxChar *psz1, const wxChar *psz2)
{
  register wxChar c1, c2;
  do {
    c1 = wxTolower(*psz1++);
    c2 = wxTolower(*psz2++);
  } while ( c1 && (c1 == c2) );
  return c1 - c2;
}
#endif

#ifndef wxStricmp
int WXDLLEXPORT wxStrnicmp(const wxChar *s1, const wxChar *s2, size_t n)
{
  register wxChar c1, c2;
  while (n && ((c1 = wxTolower(*s1)) == (c2 = wxTolower(*s2)) ) && c1) n--, s1++, s2++;
  if (n) {
    if (c1 < c2) return -1;
    if (c1 > c2) return 1;
  }
  return 0;
}
#endif

#ifndef wxStrtok
WXDLLEXPORT wxChar * wxStrtok(wxChar *psz, const wxChar *delim, wxChar **save_ptr)
{
  if (!psz) psz = *save_ptr;
  psz += wxStrspn(psz, delim);
  if (!*psz) {
    *save_ptr = (wxChar *)NULL;
    return (wxChar *)NULL;
  }
  wxChar *ret = psz;
  psz = wxStrpbrk(psz, delim);
  if (!psz) *save_ptr = (wxChar*)NULL;
  else {
    *psz = wxT('\0');
    *save_ptr = psz + 1;
  }
  return ret;
}
#endif

#ifndef wxSetlocale
WXDLLEXPORT wxWCharBuffer wxSetlocale(int category, const wxChar *locale)
{
    char *localeOld = setlocale(category, wxConvLocal.cWX2MB(locale));

    return wxWCharBuffer(wxConvLocal.cMB2WC(localeOld));
}
#endif

// ----------------------------------------------------------------------------
// string.h functions
// ----------------------------------------------------------------------------

#ifdef wxNEED_WX_STRING_H
WXDLLEXPORT wxChar * wxStrcat(wxChar *dest, const wxChar *src)
{
  wxChar *ret = dest;
  while (*dest) dest++;
  while ((*dest++ = *src++));
  return ret;
}

WXDLLEXPORT const wxChar * wxStrchr(const wxChar *s, wxChar c)
{
    // be careful here as the terminating NUL makes part of the string
    while ( *s != c )
    {
        if ( !*s++ )
            return NULL;
    }

    return s;
}

WXDLLEXPORT int wxStrcmp(const wxChar *s1, const wxChar *s2)
{
  while ((*s1 == *s2) && *s1) s1++, s2++;
  if ((wxUChar)*s1 < (wxUChar)*s2) return -1;
  if ((wxUChar)*s1 > (wxUChar)*s2) return 1;
  return 0;
}

WXDLLEXPORT wxChar * wxStrcpy(wxChar *dest, const wxChar *src)
{
  wxChar *ret = dest;
  while ((*dest++ = *src++));
  return ret;
}

WXDLLEXPORT wxChar * wxStrncat(wxChar *dest, const wxChar *src, size_t n)
{
  wxChar *ret = dest;
  while (*dest) dest++;
  while (n && (*dest++ = *src++)) n--;
  return ret;
}

WXDLLEXPORT int wxStrncmp(const wxChar *s1, const wxChar *s2, size_t n)
{
  while (n && (*s1 == *s2) && *s1) n--, s1++, s2++;
  if (n) {
    if ((wxUChar)*s1 < (wxUChar)*s2) return -1;
    if ((wxUChar)*s1 > (wxUChar)*s2) return 1;
  }
  return 0;
}

WXDLLEXPORT wxChar * wxStrncpy(wxChar *dest, const wxChar *src, size_t n)
{
  wxChar *ret = dest;
  while (n && (*dest++ = *src++)) n--;
  while (n) *dest++=0, n--; // the docs specify padding with zeroes
  return ret;
}

WXDLLEXPORT const wxChar * wxStrpbrk(const wxChar *s, const wxChar *accept)
{
  while (*s && !wxStrchr(accept, *s))
      s++;

  return *s ? s : NULL;
}

WXDLLEXPORT const wxChar * wxStrrchr(const wxChar *s, wxChar c)
{
    const wxChar *ret = NULL;
    do
    {
        if ( *s == c )
            ret = s;
        s++;
    }
    while ( *s );

    return ret;
}

WXDLLEXPORT size_t wxStrspn(const wxChar *s, const wxChar *accept)
{
  size_t len = 0;
  while (wxStrchr(accept, *s++)) len++;
  return len;
}

WXDLLEXPORT const wxChar *wxStrstr(const wxChar *haystack, const wxChar *needle)
{
    wxCHECK_RET( needle, NULL, _T("NULL argument in wxStrstr") );

    // VZ: this is not exactly the most efficient string search algorithm...

    const size_t len = wxStrlen(needle);

    while ( const wxChar *fnd = wxStrchr(haystack, *needle) )
    {
        if ( !wxStrncmp(fnd, needle, len) )
            return fnd;

        haystack = fnd + 1;
    }

    return NULL;
}

WXDLLEXPORT double wxStrtod(const wxChar *nptr, wxChar **endptr)
{
  const wxChar *start = nptr;

  // FIXME: only correct for C locale
  while (wxIsspace(*nptr)) nptr++;
  if (*nptr == wxT('+') || *nptr == wxT('-')) nptr++;
  while (wxIsdigit(*nptr)) nptr++;
  if (*nptr == wxT('.')) {
    nptr++;
    while (wxIsdigit(*nptr)) nptr++;
  }
  if (*nptr == wxT('E') || *nptr == wxT('e')) {
    nptr++;
    if (*nptr == wxT('+') || *nptr == wxT('-')) nptr++;
    while (wxIsdigit(*nptr)) nptr++;
  }

  wxString data(nptr, nptr-start);
  wxWX2MBbuf dat = data.mb_str(wxConvLocal);
  char *rdat = wxMBSTRINGCAST dat;
  double ret = strtod(dat, &rdat);

  if (endptr) *endptr = (wxChar *)(start + (rdat - (const char *)dat));

  return ret;
}

WXDLLEXPORT long int wxStrtol(const wxChar *nptr, wxChar **endptr, int base)
{
  const wxChar *start = nptr;

  // FIXME: only correct for C locale
  while (wxIsspace(*nptr)) nptr++;
  if (*nptr == wxT('+') || *nptr == wxT('-')) nptr++;
  if (((base == 0) || (base == 16)) &&
      (nptr[0] == wxT('0') && nptr[1] == wxT('x'))) {
    nptr += 2;
    base = 16;
  }
  else if ((base == 0) && (nptr[0] == wxT('0'))) base = 8;
  else if (base == 0) base = 10;

  while ((wxIsdigit(*nptr) && (*nptr - wxT('0') < base)) ||
         (wxIsalpha(*nptr) && (wxToupper(*nptr) - wxT('A') + 10 < base))) nptr++;

  wxString data(nptr, nptr-start);
  wxWX2MBbuf dat = data.mb_str(wxConvLocal);
  char *rdat = wxMBSTRINGCAST dat;
  long int ret = strtol(dat, &rdat, base);

  if (endptr) *endptr = (wxChar *)(start + (rdat - (const char *)dat));

  return ret;
}
#endif // wxNEED_WX_STRING_H

#ifdef wxNEED_WX_STDIO_H
WXDLLEXPORT FILE * wxFopen(const wxChar *path, const wxChar *mode)
{
    char mode_buffer[10];
    for (size_t i = 0; i < wxStrlen(mode)+1; i++)
       mode_buffer[i] = (char) mode[i];

    return fopen( wxConvFile.cWX2MB(path), mode_buffer );
}

WXDLLEXPORT FILE * wxFreopen(const wxChar *path, const wxChar *mode, FILE *stream)
{
    char mode_buffer[10];
    for (size_t i = 0; i < wxStrlen(mode)+1; i++)
       mode_buffer[i] = (char) mode[i];

    return freopen( wxConvFile.cWX2MB(path), mode_buffer, stream );
}

WXDLLEXPORT int wxRemove(const wxChar *path)
{
    return remove( wxConvFile.cWX2MB(path) );
}

WXDLLEXPORT int wxRename(const wxChar *oldpath, const wxChar *newpath)
{
    return rename( wxConvFile.cWX2MB(oldpath), wxConvFile.cWX2MB(newpath) );
}
#endif

#ifndef wxAtof
double   WXDLLEXPORT wxAtof(const wxChar *psz)
{
  return atof(wxConvLocal.cWX2MB(psz));
}
#endif

#ifdef wxNEED_WX_STDLIB_H
int      WXDLLEXPORT wxAtoi(const wxChar *psz)
{
  return atoi(wxConvLocal.cWX2MB(psz));
}

long     WXDLLEXPORT wxAtol(const wxChar *psz)
{
  return atol(wxConvLocal.cWX2MB(psz));
}

wxChar * WXDLLEXPORT wxGetenv(const wxChar *name)
{
  static wxHashTable env;

  // check if we already have stored the converted env var
  wxObject *data = env.Get(name);
  if (!data)
  {
    // nope, retrieve it,
#if wxUSE_UNICODE
    wxCharBuffer buffer = wxConvLocal.cWX2MB(name);
    // printf( "buffer %s\n", (const char*) buffer );
    const char *val = getenv( (const char *)buffer );
#else
    const char *val = getenv( name );
#endif

    if (!val) return (wxChar *)NULL;
    // printf( "home %s\n", val );

    // convert it,
#ifdef wxUSE_UNICODE
    data = (wxObject *)new wxString(val, wxConvLocal);
#else
    data = (wxObject *)new wxString(val);
#endif

    // and store it
    env.Put(name, data);
  }
  // return converted env var
  return (wxChar *)((wxString *)data)->c_str();
}

int WXDLLEXPORT wxSystem(const wxChar *psz)
{
    return system(wxConvLocal.cWX2MB(psz));
}

#endif

#ifdef wxNEED_WX_TIME_H
WXDLLEXPORT size_t   wxStrftime(wxChar *s, size_t max, const wxChar *fmt, const struct tm *tm)
{
    if (!max) return 0;

    char *buf = (char *)malloc(max);
    size_t ret = strftime(buf, max, wxConvLocal.cWX2MB(fmt), tm);
    if (ret)
    {
        wxStrcpy(s, wxConvLocal.cMB2WX(buf));
        free(buf);
        return wxStrlen(s);
    }
    else
    {
        free(buf);
        *s = 0;
        return 0;
  }
}
#endif
