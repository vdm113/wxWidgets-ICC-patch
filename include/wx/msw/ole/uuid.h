/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/ole/uuid.h
// Purpose:     encapsulates an UUID with some added helper functions
// Author:      Vadim Zeitlin
// Modified by:
// Created:     11.07.97
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows licence
//
// Notes:       you should link your project with RPCRT4.LIB!
///////////////////////////////////////////////////////////////////////////////

#ifndef   _WX_OLEUUID_H
#define   _WX_OLEUUID_H

#include "wx/chartype.h"
// ------------------------------------------------------------------
// UUID (Universally Unique IDentifier) definition
// ------------------------------------------------------------------

// ----- taken from RPC.H
#ifndef UUID_DEFINED            // in some cases RPC.H will be already
    typedef struct
    {
      unsigned long   Data1;
      unsigned short  Data2;
      unsigned short  Data3;
      unsigned char   Data4[8];
    } UUID;                     // UUID = GUID = CLSID = LIBID = IID
#endif  // UUID_DEFINED

#ifndef GUID_DEFINED
  typedef UUID GUID;
  #define UUID_DEFINED          // prevent redefinition
#endif  // GUID_DEFINED

typedef unsigned char uchar;

// ------------------------------------------------------------------
// a class to store UUID and it's string representation
// ------------------------------------------------------------------

// uses RPC functions to create/convert Universally Unique Identifiers
class WXDLLIMPEXP_CORE Uuid
{
private:
  UUID  m_uuid;
  wxUChar *m_pszUuid;   // this string is alloc'd and freed by RPC
  wxChar  *m_pszCForm;  // this string is allocated in Set/Create

  void  UuidToCForm();

  // function used to set initial state by all ctors
  void  Init() { m_pszUuid = NULL; m_pszCForm = NULL; }

public:
  // ctors & dtor
  Uuid()                 { Init();            }
  Uuid(const wxChar *pc) { Init(); Set(pc);   }
  Uuid(const UUID &uuid) { Init(); Set(uuid); }
 ~Uuid();

  // copy ctor and assignment operator needed for this class
  Uuid(const Uuid& uuid);
  Uuid& operator=(const Uuid& uuid);

  // create a brand new UUID
  void Create();

  // set value of UUID
  bool Set(const wxChar *pc); // from a string, returns true if ok
  void Set(const UUID& uuid); // from another UUID (never fails)

  // comparison operators
  bool operator==(const Uuid& uuid) const;
  bool operator!=(const Uuid& uuid) const { return !(*this == uuid); }

  // accessors
  operator const UUID*()   const { return &m_uuid;               }
  operator const wxChar*() const { return (wxChar *)(m_pszUuid); }

  // return string representation of the UUID in the C form
  // (as in DEFINE_GUID macro)
  const wxChar *CForm() const    { return m_pszCForm;            }
};

#endif //_WX_OLEUUID_H
