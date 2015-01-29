/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

#ifndef INLINE4_EXPAT_H
#define INLINE4_EXPAT_H

/*
** This file was auto generated by idltool 51.6.
**
** It provides compatibility to OS3 style library
** calls by substituting functions.
**
** Do not edit manually.
*/ 

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef EXEC_EXEC_H
#include <exec/exec.h>
#endif
#ifndef EXEC_INTERFACES_H
#include <exec/interfaces.h>
#endif

#ifndef LIBRARIES_EXPAT_H
#include <libraries/expat.h>
#endif

/* Inline macros for Interface "main" */
#define XML_ParserCreate(encodingName) IExpat->XML_ParserCreate(encodingName) 
#define XML_ParserCreateNS(encodingName, nsSep) IExpat->XML_ParserCreateNS(encodingName, nsSep) 
#define XML_ParserCreate_MM(encoding, memsuite, namespaceSeparator) IExpat->XML_ParserCreate_MM(encoding, memsuite, namespaceSeparator) 
#define XML_ExternalEntityParserCreate(parser, context, encoding) IExpat->XML_ExternalEntityParserCreate(parser, context, encoding) 
#define XML_ParserFree(parser) IExpat->XML_ParserFree(parser) 
#define XML_Parse(parser, s, len, isFinal) IExpat->XML_Parse(parser, s, len, isFinal) 
#define XML_ParseBuffer(parser, len, isFinal) IExpat->XML_ParseBuffer(parser, len, isFinal) 
#define XML_GetBuffer(parser, len) IExpat->XML_GetBuffer(parser, len) 
#define XML_SetStartElementHandler(parser, start) IExpat->XML_SetStartElementHandler(parser, start) 
#define XML_SetEndElementHandler(parser, end) IExpat->XML_SetEndElementHandler(parser, end) 
#define XML_SetElementHandler(parser, start, end) IExpat->XML_SetElementHandler(parser, start, end) 
#define XML_SetCharacterDataHandler(parser, handler) IExpat->XML_SetCharacterDataHandler(parser, handler) 
#define XML_SetProcessingInstructionHandler(parser, handler) IExpat->XML_SetProcessingInstructionHandler(parser, handler) 
#define XML_SetCommentHandler(parser, handler) IExpat->XML_SetCommentHandler(parser, handler) 
#define XML_SetStartCdataSectionHandler(parser, start) IExpat->XML_SetStartCdataSectionHandler(parser, start) 
#define XML_SetEndCdataSectionHandler(parser, end) IExpat->XML_SetEndCdataSectionHandler(parser, end) 
#define XML_SetCdataSectionHandler(parser, start, end) IExpat->XML_SetCdataSectionHandler(parser, start, end) 
#define XML_SetDefaultHandler(parser, handler) IExpat->XML_SetDefaultHandler(parser, handler) 
#define XML_SetDefaultHandlerExpand(parser, handler) IExpat->XML_SetDefaultHandlerExpand(parser, handler) 
#define XML_SetExternalEntityRefHandler(parser, handler) IExpat->XML_SetExternalEntityRefHandler(parser, handler) 
#define XML_SetExternalEntityRefHandlerArg(parser, arg) IExpat->XML_SetExternalEntityRefHandlerArg(parser, arg) 
#define XML_SetUnknownEncodingHandler(parser, handler, data) IExpat->XML_SetUnknownEncodingHandler(parser, handler, data) 
#define XML_SetStartNamespaceDeclHandler(parser, start) IExpat->XML_SetStartNamespaceDeclHandler(parser, start) 
#define XML_SetEndNamespaceDeclHandler(parser, end) IExpat->XML_SetEndNamespaceDeclHandler(parser, end) 
#define XML_SetNamespaceDeclHandler(parser, start, end) IExpat->XML_SetNamespaceDeclHandler(parser, start, end) 
#define XML_SetXmlDeclHandler(parser, handler) IExpat->XML_SetXmlDeclHandler(parser, handler) 
#define XML_SetStartDoctypeDeclHandler(parser, start) IExpat->XML_SetStartDoctypeDeclHandler(parser, start) 
#define XML_SetEndDoctypeDeclHandler(parser, end) IExpat->XML_SetEndDoctypeDeclHandler(parser, end) 
#define XML_SetDoctypeDeclHandler(parser, start, end) IExpat->XML_SetDoctypeDeclHandler(parser, start, end) 
#define XML_SetElementDeclHandler(parser, eldecl) IExpat->XML_SetElementDeclHandler(parser, eldecl) 
#define XML_SetAttlistDeclHandler(parser, attdecl) IExpat->XML_SetAttlistDeclHandler(parser, attdecl) 
#define XML_SetEntityDeclHandler(parser, handler) IExpat->XML_SetEntityDeclHandler(parser, handler) 
#define XML_SetUnparsedEntityDeclHandler(parser, handler) IExpat->XML_SetUnparsedEntityDeclHandler(parser, handler) 
#define XML_SetNotationDeclHandler(parser, handler) IExpat->XML_SetNotationDeclHandler(parser, handler) 
#define XML_SetNotStandaloneHandler(parser, handler) IExpat->XML_SetNotStandaloneHandler(parser, handler) 
#define XML_GetErrorCode(parser) IExpat->XML_GetErrorCode(parser) 
#define XML_ErrorString(code) IExpat->XML_ErrorString(code) 
#define XML_GetCurrentByteIndex(parser) IExpat->XML_GetCurrentByteIndex(parser) 
#define XML_GetCurrentLineNumber(parser) IExpat->XML_GetCurrentLineNumber(parser) 
#define XML_GetCurrentColumnNumber(parser) IExpat->XML_GetCurrentColumnNumber(parser) 
#define XML_GetCurrentByteCount(parser) IExpat->XML_GetCurrentByteCount(parser) 
#define XML_GetInputContext(parser, offset, size) IExpat->XML_GetInputContext(parser, offset, size) 
#define XML_SetUserData(parser, userData) IExpat->XML_SetUserData(parser, userData) 
#define XML_DefaultCurrent(parser) IExpat->XML_DefaultCurrent(parser) 
#define XML_UseParserAsHandlerArg(parser) IExpat->XML_UseParserAsHandlerArg(parser) 
#define XML_SetBase(parser, base) IExpat->XML_SetBase(parser, base) 
#define XML_GetBase(parser) IExpat->XML_GetBase(parser) 
#define XML_GetSpecifiedAttributeCount(parser) IExpat->XML_GetSpecifiedAttributeCount(parser) 
#define XML_GetIdAttributeIndex(parser) IExpat->XML_GetIdAttributeIndex(parser) 
#define XML_SetEncoding(parser, encoding) IExpat->XML_SetEncoding(parser, encoding) 
#define XML_SetParamEntityParsing(parser, parsing) IExpat->XML_SetParamEntityParsing(parser, parsing) 
#define XML_SetReturnNSTriplet(parser, do_nst) IExpat->XML_SetReturnNSTriplet(parser, do_nst) 
#define XML_ExpatVersion() IExpat->XML_ExpatVersion() 
#define XML_ExpatVersionInfo() IExpat->XML_ExpatVersionInfo() 
#define XML_ParserReset(parser, encoding) IExpat->XML_ParserReset(parser, encoding) 
#define XML_SetSkippedEntityHandler(parser, handler) IExpat->XML_SetSkippedEntityHandler(parser, handler) 
#define XML_UseForeignDTD(parser, useDTD) IExpat->XML_UseForeignDTD(parser, useDTD) 
#define XML_GetFeatureList() IExpat->XML_GetFeatureList() 
#define XML_StopParser(parser, resumable) IExpat->XML_StopParser(parser, resumable) 
#define XML_ResumeParser(parser) IExpat->XML_ResumeParser(parser) 
#define XML_GetParsingStatus(parser, status) IExpat->XML_GetParsingStatus(parser, status) 
#define XML_FreeContentModel(parser, model) IExpat->XML_FreeContentModel(parser, model) 
#define XML_MemMalloc(parser, size) IExpat->XML_MemMalloc(parser, size) 
#define XML_MemRealloc(parser, ptr, size) IExpat->XML_MemRealloc(parser, ptr, size) 
#define XML_MemFree(parser, ptr) IExpat->XML_MemFree(parser, ptr) 

#endif /* INLINE4_EXPAT_H */
