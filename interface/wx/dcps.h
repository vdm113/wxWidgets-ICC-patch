/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        dcps.h
// Purpose:     interface of wxPostScriptDC
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxPostScriptDC

    This defines the wxWidgets Encapsulated PostScript device context, which
    can write PostScript files on any platform. See wxDC for descriptions of
    the member functions.

    @library{wxbase}
    @category{dc}
*/
class wxPostScriptDC : public wxDC
{
public:
    wxPostScriptDC();

    /**
        Constructs a PostScript printer device context from a wxPrintData object.
    */
    wxPostScriptDC(const wxPrintData& printData);

};

