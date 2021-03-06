/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/uxtheme.h
// Purpose:     wxUxThemeEngine class: support for XP themes
// Author:      John Platts, Vadim Zeitlin
// Modified by:
// Created:     2003
// Copyright:   (c) 2003 John Platts, Vadim Zeitlin
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UXTHEME_H_
#define _WX_UXTHEME_H_

#include "wx/defs.h"

#include "wx/msw/private.h"     // we use GetHwndOf()
#include "wx/msw/uxthemep.h"

// Amazingly, GetThemeFont() and GetThemeSysFont() functions use LOGFONTA under
// XP but LOGFONTW (even in non-Unicode build) under later versions of Windows.
// If we declare them as taking LOGFONT below, the code would be able to
// silently pass LOGFONTA to them in ANSI build and would crash at run-time
// under Windows Vista/7 because of a buffer overrun (LOGFONTA being smaller
// than LOGFONTW expected by these functions). If we we declare them as taking
// LOGFONTW, the code wouldn't work correctly under XP. So we use a special
// wxUxThemeFont class to encapsulate this and intentionally change the LOGFONT
// output parameters of the theme functions to take it instead.

class wxUxThemeFont
{
public:
    // Trivial default ctor.
    wxUxThemeFont() { }

    // Just some unique type.
    struct Ptr { };

#if wxUSE_UNICODE
    // In Unicode build we always use LOGFONT anyhow so this class is
    // completely trivial.
    Ptr *GetPtr() { return reinterpret_cast<Ptr *>(&m_lfW); }
    const LOGFONTW& GetLOGFONT() { return m_lfW; }
#else // !wxUSE_UNICODE
    // Return either LOGFONTA or LOGFONTW pointer as required by the current
    // Windows version.
    Ptr *GetPtr()
    {
        return UseLOGFONTW() ? reinterpret_cast<Ptr *>(&m_lfW)
                             : reinterpret_cast<Ptr *>(&m_lfA);
    }

    // This method returns LOGFONT (i.e. LOGFONTA in ANSI build and LOGFONTW in
    // Unicode one) which can be used with other, normal, Windows or wx
    // functions. Internally it may need to transform LOGFONTW to LOGFONTA.
    const LOGFONTA& GetLOGFONT()
    {
        if ( UseLOGFONTW() )
        {
            // Most of the fields are the same in LOGFONTA and LOGFONTW so just
            // copy everything by default.
            memcpy(&m_lfA, &m_lfW, sizeof(m_lfA));

            // But the face name must be converted from Unicode.
            WideCharToMultiByte(CP_ACP, 0, m_lfW.lfFaceName, -1,
                                m_lfA.lfFaceName, sizeof(m_lfA.lfFaceName),
                                NULL, NULL);
        }

        return m_lfA;
    }

private:
    static bool UseLOGFONTW()
    {
        return wxGetWinVersion() >= wxWinVersion_Vista;
    }

    LOGFONTA m_lfA;
#endif // wxUSE_UNICODE/!wxUSE_UNICODE

private:
    LOGFONTW m_lfW;

    wxDECLARE_NO_COPY_CLASS(wxUxThemeFont);
};

typedef int(__stdcall *DTT_CALLBACK_PROC)(HDC hdc, const wchar_t * pszText, int cchText, RECT * prc, unsigned int dwFlags, WXLPARAM lParam);

typedef struct _DTTOPTS
{
    DWORD             dwSize;
    DWORD             dwFlags;
    COLORREF          crText;
    COLORREF          crBorder;
    COLORREF          crShadow;
    int               iTextShadowType;
    POINT             ptShadowOffset;
    int               iBorderSize;
    int               iFontPropId;
    int               iColorPropId;
    int               iStateId;
    BOOL              fApplyOverlay;
    int               iGlowSize;
    DTT_CALLBACK_PROC pfnDrawTextCallback;
    WXLPARAM          lParam;
} DTTOPTS, *PDTTOPTS;

typedef HTHEME  (__stdcall *PFNWXUOPENTHEMEDATA)(HWND, const wchar_t *);
typedef HRESULT (__stdcall *PFNWXUCLOSETHEMEDATA)(HTHEME);
typedef HRESULT (__stdcall *PFNWXUDRAWTHEMEBACKGROUND)(HTHEME, HDC, int, int, const RECT *, const RECT *);
typedef HRESULT (__stdcall *PFNWXUDRAWTHEMETEXT)(HTHEME, HDC, int, int, const wchar_t *, int, DWORD, DWORD, const RECT *);
typedef HRESULT (__stdcall *PFNWXUDRAWTHEMETEXTEX)(HTHEME, HDC, int, int, const wchar_t *, int, DWORD, RECT *, const DTTOPTS *);
typedef HRESULT (__stdcall *PFNWXUGETTHEMEBACKGROUNDCONTENTRECT)(HTHEME, HDC, int, int, const RECT *, RECT *);
typedef HRESULT (__stdcall *PFNWXUGETTHEMEBACKGROUNDEXTENT)(HTHEME, HDC, int, int, const RECT *, RECT *);
typedef HRESULT (__stdcall *PFNWXUGETTHEMEPARTSIZE)(HTHEME, HDC, int, int, const RECT *, /* enum */ THEMESIZE, SIZE *);
typedef HRESULT (__stdcall *PFNWXUGETTHEMETEXTEXTENT)(HTHEME, HDC, int, int, const wchar_t *, int, DWORD, const RECT *, RECT *);
typedef HRESULT (__stdcall *PFNWXUGETTHEMETEXTMETRICS)(HTHEME, HDC, int, int, TEXTMETRIC*);
typedef HRESULT (__stdcall *PFNWXUGETTHEMEBACKGROUNDREGION)(HTHEME, HDC, int, int, const RECT *, HRGN *);
typedef HRESULT (__stdcall *PFNWXUHITTESTTHEMEBACKGROUND)(HTHEME, HDC, int, int, DWORD, const RECT *, HRGN, POINT, unsigned short *);
typedef HRESULT (__stdcall *PFNWXUDRAWTHEMEEDGE)(HTHEME, HDC, int, int, const RECT *, unsigned int, unsigned int, RECT *);
typedef HRESULT (__stdcall *PFNWXUDRAWTHEMEICON)(HTHEME, HDC, int, int, const RECT *, HIMAGELIST, int);
typedef BOOL    (__stdcall *PFNWXUISTHEMEPARTDEFINED)(HTHEME, int, int);
typedef BOOL    (__stdcall *PFNWXUISTHEMEBACKGROUNDPARTIALLYTRANSPARENT)(HTHEME, int, int);
typedef HRESULT (__stdcall *PFNWXUGETTHEMECOLOR)(HTHEME, int, int, int, COLORREF*);
typedef HRESULT (__stdcall *PFNWXUGETTHEMEMETRIC)(HTHEME, HDC, int, int, int, int *);
typedef HRESULT (__stdcall *PFNWXUGETTHEMESTRING)(HTHEME, int, int, int, wchar_t *, int);
typedef HRESULT (__stdcall *PFNWXUGETTHEMEBOOL)(HTHEME, int, int, int, BOOL *);
typedef HRESULT (__stdcall *PFNWXUGETTHEMEINT)(HTHEME, int, int, int, int *);
typedef HRESULT (__stdcall *PFNWXUGETTHEMEENUMVALUE)(HTHEME, int, int, int, int *);
typedef HRESULT (__stdcall *PFNWXUGETTHEMEPOSITION)(HTHEME, int, int, int, POINT *);
typedef HRESULT (__stdcall *PFNWXUGETTHEMEFONT)(HTHEME, HDC, int, int, int, wxUxThemeFont::Ptr *);
typedef HRESULT (__stdcall *PFNWXUGETTHEMERECT)(HTHEME, int, int, int, RECT *);
typedef HRESULT (__stdcall *PFNWXUGETTHEMEMARGINS)(HTHEME, HDC, int, int, int, RECT *, MARGINS *);
typedef HRESULT (__stdcall *PFNWXUGETTHEMEINTLIST)(HTHEME, int, int, int, INTLIST*);
typedef HRESULT (__stdcall *PFNWXUGETTHEMEPROPERTYORIGIN)(HTHEME, int, int, int, /* enum */ PROPERTYORIGIN *);
typedef HRESULT (__stdcall *PFNWXUSETWINDOWTHEME)(HWND, const wchar_t*, const wchar_t *);
typedef HRESULT (__stdcall *PFNWXUGETTHEMEFILENAME)(HTHEME, int, int, int, wchar_t *, int);
typedef COLORREF(__stdcall *PFNWXUGETTHEMESYSCOLOR)(HTHEME, int);
typedef HBRUSH  (__stdcall *PFNWXUGETTHEMESYSCOLORBRUSH)(HTHEME, int);
typedef BOOL    (__stdcall *PFNWXUGETTHEMESYSBOOL)(HTHEME, int);
typedef int     (__stdcall *PFNWXUGETTHEMESYSSIZE)(HTHEME, int);
typedef HRESULT (__stdcall *PFNWXUGETTHEMESYSFONT)(HTHEME, int, wxUxThemeFont::Ptr *);
typedef HRESULT (__stdcall *PFNWXUGETTHEMESYSSTRING)(HTHEME, int, wchar_t *, int);
typedef HRESULT (__stdcall *PFNWXUGETTHEMESYSINT)(HTHEME, int, int *);
typedef BOOL    (__stdcall *PFNWXUISTHEMEACTIVE)();
typedef BOOL    (__stdcall *PFNWXUISAPPTHEMED)();
typedef HTHEME  (__stdcall *PFNWXUGETWINDOWTHEME)(HWND);
typedef HRESULT (__stdcall *PFNWXUENABLETHEMEDIALOGTEXTURE)(HWND, DWORD);
typedef BOOL    (__stdcall *PFNWXUISTHEMEDIALOGTEXTUREENABLED)(HWND);
typedef DWORD   (__stdcall *PFNWXUGETTHEMEAPPPROPERTIES)();
typedef void    (__stdcall *PFNWXUSETTHEMEAPPPROPERTIES)(DWORD);
typedef HRESULT (__stdcall *PFNWXUGETCURRENTTHEMENAME)(wchar_t *, int, wchar_t *, int, wchar_t *, int);
typedef HRESULT (__stdcall *PFNWXUGETTHEMEDOCUMENTATIONPROPERTY)(const wchar_t *, const wchar_t *, wchar_t *, int);
typedef HRESULT (__stdcall *PFNWXUDRAWTHEMEPARENTBACKGROUND)(HWND, HDC, RECT *);
typedef HRESULT (__stdcall *PFNWXUENABLETHEMING)(BOOL);

// ----------------------------------------------------------------------------
// wxUxThemeEngine: provides all theme functions from uxtheme.dll
// ----------------------------------------------------------------------------

// we always define this class, even if wxUSE_UXTHEME == 0, but we just make it
// empty in this case -- this allows to use it elsewhere without any #ifdefs
#if wxUSE_UXTHEME
    #include "wx/dynlib.h"

    #define wxUX_THEME_DECLARE(type, func) type func;
#else
    #define wxUX_THEME_DECLARE(type, func) type func(...) { return 0; }
#endif

class WXDLLIMPEXP_CORE wxUxThemeEngine
{
public:
    // get the theme engine or NULL if themes are not available
    static wxUxThemeEngine *Get();

    // get the theme enging or NULL if themes are not available or not used for
    // this application
    static wxUxThemeEngine *GetIfActive();

    // all uxtheme.dll functions
    wxUX_THEME_DECLARE(PFNWXUOPENTHEMEDATA, OpenThemeData)
    wxUX_THEME_DECLARE(PFNWXUCLOSETHEMEDATA, CloseThemeData)
    wxUX_THEME_DECLARE(PFNWXUDRAWTHEMEBACKGROUND, DrawThemeBackground)
    wxUX_THEME_DECLARE(PFNWXUDRAWTHEMETEXT, DrawThemeText)
    wxUX_THEME_DECLARE(PFNWXUDRAWTHEMETEXTEX, DrawThemeTextEx)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMEBACKGROUNDCONTENTRECT, GetThemeBackgroundContentRect)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMEBACKGROUNDEXTENT, GetThemeBackgroundExtent)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMEPARTSIZE, GetThemePartSize)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMETEXTEXTENT, GetThemeTextExtent)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMETEXTMETRICS, GetThemeTextMetrics)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMEBACKGROUNDREGION, GetThemeBackgroundRegion)
    wxUX_THEME_DECLARE(PFNWXUHITTESTTHEMEBACKGROUND, HitTestThemeBackground)
    wxUX_THEME_DECLARE(PFNWXUDRAWTHEMEEDGE, DrawThemeEdge)
    wxUX_THEME_DECLARE(PFNWXUDRAWTHEMEICON, DrawThemeIcon)
    wxUX_THEME_DECLARE(PFNWXUISTHEMEPARTDEFINED, IsThemePartDefined)
    wxUX_THEME_DECLARE(PFNWXUISTHEMEBACKGROUNDPARTIALLYTRANSPARENT, IsThemeBackgroundPartiallyTransparent)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMECOLOR, GetThemeColor)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMEMETRIC, GetThemeMetric)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMESTRING, GetThemeString)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMEBOOL, GetThemeBool)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMEINT, GetThemeInt)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMEENUMVALUE, GetThemeEnumValue)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMEPOSITION, GetThemePosition)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMEFONT, GetThemeFont)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMERECT, GetThemeRect)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMEMARGINS, GetThemeMargins)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMEINTLIST, GetThemeIntList)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMEPROPERTYORIGIN, GetThemePropertyOrigin)
    wxUX_THEME_DECLARE(PFNWXUSETWINDOWTHEME, SetWindowTheme)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMEFILENAME, GetThemeFilename)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMESYSCOLOR, GetThemeSysColor)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMESYSCOLORBRUSH, GetThemeSysColorBrush)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMESYSBOOL, GetThemeSysBool)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMESYSSIZE, GetThemeSysSize)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMESYSFONT, GetThemeSysFont)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMESYSSTRING, GetThemeSysString)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMESYSINT, GetThemeSysInt)
    wxUX_THEME_DECLARE(PFNWXUISTHEMEACTIVE, IsThemeActive)
    wxUX_THEME_DECLARE(PFNWXUISAPPTHEMED, IsAppThemed)
    wxUX_THEME_DECLARE(PFNWXUGETWINDOWTHEME, GetWindowTheme)
    wxUX_THEME_DECLARE(PFNWXUENABLETHEMEDIALOGTEXTURE, EnableThemeDialogTexture)
    wxUX_THEME_DECLARE(PFNWXUISTHEMEDIALOGTEXTUREENABLED, IsThemeDialogTextureEnabled)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMEAPPPROPERTIES, GetThemeAppProperties)
    wxUX_THEME_DECLARE(PFNWXUSETTHEMEAPPPROPERTIES, SetThemeAppProperties)
    wxUX_THEME_DECLARE(PFNWXUGETCURRENTTHEMENAME, GetCurrentThemeName)
    wxUX_THEME_DECLARE(PFNWXUGETTHEMEDOCUMENTATIONPROPERTY, GetThemeDocumentationProperty)
    wxUX_THEME_DECLARE(PFNWXUDRAWTHEMEPARENTBACKGROUND, DrawThemeParentBackground)
    wxUX_THEME_DECLARE(PFNWXUENABLETHEMING, EnableTheming)

private:
    // construcor is private as only Get() can create us and is also trivial as
    // everything really happens in Initialize()
    wxUxThemeEngine() { }

    // destructor is private as only Get() and wxUxThemeModule delete us, it is
    // not virtual as we're not supposed to be derived from
    ~wxUxThemeEngine() { }

#if wxUSE_UXTHEME
    // initialize the theme engine: load the DLL, resolve the functions
    //
    // return true if we can be used, false if themes are not available
    bool Initialize();


    // uxtheme.dll
    wxDynamicLibrary m_dllUxTheme;


    // the one and only theme engine, initially NULL
    static wxUxThemeEngine *ms_themeEngine;

    // this is a bool which initially has the value -1 meaning "unknown"
    static int ms_isThemeEngineAvailable;

    // it must be able to delete us
    friend class wxUxThemeModule;
#endif // wxUSE_UXTHEME

    wxDECLARE_NO_COPY_CLASS(wxUxThemeEngine);
};

#if wxUSE_UXTHEME

/* static */ inline wxUxThemeEngine *wxUxThemeEngine::GetIfActive()
{
    wxUxThemeEngine *engine = Get();
    return engine && engine->IsAppThemed() && engine->IsThemeActive()
                ? engine
                : NULL;
}

#else // !wxUSE_UXTHEME

/* static */ inline wxUxThemeEngine *wxUxThemeEngine::Get()
{
    return NULL;
}

/* static */ inline wxUxThemeEngine *wxUxThemeEngine::GetIfActive()
{
    return NULL;
}

#endif // wxUSE_UXTHEME/!wxUSE_UXTHEME

// ----------------------------------------------------------------------------
// wxUxThemeHandle: encapsulates ::Open/CloseThemeData()
// ----------------------------------------------------------------------------

class wxUxThemeHandle
{
public:
    wxUxThemeHandle(const wxWindow *win, const wchar_t *classes)
    {
        wxUxThemeEngine *engine = wxUxThemeEngine::Get();

        m_hTheme = engine ? (HTHEME)engine->OpenThemeData(GetHwndOf(win), classes)
                          : NULL;
    }

    operator HTHEME() const { return m_hTheme; }

    ~wxUxThemeHandle()
    {
        if ( m_hTheme )
        {
            wxUxThemeEngine::Get()->CloseThemeData(m_hTheme);
        }
    }

private:
    HTHEME m_hTheme;

    wxDECLARE_NO_COPY_CLASS(wxUxThemeHandle);
};

#endif // _WX_UXTHEME_H_

