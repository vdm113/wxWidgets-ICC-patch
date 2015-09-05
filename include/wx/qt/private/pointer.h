/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/private/pointer.h
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_POINTER_H_
#define _WX_QT_POINTER_H_

#include <QtCore/QPointer>

// Extend QPointer with the ability to delete the object in its destructor. The
// normal behaviour of the QPointer makes sure that this is safe, because if somebody
// has deleted the object, then data() returns NULL and delete does nothing.

template < typename T >
class wxQtPointer : public QPointer< T >
{
public:
    inline wxQtPointer()
        : QPointer< T >()
    {
    }

    inline wxQtPointer( T *p )
        : QPointer< T >( p )
    {
    }

    inline wxQtPointer< T > &operator = ( T *p )
    {
        QPointer< T >::operator = ( p );
        return *this;
    }

    inline ~wxQtPointer()
    {
        delete QPointer< T >::data();
    }
};

#endif // _WX_QT_POINTER_H_
