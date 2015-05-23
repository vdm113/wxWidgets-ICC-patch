/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/encconv.cpp
// Purpose:     wxEncodingConverter class for converting between different
//              font encodings
// Author:      Vaclav Slavik
// Copyright:   (c) 1999 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#include "wx/encconv.h"

#include <stdlib.h>

// conversion tables, generated by scripts in $(WXWIN)/misc/unictabl:
#if defined( __BORLANDC__ ) || defined(__DARWIN__)
    #include "../common/unictabl.inc"
#else
    #include "unictabl.inc"
#endif

#ifdef __WXMAC__
    #include "wx/osx/core/cfstring.h"
    #include <CoreFoundation/CFStringEncodingExt.h>

    wxUint16 gMacEncodings[wxFONTENCODING_MACMAX-wxFONTENCODING_MACMIN+1][128] ;
    bool gMacEncodingsInited[wxFONTENCODING_MACMAX-wxFONTENCODING_MACMIN+1] ;
#endif

#ifdef __WXWINCE__
    #include "wx/msw/wince/missing.h"       // for bsearch()
#endif

static const wxUint16* GetEncTable(wxFontEncoding enc)
{
#ifdef __WXMAC__
    if( enc >= wxFONTENCODING_MACMIN && enc <= wxFONTENCODING_MACMAX )
    {
        int i = enc-wxFONTENCODING_MACMIN ;
        if ( gMacEncodingsInited[i] == false )
        {
            // create
            CFStringEncoding cfencoding = wxMacGetSystemEncFromFontEnc( enc ) ;
            if( !CFStringIsEncodingAvailable( cfencoding ) )
                return NULL;

            memset( gMacEncodings[i] , 0 , 128 * 2 );
            char s[2] = { 0 , 0 };
            CFRange firstchar = CFRangeMake( 0, 1 );
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
            for( unsigned char c = 255 ; c >= 128 ; --c )
            {
                s[0] = c ;
                wxCFStringRef cfref( CFStringCreateWithCStringNoCopy( NULL, s, cfencoding , kCFAllocatorNull ) );
                CFStringGetCharacters( cfref, firstchar, (UniChar*)  &gMacEncodings[i][c-128] );
            }
            gMacEncodingsInited[i]=true;
        }
        return gMacEncodings[i] ;
    }
#endif

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
    for (int i = 0; encodings_list[i].table != NULL; i++)
    {
        if (encodings_list[i].encoding == enc)
            return encodings_list[i].table;
    }
    return NULL;
}

typedef struct {
    wxUint16 u;
    wxUint8  c;
} CharsetItem;

extern "C"
{
static int wxCMPFUNC_CONV
CompareCharsetItems(const void *i1, const void *i2)
{
    return ( ((CharsetItem*)i1) -> u - ((CharsetItem*)i2) -> u );
}
}

static CharsetItem* BuildReverseTable(const wxUint16 *tbl)
{
    CharsetItem *rev = new CharsetItem[128];

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
    for (int i = 0; i < 128; i++)
        rev[i].c = wxUint8(128 + i), rev[i].u = tbl[i];

    qsort(rev, 128, sizeof(CharsetItem), CompareCharsetItems);

    return rev;
}



wxEncodingConverter::wxEncodingConverter()
{
    m_Table = NULL;
    m_UnicodeInput = m_UnicodeOutput = false;
    m_JustCopy = false;
}



bool wxEncodingConverter::Init(wxFontEncoding input_enc, wxFontEncoding output_enc, int method)
{
    unsigned i;
    const wxUint16 *in_tbl;
    const wxUint16 *out_tbl = NULL;

    wxDELETEA(m_Table);

    if (input_enc == output_enc) {m_JustCopy = true; return true;}

    m_UnicodeOutput = (output_enc == wxFONTENCODING_UNICODE);
    m_JustCopy = false;

    if (input_enc == wxFONTENCODING_UNICODE)
    {
        if ((out_tbl = GetEncTable(output_enc)) == NULL) return false;

        m_Table = new wchar_t[65536];
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
        for (i = 0; i < 128; i++)  m_Table[i] = (wchar_t)i; // 7bit ASCII
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
        for (i = 128; i < 65536; i++)  m_Table[i] = (wchar_t)0;

        if (method == wxCONVERT_SUBSTITUTE)
        {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
            for (i = 0; i < encoding_unicode_fallback_count; i++)
                m_Table[encoding_unicode_fallback[i].c] = (wchar_t) encoding_unicode_fallback[i].s;
        }

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
        for (i = 0; i < 128; i++)
            m_Table[out_tbl[i]] = (wchar_t)(128 + i);

        m_UnicodeInput = true;
    }
    else // input !Unicode
    {
        if ((in_tbl = GetEncTable(input_enc)) == NULL) return false;
        if (output_enc != wxFONTENCODING_UNICODE)
            if ((out_tbl = GetEncTable(output_enc)) == NULL) return false;

        m_UnicodeInput = false;

        m_Table = new wchar_t[256];
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
        for (i = 0; i < 128; i++)  m_Table[i] = (wchar_t)i; // 7bit ASCII

        if (output_enc == wxFONTENCODING_UNICODE)
        {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
            for (i = 0; i < 128; i++)  m_Table[128 + i] = (wchar_t)in_tbl[i];
            return true;
        }
        else // output !Unicode
        {
            CharsetItem *rev = BuildReverseTable(out_tbl);
            CharsetItem *item;
            CharsetItem key;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
            for (i = 0; i < 128; i++)
            {
                key.u = in_tbl[i];
                item = (CharsetItem*) bsearch(&key, rev, 128, sizeof(CharsetItem), CompareCharsetItems);
                if (item == NULL && method == wxCONVERT_SUBSTITUTE)
                    item = (CharsetItem*) bsearch(&key, encoding_unicode_fallback,
                                encoding_unicode_fallback_count, sizeof(CharsetItem), CompareCharsetItems);
                if (item)
                    m_Table[128 + i] = (wchar_t)item -> c;
                else
                    m_Table[128 + i] = (wchar_t)(128 + i);
            }

            delete[] rev;
        }
    }

    return true;
}


#define REPLACEMENT_CHAR  (L'?')

inline wchar_t GetTableValue(const wchar_t *table, wchar_t value, bool& repl)
{
    wchar_t r = table[value];
    if (r == 0 && value != 0)
    {
        r = REPLACEMENT_CHAR;
        repl = true;
    }
    return r;
}


bool wxEncodingConverter::Convert(const char* input, char* output) const
{
    wxASSERT_MSG(!m_UnicodeOutput, wxT("You cannot convert to unicode if output is const char*!"));
    wxASSERT_MSG(!m_UnicodeInput, wxT("You cannot convert from unicode if input is const char*!"));

    const char *i;
    char *o;

    if (m_JustCopy)
    {
        strcpy(output, input);
        return true;
    }

    wxCHECK_MSG(m_Table != NULL, false,
                wxT("You must call wxEncodingConverter::Init() before actually converting!"));

    bool replaced = false;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
    for (i = input, o = output; *i != 0;)
        *(o++) = (char)(GetTableValue(m_Table, (wxUint8)*(i++), replaced));
    *o = 0;

    return !replaced;
}


bool wxEncodingConverter::Convert(const char* input, wchar_t* output) const
{
    wxASSERT_MSG(m_UnicodeOutput, wxT("You cannot convert to 8-bit if output is const wchar_t*!"));
    wxASSERT_MSG(!m_UnicodeInput, wxT("You cannot convert from unicode if input is const char*!"));

    const char *i;
    wchar_t *o;

    if (m_JustCopy)
    {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
        for (i = input, o = output; *i != 0;)
            *(o++) = (wchar_t)(*(i++));
        *o = 0;
        return true;
    }

    wxCHECK_MSG(m_Table != NULL, false,
                wxT("You must call wxEncodingConverter::Init() before actually converting!"));

    bool replaced = false;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
    for (i = input, o = output; *i != 0;)
        *(o++) = (wchar_t)(GetTableValue(m_Table, (wxUint8)*(i++), replaced));
    *o = 0;

    return !replaced;
}



bool wxEncodingConverter::Convert(const wchar_t* input, char* output) const
{
    wxASSERT_MSG(!m_UnicodeOutput, wxT("You cannot convert to unicode if output is const char*!"));
    wxASSERT_MSG(m_UnicodeInput, wxT("You cannot convert from 8-bit if input is const wchar_t*!"));

    const wchar_t *i;
    char *o;

    if (m_JustCopy)
    {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
        for (i = input, o = output; *i != 0;)
            *(o++) = (char)(*(i++));
        *o = 0;
        return true;
    }

    wxCHECK_MSG(m_Table != NULL, false,
                wxT("You must call wxEncodingConverter::Init() before actually converting!"));

    bool replaced = false;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
    for (i = input, o = output; *i != 0;)
        *(o++) = (char)(GetTableValue(m_Table, (wxUint16)*(i++), replaced));
    *o = 0;

    return !replaced;
}



bool wxEncodingConverter::Convert(const wchar_t* input, wchar_t* output) const
{
    wxASSERT_MSG(m_UnicodeOutput, wxT("You cannot convert to 8-bit if output is const wchar_t*!"));
    wxASSERT_MSG(m_UnicodeInput, wxT("You cannot convert from 8-bit if input is const wchar_t*!"));

    const wchar_t *i;
    wchar_t *o;

    if (m_JustCopy)
    {
        // wcscpy() is not guaranteed to exist
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
        for (i = input, o = output; *i != 0;)
            *(o++) = (*(i++));
        *o = 0;
        return true;
    }

    wxCHECK_MSG(m_Table != NULL, false,
                wxT("You must call wxEncodingConverter::Init() before actually converting!"));

    bool replaced = false;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
    for (i = input, o = output; *i != 0;)
        *(o++) = (wchar_t)(GetTableValue(m_Table, (wxUint8)*(i++), replaced));
    *o = 0;

    return !replaced;
}


wxString wxEncodingConverter::Convert(const wxString& input) const
{
    if (m_JustCopy) return input;

    wxString s;
    const wxChar *i;

    wxCHECK_MSG(m_Table != NULL, s,
                wxT("You must call wxEncodingConverter::Init() before actually converting!"));

    if (m_UnicodeInput)
    {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
        for (i = input.c_str(); *i != 0; i++)
            s << (wxChar)(m_Table[(wxUint16)*i]);
    }
    else
    {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
        for (i = input.c_str(); *i != 0; i++)
            s << (wxChar)(m_Table[(wxUint8)*i]);
    }

    return s;
}







// Following tables describe classes of encoding equivalence.
//

#define STOP wxFONTENCODING_SYSTEM

#define NUM_OF_PLATFORMS  3 /*must conform to enum wxPLATFORM_XXXX !!!*/
#define ENC_PER_PLATFORM  3
           // max no. of encodings for one language used on one platform.
           // Using maximum of everything at the current moment to not make the
           // library larger than necessary. Make larger only if necessary - MR

static const wxFontEncoding
    EquivalentEncodings[][NUM_OF_PLATFORMS][ENC_PER_PLATFORM+1] = {

    // *** Please put more common encodings as first! ***

    // Western European
    {
        /* unix    */ {wxFONTENCODING_ISO8859_1, wxFONTENCODING_ISO8859_15, STOP},
        /* windows */ {wxFONTENCODING_CP1252, STOP},
        /* mac     */ {wxFONTENCODING_MACROMAN, STOP}
    },

    // Central European
    {
        /* unix    */ {wxFONTENCODING_ISO8859_2, STOP},
        /* windows */ {wxFONTENCODING_CP1250, STOP},
        /* mac     */ {wxFONTENCODING_MACCENTRALEUR, STOP}
    },

    // Baltic
    {
        /* unix    */ {wxFONTENCODING_ISO8859_13, wxFONTENCODING_ISO8859_4, STOP},
        /* windows */ {wxFONTENCODING_CP1257, STOP},
        /* mac     */ {STOP}
    },

    // Hebrew
    {
        /* unix    */ {wxFONTENCODING_ISO8859_8, STOP},
        /* windows */ {wxFONTENCODING_CP1255, STOP},
        /* mac     */ {wxFONTENCODING_MACHEBREW, STOP}
    },

    // Greek
    {
        /* unix    */ {wxFONTENCODING_ISO8859_7, STOP},
        /* windows */ {wxFONTENCODING_CP1253, STOP},
        /* mac     */ {wxFONTENCODING_MACGREEK, STOP}
    },

    // Arabic
    {
        /* unix    */ {wxFONTENCODING_ISO8859_6, STOP},
        /* windows */ {wxFONTENCODING_CP1256, STOP},
        /* mac     */ {wxFONTENCODING_MACARABIC, STOP}
    },

    // Turkish
    {
        /* unix    */ {wxFONTENCODING_ISO8859_9, STOP},
        /* windows */ {wxFONTENCODING_CP1254, STOP},
        /* mac     */ {wxFONTENCODING_MACTURKISH, STOP}
    },

    // Cyrillic
    {
        /* unix    */ {wxFONTENCODING_KOI8, wxFONTENCODING_KOI8_U, wxFONTENCODING_ISO8859_5, STOP},
        /* windows */ {wxFONTENCODING_CP1251, STOP},
        /* mac     */ {wxFONTENCODING_MACCYRILLIC, STOP}
    },

    {{STOP},{STOP},{STOP}} /* Terminator */
    /* no, _not_ Arnold! */
};


static bool FindEncoding(const wxFontEncodingArray& arr, wxFontEncoding f)
{
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
    for (wxFontEncodingArray::const_iterator it = arr.begin(), en = arr.end();
         it != en; ++it)
        if (*it == f)
            return true;
    return false;
}

wxFontEncodingArray wxEncodingConverter::GetPlatformEquivalents(wxFontEncoding enc, int platform)
{
    if (platform == wxPLATFORM_CURRENT)
    {
#if defined(__WINDOWS__)
        platform = wxPLATFORM_WINDOWS;
#elif defined(__WXMAC__)
        platform = wxPLATFORM_MAC;
#else
        platform = wxPLATFORM_UNIX;
#endif
    }

    switch ( platform )
    {
        case wxPLATFORM_UNIX:
        case wxPLATFORM_WINDOWS:
        case wxPLATFORM_MAC:
            break;

        default:
            wxFAIL_MSG(wxS("Invalid platform specified"));
            return wxFontEncodingArray();
    }

    int i, clas, e ;
    const wxFontEncoding *f;
    wxFontEncodingArray arr;

    clas = 0;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
    while (EquivalentEncodings[clas][0][0] != STOP)
    {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
        for (i = 0; i < NUM_OF_PLATFORMS; i++)
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
            for (e = 0; EquivalentEncodings[clas][i][e] != STOP; e++)
                if (EquivalentEncodings[clas][i][e] == enc)
                {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
                    for (f = EquivalentEncodings[clas][platform]; *f != STOP; f++)
                        if (*f == enc) arr.push_back(enc);
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
                    for (f = EquivalentEncodings[clas][platform]; *f != STOP; f++)
                        if (!FindEncoding(arr, *f)) arr.push_back(*f);
                    i = NUM_OF_PLATFORMS/*hack*/; break;
                }
        clas++;
    }

    return arr;
}



wxFontEncodingArray wxEncodingConverter::GetAllEquivalents(wxFontEncoding enc)
{
    int i, clas, e, j ;
    const wxFontEncoding *f;
    wxFontEncodingArray arr;

    arr = GetPlatformEquivalents(enc); // we want them to be first items in array

    clas = 0;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
    while (EquivalentEncodings[clas][0][0] != STOP)
    {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
        for (i = 0; i < NUM_OF_PLATFORMS; i++)
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
            for (e = 0; EquivalentEncodings[clas][i][e] != STOP; e++)
                if (EquivalentEncodings[clas][i][e] == enc)
                {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
                    for (j = 0; j < NUM_OF_PLATFORMS; j++)
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
                        for (f = EquivalentEncodings[clas][j]; *f != STOP; f++)
                            if (!FindEncoding(arr, *f)) arr.push_back(*f);
                    i = NUM_OF_PLATFORMS/*hack*/; break;
                }
        clas++;
    }

    return arr;
}

