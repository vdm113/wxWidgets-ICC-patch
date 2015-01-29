/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        src/common/rendcmn.cpp
// Purpose:     wxRendererNative common functions
// Author:      Vadim Zeitlin
// Modified by:
// Created:     28.07.03
// Copyright:   (c) 2003 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/log.h"
    #include "wx/intl.h"
#endif //WX_PRECOMP

#include "wx/apptrait.h"
#include "wx/renderer.h"

#include "wx/scopedptr.h"

#if wxUSE_DYNLIB_CLASS
    #include "wx/dynlib.h"
#endif // wxUSE_DYNLIB_CLASS

// ----------------------------------------------------------------------------
// wxRendererPtr: auto pointer holding the global renderer
// ----------------------------------------------------------------------------

typedef wxScopedPtr<wxRendererNative> wxRendererPtrBase;

class wxRendererPtr : public wxRendererPtrBase
{
public:
    // return true if we have a renderer, false otherwise
    bool IsOk()
    {
        if ( !m_initialized )
        {
            // only try to create the renderer once
            m_initialized = true;

            DoInit();
        }

        return get() != NULL;
    }

    // return the global and unique wxRendererPtr
    static wxRendererPtr& Get();

private:
    wxRendererPtr() : wxRendererPtrBase(NULL) { m_initialized = false; }

    void DoInit()
    {
        wxAppTraits *traits = wxTheApp ? wxTheApp->GetTraits() : NULL;
        if ( traits )
        {
            // ask the traits object to create a renderer for us
            reset(traits->CreateRenderer());
        }
    }

    bool m_initialized;

    // just to suppress a gcc warning
    friend class wxRendererPtrDummyFriend;

    wxDECLARE_NO_COPY_CLASS(wxRendererPtr);
};

// return the global and unique wxRendererPtr
/*static*/ wxRendererPtr& wxRendererPtr::Get()
{
    static wxRendererPtr s_renderer;

    return s_renderer;
}

#if wxUSE_DYNLIB_CLASS

// ----------------------------------------------------------------------------
// wxRendererFromDynLib: represents a renderer dynamically loaded from a DLL
// ----------------------------------------------------------------------------

class wxRendererFromDynLib : public wxDelegateRendererNative
{
public:
    // create the object wrapping the given renderer created from this DLL
    //
    // we take ownership of the pointer and will delete it (and also unload the
    // DLL) when we're deleted
    wxRendererFromDynLib(wxDynamicLibrary& dll, wxRendererNative *renderer)
        : wxDelegateRendererNative(*renderer),
          m_renderer(renderer),
          m_dllHandle(dll.Detach())
        {
        }

    virtual ~wxRendererFromDynLib()
    {
        delete m_renderer;
        wxDynamicLibrary::Unload(m_dllHandle);
    }

private:
    wxRendererNative *m_renderer;
    wxDllType m_dllHandle;
};

#endif // wxUSE_DYNLIB_CLASS

// ============================================================================
// wxRendererNative implementation
// ============================================================================

wxRendererNative::~wxRendererNative()
{
    // empty but necessary
}

// ----------------------------------------------------------------------------
// Managing the global renderer
// ----------------------------------------------------------------------------

/* static */
wxRendererNative& wxRendererNative::Get()
{
    wxRendererPtr& renderer = wxRendererPtr::Get();

    return renderer.IsOk() ? *renderer.get() : GetDefault();
}

/* static */
wxRendererNative *wxRendererNative::Set(wxRendererNative *rendererNew)
{
    wxRendererPtr& renderer = wxRendererPtr::Get();

    wxRendererNative *rendererOld = renderer.release();

    renderer.reset(rendererNew);

    return rendererOld;
}


// ----------------------------------------------------------------------------
// Dynamic renderers loading
// ----------------------------------------------------------------------------

#if wxUSE_DYNLIB_CLASS

/* static */
wxRendererNative *wxRendererNative::Load(const wxString& name)
{
    wxString fullname = wxDynamicLibrary::CanonicalizePluginName(name);

    wxDynamicLibrary dll(fullname);
    if ( !dll.IsLoaded() )
        return NULL;

    // each theme DLL must export a wxCreateRenderer() function with this
    // signature
    typedef wxRendererNative *(*wxCreateRenderer_t)();

    wxDYNLIB_FUNCTION(wxCreateRenderer_t, wxCreateRenderer, dll);
    if ( !pfnwxCreateRenderer )
        return NULL;

    // create a renderer object
    wxRendererNative *renderer = (*pfnwxCreateRenderer)();
    if ( !renderer )
        return NULL;

    // check that its version is compatible with ours
    wxRendererVersion ver = renderer->GetVersion();
    if ( !wxRendererVersion::IsCompatible(ver) )
    {
        wxLogError(_("Renderer \"%s\" has incompatible version %d.%d and couldn't be loaded."),
                   name.c_str(), ver.version, ver.age);
        delete renderer;

        return NULL;
    }

    // finally wrap the renderer in an object which will delete it and unload
    // the library when it is deleted and return it to the caller
    return new wxRendererFromDynLib(dll, renderer);
}

#endif // wxUSE_DYNLIB_CLASS
