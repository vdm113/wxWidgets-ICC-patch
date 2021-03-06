/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        src/osx/core/stdpaths_cf.cpp
// Purpose:     wxStandardPaths implementation for CoreFoundation systems
// Author:      David Elliott
// Modified by:
// Created:     2004-10-27
// Copyright:   (c) 2004 David Elliott <dfe@cox.net>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/wxprec.h"

#if wxUSE_STDPATHS

#ifndef WX_PRECOMP
    #include "wx/intl.h"
#endif //ndef WX_PRECOMP

#include "wx/stdpaths.h"
#include "wx/filename.h"
#ifdef __WXMAC__
#include "wx/osx/private.h"
#endif
#include "wx/osx/core/cfstring.h"

#include <CoreFoundation/CFBundle.h>
#include <CoreFoundation/CFURL.h>

#define kDefaultPathStyle kCFURLPOSIXPathStyle

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxStandardPathsCF ctors/dtor
// ----------------------------------------------------------------------------

wxStandardPathsCF::wxStandardPathsCF()
                 : m_bundle(CFBundleGetMainBundle())
{
    CFRetain(m_bundle);
}

wxStandardPathsCF::wxStandardPathsCF(wxCFBundleRef bundle)
                 : m_bundle(bundle)
{
    CFRetain(m_bundle);
}

wxStandardPathsCF::~wxStandardPathsCF()
{
    CFRelease(m_bundle);
}

// ----------------------------------------------------------------------------
// wxStandardPathsCF Mac-specific methods
// ----------------------------------------------------------------------------

void wxStandardPathsCF::SetBundle(wxCFBundleRef bundle)
{
    CFRetain(bundle);
    CFRelease(m_bundle);
    m_bundle = bundle;
}

// ----------------------------------------------------------------------------
// generic functions in terms of which the other ones are implemented
// ----------------------------------------------------------------------------

static wxString BundleRelativeURLToPath(CFURLRef relativeURL)
{
    CFURLRef absoluteURL = CFURLCopyAbsoluteURL(relativeURL);
    wxCHECK_MSG(absoluteURL, wxEmptyString, wxT("Failed to resolve relative URL to absolute URL"));
    wxCFStringRef cfStrPath( CFURLCopyFileSystemPath(absoluteURL,kDefaultPathStyle) );
    CFRelease(absoluteURL);
    return wxCFStringRef::AsStringWithNormalizationFormC(cfStrPath);
}

wxString wxStandardPathsCF::GetFromFunc(wxCFURLRef (*func)(wxCFBundleRef)) const
{
    wxCHECK_MSG(m_bundle, wxEmptyString,
                wxT("wxStandardPaths for CoreFoundation only works with bundled apps"));
    CFURLRef relativeURL = (*func)(m_bundle);
    wxCHECK_MSG(relativeURL, wxEmptyString, wxT("Couldn't get URL"));
    wxString ret(BundleRelativeURLToPath(relativeURL));
    CFRelease(relativeURL);
    return ret;
}

wxString wxStandardPathsCF::GetUserDir(Dir userDir) const
{
#if defined( __WXMAC__ ) && wxOSX_USE_CARBON
    OSType folderType;
    switch (userDir)
    {
        case Dir_Desktop:
            folderType = kDesktopFolderType;
            break;
        case Dir_Downloads:
            folderType = kDownloadsFolderType;
            break;
        case Dir_Music:
            folderType = kMusicDocumentsFolderType;
            break;
        case Dir_Pictures:
            folderType = kPictureDocumentsFolderType;
            break;
        case Dir_Videos:
            folderType = kMovieDocumentsFolderType;
            break;
        default:
            folderType = kDocumentsFolderType;
            break;
    }

    return wxMacFindFolderNoSeparator
        (
        kUserDomain,
        folderType,
        kCreateFolder
        );
#else
    wxString userDirName;
    switch (userDir)
    {
        case Dir_Desktop:
            userDirName = "Desktop";
            break;
        case Dir_Downloads:
            userDirName = "Downloads";
            break;
        case Dir_Music:
            userDirName = "Music";
            break;
        case Dir_Pictures:
            userDirName = "Pictures";
            break;
        case Dir_Videos:
            userDirName = "Movies";
            break;
        default:
            userDirName = "Documents";
            break;
    }
    return wxFileName::GetHomeDir() + "/" + userDirName;
#endif
}

// ----------------------------------------------------------------------------
// wxStandardPathsCF public API
// ----------------------------------------------------------------------------

wxString wxStandardPathsCF::GetConfigDir() const
{
#if defined( __WXMAC__ ) && wxOSX_USE_CARBON
    return wxMacFindFolderNoSeparator((short)kLocalDomain, kPreferencesFolderType, kCreateFolder);
#else
    return wxT("/Library/Preferences");
#endif
}

wxString wxStandardPathsCF::GetUserConfigDir() const
{
#if defined( __WXMAC__ ) && wxOSX_USE_CARBON
    return wxMacFindFolderNoSeparator((short)kUserDomain, kPreferencesFolderType, kCreateFolder);
#else
    return wxFileName::GetHomeDir() + wxT("/Library/Preferences");
#endif
}

wxString wxStandardPathsCF::GetDataDir() const
{
    return GetFromFunc(CFBundleCopySharedSupportURL);
}

wxString wxStandardPathsCF::GetExecutablePath() const
{
#ifdef __WXMAC__
    return GetFromFunc(CFBundleCopyExecutableURL);
#else
    return wxStandardPathsBase::GetExecutablePath();
#endif
}

wxString wxStandardPathsCF::GetLocalDataDir() const
{
#if defined( __WXMAC__ ) && wxOSX_USE_CARBON
    return AppendAppInfo(wxMacFindFolderNoSeparator((short)kLocalDomain, kApplicationSupportFolderType, kCreateFolder));
#else
    return AppendAppInfo(wxT("/Library/Application Support"));
#endif
}

wxString wxStandardPathsCF::GetUserDataDir() const
{
#if defined( __WXMAC__ ) && wxOSX_USE_CARBON
    return AppendAppInfo(wxMacFindFolderNoSeparator((short)kUserDomain, kApplicationSupportFolderType, kCreateFolder));
#else
    return AppendAppInfo(wxFileName::GetHomeDir() + wxT("/Library/Application Support"));
#endif
}

wxString wxStandardPathsCF::GetPluginsDir() const
{
    return GetFromFunc(CFBundleCopyBuiltInPlugInsURL);
}

wxString wxStandardPathsCF::GetResourcesDir() const
{
    return GetFromFunc(CFBundleCopyResourcesDirectoryURL);
}

wxString
wxStandardPathsCF::GetLocalizedResourcesDir(const wxString& lang,
                                            ResourceCat category) const
{
    return wxStandardPathsBase::
            GetLocalizedResourcesDir(lang, category) + wxT(".lproj");
}

#endif // wxUSE_STDPATHS
