/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk1/statbox.h
// Purpose:
// Author:      Robert Roebling
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __GTKSTATICBOXH__
#define __GTKSTATICBOXH__

//-----------------------------------------------------------------------------
// wxStaticBox
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxStaticBox : public wxStaticBoxBase
{
public:
    wxStaticBox();
    wxStaticBox( wxWindow *parent,
                 wxWindowID id,
                 const wxString &label,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize,
                 long style = 0,
                 const wxString &name = wxStaticBoxNameStr );
    bool Create( wxWindow *parent,
                 wxWindowID id,
                 const wxString &label,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize,
                 long style = 0,
                 const wxString &name = wxStaticBoxNameStr );

    virtual void SetLabel( const wxString &label );

    static wxVisualAttributes
    GetClassDefaultAttributes(wxWindowVariant variant = wxWINDOW_VARIANT_NORMAL);

    // implementation

    virtual bool IsTransparentForMouse() const { return TRUE; }

protected:
    void DoApplyWidgetStyle(GtkRcStyle *style);

private:
    wxDECLARE_DYNAMIC_CLASS(wxStaticBox);
};

#endif // __GTKSTATICBOXH__
