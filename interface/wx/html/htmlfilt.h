/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        html/htmlfilt.h
// Purpose:     interface of wxHtmlFilter
// Author:      wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxHtmlFilter

    This class is the parent class of input filters for wxHtmlWindow.
    It allows you to read and display files of different file formats.

    @library{wxhtml}
    @category{html}

    @see @ref overview_html_filters
*/
class wxHtmlFilter : public wxObject
{
public:
    /**
        Constructor.
    */
    wxHtmlFilter();

    /**
        Returns @true if this filter is capable of reading file @e file.
        Example:
        @code
        bool MyFilter::CanRead(const wxFSFile& file)
        {
            return (file.GetMimeType() == "application/x-ugh");
        }
        @endcode
    */
    virtual bool CanRead(const wxFSFile& file) const = 0;

    /**
        Reads the file and returns string with HTML document.
        Example:
        @code
        wxString MyImgFilter::ReadFile(const wxFSFile& file)
        {
            return "<html><body><img src=\"" + file.GetLocation() +
                   "\"></body></html>";
        }
        @endcode
    */
    virtual wxString ReadFile(const wxFSFile& file) const = 0;
};

