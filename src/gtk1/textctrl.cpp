/////////////////////////////////////////////////////////////////////////////
// Name:        textctrl.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling, Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "textctrl.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/textctrl.h"
#include "wx/utils.h"
#include "wx/intl.h"
#include "wx/log.h"
#include "wx/settings.h"
#include "wx/panel.h"
#include "wx/strconv.h"
#include "wx/fontutil.h"        // for wxNativeFontInfo (GetNativeFontInfo())

#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include "wx/math.h"

#include "wx/gtk/private.h"
#include <gdk/gdkkeysyms.h>

//-----------------------------------------------------------------------------
// idle system
//-----------------------------------------------------------------------------

extern void wxapp_install_idle_handler();
extern bool g_isIdle;

//-----------------------------------------------------------------------------
// data
//-----------------------------------------------------------------------------

extern bool       g_blockEventsOnDrag;
extern wxCursor   g_globalCursor;
extern wxWindowGTK *g_delayedFocus;

// ----------------------------------------------------------------------------
// helpers
// ----------------------------------------------------------------------------

#ifdef __WXGTK20__
static void wxGtkTextApplyTagsFromAttr(GtkTextBuffer *text_buffer,
                                       const wxTextAttr& attr,
                                       GtkTextIter *start,
                                       GtkTextIter *end)
{
    static gchar buf[1024];
    GtkTextTag *tag;

    if (attr.HasFont())
    {
        char *font_string;
        PangoFontDescription *font_description = attr.GetFont().GetNativeFontInfo()->description;
        font_string = pango_font_description_to_string(font_description);
        g_snprintf(buf, sizeof(buf), "WXFONT %s", font_string);
        tag = gtk_text_tag_table_lookup( gtk_text_buffer_get_tag_table( text_buffer ),
                                         buf );
        if (!tag)
            tag = gtk_text_buffer_create_tag( text_buffer, buf,
                                              "font-desc", font_description,
                                              NULL );
        gtk_text_buffer_apply_tag (text_buffer, tag, start, end);
        g_free (font_string);
    }

    if (attr.HasTextColour())
    {
        GdkColor *colFg = attr.GetTextColour().GetColor();
        g_snprintf(buf, sizeof(buf), "WXFORECOLOR %d %d %d",
                   colFg->red, colFg->green, colFg->blue);
        tag = gtk_text_tag_table_lookup( gtk_text_buffer_get_tag_table( text_buffer ),
                                         buf );
        if (!tag)
            tag = gtk_text_buffer_create_tag( text_buffer, buf,
                                              "foreground-gdk", colFg, NULL );
        gtk_text_buffer_apply_tag (text_buffer, tag, start, end);
    }

    if (attr.HasBackgroundColour())
    {
        GdkColor *colBg = attr.GetBackgroundColour().GetColor();
        g_snprintf(buf, sizeof(buf), "WXBACKCOLOR %d %d %d",
                   colBg->red, colBg->green, colBg->blue);
        tag = gtk_text_tag_table_lookup( gtk_text_buffer_get_tag_table( text_buffer ),
                                         buf );
        if (!tag)
            tag = gtk_text_buffer_create_tag( text_buffer, buf,
                                              "background-gdk", colBg, NULL );
        gtk_text_buffer_apply_tag (text_buffer, tag, start, end);
    }
}

static void wxGtkTextInsert(GtkWidget *text,
                            GtkTextBuffer *text_buffer,
                            const wxTextAttr& attr,
                            wxCharBuffer buffer)

{
    gint start_offset;
    GtkTextIter iter, start;

    gtk_text_buffer_get_iter_at_mark( text_buffer, &iter,
                                     gtk_text_buffer_get_insert (text_buffer) );
    start_offset = gtk_text_iter_get_offset (&iter);
    gtk_text_buffer_insert( text_buffer, &iter, buffer, strlen(buffer) );

    gtk_text_buffer_get_iter_at_offset (text_buffer, &start, start_offset);

    wxGtkTextApplyTagsFromAttr(text_buffer, attr, &start, &iter);
}
#else
static void wxGtkTextInsert(GtkWidget *text,
                            const wxTextAttr& attr,
                            const char *txt,
                            size_t len)
{
    GdkFont *font = attr.HasFont() ? attr.GetFont().GetInternalFont()
                                   : NULL;

    GdkColor *colFg = attr.HasTextColour() ? attr.GetTextColour().GetColor()
                                           : NULL;

    GdkColor *colBg = attr.HasBackgroundColour()
                        ? attr.GetBackgroundColour().GetColor()
                        : NULL;

    gtk_text_insert( GTK_TEXT(text), font, colFg, colBg, txt, len );
}
#endif // GTK 1.x

// ----------------------------------------------------------------------------
// "insert_text" for GtkEntry
// ----------------------------------------------------------------------------

static void
gtk_insert_text_callback(GtkEditable *editable,
                         const gchar *new_text,
                         gint new_text_length,
                         gint *position,
                         wxTextCtrl *win)
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    // we should only be called if we have a max len limit at all
    GtkEntry *entry = GTK_ENTRY (editable);

    wxCHECK_RET( entry->text_max_length, _T("shouldn't be called") );

    // check that we don't overflow the max length limit
    //
    // FIXME: this doesn't work when we paste a string which is going to be
    //        truncated
    if ( entry->text_length == entry->text_max_length )
    {
        // we don't need to run the base class version at all
        gtk_signal_emit_stop_by_name(GTK_OBJECT(editable), "insert_text");

        // remember that the next changed signal is to be ignored to avoid
        // generating a dummy wxEVT_COMMAND_TEXT_UPDATED event
        win->IgnoreNextTextUpdate();

        // and generate the correct one ourselves
        wxCommandEvent event(wxEVT_COMMAND_TEXT_MAXLEN, win->GetId());
        event.SetEventObject(win);
        event.SetString(win->GetValue());
        win->GetEventHandler()->ProcessEvent( event );
    }
}

//-----------------------------------------------------------------------------
//  "changed"
//-----------------------------------------------------------------------------

static void
gtk_text_changed_callback( GtkWidget *widget, wxTextCtrl *win )
{
    if ( win->IgnoreTextUpdate() )
        return;

    if (!win->m_hasVMT) return;

    if (g_isIdle)
        wxapp_install_idle_handler();

    win->SetModified();
#ifndef __WXGTK20__
    win->UpdateFontIfNeeded();
#endif // !__WXGTK20__

    wxCommandEvent event( wxEVT_COMMAND_TEXT_UPDATED, win->GetId() );
    event.SetEventObject( win );
    event.SetString( win->GetValue() );
    win->GetEventHandler()->ProcessEvent( event );
}

//-----------------------------------------------------------------------------
// "expose_event" from scrolled window and textview
//-----------------------------------------------------------------------------

#ifdef __WXGTK20__
static gboolean
gtk_text_exposed_callback( GtkWidget *widget, GdkEventExpose *event, wxTextCtrl *win )
{
    return TRUE;
}
#endif

//-----------------------------------------------------------------------------
// "changed" from vertical scrollbar
//-----------------------------------------------------------------------------

#ifndef __WXGTK20__
static void
gtk_scrollbar_changed_callback( GtkWidget *WXUNUSED(widget), wxTextCtrl *win )
{
    if (!win->m_hasVMT) return;

    if (g_isIdle)
        wxapp_install_idle_handler();

    win->CalculateScrollbar();
}
#endif

// ----------------------------------------------------------------------------
// redraw callback for multiline text
// ----------------------------------------------------------------------------

#ifndef __WXGTK20__

// redrawing a GtkText from inside a wxYield() call results in crashes (the
// text sample shows it in its "Add lines" command which shows wxProgressDialog
// which implicitly calls wxYield()) so we override GtkText::draw() and simply
// don't do anything if we're inside wxYield()

extern bool wxIsInsideYield;

extern "C" {
    typedef void (*GtkDrawCallback)(GtkWidget *widget, GdkRectangle *rect);
}

static GtkDrawCallback gs_gtk_text_draw = NULL;

extern "C"
void wxgtk_text_draw( GtkWidget *widget, GdkRectangle *rect)
{
    if ( !wxIsInsideYield )
    {
        wxCHECK_RET( gs_gtk_text_draw != wxgtk_text_draw,
                     _T("infinite recursion in wxgtk_text_draw aborted") );

        gs_gtk_text_draw(widget, rect);
    }
}

#endif // __WXGTK20__

//-----------------------------------------------------------------------------
//  wxTextCtrl
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxTextCtrl,wxControl)

BEGIN_EVENT_TABLE(wxTextCtrl, wxControl)
    EVT_CHAR(wxTextCtrl::OnChar)

    EVT_MENU(wxID_CUT, wxTextCtrl::OnCut)
    EVT_MENU(wxID_COPY, wxTextCtrl::OnCopy)
    EVT_MENU(wxID_PASTE, wxTextCtrl::OnPaste)
    EVT_MENU(wxID_UNDO, wxTextCtrl::OnUndo)
    EVT_MENU(wxID_REDO, wxTextCtrl::OnRedo)

    EVT_UPDATE_UI(wxID_CUT, wxTextCtrl::OnUpdateCut)
    EVT_UPDATE_UI(wxID_COPY, wxTextCtrl::OnUpdateCopy)
    EVT_UPDATE_UI(wxID_PASTE, wxTextCtrl::OnUpdatePaste)
    EVT_UPDATE_UI(wxID_UNDO, wxTextCtrl::OnUpdateUndo)
    EVT_UPDATE_UI(wxID_REDO, wxTextCtrl::OnUpdateRedo)
END_EVENT_TABLE()

void wxTextCtrl::Init()
{
    m_ignoreNextUpdate =
    m_modified = FALSE;
    SetUpdateFont(FALSE);
    m_text =
    m_vScrollbar = (GtkWidget *)NULL;
#ifdef __WXGTK20__
    m_frozenness = 0;
#endif 
}

wxTextCtrl::wxTextCtrl( wxWindow *parent,
                        wxWindowID id,
                        const wxString &value,
                        const wxPoint &pos,
                        const wxSize &size,
                        long style,
                        const wxValidator& validator,
                        const wxString &name )
{
    Init();

    Create( parent, id, value, pos, size, style, validator, name );
}

bool wxTextCtrl::Create( wxWindow *parent,
                         wxWindowID id,
                         const wxString &value,
                         const wxPoint &pos,
                         const wxSize &size,
                         long style,
                         const wxValidator& validator,
                         const wxString &name )
{
    m_needParent = TRUE;
    m_acceptsFocus = TRUE;

    if (!PreCreation( parent, pos, size ) ||
        !CreateBase( parent, id, pos, size, style, validator, name ))
    {
        wxFAIL_MSG( wxT("wxTextCtrl creation failed") );
        return FALSE;
    }


    m_vScrollbarVisible = FALSE;

    bool multi_line = (style & wxTE_MULTILINE) != 0;

    if (multi_line)
    {
#ifdef __WXGTK20__
        // Create view
        m_text = gtk_text_view_new();

        m_buffer = gtk_text_view_get_buffer( GTK_TEXT_VIEW(m_text) );

        // create scrolled window
        m_widget = gtk_scrolled_window_new( NULL, NULL );
        gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW( m_widget ),
                                        GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );

        // Insert view into scrolled window
        gtk_container_add( GTK_CONTAINER(m_widget), m_text );

        // Global settings which can be overridden by tags, I guess.
        if (HasFlag( wxHSCROLL ) || HasFlag( wxTE_DONTWRAP ))
            gtk_text_view_set_wrap_mode( GTK_TEXT_VIEW( m_text ), GTK_WRAP_NONE );
        else
            gtk_text_view_set_wrap_mode( GTK_TEXT_VIEW( m_text ), GTK_WRAP_WORD );

        if (!HasFlag(wxNO_BORDER))
            gtk_scrolled_window_set_shadow_type( GTK_SCROLLED_WINDOW(m_widget), GTK_SHADOW_IN );
            
        GTK_WIDGET_UNSET_FLAGS( m_widget, GTK_CAN_FOCUS );
#else
        // create our control ...
        m_text = gtk_text_new( (GtkAdjustment *) NULL, (GtkAdjustment *) NULL );

        // ... and put into the upper left hand corner of the table
        bool bHasHScrollbar = FALSE;
        m_widget = gtk_table_new(bHasHScrollbar ? 2 : 1, 2, FALSE);
        GTK_WIDGET_UNSET_FLAGS( m_widget, GTK_CAN_FOCUS );
        gtk_table_attach( GTK_TABLE(m_widget), m_text, 0, 1, 0, 1,
                      (GtkAttachOptions)(GTK_FILL | GTK_EXPAND | GTK_SHRINK),
                      (GtkAttachOptions)(GTK_FILL | GTK_EXPAND | GTK_SHRINK),
                       0, 0);

        // always wrap words
        gtk_text_set_word_wrap( GTK_TEXT(m_text), TRUE );

        // finally, put the vertical scrollbar in the upper right corner
        m_vScrollbar = gtk_vscrollbar_new( GTK_TEXT(m_text)->vadj );
        GTK_WIDGET_UNSET_FLAGS( m_vScrollbar, GTK_CAN_FOCUS );
        gtk_table_attach(GTK_TABLE(m_widget), m_vScrollbar, 1, 2, 0, 1,
                     GTK_FILL,
                     (GtkAttachOptions)(GTK_EXPAND | GTK_FILL | GTK_SHRINK),
                     0, 0);
#endif
    }
    else
    {
        // a single-line text control: no need for scrollbars
        m_widget =
        m_text = gtk_entry_new();
        
        if (style & wxNO_BORDER)
            g_object_set( GTK_ENTRY(m_text), "has-frame", FALSE, NULL );
    }

    m_parent->DoAddChild( this );

    m_focusWidget = m_text;

    PostCreation(size);

    if (multi_line)
        gtk_widget_show(m_text);

#ifndef __WXGTK20__
    if (multi_line)
    {
        gtk_signal_connect(GTK_OBJECT(GTK_TEXT(m_text)->vadj), "changed",
          (GtkSignalFunc) gtk_scrollbar_changed_callback, (gpointer) this );

        // only initialize gs_gtk_text_draw once, starting from the next the
        // klass::draw will already be wxgtk_text_draw
        if ( !gs_gtk_text_draw )
        {
            GtkDrawCallback&
                draw = GTK_WIDGET_CLASS(GTK_OBJECT(m_text)->klass)->draw;

            gs_gtk_text_draw = draw;

            draw = wxgtk_text_draw;
        }
    }
#endif // GTK+ 1.x

    if (!value.IsEmpty())
    {
#ifdef __WXGTK20__
        SetValue( value );
#else

#if !GTK_CHECK_VERSION(1, 2, 0)
        // if we don't realize it, GTK 1.0.6 dies with a SIGSEGV in
        // gtk_editable_insert_text()
        gtk_widget_realize(m_text);
#endif // GTK 1.0

        gint tmp = 0;
#if wxUSE_UNICODE
        wxWX2MBbuf val = value.mbc_str();
        gtk_editable_insert_text( GTK_EDITABLE(m_text), val, strlen(val), &tmp );
#else
        gtk_editable_insert_text( GTK_EDITABLE(m_text), value, value.Length(), &tmp );
#endif

        if (multi_line)
        {
            // Bring editable's cursor uptodate. Bug in GTK.
            SET_EDITABLE_POS(m_text, gtk_text_get_point( GTK_TEXT(m_text) ));
        }

#endif
    }

    if (style & wxTE_PASSWORD)
    {
        if (!multi_line)
            gtk_entry_set_visibility( GTK_ENTRY(m_text), FALSE );
    }

    if (style & wxTE_READONLY)
    {
        if (!multi_line)
            gtk_entry_set_editable( GTK_ENTRY(m_text), FALSE );
#ifdef __WXGTK20__
        else
            gtk_text_view_set_editable( GTK_TEXT_VIEW( m_text), FALSE);
#else
    }
    else
    {
        if (multi_line)
            gtk_text_set_editable( GTK_TEXT(m_text), 1 );
#endif
    }

#ifdef __WXGTK20__
    if (multi_line)
    {
        if (style & wxTE_RIGHT)
            gtk_text_view_set_justification( GTK_TEXT_VIEW(m_text), GTK_JUSTIFY_RIGHT );
        else if (style & wxTE_CENTRE)
            gtk_text_view_set_justification( GTK_TEXT_VIEW(m_text), GTK_JUSTIFY_CENTER );
        // Left justify (alignment) is the default and we don't need to apply GTK_JUSTIFY_LEFT
    }
    else
    {
#ifdef __WXGTK24__
        // gtk_entry_set_alignment was introduced in gtk+-2.3.5
        if (!gtk_check_version(2,4,0))
        {
            if (style & wxTE_RIGHT)
                gtk_entry_set_alignment( GTK_ENTRY(m_text), 1.0 );
            else if (style & wxTE_CENTRE)
                gtk_entry_set_alignment( GTK_ENTRY(m_text), 0.5 );
        }
#endif
    }
#endif // __WXGTK20__
    
    // We want to be notified about text changes.
#ifdef __WXGTK20__
    if (multi_line)
    {
        g_signal_connect( G_OBJECT(m_buffer), "changed",
            GTK_SIGNAL_FUNC(gtk_text_changed_callback), (gpointer)this);
    }
    else
#endif
    
    {
        gtk_signal_connect( GTK_OBJECT(m_text), "changed",
            GTK_SIGNAL_FUNC(gtk_text_changed_callback), (gpointer)this);
    }

    m_cursor = wxCursor( wxCURSOR_IBEAM );

    wxTextAttr attrDef(GetForegroundColour(), GetBackgroundColour(), GetFont());
    SetDefaultStyle( attrDef );

    return TRUE;
}


void wxTextCtrl::CalculateScrollbar()
{
#ifndef __WXGTK20__
    if ((m_windowStyle & wxTE_MULTILINE) == 0) return;

    GtkAdjustment *adj = GTK_TEXT(m_text)->vadj;

    if (adj->upper - adj->page_size < 0.8)
    {
        if (m_vScrollbarVisible)
        {
            gtk_widget_hide( m_vScrollbar );
            m_vScrollbarVisible = FALSE;
        }
    }
    else
    {
        if (!m_vScrollbarVisible)
        {
            gtk_widget_show( m_vScrollbar );
            m_vScrollbarVisible = TRUE;
        }
    }
#endif
}

wxString wxTextCtrl::GetValue() const
{
    wxCHECK_MSG( m_text != NULL, wxT(""), wxT("invalid text ctrl") );

    wxString tmp;
    if (m_windowStyle & wxTE_MULTILINE)
    {
#ifdef __WXGTK20__
        GtkTextIter start;
        gtk_text_buffer_get_start_iter( m_buffer, &start );
        GtkTextIter end;
        gtk_text_buffer_get_end_iter( m_buffer, &end );
        gchar *text = gtk_text_buffer_get_text( m_buffer, &start, &end, TRUE );

#if wxUSE_UNICODE
        wxWCharBuffer buffer( wxConvUTF8.cMB2WX( text ) );
#else
        wxCharBuffer buffer( wxConvLocal.cWC2WX( wxConvUTF8.cMB2WC( text ) ) );
#endif
        if ( buffer )
            tmp = buffer;

        g_free( text );
#else
        gint len = gtk_text_get_length( GTK_TEXT(m_text) );
        char *text = gtk_editable_get_chars( GTK_EDITABLE(m_text), 0, len );
        tmp = text;
        g_free( text );
#endif
    }
    else
    {
        tmp = wxGTK_CONV_BACK( gtk_entry_get_text( GTK_ENTRY(m_text) ) );
    }

    return tmp;
}

void wxTextCtrl::SetValue( const wxString &value )
{
    wxCHECK_RET( m_text != NULL, wxT("invalid text ctrl") );

    if (m_windowStyle & wxTE_MULTILINE)
    {
#ifdef __WXGTK20__

#if wxUSE_UNICODE
        wxCharBuffer buffer( wxConvUTF8.cWX2MB( value) );
#else
        wxCharBuffer buffer( wxConvUTF8.cWC2MB( wxConvLocal.cWX2WC( value ) ) );
#endif
        if (gtk_text_buffer_get_char_count(m_buffer) != 0)
            IgnoreNextTextUpdate();

        gtk_text_buffer_set_text( m_buffer, buffer, strlen(buffer) );

#else
        gint len = gtk_text_get_length( GTK_TEXT(m_text) );
        gtk_editable_delete_text( GTK_EDITABLE(m_text), 0, len );
        len = 0;
        gtk_editable_insert_text( GTK_EDITABLE(m_text), value.mbc_str(), value.Length(), &len );
#endif
    }
    else
    {
        gtk_entry_set_text( GTK_ENTRY(m_text), wxGTK_CONV( value ) );
    }

    // GRG, Jun/2000: Changed this after a lot of discussion in
    //   the lists. wxWidgets 2.2 will have a set of flags to
    //   customize this behaviour.
    SetInsertionPoint(0);

    m_modified = FALSE;
}

void wxTextCtrl::WriteText( const wxString &text )
{
    wxCHECK_RET( m_text != NULL, wxT("invalid text ctrl") );

    if ( text.empty() )
        return;

    // gtk_text_changed_callback() will set m_modified to true but m_modified
    // shouldn't be changed by the program writing to the text control itself,
    // so save the old value and restore when we're done
    bool oldModified = m_modified;

    if ( m_windowStyle & wxTE_MULTILINE )
    {
#ifdef __WXGTK20__

#if wxUSE_UNICODE
        wxCharBuffer buffer( wxConvUTF8.cWX2MB( text ) );
#else
        wxCharBuffer buffer( wxConvUTF8.cWC2MB( wxConvLocal.cWX2WC( text ) ) );
#endif
        if ( !buffer )
        {
            // what else can we do? at least don't crash...
            return;
        }

        // TODO: Call whatever is needed to delete the selection.
        wxGtkTextInsert( m_text, m_buffer, m_defaultStyle, buffer );

        GtkAdjustment *adj = gtk_scrolled_window_get_vadjustment( GTK_SCROLLED_WINDOW(m_widget) );
        // Scroll to cursor, but only if scrollbar thumb is at the very bottom
        if ( adj->value == adj->upper - adj->page_size )
        {
            gtk_text_view_scroll_to_mark( GTK_TEXT_VIEW(m_text),
                    gtk_text_buffer_get_insert( m_buffer ), 0.0, FALSE, 0.0, 1.0 );
        }
#else // GTK 1.x
        // After cursor movements, gtk_text_get_point() is wrong by one.
        gtk_text_set_point( GTK_TEXT(m_text), GET_EDITABLE_POS(m_text) );

        // always use m_defaultStyle, even if it is empty as otherwise
        // resetting the style and appending some more text wouldn't work: if
        // we don't specify the style explicitly, the old style would be used
        gtk_editable_delete_selection( GTK_EDITABLE(m_text) );
        wxGtkTextInsert(m_text, m_defaultStyle, text.c_str(), text.Len());

        // we called wxGtkTextInsert with correct font, no need to do anything
        // in UpdateFontIfNeeded() any longer
        if ( !text.empty() )
        {
            SetUpdateFont(FALSE);
        }

        // Bring editable's cursor back uptodate.
        SET_EDITABLE_POS(m_text, gtk_text_get_point( GTK_TEXT(m_text) ));
#endif // GTK 1.x/2.0
    }
    else // single line
    {
        // First remove the selection if there is one
        gtk_editable_delete_selection( GTK_EDITABLE(m_text) );

        // This moves the cursor pos to behind the inserted text.
        gint len = GET_EDITABLE_POS(m_text);

#ifdef __WXGTK20__

#if wxUSE_UNICODE
        wxCharBuffer buffer( wxConvUTF8.cWX2MB( text ) );
#else
        wxCharBuffer buffer( wxConvUTF8.cWC2MB( wxConvLocal.cWX2WC( text ) ) );
#endif
        gtk_editable_insert_text( GTK_EDITABLE(m_text), buffer, strlen(buffer), &len );

#else
        gtk_editable_insert_text( GTK_EDITABLE(m_text), text.c_str(), text.Len(), &len );
#endif

        // Bring entry's cursor uptodate.
        gtk_entry_set_position( GTK_ENTRY(m_text), len );
    }

    m_modified = oldModified;
}

void wxTextCtrl::AppendText( const wxString &text )
{
    SetInsertionPointEnd();
    WriteText( text );
}

wxString wxTextCtrl::GetLineText( long lineNo ) const
{
    if (m_windowStyle & wxTE_MULTILINE)
    {
#ifndef __WXGTK20__
        gint len = gtk_text_get_length( GTK_TEXT(m_text) );
        char *text = gtk_editable_get_chars( GTK_EDITABLE(m_text), 0, len );

        if (text)
        {
            wxString buf(wxT(""));
            long i;
            int currentLine = 0;
            for (i = 0; currentLine != lineNo && text[i]; i++ )
                if (text[i] == '\n')
            currentLine++;
            // Now get the text
            int j;
            for (j = 0; text[i] && text[i] != '\n'; i++, j++ )
                buf += text[i];

            g_free( text );
            return buf;
        }
        else
        {
            return wxEmptyString;
        }
#else
        GtkTextIter line;
        gtk_text_buffer_get_iter_at_line(m_buffer,&line,lineNo);
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(m_buffer,&end );
        gchar *text = gtk_text_buffer_get_text(m_buffer,&line,&end,TRUE);
        wxString result(wxGTK_CONV_BACK(text));
        g_free(text);
        return result.BeforeFirst(wxT('\n'));
#endif
    }
    else
    {
        if (lineNo == 0) return GetValue();
        return wxEmptyString;
    }
}

void wxTextCtrl::OnDropFiles( wxDropFilesEvent &WXUNUSED(event) )
{
  /* If you implement this, don't forget to update the documentation!
   * (file docs/latex/wx/text.tex) */
    wxFAIL_MSG( wxT("wxTextCtrl::OnDropFiles not implemented") );
}

bool wxTextCtrl::PositionToXY(long pos, long *x, long *y ) const
{
    if ( m_windowStyle & wxTE_MULTILINE )
    {
        wxString text = GetValue();

        // cast to prevent warning. But pos really should've been unsigned.
        if( (unsigned long)pos > text.Len()  )
            return FALSE;

        *x=0;   // First Col
        *y=0;   // First Line

        const wxChar* stop = text.c_str() + pos;
        for ( const wxChar *p = text.c_str(); p < stop; p++ )
        {
            if (*p == wxT('\n'))
            {
                (*y)++;
                *x=0;
            }
            else
                (*x)++;
        }
    }
    else // single line control
    {
        if ( pos <= GTK_ENTRY(m_text)->text_length )
        {
            *y = 0;
            *x = pos;
        }
        else
        {
            // index out of bounds
            return FALSE;
        }
    }

    return TRUE;
}

long wxTextCtrl::XYToPosition(long x, long y ) const
{
    if (!(m_windowStyle & wxTE_MULTILINE)) return 0;

    long pos=0;
    for( int i=0; i<y; i++ ) pos += GetLineLength(i) + 1; // one for '\n'

    pos += x;
    return pos;
}

int wxTextCtrl::GetLineLength(long lineNo) const
{
    wxString str = GetLineText (lineNo);
    return (int) str.Length();
}

int wxTextCtrl::GetNumberOfLines() const
{
    if (m_windowStyle & wxTE_MULTILINE)
    {
#ifdef __WXGTK20__
        return gtk_text_buffer_get_line_count( m_buffer );
#else
        gint len = gtk_text_get_length( GTK_TEXT(m_text) );
        char *text = gtk_editable_get_chars( GTK_EDITABLE(m_text), 0, len );

        if (text)
        {
            int currentLine = 0;
            for (int i = 0; i < len; i++ )
            {
                if (text[i] == '\n')
                    currentLine++;
            }
            g_free( text );

            // currentLine is 0 based, add 1 to get number of lines
            return currentLine + 1;
        }
        else
        {
            return 0;
        }
#endif
    }
    else
    {
        return 1;
    }
}

void wxTextCtrl::SetInsertionPoint( long pos )
{
    wxCHECK_RET( m_text != NULL, wxT("invalid text ctrl") );

    if ( IsMultiLine() )
    {
#ifdef __WXGTK20__
        GtkTextIter iter;
        gtk_text_buffer_get_iter_at_offset( m_buffer, &iter, pos );
        gtk_text_buffer_place_cursor( m_buffer, &iter );
        gtk_text_view_scroll_mark_onscreen
        (
            GTK_TEXT_VIEW(m_text),
            gtk_text_buffer_get_insert( m_buffer )
        );
#else // GTK+ 1.x
        gtk_signal_disconnect_by_func( GTK_OBJECT(m_text),
          GTK_SIGNAL_FUNC(gtk_text_changed_callback), (gpointer)this);

        /* we fake a set_point by inserting and deleting. as the user
           isn't supposed to get to know about this non-sense, we
           disconnect so that no events are sent to the user program. */

        gint tmp = (gint)pos;
        gtk_editable_insert_text( GTK_EDITABLE(m_text), " ", 1, &tmp );
        gtk_editable_delete_text( GTK_EDITABLE(m_text), tmp-1, tmp );

        gtk_signal_connect( GTK_OBJECT(m_text), "changed",
          GTK_SIGNAL_FUNC(gtk_text_changed_callback), (gpointer)this);

        // bring editable's cursor uptodate. Bug in GTK.
        SET_EDITABLE_POS(m_text, gtk_text_get_point( GTK_TEXT(m_text) ));
#endif // GTK+ 2/1
    }
    else
    {
        gtk_entry_set_position( GTK_ENTRY(m_text), (int)pos );

        // Bring editable's cursor uptodate. Bug in GTK.
        SET_EDITABLE_POS(m_text, (guint32)pos);
    }
}

void wxTextCtrl::SetInsertionPointEnd()
{
    wxCHECK_RET( m_text != NULL, wxT("invalid text ctrl") );

    if (m_windowStyle & wxTE_MULTILINE)
    {
#ifdef __WXGTK20__
        GtkTextIter end;
        gtk_text_buffer_get_end_iter( m_buffer, &end );
        gtk_text_buffer_place_cursor( m_buffer, &end );
#else
        SetInsertionPoint(gtk_text_get_length(GTK_TEXT(m_text)));
#endif
    }
    else
    {
        gtk_entry_set_position( GTK_ENTRY(m_text), -1 );
    }
}

void wxTextCtrl::SetEditable( bool editable )
{
    wxCHECK_RET( m_text != NULL, wxT("invalid text ctrl") );

    if (m_windowStyle & wxTE_MULTILINE)
    {
#ifdef __WXGTK20__
        gtk_text_view_set_editable( GTK_TEXT_VIEW(m_text), editable );
#else
        gtk_text_set_editable( GTK_TEXT(m_text), editable );
#endif
    }
    else
    {
        gtk_entry_set_editable( GTK_ENTRY(m_text), editable );
    }
}

bool wxTextCtrl::Enable( bool enable )
{
    if (!wxWindowBase::Enable(enable))
    {
        // nothing to do
        return FALSE;
    }

    if (m_windowStyle & wxTE_MULTILINE)
    {
#ifdef __WXGTK20__
        SetEditable( enable );
#else
        gtk_text_set_editable( GTK_TEXT(m_text), enable );
        OnParentEnable(enable);
#endif
    }
    else
    {
        gtk_widget_set_sensitive( m_text, enable );
    }

    return TRUE;
}

// wxGTK-specific: called recursively by Enable,
// to give widgets an oppprtunity to correct their colours after they
// have been changed by Enable
void wxTextCtrl::OnParentEnable( bool enable )
{
    // If we have a custom background colour, we use this colour in both
    // disabled and enabled mode, or we end up with a different colour under the
    // text.
    wxColour oldColour = GetBackgroundColour();
    if (oldColour.Ok())
    {
        // Need to set twice or it'll optimize the useful stuff out
        if (oldColour == * wxWHITE)
            SetBackgroundColour(*wxBLACK);
        else
            SetBackgroundColour(*wxWHITE);
        SetBackgroundColour(oldColour);
    }
}

void wxTextCtrl::MarkDirty()
{
    m_modified = TRUE;
}

void wxTextCtrl::DiscardEdits()
{
    m_modified = FALSE;
}

// ----------------------------------------------------------------------------
// max text length support
// ----------------------------------------------------------------------------

void wxTextCtrl::IgnoreNextTextUpdate()
{
    m_ignoreNextUpdate = TRUE;
}

bool wxTextCtrl::IgnoreTextUpdate()
{
    if ( m_ignoreNextUpdate )
    {
        m_ignoreNextUpdate = FALSE;

        return TRUE;
    }

    return FALSE;
}

void wxTextCtrl::SetMaxLength(unsigned long len)
{
    if ( !HasFlag(wxTE_MULTILINE) )
    {
        gtk_entry_set_max_length(GTK_ENTRY(m_text), len);

        // there is a bug in GTK+ 1.2.x: "changed" signal is emitted even if
        // we had tried to enter more text than allowed by max text length and
        // the text wasn't really changed
        //
        // to detect this and generate TEXT_MAXLEN event instead of
        // TEXT_CHANGED one in this case we also catch "insert_text" signal
        //
        // when max len is set to 0 we disconnect our handler as it means that
        // we shouldn't check anything any more
        if ( len )
        {
            gtk_signal_connect( GTK_OBJECT(m_text),
                                "insert_text",
                                GTK_SIGNAL_FUNC(gtk_insert_text_callback),
                                (gpointer)this);
        }
        else // no checking
        {
            gtk_signal_disconnect_by_func
            (
                GTK_OBJECT(m_text),
                GTK_SIGNAL_FUNC(gtk_insert_text_callback),
                (gpointer)this
            );
        }
    }
}

void wxTextCtrl::SetSelection( long from, long to )
{
    wxCHECK_RET( m_text != NULL, wxT("invalid text ctrl") );

    if (from == -1 && to == -1)
    {
        from = 0;
        to = GetValue().Length();
    }

#ifndef __WXGTK20__
    if ( (m_windowStyle & wxTE_MULTILINE) &&
         !GTK_TEXT(m_text)->line_start_cache )
    {
        // tell the programmer that it didn't work
        wxLogDebug(_T("Can't call SetSelection() before realizing the control"));
        return;
    }
#endif

    if (m_windowStyle & wxTE_MULTILINE)
    {
#ifdef __WXGTK20__
        GtkTextIter fromi, toi;
        gtk_text_buffer_get_iter_at_offset( m_buffer, &fromi, from );
        gtk_text_buffer_get_iter_at_offset( m_buffer, &toi, to );

        gtk_text_buffer_place_cursor( m_buffer, &toi );
        gtk_text_buffer_move_mark_by_name( m_buffer, "selection_bound", &fromi );
#else
        gtk_editable_select_region( GTK_EDITABLE(m_text), (gint)from, (gint)to );
#endif
    }
    else
    {
        gtk_editable_select_region( GTK_EDITABLE(m_text), (gint)from, (gint)to );
    }
}

void wxTextCtrl::ShowPosition( long pos )
{
    if (m_windowStyle & wxTE_MULTILINE)
    {
#ifdef __WXGTK20__
        GtkTextIter iter;
        gtk_text_buffer_get_start_iter( m_buffer, &iter );
        gtk_text_iter_set_offset( &iter, pos );
        GtkTextMark *mark = gtk_text_buffer_create_mark( m_buffer, NULL, &iter, TRUE );
        gtk_text_view_scroll_to_mark( GTK_TEXT_VIEW(m_text), mark, 0.0, FALSE, 0.0, 0.0 );
#else // GTK 1.x
        GtkAdjustment *vp = GTK_TEXT(m_text)->vadj;
        float totalLines =  (float) GetNumberOfLines();
        long posX;
        long posY;
        PositionToXY(pos, &posX, &posY);
        float posLine = (float) posY;
        float p = (posLine/totalLines)*(vp->upper - vp->lower) + vp->lower;
        gtk_adjustment_set_value(GTK_TEXT(m_text)->vadj, p);
#endif // GTK 1.x/2.x
    }
}

#ifdef __WXGTK20__

wxTextCtrlHitTestResult
wxTextCtrl::HitTest(const wxPoint& pt, long *pos) const
{
    if ( !IsMultiLine() )
    {
        // not supported
        return wxTE_HT_UNKNOWN;
    }

    int x, y;
    gtk_text_view_window_to_buffer_coords
    (
        GTK_TEXT_VIEW(m_text),
        GTK_TEXT_WINDOW_TEXT,
        pt.x, pt.y,
        &x, &y
    );

    GtkTextIter iter;
    gtk_text_view_get_iter_at_location(GTK_TEXT_VIEW(m_text), &iter, x, y);
    if ( pos )
        *pos = gtk_text_iter_get_offset(&iter);

    return wxTE_HT_ON_TEXT;
}

#endif // __WXGTK20__

long wxTextCtrl::GetInsertionPoint() const
{
    wxCHECK_MSG( m_text != NULL, 0, wxT("invalid text ctrl") );

#ifdef __WXGTK20__
    if (m_windowStyle & wxTE_MULTILINE)
    {
        // There is no direct accessor for the cursor, but
        // internally, the cursor is the "mark" called
        // "insert" in the text view's btree structure.

        GtkTextMark *mark = gtk_text_buffer_get_insert( m_buffer );
        GtkTextIter cursor;
        gtk_text_buffer_get_iter_at_mark( m_buffer, &cursor, mark );

        return gtk_text_iter_get_offset( &cursor );
    }
    else
#endif
    {
    return (long) GET_EDITABLE_POS(m_text);
    }
}

long wxTextCtrl::GetLastPosition() const
{
    wxCHECK_MSG( m_text != NULL, 0, wxT("invalid text ctrl") );

    int pos = 0;

    if (m_windowStyle & wxTE_MULTILINE)
    {
#ifdef __WXGTK20__
        GtkTextIter end;
        gtk_text_buffer_get_end_iter( m_buffer, &end );

        pos = gtk_text_iter_get_offset( &end );
#else
        pos = gtk_text_get_length( GTK_TEXT(m_text) );
#endif
    }
    else
    {
        pos = GTK_ENTRY(m_text)->text_length;
    }

    return (long)pos;
}

void wxTextCtrl::Remove( long from, long to )
{
    wxCHECK_RET( m_text != NULL, wxT("invalid text ctrl") );

#ifdef __WXGTK20__
    if (m_windowStyle & wxTE_MULTILINE)
    {
        GtkTextIter fromi, toi;
        gtk_text_buffer_get_iter_at_offset( m_buffer, &fromi, from );
        gtk_text_buffer_get_iter_at_offset( m_buffer, &toi, to );

        gtk_text_buffer_delete( m_buffer, &fromi, &toi );
    }
    else // single line
#endif
    gtk_editable_delete_text( GTK_EDITABLE(m_text), (gint)from, (gint)to );
}

void wxTextCtrl::Replace( long from, long to, const wxString &value )
{
    wxCHECK_RET( m_text != NULL, wxT("invalid text ctrl") );

    Remove( from, to );

    if (!value.IsEmpty())
    {
#ifdef __WXGTK20__
        SetInsertionPoint( from );
        WriteText( value );
#else // GTK 1.x
        gint pos = (gint)from;
#if wxUSE_UNICODE
        wxWX2MBbuf buf = value.mbc_str();
        gtk_editable_insert_text( GTK_EDITABLE(m_text), buf, strlen(buf), &pos );
#else
        gtk_editable_insert_text( GTK_EDITABLE(m_text), value, value.Length(), &pos );
#endif // wxUSE_UNICODE
#endif // GTK 1.x/2.x
    }
}

void wxTextCtrl::Cut()
{
    wxCHECK_RET( m_text != NULL, wxT("invalid text ctrl") );

#ifdef __WXGTK20__
    if (m_windowStyle & wxTE_MULTILINE)
        g_signal_emit_by_name(m_text, "cut-clipboard");
    else
#endif
        gtk_editable_cut_clipboard(GTK_EDITABLE(m_text) DUMMY_CLIPBOARD_ARG);
}

void wxTextCtrl::Copy()
{
    wxCHECK_RET( m_text != NULL, wxT("invalid text ctrl") );

#ifdef __WXGTK20__
    if (m_windowStyle & wxTE_MULTILINE)
        g_signal_emit_by_name(m_text, "copy-clipboard");
    else
#endif
        gtk_editable_copy_clipboard(GTK_EDITABLE(m_text) DUMMY_CLIPBOARD_ARG);
}

void wxTextCtrl::Paste()
{
    wxCHECK_RET( m_text != NULL, wxT("invalid text ctrl") );

#ifdef __WXGTK20__
    if (m_windowStyle & wxTE_MULTILINE)
        g_signal_emit_by_name(m_text, "paste-clipboard");
    else
#endif
        gtk_editable_paste_clipboard(GTK_EDITABLE(m_text) DUMMY_CLIPBOARD_ARG);
}

// Undo/redo
void wxTextCtrl::Undo()
{
    // TODO
    wxFAIL_MSG( wxT("wxTextCtrl::Undo not implemented") );
}

void wxTextCtrl::Redo()
{
    // TODO
    wxFAIL_MSG( wxT("wxTextCtrl::Redo not implemented") );
}

bool wxTextCtrl::CanUndo() const
{
    // TODO
    //wxFAIL_MSG( wxT("wxTextCtrl::CanUndo not implemented") );
    return FALSE;
}

bool wxTextCtrl::CanRedo() const
{
    // TODO
    //wxFAIL_MSG( wxT("wxTextCtrl::CanRedo not implemented") );
    return FALSE;
}

// If the return values from and to are the same, there is no
// selection.
void wxTextCtrl::GetSelection(long* fromOut, long* toOut) const
{
    wxCHECK_RET( m_text != NULL, wxT("invalid text ctrl") );

    gint from = -1;
    gint to = -1;
    bool haveSelection = FALSE;

#ifdef __WXGTK20__
     if (m_windowStyle & wxTE_MULTILINE)
     {
         GtkTextIter ifrom, ito;
         if ( gtk_text_buffer_get_selection_bounds(m_buffer, &ifrom, &ito) )
         {
             haveSelection = TRUE;
             from = gtk_text_iter_get_offset(&ifrom);
             to = gtk_text_iter_get_offset(&ito);
         }
     }
     else  // not multi-line
     {
         if ( gtk_editable_get_selection_bounds( GTK_EDITABLE(m_text),
                                                 &from, &to) )
         {
             haveSelection = TRUE;
         }
     }
#else //  not GTK2
     if ( (GTK_EDITABLE(m_text)->has_selection) )
     {
         haveSelection = TRUE;
         from = (long) GTK_EDITABLE(m_text)->selection_start_pos;
         to = (long) GTK_EDITABLE(m_text)->selection_end_pos;
     }
#endif

     if (! haveSelection )
          from = to = GetInsertionPoint();

     if ( from > to )
     {
         // exchange them to be compatible with wxMSW
         gint tmp = from;
         from = to;
         to = tmp;
     }

    if ( fromOut )
        *fromOut = from;
    if ( toOut )
        *toOut = to;
}


bool wxTextCtrl::IsEditable() const
{
    wxCHECK_MSG( m_text != NULL, FALSE, wxT("invalid text ctrl") );

#ifdef __WXGTK20__
    if (m_windowStyle & wxTE_MULTILINE)
    {
        return gtk_text_view_get_editable(GTK_TEXT_VIEW(m_text));
    }
    else
    {
        return gtk_editable_get_editable(GTK_EDITABLE(m_text));
    }
#else
    return GTK_EDITABLE(m_text)->editable;
#endif
}

bool wxTextCtrl::IsModified() const
{
    return m_modified;
}

void wxTextCtrl::Clear()
{
    SetValue( wxT("") );
}

void wxTextCtrl::OnChar( wxKeyEvent &key_event )
{
    wxCHECK_RET( m_text != NULL, wxT("invalid text ctrl") );

    if ((key_event.GetKeyCode() == WXK_RETURN) && (m_windowStyle & wxPROCESS_ENTER))
    {
        wxCommandEvent event(wxEVT_COMMAND_TEXT_ENTER, m_windowId);
        event.SetEventObject(this);
        event.SetString(GetValue());
        if (GetEventHandler()->ProcessEvent(event)) return;
    }

    if ((key_event.GetKeyCode() == WXK_RETURN) && !(m_windowStyle & wxTE_MULTILINE))
    {
        // This will invoke the dialog default action, such
        // as the clicking the default button.

        wxWindow *top_frame = m_parent;
        while (top_frame->GetParent() && !(top_frame->IsTopLevel()))
            top_frame = top_frame->GetParent();

        if (top_frame && GTK_IS_WINDOW(top_frame->m_widget))
        {
            GtkWindow *window = GTK_WINDOW(top_frame->m_widget);

            if (window->default_widget)
            {
                gtk_widget_activate (window->default_widget);
                return;
            }
        }
    }

    key_event.Skip();
}

GtkWidget* wxTextCtrl::GetConnectWidget()
{
    return GTK_WIDGET(m_text);
}

bool wxTextCtrl::IsOwnGtkWindow( GdkWindow *window )
{
    if (m_windowStyle & wxTE_MULTILINE)
    {
#ifdef __WXGTK20__
        return window == gtk_text_view_get_window( GTK_TEXT_VIEW( m_text ), GTK_TEXT_WINDOW_TEXT );  // pure guesswork
#else
        return (window == GTK_TEXT(m_text)->text_area);
#endif
    }
    else
    {
        return (window == GTK_ENTRY(m_text)->text_area);
    }
}

// the font will change for subsequent text insertiongs
bool wxTextCtrl::SetFont( const wxFont &font )
{
    wxCHECK_MSG( m_text != NULL, FALSE, wxT("invalid text ctrl") );

    if ( !wxTextCtrlBase::SetFont(font) )
    {
        // font didn't change, nothing to do
        return FALSE;
    }

    if ( m_windowStyle & wxTE_MULTILINE )
    {
        SetUpdateFont(TRUE);

        m_defaultStyle.SetFont(font);

        ChangeFontGlobally();
    }

    return TRUE;
}

void wxTextCtrl::ChangeFontGlobally()
{
    // this method is very inefficient and hence should be called as rarely as
    // possible!
    //
    // TODO: it can be implemented much more efficiently for GTK2
#ifndef __WXGTK20__
    wxASSERT_MSG( (m_windowStyle & wxTE_MULTILINE) && m_updateFont,

                  _T("shouldn't be called for single line controls") );
#else
    wxASSERT_MSG( (m_windowStyle & wxTE_MULTILINE),
                  _T("shouldn't be called for single line controls") );
#endif

    wxString value = GetValue();
    if ( !value.IsEmpty() )
    {
        SetUpdateFont(FALSE);

        Clear();
        AppendText(value);
    }
}

#ifndef __WXGTK20__

void wxTextCtrl::UpdateFontIfNeeded()
{
    if ( m_updateFont )
        ChangeFontGlobally();
}

#endif // GTK+ 1.x

bool wxTextCtrl::SetForegroundColour(const wxColour& colour)
{
    if ( !wxControl::SetForegroundColour(colour) )
        return FALSE;

    // update default fg colour too
    m_defaultStyle.SetTextColour(colour);

    return TRUE;
}

bool wxTextCtrl::SetBackgroundColour( const wxColour &colour )
{
    wxCHECK_MSG( m_text != NULL, FALSE, wxT("invalid text ctrl") );

    if ( !wxControl::SetBackgroundColour( colour ) )
        return FALSE;

#ifndef __WXGTK20__
    if (!m_widget->window)
        return FALSE;
#endif

    if (!m_backgroundColour.Ok())
        return FALSE;

    if (m_windowStyle & wxTE_MULTILINE)
    {
#ifndef __WXGTK20__
        GdkWindow *window = GTK_TEXT(m_text)->text_area;
        if (!window)
            return FALSE;
        m_backgroundColour.CalcPixel( gdk_window_get_colormap( window ) );
        gdk_window_set_background( window, m_backgroundColour.GetColor() );
        gdk_window_clear( window );
#endif
    }

    // change active background color too
    m_defaultStyle.SetBackgroundColour( colour );

    return TRUE;
}

bool wxTextCtrl::SetStyle( long start, long end, const wxTextAttr& style )
{
    if ( m_windowStyle & wxTE_MULTILINE )
    {
        if ( style.IsDefault() )
        {
            // nothing to do
            return TRUE;
        }
#ifdef __WXGTK20__
        gint l = gtk_text_buffer_get_char_count( m_buffer );

        wxCHECK_MSG( start >= 0 && end <= l, FALSE,
                     _T("invalid range in wxTextCtrl::SetStyle") );

        GtkTextIter starti, endi;
        gtk_text_buffer_get_iter_at_offset( m_buffer, &starti, start );
        gtk_text_buffer_get_iter_at_offset( m_buffer, &endi, end );

        // use the attributes from style which are set in it and fall back
        // first to the default style and then to the text control default
        // colours for the others
        wxTextAttr attr = wxTextAttr::Combine(style, m_defaultStyle, this);

        wxGtkTextApplyTagsFromAttr( m_buffer, attr, &starti, &endi );

         return TRUE;
#else
        // VERY dirty way to do that - removes the required text and re-adds it
        // with styling (FIXME)

        gint l = gtk_text_get_length( GTK_TEXT(m_text) );

        wxCHECK_MSG( start >= 0 && end <= l, FALSE,
                     _T("invalid range in wxTextCtrl::SetStyle") );

        gint old_pos = gtk_editable_get_position( GTK_EDITABLE(m_text) );
        char *text = gtk_editable_get_chars( GTK_EDITABLE(m_text), start, end );
        wxString tmp(text,*wxConvCurrent);
        g_free( text );

        gtk_editable_delete_text( GTK_EDITABLE(m_text), start, end );
        gtk_editable_set_position( GTK_EDITABLE(m_text), start );

#if wxUSE_UNICODE
        wxWX2MBbuf buf = tmp.mbc_str();
        const char *txt = buf;
        size_t txtlen = strlen(buf);
#else
        const char *txt = tmp;
        size_t txtlen = tmp.length();
#endif

        // use the attributes from style which are set in it and fall back
        // first to the default style and then to the text control default
        // colours for the others
        wxGtkTextInsert(m_text,
                        wxTextAttr::Combine(style, m_defaultStyle, this),
                        txt,
                        txtlen);

        /* does not seem to help under GTK+ 1.2 !!!
        gtk_editable_set_position( GTK_EDITABLE(m_text), old_pos ); */
        SetInsertionPoint( old_pos );
#endif
        return TRUE;
    }
    else // singe line
    {
        // cannot do this for GTK+'s Entry widget
        return FALSE;
    }
}

void wxTextCtrl::DoApplyWidgetStyle(GtkRcStyle *style)
{
    gtk_widget_modify_style(m_text, style);
}

void wxTextCtrl::OnCut(wxCommandEvent& WXUNUSED(event))
{
    Cut();
}

void wxTextCtrl::OnCopy(wxCommandEvent& WXUNUSED(event))
{
    Copy();
}

void wxTextCtrl::OnPaste(wxCommandEvent& WXUNUSED(event))
{
    Paste();
}

void wxTextCtrl::OnUndo(wxCommandEvent& WXUNUSED(event))
{
    Undo();
}

void wxTextCtrl::OnRedo(wxCommandEvent& WXUNUSED(event))
{
    Redo();
}

void wxTextCtrl::OnUpdateCut(wxUpdateUIEvent& event)
{
    event.Enable( CanCut() );
}

void wxTextCtrl::OnUpdateCopy(wxUpdateUIEvent& event)
{
    event.Enable( CanCopy() );
}

void wxTextCtrl::OnUpdatePaste(wxUpdateUIEvent& event)
{
    event.Enable( CanPaste() );
}

void wxTextCtrl::OnUpdateUndo(wxUpdateUIEvent& event)
{
    event.Enable( CanUndo() );
}

void wxTextCtrl::OnUpdateRedo(wxUpdateUIEvent& event)
{
    event.Enable( CanRedo() );
}

void wxTextCtrl::OnInternalIdle()
{
    wxCursor cursor = m_cursor;
    if (g_globalCursor.Ok()) cursor = g_globalCursor;

    if (cursor.Ok())
    {
#ifndef __WXGTK20__
        GdkWindow *window = (GdkWindow*) NULL;
        if (HasFlag(wxTE_MULTILINE))
            window = GTK_TEXT(m_text)->text_area;
        else
            window = GTK_ENTRY(m_text)->text_area;

        if (window)
            gdk_window_set_cursor( window, cursor.GetCursor() );

        if (!g_globalCursor.Ok())
            cursor = *wxSTANDARD_CURSOR;

        window = m_widget->window;
        if ((window) && !(GTK_WIDGET_NO_WINDOW(m_widget)))
            gdk_window_set_cursor( window, cursor.GetCursor() );
#endif
    }

    if (g_delayedFocus == this)
    {
        if (GTK_WIDGET_REALIZED(m_widget))
        {
            gtk_widget_grab_focus( m_widget );
            g_delayedFocus = NULL;
        }
    }

    if (wxUpdateUIEvent::CanUpdate(this))
        UpdateWindowUI(wxUPDATE_UI_FROMIDLE);
}

wxSize wxTextCtrl::DoGetBestSize() const
{
    // FIXME should be different for multi-line controls...
    wxSize ret( wxControl::DoGetBestSize() );
    wxSize best(80, ret.y);
    CacheBestSize(best);
    return best;
}

// ----------------------------------------------------------------------------
// freeze/thaw
// ----------------------------------------------------------------------------

void wxTextCtrl::Freeze()
{
    if ( HasFlag(wxTE_MULTILINE) )
    {
#ifdef __WXGTK20__
        if ( !m_frozenness++ )
        {
            // freeze textview updates and remove buffer
            g_signal_connect( G_OBJECT(m_text), "expose_event",
                GTK_SIGNAL_FUNC(gtk_text_exposed_callback), (gpointer)this);
            g_signal_connect( G_OBJECT(m_widget), "expose_event",
                GTK_SIGNAL_FUNC(gtk_text_exposed_callback), (gpointer)this);
            gtk_widget_set_sensitive(m_widget, false);
            g_object_ref(m_buffer);
            gtk_text_view_set_buffer(GTK_TEXT_VIEW(m_text), gtk_text_buffer_new(NULL));
    }
#else
        gtk_text_freeze(GTK_TEXT(m_text));
#endif
    }
}

void wxTextCtrl::Thaw()
{
    if ( HasFlag(wxTE_MULTILINE) )
    {
#ifdef __WXGTK20__
        wxASSERT_MSG( m_frozenness > 0, _T("Thaw() without matching Freeze()") );

        if ( !--m_frozenness )
        {
            // Reattach buffer and thaw textview updates
            gtk_text_view_set_buffer(GTK_TEXT_VIEW(m_text), m_buffer);
            g_object_unref(m_buffer);
            gtk_widget_set_sensitive(m_widget, true);
            g_signal_handlers_disconnect_by_func(m_widget, (gpointer)gtk_text_exposed_callback, this);
            g_signal_handlers_disconnect_by_func(m_text, (gpointer)gtk_text_exposed_callback, this);
        }
#else
        GTK_TEXT(m_text)->vadj->value = 0.0;

        gtk_text_thaw(GTK_TEXT(m_text));
#endif
    }
}

// ----------------------------------------------------------------------------
// scrolling
// ----------------------------------------------------------------------------

GtkAdjustment *wxTextCtrl::GetVAdj() const
{
    if ( !IsMultiLine() )
        return NULL;

#ifdef __WXGTK20__
    return gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(m_widget));
#else
    return GTK_TEXT(m_text)->vadj;
#endif
}

bool wxTextCtrl::DoScroll(GtkAdjustment *adj, int diff)
{
    float value = adj->value + diff;

    if ( value < 0 )
        value = 0;

    float upper = adj->upper - adj->page_size;
    if ( value > upper )
        value = upper;

    // did we noticeably change the scroll position?
    if ( fabs(adj->value - value) < 0.2 )
    {
        // well, this is what Robert does in wxScrollBar, so it must be good...
        return FALSE;
    }

    adj->value = value;

#ifdef __WXGTK20__
    gtk_adjustment_value_changed(GTK_ADJUSTMENT(adj));
#else
    gtk_signal_emit_by_name(GTK_OBJECT(adj), "value_changed");
#endif

    return TRUE;
}

bool wxTextCtrl::ScrollLines(int lines)
{
    GtkAdjustment *adj = GetVAdj();
    if ( !adj )
        return FALSE;

#ifdef __WXGTK20__
    int diff = (int)ceil(lines*adj->step_increment);
#else
    // this is hardcoded to 10 in GTK+ 1.2 (great idea)
    int diff = 10*lines;
#endif

    return DoScroll(adj, diff);
}

bool wxTextCtrl::ScrollPages(int pages)
{
    GtkAdjustment *adj = GetVAdj();
    if ( !adj )
        return FALSE;

    return DoScroll(adj, (int)ceil(pages*adj->page_increment));
}


// static
wxVisualAttributes
wxTextCtrl::GetClassDefaultAttributes(wxWindowVariant WXUNUSED(variant))
{
    return GetDefaultAttributesFromGTKWidget(gtk_entry_new, true);
}
