/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

// Copyright 1998, Ben Goetter.  All rights reserved.

/*
 patch holes in winsock

  WCE 2.0 lacks many of the 'database' winsock routines.
  Stub just enough them for ss.dll.

  getprotobynumber
  getservbyport
  getservbyname

*/

struct servent * WINSOCKAPI getservbyport(int port, const char * proto) ;

struct servent * WINSOCKAPI getservbyname(const char * name,
                                          const char * proto) ;
struct protoent * WINSOCKAPI getprotobynumber(int proto) ;

