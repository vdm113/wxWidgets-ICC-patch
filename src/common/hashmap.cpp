/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/hashmap.cpp
// Purpose:     wxHashMap implementation
// Author:      Mattia Barbon
// Modified by:
// Created:     29/01/2002
// Copyright:   (c) Mattia Barbon
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/hashmap.h"

/* FYI: This is the "One-at-a-Time" algorithm by Bob Jenkins */
/* from requirements by Colin Plumb. */
/* (http://burtleburtle.net/bob/hash/doobs.html) */
/* adapted from Perl sources ( hv.h ) */
template<typename T>
static unsigned long DoStringHash(T *k)
{
    unsigned long hash = 0;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   pragma prefetch
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
    while( *k )
    {
        hash += *k++;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);

    return hash + (hash << 15);
}

unsigned long wxStringHash::stringHash( const char* k )
  { return DoStringHash(k); }

unsigned long wxStringHash::stringHash( const wchar_t* k )
  { return DoStringHash(k); }


#ifdef wxNEEDS_WX_HASH_MAP

/* from SGI STL */
const unsigned long _wxHashTableBase2::ms_primes[prime_count] =
{
    7ul,          13ul,         29ul,
    53ul,         97ul,         193ul,       389ul,       769ul,
    1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
    49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
    1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
    50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
    1610612741ul, 3221225473ul, 4294967291ul
};

unsigned long _wxHashTableBase2::GetNextPrime( unsigned long n )
{
    const unsigned long* ptr = &ms_primes[0];
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   pragma prefetch
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
    for( size_t i = 0; i < prime_count; ++i, ++ptr )
    {
        if( n < *ptr )
            return *ptr;
    }

    /* someone might try to alloc a 2^32-element hash table */
    wxFAIL_MSG( wxT("hash table too big?") );

    /* quiet warning */
    return 0;
}

unsigned long _wxHashTableBase2::GetPreviousPrime( unsigned long n )
{
    const unsigned long* ptr = &ms_primes[prime_count - 1];

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   pragma prefetch
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
    for( size_t i = 0; i < prime_count; ++i, --ptr )
    {
        if( n > *ptr )
            return *ptr;
    }

    /* quiet warning */
    return 1;
}

void _wxHashTableBase2::DeleteNodes( size_t buckets,
                                     _wxHashTable_NodeBase** table,
                                     NodeDtor dtor )
{
    size_t i;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   pragma prefetch
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
    for( i = 0; i < buckets; ++i )
    {
        _wxHashTable_NodeBase* node = table[i];
        _wxHashTable_NodeBase* tmp;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   pragma prefetch
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
        while( node )
        {
            tmp = node->m_next;
            dtor( node );
            node = tmp;
        }
    }

    memset( table, 0, buckets * sizeof(void*) );
}

void _wxHashTableBase2::CopyHashTable( _wxHashTable_NodeBase** srcTable,
                                       size_t srcBuckets,
                                       _wxHashTableBase2* dst,
                                       _wxHashTable_NodeBase** dstTable,
                                       BucketFromNode func, ProcessNode proc )
{
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   pragma prefetch
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
    for( size_t i = 0; i < srcBuckets; ++i )
    {
        _wxHashTable_NodeBase* nextnode;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   pragma prefetch
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
        for( _wxHashTable_NodeBase* node = srcTable[i]; node; node = nextnode )
        {
            size_t bucket = func( dst, node );

            nextnode = node->m_next;
            _wxHashTable_NodeBase* newnode = proc( node );
            newnode->m_next = dstTable[bucket];
            dstTable[bucket] = newnode;
        }
    }
}

_wxHashTable_NodeBase* _wxHashTableBase2::DummyProcessNode(_wxHashTable_NodeBase* node)
{
    return node;
}

#endif // wxNEEDS_WX_HASH_MAP
