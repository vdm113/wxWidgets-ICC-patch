/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        regconf.h
// Purpose:     interface of wxRegConfig
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxRegConfig

    wxRegConfig implements the wxConfigBase interface for
    storing and retrieving configuration information using Windows registry.

    This class is used by default for wxConfig on Windows platforms; see wxFileConfig
    for an alternative you may want to use (also on Windows).

    @library{wxbase}
    @category{cfg}

    @see wxConfigBase
*/
class wxRegConfig : public wxConfigBase
{
public:
    /**
        The wxRegConfig constructor. For more info see the docs for the
        wxConfigBase::wxConfigBase() constructor.
        
        Note that wxRegConfig's @a style argument defaults to @c wxCONFIG_USE_GLOBAL_FILE,
        i.e. to the use of the @c HKLM key (also known as "HKEY_LOCAL_MACHINE").
    */
    wxRegConfig(const wxString& appName = wxEmptyString,
                const wxString& vendorName = wxEmptyString,
                const wxString& localFilename = wxEmptyString,
                const wxString& globalFilename = wxEmptyString,
                long style = wxCONFIG_USE_GLOBAL_FILE);
};

