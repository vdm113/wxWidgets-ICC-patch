/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        tracker.h
// Purpose:     interface of wxTrackable
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxTrackable

    Add-on base class for a trackable object. This class maintains an internal
    linked list of classes of type wxTrackerNode and calls OnObjectDestroy() on
    them if this object is destroyed. The most common usage is by using the
    wxWeakRef<T> class template which automates this. This class has no public
    API. Its only use is by deriving another class from it to make it trackable.

    @code
    class MyClass: public Foo, public wxTrackable
    {
        // whatever
    }

    typedef wxWeakRef<MyClass> MyClassRef;
    @endcode

    @library{wxbase}
    @category{smartpointers}
*/
class wxTrackable
{
public:

};

