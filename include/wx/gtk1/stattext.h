/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk1/stattext.h
// Purpose:
// Author:      Robert Roebling
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __GTKSTATICTEXTH__
#define __GTKSTATICTEXTH__

//-----------------------------------------------------------------------------
// wxStaticText
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxStaticText : public wxStaticTextBase
{
public:
    wxStaticText();
    wxStaticText(wxWindow *parent,
                 wxWindowID id,
                 const wxString &label,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize,
                 long style = 0,
                 const wxString &name = wxStaticTextNameStr );

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString &label,
                const wxPoint &pos = wxDefaultPosition,
                const wxSize &size = wxDefaultSize,
                long style = 0,
                const wxString &name = wxStaticTextNameStr );

    virtual wxString GetLabel() const;
    virtual void SetLabel( const wxString &label );

    virtual bool SetFont( const wxFont &font );
    virtual bool SetForegroundColour( const wxColour& colour );

    static wxVisualAttributes
    GetClassDefaultAttributes(wxWindowVariant variant = wxWINDOW_VARIANT_NORMAL);

protected:
    virtual void DoSetSize(int x, int y,
                           int width, int height,
                           int sizeFlags = wxSIZE_AUTO);

    virtual wxSize DoGetBestSize() const;

    wxDECLARE_DYNAMIC_CLASS(wxStaticText);
};

#endif // __GTKSTATICTEXTH__
