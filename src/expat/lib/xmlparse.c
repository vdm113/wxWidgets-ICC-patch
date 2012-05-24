/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/* Copyright (c) 1998, 1999, 2000 Thai Open Source Software Center Ltd
   See the file COPYING for copying permission.
*/

#include <stddef.h>
#include <string.h>                     /* memset(), memcpy() */

#ifdef COMPILED_FROM_DSP

#include "winconfig.h"
#include "expat.h"

#elif defined(OS2_32)

#include "os2config.h"
#include "expat.h"

#elif defined(__MSDOS__)

#include "dosconfig.h"
#include "expat.h"

#elif defined(MACOS_CLASSIC)

#include "macconfig.h"
#include "expat.h"

#else

#include "expat_config.h"

#ifdef __declspec
#define XMLPARSEAPI(type) type __cdecl
#endif

#include "expat.h"

#ifdef __declspec
#undef XMLPARSEAPI
#endif
#endif /* ndef COMPILED_FROM_DSP */

#ifdef XML_UNICODE
#define XML_ENCODE_MAX XML_UTF16_ENCODE_MAX
#define XmlConvert XmlUtf16Convert
#define XmlGetInternalEncoding XmlGetUtf16InternalEncoding
#define XmlGetInternalEncodingNS XmlGetUtf16InternalEncodingNS
#define XmlEncode XmlUtf16Encode
#define MUST_CONVERT(enc, s) (!(enc)->isUtf16 || (((unsigned long)s) & 1))
typedef unsigned short ICHAR;
#else
#define XML_ENCODE_MAX XML_UTF8_ENCODE_MAX
#define XmlConvert XmlUtf8Convert
#define XmlGetInternalEncoding XmlGetUtf8InternalEncoding
#define XmlGetInternalEncodingNS XmlGetUtf8InternalEncodingNS
#define XmlEncode XmlUtf8Encode
#define MUST_CONVERT(enc, s) (!(enc)->isUtf8)
typedef char ICHAR;
#endif


#ifndef XML_NS

#define XmlInitEncodingNS XmlInitEncoding
#define XmlInitUnknownEncodingNS XmlInitUnknownEncoding
#undef XmlGetInternalEncodingNS
#define XmlGetInternalEncodingNS XmlGetInternalEncoding
#define XmlParseXmlDeclNS XmlParseXmlDecl

#endif

#ifdef XML_UNICODE

#ifdef XML_UNICODE_WCHAR_T
#define XML_T(x) (const wchar_t)x
#define XML_L(x) L ## x
#else
#define XML_T(x) (const unsigned short)x
#define XML_L(x) x
#endif

#else

#define XML_T(x) x
#define XML_L(x) x

#endif

/* Round up n to be a multiple of sz, where sz is a power of 2. */
#define ROUND_UP(n, sz) (((n) + ((sz) - 1)) & ~((sz) - 1))

/* Handle the case where memmove() doesn't exist. */
#ifndef HAVE_MEMMOVE
#ifdef HAVE_BCOPY
#define memmove(d,s,l) bcopy((s),(d),(l))
#else
#error memmove does not exist on this platform, nor is a substitute available
#endif /* HAVE_BCOPY */
#endif /* HAVE_MEMMOVE */

#include "internal.h"
#include "xmltok.h"
#include "xmlrole.h"

typedef const XML_Char *KEY;

typedef struct {
  KEY name;
} NAMED;

typedef struct {
  NAMED **v;
  size_t size;
  size_t used;
  size_t usedLim;
  const XML_Memory_Handling_Suite *mem;
} HASH_TABLE;

typedef struct {
  NAMED **p;
  NAMED **end;
} HASH_TABLE_ITER;

#define INIT_TAG_BUF_SIZE 32  /* must be a multiple of sizeof(XML_Char) */
#define INIT_DATA_BUF_SIZE 1024
#define INIT_ATTS_SIZE 16
#define INIT_BLOCK_SIZE 1024
#define INIT_BUFFER_SIZE 1024

#define EXPAND_SPARE 24

typedef struct binding {
  struct prefix *prefix;
  struct binding *nextTagBinding;
  struct binding *prevPrefixBinding;
  const struct attribute_id *attId;
  XML_Char *uri;
  int uriLen;
  int uriAlloc;
} BINDING;

typedef struct prefix {
  const XML_Char *name;
  BINDING *binding;
} PREFIX;

typedef struct {
  const XML_Char *str;
  const XML_Char *localPart;
  const XML_Char *prefix;
  int strLen;
  int uriLen;
  int prefixLen;
} TAG_NAME;

/* TAG represents an open element.
   The name of the element is stored in both the document and API
   encodings.  The memory buffer 'buf' is a separately-allocated
   memory area which stores the name.  During the XML_Parse()/
   XMLParseBuffer() when the element is open, the memory for the 'raw'
   version of the name (in the document encoding) is shared with the
   document buffer.  If the element is open across calls to
   XML_Parse()/XML_ParseBuffer(), the buffer is re-allocated to
   contain the 'raw' name as well.

   A parser re-uses these structures, maintaining a list of allocated
   TAG objects in a free list.
*/
typedef struct tag {
  struct tag *parent;           /* parent of this element */
  const char *rawName;          /* tagName in the original encoding */
  int rawNameLength;
  TAG_NAME name;                /* tagName in the API encoding */
  char *buf;                    /* buffer for name components */
  char *bufEnd;                 /* end of the buffer */
  BINDING *bindings;
} TAG;

typedef struct {
  const XML_Char *name;
  const XML_Char *textPtr;
  int textLen;
  const XML_Char *systemId;
  const XML_Char *base;
  const XML_Char *publicId;
  const XML_Char *notation;
  XML_Bool open;
  XML_Bool is_param;
  XML_Bool is_internal; /* true if declared in internal subset outside PE */
} ENTITY;

typedef struct {
  enum XML_Content_Type         type;
  enum XML_Content_Quant        quant;
  const XML_Char *              name;
  int                           firstchild;
  int                           lastchild;
  int                           childcnt;
  int                           nextsib;
} CONTENT_SCAFFOLD;

#define INIT_SCAFFOLD_ELEMENTS 32

typedef struct block {
  struct block *next;
  int size;
  XML_Char s[1];
} BLOCK;

typedef struct {
  BLOCK *blocks;
  BLOCK *freeBlocks;
  const XML_Char *end;
  XML_Char *ptr;
  XML_Char *start;
  const XML_Memory_Handling_Suite *mem;
} STRING_POOL;

/* The XML_Char before the name is used to determine whether
   an attribute has been specified. */
typedef struct attribute_id {
  XML_Char *name;
  PREFIX *prefix;
  XML_Bool maybeTokenized;
  XML_Bool xmlns;
} ATTRIBUTE_ID;

typedef struct {
  const ATTRIBUTE_ID *id;
  XML_Bool isCdata;
  const XML_Char *value;
} DEFAULT_ATTRIBUTE;

typedef struct {
  const XML_Char *name;
  PREFIX *prefix;
  const ATTRIBUTE_ID *idAtt;
  int nDefaultAtts;
  int allocDefaultAtts;
  DEFAULT_ATTRIBUTE *defaultAtts;
} ELEMENT_TYPE;

typedef struct {
  HASH_TABLE generalEntities;
  HASH_TABLE elementTypes;
  HASH_TABLE attributeIds;
  HASH_TABLE prefixes;
  STRING_POOL pool;
  STRING_POOL entityValuePool;
  /* false once a parameter entity reference has been skipped */
  XML_Bool keepProcessing;
  /* true once an internal or external PE reference has been encountered;
     this includes the reference to an external subset */
  XML_Bool hasParamEntityRefs;
  XML_Bool standalone;
#ifdef XML_DTD
  /* indicates if external PE has been read */
  XML_Bool paramEntityRead;
  HASH_TABLE paramEntities;
#endif /* XML_DTD */
  PREFIX defaultPrefix;
  /* === scaffolding for building content model === */
  XML_Bool in_eldecl;
  CONTENT_SCAFFOLD *scaffold;
  unsigned contentStringLen;
  unsigned scaffSize;
  unsigned scaffCount;
  int scaffLevel;
  int *scaffIndex;
} DTD;

typedef struct open_internal_entity {
  const char *internalEventPtr;
  const char *internalEventEndPtr;
  struct open_internal_entity *next;
  ENTITY *entity;
} OPEN_INTERNAL_ENTITY;

typedef enum XML_Error PTRCALL Processor(XML_Parser parser,
                                         const char *start,
                                         const char *end,
                                         const char **endPtr);

static Processor prologProcessor;
static Processor prologInitProcessor;
static Processor contentProcessor;
static Processor cdataSectionProcessor;
#ifdef XML_DTD
static Processor ignoreSectionProcessor;
static Processor externalParEntProcessor;
static Processor externalParEntInitProcessor;
static Processor entityValueProcessor;
static Processor entityValueInitProcessor;
#endif /* XML_DTD */
static Processor epilogProcessor;
static Processor errorProcessor;
static Processor externalEntityInitProcessor;
static Processor externalEntityInitProcessor2;
static Processor externalEntityInitProcessor3;
static Processor externalEntityContentProcessor;

static enum XML_Error
handleUnknownEncoding(XML_Parser parser, const XML_Char *encodingName);
static enum XML_Error
processXmlDecl(XML_Parser parser, int isGeneralTextEntity,
               const char *, const char *);
static enum XML_Error
initializeEncoding(XML_Parser parser);
static enum XML_Error
doProlog(XML_Parser parser, const ENCODING *enc, const char *s,
         const char *end, int tok, const char *next, const char **nextPtr);
static enum XML_Error
processInternalParamEntity(XML_Parser parser, ENTITY *entity);
static enum XML_Error
doContent(XML_Parser parser, int startTagLevel, const ENCODING *enc,
          const char *start, const char *end, const char **endPtr);
static enum XML_Error
doCdataSection(XML_Parser parser, const ENCODING *, const char **startPtr,
               const char *end, const char **nextPtr);
#ifdef XML_DTD
static enum XML_Error
doIgnoreSection(XML_Parser parser, const ENCODING *, const char **startPtr,
                const char *end, const char **nextPtr);
#endif /* XML_DTD */

static enum XML_Error
storeAtts(XML_Parser parser, const ENCODING *, const char *s,
          TAG_NAME *tagNamePtr, BINDING **bindingsPtr);
static enum XML_Error
addBinding(XML_Parser parser, PREFIX *prefix, const ATTRIBUTE_ID *attId,
           const XML_Char *uri, BINDING **bindingsPtr);
static int
defineAttribute(ELEMENT_TYPE *type, ATTRIBUTE_ID *,
                XML_Bool isCdata, XML_Bool isId, const XML_Char *dfltValue,
                XML_Parser parser);
static enum XML_Error
storeAttributeValue(XML_Parser parser, const ENCODING *, XML_Bool isCdata,
                    const char *, const char *, STRING_POOL *);
static enum XML_Error
appendAttributeValue(XML_Parser parser, const ENCODING *, XML_Bool isCdata,
                     const char *, const char *, STRING_POOL *);
static ATTRIBUTE_ID *
getAttributeId(XML_Parser parser, const ENCODING *enc, const char *start,
               const char *end);
static int
setElementTypePrefix(XML_Parser parser, ELEMENT_TYPE *);
static enum XML_Error
storeEntityValue(XML_Parser parser, const ENCODING *enc, const char *start,
                 const char *end);
static int
reportProcessingInstruction(XML_Parser parser, const ENCODING *enc,
                            const char *start, const char *end);
static int
reportComment(XML_Parser parser, const ENCODING *enc, const char *start,
              const char *end);
static void
reportDefault(XML_Parser parser, const ENCODING *enc, const char *start,
              const char *end);

static const XML_Char * getContext(XML_Parser parser);
static XML_Bool
setContext(XML_Parser parser, const XML_Char *context);

static void FASTCALL normalizePublicId(XML_Char *s);

static DTD * dtdCreate(const XML_Memory_Handling_Suite *ms);
/* do not call if parentParser != NULL */
static void dtdReset(DTD *p, const XML_Memory_Handling_Suite *ms);
static void
dtdDestroy(DTD *p, XML_Bool isDocEntity, const XML_Memory_Handling_Suite *ms);
static int
dtdCopy(DTD *newDtd, const DTD *oldDtd, const XML_Memory_Handling_Suite *ms);
static int
copyEntityTable(HASH_TABLE *, STRING_POOL *, const HASH_TABLE *);

static NAMED *
lookup(HASH_TABLE *table, KEY name, size_t createSize);
static void FASTCALL
hashTableInit(HASH_TABLE *, const XML_Memory_Handling_Suite *ms);
static void FASTCALL hashTableClear(HASH_TABLE *);
static void FASTCALL hashTableDestroy(HASH_TABLE *);
static void FASTCALL
hashTableIterInit(HASH_TABLE_ITER *, const HASH_TABLE *);
static NAMED * FASTCALL hashTableIterNext(HASH_TABLE_ITER *);

static void FASTCALL
poolInit(STRING_POOL *, const XML_Memory_Handling_Suite *ms);
static void FASTCALL poolClear(STRING_POOL *);
static void FASTCALL poolDestroy(STRING_POOL *);
static XML_Char *
poolAppend(STRING_POOL *pool, const ENCODING *enc,
           const char *ptr, const char *end);
static XML_Char *
poolStoreString(STRING_POOL *pool, const ENCODING *enc,
                const char *ptr, const char *end);
static XML_Bool FASTCALL poolGrow(STRING_POOL *pool);
static const XML_Char * FASTCALL
poolCopyString(STRING_POOL *pool, const XML_Char *s);
static const XML_Char *
poolCopyStringN(STRING_POOL *pool, const XML_Char *s, int n);
static const XML_Char * FASTCALL
poolAppendString(STRING_POOL *pool, const XML_Char *s);

static int FASTCALL nextScaffoldPart(XML_Parser parser);
static XML_Content * build_model(XML_Parser parser);
static ELEMENT_TYPE *
getElementType(XML_Parser parser, const ENCODING *enc,
               const char *ptr, const char *end);

static XML_Parser
parserCreate(const XML_Char *encodingName,
             const XML_Memory_Handling_Suite *memsuite,
             const XML_Char *nameSep,
             DTD *dtd);
static void
parserInit(XML_Parser parser, const XML_Char *encodingName);

#define poolStart(pool) ((pool)->start)
#define poolEnd(pool) ((pool)->ptr)
#define poolLength(pool) ((pool)->ptr - (pool)->start)
#define poolChop(pool) ((void)--(pool->ptr))
#define poolLastChar(pool) (((pool)->ptr)[-1])
#define poolDiscard(pool) ((pool)->ptr = (pool)->start)
#define poolFinish(pool) ((pool)->start = (pool)->ptr)
#define poolAppendChar(pool, c) \
  (((pool)->ptr == (pool)->end && !poolGrow(pool)) \
   ? 0 \
   : ((*((pool)->ptr)++ = c), 1))

struct XML_ParserStruct {
  /* The first member must be userData so that the XML_GetUserData
     macro works. */
  void *m_userData;
  void *m_handlerArg;
  char *m_buffer;
  const XML_Memory_Handling_Suite m_mem;
  /* first character to be parsed */
  const char *m_bufferPtr;
  /* past last character to be parsed */
  char *m_bufferEnd;
  /* allocated end of buffer */
  const char *m_bufferLim;
  long m_parseEndByteIndex;
  const char *m_parseEndPtr;
  XML_Char *m_dataBuf;
  XML_Char *m_dataBufEnd;
  XML_StartElementHandler m_startElementHandler;
  XML_EndElementHandler m_endElementHandler;
  XML_CharacterDataHandler m_characterDataHandler;
  XML_ProcessingInstructionHandler m_processingInstructionHandler;
  XML_CommentHandler m_commentHandler;
  XML_StartCdataSectionHandler m_startCdataSectionHandler;
  XML_EndCdataSectionHandler m_endCdataSectionHandler;
  XML_DefaultHandler m_defaultHandler;
  XML_StartDoctypeDeclHandler m_startDoctypeDeclHandler;
  XML_EndDoctypeDeclHandler m_endDoctypeDeclHandler;
  XML_UnparsedEntityDeclHandler m_unparsedEntityDeclHandler;
  XML_NotationDeclHandler m_notationDeclHandler;
  XML_StartNamespaceDeclHandler m_startNamespaceDeclHandler;
  XML_EndNamespaceDeclHandler m_endNamespaceDeclHandler;
  XML_NotStandaloneHandler m_notStandaloneHandler;
  XML_ExternalEntityRefHandler m_externalEntityRefHandler;
  XML_Parser m_externalEntityRefHandlerArg;
  XML_SkippedEntityHandler m_skippedEntityHandler;
  XML_UnknownEncodingHandler m_unknownEncodingHandler;
  XML_ElementDeclHandler m_elementDeclHandler;
  XML_AttlistDeclHandler m_attlistDeclHandler;
  XML_EntityDeclHandler m_entityDeclHandler;
  XML_XmlDeclHandler m_xmlDeclHandler;
  const ENCODING *m_encoding;
  INIT_ENCODING m_initEncoding;
  const ENCODING *m_internalEncoding;
  const XML_Char *m_protocolEncodingName;
  XML_Bool m_ns;
  XML_Bool m_ns_triplets;
  void *m_unknownEncodingMem;
  void *m_unknownEncodingData;
  void *m_unknownEncodingHandlerData;
  void (*m_unknownEncodingRelease)(void *);
  PROLOG_STATE m_prologState;
  Processor *m_processor;
  enum XML_Error m_errorCode;
  const char *m_eventPtr;
  const char *m_eventEndPtr;
  const char *m_positionPtr;
  OPEN_INTERNAL_ENTITY *m_openInternalEntities;
  XML_Bool m_defaultExpandInternalEntities;
  int m_tagLevel;
  ENTITY *m_declEntity;
  const XML_Char *m_doctypeName;
  const XML_Char *m_doctypeSysid;
  const XML_Char *m_doctypePubid;
  const XML_Char *m_declAttributeType;
  const XML_Char *m_declNotationName;
  const XML_Char *m_declNotationPublicId;
  ELEMENT_TYPE *m_declElementType;
  ATTRIBUTE_ID *m_declAttributeId;
  XML_Bool m_declAttributeIsCdata;
  XML_Bool m_declAttributeIsId;
  DTD *m_dtd;
  const XML_Char *m_curBase;
  TAG *m_tagStack;
  TAG *m_freeTagList;
  BINDING *m_inheritedBindings;
  BINDING *m_freeBindingList;
  int m_attsSize;
  int m_nSpecifiedAtts;
  int m_idAttIndex;
  ATTRIBUTE *m_atts;
  POSITION m_position;
  STRING_POOL m_tempPool;
  STRING_POOL m_temp2Pool;
  char *m_groupConnector;
  unsigned m_groupSize;
  XML_Char m_namespaceSeparator;
  XML_Parser m_parentParser;
#ifdef XML_DTD
  XML_Bool m_isParamEntity;
  XML_Bool m_useForeignDTD;
  enum XML_ParamEntityParsing m_paramEntityParsing;
#endif
};

#define MALLOC(s) (parser->m_mem.malloc_fcn((s)))
#define REALLOC(p,s) (parser->m_mem.realloc_fcn((p),(s)))
#define FREE(p) (parser->m_mem.free_fcn((p)))

#define userData (parser->m_userData)
#define handlerArg (parser->m_handlerArg)
#define startElementHandler (parser->m_startElementHandler)
#define endElementHandler (parser->m_endElementHandler)
#define characterDataHandler (parser->m_characterDataHandler)
#define processingInstructionHandler \
        (parser->m_processingInstructionHandler)
#define commentHandler (parser->m_commentHandler)
#define startCdataSectionHandler \
        (parser->m_startCdataSectionHandler)
#define endCdataSectionHandler (parser->m_endCdataSectionHandler)
#define defaultHandler (parser->m_defaultHandler)
#define startDoctypeDeclHandler (parser->m_startDoctypeDeclHandler)
#define endDoctypeDeclHandler (parser->m_endDoctypeDeclHandler)
#define unparsedEntityDeclHandler \
        (parser->m_unparsedEntityDeclHandler)
#define notationDeclHandler (parser->m_notationDeclHandler)
#define startNamespaceDeclHandler \
        (parser->m_startNamespaceDeclHandler)
#define endNamespaceDeclHandler (parser->m_endNamespaceDeclHandler)
#define notStandaloneHandler (parser->m_notStandaloneHandler)
#define externalEntityRefHandler \
        (parser->m_externalEntityRefHandler)
#define externalEntityRefHandlerArg \
        (parser->m_externalEntityRefHandlerArg)
#define internalEntityRefHandler \
        (parser->m_internalEntityRefHandler)
#define skippedEntityHandler (parser->m_skippedEntityHandler)
#define unknownEncodingHandler (parser->m_unknownEncodingHandler)
#define elementDeclHandler (parser->m_elementDeclHandler)
#define attlistDeclHandler (parser->m_attlistDeclHandler)
#define entityDeclHandler (parser->m_entityDeclHandler)
#define xmlDeclHandler (parser->m_xmlDeclHandler)
#define encoding (parser->m_encoding)
#define initEncoding (parser->m_initEncoding)
#define internalEncoding (parser->m_internalEncoding)
#define unknownEncodingMem (parser->m_unknownEncodingMem)
#define unknownEncodingData (parser->m_unknownEncodingData)
#define unknownEncodingHandlerData \
  (parser->m_unknownEncodingHandlerData)
#define unknownEncodingRelease (parser->m_unknownEncodingRelease)
#define protocolEncodingName (parser->m_protocolEncodingName)
#define ns (parser->m_ns)
#define ns_triplets (parser->m_ns_triplets)
#define prologState (parser->m_prologState)
#define processor (parser->m_processor)
#define errorCode (parser->m_errorCode)
#define eventPtr (parser->m_eventPtr)
#define eventEndPtr (parser->m_eventEndPtr)
#define positionPtr (parser->m_positionPtr)
#define position (parser->m_position)
#define openInternalEntities (parser->m_openInternalEntities)
#define defaultExpandInternalEntities \
        (parser->m_defaultExpandInternalEntities)
#define tagLevel (parser->m_tagLevel)
#define buffer (parser->m_buffer)
#define bufferPtr (parser->m_bufferPtr)
#define bufferEnd (parser->m_bufferEnd)
#define parseEndByteIndex (parser->m_parseEndByteIndex)
#define parseEndPtr (parser->m_parseEndPtr)
#define bufferLim (parser->m_bufferLim)
#define dataBuf (parser->m_dataBuf)
#define dataBufEnd (parser->m_dataBufEnd)
#define _dtd (parser->m_dtd)
#define curBase (parser->m_curBase)
#define declEntity (parser->m_declEntity)
#define doctypeName (parser->m_doctypeName)
#define doctypeSysid (parser->m_doctypeSysid)
#define doctypePubid (parser->m_doctypePubid)
#define declAttributeType (parser->m_declAttributeType)
#define declNotationName (parser->m_declNotationName)
#define declNotationPublicId (parser->m_declNotationPublicId)
#define declElementType (parser->m_declElementType)
#define declAttributeId (parser->m_declAttributeId)
#define declAttributeIsCdata (parser->m_declAttributeIsCdata)
#define declAttributeIsId (parser->m_declAttributeIsId)
#define freeTagList (parser->m_freeTagList)
#define freeBindingList (parser->m_freeBindingList)
#define inheritedBindings (parser->m_inheritedBindings)
#define tagStack (parser->m_tagStack)
#define atts (parser->m_atts)
#define attsSize (parser->m_attsSize)
#define nSpecifiedAtts (parser->m_nSpecifiedAtts)
#define idAttIndex (parser->m_idAttIndex)
#define tempPool (parser->m_tempPool)
#define temp2Pool (parser->m_temp2Pool)
#define groupConnector (parser->m_groupConnector)
#define groupSize (parser->m_groupSize)
#define namespaceSeparator (parser->m_namespaceSeparator)
#define parentParser (parser->m_parentParser)
#ifdef XML_DTD
#define isParamEntity (parser->m_isParamEntity)
#define useForeignDTD (parser->m_useForeignDTD)
#define paramEntityParsing (parser->m_paramEntityParsing)
#endif /* XML_DTD */

#define parsing \
  (parentParser \
    ? \
    (isParamEntity \
      ? \
      (processor != externalParEntInitProcessor) \
      : \
      (processor != externalEntityInitProcessor)) \
    : \
    (processor != prologInitProcessor))

XML_Parser
XML_ParserCreate(const XML_Char *encodingName)
{
  return XML_ParserCreate_MM(encodingName, NULL, NULL);
}

XML_Parser
XML_ParserCreateNS(const XML_Char *encodingName, XML_Char nsSep)
{
  XML_Char tmp[2];
  *tmp = nsSep;
  return XML_ParserCreate_MM(encodingName, NULL, tmp);
}

static const XML_Char implicitContext[] = {
  'x', 'm', 'l', '=', 'h', 't', 't', 'p', ':', '/', '/',
  'w', 'w', 'w', '.', 'w', '3', '.', 'o', 'r', 'g', '/',
  'X', 'M', 'L', '/', '1', '9', '9', '8', '/',
  'n', 'a', 'm', 'e', 's', 'p', 'a', 'c', 'e', '\0'
};

XML_Parser
XML_ParserCreate_MM(const XML_Char *encodingName,
                    const XML_Memory_Handling_Suite *memsuite,
                    const XML_Char *nameSep)
{
  XML_Parser parser = parserCreate(encodingName, memsuite, nameSep, NULL);
  if (parser != NULL && ns) {
    /* implicit context only set for root parser, since child
       parsers (i.e. external entity parsers) will inherit it
    */
    if (!setContext(parser, implicitContext)) {
      XML_ParserFree(parser);
      return NULL;
    }
  }
  return parser;
}

static XML_Parser
parserCreate(const XML_Char *encodingName,
             const XML_Memory_Handling_Suite *memsuite,
             const XML_Char *nameSep,
             DTD *dtd)
{
  XML_Parser parser;

  if (memsuite) {
    XML_Memory_Handling_Suite *mtemp;
    parser = (XML_Parser)
      memsuite->malloc_fcn(sizeof(struct XML_ParserStruct));
    if (parser != NULL) {
      mtemp = (XML_Memory_Handling_Suite *)&(parser->m_mem);
      mtemp->malloc_fcn = memsuite->malloc_fcn;
      mtemp->realloc_fcn = memsuite->realloc_fcn;
      mtemp->free_fcn = memsuite->free_fcn;
    }
  }
  else {
    XML_Memory_Handling_Suite *mtemp;
    parser = (XML_Parser)malloc(sizeof(struct XML_ParserStruct));
    if (parser != NULL) {
      mtemp = (XML_Memory_Handling_Suite *)&(parser->m_mem);
      mtemp->malloc_fcn = malloc;
      mtemp->realloc_fcn = realloc;
      mtemp->free_fcn = free;
    }
  }

  if (!parser)
    return parser;

  buffer = NULL;
  bufferLim = NULL;

  attsSize = INIT_ATTS_SIZE;
  atts = (ATTRIBUTE *)MALLOC(attsSize * sizeof(ATTRIBUTE));
  if (atts == NULL) {
    FREE(parser);
    return NULL;
  }
  dataBuf = (XML_Char *)MALLOC(INIT_DATA_BUF_SIZE * sizeof(XML_Char));
  if (dataBuf == NULL) {
    FREE(atts);
    FREE(parser);
    return NULL;
  }
  dataBufEnd = dataBuf + INIT_DATA_BUF_SIZE;

  if (dtd)
    _dtd = dtd;
  else {
    _dtd = dtdCreate(&parser->m_mem);
    if (_dtd == NULL) {
      FREE(dataBuf);
      FREE(atts);
      FREE(parser);
      return NULL;
    }
  }

  freeBindingList = NULL;
  freeTagList = NULL;

  groupSize = 0;
  groupConnector = NULL;

  unknownEncodingHandler = NULL;
  unknownEncodingHandlerData = NULL;

  namespaceSeparator = '!';
  ns = XML_FALSE;
  ns_triplets = XML_FALSE;

  poolInit(&tempPool, &(parser->m_mem));
  poolInit(&temp2Pool, &(parser->m_mem));
  parserInit(parser, encodingName);

  if (encodingName && !protocolEncodingName) {
    XML_ParserFree(parser);
    return NULL;
  }

  if (nameSep) {
    ns = XML_TRUE;
    internalEncoding = XmlGetInternalEncodingNS();
    namespaceSeparator = *nameSep;
  }
  else {
    internalEncoding = XmlGetInternalEncoding();
  }

  return parser;
}

static void
parserInit(XML_Parser parser, const XML_Char *encodingName)
{
  processor = prologInitProcessor;
  XmlPrologStateInit(&prologState);
  protocolEncodingName = (encodingName != NULL
                          ? poolCopyString(&tempPool, encodingName)
                          : NULL);
  curBase = NULL;
  XmlInitEncoding(&initEncoding, &encoding, 0);
  userData = NULL;
  handlerArg = NULL;
  startElementHandler = NULL;
  endElementHandler = NULL;
  characterDataHandler = NULL;
  processingInstructionHandler = NULL;
  commentHandler = NULL;
  startCdataSectionHandler = NULL;
  endCdataSectionHandler = NULL;
  defaultHandler = NULL;
  startDoctypeDeclHandler = NULL;
  endDoctypeDeclHandler = NULL;
  unparsedEntityDeclHandler = NULL;
  notationDeclHandler = NULL;
  startNamespaceDeclHandler = NULL;
  endNamespaceDeclHandler = NULL;
  notStandaloneHandler = NULL;
  externalEntityRefHandler = NULL;
  externalEntityRefHandlerArg = parser;
  skippedEntityHandler = NULL;
  elementDeclHandler = NULL;
  attlistDeclHandler = NULL;
  entityDeclHandler = NULL;
  xmlDeclHandler = NULL;
  bufferPtr = buffer;
  bufferEnd = buffer;
  parseEndByteIndex = 0;
  parseEndPtr = NULL;
  declElementType = NULL;
  declAttributeId = NULL;
  declEntity = NULL;
  doctypeName = NULL;
  doctypeSysid = NULL;
  doctypePubid = NULL;
  declAttributeType = NULL;
  declNotationName = NULL;
  declNotationPublicId = NULL;
  declAttributeIsCdata = XML_FALSE;
  declAttributeIsId = XML_FALSE;
  memset(&position, 0, sizeof(POSITION));
  errorCode = XML_ERROR_NONE;
  eventPtr = NULL;
  eventEndPtr = NULL;
  positionPtr = NULL;
  openInternalEntities = 0;
  defaultExpandInternalEntities = XML_TRUE;
  tagLevel = 0;
  tagStack = NULL;
  inheritedBindings = NULL;
  nSpecifiedAtts = 0;
  unknownEncodingMem = NULL;
  unknownEncodingRelease = NULL;
  unknownEncodingData = NULL;
  parentParser = NULL;
#ifdef XML_DTD
  isParamEntity = XML_FALSE;
  useForeignDTD = XML_FALSE;
  paramEntityParsing = XML_PARAM_ENTITY_PARSING_NEVER;
#endif
}

/* moves list of bindings to freeBindingList */
static void FASTCALL
moveToFreeBindingList(XML_Parser parser, BINDING *bindings)
{
  while (bindings) {
    BINDING *b = bindings;
    bindings = bindings->nextTagBinding;
    b->nextTagBinding = freeBindingList;
    freeBindingList = b;
  }
}

XML_Bool
XML_ParserReset(XML_Parser parser, const XML_Char *encodingName)
{
  TAG *tStk;
  if (parentParser)
    return XML_FALSE;
  /* move tagStack to freeTagList */
  tStk = tagStack;
  while (tStk) {
    TAG *tag = tStk;
    tStk = tStk->parent;
    tag->parent = freeTagList;
    moveToFreeBindingList(parser, tag->bindings);
    tag->bindings = NULL;
    freeTagList = tag;
  }
  moveToFreeBindingList(parser, inheritedBindings);
  if (unknownEncodingMem)
    FREE(unknownEncodingMem);
  if (unknownEncodingRelease)
    unknownEncodingRelease(unknownEncodingData);
  poolClear(&tempPool);
  poolClear(&temp2Pool);
  parserInit(parser, encodingName);
  dtdReset(_dtd, &parser->m_mem);
  return setContext(parser, implicitContext);
}

enum XML_Status
XML_SetEncoding(XML_Parser parser, const XML_Char *encodingName)
{
  /* Block after XML_Parse()/XML_ParseBuffer() has been called.
     XXX There's no way for the caller to determine which of the
     XXX possible error cases caused the XML_STATUS_ERROR return.
  */
  if (parsing)
    return XML_STATUS_ERROR;
  if (encodingName == NULL)
    protocolEncodingName = NULL;
  else {
    protocolEncodingName = poolCopyString(&tempPool, encodingName);
    if (!protocolEncodingName)
      return XML_STATUS_ERROR;
  }
  return XML_STATUS_OK;
}

XML_Parser
XML_ExternalEntityParserCreate(XML_Parser oldParser,
                               const XML_Char *context,
                               const XML_Char *encodingName)
{
  XML_Parser parser = oldParser;
  DTD *newDtd = NULL;
  DTD *oldDtd = _dtd;
  XML_StartElementHandler oldStartElementHandler = startElementHandler;
  XML_EndElementHandler oldEndElementHandler = endElementHandler;
  XML_CharacterDataHandler oldCharacterDataHandler = characterDataHandler;
  XML_ProcessingInstructionHandler oldProcessingInstructionHandler
      = processingInstructionHandler;
  XML_CommentHandler oldCommentHandler = commentHandler;
  XML_StartCdataSectionHandler oldStartCdataSectionHandler
      = startCdataSectionHandler;
  XML_EndCdataSectionHandler oldEndCdataSectionHandler
      = endCdataSectionHandler;
  XML_DefaultHandler oldDefaultHandler = defaultHandler;
  XML_UnparsedEntityDeclHandler oldUnparsedEntityDeclHandler
      = unparsedEntityDeclHandler;
  XML_NotationDeclHandler oldNotationDeclHandler = notationDeclHandler;
  XML_StartNamespaceDeclHandler oldStartNamespaceDeclHandler
      = startNamespaceDeclHandler;
  XML_EndNamespaceDeclHandler oldEndNamespaceDeclHandler
      = endNamespaceDeclHandler;
  XML_NotStandaloneHandler oldNotStandaloneHandler = notStandaloneHandler;
  XML_ExternalEntityRefHandler oldExternalEntityRefHandler
      = externalEntityRefHandler;
  XML_SkippedEntityHandler oldSkippedEntityHandler = skippedEntityHandler;
  XML_UnknownEncodingHandler oldUnknownEncodingHandler
      = unknownEncodingHandler;
  XML_ElementDeclHandler oldElementDeclHandler = elementDeclHandler;
  XML_AttlistDeclHandler oldAttlistDeclHandler = attlistDeclHandler;
  XML_EntityDeclHandler oldEntityDeclHandler = entityDeclHandler;
  XML_XmlDeclHandler oldXmlDeclHandler = xmlDeclHandler;
  ELEMENT_TYPE * oldDeclElementType = declElementType;

  void *oldUserData = userData;
  void *oldHandlerArg = handlerArg;
  XML_Bool oldDefaultExpandInternalEntities = defaultExpandInternalEntities;
  XML_Parser oldExternalEntityRefHandlerArg = externalEntityRefHandlerArg;
#ifdef XML_DTD
  enum XML_ParamEntityParsing oldParamEntityParsing = paramEntityParsing;
  int oldInEntityValue = prologState.inEntityValue;
#endif
  XML_Bool oldns_triplets = ns_triplets;

#ifdef XML_DTD
  if (!context)
    newDtd = oldDtd;
#endif /* XML_DTD */

  /* Note that the magical uses of the pre-processor to make field
     access look more like C++ require that `parser' be overwritten
     here.  This makes this function more painful to follow than it
     would be otherwise.
  */
  if (ns) {
    XML_Char tmp[2];
    *tmp = namespaceSeparator;
    parser = parserCreate(encodingName, &parser->m_mem, tmp, newDtd);
  }
  else {
    parser = parserCreate(encodingName, &parser->m_mem, NULL, newDtd);
  }

  if (!parser)
    return NULL;

  startElementHandler = oldStartElementHandler;
  endElementHandler = oldEndElementHandler;
  characterDataHandler = oldCharacterDataHandler;
  processingInstructionHandler = oldProcessingInstructionHandler;
  commentHandler = oldCommentHandler;
  startCdataSectionHandler = oldStartCdataSectionHandler;
  endCdataSectionHandler = oldEndCdataSectionHandler;
  defaultHandler = oldDefaultHandler;
  unparsedEntityDeclHandler = oldUnparsedEntityDeclHandler;
  notationDeclHandler = oldNotationDeclHandler;
  startNamespaceDeclHandler = oldStartNamespaceDeclHandler;
  endNamespaceDeclHandler = oldEndNamespaceDeclHandler;
  notStandaloneHandler = oldNotStandaloneHandler;
  externalEntityRefHandler = oldExternalEntityRefHandler;
  skippedEntityHandler = oldSkippedEntityHandler;
  unknownEncodingHandler = oldUnknownEncodingHandler;
  elementDeclHandler = oldElementDeclHandler;
  attlistDeclHandler = oldAttlistDeclHandler;
  entityDeclHandler = oldEntityDeclHandler;
  xmlDeclHandler = oldXmlDeclHandler;
  declElementType = oldDeclElementType;
  userData = oldUserData;
  if (oldUserData == oldHandlerArg)
    handlerArg = userData;
  else
    handlerArg = parser;
  if (oldExternalEntityRefHandlerArg != oldParser)
    externalEntityRefHandlerArg = oldExternalEntityRefHandlerArg;
  defaultExpandInternalEntities = oldDefaultExpandInternalEntities;
  ns_triplets = oldns_triplets;
  parentParser = oldParser;
#ifdef XML_DTD
  paramEntityParsing = oldParamEntityParsing;
  prologState.inEntityValue = oldInEntityValue;
  if (context) {
#endif /* XML_DTD */
    if (!dtdCopy(_dtd, oldDtd, &parser->m_mem)
      || !setContext(parser, context)) {
      XML_ParserFree(parser);
      return NULL;
    }
    processor = externalEntityInitProcessor;
#ifdef XML_DTD
  }
  else {
    /* The DTD instance referenced by _dtd is shared between the document's
       root parser and external PE parsers, therefore one does not need to
       call setContext. In addition, one also *must* not call setContext,
       because this would overwrite existing prefix->binding pointers in
       _dtd with ones that get destroyed with the external PE parser.
       This would leave those prefixes with dangling pointers.
    */
    isParamEntity = XML_TRUE;
    XmlPrologStateInitExternalEntity(&prologState);
    processor = externalParEntInitProcessor;
  }
#endif /* XML_DTD */
  return parser;
}

static void FASTCALL
destroyBindings(BINDING *bindings, XML_Parser parser)
{
  for (;;) {
    BINDING *b = bindings;
    if (!b)
      break;
    bindings = b->nextTagBinding;
    FREE(b->uri);
    FREE(b);
  }
}

void
XML_ParserFree(XML_Parser parser)
{
  for (;;) {
    TAG *p;
    if (tagStack == NULL) {
      if (freeTagList == NULL)
        break;
      tagStack = freeTagList;
      freeTagList = NULL;
    }
    p = tagStack;
    tagStack = tagStack->parent;
    FREE(p->buf);
    destroyBindings(p->bindings, parser);
    FREE(p);
  }
  destroyBindings(freeBindingList, parser);
  destroyBindings(inheritedBindings, parser);
  poolDestroy(&tempPool);
  poolDestroy(&temp2Pool);
#ifdef XML_DTD
  /* external parameter entity parsers share the DTD structure
     parser->m_dtd with the root parser, so we must not destroy it
  */
  if (!isParamEntity && _dtd)
#else
  if (_dtd)
#endif /* XML_DTD */
    dtdDestroy(_dtd, (XML_Bool)!parentParser, &parser->m_mem);
  FREE((void *)atts);
  if (groupConnector)
    FREE(groupConnector);
  if (buffer)
    FREE(buffer);
  FREE(dataBuf);
  if (unknownEncodingMem)
    FREE(unknownEncodingMem);
  if (unknownEncodingRelease)
    unknownEncodingRelease(unknownEncodingData);
  FREE(parser);
}

void
XML_UseParserAsHandlerArg(XML_Parser parser)
{
  handlerArg = parser;
}

enum XML_Error
XML_UseForeignDTD(XML_Parser parser, XML_Bool useDTD)
{
#ifdef XML_DTD
  /* block after XML_Parse()/XML_ParseBuffer() has been called */
  if (parsing)
    return XML_ERROR_CANT_CHANGE_FEATURE_ONCE_PARSING;
  useForeignDTD = useDTD;
  return XML_ERROR_NONE;
#else
  return XML_ERROR_FEATURE_REQUIRES_XML_DTD;
#endif
}

void
XML_SetReturnNSTriplet(XML_Parser parser, int do_nst)
{
  /* block after XML_Parse()/XML_ParseBuffer() has been called */
  if (parsing)
    return;
  ns_triplets = do_nst ? XML_TRUE : XML_FALSE;
}

void
XML_SetUserData(XML_Parser parser, void *p)
{
  if (handlerArg == userData)
    handlerArg = userData = p;
  else
    userData = p;
}

enum XML_Status
XML_SetBase(XML_Parser parser, const XML_Char *p)
{
  if (p) {
    p = poolCopyString(&_dtd->pool, p);
    if (!p)
      return XML_STATUS_ERROR;
    curBase = p;
  }
  else
    curBase = NULL;
  return XML_STATUS_OK;
}

const XML_Char *
XML_GetBase(XML_Parser parser)
{
  return curBase;
}

int
XML_GetSpecifiedAttributeCount(XML_Parser parser)
{
  return nSpecifiedAtts;
}

int
XML_GetIdAttributeIndex(XML_Parser parser)
{
  return idAttIndex;
}

void
XML_SetElementHandler(XML_Parser parser,
                      XML_StartElementHandler start,
                      XML_EndElementHandler end)
{
  startElementHandler = start;
  endElementHandler = end;
}

void
XML_SetStartElementHandler(XML_Parser parser,
                           XML_StartElementHandler start) {
  startElementHandler = start;
}

void
XML_SetEndElementHandler(XML_Parser parser,
                         XML_EndElementHandler end) {
  endElementHandler = end;
}

void
XML_SetCharacterDataHandler(XML_Parser parser,
                            XML_CharacterDataHandler handler)
{
  characterDataHandler = handler;
}

void
XML_SetProcessingInstructionHandler(XML_Parser parser,
                                    XML_ProcessingInstructionHandler handler)
{
  processingInstructionHandler = handler;
}

void
XML_SetCommentHandler(XML_Parser parser,
                      XML_CommentHandler handler)
{
  commentHandler = handler;
}

void
XML_SetCdataSectionHandler(XML_Parser parser,
                           XML_StartCdataSectionHandler start,
                           XML_EndCdataSectionHandler end)
{
  startCdataSectionHandler = start;
  endCdataSectionHandler = end;
}

void
XML_SetStartCdataSectionHandler(XML_Parser parser,
                                XML_StartCdataSectionHandler start) {
  startCdataSectionHandler = start;
}

void
XML_SetEndCdataSectionHandler(XML_Parser parser,
                              XML_EndCdataSectionHandler end) {
  endCdataSectionHandler = end;
}

void
XML_SetDefaultHandler(XML_Parser parser,
                      XML_DefaultHandler handler)
{
  defaultHandler = handler;
  defaultExpandInternalEntities = XML_FALSE;
}

void
XML_SetDefaultHandlerExpand(XML_Parser parser,
                            XML_DefaultHandler handler)
{
  defaultHandler = handler;
  defaultExpandInternalEntities = XML_TRUE;
}

void
XML_SetDoctypeDeclHandler(XML_Parser parser,
                          XML_StartDoctypeDeclHandler start,
                          XML_EndDoctypeDeclHandler end)
{
  startDoctypeDeclHandler = start;
  endDoctypeDeclHandler = end;
}

void
XML_SetStartDoctypeDeclHandler(XML_Parser parser,
                               XML_StartDoctypeDeclHandler start) {
  startDoctypeDeclHandler = start;
}

void
XML_SetEndDoctypeDeclHandler(XML_Parser parser,
                             XML_EndDoctypeDeclHandler end) {
  endDoctypeDeclHandler = end;
}

void
XML_SetUnparsedEntityDeclHandler(XML_Parser parser,
                                 XML_UnparsedEntityDeclHandler handler)
{
  unparsedEntityDeclHandler = handler;
}

void
XML_SetNotationDeclHandler(XML_Parser parser,
                           XML_NotationDeclHandler handler)
{
  notationDeclHandler = handler;
}

void
XML_SetNamespaceDeclHandler(XML_Parser parser,
                            XML_StartNamespaceDeclHandler start,
                            XML_EndNamespaceDeclHandler end)
{
  startNamespaceDeclHandler = start;
  endNamespaceDeclHandler = end;
}

void
XML_SetStartNamespaceDeclHandler(XML_Parser parser,
                                 XML_StartNamespaceDeclHandler start) {
  startNamespaceDeclHandler = start;
}

void
XML_SetEndNamespaceDeclHandler(XML_Parser parser,
                               XML_EndNamespaceDeclHandler end) {
  endNamespaceDeclHandler = end;
}

void
XML_SetNotStandaloneHandler(XML_Parser parser,
                            XML_NotStandaloneHandler handler)
{
  notStandaloneHandler = handler;
}

void
XML_SetExternalEntityRefHandler(XML_Parser parser,
                                XML_ExternalEntityRefHandler handler)
{
  externalEntityRefHandler = handler;
}

void
XML_SetExternalEntityRefHandlerArg(XML_Parser parser, void *arg)
{
  if (arg)
    externalEntityRefHandlerArg = (XML_Parser)arg;
  else
    externalEntityRefHandlerArg = parser;
}

void
XML_SetSkippedEntityHandler(XML_Parser parser,
                            XML_SkippedEntityHandler handler)
{
  skippedEntityHandler = handler;
}

void
XML_SetUnknownEncodingHandler(XML_Parser parser,
                              XML_UnknownEncodingHandler handler,
                              void *data)
{
  unknownEncodingHandler = handler;
  unknownEncodingHandlerData = data;
}

void
XML_SetElementDeclHandler(XML_Parser parser,
                          XML_ElementDeclHandler eldecl)
{
  elementDeclHandler = eldecl;
}

void
XML_SetAttlistDeclHandler(XML_Parser parser,
                          XML_AttlistDeclHandler attdecl)
{
  attlistDeclHandler = attdecl;
}

void
XML_SetEntityDeclHandler(XML_Parser parser,
                         XML_EntityDeclHandler handler)
{
  entityDeclHandler = handler;
}

void
XML_SetXmlDeclHandler(XML_Parser parser,
                      XML_XmlDeclHandler handler) {
  xmlDeclHandler = handler;
}

int
XML_SetParamEntityParsing(XML_Parser parser,
                          enum XML_ParamEntityParsing peParsing)
{
  /* block after XML_Parse()/XML_ParseBuffer() has been called */
  if (parsing)
    return 0;
#ifdef XML_DTD
  paramEntityParsing = peParsing;
  return 1;
#else
  return peParsing == XML_PARAM_ENTITY_PARSING_NEVER;
#endif
}

enum XML_Status
XML_Parse(XML_Parser parser, const char *s, int len, int isFinal)
{
  if (len == 0) {
    if (!isFinal)
      return XML_STATUS_OK;
    positionPtr = bufferPtr;
    errorCode = processor(parser, bufferPtr, parseEndPtr = bufferEnd, 0);
    if (errorCode == XML_ERROR_NONE)
      return XML_STATUS_OK;
    eventEndPtr = eventPtr;
    processor = errorProcessor;
    return XML_STATUS_ERROR;
  }
#ifndef XML_CONTEXT_BYTES
  else if (bufferPtr == bufferEnd) {
    const char *end;
    int nLeftOver;
    parseEndByteIndex += len;
    positionPtr = s;
    if (isFinal) {
      errorCode = processor(parser, s, parseEndPtr = s + len, 0);
      if (errorCode == XML_ERROR_NONE)
        return XML_STATUS_OK;
      eventEndPtr = eventPtr;
      processor = errorProcessor;
      return XML_STATUS_ERROR;
    }
    errorCode = processor(parser, s, parseEndPtr = s + len, &end);
    if (errorCode != XML_ERROR_NONE) {
      eventEndPtr = eventPtr;
      processor = errorProcessor;
      return XML_STATUS_ERROR;
    }
    XmlUpdatePosition(encoding, positionPtr, end, &position);
    positionPtr = end;
    nLeftOver = s + len - end;
    if (nLeftOver) {
      if (buffer == NULL || nLeftOver > bufferLim - buffer) {
        /* FIXME avoid integer overflow */
        char *temp;
        temp = (buffer == NULL
                ? (char *)MALLOC(len * 2)
                : (char *)REALLOC(buffer, len * 2));
        if (temp == NULL) {
          errorCode = XML_ERROR_NO_MEMORY;
          return XML_STATUS_ERROR;
        }
        buffer = temp;
        if (!buffer) {
          errorCode = XML_ERROR_NO_MEMORY;
          eventPtr = eventEndPtr = NULL;
          processor = errorProcessor;
          return XML_STATUS_ERROR;
        }
        bufferLim = buffer + len * 2;
      }
      memcpy(buffer, end, nLeftOver);
      bufferPtr = buffer;
      bufferEnd = buffer + nLeftOver;
    }
    return XML_STATUS_OK;
  }
#endif  /* not defined XML_CONTEXT_BYTES */
  else {
    void *buff = XML_GetBuffer(parser, len);
    if (buff == NULL)
      return XML_STATUS_ERROR;
    else {
      memcpy(buff, s, len);
      return XML_ParseBuffer(parser, len, isFinal);
    }
  }
}

enum XML_Status
XML_ParseBuffer(XML_Parser parser, int len, int isFinal)
{
  const char *start = bufferPtr;
  positionPtr = start;
  bufferEnd += len;
  parseEndByteIndex += len;
  errorCode = processor(parser, start, parseEndPtr = bufferEnd,
                        isFinal ? (const char **)NULL : &bufferPtr);
  if (errorCode == XML_ERROR_NONE) {
    if (!isFinal) {
      XmlUpdatePosition(encoding, positionPtr, bufferPtr, &position);
      positionPtr = bufferPtr;
    }
    return XML_STATUS_OK;
  }
  else {
    eventEndPtr = eventPtr;
    processor = errorProcessor;
    return XML_STATUS_ERROR;
  }
}

void *
XML_GetBuffer(XML_Parser parser, int len)
{
  if (len > bufferLim - bufferEnd) {
    /* FIXME avoid integer overflow */
    int neededSize = len + (bufferEnd - bufferPtr);
#ifdef XML_CONTEXT_BYTES
    int keep = bufferPtr - buffer;

    if (keep > XML_CONTEXT_BYTES)
      keep = XML_CONTEXT_BYTES;
    neededSize += keep;
#endif  /* defined XML_CONTEXT_BYTES */
    if (neededSize  <= bufferLim - buffer) {
#ifdef XML_CONTEXT_BYTES
      if (keep < bufferPtr - buffer) {
        int offset = (bufferPtr - buffer) - keep;
        memmove(buffer, &buffer[offset], bufferEnd - bufferPtr + keep);
        bufferEnd -= offset;
        bufferPtr -= offset;
      }
#else
      memmove(buffer, bufferPtr, bufferEnd - bufferPtr);
      bufferEnd = buffer + (bufferEnd - bufferPtr);
      bufferPtr = buffer;
#endif  /* not defined XML_CONTEXT_BYTES */
    }
    else {
      char *newBuf;
      int bufferSize = bufferLim - bufferPtr;
      if (bufferSize == 0)
        bufferSize = INIT_BUFFER_SIZE;
      do {
        bufferSize *= 2;
      } while (bufferSize < neededSize);
      newBuf = (char *)MALLOC(bufferSize);
      if (newBuf == 0) {
        errorCode = XML_ERROR_NO_MEMORY;
        return NULL;
      }
      bufferLim = newBuf + bufferSize;
#ifdef XML_CONTEXT_BYTES
      if (bufferPtr) {
        int keep = bufferPtr - buffer;
        if (keep > XML_CONTEXT_BYTES)
          keep = XML_CONTEXT_BYTES;
        memcpy(newBuf, &bufferPtr[-keep], bufferEnd - bufferPtr + keep);
        FREE(buffer);
        buffer = newBuf;
        bufferEnd = buffer + (bufferEnd - bufferPtr) + keep;
        bufferPtr = buffer + keep;
      }
      else {
        bufferEnd = newBuf + (bufferEnd - bufferPtr);
        bufferPtr = buffer = newBuf;
      }
#else
      if (bufferPtr) {
        memcpy(newBuf, bufferPtr, bufferEnd - bufferPtr);
        FREE(buffer);
      }
      bufferEnd = newBuf + (bufferEnd - bufferPtr);
      bufferPtr = buffer = newBuf;
#endif  /* not defined XML_CONTEXT_BYTES */
    }
  }
  return bufferEnd;
}

enum XML_Error
XML_GetErrorCode(XML_Parser parser)
{
  return errorCode;
}

long
XML_GetCurrentByteIndex(XML_Parser parser)
{
  if (eventPtr)
    return parseEndByteIndex - (parseEndPtr - eventPtr);
  return -1;
}

int
XML_GetCurrentByteCount(XML_Parser parser)
{
  if (eventEndPtr && eventPtr)
    return eventEndPtr - eventPtr;
  return 0;
}

const char *
XML_GetInputContext(XML_Parser parser, int *offset, int *size)
{
#ifdef XML_CONTEXT_BYTES
  if (eventPtr && buffer) {
    *offset = eventPtr - buffer;
    *size   = bufferEnd - buffer;
    return buffer;
  }
#endif /* defined XML_CONTEXT_BYTES */
  return (char *) 0;
}

int
XML_GetCurrentLineNumber(XML_Parser parser)
{
  if (eventPtr) {
    XmlUpdatePosition(encoding, positionPtr, eventPtr, &position);
    positionPtr = eventPtr;
  }
  return position.lineNumber + 1;
}

int
XML_GetCurrentColumnNumber(XML_Parser parser)
{
  if (eventPtr) {
    XmlUpdatePosition(encoding, positionPtr, eventPtr, &position);
    positionPtr = eventPtr;
  }
  return position.columnNumber;
}

void
XML_FreeContentModel(XML_Parser parser, XML_Content *model)
{
  FREE(model);
}

void *
XML_MemMalloc(XML_Parser parser, size_t size)
{
  return MALLOC(size);
}

void *
XML_MemRealloc(XML_Parser parser, void *ptr, size_t size)
{
  return REALLOC(ptr, size);
}

void
XML_MemFree(XML_Parser parser, void *ptr)
{
  FREE(ptr);
}

void
XML_DefaultCurrent(XML_Parser parser)
{
  if (defaultHandler) {
    if (openInternalEntities)
      reportDefault(parser,
                    internalEncoding,
                    openInternalEntities->internalEventPtr,
                    openInternalEntities->internalEventEndPtr);
    else
      reportDefault(parser, encoding, eventPtr, eventEndPtr);
  }
}

const XML_LChar *
XML_ErrorString(enum XML_Error code)
{
  static const XML_LChar *message[] = {
    0,
    XML_L("out of memory"),
    XML_L("syntax error"),
    XML_L("no element found"),
    XML_L("not well-formed (invalid token)"),
    XML_L("unclosed token"),
    XML_L("partial character"),
    XML_L("mismatched tag"),
    XML_L("duplicate attribute"),
    XML_L("junk after document element"),
    XML_L("illegal parameter entity reference"),
    XML_L("undefined entity"),
    XML_L("recursive entity reference"),
    XML_L("asynchronous entity"),
    XML_L("reference to invalid character number"),
    XML_L("reference to binary entity"),
    XML_L("reference to external entity in attribute"),
    XML_L("xml declaration not at start of external entity"),
    XML_L("unknown encoding"),
    XML_L("encoding specified in XML declaration is incorrect"),
    XML_L("unclosed CDATA section"),
    XML_L("error in processing external entity reference"),
    XML_L("document is not standalone"),
    XML_L("unexpected parser state - please send a bug report"),
    XML_L("entity declared in parameter entity"),
    XML_L("requested feature requires XML_DTD support in Expat"),
    XML_L("cannot change setting once parsing has begun")
  };
  if (code > 0 && code < sizeof(message)/sizeof(message[0]))
    return message[code];
  return NULL;
}

const XML_LChar *
XML_ExpatVersion(void) {

  /* V1 is used to string-ize the version number. However, it would
     string-ize the actual version macro *names* unless we get them
     substituted before being passed to V1. CPP is defined to expand
     a macro, then rescan for more expansions. Thus, we use V2 to expand
     the version macros, then CPP will expand the resulting V1() macro
     with the correct numerals. */
  /* ### I'm assuming cpp is portable in this respect... */

#define V1(a,b,c) XML_L(#a)XML_L(".")XML_L(#b)XML_L(".")XML_L(#c)
#define V2(a,b,c) XML_L("expat_")V1(a,b,c)

  return V2(XML_MAJOR_VERSION, XML_MINOR_VERSION, XML_MICRO_VERSION);

#undef V1
#undef V2
}

XML_Expat_Version
XML_ExpatVersionInfo(void)
{
  XML_Expat_Version version;

  version.major = XML_MAJOR_VERSION;
  version.minor = XML_MINOR_VERSION;
  version.micro = XML_MICRO_VERSION;

  return version;
}

const XML_Feature *
XML_GetFeatureList(void)
{
  static XML_Feature features[] = {
    {XML_FEATURE_SIZEOF_XML_CHAR,  XML_L("sizeof(XML_Char)")},
    {XML_FEATURE_SIZEOF_XML_LCHAR, XML_L("sizeof(XML_LChar)")},
#ifdef XML_UNICODE
    {XML_FEATURE_UNICODE,          XML_L("XML_UNICODE")},
#endif
#ifdef XML_UNICODE_WCHAR_T
    {XML_FEATURE_UNICODE_WCHAR_T,  XML_L("XML_UNICODE_WCHAR_T")},
#endif
#ifdef XML_DTD
    {XML_FEATURE_DTD,              XML_L("XML_DTD")},
#endif
#ifdef XML_CONTEXT_BYTES
    {XML_FEATURE_CONTEXT_BYTES,    XML_L("XML_CONTEXT_BYTES"),
     XML_CONTEXT_BYTES},
#endif
#ifdef XML_MIN_SIZE
    {XML_FEATURE_MIN_SIZE,         XML_L("XML_MIN_SIZE")},
#endif
    {XML_FEATURE_END,              NULL}
  };

  features[0].value = sizeof(XML_Char);
  features[1].value = sizeof(XML_LChar);
  return features;
}

/* Initially tag->rawName always points into the parse buffer;
   for those TAG instances opened while the current parse buffer was
   processed, and not yet closed, we need to store tag->rawName in a more
   permanent location, since the parse buffer is about to be discarded.
*/
static XML_Bool
storeRawNames(XML_Parser parser)
{
  TAG *tag = tagStack;
  while (tag) {
    int bufSize;
    int nameLen = sizeof(XML_Char) * (tag->name.strLen + 1);
    char *rawNameBuf = tag->buf + nameLen;
    /* Stop if already stored.  Since tagStack is a stack, we can stop
       at the first entry that has already been copied; everything
       below it in the stack is already been accounted for in a
       previous call to this function.
    */
    if (tag->rawName == rawNameBuf)
      break;
    /* For re-use purposes we need to ensure that the
       size of tag->buf is a multiple of sizeof(XML_Char).
    */
    bufSize = nameLen + ROUND_UP(tag->rawNameLength, sizeof(XML_Char));
    if (bufSize > tag->bufEnd - tag->buf) {
      char *temp = (char *)REALLOC(tag->buf, bufSize);
      if (temp == NULL)
        return XML_FALSE;
      /* if tag->name.str points to tag->buf (only when namespace
         processing is off) then we have to update it
      */
      if (tag->name.str == (XML_Char *)tag->buf)
        tag->name.str = (XML_Char *)temp;
      /* if tag->name.localPart is set (when namespace processing is on)
         then update it as well, since it will always point into tag->buf
      */
      if (tag->name.localPart)
        tag->name.localPart = (XML_Char *)temp + (tag->name.localPart -
                                                  (XML_Char *)tag->buf);
      tag->buf = temp;
      tag->bufEnd = temp + bufSize;
      rawNameBuf = temp + nameLen;
    }
    memcpy(rawNameBuf, tag->rawName, tag->rawNameLength);
    tag->rawName = rawNameBuf;
    tag = tag->parent;
  }
  return XML_TRUE;
}

static enum XML_Error PTRCALL
contentProcessor(XML_Parser parser,
                 const char *start,
                 const char *end,
                 const char **endPtr)
{
  enum XML_Error result =
    doContent(parser, 0, encoding, start, end, endPtr);
  if (result != XML_ERROR_NONE)
    return result;
  if (!storeRawNames(parser))
    return XML_ERROR_NO_MEMORY;
  return result;
}

static enum XML_Error PTRCALL
externalEntityInitProcessor(XML_Parser parser,
                            const char *start,
                            const char *end,
                            const char **endPtr)
{
  enum XML_Error result = initializeEncoding(parser);
  if (result != XML_ERROR_NONE)
    return result;
  processor = externalEntityInitProcessor2;
  return externalEntityInitProcessor2(parser, start, end, endPtr);
}

static enum XML_Error PTRCALL
externalEntityInitProcessor2(XML_Parser parser,
                             const char *start,
                             const char *end,
                             const char **endPtr)
{
  const char *next = start; /* XmlContentTok doesn't always set the last arg */
  int tok = XmlContentTok(encoding, start, end, &next);
  switch (tok) {
  case XML_TOK_BOM:
    /* If we are at the end of the buffer, this would cause the next stage,
       i.e. externalEntityInitProcessor3, to pass control directly to
       doContent (by detecting XML_TOK_NONE) without processing any xml text
       declaration - causing the error XML_ERROR_MISPLACED_XML_PI in doContent.
    */
    if (next == end && endPtr) {
      *endPtr = next;
      return XML_ERROR_NONE;
    }
    start = next;
    break;
  case XML_TOK_PARTIAL:
    if (endPtr) {
      *endPtr = start;
      return XML_ERROR_NONE;
    }
    eventPtr = start;
    return XML_ERROR_UNCLOSED_TOKEN;
  case XML_TOK_PARTIAL_CHAR:
    if (endPtr) {
      *endPtr = start;
      return XML_ERROR_NONE;
    }
    eventPtr = start;
    return XML_ERROR_PARTIAL_CHAR;
  }
  processor = externalEntityInitProcessor3;
  return externalEntityInitProcessor3(parser, start, end, endPtr);
}

static enum XML_Error PTRCALL
externalEntityInitProcessor3(XML_Parser parser,
                             const char *start,
                             const char *end,
                             const char **endPtr)
{
  const char *next = start; /* XmlContentTok doesn't always set the last arg */
  int tok = XmlContentTok(encoding, start, end, &next);
  switch (tok) {
  case XML_TOK_XML_DECL:
    {
      enum XML_Error result = processXmlDecl(parser, 1, start, next);
      if (result != XML_ERROR_NONE)
        return result;
      start = next;
    }
    break;
  case XML_TOK_PARTIAL:
    if (endPtr) {
      *endPtr = start;
      return XML_ERROR_NONE;
    }
    eventPtr = start;
    return XML_ERROR_UNCLOSED_TOKEN;
  case XML_TOK_PARTIAL_CHAR:
    if (endPtr) {
      *endPtr = start;
      return XML_ERROR_NONE;
    }
    eventPtr = start;
    return XML_ERROR_PARTIAL_CHAR;
  }
  processor = externalEntityContentProcessor;
  tagLevel = 1;
  return externalEntityContentProcessor(parser, start, end, endPtr);
}

static enum XML_Error PTRCALL
externalEntityContentProcessor(XML_Parser parser,
                               const char *start,
                               const char *end,
                               const char **endPtr)
{
  enum XML_Error result =
    doContent(parser, 1, encoding, start, end, endPtr);
  if (result != XML_ERROR_NONE)
    return result;
  if (!storeRawNames(parser))
    return XML_ERROR_NO_MEMORY;
  return result;
}

static enum XML_Error
doContent(XML_Parser parser,
          int startTagLevel,
          const ENCODING *enc,
          const char *s,
          const char *end,
          const char **nextPtr)
{
  DTD * const dtd = _dtd;  /* save one level of indirection */
  const char **eventPP;
  const char **eventEndPP;
  if (enc == encoding) {
    eventPP = &eventPtr;
    eventEndPP = &eventEndPtr;
  }
  else {
    eventPP = &(openInternalEntities->internalEventPtr);
    eventEndPP = &(openInternalEntities->internalEventEndPtr);
  }
  *eventPP = s;
  for (;;) {
    const char *next = s; /* XmlContentTok doesn't always set the last arg */
    int tok = XmlContentTok(enc, s, end, &next);
    *eventEndPP = next;
    switch (tok) {
    case XML_TOK_TRAILING_CR:
      if (nextPtr) {
        *nextPtr = s;
        return XML_ERROR_NONE;
      }
      *eventEndPP = end;
      if (characterDataHandler) {
        XML_Char c = 0xA;
        characterDataHandler(handlerArg, &c, 1);
      }
      else if (defaultHandler)
        reportDefault(parser, enc, s, end);
      if (startTagLevel == 0)
        return XML_ERROR_NO_ELEMENTS;
      if (tagLevel != startTagLevel)
        return XML_ERROR_ASYNC_ENTITY;
      return XML_ERROR_NONE;
    case XML_TOK_NONE:
      if (nextPtr) {
        *nextPtr = s;
        return XML_ERROR_NONE;
      }
      if (startTagLevel > 0) {
        if (tagLevel != startTagLevel)
          return XML_ERROR_ASYNC_ENTITY;
        return XML_ERROR_NONE;
      }
      return XML_ERROR_NO_ELEMENTS;
    case XML_TOK_INVALID:
      *eventPP = next;
      return XML_ERROR_INVALID_TOKEN;
    case XML_TOK_PARTIAL:
      if (nextPtr) {
        *nextPtr = s;
        return XML_ERROR_NONE;
      }
      return XML_ERROR_UNCLOSED_TOKEN;
    case XML_TOK_PARTIAL_CHAR:
      if (nextPtr) {
        *nextPtr = s;
        return XML_ERROR_NONE;
      }
      return XML_ERROR_PARTIAL_CHAR;
    case XML_TOK_ENTITY_REF:
      {
        const XML_Char *name;
        ENTITY *entity;
        XML_Char ch = (XML_Char) XmlPredefinedEntityName(enc,
                                              s + enc->minBytesPerChar,
                                              next - enc->minBytesPerChar);
        if (ch) {
          if (characterDataHandler)
            characterDataHandler(handlerArg, &ch, 1);
          else if (defaultHandler)
            reportDefault(parser, enc, s, next);
          break;
        }
        name = poolStoreString(&dtd->pool, enc,
                                s + enc->minBytesPerChar,
                                next - enc->minBytesPerChar);
        if (!name)
          return XML_ERROR_NO_MEMORY;
        entity = (ENTITY *)lookup(&dtd->generalEntities, name, 0);
        poolDiscard(&dtd->pool);
        /* First, determine if a check for an existing declaration is needed;
           if yes, check that the entity exists, and that it is internal,
           otherwise call the skipped entity or default handler.
        */
        if (!dtd->hasParamEntityRefs || dtd->standalone) {
          if (!entity)
            return XML_ERROR_UNDEFINED_ENTITY;
          else if (!entity->is_internal)
            return XML_ERROR_ENTITY_DECLARED_IN_PE;
        }
        else if (!entity) {
          if (skippedEntityHandler)
            skippedEntityHandler(handlerArg, name, 0);
          else if (defaultHandler)
            reportDefault(parser, enc, s, next);
          break;
        }
        if (entity->open)
          return XML_ERROR_RECURSIVE_ENTITY_REF;
        if (entity->notation)
          return XML_ERROR_BINARY_ENTITY_REF;
        if (entity->textPtr) {
          enum XML_Error result;
          OPEN_INTERNAL_ENTITY openEntity;
          if (!defaultExpandInternalEntities) {
            if (skippedEntityHandler)
              skippedEntityHandler(handlerArg, entity->name, 0);
            else if (defaultHandler)
              reportDefault(parser, enc, s, next);
            break;
          }
          entity->open = XML_TRUE;
          openEntity.next = openInternalEntities;
          openInternalEntities = &openEntity;
          openEntity.entity = entity;
          openEntity.internalEventPtr = NULL;
          openEntity.internalEventEndPtr = NULL;
          result = doContent(parser,
                             tagLevel,
                             internalEncoding,
                             (char *)entity->textPtr,
                             (char *)(entity->textPtr + entity->textLen),
                             0);
          entity->open = XML_FALSE;
          openInternalEntities = openEntity.next;
          if (result)
            return result;
        }
        else if (externalEntityRefHandler) {
          const XML_Char *context;
          entity->open = XML_TRUE;
          context = getContext(parser);
          entity->open = XML_FALSE;
          if (!context)
            return XML_ERROR_NO_MEMORY;
          if (!externalEntityRefHandler((XML_Parser)externalEntityRefHandlerArg,
                                        context,
                                        entity->base,
                                        entity->systemId,
                                        entity->publicId))
            return XML_ERROR_EXTERNAL_ENTITY_HANDLING;
          poolDiscard(&tempPool);
        }
        else if (defaultHandler)
          reportDefault(parser, enc, s, next);
        break;
      }
    case XML_TOK_START_TAG_NO_ATTS:
      /* fall through */
    case XML_TOK_START_TAG_WITH_ATTS:
      {
        TAG *tag;
        enum XML_Error result;
        XML_Char *toPtr;
        if (freeTagList) {
          tag = freeTagList;
          freeTagList = freeTagList->parent;
        }
        else {
          tag = (TAG *)MALLOC(sizeof(TAG));
          if (!tag)
            return XML_ERROR_NO_MEMORY;
          tag->buf = (char *)MALLOC(INIT_TAG_BUF_SIZE);
          if (!tag->buf) {
            FREE(tag);
            return XML_ERROR_NO_MEMORY;
          }
          tag->bufEnd = tag->buf + INIT_TAG_BUF_SIZE;
        }
        tag->bindings = NULL;
        tag->parent = tagStack;
        tagStack = tag;
        tag->name.localPart = NULL;
        tag->name.prefix = NULL;
        tag->rawName = s + enc->minBytesPerChar;
        tag->rawNameLength = XmlNameLength(enc, tag->rawName);
        ++tagLevel;
        {
          const char *rawNameEnd = tag->rawName + tag->rawNameLength;
          const char *fromPtr = tag->rawName;
          toPtr = (XML_Char *)tag->buf;
          for (;;) {
            int bufSize;
            int convLen;
            XmlConvert(enc,
                       &fromPtr, rawNameEnd,
                       (ICHAR **)&toPtr, (ICHAR *)tag->bufEnd - 1);
            convLen = toPtr - (XML_Char *)tag->buf;
            if (fromPtr == rawNameEnd) {
              tag->name.strLen = convLen;
              break;
            }
            bufSize = (tag->bufEnd - tag->buf) << 1;
            {
              char *temp = (char *)REALLOC(tag->buf, bufSize);
              if (temp == NULL)
                return XML_ERROR_NO_MEMORY;
              tag->buf = temp;
              tag->bufEnd = temp + bufSize;
              toPtr = (XML_Char *)temp + convLen;
            }
          }
        }
        tag->name.str = (XML_Char *)tag->buf;
        *toPtr = XML_T('\0');
        result = storeAtts(parser, enc, s, &(tag->name), &(tag->bindings));
        if (result)
          return result;
        if (startElementHandler)
          startElementHandler(handlerArg, tag->name.str,
                              (const XML_Char **)atts);
        else if (defaultHandler)
          reportDefault(parser, enc, s, next);
        poolClear(&tempPool);
        break;
      }
    case XML_TOK_EMPTY_ELEMENT_NO_ATTS:
      /* fall through */
    case XML_TOK_EMPTY_ELEMENT_WITH_ATTS:
      {
        const char *rawName = s + enc->minBytesPerChar;
        enum XML_Error result;
        BINDING *bindings = NULL;
        XML_Bool noElmHandlers = XML_TRUE;
        TAG_NAME name;
        name.str = poolStoreString(&tempPool, enc, rawName,
                                   rawName + XmlNameLength(enc, rawName));
        if (!name.str)
          return XML_ERROR_NO_MEMORY;
        poolFinish(&tempPool);
        result = storeAtts(parser, enc, s, &name, &bindings);
        if (result)
          return result;
        poolFinish(&tempPool);
        if (startElementHandler) {
          startElementHandler(handlerArg, name.str, (const XML_Char **)atts);
          noElmHandlers = XML_FALSE;
        }
        if (endElementHandler) {
          if (startElementHandler)
            *eventPP = *eventEndPP;
          endElementHandler(handlerArg, name.str);
          noElmHandlers = XML_FALSE;
        }
        if (noElmHandlers && defaultHandler)
          reportDefault(parser, enc, s, next);
        poolClear(&tempPool);
        while (bindings) {
          BINDING *b = bindings;
          if (endNamespaceDeclHandler)
            endNamespaceDeclHandler(handlerArg, b->prefix->name);
          bindings = bindings->nextTagBinding;
          b->nextTagBinding = freeBindingList;
          freeBindingList = b;
          b->prefix->binding = b->prevPrefixBinding;
        }
      }
      if (tagLevel == 0)
        return epilogProcessor(parser, next, end, nextPtr);
      break;
    case XML_TOK_END_TAG:
      if (tagLevel == startTagLevel)
        return XML_ERROR_ASYNC_ENTITY;
      else {
        int len;
        const char *rawName;
        TAG *tag = tagStack;
        tagStack = tag->parent;
        tag->parent = freeTagList;
        freeTagList = tag;
        rawName = s + enc->minBytesPerChar*2;
        len = XmlNameLength(enc, rawName);
        if (len != tag->rawNameLength
            || memcmp(tag->rawName, rawName, len) != 0) {
          *eventPP = rawName;
          return XML_ERROR_TAG_MISMATCH;
        }
        --tagLevel;
        if (endElementHandler) {
          const XML_Char *localPart;
          const XML_Char *prefix;
          XML_Char *uri;
          localPart = tag->name.localPart;
          if (ns && localPart) {
            /* localPart and prefix may have been overwritten in
               tag->name.str, since this points to the binding->uri
               buffer which gets re-used; so we have to add them again
            */
            uri = (XML_Char *)tag->name.str + tag->name.uriLen;
            /* don't need to check for space - already done in storeAtts() */
            while (*localPart) *uri++ = *localPart++;
            prefix = (XML_Char *)tag->name.prefix;
            if (ns_triplets && prefix) {
              *uri++ = namespaceSeparator;
              while (*prefix) *uri++ = *prefix++;
             }
            *uri = XML_T('\0');
          }
          endElementHandler(handlerArg, tag->name.str);
        }
        else if (defaultHandler)
          reportDefault(parser, enc, s, next);
        while (tag->bindings) {
          BINDING *b = tag->bindings;
          if (endNamespaceDeclHandler)
            endNamespaceDeclHandler(handlerArg, b->prefix->name);
          tag->bindings = tag->bindings->nextTagBinding;
          b->nextTagBinding = freeBindingList;
          freeBindingList = b;
          b->prefix->binding = b->prevPrefixBinding;
        }
        if (tagLevel == 0)
          return epilogProcessor(parser, next, end, nextPtr);
      }
      break;
    case XML_TOK_CHAR_REF:
      {
        int n = XmlCharRefNumber(enc, s);
        if (n < 0)
          return XML_ERROR_BAD_CHAR_REF;
        if (characterDataHandler) {
          XML_Char buf[XML_ENCODE_MAX];
          characterDataHandler(handlerArg, buf, XmlEncode(n, (ICHAR *)buf));
        }
        else if (defaultHandler)
          reportDefault(parser, enc, s, next);
      }
      break;
    case XML_TOK_XML_DECL:
      return XML_ERROR_MISPLACED_XML_PI;
    case XML_TOK_DATA_NEWLINE:
      if (characterDataHandler) {
        XML_Char c = 0xA;
        characterDataHandler(handlerArg, &c, 1);
      }
      else if (defaultHandler)
        reportDefault(parser, enc, s, next);
      break;
    case XML_TOK_CDATA_SECT_OPEN:
      {
        enum XML_Error result;
        if (startCdataSectionHandler)
          startCdataSectionHandler(handlerArg);
#if 0
        /* Suppose you doing a transformation on a document that involves
           changing only the character data.  You set up a defaultHandler
           and a characterDataHandler.  The defaultHandler simply copies
           characters through.  The characterDataHandler does the
           transformation and writes the characters out escaping them as
           necessary.  This case will fail to work if we leave out the
           following two lines (because & and < inside CDATA sections will
           be incorrectly escaped).

           However, now we have a start/endCdataSectionHandler, so it seems
           easier to let the user deal with this.
        */
        else if (characterDataHandler)
          characterDataHandler(handlerArg, dataBuf, 0);
#endif
        else if (defaultHandler)
          reportDefault(parser, enc, s, next);
        result = doCdataSection(parser, enc, &next, end, nextPtr);
        if (!next) {
          processor = cdataSectionProcessor;
          return result;
        }
      }
      break;
    case XML_TOK_TRAILING_RSQB:
      if (nextPtr) {
        *nextPtr = s;
        return XML_ERROR_NONE;
      }
      if (characterDataHandler) {
        if (MUST_CONVERT(enc, s)) {
          ICHAR *dataPtr = (ICHAR *)dataBuf;
          XmlConvert(enc, &s, end, &dataPtr, (ICHAR *)dataBufEnd);
          characterDataHandler(handlerArg, dataBuf,
                               dataPtr - (ICHAR *)dataBuf);
        }
        else
          characterDataHandler(handlerArg,
                               (XML_Char *)s,
                               (XML_Char *)end - (XML_Char *)s);
      }
      else if (defaultHandler)
        reportDefault(parser, enc, s, end);
      if (startTagLevel == 0) {
        *eventPP = end;
        return XML_ERROR_NO_ELEMENTS;
      }
      if (tagLevel != startTagLevel) {
        *eventPP = end;
        return XML_ERROR_ASYNC_ENTITY;
      }
      return XML_ERROR_NONE;
    case XML_TOK_DATA_CHARS:
      if (characterDataHandler) {
        if (MUST_CONVERT(enc, s)) {
          for (;;) {
            ICHAR *dataPtr = (ICHAR *)dataBuf;
            XmlConvert(enc, &s, next, &dataPtr, (ICHAR *)dataBufEnd);
            *eventEndPP = s;
            characterDataHandler(handlerArg, dataBuf,
                                 dataPtr - (ICHAR *)dataBuf);
            if (s == next)
              break;
            *eventPP = s;
          }
        }
        else
          characterDataHandler(handlerArg,
                               (XML_Char *)s,
                               (XML_Char *)next - (XML_Char *)s);
      }
      else if (defaultHandler)
        reportDefault(parser, enc, s, next);
      break;
    case XML_TOK_PI:
      if (!reportProcessingInstruction(parser, enc, s, next))
        return XML_ERROR_NO_MEMORY;
      break;
    case XML_TOK_COMMENT:
      if (!reportComment(parser, enc, s, next))
        return XML_ERROR_NO_MEMORY;
      break;
    default:
      if (defaultHandler)
        reportDefault(parser, enc, s, next);
      break;
    }
    *eventPP = s = next;
  }
  /* not reached */
}

/* Precondition: all arguments must be non-NULL;
   Purpose:
   - normalize attributes
   - check attributes for well-formedness
   - generate namespace aware attribute names (URI, prefix)
   - build list of attributes for startElementHandler
   - default attributes
   - process namespace declarations (check and report them)
   - generate namespace aware element name (URI, prefix)
*/
static enum XML_Error
storeAtts(XML_Parser parser, const ENCODING *enc,
          const char *attStr, TAG_NAME *tagNamePtr,
          BINDING **bindingsPtr)
{
  DTD * const dtd = _dtd;  /* save one level of indirection */
  ELEMENT_TYPE *elementType = NULL;
  int nDefaultAtts = 0;
  const XML_Char **appAtts;   /* the attribute list for the application */
  int attIndex = 0;
  int prefixLen;
  int i;
  int n;
  XML_Char *uri;
  int nPrefixes = 0;
  BINDING *binding;
  const XML_Char *localPart;

  /* lookup the element type name */
  elementType = (ELEMENT_TYPE *)lookup(&dtd->elementTypes, tagNamePtr->str,0);
  if (!elementType) {
    const XML_Char *name = poolCopyString(&dtd->pool, tagNamePtr->str);
    if (!name)
      return XML_ERROR_NO_MEMORY;
    elementType = (ELEMENT_TYPE *)lookup(&dtd->elementTypes, name,
                                         sizeof(ELEMENT_TYPE));
    if (!elementType)
      return XML_ERROR_NO_MEMORY;
    if (ns && !setElementTypePrefix(parser, elementType))
      return XML_ERROR_NO_MEMORY;
  }
  nDefaultAtts = elementType->nDefaultAtts;

  /* get the attributes from the tokenizer */
  n = XmlGetAttributes(enc, attStr, attsSize, atts);
  if (n + nDefaultAtts > attsSize) {
    int oldAttsSize = attsSize;
    ATTRIBUTE *temp;
    attsSize = n + nDefaultAtts + INIT_ATTS_SIZE;
    temp = (ATTRIBUTE *)REALLOC((void *)atts, attsSize * sizeof(ATTRIBUTE));
    if (temp == NULL)
      return XML_ERROR_NO_MEMORY;
    atts = temp;
    if (n > oldAttsSize)
      XmlGetAttributes(enc, attStr, n, atts);
  }

  appAtts = (const XML_Char **)atts;
  for (i = 0; i < n; i++) {
    /* add the name and value to the attribute list */
    ATTRIBUTE_ID *attId = getAttributeId(parser, enc, atts[i].name,
                                         atts[i].name
                                         + XmlNameLength(enc, atts[i].name));
    if (!attId)
      return XML_ERROR_NO_MEMORY;
    /* detect duplicate attributes */
    if ((attId->name)[-1]) {
      if (enc == encoding)
        eventPtr = atts[i].name;
      return XML_ERROR_DUPLICATE_ATTRIBUTE;
    }
    (attId->name)[-1] = 1;
    appAtts[attIndex++] = attId->name;
    if (!atts[i].normalized) {
      enum XML_Error result;
      XML_Bool isCdata = XML_TRUE;

      /* figure out whether declared as other than CDATA */
      if (attId->maybeTokenized) {
        int j;
        for (j = 0; j < nDefaultAtts; j++) {
          if (attId == elementType->defaultAtts[j].id) {
            isCdata = elementType->defaultAtts[j].isCdata;
            break;
          }
        }
      }

      /* normalize the attribute value */
      result = storeAttributeValue(parser, enc, isCdata,
                                   atts[i].valuePtr, atts[i].valueEnd,
                                   &tempPool);
      if (result)
        return result;
      appAtts[attIndex] = poolStart(&tempPool);
      poolFinish(&tempPool);
    }
    else {
      /* the value did not need normalizing */
      appAtts[attIndex] = poolStoreString(&tempPool, enc, atts[i].valuePtr,
                                          atts[i].valueEnd);
      if (appAtts[attIndex] == 0)
        return XML_ERROR_NO_MEMORY;
      poolFinish(&tempPool);
    }
    /* handle prefixed attribute names */
    if (attId->prefix) {
      if (attId->xmlns) {
        /* deal with namespace declarations here */
        enum XML_Error result = addBinding(parser, attId->prefix, attId,
                                           appAtts[attIndex], bindingsPtr);
        if (result)
          return result;
        --attIndex;
      }
      else {
        /* deal with other prefixed names later */
        attIndex++;
        nPrefixes++;
        (attId->name)[-1] = 2;
      }
    }
    else
      attIndex++;
  }

  /* set-up for XML_GetSpecifiedAttributeCount and XML_GetIdAttributeIndex */
  nSpecifiedAtts = attIndex;
  if (elementType->idAtt && (elementType->idAtt->name)[-1]) {
    for (i = 0; i < attIndex; i += 2)
      if (appAtts[i] == elementType->idAtt->name) {
        idAttIndex = i;
        break;
      }
  }
  else
    idAttIndex = -1;

  /* do attribute defaulting */
  for (i = 0; i < nDefaultAtts; i++) {
    const DEFAULT_ATTRIBUTE *da = elementType->defaultAtts + i;
    if (!(da->id->name)[-1] && da->value) {
      if (da->id->prefix) {
        if (da->id->xmlns) {
          enum XML_Error result = addBinding(parser, da->id->prefix, da->id,
                                             da->value, bindingsPtr);
          if (result)
            return result;
        }
        else {
          (da->id->name)[-1] = 2;
          nPrefixes++;
          appAtts[attIndex++] = da->id->name;
          appAtts[attIndex++] = da->value;
        }
      }
      else {
        (da->id->name)[-1] = 1;
        appAtts[attIndex++] = da->id->name;
        appAtts[attIndex++] = da->value;
      }
    }
  }
  appAtts[attIndex] = 0;

  i = 0;
  if (nPrefixes) {
    /* expand prefixed attribute names */
    for (; i < attIndex; i += 2) {
      if (appAtts[i][-1] == 2) {
        ATTRIBUTE_ID *id;
        ((XML_Char *)(appAtts[i]))[-1] = 0;
        id = (ATTRIBUTE_ID *)lookup(&dtd->attributeIds, appAtts[i], 0);
        if (id->prefix->binding) {
          int j;
          const BINDING *b = id->prefix->binding;
          const XML_Char *s = appAtts[i];
          for (j = 0; j < b->uriLen; j++) {
            if (!poolAppendChar(&tempPool, b->uri[j]))
              return XML_ERROR_NO_MEMORY;
          }
          while (*s++ != XML_T(':'))
            ;
          do {
            if (!poolAppendChar(&tempPool, *s))
              return XML_ERROR_NO_MEMORY;
          } while (*s++);
          if (ns_triplets) {
            tempPool.ptr[-1] = namespaceSeparator;
            s = b->prefix->name;
            do {
              if (!poolAppendChar(&tempPool, *s))
                return XML_ERROR_NO_MEMORY;
            } while (*s++);
          }

          appAtts[i] = poolStart(&tempPool);
          poolFinish(&tempPool);
        }
        if (!--nPrefixes)
          break;
      }
      else
        ((XML_Char *)(appAtts[i]))[-1] = 0;
    }
  }
  /* clear the flags that say whether attributes were specified */
  for (; i < attIndex; i += 2)
    ((XML_Char *)(appAtts[i]))[-1] = 0;
  for (binding = *bindingsPtr; binding; binding = binding->nextTagBinding)
    binding->attId->name[-1] = 0;

  /* expand the element type name */
  if (elementType->prefix) {
    binding = elementType->prefix->binding;
    if (!binding)
      return XML_ERROR_NONE;
    localPart = tagNamePtr->str;
    while (*localPart++ != XML_T(':'))
      ;
  }
  else if (dtd->defaultPrefix.binding) {
    binding = dtd->defaultPrefix.binding;
    localPart = tagNamePtr->str;
  }
  else
    return XML_ERROR_NONE;
  prefixLen = 0;
  if (ns && ns_triplets && binding->prefix->name) {
    for (; binding->prefix->name[prefixLen++];)
      ;
  }
  tagNamePtr->localPart = localPart;
  tagNamePtr->uriLen = binding->uriLen;
  tagNamePtr->prefix = binding->prefix->name;
  tagNamePtr->prefixLen = prefixLen;
  for (i = 0; localPart[i++];)
    ;
  n = i + binding->uriLen + prefixLen;
  if (n > binding->uriAlloc) {
    TAG *p;
    uri = (XML_Char *)MALLOC((n + EXPAND_SPARE) * sizeof(XML_Char));
    if (!uri)
      return XML_ERROR_NO_MEMORY;
    binding->uriAlloc = n + EXPAND_SPARE;
    memcpy(uri, binding->uri, binding->uriLen * sizeof(XML_Char));
    for (p = tagStack; p; p = p->parent)
      if (p->name.str == binding->uri)
        p->name.str = uri;
    FREE(binding->uri);
    binding->uri = uri;
  }
  uri = binding->uri + binding->uriLen;
  memcpy(uri, localPart, i * sizeof(XML_Char));
  if (prefixLen) {
        uri = uri + (i - 1);
    if (namespaceSeparator) { *(uri) = namespaceSeparator; }
    memcpy(uri + 1, binding->prefix->name, prefixLen * sizeof(XML_Char));
  }
  tagNamePtr->str = binding->uri;
  return XML_ERROR_NONE;
}

/* addBinding() overwrites the value of prefix->binding without checking.
   Therefore one must keep track of the old value outside of addBinding().
*/
static enum XML_Error
addBinding(XML_Parser parser, PREFIX *prefix, const ATTRIBUTE_ID *attId,
           const XML_Char *uri, BINDING **bindingsPtr)
{
  BINDING *b;
  int len;

  /* empty string is only valid when there is no prefix per XML NS 1.0 */
  if (*uri == XML_T('\0') && prefix->name)
    return XML_ERROR_SYNTAX;

  for (len = 0; uri[len]; len++)
    ;
  if (namespaceSeparator)
    len++;
  if (freeBindingList) {
    b = freeBindingList;
    if (len > b->uriAlloc) {
      XML_Char *temp = (XML_Char *)REALLOC(b->uri,
                          sizeof(XML_Char) * (len + EXPAND_SPARE));
      if (temp == NULL)
        return XML_ERROR_NO_MEMORY;
      b->uri = temp;
      b->uriAlloc = len + EXPAND_SPARE;
    }
    freeBindingList = b->nextTagBinding;
  }
  else {
    b = (BINDING *)MALLOC(sizeof(BINDING));
    if (!b)
      return XML_ERROR_NO_MEMORY;
    b->uri = (XML_Char *)MALLOC(sizeof(XML_Char) * (len + EXPAND_SPARE));
    if (!b->uri) {
      FREE(b);
      return XML_ERROR_NO_MEMORY;
    }
    b->uriAlloc = len + EXPAND_SPARE;
  }
  b->uriLen = len;
  memcpy(b->uri, uri, len * sizeof(XML_Char));
  if (namespaceSeparator)
    b->uri[len - 1] = namespaceSeparator;
  b->prefix = prefix;
  b->attId = attId;
  b->prevPrefixBinding = prefix->binding;
  if (*uri == XML_T('\0') && prefix == &_dtd->defaultPrefix)
    prefix->binding = NULL;
  else
    prefix->binding = b;
  b->nextTagBinding = *bindingsPtr;
  *bindingsPtr = b;
  if (startNamespaceDeclHandler)
    startNamespaceDeclHandler(handlerArg, prefix->name,
                              prefix->binding ? uri : 0);
  return XML_ERROR_NONE;
}

/* The idea here is to avoid using stack for each CDATA section when
   the whole file is parsed with one call.
*/
static enum XML_Error PTRCALL
cdataSectionProcessor(XML_Parser parser,
                      const char *start,
                      const char *end,
                      const char **endPtr)
{
  enum XML_Error result = doCdataSection(parser, encoding, &start,
                                         end, endPtr);
  if (start) {
    if (parentParser) {  /* we are parsing an external entity */
      processor = externalEntityContentProcessor;
      return externalEntityContentProcessor(parser, start, end, endPtr);
    }
    else {
      processor = contentProcessor;
      return contentProcessor(parser, start, end, endPtr);
    }
  }
  return result;
}

/* startPtr gets set to non-null is the section is closed, and to null if
   the section is not yet closed.
*/
static enum XML_Error
doCdataSection(XML_Parser parser,
               const ENCODING *enc,
               const char **startPtr,
               const char *end,
               const char **nextPtr)
{
  const char *s = *startPtr;
  const char **eventPP;
  const char **eventEndPP;
  if (enc == encoding) {
    eventPP = &eventPtr;
    *eventPP = s;
    eventEndPP = &eventEndPtr;
  }
  else {
    eventPP = &(openInternalEntities->internalEventPtr);
    eventEndPP = &(openInternalEntities->internalEventEndPtr);
  }
  *eventPP = s;
  *startPtr = NULL;
  for (;;) {
    const char *next;
    int tok = XmlCdataSectionTok(enc, s, end, &next);
    *eventEndPP = next;
    switch (tok) {
    case XML_TOK_CDATA_SECT_CLOSE:
      if (endCdataSectionHandler)
        endCdataSectionHandler(handlerArg);
#if 0
      /* see comment under XML_TOK_CDATA_SECT_OPEN */
      else if (characterDataHandler)
        characterDataHandler(handlerArg, dataBuf, 0);
#endif
      else if (defaultHandler)
        reportDefault(parser, enc, s, next);
      *startPtr = next;
      return XML_ERROR_NONE;
    case XML_TOK_DATA_NEWLINE:
      if (characterDataHandler) {
        XML_Char c = 0xA;
        characterDataHandler(handlerArg, &c, 1);
      }
      else if (defaultHandler)
        reportDefault(parser, enc, s, next);
      break;
    case XML_TOK_DATA_CHARS:
      if (characterDataHandler) {
        if (MUST_CONVERT(enc, s)) {
          for (;;) {
            ICHAR *dataPtr = (ICHAR *)dataBuf;
            XmlConvert(enc, &s, next, &dataPtr, (ICHAR *)dataBufEnd);
            *eventEndPP = next;
            characterDataHandler(handlerArg, dataBuf,
                                 dataPtr - (ICHAR *)dataBuf);
            if (s == next)
              break;
            *eventPP = s;
          }
        }
        else
          characterDataHandler(handlerArg,
                               (XML_Char *)s,
                               (XML_Char *)next - (XML_Char *)s);
      }
      else if (defaultHandler)
        reportDefault(parser, enc, s, next);
      break;
    case XML_TOK_INVALID:
      *eventPP = next;
      return XML_ERROR_INVALID_TOKEN;
    case XML_TOK_PARTIAL_CHAR:
      if (nextPtr) {
        *nextPtr = s;
        return XML_ERROR_NONE;
      }
      return XML_ERROR_PARTIAL_CHAR;
    case XML_TOK_PARTIAL:
    case XML_TOK_NONE:
      if (nextPtr) {
        *nextPtr = s;
        return XML_ERROR_NONE;
      }
      return XML_ERROR_UNCLOSED_CDATA_SECTION;
    default:
      *eventPP = next;
      return XML_ERROR_UNEXPECTED_STATE;
    }
    *eventPP = s = next;
  }
  /* not reached */
}

#ifdef XML_DTD

/* The idea here is to avoid using stack for each IGNORE section when
   the whole file is parsed with one call.
*/
static enum XML_Error PTRCALL
ignoreSectionProcessor(XML_Parser parser,
                       const char *start,
                       const char *end,
                       const char **endPtr)
{
  enum XML_Error result = doIgnoreSection(parser, encoding, &start,
                                          end, endPtr);
  if (start) {
    processor = prologProcessor;
    return prologProcessor(parser, start, end, endPtr);
  }
  return result;
}

/* startPtr gets set to non-null is the section is closed, and to null
   if the section is not yet closed.
*/
static enum XML_Error
doIgnoreSection(XML_Parser parser,
                const ENCODING *enc,
                const char **startPtr,
                const char *end,
                const char **nextPtr)
{
  const char *next;
  int tok;
  const char *s = *startPtr;
  const char **eventPP;
  const char **eventEndPP;
  if (enc == encoding) {
    eventPP = &eventPtr;
    *eventPP = s;
    eventEndPP = &eventEndPtr;
  }
  else {
    eventPP = &(openInternalEntities->internalEventPtr);
    eventEndPP = &(openInternalEntities->internalEventEndPtr);
  }
  *eventPP = s;
  *startPtr = NULL;
  tok = XmlIgnoreSectionTok(enc, s, end, &next);
  *eventEndPP = next;
  switch (tok) {
  case XML_TOK_IGNORE_SECT:
    if (defaultHandler)
      reportDefault(parser, enc, s, next);
    *startPtr = next;
    return XML_ERROR_NONE;
  case XML_TOK_INVALID:
    *eventPP = next;
    return XML_ERROR_INVALID_TOKEN;
  case XML_TOK_PARTIAL_CHAR:
    if (nextPtr) {
      *nextPtr = s;
      return XML_ERROR_NONE;
    }
    return XML_ERROR_PARTIAL_CHAR;
  case XML_TOK_PARTIAL:
  case XML_TOK_NONE:
    if (nextPtr) {
      *nextPtr = s;
      return XML_ERROR_NONE;
    }
    return XML_ERROR_SYNTAX; /* XML_ERROR_UNCLOSED_IGNORE_SECTION */
  default:
    *eventPP = next;
    return XML_ERROR_UNEXPECTED_STATE;
  }
  /* not reached */
}

#endif /* XML_DTD */

static enum XML_Error
initializeEncoding(XML_Parser parser)
{
  const char *s;
#ifdef XML_UNICODE
  char encodingBuf[128];
  if (!protocolEncodingName)
    s = NULL;
  else {
    int i;
    for (i = 0; protocolEncodingName[i]; i++) {
      if (i == sizeof(encodingBuf) - 1
          || (protocolEncodingName[i] & ~0x7f) != 0) {
        encodingBuf[0] = '\0';
        break;
      }
      encodingBuf[i] = (char)protocolEncodingName[i];
    }
    encodingBuf[i] = '\0';
    s = encodingBuf;
  }
#else
  s = protocolEncodingName;
#endif
  if ((ns ? XmlInitEncodingNS : XmlInitEncoding)(&initEncoding, &encoding, s))
    return XML_ERROR_NONE;
  return handleUnknownEncoding(parser, protocolEncodingName);
}

static enum XML_Error
processXmlDecl(XML_Parser parser, int isGeneralTextEntity,
               const char *s, const char *next)
{
  const char *encodingName = NULL;
  const XML_Char *storedEncName = NULL;
  const ENCODING *newEncoding = NULL;
  const char *version = NULL;
  const char *versionend;
  const XML_Char *storedversion = NULL;
  int standalone = -1;
  if (!(ns
        ? XmlParseXmlDeclNS
        : XmlParseXmlDecl)(isGeneralTextEntity,
                           encoding,
                           s,
                           next,
                           &eventPtr,
                           &version,
                           &versionend,
                           &encodingName,
                           &newEncoding,
                           &standalone))
    return XML_ERROR_SYNTAX;
  if (!isGeneralTextEntity && standalone == 1) {
    _dtd->standalone = XML_TRUE;
#ifdef XML_DTD
    if (paramEntityParsing == XML_PARAM_ENTITY_PARSING_UNLESS_STANDALONE)
      paramEntityParsing = XML_PARAM_ENTITY_PARSING_NEVER;
#endif /* XML_DTD */
  }
  if (xmlDeclHandler) {
    if (encodingName != NULL) {
      storedEncName = poolStoreString(&temp2Pool,
                                      encoding,
                                      encodingName,
                                      encodingName
                                      + XmlNameLength(encoding, encodingName));
      if (!storedEncName)
              return XML_ERROR_NO_MEMORY;
      poolFinish(&temp2Pool);
    }
    if (version) {
      storedversion = poolStoreString(&temp2Pool,
                                      encoding,
                                      version,
                                      versionend - encoding->minBytesPerChar);
      if (!storedversion)
        return XML_ERROR_NO_MEMORY;
    }
    xmlDeclHandler(handlerArg, storedversion, storedEncName, standalone);
  }
  else if (defaultHandler)
    reportDefault(parser, encoding, s, next);
  if (protocolEncodingName == NULL) {
    if (newEncoding) {
      if (newEncoding->minBytesPerChar != encoding->minBytesPerChar) {
        eventPtr = encodingName;
        return XML_ERROR_INCORRECT_ENCODING;
      }
      encoding = newEncoding;
    }
    else if (encodingName) {
      enum XML_Error result;
      if (!storedEncName) {
        storedEncName = poolStoreString(
          &temp2Pool, encoding, encodingName,
          encodingName + XmlNameLength(encoding, encodingName));
        if (!storedEncName)
          return XML_ERROR_NO_MEMORY;
      }
      result = handleUnknownEncoding(parser, storedEncName);
      poolClear(&temp2Pool);
      if (result == XML_ERROR_UNKNOWN_ENCODING)
        eventPtr = encodingName;
      return result;
    }
  }

  if (storedEncName || storedversion)
    poolClear(&temp2Pool);

  return XML_ERROR_NONE;
}

static enum XML_Error
handleUnknownEncoding(XML_Parser parser, const XML_Char *encodingName)
{
  if (unknownEncodingHandler) {
    XML_Encoding info;
    int i;
    for (i = 0; i < 256; i++)
      info.map[i] = -1;
    info.convert = NULL;
    info.data = NULL;
    info.release = NULL;
    if (unknownEncodingHandler(unknownEncodingHandlerData, encodingName,
                               &info)) {
      ENCODING *enc;
      unknownEncodingMem = MALLOC(XmlSizeOfUnknownEncoding());
      if (!unknownEncodingMem) {
        if (info.release)
          info.release(info.data);
        return XML_ERROR_NO_MEMORY;
      }
      enc = (ns
             ? XmlInitUnknownEncodingNS
             : XmlInitUnknownEncoding)(unknownEncodingMem,
                                       info.map,
                                       info.convert,
                                       info.data);
      if (enc) {
        unknownEncodingData = info.data;
        unknownEncodingRelease = info.release;
        encoding = enc;
        return XML_ERROR_NONE;
      }
    }
    if (info.release != NULL)
      info.release(info.data);
  }
  return XML_ERROR_UNKNOWN_ENCODING;
}

static enum XML_Error PTRCALL
prologInitProcessor(XML_Parser parser,
                    const char *s,
                    const char *end,
                    const char **nextPtr)
{
  enum XML_Error result = initializeEncoding(parser);
  if (result != XML_ERROR_NONE)
    return result;
  processor = prologProcessor;
  return prologProcessor(parser, s, end, nextPtr);
}

#ifdef XML_DTD

static enum XML_Error PTRCALL
externalParEntInitProcessor(XML_Parser parser,
                            const char *s,
                            const char *end,
                            const char **nextPtr)
{
  enum XML_Error result = initializeEncoding(parser);
  if (result != XML_ERROR_NONE)
    return result;

  /* we know now that XML_Parse(Buffer) has been called,
     so we consider the external parameter entity read */
  _dtd->paramEntityRead = XML_TRUE;

  if (prologState.inEntityValue) {
    processor = entityValueInitProcessor;
    return entityValueInitProcessor(parser, s, end, nextPtr);
  }
  else {
    processor = externalParEntProcessor;
    return externalParEntProcessor(parser, s, end, nextPtr);
  }
}

static enum XML_Error PTRCALL
entityValueInitProcessor(XML_Parser parser,
                         const char *s,
                         const char *end,
                         const char **nextPtr)
{
  const char *start = s;
  const char *next = s;
  int tok;

  for (;;) {
    tok = XmlPrologTok(encoding, start, end, &next);
    if (tok <= 0) {
      if (nextPtr != 0 && tok != XML_TOK_INVALID) {
              *nextPtr = s;
              return XML_ERROR_NONE;
      }
      switch (tok) {
      case XML_TOK_INVALID:
              return XML_ERROR_INVALID_TOKEN;
      case XML_TOK_PARTIAL:
              return XML_ERROR_UNCLOSED_TOKEN;
      case XML_TOK_PARTIAL_CHAR:
              return XML_ERROR_PARTIAL_CHAR;
      case XML_TOK_NONE:   /* start == end */
      default:
        break;
      }
      return storeEntityValue(parser, encoding, s, end);
    }
    else if (tok == XML_TOK_XML_DECL) {
      enum XML_Error result = processXmlDecl(parser, 0, start, next);
            if (result != XML_ERROR_NONE)
              return result;
      if (nextPtr) *nextPtr = next;
      /* stop scanning for text declaration - we found one */
      processor = entityValueProcessor;
      return entityValueProcessor(parser, next, end, nextPtr);
    }
    /* If we are at the end of the buffer, this would cause XmlPrologTok to
       return XML_TOK_NONE on the next call, which would then cause the
       function to exit with *nextPtr set to s - that is what we want for other
       tokens, but not for the BOM - we would rather like to skip it;
       then, when this routine is entered the next time, XmlPrologTok will
       return XML_TOK_INVALID, since the BOM is still in the buffer
    */
    else if (tok == XML_TOK_BOM && next == end && nextPtr) {
      *nextPtr = next;
      return XML_ERROR_NONE;
    }
    start = next;
  }
}

static enum XML_Error PTRCALL
externalParEntProcessor(XML_Parser parser,
                        const char *s,
                        const char *end,
                        const char **nextPtr)
{
  const char *start = s;
  const char *next = s;
  int tok;

  tok = XmlPrologTok(encoding, start, end, &next);
  if (tok <= 0) {
    if (nextPtr != 0 && tok != XML_TOK_INVALID) {
      *nextPtr = s;
      return XML_ERROR_NONE;
    }
    switch (tok) {
    case XML_TOK_INVALID:
      return XML_ERROR_INVALID_TOKEN;
    case XML_TOK_PARTIAL:
      return XML_ERROR_UNCLOSED_TOKEN;
    case XML_TOK_PARTIAL_CHAR:
      return XML_ERROR_PARTIAL_CHAR;
    case XML_TOK_NONE:   /* start == end */
    default:
      break;
    }
  }
  /* This would cause the next stage, i.e. doProlog to be passed XML_TOK_BOM.
     However, when parsing an external subset, doProlog will not accept a BOM
     as valid, and report a syntax error, so we have to skip the BOM
  */
  else if (tok == XML_TOK_BOM) {
    s = next;
    tok = XmlPrologTok(encoding, s, end, &next);
  }

  processor = prologProcessor;
  return doProlog(parser, encoding, s, end, tok, next, nextPtr);
}

static enum XML_Error PTRCALL
entityValueProcessor(XML_Parser parser,
                     const char *s,
                     const char *end,
                     const char **nextPtr)
{
  const char *start = s;
  const char *next = s;
  const ENCODING *enc = encoding;
  int tok;

  for (;;) {
    tok = XmlPrologTok(enc, start, end, &next);
    if (tok <= 0) {
      if (nextPtr != 0 && tok != XML_TOK_INVALID) {
        *nextPtr = s;
        return XML_ERROR_NONE;
      }
      switch (tok) {
      case XML_TOK_INVALID:
              return XML_ERROR_INVALID_TOKEN;
      case XML_TOK_PARTIAL:
              return XML_ERROR_UNCLOSED_TOKEN;
      case XML_TOK_PARTIAL_CHAR:
              return XML_ERROR_PARTIAL_CHAR;
      case XML_TOK_NONE:   /* start == end */
      default:
        break;
      }
      return storeEntityValue(parser, enc, s, end);
    }
    start = next;
  }
}

#endif /* XML_DTD */

static enum XML_Error PTRCALL
prologProcessor(XML_Parser parser,
                const char *s,
                const char *end,
                const char **nextPtr)
{
  const char *next = s;
  int tok = XmlPrologTok(encoding, s, end, &next);
  return doProlog(parser, encoding, s, end, tok, next, nextPtr);
}

static enum XML_Error
doProlog(XML_Parser parser,
         const ENCODING *enc,
         const char *s,
         const char *end,
         int tok,
         const char *next,
         const char **nextPtr)
{
#ifdef XML_DTD
  static const XML_Char externalSubsetName[] = { '#' , '\0' };
#endif /* XML_DTD */
  static const XML_Char atypeCDATA[] = { 'C', 'D', 'A', 'T', 'A', '\0' };
  static const XML_Char atypeID[] = { 'I', 'D', '\0' };
  static const XML_Char atypeIDREF[] = { 'I', 'D', 'R', 'E', 'F', '\0' };
  static const XML_Char atypeIDREFS[] = { 'I', 'D', 'R', 'E', 'F', 'S', '\0' };
  static const XML_Char atypeENTITY[] = { 'E', 'N', 'T', 'I', 'T', 'Y', '\0' };
  static const XML_Char atypeENTITIES[] =
      { 'E', 'N', 'T', 'I', 'T', 'I', 'E', 'S', '\0' };
  static const XML_Char atypeNMTOKEN[] = {
      'N', 'M', 'T', 'O', 'K', 'E', 'N', '\0' };
  static const XML_Char atypeNMTOKENS[] = {
      'N', 'M', 'T', 'O', 'K', 'E', 'N', 'S', '\0' };
  static const XML_Char notationPrefix[] = {
      'N', 'O', 'T', 'A', 'T', 'I', 'O', 'N', '(', '\0' };
  static const XML_Char enumValueSep[] = { '|', '\0' };
  static const XML_Char enumValueStart[] = { '(', '\0' };

  DTD * const dtd = _dtd;  /* save one level of indirection */

  const char **eventPP;
  const char **eventEndPP;
  enum XML_Content_Quant quant;

  if (enc == encoding) {
    eventPP = &eventPtr;
    eventEndPP = &eventEndPtr;
  }
  else {
    eventPP = &(openInternalEntities->internalEventPtr);
    eventEndPP = &(openInternalEntities->internalEventEndPtr);
  }
  for (;;) {
    int role;
    XML_Bool handleDefault = XML_TRUE;
    *eventPP = s;
    *eventEndPP = next;
    if (tok <= 0) {
      if (nextPtr != 0 && tok != XML_TOK_INVALID) {
        *nextPtr = s;
        return XML_ERROR_NONE;
      }
      switch (tok) {
      case XML_TOK_INVALID:
        *eventPP = next;
        return XML_ERROR_INVALID_TOKEN;
      case XML_TOK_PARTIAL:
        return XML_ERROR_UNCLOSED_TOKEN;
      case XML_TOK_PARTIAL_CHAR:
        return XML_ERROR_PARTIAL_CHAR;
      case XML_TOK_NONE:
#ifdef XML_DTD
        if (enc != encoding)
          return XML_ERROR_NONE;
        if (isParamEntity) {
          if (XmlTokenRole(&prologState, XML_TOK_NONE, end, end, enc)
              == XML_ROLE_ERROR)
            return XML_ERROR_SYNTAX;
          return XML_ERROR_NONE;
        }
#endif /* XML_DTD */
        return XML_ERROR_NO_ELEMENTS;
      default:
        tok = -tok;
        next = end;
        break;
      }
    }
    role = XmlTokenRole(&prologState, tok, s, next, enc);
    switch (role) {
    case XML_ROLE_XML_DECL:
      {
        enum XML_Error result = processXmlDecl(parser, 0, s, next);
        if (result != XML_ERROR_NONE)
          return result;
        enc = encoding;
        handleDefault = XML_FALSE;
      }
      break;
    case XML_ROLE_DOCTYPE_NAME:
      if (startDoctypeDeclHandler) {
        doctypeName = poolStoreString(&tempPool, enc, s, next);
        if (!doctypeName)
          return XML_ERROR_NO_MEMORY;
        poolFinish(&tempPool);
        doctypePubid = NULL;
        handleDefault = XML_FALSE;
      }
      doctypeSysid = NULL; /* always initialize to NULL */
      break;
    case XML_ROLE_DOCTYPE_INTERNAL_SUBSET:
      if (startDoctypeDeclHandler) {
        startDoctypeDeclHandler(handlerArg, doctypeName, doctypeSysid,
                                doctypePubid, 1);
        doctypeName = NULL;
        poolClear(&tempPool);
        handleDefault = XML_FALSE;
      }
      break;
#ifdef XML_DTD
    case XML_ROLE_TEXT_DECL:
      {
        enum XML_Error result = processXmlDecl(parser, 1, s, next);
        if (result != XML_ERROR_NONE)
          return result;
        enc = encoding;
        handleDefault = XML_FALSE;
      }
      break;
#endif /* XML_DTD */
    case XML_ROLE_DOCTYPE_PUBLIC_ID:
#ifdef XML_DTD
      useForeignDTD = XML_FALSE;
#endif /* XML_DTD */
      dtd->hasParamEntityRefs = XML_TRUE;
      if (startDoctypeDeclHandler) {
        doctypePubid = poolStoreString(&tempPool, enc,
                                       s + enc->minBytesPerChar,
                                       next - enc->minBytesPerChar);
        if (!doctypePubid)
          return XML_ERROR_NO_MEMORY;
        poolFinish(&tempPool);
        handleDefault = XML_FALSE;
      }
#ifdef XML_DTD
      declEntity = (ENTITY *)lookup(&dtd->paramEntities,
                                    externalSubsetName,
                                    sizeof(ENTITY));
      if (!declEntity)
        return XML_ERROR_NO_MEMORY;
#endif /* XML_DTD */
      /* fall through */
    case XML_ROLE_ENTITY_PUBLIC_ID:
      if (!XmlIsPublicId(enc, s, next, eventPP))
        return XML_ERROR_SYNTAX;
      if (dtd->keepProcessing && declEntity) {
        XML_Char *tem = poolStoreString(&dtd->pool,
                                        enc,
                                        s + enc->minBytesPerChar,
                                        next - enc->minBytesPerChar);
        if (!tem)
          return XML_ERROR_NO_MEMORY;
        normalizePublicId(tem);
        declEntity->publicId = tem;
        poolFinish(&dtd->pool);
        if (entityDeclHandler)
          handleDefault = XML_FALSE;
      }
      break;
    case XML_ROLE_DOCTYPE_CLOSE:
      if (doctypeName) {
        startDoctypeDeclHandler(handlerArg, doctypeName,
                                doctypeSysid, doctypePubid, 0);
        poolClear(&tempPool);
        handleDefault = XML_FALSE;
      }
      /* doctypeSysid will be non-NULL in the case of a previous
         XML_ROLE_DOCTYPE_SYSTEM_ID, even if startDoctypeDeclHandler
         was not set, indicating an external subset
      */
#ifdef XML_DTD
      if (doctypeSysid || useForeignDTD) {
        dtd->hasParamEntityRefs = XML_TRUE; /* when docTypeSysid == NULL */
        if (paramEntityParsing && externalEntityRefHandler) {
          ENTITY *entity = (ENTITY *)lookup(&dtd->paramEntities,
                                            externalSubsetName,
                                            sizeof(ENTITY));
          if (!entity)
            return XML_ERROR_NO_MEMORY;
          if (useForeignDTD)
            entity->base = curBase;
          dtd->paramEntityRead = XML_FALSE;
          if (!externalEntityRefHandler(externalEntityRefHandlerArg,
                                        0,
                                        entity->base,
                                        entity->systemId,
                                        entity->publicId))
            return XML_ERROR_EXTERNAL_ENTITY_HANDLING;
          if (dtd->paramEntityRead &&
              !dtd->standalone &&
              notStandaloneHandler &&
              !notStandaloneHandler(handlerArg))
            return XML_ERROR_NOT_STANDALONE;
          /* end of DTD - no need to update dtd->keepProcessing */
        }
        useForeignDTD = XML_FALSE;
      }
#endif /* XML_DTD */
      if (endDoctypeDeclHandler) {
        endDoctypeDeclHandler(handlerArg);
        handleDefault = XML_FALSE;
      }
      break;
    case XML_ROLE_INSTANCE_START:
#ifdef XML_DTD
      /* if there is no DOCTYPE declaration then now is the
         last chance to read the foreign DTD
      */
      if (useForeignDTD) {
        dtd->hasParamEntityRefs = XML_TRUE;
        if (paramEntityParsing && externalEntityRefHandler) {
          ENTITY *entity = (ENTITY *)lookup(&dtd->paramEntities,
                                            externalSubsetName,
                                            sizeof(ENTITY));
          if (!entity)
            return XML_ERROR_NO_MEMORY;
          entity->base = curBase;
          dtd->paramEntityRead = XML_FALSE;
          if (!externalEntityRefHandler(externalEntityRefHandlerArg,
                                        0,
                                        entity->base,
                                        entity->systemId,
                                        entity->publicId))
            return XML_ERROR_EXTERNAL_ENTITY_HANDLING;
          if (dtd->paramEntityRead &&
              !dtd->standalone &&
              notStandaloneHandler &&
              !notStandaloneHandler(handlerArg))
            return XML_ERROR_NOT_STANDALONE;
          /* end of DTD - no need to update dtd->keepProcessing */
        }
      }
#endif /* XML_DTD */
      processor = contentProcessor;
      return contentProcessor(parser, s, end, nextPtr);
    case XML_ROLE_ATTLIST_ELEMENT_NAME:
      declElementType = getElementType(parser, enc, s, next);
      if (!declElementType)
        return XML_ERROR_NO_MEMORY;
      goto checkAttListDeclHandler;
    case XML_ROLE_ATTRIBUTE_NAME:
      declAttributeId = getAttributeId(parser, enc, s, next);
      if (!declAttributeId)
        return XML_ERROR_NO_MEMORY;
      declAttributeIsCdata = XML_FALSE;
      declAttributeType = NULL;
      declAttributeIsId = XML_FALSE;
      goto checkAttListDeclHandler;
    case XML_ROLE_ATTRIBUTE_TYPE_CDATA:
      declAttributeIsCdata = XML_TRUE;
      declAttributeType = atypeCDATA;
      goto checkAttListDeclHandler;
    case XML_ROLE_ATTRIBUTE_TYPE_ID:
      declAttributeIsId = XML_TRUE;
      declAttributeType = atypeID;
      goto checkAttListDeclHandler;
    case XML_ROLE_ATTRIBUTE_TYPE_IDREF:
      declAttributeType = atypeIDREF;
      goto checkAttListDeclHandler;
    case XML_ROLE_ATTRIBUTE_TYPE_IDREFS:
      declAttributeType = atypeIDREFS;
      goto checkAttListDeclHandler;
    case XML_ROLE_ATTRIBUTE_TYPE_ENTITY:
      declAttributeType = atypeENTITY;
      goto checkAttListDeclHandler;
    case XML_ROLE_ATTRIBUTE_TYPE_ENTITIES:
      declAttributeType = atypeENTITIES;
      goto checkAttListDeclHandler;
    case XML_ROLE_ATTRIBUTE_TYPE_NMTOKEN:
      declAttributeType = atypeNMTOKEN;
      goto checkAttListDeclHandler;
    case XML_ROLE_ATTRIBUTE_TYPE_NMTOKENS:
      declAttributeType = atypeNMTOKENS;
    checkAttListDeclHandler:
      if (dtd->keepProcessing && attlistDeclHandler)
        handleDefault = XML_FALSE;
      break;
    case XML_ROLE_ATTRIBUTE_ENUM_VALUE:
    case XML_ROLE_ATTRIBUTE_NOTATION_VALUE:
      if (dtd->keepProcessing && attlistDeclHandler) {
        const XML_Char *prefix;
        if (declAttributeType) {
          prefix = enumValueSep;
        }
        else {
          prefix = (role == XML_ROLE_ATTRIBUTE_NOTATION_VALUE
                    ? notationPrefix
                    : enumValueStart);
        }
        if (!poolAppendString(&tempPool, prefix))
          return XML_ERROR_NO_MEMORY;
        if (!poolAppend(&tempPool, enc, s, next))
          return XML_ERROR_NO_MEMORY;
        declAttributeType = tempPool.start;
        handleDefault = XML_FALSE;
      }
      break;
    case XML_ROLE_IMPLIED_ATTRIBUTE_VALUE:
    case XML_ROLE_REQUIRED_ATTRIBUTE_VALUE:
      if (dtd->keepProcessing) {
        if (!defineAttribute(declElementType, declAttributeId,
                              declAttributeIsCdata, declAttributeIsId, 0,
                              parser))
          return XML_ERROR_NO_MEMORY;
        if (attlistDeclHandler && declAttributeType) {
          if (*declAttributeType == XML_T('(')
              || (*declAttributeType == XML_T('N')
                  && declAttributeType[1] == XML_T('O'))) {
            /* Enumerated or Notation type */
            if (!poolAppendChar(&tempPool, XML_T(')'))
                || !poolAppendChar(&tempPool, XML_T('\0')))
              return XML_ERROR_NO_MEMORY;
            declAttributeType = tempPool.start;
            poolFinish(&tempPool);
          }
          *eventEndPP = s;
          attlistDeclHandler(handlerArg, declElementType->name,
                             declAttributeId->name, declAttributeType,
                             0, role == XML_ROLE_REQUIRED_ATTRIBUTE_VALUE);
          poolClear(&tempPool);
          handleDefault = XML_FALSE;
        }
      }
      break;
    case XML_ROLE_DEFAULT_ATTRIBUTE_VALUE:
    case XML_ROLE_FIXED_ATTRIBUTE_VALUE:
      if (dtd->keepProcessing) {
        const XML_Char *attVal;
        enum XML_Error result
          = storeAttributeValue(parser, enc, declAttributeIsCdata,
                                s + enc->minBytesPerChar,
                                next - enc->minBytesPerChar,
                                &dtd->pool);
        if (result)
          return result;
        attVal = poolStart(&dtd->pool);
        poolFinish(&dtd->pool);
        /* ID attributes aren't allowed to have a default */
        if (!defineAttribute(declElementType, declAttributeId,
                             declAttributeIsCdata, XML_FALSE, attVal, parser))
          return XML_ERROR_NO_MEMORY;
        if (attlistDeclHandler && declAttributeType) {
          if (*declAttributeType == XML_T('(')
              || (*declAttributeType == XML_T('N')
                  && declAttributeType[1] == XML_T('O'))) {
            /* Enumerated or Notation type */
            if (!poolAppendChar(&tempPool, XML_T(')'))
                || !poolAppendChar(&tempPool, XML_T('\0')))
              return XML_ERROR_NO_MEMORY;
            declAttributeType = tempPool.start;
            poolFinish(&tempPool);
          }
          *eventEndPP = s;
          attlistDeclHandler(handlerArg, declElementType->name,
                             declAttributeId->name, declAttributeType,
                             attVal,
                             role == XML_ROLE_FIXED_ATTRIBUTE_VALUE);
          poolClear(&tempPool);
          handleDefault = XML_FALSE;
        }
      }
      break;
    case XML_ROLE_ENTITY_VALUE:
      if (dtd->keepProcessing) {
        enum XML_Error result = storeEntityValue(parser, enc,
                                            s + enc->minBytesPerChar,
                                            next - enc->minBytesPerChar);
        if (declEntity) {
          declEntity->textPtr = poolStart(&dtd->entityValuePool);
          declEntity->textLen = poolLength(&dtd->entityValuePool);
          poolFinish(&dtd->entityValuePool);
          if (entityDeclHandler) {
            *eventEndPP = s;
            entityDeclHandler(handlerArg,
                              declEntity->name,
                              declEntity->is_param,
                              declEntity->textPtr,
                              declEntity->textLen,
                              curBase, 0, 0, 0);
            handleDefault = XML_FALSE;
          }
        }
        else
          poolDiscard(&dtd->entityValuePool);
        if (result != XML_ERROR_NONE)
          return result;
      }
      break;
    case XML_ROLE_DOCTYPE_SYSTEM_ID:
#ifdef XML_DTD
      useForeignDTD = XML_FALSE;
#endif /* XML_DTD */
      dtd->hasParamEntityRefs = XML_TRUE;
      if (startDoctypeDeclHandler) {
        doctypeSysid = poolStoreString(&tempPool, enc,
                                       s + enc->minBytesPerChar,
                                       next - enc->minBytesPerChar);
        if (doctypeSysid == NULL)
          return XML_ERROR_NO_MEMORY;
        poolFinish(&tempPool);
        handleDefault = XML_FALSE;
      }
#ifdef XML_DTD
      else
        /* use externalSubsetName to make doctypeSysid non-NULL
           for the case where no startDoctypeDeclHandler is set */
        doctypeSysid = externalSubsetName;
#endif /* XML_DTD */
      if (!dtd->standalone
#ifdef XML_DTD
          && !paramEntityParsing
#endif /* XML_DTD */
          && notStandaloneHandler
          && !notStandaloneHandler(handlerArg))
        return XML_ERROR_NOT_STANDALONE;
#ifndef XML_DTD
      break;
#else /* XML_DTD */
      if (!declEntity) {
        declEntity = (ENTITY *)lookup(&dtd->paramEntities,
                                      externalSubsetName,
                                      sizeof(ENTITY));
        if (!declEntity)
          return XML_ERROR_NO_MEMORY;
        declEntity->publicId = NULL;
      }
      /* fall through */
#endif /* XML_DTD */
    case XML_ROLE_ENTITY_SYSTEM_ID:
      if (dtd->keepProcessing && declEntity) {
        declEntity->systemId = poolStoreString(&dtd->pool, enc,
                                               s + enc->minBytesPerChar,
                                               next - enc->minBytesPerChar);
        if (!declEntity->systemId)
          return XML_ERROR_NO_MEMORY;
        declEntity->base = curBase;
        poolFinish(&dtd->pool);
        if (entityDeclHandler)
          handleDefault = XML_FALSE;
      }
      break;
    case XML_ROLE_ENTITY_COMPLETE:
      if (dtd->keepProcessing && declEntity && entityDeclHandler) {
        *eventEndPP = s;
        entityDeclHandler(handlerArg,
                          declEntity->name,
                          declEntity->is_param,
                          0,0,
                          declEntity->base,
                          declEntity->systemId,
                          declEntity->publicId,
                          0);
        handleDefault = XML_FALSE;
      }
      break;
    case XML_ROLE_ENTITY_NOTATION_NAME:
      if (dtd->keepProcessing && declEntity) {
        declEntity->notation = poolStoreString(&dtd->pool, enc, s, next);
        if (!declEntity->notation)
          return XML_ERROR_NO_MEMORY;
        poolFinish(&dtd->pool);
        if (unparsedEntityDeclHandler) {
          *eventEndPP = s;
          unparsedEntityDeclHandler(handlerArg,
                                    declEntity->name,
                                    declEntity->base,
                                    declEntity->systemId,
                                    declEntity->publicId,
                                    declEntity->notation);
          handleDefault = XML_FALSE;
        }
        else if (entityDeclHandler) {
          *eventEndPP = s;
          entityDeclHandler(handlerArg,
                            declEntity->name,
                            0,0,0,
                            declEntity->base,
                            declEntity->systemId,
                            declEntity->publicId,
                            declEntity->notation);
          handleDefault = XML_FALSE;
        }
      }
      break;
    case XML_ROLE_GENERAL_ENTITY_NAME:
      {
        if (XmlPredefinedEntityName(enc, s, next)) {
          declEntity = NULL;
          break;
        }
        if (dtd->keepProcessing) {
          const XML_Char *name = poolStoreString(&dtd->pool, enc, s, next);
          if (!name)
            return XML_ERROR_NO_MEMORY;
          declEntity = (ENTITY *)lookup(&dtd->generalEntities, name,
                                        sizeof(ENTITY));
          if (!declEntity)
            return XML_ERROR_NO_MEMORY;
          if (declEntity->name != name) {
            poolDiscard(&dtd->pool);
            declEntity = NULL;
          }
          else {
            poolFinish(&dtd->pool);
            declEntity->publicId = NULL;
            declEntity->is_param = XML_FALSE;
            /* if we have a parent parser or are reading an internal parameter
               entity, then the entity declaration is not considered "internal"
            */
            declEntity->is_internal = !(parentParser || openInternalEntities);
            if (entityDeclHandler)
              handleDefault = XML_FALSE;
          }
        }
        else {
          poolDiscard(&dtd->pool);
          declEntity = NULL;
        }
      }
      break;
    case XML_ROLE_PARAM_ENTITY_NAME:
#ifdef XML_DTD
      if (dtd->keepProcessing) {
        const XML_Char *name = poolStoreString(&dtd->pool, enc, s, next);
        if (!name)
          return XML_ERROR_NO_MEMORY;
        declEntity = (ENTITY *)lookup(&dtd->paramEntities,
                                           name, sizeof(ENTITY));
        if (!declEntity)
          return XML_ERROR_NO_MEMORY;
        if (declEntity->name != name) {
          poolDiscard(&dtd->pool);
          declEntity = NULL;
        }
        else {
          poolFinish(&dtd->pool);
          declEntity->publicId = NULL;
          declEntity->is_param = XML_TRUE;
          /* if we have a parent parser or are reading an internal parameter
             entity, then the entity declaration is not considered "internal"
          */
          declEntity->is_internal = !(parentParser || openInternalEntities);
          if (entityDeclHandler)
            handleDefault = XML_FALSE;
        }
      }
      else {
        poolDiscard(&dtd->pool);
        declEntity = NULL;
      }
#else /* not XML_DTD */
      declEntity = NULL;
#endif /* XML_DTD */
      break;
    case XML_ROLE_NOTATION_NAME:
      declNotationPublicId = NULL;
      declNotationName = NULL;
      if (notationDeclHandler) {
        declNotationName = poolStoreString(&tempPool, enc, s, next);
        if (!declNotationName)
          return XML_ERROR_NO_MEMORY;
        poolFinish(&tempPool);
        handleDefault = XML_FALSE;
      }
      break;
    case XML_ROLE_NOTATION_PUBLIC_ID:
      if (!XmlIsPublicId(enc, s, next, eventPP))
        return XML_ERROR_SYNTAX;
      if (declNotationName) {  /* means notationDeclHandler != NULL */
        XML_Char *tem = poolStoreString(&tempPool,
                                        enc,
                                        s + enc->minBytesPerChar,
                                        next - enc->minBytesPerChar);
        if (!tem)
          return XML_ERROR_NO_MEMORY;
        normalizePublicId(tem);
        declNotationPublicId = tem;
        poolFinish(&tempPool);
        handleDefault = XML_FALSE;
      }
      break;
    case XML_ROLE_NOTATION_SYSTEM_ID:
      if (declNotationName && notationDeclHandler) {
        const XML_Char *systemId
          = poolStoreString(&tempPool, enc,
                            s + enc->minBytesPerChar,
                            next - enc->minBytesPerChar);
        if (!systemId)
          return XML_ERROR_NO_MEMORY;
        *eventEndPP = s;
        notationDeclHandler(handlerArg,
                            declNotationName,
                            curBase,
                            systemId,
                            declNotationPublicId);
        handleDefault = XML_FALSE;
      }
      poolClear(&tempPool);
      break;
    case XML_ROLE_NOTATION_NO_SYSTEM_ID:
      if (declNotationPublicId && notationDeclHandler) {
        *eventEndPP = s;
        notationDeclHandler(handlerArg,
                            declNotationName,
                            curBase,
                            0,
                            declNotationPublicId);
        handleDefault = XML_FALSE;
      }
      poolClear(&tempPool);
      break;
    case XML_ROLE_ERROR:
      switch (tok) {
      case XML_TOK_PARAM_ENTITY_REF:
        return XML_ERROR_PARAM_ENTITY_REF;
      case XML_TOK_XML_DECL:
        return XML_ERROR_MISPLACED_XML_PI;
      default:
        return XML_ERROR_SYNTAX;
      }
#ifdef XML_DTD
    case XML_ROLE_IGNORE_SECT:
      {
        enum XML_Error result;
        if (defaultHandler)
          reportDefault(parser, enc, s, next);
        handleDefault = XML_FALSE;
        result = doIgnoreSection(parser, enc, &next, end, nextPtr);
        if (!next) {
          processor = ignoreSectionProcessor;
          return result;
        }
      }
      break;
#endif /* XML_DTD */
    case XML_ROLE_GROUP_OPEN:
      if (prologState.level >= groupSize) {
        if (groupSize) {
          char *temp = (char *)REALLOC(groupConnector, groupSize *= 2);
          if (temp == NULL)
            return XML_ERROR_NO_MEMORY;
          groupConnector = temp;
          if (dtd->scaffIndex) {
            int *temp = (int *)REALLOC(dtd->scaffIndex,
                          groupSize * sizeof(int));
            if (temp == NULL)
              return XML_ERROR_NO_MEMORY;
            dtd->scaffIndex = temp;
          }
        }
        else {
          groupConnector = (char *)MALLOC(groupSize = 32);
          if (!groupConnector)
            return XML_ERROR_NO_MEMORY;
        }
      }
      groupConnector[prologState.level] = 0;
      if (dtd->in_eldecl) {
        int myindex = nextScaffoldPart(parser);
        if (myindex < 0)
          return XML_ERROR_NO_MEMORY;
        dtd->scaffIndex[dtd->scaffLevel] = myindex;
        dtd->scaffLevel++;
        dtd->scaffold[myindex].type = XML_CTYPE_SEQ;
        if (elementDeclHandler)
          handleDefault = XML_FALSE;
      }
      break;
    case XML_ROLE_GROUP_SEQUENCE:
      if (groupConnector[prologState.level] == '|')
        return XML_ERROR_SYNTAX;
      groupConnector[prologState.level] = ',';
      if (dtd->in_eldecl && elementDeclHandler)
        handleDefault = XML_FALSE;
      break;
    case XML_ROLE_GROUP_CHOICE:
      if (groupConnector[prologState.level] == ',')
        return XML_ERROR_SYNTAX;
      if (dtd->in_eldecl
          && !groupConnector[prologState.level]
          && (dtd->scaffold[dtd->scaffIndex[dtd->scaffLevel - 1]].type
              != XML_CTYPE_MIXED)
          ) {
        dtd->scaffold[dtd->scaffIndex[dtd->scaffLevel - 1]].type
            = XML_CTYPE_CHOICE;
        if (elementDeclHandler)
          handleDefault = XML_FALSE;
      }
      groupConnector[prologState.level] = '|';
      break;
    case XML_ROLE_PARAM_ENTITY_REF:
#ifdef XML_DTD
    case XML_ROLE_INNER_PARAM_ENTITY_REF:
      /* PE references in internal subset are
         not allowed within declarations      */
      if (prologState.documentEntity &&
          role == XML_ROLE_INNER_PARAM_ENTITY_REF)
        return XML_ERROR_PARAM_ENTITY_REF;
      dtd->hasParamEntityRefs = XML_TRUE;
      if (!paramEntityParsing)
        dtd->keepProcessing = dtd->standalone;
      else {
        const XML_Char *name;
        ENTITY *entity;
        name = poolStoreString(&dtd->pool, enc,
                                s + enc->minBytesPerChar,
                                next - enc->minBytesPerChar);
        if (!name)
          return XML_ERROR_NO_MEMORY;
        entity = (ENTITY *)lookup(&dtd->paramEntities, name, 0);
        poolDiscard(&dtd->pool);
        /* first, determine if a check for an existing declaration is needed;
           if yes, check that the entity exists, and that it is internal,
           otherwise call the skipped entity handler
        */
        if (prologState.documentEntity &&
            (dtd->standalone
             ? !openInternalEntities
             : !dtd->hasParamEntityRefs)) {
          if (!entity)
            return XML_ERROR_UNDEFINED_ENTITY;
          else if (!entity->is_internal)
            return XML_ERROR_ENTITY_DECLARED_IN_PE;
        }
        else if (!entity) {
          dtd->keepProcessing = dtd->standalone;
          /* cannot report skipped entities in declarations */
          if ((role == XML_ROLE_PARAM_ENTITY_REF) && skippedEntityHandler) {
            skippedEntityHandler(handlerArg, name, 1);
            handleDefault = XML_FALSE;
          }
          break;
        }
        if (entity->open)
          return XML_ERROR_RECURSIVE_ENTITY_REF;
        if (entity->textPtr) {
          enum XML_Error result;
          result = processInternalParamEntity(parser, entity);
          if (result != XML_ERROR_NONE)
            return result;
          handleDefault = XML_FALSE;
          break;
        }
        if (externalEntityRefHandler) {
          dtd->paramEntityRead = XML_FALSE;
          entity->open = XML_TRUE;
          if (!externalEntityRefHandler(externalEntityRefHandlerArg,
                                        0,
                                        entity->base,
                                        entity->systemId,
                                        entity->publicId)) {
            entity->open = XML_FALSE;
            return XML_ERROR_EXTERNAL_ENTITY_HANDLING;
          }
          entity->open = XML_FALSE;
          handleDefault = XML_FALSE;
          if (!dtd->paramEntityRead) {
            dtd->keepProcessing = dtd->standalone;
            break;
          }
        }
        else {
          dtd->keepProcessing = dtd->standalone;
          break;
        }
      }
#endif /* XML_DTD */
      if (!dtd->standalone &&
          notStandaloneHandler &&
          !notStandaloneHandler(handlerArg))
        return XML_ERROR_NOT_STANDALONE;
      break;

    /* Element declaration stuff */

    case XML_ROLE_ELEMENT_NAME:
      if (elementDeclHandler) {
        declElementType = getElementType(parser, enc, s, next);
        if (!declElementType)
          return XML_ERROR_NO_MEMORY;
        dtd->scaffLevel = 0;
        dtd->scaffCount = 0;
        dtd->in_eldecl = XML_TRUE;
        handleDefault = XML_FALSE;
      }
      break;

    case XML_ROLE_CONTENT_ANY:
    case XML_ROLE_CONTENT_EMPTY:
      if (dtd->in_eldecl) {
        if (elementDeclHandler) {
          XML_Content * content = (XML_Content *) MALLOC(sizeof(XML_Content));
          if (!content)
            return XML_ERROR_NO_MEMORY;
          content->quant = XML_CQUANT_NONE;
          content->name = NULL;
          content->numchildren = 0;
          content->children = NULL;
          content->type = ((role == XML_ROLE_CONTENT_ANY) ?
                           XML_CTYPE_ANY :
                           XML_CTYPE_EMPTY);
          *eventEndPP = s;
          elementDeclHandler(handlerArg, declElementType->name, content);
          handleDefault = XML_FALSE;
        }
        dtd->in_eldecl = XML_FALSE;
      }
      break;

    case XML_ROLE_CONTENT_PCDATA:
      if (dtd->in_eldecl) {
        dtd->scaffold[dtd->scaffIndex[dtd->scaffLevel - 1]].type
            = XML_CTYPE_MIXED;
        if (elementDeclHandler)
          handleDefault = XML_FALSE;
      }
      break;

    case XML_ROLE_CONTENT_ELEMENT:
      quant = XML_CQUANT_NONE;
      goto elementContent;
    case XML_ROLE_CONTENT_ELEMENT_OPT:
      quant = XML_CQUANT_OPT;
      goto elementContent;
    case XML_ROLE_CONTENT_ELEMENT_REP:
      quant = XML_CQUANT_REP;
      goto elementContent;
    case XML_ROLE_CONTENT_ELEMENT_PLUS:
      quant = XML_CQUANT_PLUS;
    elementContent:
      if (dtd->in_eldecl) {
        ELEMENT_TYPE *el;
        const XML_Char *name;
        int nameLen;
        const char *nxt = (quant == XML_CQUANT_NONE
                           ? next
                           : next - enc->minBytesPerChar);
        int myindex = nextScaffoldPart(parser);
        if (myindex < 0)
          return XML_ERROR_NO_MEMORY;
        dtd->scaffold[myindex].type = XML_CTYPE_NAME;
        dtd->scaffold[myindex].quant = quant;
        el = getElementType(parser, enc, s, nxt);
        if (!el)
          return XML_ERROR_NO_MEMORY;
        name = el->name;
        dtd->scaffold[myindex].name = name;
        nameLen = 0;
        for (; name[nameLen++]; )
            ;
        dtd->contentStringLen +=  nameLen;
        if (elementDeclHandler)
          handleDefault = XML_FALSE;
      }
      break;

    case XML_ROLE_GROUP_CLOSE:
      quant = XML_CQUANT_NONE;
      goto closeGroup;
    case XML_ROLE_GROUP_CLOSE_OPT:
      quant = XML_CQUANT_OPT;
      goto closeGroup;
    case XML_ROLE_GROUP_CLOSE_REP:
      quant = XML_CQUANT_REP;
      goto closeGroup;
    case XML_ROLE_GROUP_CLOSE_PLUS:
      quant = XML_CQUANT_PLUS;
    closeGroup:
      if (dtd->in_eldecl) {
        if (elementDeclHandler)
          handleDefault = XML_FALSE;
        dtd->scaffLevel--;
        dtd->scaffold[dtd->scaffIndex[dtd->scaffLevel]].quant = quant;
        if (dtd->scaffLevel == 0) {
          if (!handleDefault) {
            XML_Content *model = build_model(parser);
            if (!model)
              return XML_ERROR_NO_MEMORY;
            *eventEndPP = s;
            elementDeclHandler(handlerArg, declElementType->name, model);
          }
          dtd->in_eldecl = XML_FALSE;
          dtd->contentStringLen = 0;
        }
      }
      break;
      /* End element declaration stuff */

    case XML_ROLE_PI:
      if (!reportProcessingInstruction(parser, enc, s, next))
        return XML_ERROR_NO_MEMORY;
      handleDefault = XML_FALSE;
      break;
    case XML_ROLE_COMMENT:
      if (!reportComment(parser, enc, s, next))
        return XML_ERROR_NO_MEMORY;
      handleDefault = XML_FALSE;
      break;
    case XML_ROLE_NONE:
      switch (tok) {
      case XML_TOK_BOM:
        handleDefault = XML_FALSE;
        break;
      }
      break;
    case XML_ROLE_DOCTYPE_NONE:
      if (startDoctypeDeclHandler)
        handleDefault = XML_FALSE;
      break;
    case XML_ROLE_ENTITY_NONE:
      if (dtd->keepProcessing && entityDeclHandler)
        handleDefault = XML_FALSE;
      break;
    case XML_ROLE_NOTATION_NONE:
      if (notationDeclHandler)
        handleDefault = XML_FALSE;
      break;
    case XML_ROLE_ATTLIST_NONE:
      if (dtd->keepProcessing && attlistDeclHandler)
        handleDefault = XML_FALSE;
      break;
    case XML_ROLE_ELEMENT_NONE:
      if (elementDeclHandler)
        handleDefault = XML_FALSE;
      break;
    } /* end of big switch */

    if (handleDefault && defaultHandler)
      reportDefault(parser, enc, s, next);

    s = next;
    tok = XmlPrologTok(enc, s, end, &next);
  }
  /* not reached */
}

static enum XML_Error PTRCALL
epilogProcessor(XML_Parser parser,
                const char *s,
                const char *end,
                const char **nextPtr)
{
  processor = epilogProcessor;
  eventPtr = s;
  for (;;) {
    const char *next = NULL;
    int tok = XmlPrologTok(encoding, s, end, &next);
    eventEndPtr = next;
    switch (tok) {
    /* report partial linebreak - it might be the last token */
    case -XML_TOK_PROLOG_S:
      if (defaultHandler) {
        eventEndPtr = next;
        reportDefault(parser, encoding, s, next);
      }
      if (nextPtr)
        *nextPtr = next;
      return XML_ERROR_NONE;
    case XML_TOK_NONE:
      if (nextPtr)
        *nextPtr = s;
      return XML_ERROR_NONE;
    case XML_TOK_PROLOG_S:
      if (defaultHandler)
        reportDefault(parser, encoding, s, next);
      break;
    case XML_TOK_PI:
      if (!reportProcessingInstruction(parser, encoding, s, next))
        return XML_ERROR_NO_MEMORY;
      break;
    case XML_TOK_COMMENT:
      if (!reportComment(parser, encoding, s, next))
        return XML_ERROR_NO_MEMORY;
      break;
    case XML_TOK_INVALID:
      eventPtr = next;
      return XML_ERROR_INVALID_TOKEN;
    case XML_TOK_PARTIAL:
      if (nextPtr) {
        *nextPtr = s;
        return XML_ERROR_NONE;
      }
      return XML_ERROR_UNCLOSED_TOKEN;
    case XML_TOK_PARTIAL_CHAR:
      if (nextPtr) {
        *nextPtr = s;
        return XML_ERROR_NONE;
      }
      return XML_ERROR_PARTIAL_CHAR;
    default:
      return XML_ERROR_JUNK_AFTER_DOC_ELEMENT;
    }
    eventPtr = s = next;
  }
}

#ifdef XML_DTD

static enum XML_Error
processInternalParamEntity(XML_Parser parser, ENTITY *entity)
{
  const char *s, *end, *next;
  int tok;
  enum XML_Error result;
  OPEN_INTERNAL_ENTITY openEntity;
  entity->open = XML_TRUE;
  openEntity.next = openInternalEntities;
  openInternalEntities = &openEntity;
  openEntity.entity = entity;
  openEntity.internalEventPtr = NULL;
  openEntity.internalEventEndPtr = NULL;
  s = (char *)entity->textPtr;
  end = (char *)(entity->textPtr + entity->textLen);
  tok = XmlPrologTok(internalEncoding, s, end, &next);
  result = doProlog(parser, internalEncoding, s, end, tok, next, 0);
  entity->open = XML_FALSE;
  openInternalEntities = openEntity.next;
  return result;
}

#endif /* XML_DTD */

static enum XML_Error PTRCALL
errorProcessor(XML_Parser parser,
               const char *s,
               const char *end,
               const char **nextPtr)
{
  return errorCode;
}

static enum XML_Error
storeAttributeValue(XML_Parser parser, const ENCODING *enc, XML_Bool isCdata,
                    const char *ptr, const char *end,
                    STRING_POOL *pool)
{
  enum XML_Error result = appendAttributeValue(parser, enc, isCdata, ptr,
                                               end, pool);
  if (result)
    return result;
  if (!isCdata && poolLength(pool) && poolLastChar(pool) == 0x20)
    poolChop(pool);
  if (!poolAppendChar(pool, XML_T('\0')))
    return XML_ERROR_NO_MEMORY;
  return XML_ERROR_NONE;
}

static enum XML_Error
appendAttributeValue(XML_Parser parser, const ENCODING *enc, XML_Bool isCdata,
                     const char *ptr, const char *end,
                     STRING_POOL *pool)
{
  DTD * const dtd = _dtd;  /* save one level of indirection */
  for (;;) {
    const char *next;
    int tok = XmlAttributeValueTok(enc, ptr, end, &next);
    switch (tok) {
    case XML_TOK_NONE:
      return XML_ERROR_NONE;
    case XML_TOK_INVALID:
      if (enc == encoding)
        eventPtr = next;
      return XML_ERROR_INVALID_TOKEN;
    case XML_TOK_PARTIAL:
      if (enc == encoding)
        eventPtr = ptr;
      return XML_ERROR_INVALID_TOKEN;
    case XML_TOK_CHAR_REF:
      {
        XML_Char buf[XML_ENCODE_MAX];
        int i;
        int n = XmlCharRefNumber(enc, ptr);
        if (n < 0) {
          if (enc == encoding)
            eventPtr = ptr;
          return XML_ERROR_BAD_CHAR_REF;
        }
        if (!isCdata
            && n == 0x20 /* space */
            && (poolLength(pool) == 0 || poolLastChar(pool) == 0x20))
          break;
        n = XmlEncode(n, (ICHAR *)buf);
        if (!n) {
          if (enc == encoding)
            eventPtr = ptr;
          return XML_ERROR_BAD_CHAR_REF;
        }
        for (i = 0; i < n; i++) {
          if (!poolAppendChar(pool, buf[i]))
            return XML_ERROR_NO_MEMORY;
        }
      }
      break;
    case XML_TOK_DATA_CHARS:
      if (!poolAppend(pool, enc, ptr, next))
        return XML_ERROR_NO_MEMORY;
      break;
    case XML_TOK_TRAILING_CR:
      next = ptr + enc->minBytesPerChar;
      /* fall through */
    case XML_TOK_ATTRIBUTE_VALUE_S:
    case XML_TOK_DATA_NEWLINE:
      if (!isCdata && (poolLength(pool) == 0 || poolLastChar(pool) == 0x20))
        break;
      if (!poolAppendChar(pool, 0x20))
        return XML_ERROR_NO_MEMORY;
      break;
    case XML_TOK_ENTITY_REF:
      {
        const XML_Char *name;
        ENTITY *entity;
        char checkEntityDecl;
        XML_Char ch = (XML_Char) XmlPredefinedEntityName(enc,
                                              ptr + enc->minBytesPerChar,
                                              next - enc->minBytesPerChar);
        if (ch) {
          if (!poolAppendChar(pool, ch))
                return XML_ERROR_NO_MEMORY;
          break;
        }
        name = poolStoreString(&temp2Pool, enc,
                               ptr + enc->minBytesPerChar,
                               next - enc->minBytesPerChar);
        if (!name)
          return XML_ERROR_NO_MEMORY;
        entity = (ENTITY *)lookup(&dtd->generalEntities, name, 0);
        poolDiscard(&temp2Pool);
        /* first, determine if a check for an existing declaration is needed;
           if yes, check that the entity exists, and that it is internal,
           otherwise call the default handler (if called from content)
        */
        if (pool == &dtd->pool)  /* are we called from prolog? */
          checkEntityDecl =
#ifdef XML_DTD
              prologState.documentEntity &&
#endif /* XML_DTD */
              (dtd->standalone
               ? !openInternalEntities
               : !dtd->hasParamEntityRefs);
        else /* if (pool == &tempPool): we are called from content */
          checkEntityDecl = !dtd->hasParamEntityRefs || dtd->standalone;
        if (checkEntityDecl) {
          if (!entity)
            return XML_ERROR_UNDEFINED_ENTITY;
          else if (!entity->is_internal)
            return XML_ERROR_ENTITY_DECLARED_IN_PE;
        }
        else if (!entity) {
          /* cannot report skipped entity here - see comments on
             skippedEntityHandler
          if (skippedEntityHandler)
            skippedEntityHandler(handlerArg, name, 0);
          */
          if ((pool == &tempPool) && defaultHandler)
            reportDefault(parser, enc, ptr, next);
          break;
        }
        if (entity->open) {
          if (enc == encoding)
            eventPtr = ptr;
          return XML_ERROR_RECURSIVE_ENTITY_REF;
        }
        if (entity->notation) {
          if (enc == encoding)
            eventPtr = ptr;
          return XML_ERROR_BINARY_ENTITY_REF;
        }
        if (!entity->textPtr) {
          if (enc == encoding)
            eventPtr = ptr;
              return XML_ERROR_ATTRIBUTE_EXTERNAL_ENTITY_REF;
        }
        else {
          enum XML_Error result;
          const XML_Char *textEnd = entity->textPtr + entity->textLen;
          entity->open = XML_TRUE;
          result = appendAttributeValue(parser, internalEncoding, isCdata,
                                        (char *)entity->textPtr,
                                        (char *)textEnd, pool);
          entity->open = XML_FALSE;
          if (result)
            return result;
        }
      }
      break;
    default:
      if (enc == encoding)
        eventPtr = ptr;
      return XML_ERROR_UNEXPECTED_STATE;
    }
    ptr = next;
  }
  /* not reached */
}

static enum XML_Error
storeEntityValue(XML_Parser parser,
                 const ENCODING *enc,
                 const char *entityTextPtr,
                 const char *entityTextEnd)
{
  DTD * const dtd = _dtd;  /* save one level of indirection */
  STRING_POOL *pool = &(dtd->entityValuePool);
  enum XML_Error result = XML_ERROR_NONE;
#ifdef XML_DTD
  int oldInEntityValue = prologState.inEntityValue;
  prologState.inEntityValue = 1;
#endif /* XML_DTD */
  /* never return Null for the value argument in EntityDeclHandler,
     since this would indicate an external entity; therefore we
     have to make sure that entityValuePool.start is not null */
  if (!pool->blocks) {
    if (!poolGrow(pool))
      return XML_ERROR_NO_MEMORY;
  }

  for (;;) {
    const char *next;
    int tok = XmlEntityValueTok(enc, entityTextPtr, entityTextEnd, &next);
    switch (tok) {
    case XML_TOK_PARAM_ENTITY_REF:
#ifdef XML_DTD
      if (isParamEntity || enc != encoding) {
        const XML_Char *name;
        ENTITY *entity;
        name = poolStoreString(&tempPool, enc,
                               entityTextPtr + enc->minBytesPerChar,
                               next - enc->minBytesPerChar);
        if (!name) {
          result = XML_ERROR_NO_MEMORY;
          goto endEntityValue;
        }
        entity = (ENTITY *)lookup(&dtd->paramEntities, name, 0);
        poolDiscard(&tempPool);
        if (!entity) {
          /* not a well-formedness error - see XML 1.0: WFC Entity Declared */
          /* cannot report skipped entity here - see comments on
             skippedEntityHandler
          if (skippedEntityHandler)
            skippedEntityHandler(handlerArg, name, 0);
          */
          dtd->keepProcessing = dtd->standalone;
          goto endEntityValue;
        }
        if (entity->open) {
          if (enc == encoding)
            eventPtr = entityTextPtr;
          result = XML_ERROR_RECURSIVE_ENTITY_REF;
          goto endEntityValue;
        }
        if (entity->systemId) {
          if (externalEntityRefHandler) {
            dtd->paramEntityRead = XML_FALSE;
            entity->open = XML_TRUE;
            if (!externalEntityRefHandler(externalEntityRefHandlerArg,
                                          0,
                                          entity->base,
                                          entity->systemId,
                                          entity->publicId)) {
              entity->open = XML_FALSE;
              result = XML_ERROR_EXTERNAL_ENTITY_HANDLING;
              goto endEntityValue;
            }
            entity->open = XML_FALSE;
            if (!dtd->paramEntityRead)
              dtd->keepProcessing = dtd->standalone;
          }
          else
            dtd->keepProcessing = dtd->standalone;
        }
        else {
          entity->open = XML_TRUE;
          result = storeEntityValue(parser,
                                    internalEncoding,
                                    (char *)entity->textPtr,
                                    (char *)(entity->textPtr
                                             + entity->textLen));
          entity->open = XML_FALSE;
          if (result)
            goto endEntityValue;
        }
        break;
      }
#endif /* XML_DTD */
      /* in the internal subset, PE references are not legal
         within markup declarations, e.g entity values in this case */
      eventPtr = entityTextPtr;
      result = XML_ERROR_PARAM_ENTITY_REF;
      goto endEntityValue;
    case XML_TOK_NONE:
      result = XML_ERROR_NONE;
      goto endEntityValue;
    case XML_TOK_ENTITY_REF:
    case XML_TOK_DATA_CHARS:
      if (!poolAppend(pool, enc, entityTextPtr, next)) {
        result = XML_ERROR_NO_MEMORY;
        goto endEntityValue;
      }
      break;
    case XML_TOK_TRAILING_CR:
      next = entityTextPtr + enc->minBytesPerChar;
      /* fall through */
    case XML_TOK_DATA_NEWLINE:
      if (pool->end == pool->ptr && !poolGrow(pool)) {
              result = XML_ERROR_NO_MEMORY;
        goto endEntityValue;
      }
      *(pool->ptr)++ = 0xA;
      break;
    case XML_TOK_CHAR_REF:
      {
        XML_Char buf[XML_ENCODE_MAX];
        int i;
        int n = XmlCharRefNumber(enc, entityTextPtr);
        if (n < 0) {
          if (enc == encoding)
            eventPtr = entityTextPtr;
          result = XML_ERROR_BAD_CHAR_REF;
          goto endEntityValue;
        }
        n = XmlEncode(n, (ICHAR *)buf);
        if (!n) {
          if (enc == encoding)
            eventPtr = entityTextPtr;
          result = XML_ERROR_BAD_CHAR_REF;
          goto endEntityValue;
        }
        for (i = 0; i < n; i++) {
          if (pool->end == pool->ptr && !poolGrow(pool)) {
            result = XML_ERROR_NO_MEMORY;
            goto endEntityValue;
          }
          *(pool->ptr)++ = buf[i];
        }
      }
      break;
    case XML_TOK_PARTIAL:
      if (enc == encoding)
        eventPtr = entityTextPtr;
      result = XML_ERROR_INVALID_TOKEN;
      goto endEntityValue;
    case XML_TOK_INVALID:
      if (enc == encoding)
        eventPtr = next;
      result = XML_ERROR_INVALID_TOKEN;
      goto endEntityValue;
    default:
      if (enc == encoding)
        eventPtr = entityTextPtr;
      result = XML_ERROR_UNEXPECTED_STATE;
      goto endEntityValue;
    }
    entityTextPtr = next;
  }
endEntityValue:
#ifdef XML_DTD
  prologState.inEntityValue = oldInEntityValue;
#endif /* XML_DTD */
  return result;
}

static void FASTCALL
normalizeLines(XML_Char *s)
{
  XML_Char *p;
  for (;; s++) {
    if (*s == XML_T('\0'))
      return;
    if (*s == 0xD)
      break;
  }
  p = s;
  do {
    if (*s == 0xD) {
      *p++ = 0xA;
      if (*++s == 0xA)
        s++;
    }
    else
      *p++ = *s++;
  } while (*s);
  *p = XML_T('\0');
}

static int
reportProcessingInstruction(XML_Parser parser, const ENCODING *enc,
                            const char *start, const char *end)
{
  const XML_Char *target;
  XML_Char *data;
  const char *tem;
  if (!processingInstructionHandler) {
    if (defaultHandler)
      reportDefault(parser, enc, start, end);
    return 1;
  }
  start += enc->minBytesPerChar * 2;
  tem = start + XmlNameLength(enc, start);
  target = poolStoreString(&tempPool, enc, start, tem);
  if (!target)
    return 0;
  poolFinish(&tempPool);
  data = poolStoreString(&tempPool, enc,
                        XmlSkipS(enc, tem),
                        end - enc->minBytesPerChar*2);
  if (!data)
    return 0;
  normalizeLines(data);
  processingInstructionHandler(handlerArg, target, data);
  poolClear(&tempPool);
  return 1;
}

static int
reportComment(XML_Parser parser, const ENCODING *enc,
              const char *start, const char *end)
{
  XML_Char *data;
  if (!commentHandler) {
    if (defaultHandler)
      reportDefault(parser, enc, start, end);
    return 1;
  }
  data = poolStoreString(&tempPool,
                         enc,
                         start + enc->minBytesPerChar * 4,
                         end - enc->minBytesPerChar * 3);
  if (!data)
    return 0;
  normalizeLines(data);
  commentHandler(handlerArg, data);
  poolClear(&tempPool);
  return 1;
}

static void
reportDefault(XML_Parser parser, const ENCODING *enc,
              const char *s, const char *end)
{
  if (MUST_CONVERT(enc, s)) {
    const char **eventPP;
    const char **eventEndPP;
    if (enc == encoding) {
      eventPP = &eventPtr;
      eventEndPP = &eventEndPtr;
    }
    else {
      eventPP = &(openInternalEntities->internalEventPtr);
      eventEndPP = &(openInternalEntities->internalEventEndPtr);
    }
    do {
      ICHAR *dataPtr = (ICHAR *)dataBuf;
      XmlConvert(enc, &s, end, &dataPtr, (ICHAR *)dataBufEnd);
      *eventEndPP = s;
      defaultHandler(handlerArg, dataBuf, dataPtr - (ICHAR *)dataBuf);
      *eventPP = s;
    } while (s != end);
  }
  else
    defaultHandler(handlerArg, (XML_Char *)s, (XML_Char *)end - (XML_Char *)s);
}


static int
defineAttribute(ELEMENT_TYPE *type, ATTRIBUTE_ID *attId, XML_Bool isCdata,
                XML_Bool isId, const XML_Char *value, XML_Parser parser)
{
  DEFAULT_ATTRIBUTE *att;
  if (value || isId) {
    /* The handling of default attributes gets messed up if we have
       a default which duplicates a non-default. */
    int i;
    for (i = 0; i < type->nDefaultAtts; i++)
      if (attId == type->defaultAtts[i].id)
        return 1;
    if (isId && !type->idAtt && !attId->xmlns)
      type->idAtt = attId;
  }
  if (type->nDefaultAtts == type->allocDefaultAtts) {
    if (type->allocDefaultAtts == 0) {
      type->allocDefaultAtts = 8;
      type->defaultAtts = (DEFAULT_ATTRIBUTE *)MALLOC(type->allocDefaultAtts
                            * sizeof(DEFAULT_ATTRIBUTE));
      if (!type->defaultAtts)
        return 0;
    }
    else {
      DEFAULT_ATTRIBUTE *temp;
      int count = type->allocDefaultAtts * 2;
      temp = (DEFAULT_ATTRIBUTE *)
        REALLOC(type->defaultAtts, (count * sizeof(DEFAULT_ATTRIBUTE)));
      if (temp == NULL)
        return 0;
      type->allocDefaultAtts = count;
      type->defaultAtts = temp;
    }
  }
  att = type->defaultAtts + type->nDefaultAtts;
  att->id = attId;
  att->value = value;
  att->isCdata = isCdata;
  if (!isCdata)
    attId->maybeTokenized = XML_TRUE;
  type->nDefaultAtts += 1;
  return 1;
}

static int
setElementTypePrefix(XML_Parser parser, ELEMENT_TYPE *elementType)
{
  DTD * const dtd = _dtd;  /* save one level of indirection */
  const XML_Char *name;
  for (name = elementType->name; *name; name++) {
    if (*name == XML_T(':')) {
      PREFIX *prefix;
      const XML_Char *s;
      for (s = elementType->name; s != name; s++) {
        if (!poolAppendChar(&dtd->pool, *s))
          return 0;
      }
      if (!poolAppendChar(&dtd->pool, XML_T('\0')))
        return 0;
      prefix = (PREFIX *)lookup(&dtd->prefixes, poolStart(&dtd->pool),
                                sizeof(PREFIX));
      if (!prefix)
        return 0;
      if (prefix->name == poolStart(&dtd->pool))
        poolFinish(&dtd->pool);
      else
        poolDiscard(&dtd->pool);
      elementType->prefix = prefix;

    }
  }
  return 1;
}

static ATTRIBUTE_ID *
getAttributeId(XML_Parser parser, const ENCODING *enc,
               const char *start, const char *end)
{
  DTD * const dtd = _dtd;  /* save one level of indirection */
  ATTRIBUTE_ID *id;
  const XML_Char *name;
  if (!poolAppendChar(&dtd->pool, XML_T('\0')))
    return NULL;
  name = poolStoreString(&dtd->pool, enc, start, end);
  if (!name)
    return NULL;
  ++name;
  id = (ATTRIBUTE_ID *)lookup(&dtd->attributeIds, name, sizeof(ATTRIBUTE_ID));
  if (!id)
    return NULL;
  if (id->name != name)
    poolDiscard(&dtd->pool);
  else {
    poolFinish(&dtd->pool);
    if (!ns)
      ;
    else if (name[0] == XML_T('x')
        && name[1] == XML_T('m')
        && name[2] == XML_T('l')
        && name[3] == XML_T('n')
        && name[4] == XML_T('s')
        && (name[5] == XML_T('\0') || name[5] == XML_T(':'))) {
      if (name[5] == XML_T('\0'))
        id->prefix = &dtd->defaultPrefix;
      else
        id->prefix = (PREFIX *)lookup(&dtd->prefixes, name + 6, sizeof(PREFIX));
      id->xmlns = XML_TRUE;
    }
    else {
      int i;
      for (i = 0; name[i]; i++) {
        if (name[i] == XML_T(':')) {
          int j;
          for (j = 0; j < i; j++) {
            if (!poolAppendChar(&dtd->pool, name[j]))
              return NULL;
          }
          if (!poolAppendChar(&dtd->pool, XML_T('\0')))
            return NULL;
          id->prefix = (PREFIX *)lookup(&dtd->prefixes, poolStart(&dtd->pool),
                                        sizeof(PREFIX));
          if (id->prefix->name == poolStart(&dtd->pool))
            poolFinish(&dtd->pool);
          else
            poolDiscard(&dtd->pool);
          break;
        }
      }
    }
  }
  return id;
}

#define CONTEXT_SEP XML_T('\f')

static const XML_Char *
getContext(XML_Parser parser)
{
  DTD * const dtd = _dtd;  /* save one level of indirection */
  HASH_TABLE_ITER iter;
  XML_Bool needSep = XML_FALSE;

  if (dtd->defaultPrefix.binding) {
    int i;
    int len;
    if (!poolAppendChar(&tempPool, XML_T('=')))
      return NULL;
    len = dtd->defaultPrefix.binding->uriLen;
    if (namespaceSeparator != XML_T('\0'))
      len--;
    for (i = 0; i < len; i++)
      if (!poolAppendChar(&tempPool, dtd->defaultPrefix.binding->uri[i]))
        return NULL;
    needSep = XML_TRUE;
  }

  hashTableIterInit(&iter, &(dtd->prefixes));
  for (;;) {
    int i;
    int len;
    const XML_Char *s;
    PREFIX *prefix = (PREFIX *)hashTableIterNext(&iter);
    if (!prefix)
      break;
    if (!prefix->binding)
      continue;
    if (needSep && !poolAppendChar(&tempPool, CONTEXT_SEP))
      return NULL;
    for (s = prefix->name; *s; s++)
      if (!poolAppendChar(&tempPool, *s))
        return NULL;
    if (!poolAppendChar(&tempPool, XML_T('=')))
      return NULL;
    len = prefix->binding->uriLen;
    if (namespaceSeparator != XML_T('\0'))
      len--;
    for (i = 0; i < len; i++)
      if (!poolAppendChar(&tempPool, prefix->binding->uri[i]))
        return NULL;
    needSep = XML_TRUE;
  }


  hashTableIterInit(&iter, &(dtd->generalEntities));
  for (;;) {
    const XML_Char *s;
    ENTITY *e = (ENTITY *)hashTableIterNext(&iter);
    if (!e)
      break;
    if (!e->open)
      continue;
    if (needSep && !poolAppendChar(&tempPool, CONTEXT_SEP))
      return NULL;
    for (s = e->name; *s; s++)
      if (!poolAppendChar(&tempPool, *s))
        return 0;
    needSep = XML_TRUE;
  }

  if (!poolAppendChar(&tempPool, XML_T('\0')))
    return NULL;
  return tempPool.start;
}

static XML_Bool
setContext(XML_Parser parser, const XML_Char *context)
{
  DTD * const dtd = _dtd;  /* save one level of indirection */
  const XML_Char *s = context;

  while (*context != XML_T('\0')) {
    if (*s == CONTEXT_SEP || *s == XML_T('\0')) {
      ENTITY *e;
      if (!poolAppendChar(&tempPool, XML_T('\0')))
        return XML_FALSE;
      e = (ENTITY *)lookup(&dtd->generalEntities, poolStart(&tempPool), 0);
      if (e)
        e->open = XML_TRUE;
      if (*s != XML_T('\0'))
        s++;
      context = s;
      poolDiscard(&tempPool);
    }
    else if (*s == XML_T('=')) {
      PREFIX *prefix;
      if (poolLength(&tempPool) == 0)
        prefix = &dtd->defaultPrefix;
      else {
        if (!poolAppendChar(&tempPool, XML_T('\0')))
          return XML_FALSE;
        prefix = (PREFIX *)lookup(&dtd->prefixes, poolStart(&tempPool),
                                  sizeof(PREFIX));
        if (!prefix)
          return XML_FALSE;
        if (prefix->name == poolStart(&tempPool)) {
          prefix->name = poolCopyString(&dtd->pool, prefix->name);
          if (!prefix->name)
            return XML_FALSE;
        }
        poolDiscard(&tempPool);
      }
      for (context = s + 1;
           *context != CONTEXT_SEP && *context != XML_T('\0');
           context++)
        if (!poolAppendChar(&tempPool, *context))
          return XML_FALSE;
      if (!poolAppendChar(&tempPool, XML_T('\0')))
        return XML_FALSE;
      if (addBinding(parser, prefix, 0, poolStart(&tempPool),
                     &inheritedBindings) != XML_ERROR_NONE)
        return XML_FALSE;
      poolDiscard(&tempPool);
      if (*context != XML_T('\0'))
        ++context;
      s = context;
    }
    else {
      if (!poolAppendChar(&tempPool, *s))
        return XML_FALSE;
      s++;
    }
  }
  return XML_TRUE;
}

static void FASTCALL
normalizePublicId(XML_Char *publicId)
{
  XML_Char *p = publicId;
  XML_Char *s;
  for (s = publicId; *s; s++) {
    switch (*s) {
    case 0x20:
    case 0xD:
    case 0xA:
      if (p != publicId && p[-1] != 0x20)
        *p++ = 0x20;
      break;
    default:
      *p++ = *s;
    }
  }
  if (p != publicId && p[-1] == 0x20)
    --p;
  *p = XML_T('\0');
}

static DTD *
dtdCreate(const XML_Memory_Handling_Suite *ms)
{
  DTD *p = (DTD *)ms->malloc_fcn(sizeof(DTD));
  if (p == NULL)
    return p;
  poolInit(&(p->pool), ms);
#ifdef XML_DTD
  poolInit(&(p->entityValuePool), ms);
#endif /* XML_DTD */
  hashTableInit(&(p->generalEntities), ms);
  hashTableInit(&(p->elementTypes), ms);
  hashTableInit(&(p->attributeIds), ms);
  hashTableInit(&(p->prefixes), ms);
#ifdef XML_DTD
  p->paramEntityRead = XML_FALSE;
  hashTableInit(&(p->paramEntities), ms);
#endif /* XML_DTD */
  p->defaultPrefix.name = NULL;
  p->defaultPrefix.binding = NULL;

  p->in_eldecl = XML_FALSE;
  p->scaffIndex = NULL;
  p->scaffold = NULL;
  p->scaffLevel = 0;
  p->scaffSize = 0;
  p->scaffCount = 0;
  p->contentStringLen = 0;

  p->keepProcessing = XML_TRUE;
  p->hasParamEntityRefs = XML_FALSE;
  p->standalone = XML_FALSE;
  return p;
}

static void
dtdReset(DTD *p, const XML_Memory_Handling_Suite *ms)
{
  HASH_TABLE_ITER iter;
  hashTableIterInit(&iter, &(p->elementTypes));
  for (;;) {
    ELEMENT_TYPE *e = (ELEMENT_TYPE *)hashTableIterNext(&iter);
    if (!e)
      break;
    if (e->allocDefaultAtts != 0)
      ms->free_fcn(e->defaultAtts);
  }
  hashTableClear(&(p->generalEntities));
#ifdef XML_DTD
  p->paramEntityRead = XML_FALSE;
  hashTableClear(&(p->paramEntities));
#endif /* XML_DTD */
  hashTableClear(&(p->elementTypes));
  hashTableClear(&(p->attributeIds));
  hashTableClear(&(p->prefixes));
  poolClear(&(p->pool));
#ifdef XML_DTD
  poolClear(&(p->entityValuePool));
#endif /* XML_DTD */
  p->defaultPrefix.name = NULL;
  p->defaultPrefix.binding = NULL;

  p->in_eldecl = XML_FALSE;
  if (p->scaffIndex) {
    ms->free_fcn(p->scaffIndex);
    p->scaffIndex = NULL;
  }
  if (p->scaffold) {
    ms->free_fcn(p->scaffold);
    p->scaffold = NULL;
  }
  p->scaffLevel = 0;
  p->scaffSize = 0;
  p->scaffCount = 0;
  p->contentStringLen = 0;

  p->keepProcessing = XML_TRUE;
  p->hasParamEntityRefs = XML_FALSE;
  p->standalone = XML_FALSE;
}

static void
dtdDestroy(DTD *p, XML_Bool isDocEntity, const XML_Memory_Handling_Suite *ms)
{
  HASH_TABLE_ITER iter;
  hashTableIterInit(&iter, &(p->elementTypes));
  for (;;) {
    ELEMENT_TYPE *e = (ELEMENT_TYPE *)hashTableIterNext(&iter);
    if (!e)
      break;
    if (e->allocDefaultAtts != 0)
      ms->free_fcn(e->defaultAtts);
  }
  hashTableDestroy(&(p->generalEntities));
#ifdef XML_DTD
  hashTableDestroy(&(p->paramEntities));
#endif /* XML_DTD */
  hashTableDestroy(&(p->elementTypes));
  hashTableDestroy(&(p->attributeIds));
  hashTableDestroy(&(p->prefixes));
  poolDestroy(&(p->pool));
#ifdef XML_DTD
  poolDestroy(&(p->entityValuePool));
#endif /* XML_DTD */
  if (isDocEntity) {
    if (p->scaffIndex)
      ms->free_fcn(p->scaffIndex);
    if (p->scaffold)
      ms->free_fcn(p->scaffold);
  }
  ms->free_fcn(p);
}

/* Do a deep copy of the DTD. Return 0 for out of memory, non-zero otherwise.
   The new DTD has already been initialized.
*/
static int
dtdCopy(DTD *newDtd, const DTD *oldDtd, const XML_Memory_Handling_Suite *ms)
{
  HASH_TABLE_ITER iter;

  /* Copy the prefix table. */

  hashTableIterInit(&iter, &(oldDtd->prefixes));
  for (;;) {
    const XML_Char *name;
    const PREFIX *oldP = (PREFIX *)hashTableIterNext(&iter);
    if (!oldP)
      break;
    name = poolCopyString(&(newDtd->pool), oldP->name);
    if (!name)
      return 0;
    if (!lookup(&(newDtd->prefixes), name, sizeof(PREFIX)))
      return 0;
  }

  hashTableIterInit(&iter, &(oldDtd->attributeIds));

  /* Copy the attribute id table. */

  for (;;) {
    ATTRIBUTE_ID *newA;
    const XML_Char *name;
    const ATTRIBUTE_ID *oldA = (ATTRIBUTE_ID *)hashTableIterNext(&iter);

    if (!oldA)
      break;
    /* Remember to allocate the scratch byte before the name. */
    if (!poolAppendChar(&(newDtd->pool), XML_T('\0')))
      return 0;
    name = poolCopyString(&(newDtd->pool), oldA->name);
    if (!name)
      return 0;
    ++name;
    newA = (ATTRIBUTE_ID *)lookup(&(newDtd->attributeIds), name,
                                  sizeof(ATTRIBUTE_ID));
    if (!newA)
      return 0;
    newA->maybeTokenized = oldA->maybeTokenized;
    if (oldA->prefix) {
      newA->xmlns = oldA->xmlns;
      if (oldA->prefix == &oldDtd->defaultPrefix)
        newA->prefix = &newDtd->defaultPrefix;
      else
        newA->prefix = (PREFIX *)lookup(&(newDtd->prefixes),
                                        oldA->prefix->name, 0);
    }
  }

  /* Copy the element type table. */

  hashTableIterInit(&iter, &(oldDtd->elementTypes));

  for (;;) {
    int i;
    ELEMENT_TYPE *newE;
    const XML_Char *name;
    const ELEMENT_TYPE *oldE = (ELEMENT_TYPE *)hashTableIterNext(&iter);
    if (!oldE)
      break;
    name = poolCopyString(&(newDtd->pool), oldE->name);
    if (!name)
      return 0;
    newE = (ELEMENT_TYPE *)lookup(&(newDtd->elementTypes), name,
                                  sizeof(ELEMENT_TYPE));
    if (!newE)
      return 0;
    if (oldE->nDefaultAtts) {
      newE->defaultAtts = (DEFAULT_ATTRIBUTE *)
          ms->malloc_fcn(oldE->nDefaultAtts * sizeof(DEFAULT_ATTRIBUTE));
      if (!newE->defaultAtts) {
        ms->free_fcn(newE);
        return 0;
      }
    }
    if (oldE->idAtt)
      newE->idAtt = (ATTRIBUTE_ID *)
          lookup(&(newDtd->attributeIds), oldE->idAtt->name, 0);
    newE->allocDefaultAtts = newE->nDefaultAtts = oldE->nDefaultAtts;
    if (oldE->prefix)
      newE->prefix = (PREFIX *)lookup(&(newDtd->prefixes),
                                      oldE->prefix->name, 0);
    for (i = 0; i < newE->nDefaultAtts; i++) {
      newE->defaultAtts[i].id = (ATTRIBUTE_ID *)
          lookup(&(newDtd->attributeIds), oldE->defaultAtts[i].id->name, 0);
      newE->defaultAtts[i].isCdata = oldE->defaultAtts[i].isCdata;
      if (oldE->defaultAtts[i].value) {
        newE->defaultAtts[i].value
            = poolCopyString(&(newDtd->pool), oldE->defaultAtts[i].value);
        if (!newE->defaultAtts[i].value)
          return 0;
      }
      else
        newE->defaultAtts[i].value = NULL;
    }
  }

  /* Copy the entity tables. */
  if (!copyEntityTable(&(newDtd->generalEntities),
                       &(newDtd->pool),
                       &(oldDtd->generalEntities)))
      return 0;

#ifdef XML_DTD
  if (!copyEntityTable(&(newDtd->paramEntities),
                       &(newDtd->pool),
                       &(oldDtd->paramEntities)))
      return 0;
  newDtd->paramEntityRead = oldDtd->paramEntityRead;
#endif /* XML_DTD */

  newDtd->keepProcessing = oldDtd->keepProcessing;
  newDtd->hasParamEntityRefs = oldDtd->hasParamEntityRefs;
  newDtd->standalone = oldDtd->standalone;

  /* Don't want deep copying for scaffolding */
  newDtd->in_eldecl = oldDtd->in_eldecl;
  newDtd->scaffold = oldDtd->scaffold;
  newDtd->contentStringLen = oldDtd->contentStringLen;
  newDtd->scaffSize = oldDtd->scaffSize;
  newDtd->scaffLevel = oldDtd->scaffLevel;
  newDtd->scaffIndex = oldDtd->scaffIndex;

  return 1;
}  /* End dtdCopy */

static int
copyEntityTable(HASH_TABLE *newTable,
                STRING_POOL *newPool,
                const HASH_TABLE *oldTable)
{
  HASH_TABLE_ITER iter;
  const XML_Char *cachedOldBase = NULL;
  const XML_Char *cachedNewBase = NULL;

  hashTableIterInit(&iter, oldTable);

  for (;;) {
    ENTITY *newE;
    const XML_Char *name;
    const ENTITY *oldE = (ENTITY *)hashTableIterNext(&iter);
    if (!oldE)
      break;
    name = poolCopyString(newPool, oldE->name);
    if (!name)
      return 0;
    newE = (ENTITY *)lookup(newTable, name, sizeof(ENTITY));
    if (!newE)
      return 0;
    if (oldE->systemId) {
      const XML_Char *tem = poolCopyString(newPool, oldE->systemId);
      if (!tem)
        return 0;
      newE->systemId = tem;
      if (oldE->base) {
        if (oldE->base == cachedOldBase)
          newE->base = cachedNewBase;
        else {
          cachedOldBase = oldE->base;
          tem = poolCopyString(newPool, cachedOldBase);
          if (!tem)
            return 0;
          cachedNewBase = newE->base = tem;
        }
      }
      if (oldE->publicId) {
        tem = poolCopyString(newPool, oldE->publicId);
        if (!tem)
          return 0;
        newE->publicId = tem;
      }
    }
    else {
      const XML_Char *tem = poolCopyStringN(newPool, oldE->textPtr,
                                            oldE->textLen);
      if (!tem)
        return 0;
      newE->textPtr = tem;
      newE->textLen = oldE->textLen;
    }
    if (oldE->notation) {
      const XML_Char *tem = poolCopyString(newPool, oldE->notation);
      if (!tem)
        return 0;
      newE->notation = tem;
    }
    newE->is_param = oldE->is_param;
    newE->is_internal = oldE->is_internal;
  }
  return 1;
}

#define INIT_SIZE 64

static int FASTCALL
keyeq(KEY s1, KEY s2)
{
  for (; *s1 == *s2; s1++, s2++)
    if (*s1 == 0)
      return 1;
  return 0;
}

static unsigned long FASTCALL
hash(KEY s)
{
  unsigned long h = 0;
  while (*s)
    h = (h << 5) + h + (unsigned char)*s++;
  return h;
}

static NAMED *
lookup(HASH_TABLE *table, KEY name, size_t createSize)
{
  size_t i;
  if (table->size == 0) {
    size_t tsize;

    if (!createSize)
      return NULL;
    tsize = INIT_SIZE * sizeof(NAMED *);
    table->v = (NAMED **)table->mem->malloc_fcn(tsize);
    if (!table->v)
      return NULL;
    memset(table->v, 0, tsize);
    table->size = INIT_SIZE;
    table->usedLim = INIT_SIZE / 2;
    i = hash(name) & (table->size - 1);
  }
  else {
    unsigned long h = hash(name);
    for (i = h & (table->size - 1);
         table->v[i];
         i == 0 ? i = table->size - 1 : --i) {
      if (keyeq(name, table->v[i]->name))
        return table->v[i];
    }
    if (!createSize)
      return NULL;
    if (table->used == table->usedLim) {
      /* check for overflow */
      size_t newSize = table->size * 2;
      size_t tsize = newSize * sizeof(NAMED *);
      NAMED **newV = (NAMED **)table->mem->malloc_fcn(tsize);
      if (!newV)
        return NULL;
      memset(newV, 0, tsize);
      for (i = 0; i < table->size; i++)
        if (table->v[i]) {
          size_t j;
          for (j = hash(table->v[i]->name) & (newSize - 1);
               newV[j];
               j == 0 ? j = newSize - 1 : --j)
            ;
          newV[j] = table->v[i];
        }
      table->mem->free_fcn(table->v);
      table->v = newV;
      table->size = newSize;
      table->usedLim = newSize/2;
      for (i = h & (table->size - 1);
           table->v[i];
           i == 0 ? i = table->size - 1 : --i)
        ;
    }
  }
  table->v[i] = (NAMED *)table->mem->malloc_fcn(createSize);
  if (!table->v[i])
    return NULL;
  memset(table->v[i], 0, createSize);
  table->v[i]->name = name;
  (table->used)++;
  return table->v[i];
}

static void FASTCALL
hashTableClear(HASH_TABLE *table)
{
  size_t i;
  for (i = 0; i < table->size; i++) {
    NAMED *p = table->v[i];
    if (p) {
      table->mem->free_fcn(p);
      table->v[i] = NULL;
    }
  }
  table->usedLim = table->size / 2;
  table->used = 0;
}

static void FASTCALL
hashTableDestroy(HASH_TABLE *table)
{
  size_t i;
  for (i = 0; i < table->size; i++) {
    NAMED *p = table->v[i];
    if (p)
      table->mem->free_fcn(p);
  }
  if (table->v)
    table->mem->free_fcn(table->v);
}

static void FASTCALL
hashTableInit(HASH_TABLE *p, const XML_Memory_Handling_Suite *ms)
{
  p->size = 0;
  p->usedLim = 0;
  p->used = 0;
  p->v = NULL;
  p->mem = ms;
}

static void FASTCALL
hashTableIterInit(HASH_TABLE_ITER *iter, const HASH_TABLE *table)
{
  iter->p = table->v;
  iter->end = iter->p + table->size;
}

static NAMED * FASTCALL
hashTableIterNext(HASH_TABLE_ITER *iter)
{
  while (iter->p != iter->end) {
    NAMED *tem = *(iter->p)++;
    if (tem)
      return tem;
  }
  return NULL;
}

static void FASTCALL
poolInit(STRING_POOL *pool, const XML_Memory_Handling_Suite *ms)
{
  pool->blocks = NULL;
  pool->freeBlocks = NULL;
  pool->start = NULL;
  pool->ptr = NULL;
  pool->end = NULL;
  pool->mem = ms;
}

static void FASTCALL
poolClear(STRING_POOL *pool)
{
  if (!pool->freeBlocks)
    pool->freeBlocks = pool->blocks;
  else {
    BLOCK *p = pool->blocks;
    while (p) {
      BLOCK *tem = p->next;
      p->next = pool->freeBlocks;
      pool->freeBlocks = p;
      p = tem;
    }
  }
  pool->blocks = NULL;
  pool->start = NULL;
  pool->ptr = NULL;
  pool->end = NULL;
}

static void FASTCALL
poolDestroy(STRING_POOL *pool)
{
  BLOCK *p = pool->blocks;
  while (p) {
    BLOCK *tem = p->next;
    pool->mem->free_fcn(p);
    p = tem;
  }
  p = pool->freeBlocks;
  while (p) {
    BLOCK *tem = p->next;
    pool->mem->free_fcn(p);
    p = tem;
  }
}

static XML_Char *
poolAppend(STRING_POOL *pool, const ENCODING *enc,
           const char *ptr, const char *end)
{
  if (!pool->ptr && !poolGrow(pool))
    return NULL;
  for (;;) {
    XmlConvert(enc, &ptr, end, (ICHAR **)&(pool->ptr), (ICHAR *)pool->end);
    if (ptr == end)
      break;
    if (!poolGrow(pool))
      return NULL;
  }
  return pool->start;
}

static const XML_Char * FASTCALL
poolCopyString(STRING_POOL *pool, const XML_Char *s)
{
  do {
    if (!poolAppendChar(pool, *s))
      return NULL;
  } while (*s++);
  s = pool->start;
  poolFinish(pool);
  return s;
}

static const XML_Char *
poolCopyStringN(STRING_POOL *pool, const XML_Char *s, int n)
{
  if (!pool->ptr && !poolGrow(pool))
    return NULL;
  for (; n > 0; --n, s++) {
    if (!poolAppendChar(pool, *s))
      return NULL;
  }
  s = pool->start;
  poolFinish(pool);
  return s;
}

static const XML_Char * FASTCALL
poolAppendString(STRING_POOL *pool, const XML_Char *s)
{
  while (*s) {
    if (!poolAppendChar(pool, *s))
      return NULL;
    s++;
  }
  return pool->start;
}

static XML_Char *
poolStoreString(STRING_POOL *pool, const ENCODING *enc,
                const char *ptr, const char *end)
{
  if (!poolAppend(pool, enc, ptr, end))
    return NULL;
  if (pool->ptr == pool->end && !poolGrow(pool))
    return NULL;
  *(pool->ptr)++ = 0;
  return pool->start;
}

static XML_Bool FASTCALL
poolGrow(STRING_POOL *pool)
{
  if (pool->freeBlocks) {
    if (pool->start == 0) {
      pool->blocks = pool->freeBlocks;
      pool->freeBlocks = pool->freeBlocks->next;
      pool->blocks->next = NULL;
      pool->start = pool->blocks->s;
      pool->end = pool->start + pool->blocks->size;
      pool->ptr = pool->start;
      return XML_TRUE;
    }
    if (pool->end - pool->start < pool->freeBlocks->size) {
      BLOCK *tem = pool->freeBlocks->next;
      pool->freeBlocks->next = pool->blocks;
      pool->blocks = pool->freeBlocks;
      pool->freeBlocks = tem;
      memcpy(pool->blocks->s, pool->start,
             (pool->end - pool->start) * sizeof(XML_Char));
      pool->ptr = pool->blocks->s + (pool->ptr - pool->start);
      pool->start = pool->blocks->s;
      pool->end = pool->start + pool->blocks->size;
      return XML_TRUE;
    }
  }
  if (pool->blocks && pool->start == pool->blocks->s) {
    int blockSize = (pool->end - pool->start)*2;
    pool->blocks = (BLOCK *)
      pool->mem->realloc_fcn(pool->blocks,
                             (offsetof(BLOCK, s)
                             + blockSize * sizeof(XML_Char)));
    if (pool->blocks == NULL)
      return XML_FALSE;
    pool->blocks->size = blockSize;
    pool->ptr = pool->blocks->s + (pool->ptr - pool->start);
    pool->start = pool->blocks->s;
    pool->end = pool->start + blockSize;
  }
  else {
    BLOCK *tem;
    int blockSize = pool->end - pool->start;
    if (blockSize < INIT_BLOCK_SIZE)
      blockSize = INIT_BLOCK_SIZE;
    else
      blockSize *= 2;
    tem = (BLOCK *)pool->mem->malloc_fcn(offsetof(BLOCK, s)
                                         + blockSize * sizeof(XML_Char));
    if (!tem)
      return XML_FALSE;
    tem->size = blockSize;
    tem->next = pool->blocks;
    pool->blocks = tem;
    if (pool->ptr != pool->start)
      memcpy(tem->s, pool->start,
             (pool->ptr - pool->start) * sizeof(XML_Char));
    pool->ptr = tem->s + (pool->ptr - pool->start);
    pool->start = tem->s;
    pool->end = tem->s + blockSize;
  }
  return XML_TRUE;
}

static int FASTCALL
nextScaffoldPart(XML_Parser parser)
{
  DTD * const dtd = _dtd;  /* save one level of indirection */
  CONTENT_SCAFFOLD * me;
  int next;

  if (!dtd->scaffIndex) {
    dtd->scaffIndex = (int *)MALLOC(groupSize * sizeof(int));
    if (!dtd->scaffIndex)
      return -1;
    dtd->scaffIndex[0] = 0;
  }

  if (dtd->scaffCount >= dtd->scaffSize) {
    CONTENT_SCAFFOLD *temp;
    if (dtd->scaffold) {
      temp = (CONTENT_SCAFFOLD *)
        REALLOC(dtd->scaffold, dtd->scaffSize * 2 * sizeof(CONTENT_SCAFFOLD));
      if (temp == NULL)
        return -1;
      dtd->scaffSize *= 2;
    }
    else {
      temp = (CONTENT_SCAFFOLD *)MALLOC(INIT_SCAFFOLD_ELEMENTS
                                        * sizeof(CONTENT_SCAFFOLD));
      if (temp == NULL)
        return -1;
      dtd->scaffSize = INIT_SCAFFOLD_ELEMENTS;
    }
    dtd->scaffold = temp;
  }
  next = dtd->scaffCount++;
  me = &dtd->scaffold[next];
  if (dtd->scaffLevel) {
    CONTENT_SCAFFOLD *parent = &dtd->scaffold[dtd->scaffIndex[dtd->scaffLevel-1]];
    if (parent->lastchild) {
      dtd->scaffold[parent->lastchild].nextsib = next;
    }
    if (!parent->childcnt)
      parent->firstchild = next;
    parent->lastchild = next;
    parent->childcnt++;
  }
  me->firstchild = me->lastchild = me->childcnt = me->nextsib = 0;
  return next;
}

static void
build_node(XML_Parser parser,
           int src_node,
           XML_Content *dest,
           XML_Content **contpos,
           XML_Char **strpos)
{
  DTD * const dtd = _dtd;  /* save one level of indirection */
  dest->type = dtd->scaffold[src_node].type;
  dest->quant = dtd->scaffold[src_node].quant;
  if (dest->type == XML_CTYPE_NAME) {
    const XML_Char *src;
    dest->name = *strpos;
    src = dtd->scaffold[src_node].name;
    for (;;) {
      *(*strpos)++ = *src;
      if (!*src)
        break;
      src++;
    }
    dest->numchildren = 0;
    dest->children = NULL;
  }
  else {
    unsigned int i;
    int cn;
    dest->numchildren = dtd->scaffold[src_node].childcnt;
    dest->children = *contpos;
    *contpos += dest->numchildren;
    for (i = 0, cn = dtd->scaffold[src_node].firstchild;
         i < dest->numchildren;
         i++, cn = dtd->scaffold[cn].nextsib) {
      build_node(parser, cn, &(dest->children[i]), contpos, strpos);
    }
    dest->name = NULL;
  }
}

static XML_Content *
build_model (XML_Parser parser)
{
  DTD * const dtd = _dtd;  /* save one level of indirection */
  XML_Content *ret;
  XML_Content *cpos;
  XML_Char * str;
  int allocsize = (dtd->scaffCount * sizeof(XML_Content)
                   + (dtd->contentStringLen * sizeof(XML_Char)));

  ret = (XML_Content *)MALLOC(allocsize);
  if (!ret)
    return NULL;

  str =  (XML_Char *) (&ret[dtd->scaffCount]);
  cpos = &ret[1];

  build_node(parser, 0, ret, &cpos, &str);
  return ret;
}

static ELEMENT_TYPE *
getElementType(XML_Parser parser,
               const ENCODING *enc,
               const char *ptr,
               const char *end)
{
  DTD * const dtd = _dtd;  /* save one level of indirection */
  const XML_Char *name = poolStoreString(&dtd->pool, enc, ptr, end);
  ELEMENT_TYPE *ret;

  if (!name)
    return NULL;
  ret = (ELEMENT_TYPE *) lookup(&dtd->elementTypes, name, sizeof(ELEMENT_TYPE));
  if (!ret)
    return NULL;
  if (ret->name != name)
    poolDiscard(&dtd->pool);
  else {
    poolFinish(&dtd->pool);
    if (!setElementTypePrefix(parser, ret))
      return NULL;
  }
  return ret;
}
