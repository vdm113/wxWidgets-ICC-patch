/////////////////////////////////////////////////////////////////////////////
// Name:        control.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling, Julian Smart and Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "control.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/defs.h"

#if wxUSE_CONTROLS

#include "wx/control.h"
#include "wx/fontutil.h"
#include "wx/settings.h"

#include <gtk/gtk.h>

//-----------------------------------------------------------------------------
// wxControl
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxControl, wxWindow)

wxControl::wxControl()
{
    m_needParent = TRUE;
    m_createComplete = false;
}

bool wxControl::Create( wxWindow *parent,
                      wxWindowID id,
                      const wxPoint &pos,
                      const wxSize &size,
                      long style,
                      const wxValidator& validator,
                      const wxString &name )
{
    m_createComplete = false;
    bool ret = wxWindow::Create(parent, id, pos, size, style, name);
    
#if wxUSE_VALIDATORS
    SetValidator(validator);
#endif

    return ret;
}

void wxControl::SetLabel( const wxString &label )
{
    m_label.Empty();
    for ( const wxChar *pc = label; *pc != wxT('\0'); pc++ )
    {
        if ( *pc == wxT('&') )
        {
            pc++; // skip it
#if 0 // it would be unused anyhow for now - kbd interface not done yet
            if ( *pc != wxT('&') ) m_chAccel = *pc;
#endif
        }
        m_label << *pc;
    }
}

void wxControl::PostSetLabel()
{
    // make sure the widget has been created, and that PostCreate has already
    // been called
    if (m_widget && m_createComplete && GetAdjustMinSizeFlag())
        SetBestSize(wxDefaultSize);
}

wxString wxControl::GetLabel() const
{
    return m_label;
}


wxSize wxControl::DoGetBestSize() const
{
    // Do not return any arbitrary default value...
    wxASSERT_MSG( m_widget, wxT("DoGetBestSize called before creation") );

    GtkRequisition req;
    req.width = 2;
    req.height = 2;
    (* GTK_WIDGET_CLASS( GTK_OBJECT_GET_CLASS(m_widget) )->size_request )
        (m_widget, &req );

    return wxSize(req.width, req.height);
}


void wxControl::PostCreation(const wxSize& size)
{
    wxWindow::PostCreation();

    // NB: GetBestSize needs to know the style, otherwise it will assume
    //     default font and if the user uses a different font, determined
    //     best size will be different (typically, smaller) than the desired
    //     size. This call ensure that a style is available at the time
    //     GetBestSize is called.
    gtk_widget_ensure_style(m_widget);
    
    InheritAttributes();
    ApplyWidgetStyle();
    SetInitialBestSize(size);
    m_createComplete = true;
}


#ifdef __WXGTK20__
wxString wxControl::PrepareLabelMnemonics( const wxString &label ) const
{
    //Format mnemonics properly for GTK2. This can be called from GTK1.x, but
    //it's not very useful because mnemonics don't exist prior to GTK2.
    wxString label2;
    for (size_t i = 0; i < label.Len(); i++)
    {
        if (label.GetChar(i) == wxT('&'))
        {
            //Mnemonic escape sequence "&&" is a literal "&" in the output.
            if (label.GetChar(i + 1) == wxT('&'))
            {
                label2 << wxT('&');
                i++;
            }
            //Handle special case of "&_" (i.e. "_" is the mnemonic).
            //FIXME - Is it possible to use "_" as a GTK mnemonic? Just use a
            //dash for now.
            else if (label.GetChar(i + 1) == wxT('_'))
            {
                label2 << wxT("_-");
                i++;
            }
            //Replace WX mnemonic indicator "&" with GTK indicator "_".
            else
            {
                label2 << wxT('_');
            }
        }
        else if (label.GetChar(i) == wxT('_'))
        {
            //Escape any underlines in the string so GTK doesn't use them.
            label2 << wxT("__");
        }
        else
        {
            label2 << label.GetChar(i);
        }
    }
    return label2;
}
#endif


wxVisualAttributes wxControl::GetDefaultAttributes() const
{
    return GetDefaultAttributesFromGTKWidget(m_widget,
                                             UseGTKStyleBase());
}


#define SHIFT (8*(sizeof(short int)-sizeof(char)))

// static
wxVisualAttributes
wxControl::GetDefaultAttributesFromGTKWidget(GtkWidget* widget,
                                             bool useBase,
                                             int state)
{
    GtkStyle* style;
    wxVisualAttributes attr;

    style = gtk_rc_get_style(widget);
    if (!style)
        style = gtk_widget_get_default_style();

    if (!style)
    {
        return wxWindow::GetClassDefaultAttributes(wxWINDOW_VARIANT_NORMAL);
    }

    if (state == -1)
        state = GTK_STATE_NORMAL;
        
    // get the style's colours
    attr.colFg = wxColour(style->fg[state].red   >> SHIFT,
                          style->fg[state].green >> SHIFT,
                          style->fg[state].blue  >> SHIFT);
    if (useBase)
        attr.colBg = wxColour(style->base[state].red   >> SHIFT,
                              style->base[state].green >> SHIFT,
                              style->base[state].blue  >> SHIFT);
    else
        attr.colBg = wxColour(style->bg[state].red   >> SHIFT,
                              style->bg[state].green >> SHIFT,
                              style->bg[state].blue  >> SHIFT);

    // get the style's font
#ifdef __WXGTK20__
    if ( !style->font_desc )
        style = gtk_widget_get_default_style();  
    if ( style && style->font_desc )
    {  
        wxNativeFontInfo info;  
        info.description = style->font_desc;  
        attr.font = wxFont(info);  
    }  
    else  
    {  
        GtkSettings *settings = gtk_settings_get_default();
        gchar *font_name = NULL;
        g_object_get ( settings,
                       "gtk-font-name", 
                       &font_name,
                       NULL);
        if (!font_name)
            attr.font = wxSystemSettings::GetFont( wxSYS_DEFAULT_GUI_FONT );
        else
            attr.font = wxFont(wxString::FromAscii(font_name));
        g_free (font_name);
    }  
#else
    // TODO: isn't there a way to get a standard gtk 1.2 font?
    attr.font = wxFont( 12, wxSWISS, wxNORMAL, wxNORMAL );
#endif
    
    return attr;
}


//static
wxVisualAttributes
wxControl::GetDefaultAttributesFromGTKWidget(GtkWidget* (*widget_new)(void),
                                             bool useBase,
                                             int state)
{
    wxVisualAttributes attr;
    GtkWidget* widget = widget_new();
    attr = GetDefaultAttributesFromGTKWidget(widget, useBase, state);
    gtk_widget_destroy(widget);
    return attr;
}

//static
wxVisualAttributes
wxControl::GetDefaultAttributesFromGTKWidget(GtkWidget* (*widget_new)(const gchar*),
                                             bool useBase,
                                             int state)
{
    wxVisualAttributes attr;
    GtkWidget* widget = widget_new("");
    attr = GetDefaultAttributesFromGTKWidget(widget, useBase, state);
    gtk_widget_destroy(widget);
    return attr;
}


//static
wxVisualAttributes
wxControl::GetDefaultAttributesFromGTKWidget(GtkWidget* (*widget_new)(GtkAdjustment*),
                                             bool useBase,
                                             int state)
{
    wxVisualAttributes attr;
    GtkWidget* widget = widget_new(NULL);
    attr = GetDefaultAttributesFromGTKWidget(widget, useBase, state);
    gtk_widget_destroy(widget);
    return attr;
}

void wxControl::ApplyWidgetStyle(bool forceStyle)
{
    GtkRcStyle *style = CreateWidgetStyle(forceStyle);
    if ( style )
    {
        DoApplyWidgetStyle(style);
        gtk_rc_style_unref(style);
    }
}

void wxControl::DoApplyWidgetStyle(GtkRcStyle *style)
{
    gtk_widget_modify_style(m_widget, style);
}


#endif // wxUSE_CONTROLS

