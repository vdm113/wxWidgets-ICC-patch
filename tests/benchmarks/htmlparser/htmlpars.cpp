/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/html/htmlpars.cpp
// Purpose:     wx28HtmlParser class (generic parser)
// Author:      Vaclav Slavik
// Copyright:   (c) 1999 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "htmlpars.h"

#ifndef WXPRECOMP
    #include "wx/dynarray.h"
    #include "wx/log.h"
    #include "wx/intl.h"
    #include "wx/app.h"
#endif

#include "wx/tokenzr.h"
#include "wx/wfstream.h"
#include "wx/url.h"
#include "wx/fontmap.h"
#include "wx/html/htmldefs.h"
#include "wx/arrimpl.cpp"

#ifdef __WXWINCE__
    #include "wx/msw/wince/missing.h"       // for bsearch()
#endif

// DLL options compatibility check:
WX_CHECK_BUILD_OPTIONS("wxHTML")

const wxChar *wxTRACE_HTML_DEBUG = wxT("htmldebug");

//-----------------------------------------------------------------------------
// wx28HtmlParser helpers
//-----------------------------------------------------------------------------

class wx28HtmlTextPiece
{
public:
    wx28HtmlTextPiece(int pos, int lng) : m_pos(pos), m_lng(lng) {}
    int m_pos, m_lng;
};

WX_DECLARE_OBJARRAY(wx28HtmlTextPiece, wx28HtmlTextPieces);
WX_DEFINE_OBJARRAY(wx28HtmlTextPieces)

class wx28HtmlParserState
{
public:
    wx28HtmlTag         *m_curTag;
    wx28HtmlTag         *m_tags;
    wx28HtmlTextPieces  *m_textPieces;
    int                m_curTextPiece;
    wxString           m_source;
    wx28HtmlParserState *m_nextState;
};

//-----------------------------------------------------------------------------
// wx28HtmlParser
//-----------------------------------------------------------------------------

wxIMPLEMENT_ABSTRACT_CLASS(wx28HtmlParser, wxObject);

wx28HtmlParser::wx28HtmlParser()
    : wxObject(), m_HandlersHash(wxKEY_STRING),
      m_FS(NULL), m_HandlersStack(NULL)
{
    m_entitiesParser = new wx28HtmlEntitiesParser;
    m_Tags = NULL;
    m_CurTag = NULL;
    m_TextPieces = NULL;
    m_CurTextPiece = 0;
    m_SavedStates = NULL;
}

wx28HtmlParser::~wx28HtmlParser()
{
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
    while (RestoreState()) {}
    DestroyDOMTree();

    if (m_HandlersStack)
    {
        wxList& tmp = *m_HandlersStack;
        wxList::iterator it, en;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
        for( it = tmp.begin(), en = tmp.end(); it != en; ++it )
            delete (wxHashTable*)*it;
        tmp.clear();
    }
    delete m_HandlersStack;
    m_HandlersHash.Clear();
    WX_CLEAR_LIST(wxList, m_HandlersList);
    delete m_entitiesParser;
}

wxObject* wx28HtmlParser::Parse(const wxString& source)
{
    InitParser(source);
    DoParsing();
    wxObject *result = GetProduct();
    DoneParser();
    return result;
}

void wx28HtmlParser::InitParser(const wxString& source)
{
    SetSource(source);
    m_stopParsing = false;
}

void wx28HtmlParser::DoneParser()
{
    DestroyDOMTree();
}

void wx28HtmlParser::SetSource(const wxString& src)
{
    DestroyDOMTree();
    m_Source = src;
    CreateDOMTree();
    m_CurTag = NULL;
    m_CurTextPiece = 0;
}

void wx28HtmlParser::CreateDOMTree()
{
    wx28HtmlTagsCache cache(m_Source);
    m_TextPieces = new wx28HtmlTextPieces;
    CreateDOMSubTree(NULL, 0, m_Source.length(), &cache);
    m_CurTextPiece = 0;
}

extern bool wxIsCDATAElement(const wxChar *tag);

void wx28HtmlParser::CreateDOMSubTree(wx28HtmlTag *cur,
                                    int begin_pos, int end_pos,
                                    wx28HtmlTagsCache *cache)
{
    if (end_pos <= begin_pos) return;

    wxChar c;
    int i = begin_pos;
    int textBeginning = begin_pos;

    // If the tag contains CDATA text, we include the text between beginning
    // and ending tag verbosely. Setting i=end_pos will skip to the very
    // end of this function where text piece is added, bypassing any child
    // tags parsing (CDATA element can't have child elements by definition):
    if (cur != NULL && wxIsCDATAElement(cur->GetName().c_str()))
    {
        i = end_pos;
    }

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
    while (i < end_pos)
    {
        c = m_Source.GetChar(i);

        if (c == wxT('<'))
        {
            // add text to m_TextPieces:
            if (i - textBeginning > 0)
                m_TextPieces->Add(
                    wx28HtmlTextPiece(textBeginning, i - textBeginning));

            // if it is a comment, skip it:
            if (i < end_pos-6 && m_Source.GetChar(i+1) == wxT('!') &&
                                 m_Source.GetChar(i+2) == wxT('-') &&
                                 m_Source.GetChar(i+3) == wxT('-'))
            {
                // Comments begin with "<!--" and end with "--[ \t\r\n]*>"
                // according to HTML 4.0
                int dashes = 0;
                i += 4;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
                while (i < end_pos)
                {
                    c = m_Source.GetChar(i++);
                    if ((c == wxT(' ') || c == wxT('\n') ||
                        c == wxT('\r') || c == wxT('\t')) && dashes >= 2) {}
                    else if (c == wxT('>') && dashes >= 2)
                    {
                        textBeginning = i;
                        break;
                    }
                    else if (c == wxT('-'))
                        dashes++;
                    else
                        dashes = 0;
                }
            }

            // add another tag to the tree:
            else if (i < end_pos-1 && m_Source.GetChar(i+1) != wxT('/'))
            {
                wx28HtmlTag *chd;
                if (cur)
                    chd = new wx28HtmlTag(cur, m_Source,
                                        i, end_pos, cache, m_entitiesParser);
                else
                {
                    chd = new wx28HtmlTag(NULL, m_Source,
                                        i, end_pos, cache, m_entitiesParser);
                    if (!m_Tags)
                    {
                        // if this is the first tag to be created make the root
                        // m_Tags point to it:
                        m_Tags = chd;
                    }
                    else
                    {
                        // if there is already a root tag add this tag as
                        // the last sibling:
                        chd->m_Prev = m_Tags->GetLastSibling();
                        chd->m_Prev->m_Next = chd;
                    }
                }

                if (chd->HasEnding())
                {
                    CreateDOMSubTree(chd,
                                     chd->GetBeginPos(), chd->GetEndPos1(),
                                     cache);
                    i = chd->GetEndPos2();
                }
                else
                    i = chd->GetBeginPos();

                textBeginning = i;
            }

            // ... or skip ending tag:
            else
            {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
                while (i < end_pos && m_Source.GetChar(i) != wxT('>')) i++;
                textBeginning = i+1;
            }
        }
        else i++;
    }

    // add remaining text to m_TextPieces:
    if (end_pos - textBeginning > 0)
        m_TextPieces->Add(
            wx28HtmlTextPiece(textBeginning, end_pos - textBeginning));
}

void wx28HtmlParser::DestroyDOMTree()
{
    wx28HtmlTag *t1, *t2;
    t1 = m_Tags;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
    while (t1)
    {
        t2 = t1->GetNextSibling();
        delete t1;
        t1 = t2;
    }
    m_Tags = m_CurTag = NULL;

    delete m_TextPieces;
    m_TextPieces = NULL;
}

void wx28HtmlParser::DoParsing()
{
    m_CurTag = m_Tags;
    m_CurTextPiece = 0;
    DoParsing(0, m_Source.length());
}

void wx28HtmlParser::DoParsing(int begin_pos, int end_pos)
{
    if (end_pos <= begin_pos) return;

    wx28HtmlTextPieces& pieces = *m_TextPieces;
    size_t piecesCnt = pieces.GetCount();

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
    while (begin_pos < end_pos)
    {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
        while (m_CurTag && m_CurTag->GetBeginPos() < begin_pos)
            m_CurTag = m_CurTag->GetNextTag();
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
        while (m_CurTextPiece < piecesCnt &&
               pieces[m_CurTextPiece].m_pos < begin_pos)
            m_CurTextPiece++;

        if (m_CurTextPiece < piecesCnt &&
            (!m_CurTag ||
             pieces[m_CurTextPiece].m_pos < m_CurTag->GetBeginPos()))
        {
            // Add text:
            AddText(GetEntitiesParser()->Parse(
                       m_Source.Mid(pieces[m_CurTextPiece].m_pos,
                                    pieces[m_CurTextPiece].m_lng)).t_str());
            begin_pos = pieces[m_CurTextPiece].m_pos +
                        pieces[m_CurTextPiece].m_lng;
            m_CurTextPiece++;
        }
        else if (m_CurTag)
        {
            if (m_CurTag->HasEnding())
                begin_pos = m_CurTag->GetEndPos2();
            else
                begin_pos = m_CurTag->GetBeginPos();
            wx28HtmlTag *t = m_CurTag;
            m_CurTag = m_CurTag->GetNextTag();
            AddTag(*t);
            if (m_stopParsing)
                return;
        }
        else break;
    }
}

void wx28HtmlParser::AddTag(const wx28HtmlTag& tag)
{
    wx28HtmlTagHandler *h;
    bool inner = false;

    h = (wx28HtmlTagHandler*) m_HandlersHash.Get(tag.GetName());
    if (h)
    {
        inner = h->HandleTag(tag);
        if (m_stopParsing)
            return;
    }
    if (!inner)
    {
        if (tag.HasEnding())
            DoParsing(tag.GetBeginPos(), tag.GetEndPos1());
    }
}

void wx28HtmlParser::AddTagHandler(wx28HtmlTagHandler *handler)
{
    wxString s(handler->GetSupportedTags());
    wxStringTokenizer tokenizer(s, wxT(", "));

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
    while (tokenizer.HasMoreTokens())
        m_HandlersHash.Put(tokenizer.GetNextToken(), handler);

    if (m_HandlersList.IndexOf(handler) == wxNOT_FOUND)
        m_HandlersList.Append(handler);

    handler->SetParser(this);
}

void wx28HtmlParser::PushTagHandler(wx28HtmlTagHandler *handler, const wxString& tags)
{
    wxStringTokenizer tokenizer(tags, wxT(", "));
    wxString key;

    if (m_HandlersStack == NULL)
    {
        m_HandlersStack = new wxList;
    }

    m_HandlersStack->Insert((wxObject*)new wxHashTable(m_HandlersHash));

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
    while (tokenizer.HasMoreTokens())
    {
        key = tokenizer.GetNextToken();
        m_HandlersHash.Delete(key);
        m_HandlersHash.Put(key, handler);
    }
}

void wx28HtmlParser::PopTagHandler()
{
    wxList::compatibility_iterator first;

    if ( !m_HandlersStack ||
#if wxUSE_STL
         !(first = m_HandlersStack->GetFirst())
#else // !wxUSE_STL
         ((first = m_HandlersStack->GetFirst()) == NULL)
#endif // wxUSE_STL/!wxUSE_STL
        )
    {
        wxLogWarning(_("Warning: attempt to remove HTML tag handler from empty stack."));
        return;
    }
    m_HandlersHash = *((wxHashTable*) first->GetData());
    delete (wxHashTable*) first->GetData();
    m_HandlersStack->Erase(first);
}

void wx28HtmlParser::SetSourceAndSaveState(const wxString& src)
{
    wx28HtmlParserState *s = new wx28HtmlParserState;

    s->m_curTag = m_CurTag;
    s->m_tags = m_Tags;
    s->m_textPieces = m_TextPieces;
    s->m_curTextPiece = m_CurTextPiece;
    s->m_source = m_Source;

    s->m_nextState = m_SavedStates;
    m_SavedStates = s;

    m_CurTag = NULL;
    m_Tags = NULL;
    m_TextPieces = NULL;
    m_CurTextPiece = 0;
    m_Source = wxEmptyString;

    SetSource(src);
}

bool wx28HtmlParser::RestoreState()
{
    if (!m_SavedStates) return false;

    DestroyDOMTree();

    wx28HtmlParserState *s = m_SavedStates;
    m_SavedStates = s->m_nextState;

    m_CurTag = s->m_curTag;
    m_Tags = s->m_tags;
    m_TextPieces = s->m_textPieces;
    m_CurTextPiece = s->m_curTextPiece;
    m_Source = s->m_source;

    delete s;
    return true;
}

wxString wx28HtmlParser::GetInnerSource(const wx28HtmlTag& tag)
{
    return GetSource()->Mid(tag.GetBeginPos(),
                            tag.GetEndPos1() - tag.GetBeginPos());
}

//-----------------------------------------------------------------------------
// wx28HtmlTagHandler
//-----------------------------------------------------------------------------

wxIMPLEMENT_ABSTRACT_CLASS(wx28HtmlTagHandler, wxObject);

void wx28HtmlTagHandler::ParseInnerSource(const wxString& source)
{
    // It is safe to temporarily change the source being parsed,
    // provided we restore the state back after parsing
    m_Parser->SetSourceAndSaveState(source);
    m_Parser->DoParsing();
    m_Parser->RestoreState();
}


//-----------------------------------------------------------------------------
// wx28HtmlEntitiesParser
//-----------------------------------------------------------------------------

wxIMPLEMENT_DYNAMIC_CLASS(wx28HtmlEntitiesParser,wxObject);

wx28HtmlEntitiesParser::wx28HtmlEntitiesParser()
#if !wxUSE_UNICODE
    : m_conv(NULL), m_encoding(wxFONTENCODING_SYSTEM)
#endif
{
}

wx28HtmlEntitiesParser::~wx28HtmlEntitiesParser()
{
#if !wxUSE_UNICODE
    delete m_conv;
#endif
}

void wx28HtmlEntitiesParser::SetEncoding(wxFontEncoding encoding)
{
#if !wxUSE_UNICODE
    if (encoding == m_encoding)
        return;

    delete m_conv;

    m_encoding = encoding;
    if (m_encoding == wxFONTENCODING_SYSTEM)
        m_conv = NULL;
    else
        m_conv = new wxCSConv(wxFontMapper::GetEncodingName(m_encoding));
#else
    (void) encoding;
#endif
}

wxString wx28HtmlEntitiesParser::Parse(const wxString& input)
{
    const wxChar *c, *last;
    const wxChar *in_str = input.c_str();
    wxString output;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
    for (c = in_str, last = in_str; *c != wxT('\0'); c++)
    {
        if (*c == wxT('&'))
        {
            if ( output.empty() )
                output.reserve(input.length());

            if (c - last > 0)
                output.append(last, c - last);
            if ( *++c == wxT('\0') )
                break;

            wxString entity;
            const wxChar *ent_s = c;
            wxChar entity_char;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
            for (; (*c >= wxT('a') && *c <= wxT('z')) ||
                   (*c >= wxT('A') && *c <= wxT('Z')) ||
                   (*c >= wxT('0') && *c <= wxT('9')) ||
                   *c == wxT('_') || *c == wxT('#'); c++) {}
            entity.append(ent_s, c - ent_s);
            if (*c != wxT(';')) c--;
            last = c+1;
            entity_char = GetEntityChar(entity);
            if (entity_char)
                output << entity_char;
            else
            {
                output.append(ent_s-1, c-ent_s+2);
                wxLogTrace(wxTRACE_HTML_DEBUG,
                           wxT("Unrecognized HTML entity: '%s'"),
                           entity.c_str());
            }
        }
    }
    if (last == in_str) // common case: no entity
        return input;
    if (*last != wxT('\0'))
        output.append(last);
    return output;
}

struct wx28HtmlEntityInfo
{
    const wxChar *name;
    unsigned code;
};

extern "C" int LINKAGEMODE wx28HtmlEntityCompare(const void *key, const void *item)
{
    return wxStrcmp((wxChar*)key, ((wx28HtmlEntityInfo*)item)->name);
}

#if !wxUSE_UNICODE
wxChar wx28HtmlEntitiesParser::GetCharForCode(unsigned code)
{
    char buf[2];
    wchar_t wbuf[2];
    wbuf[0] = (wchar_t)code;
    wbuf[1] = 0;
    wxMBConv *conv = m_conv ? m_conv : &wxConvLocal;
    if (conv->WC2MB(buf, wbuf, 2) == (size_t)-1)
        return '?';
    return buf[0];
}
#endif

wxChar wx28HtmlEntitiesParser::GetEntityChar(const wxString& entity)
{
    unsigned code = 0;

    if (entity[0] == wxT('#'))
    {
        const wxChar *ent_s = entity.c_str();
        const wxChar *format;

        if (ent_s[1] == wxT('x') || ent_s[1] == wxT('X'))
        {
            format = wxT("%x");
            ent_s++;
        }
        else
            format = wxT("%u");
        ent_s++;

        if (wxSscanf(ent_s, format, &code) != 1)
            code = 0;
    }
    else
    {
        static wx28HtmlEntityInfo substitutions[] = {
            { wxT("AElig"),198 },
            { wxT("Aacute"),193 },
            { wxT("Acirc"),194 },
            { wxT("Agrave"),192 },
            { wxT("Alpha"),913 },
            { wxT("Aring"),197 },
            { wxT("Atilde"),195 },
            { wxT("Auml"),196 },
            { wxT("Beta"),914 },
            { wxT("Ccedil"),199 },
            { wxT("Chi"),935 },
            { wxT("Dagger"),8225 },
            { wxT("Delta"),916 },
            { wxT("ETH"),208 },
            { wxT("Eacute"),201 },
            { wxT("Ecirc"),202 },
            { wxT("Egrave"),200 },
            { wxT("Epsilon"),917 },
            { wxT("Eta"),919 },
            { wxT("Euml"),203 },
            { wxT("Gamma"),915 },
            { wxT("Iacute"),205 },
            { wxT("Icirc"),206 },
            { wxT("Igrave"),204 },
            { wxT("Iota"),921 },
            { wxT("Iuml"),207 },
            { wxT("Kappa"),922 },
            { wxT("Lambda"),923 },
            { wxT("Mu"),924 },
            { wxT("Ntilde"),209 },
            { wxT("Nu"),925 },
            { wxT("OElig"),338 },
            { wxT("Oacute"),211 },
            { wxT("Ocirc"),212 },
            { wxT("Ograve"),210 },
            { wxT("Omega"),937 },
            { wxT("Omicron"),927 },
            { wxT("Oslash"),216 },
            { wxT("Otilde"),213 },
            { wxT("Ouml"),214 },
            { wxT("Phi"),934 },
            { wxT("Pi"),928 },
            { wxT("Prime"),8243 },
            { wxT("Psi"),936 },
            { wxT("Rho"),929 },
            { wxT("Scaron"),352 },
            { wxT("Sigma"),931 },
            { wxT("THORN"),222 },
            { wxT("Tau"),932 },
            { wxT("Theta"),920 },
            { wxT("Uacute"),218 },
            { wxT("Ucirc"),219 },
            { wxT("Ugrave"),217 },
            { wxT("Upsilon"),933 },
            { wxT("Uuml"),220 },
            { wxT("Xi"),926 },
            { wxT("Yacute"),221 },
            { wxT("Yuml"),376 },
            { wxT("Zeta"),918 },
            { wxT("aacute"),225 },
            { wxT("acirc"),226 },
            { wxT("acute"),180 },
            { wxT("aelig"),230 },
            { wxT("agrave"),224 },
            { wxT("alefsym"),8501 },
            { wxT("alpha"),945 },
            { wxT("amp"),38 },
            { wxT("and"),8743 },
            { wxT("ang"),8736 },
            { wxT("aring"),229 },
            { wxT("asymp"),8776 },
            { wxT("atilde"),227 },
            { wxT("auml"),228 },
            { wxT("bdquo"),8222 },
            { wxT("beta"),946 },
            { wxT("brvbar"),166 },
            { wxT("bull"),8226 },
            { wxT("cap"),8745 },
            { wxT("ccedil"),231 },
            { wxT("cedil"),184 },
            { wxT("cent"),162 },
            { wxT("chi"),967 },
            { wxT("circ"),710 },
            { wxT("clubs"),9827 },
            { wxT("cong"),8773 },
            { wxT("copy"),169 },
            { wxT("crarr"),8629 },
            { wxT("cup"),8746 },
            { wxT("curren"),164 },
            { wxT("dArr"),8659 },
            { wxT("dagger"),8224 },
            { wxT("darr"),8595 },
            { wxT("deg"),176 },
            { wxT("delta"),948 },
            { wxT("diams"),9830 },
            { wxT("divide"),247 },
            { wxT("eacute"),233 },
            { wxT("ecirc"),234 },
            { wxT("egrave"),232 },
            { wxT("empty"),8709 },
            { wxT("emsp"),8195 },
            { wxT("ensp"),8194 },
            { wxT("epsilon"),949 },
            { wxT("equiv"),8801 },
            { wxT("eta"),951 },
            { wxT("eth"),240 },
            { wxT("euml"),235 },
            { wxT("euro"),8364 },
            { wxT("exist"),8707 },
            { wxT("fnof"),402 },
            { wxT("forall"),8704 },
            { wxT("frac12"),189 },
            { wxT("frac14"),188 },
            { wxT("frac34"),190 },
            { wxT("frasl"),8260 },
            { wxT("gamma"),947 },
            { wxT("ge"),8805 },
            { wxT("gt"),62 },
            { wxT("hArr"),8660 },
            { wxT("harr"),8596 },
            { wxT("hearts"),9829 },
            { wxT("hellip"),8230 },
            { wxT("iacute"),237 },
            { wxT("icirc"),238 },
            { wxT("iexcl"),161 },
            { wxT("igrave"),236 },
            { wxT("image"),8465 },
            { wxT("infin"),8734 },
            { wxT("int"),8747 },
            { wxT("iota"),953 },
            { wxT("iquest"),191 },
            { wxT("isin"),8712 },
            { wxT("iuml"),239 },
            { wxT("kappa"),954 },
            { wxT("lArr"),8656 },
            { wxT("lambda"),955 },
            { wxT("lang"),9001 },
            { wxT("laquo"),171 },
            { wxT("larr"),8592 },
            { wxT("lceil"),8968 },
            { wxT("ldquo"),8220 },
            { wxT("le"),8804 },
            { wxT("lfloor"),8970 },
            { wxT("lowast"),8727 },
            { wxT("loz"),9674 },
            { wxT("lrm"),8206 },
            { wxT("lsaquo"),8249 },
            { wxT("lsquo"),8216 },
            { wxT("lt"),60 },
            { wxT("macr"),175 },
            { wxT("mdash"),8212 },
            { wxT("micro"),181 },
            { wxT("middot"),183 },
            { wxT("minus"),8722 },
            { wxT("mu"),956 },
            { wxT("nabla"),8711 },
            { wxT("nbsp"),160 },
            { wxT("ndash"),8211 },
            { wxT("ne"),8800 },
            { wxT("ni"),8715 },
            { wxT("not"),172 },
            { wxT("notin"),8713 },
            { wxT("nsub"),8836 },
            { wxT("ntilde"),241 },
            { wxT("nu"),957 },
            { wxT("oacute"),243 },
            { wxT("ocirc"),244 },
            { wxT("oelig"),339 },
            { wxT("ograve"),242 },
            { wxT("oline"),8254 },
            { wxT("omega"),969 },
            { wxT("omicron"),959 },
            { wxT("oplus"),8853 },
            { wxT("or"),8744 },
            { wxT("ordf"),170 },
            { wxT("ordm"),186 },
            { wxT("oslash"),248 },
            { wxT("otilde"),245 },
            { wxT("otimes"),8855 },
            { wxT("ouml"),246 },
            { wxT("para"),182 },
            { wxT("part"),8706 },
            { wxT("permil"),8240 },
            { wxT("perp"),8869 },
            { wxT("phi"),966 },
            { wxT("pi"),960 },
            { wxT("piv"),982 },
            { wxT("plusmn"),177 },
            { wxT("pound"),163 },
            { wxT("prime"),8242 },
            { wxT("prod"),8719 },
            { wxT("prop"),8733 },
            { wxT("psi"),968 },
            { wxT("quot"),34 },
            { wxT("rArr"),8658 },
            { wxT("radic"),8730 },
            { wxT("rang"),9002 },
            { wxT("raquo"),187 },
            { wxT("rarr"),8594 },
            { wxT("rceil"),8969 },
            { wxT("rdquo"),8221 },
            { wxT("real"),8476 },
            { wxT("reg"),174 },
            { wxT("rfloor"),8971 },
            { wxT("rho"),961 },
            { wxT("rlm"),8207 },
            { wxT("rsaquo"),8250 },
            { wxT("rsquo"),8217 },
            { wxT("sbquo"),8218 },
            { wxT("scaron"),353 },
            { wxT("sdot"),8901 },
            { wxT("sect"),167 },
            { wxT("shy"),173 },
            { wxT("sigma"),963 },
            { wxT("sigmaf"),962 },
            { wxT("sim"),8764 },
            { wxT("spades"),9824 },
            { wxT("sub"),8834 },
            { wxT("sube"),8838 },
            { wxT("sum"),8721 },
            { wxT("sup"),8835 },
            { wxT("sup1"),185 },
            { wxT("sup2"),178 },
            { wxT("sup3"),179 },
            { wxT("supe"),8839 },
            { wxT("szlig"),223 },
            { wxT("tau"),964 },
            { wxT("there4"),8756 },
            { wxT("theta"),952 },
            { wxT("thetasym"),977 },
            { wxT("thinsp"),8201 },
            { wxT("thorn"),254 },
            { wxT("tilde"),732 },
            { wxT("times"),215 },
            { wxT("trade"),8482 },
            { wxT("uArr"),8657 },
            { wxT("uacute"),250 },
            { wxT("uarr"),8593 },
            { wxT("ucirc"),251 },
            { wxT("ugrave"),249 },
            { wxT("uml"),168 },
            { wxT("upsih"),978 },
            { wxT("upsilon"),965 },
            { wxT("uuml"),252 },
            { wxT("weierp"),8472 },
            { wxT("xi"),958 },
            { wxT("yacute"),253 },
            { wxT("yen"),165 },
            { wxT("yuml"),255 },
            { wxT("zeta"),950 },
            { wxT("zwj"),8205 },
            { wxT("zwnj"),8204 },
            {NULL, 0}};
        static size_t substitutions_cnt = 0;

        if (substitutions_cnt == 0)
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
            while (substitutions[substitutions_cnt].code != 0)
                substitutions_cnt++;

        wx28HtmlEntityInfo *info = NULL;
#ifdef __WXWINCE__
        // bsearch crashes under WinCE for some reason
        size_t i;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif /* VDM auto patch */
        for (i = 0; i < substitutions_cnt; i++)
        {
            if (entity == substitutions[i].name)
            {
                info = & substitutions[i];
                break;
            }
        }
#else
        info = (wx28HtmlEntityInfo*) bsearch(entity.c_str(), substitutions,
                                           substitutions_cnt,
                                           sizeof(wx28HtmlEntityInfo),
                                           wx28HtmlEntityCompare);
#endif
        if (info)
            code = info->code;
    }

    if (code == 0)
        return 0;
    else
        return GetCharForCode(code);
}

wxFSFile *wx28HtmlParser::OpenURL(wx28HtmlURLType WXUNUSED(type),
                                const wxString& url) const
{
    return m_FS ? m_FS->OpenFile(url) : NULL;

}


//-----------------------------------------------------------------------------
// wx28HtmlParser::ExtractCharsetInformation
//-----------------------------------------------------------------------------

class wxMetaTagParser : public wx28HtmlParser
{
public:
    wxMetaTagParser() { }

    wxObject* GetProduct() { return NULL; }

protected:
    virtual void AddText(const wxChar* WXUNUSED(txt)) {}

    wxDECLARE_NO_COPY_CLASS(wxMetaTagParser);
};

class wxMetaTagHandler : public wx28HtmlTagHandler
{
public:
    wxMetaTagHandler(wxString *retval) : wx28HtmlTagHandler(), m_retval(retval) {}
    wxString GetSupportedTags() { return wxT("META,BODY"); }
    bool HandleTag(const wx28HtmlTag& tag);

private:
    wxString *m_retval;

    wxDECLARE_NO_COPY_CLASS(wxMetaTagHandler);
};

bool wxMetaTagHandler::HandleTag(const wx28HtmlTag& tag)
{
    if (tag.GetName() == wxT("BODY"))
    {
        m_Parser->StopParsing();
        return false;
    }

    if (tag.HasParam(wxT("HTTP-EQUIV")) &&
        tag.GetParam(wxT("HTTP-EQUIV")).IsSameAs(wxT("Content-Type"), false) &&
        tag.HasParam(wxT("CONTENT")))
    {
        wxString content = tag.GetParam(wxT("CONTENT")).Lower();
        if (content.Left(19) == wxT("text/html; charset="))
        {
            *m_retval = content.Mid(19);
            m_Parser->StopParsing();
        }
    }
    return false;
}


/*static*/
wxString wx28HtmlParser::ExtractCharsetInformation(const wxString& markup)
{
    wxString charset;
    wxMetaTagParser *parser = new wxMetaTagParser();
    if(parser)
    {
        parser->AddTagHandler(new wxMetaTagHandler(&charset));
        parser->Parse(markup);
        delete parser;
    }
    return charset;
}
