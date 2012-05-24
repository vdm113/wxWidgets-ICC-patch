/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        fontutil.h
// Purpose:     interface of wxNativeFontInfo
// Author:      wxWidgets team
// RCS-ID:      $Id: $
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


/**
    @class wxNativeFontInfo
   
    wxNativeFontInfo is platform-specific font representation: this class
    should be considered as an opaque font description only used by the native
    functions, the user code can only get the objects of this type from
    somewhere and pass it somewhere else (possibly save them somewhere using
    ToString() and restore them using FromString())

    @library{wxcore}
    @category{gdi}
*/
class wxNativeFontInfo
{
public:
    wxNativeFontInfo();
    wxNativeFontInfo(const wxNativeFontInfo& info);
    ~wxNativeFontInfo();

    wxNativeFontInfo& operator=(const wxNativeFontInfo& info);

    void Init();
    void InitFromFont(const wxFont& font);
    
    int GetPointSize() const;
    wxSize GetPixelSize() const;
    wxFontStyle GetStyle() const;
    wxFontWeight GetWeight() const;
    bool GetUnderlined() const;
    wxString GetFaceName() const;
    wxFontFamily GetFamily() const;
    wxFontEncoding GetEncoding() const;

    void SetPointSize(int pointsize);
    void SetPixelSize(const wxSize& pixelSize);
    void SetStyle(wxFontStyle style);
    void SetWeight(wxFontWeight weight);
    void SetUnderlined(bool underlined);
    bool SetFaceName(const wxString& facename);
    void SetFamily(wxFontFamily family);
    void SetEncoding(wxFontEncoding encoding);

    void SetFaceName(const wxArrayString &facenames);

    bool FromString(const wxString& s);
    wxString ToString() const;

    bool FromUserString(const wxString& s);
    wxString ToUserString() const;
};




