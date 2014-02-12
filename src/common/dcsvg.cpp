/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/svg.cpp
// Purpose:     SVG sample
// Author:      Chris Elliott
// Modified by:
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#if wxUSE_SVG

#ifndef WX_PRECOMP
    #include "wx/dcmemory.h"
    #include "wx/dcscreen.h"
    #include "wx/icon.h"
    #include "wx/image.h"
#endif

#include "wx/dcsvg.h"
#include "wx/wfstream.h"
#include "wx/filename.h"

#include "wx/private/markupparser.h"

// ----------------------------------------------------------
// Global utilities
// ----------------------------------------------------------

namespace
{

inline double DegToRad(double deg) { return (deg * M_PI) / 180.0; }

// This function returns a string representation of a floating point number in
// C locale (i.e. always using "." for the decimal separator) and with the
// fixed precision (which is 2 for some unknown reason but this is what it was
// in this code originally).
inline wxString NumStr(double f)
{
    return wxString::FromCDouble(f, 2);
}

// Return the colour representation as HTML-like "#rrggbb" string and also
// returns its alpha as opacity number in 0..1 range.
wxString Col2SVG(wxColour c, float *opacity)
{
    if ( c.Alpha() != wxALPHA_OPAQUE )
    {
        *opacity = c.Alpha()/255.;

        // Remove the alpha before using GetAsString(wxC2S_HTML_SYNTAX) as it
        // doesn't support colours with alpha channel.
        c = wxColour(c.GetRGB());
    }
    else // No alpha.
    {
        *opacity = 1.;
    }

    return c.GetAsString(wxC2S_HTML_SYNTAX);
}

wxString wxPenString(wxColour c, int style = wxPENSTYLE_SOLID)
{
    float opacity;
    wxString s = wxT("stroke:") + Col2SVG(c, &opacity)  + wxT("; ");

    switch ( style )
    {
        case wxPENSTYLE_SOLID:
            s += wxString::Format(wxT("stroke-opacity:%s; "), NumStr(opacity));
            break;
        case wxPENSTYLE_TRANSPARENT:
            s += wxT("stroke-opacity:0.0; ");
            break;
        default :
            wxASSERT_MSG(false, wxT("wxSVGFileDC::Requested Pen Style not available"));
    }

    return s;
}

wxString wxBrushString(wxColour c, int style = wxBRUSHSTYLE_SOLID)
{
    float opacity;
    wxString s = wxT("fill:") + Col2SVG(c, &opacity)  + wxT("; ");

    switch ( style )
    {
        case wxBRUSHSTYLE_SOLID:
            s += wxString::Format(wxT("fill-opacity:%s; "), NumStr(opacity));
            break;
        case wxBRUSHSTYLE_TRANSPARENT:
            s += wxT("fill-opacity:0.0; ");
            break;
        default :
            wxASSERT_MSG(false, wxT("wxSVGFileDC::Requested Brush Style not available"));
    }

    return s;
}

} // anonymous namespace

// ----------------------------------------------------------
// wxSVGFileDCImpl
// ----------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxSVGFileDCImpl, wxDC)

wxSVGFileDCImpl::wxSVGFileDCImpl( wxSVGFileDC *owner, const wxString &filename,
                    int width, int height, double dpi ) :
        wxDCImpl( owner )
    {
        Init( filename, width, height, dpi );
    }

void wxSVGFileDCImpl::Init (const wxString &filename, int Width, int Height, double dpi)
{
    m_width = Width;
    m_height = Height;

    m_dpi = dpi;

    m_OK = true;

    m_clipUniqueId = 0;
    m_clipNestingLevel = 0;

    m_mm_to_pix_x = dpi/25.4;
    m_mm_to_pix_y = dpi/25.4;

    m_backgroundBrush = *wxTRANSPARENT_BRUSH;
    m_textForegroundColour = *wxBLACK;
    m_textBackgroundColour = *wxWHITE;
    m_colour = wxColourDisplay();

    m_pen   = *wxBLACK_PEN;
    m_font  = *wxNORMAL_FONT;
    m_brush = *wxWHITE_BRUSH;

    m_graphics_changed = true;

    ////////////////////code here

    m_outfile = new wxFileOutputStream(filename);
    m_OK = m_outfile->IsOk();
    if (m_OK)
    {
        m_filename = filename;
        m_sub_images = 0;
        wxString s;
        s = wxT("<?xml version=\"1.0\" standalone=\"no\"?>\n");
        write(s);
        s = wxT("<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 20010904//EN\"\n");
        write(s);
        s = wxT("\"http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd\">\n");
        write(s);
        s = wxT("<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n");
        write(s);
        s.Printf( wxT("    width=\"%scm\" height=\"%scm\" viewBox=\"0 0 %d %d \">\n"), NumStr(float(Width)/dpi*2.54), NumStr(float(Height)/dpi*2.54), Width, Height );
        write(s);
        s = wxT("<title>SVG Picture created as ") + wxFileName(filename).GetFullName() + wxT(" </title>\n");
        write(s);
        s = wxString (wxT("<desc>Picture generated by wxSVG ")) + wxSVGVersion + wxT(" </desc>\n");
        write(s);
        s =  wxT("<g style=\"fill:black; stroke:black; stroke-width:1\">\n");
        write(s);
    }
}

wxSVGFileDCImpl::~wxSVGFileDCImpl()
{
    wxString s = wxT("</g> \n</svg> \n");
    write(s);
    delete m_outfile;
}

void wxSVGFileDCImpl::DoGetSizeMM( int *width, int *height ) const
{
    if (width)
        *width = wxRound( (double)m_width / m_mm_to_pix_x );

    if (height)
        *height = wxRound( (double)m_height / m_mm_to_pix_y );
}

wxSize wxSVGFileDCImpl::GetPPI() const
{
    return wxSize( wxRound(m_dpi), wxRound(m_dpi) );
}

void wxSVGFileDCImpl::DoDrawLine (wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2)
{
    NewGraphicsIfNeeded();
    wxString s;
    s.Printf ( wxT("<path d=\"M%d %d L%d %d\" /> \n"), x1,y1,x2,y2 );
    if (m_OK)
    {
        write(s);
    }
    CalcBoundingBox(x1, y1);
    CalcBoundingBox(x2, y2);
}

void wxSVGFileDCImpl::DoDrawLines(int n, const wxPoint points[], wxCoord xoffset , wxCoord yoffset )
{
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#endif
    for ( int i = 1; i < n; i++ )
    {
        DoDrawLine ( points [i-1].x + xoffset, points [i-1].y + yoffset,
            points [ i ].x + xoffset, points [ i ].y + yoffset );
    }
}

void wxSVGFileDCImpl::DoDrawPoint (wxCoord x1, wxCoord y1)
{
    wxString s;
    NewGraphicsIfNeeded();
    s = wxT("<g style = \"stroke-linecap:round;\" > \n");
    write(s);
    DoDrawLine ( x1,y1,x1,y1 );
    s = wxT("</g>");
    write(s);
}

void wxSVGFileDCImpl::DoDrawCheckMark(wxCoord x1, wxCoord y1, wxCoord width, wxCoord height)
{
    wxDCImpl::DoDrawCheckMark (x1,y1,width,height);
}

void wxSVGFileDCImpl::DoDrawText(const wxString& text, wxCoord x1, wxCoord y1)
{
    DoDrawRotatedText(text, x1,y1,0.0);
}

void wxSVGFileDCImpl::DoDrawRotatedText(const wxString& sText, wxCoord x, wxCoord y, double angle)
{
    //known bug; if the font is drawn in a scaled DC, it will not behave exactly as wxMSW
    NewGraphicsIfNeeded();
    wxString s, sTmp;

    // calculate bounding box
    wxCoord w, h, desc;
    DoGetTextExtent(sText, &w, &h, &desc);

    double rad = DegToRad(angle);

    // wxT("upper left") and wxT("upper right")
    CalcBoundingBox(x, y);
    CalcBoundingBox((wxCoord)(x + w*cos(rad)), (wxCoord)(y - h*sin(rad)));

    // wxT("bottom left") and wxT("bottom right")
    CalcBoundingBox((wxCoord)(x + h*sin(rad)), (wxCoord)(y + h*cos(rad)));
    CalcBoundingBox((wxCoord)(x + h*sin(rad) + w*cos(rad)), (wxCoord)(y + h*cos(rad) - w*sin(rad)));

    if (m_backgroundMode == wxBRUSHSTYLE_SOLID)
    {
        // draw background first
        // just like DoDrawRectangle except we pass the text color to it and set the border to a 1 pixel wide text background

        sTmp.Printf ( wxT(" <rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" "), x, y, w, h );
        s = sTmp + wxT("style=\"") + wxBrushString(m_textBackgroundColour);
        s += wxT("stroke-width:1; ") + wxPenString(m_textBackgroundColour);
        sTmp.Printf ( wxT("\" transform=\"rotate( %s %d %d )  \" />"), NumStr(-angle), x,y );
        s += sTmp + wxT("\n");
        write(s);
    }

    // convert x,y to SVG text x,y (the coordinates of the text baseline)
    x = (wxCoord)(x + (h-desc)*sin(rad));
    y = (wxCoord)(y + (h-desc)*cos(rad));

    //now do the text itself
    s.Printf (wxT(" <text x=\"%d\" y=\"%d\" "),x,y );

    sTmp = m_font.GetFaceName();
    if (sTmp.Len() > 0)  s += wxT("style=\"font-family:") + sTmp + wxT("; ");
    else s += wxT("style=\" ");

    wxString fontweights [3] = { wxT("normal"), wxT("lighter"), wxT("bold") };
    s += wxT("font-weight:") + fontweights[m_font.GetWeight() - wxNORMAL] + wxT("; ");

    wxString fontstyles [5] = { wxT("normal"), wxT("style error"), wxT("style error"), wxT("italic"), wxT("oblique") };
    s += wxT("font-style:") + fontstyles[m_font.GetStyle() - wxNORMAL] + wxT("; ");

    sTmp.Printf (wxT("font-size:%dpt; "), m_font.GetPointSize() );
    s += sTmp;
    //text will be solid, unless alpha value isn't opaque in the foreground colour
    s += wxBrushString(m_textForegroundColour) + wxPenString(m_textForegroundColour);
    sTmp.Printf ( wxT("stroke-width:0;\"  transform=\"rotate( %s %d %d )  \" >"),  NumStr(-angle), x,y );
    s += sTmp + wxMarkupParser::Quote(sText) + wxT("</text> ") + wxT("\n");
    if (m_OK)
    {
        write(s);
    }
}

void wxSVGFileDCImpl::DoDrawRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
    DoDrawRoundedRectangle(x, y, width, height, 0);
}

void wxSVGFileDCImpl::DoDrawRoundedRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height, double radius )

{
    NewGraphicsIfNeeded();
    wxString s;

    s.Printf ( wxT(" <rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" rx=\"%s\" "),
            x, y, width, height, NumStr(radius) );

    s += wxT(" /> \n");
    write(s);

    CalcBoundingBox(x, y);
    CalcBoundingBox(x + width, y + height);
}

void wxSVGFileDCImpl::DoDrawPolygon(int n, const wxPoint points[],
                                    wxCoord xoffset, wxCoord yoffset,
                                    wxPolygonFillMode fillStyle)
{
    NewGraphicsIfNeeded();
    wxString s, sTmp;
    s = wxT("<polygon style=\"");
    if ( fillStyle == wxODDEVEN_RULE )
        s += wxT("fill-rule:evenodd; ");
    else
        s += wxT("fill-rule:nonzero; ");

    s += wxT("\" \npoints=\"");

#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#endif
    for (int i = 0; i < n;  i++)
    {
        sTmp.Printf ( wxT("%d,%d"), points [i].x+xoffset, points[i].y+yoffset );
        s += sTmp + wxT("\n");
        CalcBoundingBox ( points [i].x+xoffset, points[i].y+yoffset);
    }
    s += wxT("\" /> \n");
    write(s);
}

void wxSVGFileDCImpl::DoDrawEllipse (wxCoord x, wxCoord y, wxCoord width, wxCoord height)

{
    NewGraphicsIfNeeded();

    int rh = height /2;
    int rw = width  /2;

    wxString s;
    s.Printf ( wxT("<ellipse cx=\"%d\" cy=\"%d\" rx=\"%d\" ry=\"%d\" "), x+rw,y+rh, rw, rh );
    s += wxT(" /> \n");

    write(s);

    CalcBoundingBox(x, y);
    CalcBoundingBox(x + width, y + height);
}

void wxSVGFileDCImpl::DoDrawArc(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2, wxCoord xc, wxCoord yc)
{
    /* Draws an arc of a circle, centred on (xc, yc), with starting point
    (x1, y1) and ending at (x2, y2). The current pen is used for the outline
    and the current brush for filling the shape.

    The arc is drawn in an anticlockwise direction from the start point to
    the end point.

    Might be better described as Pie drawing */

    NewGraphicsIfNeeded();
    wxString s;

    // we need the radius of the circle which has two estimates
    double r1 = sqrt ( double( (x1-xc)*(x1-xc) ) + double( (y1-yc)*(y1-yc) ) );
    double r2 = sqrt ( double( (x2-xc)*(x2-xc) ) + double( (y2-yc)*(y2-yc) ) );

    wxASSERT_MSG( (fabs ( r2-r1 ) <= 3), wxT("wxSVGFileDC::DoDrawArc Error in getting radii of circle"));
    if ( fabs ( r2-r1 ) > 3 )    //pixels
    {
        s = wxT("<!--- wxSVGFileDC::DoDrawArc Error in getting radii of circle --> \n");
        write(s);
    }

    double theta1 = atan2((double)(yc-y1),(double)(x1-xc));
    if ( theta1 < 0 ) theta1 = theta1 + M_PI * 2;
    double theta2 = atan2((double)(yc-y2), (double)(x2-xc));
    if ( theta2 < 0 ) theta2 = theta2 + M_PI * 2;
    if ( theta2 < theta1 ) theta2 = theta2 + M_PI *2;

    int fArc;                  // flag for large or small arc 0 means less than 180 degrees
    if ( fabs(theta2 - theta1) > M_PI ) fArc = 1; else fArc = 0;

    int fSweep = 0;             // flag for sweep always 0

    s.Printf ( wxT("<path d=\"M%d %d A%s %s 0.0 %d %d %d %d L%d %d z "),
        x1,y1, NumStr(r1), NumStr(r2), fArc, fSweep, x2, y2, xc, yc );

    // the z means close the path and fill
    s += wxT(" \" /> \n");


    if (m_OK)
    {
        write(s);
    }
}

void wxSVGFileDCImpl::DoDrawEllipticArc(wxCoord x,wxCoord y,wxCoord w,wxCoord h,double sa,double ea)
{
    /*
    Draws an arc of an ellipse. The current pen is used for drawing the arc
    and the current brush is used for drawing the pie. This function is
    currently only available for X window and PostScript device contexts.

    x and y specify the x and y coordinates of the upper-left corner of the
    rectangle that contains the ellipse.

    width and height specify the width and height of the rectangle that
    contains the ellipse.

    start and end specify the start and end of the arc relative to the
    three-o'clock position from the center of the rectangle. Angles are
    specified in degrees (360 is a complete circle). Positive values mean
    counter-clockwise motion. If start is equal to end, a complete ellipse
    will be drawn. */

    //known bug: SVG draws with the current pen along the radii, but this does not happen in wxMSW

    NewGraphicsIfNeeded();

    wxString s;
    //radius
    double rx = w / 2;
    double ry = h / 2;
    // center
    double xc = x + rx;
    double yc = y + ry;

    double xs, ys, xe, ye;
    xs = xc + rx * cos (DegToRad(sa));
    xe = xc + rx * cos (DegToRad(ea));
    ys = yc - ry * sin (DegToRad(sa));
    ye = yc - ry * sin (DegToRad(ea));

    ///now same as circle arc...

    double theta1 = atan2(ys-yc, xs-xc);
    double theta2 = atan2(ye-yc, xe-xc);

    int fArc;                  // flag for large or small arc 0 means less than 180 degrees
    if ( (theta2 - theta1) > 0 ) fArc = 1; else fArc = 0;

    int fSweep;
    if ( fabs(theta2 - theta1) > M_PI) fSweep = 1; else fSweep = 0;

    s.Printf ( wxT("<path d=\"M%d %d A%d %d 0.0 %d %d  %d %d L %d %d z "),
        int(xs), int(ys), int(rx), int(ry),
        fArc, fSweep, int(xe), int(ye), int(xc), int(yc)  );

    s += wxT(" \" /> \n");

    if (m_OK)
    {
        write(s);
    }
}

void wxSVGFileDCImpl::DoSetClippingRegion( int x,  int y, int width, int height )
{
    wxString svg;

    // End current graphics group to ensure proper xml nesting (e.g. so that
    // graphics can be subsequently changed inside the clipping region)
    svg << "</g>\n"
           "<defs>\n"
           "<clipPath id=\"clip" << m_clipNestingLevel << "\">\n"
           "<rect id=\"cliprect" << m_clipNestingLevel << "\" "
                "x=\"" << x << "\" "
                "y=\"" << y << "\" "
                "width=\"" << width << "\" "
                "height=\"" << height << "\" "
                "style=\"stroke: gray; fill: none;\"/>\n"
           "</clipPath>\n"
           "</defs>\n"
           "<g style=\"clip-path: url(#clip" << m_clipNestingLevel << ");\">\n";

    write(svg);

    // Re-apply current graphics to ensure proper xml nesting
    DoStartNewGraphics();

    m_clipUniqueId++;
    m_clipNestingLevel++;
}

void wxSVGFileDCImpl::DestroyClippingRegion()
{
    wxString svg;

    // End current graphics element to ensure proper xml nesting (e.g. graphics
    // might have been changed inside the clipping region)
    svg << "</g>\n";

    // Close clipping group elements
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#endif
    for ( size_t i = 0; i < m_clipUniqueId; i++ )
    {
        svg << "</g>";
    }
    svg << "\n";

    write(svg);

    // Re-apply current graphics (e.g. brush may have been changed inside one
    // of the clipped regions - that change will have been lost after xml
    // elements for the clipped region have been closed).
    DoStartNewGraphics();

    m_clipUniqueId = 0;
}

void wxSVGFileDCImpl::DoGetTextExtent(const wxString& string, wxCoord *w, wxCoord *h, wxCoord *descent , wxCoord *externalLeading , const wxFont *font) const

{
    wxScreenDC sDC;

    sDC.SetFont (m_font);
    if ( font != NULL ) sDC.SetFont ( *font );
    sDC.GetTextExtent(string, w,  h, descent, externalLeading );
}

wxCoord wxSVGFileDCImpl::GetCharHeight() const
{
    wxScreenDC sDC;
    sDC.SetFont (m_font);

    return sDC.GetCharHeight();

}

wxCoord wxSVGFileDCImpl::GetCharWidth() const
{
    wxScreenDC sDC;
    sDC.SetFont (m_font);

    return sDC.GetCharWidth();
}


// ----------------------------------------------------------
// wxSVGFileDCImpl - set functions
// ----------------------------------------------------------

void wxSVGFileDCImpl::SetBackground( const wxBrush &brush )
{
    m_backgroundBrush = brush;
}


void wxSVGFileDCImpl::SetBackgroundMode( int mode )
{
    m_backgroundMode = mode;
}


void wxSVGFileDCImpl::SetBrush(const wxBrush& brush)
{
    m_brush = brush;

    m_graphics_changed = true;
}


void wxSVGFileDCImpl::SetPen(const wxPen& pen)
{
    // width, color, ends, joins : currently implemented
    // dashes, stipple :  not implemented
    m_pen = pen;

    m_graphics_changed = true;
}

void wxSVGFileDCImpl::NewGraphicsIfNeeded()
{
    if ( !m_graphics_changed )
        return;

    m_graphics_changed = false;

    write(wxS("</g>\n"));

    DoStartNewGraphics();
}

void wxSVGFileDCImpl::DoStartNewGraphics()
{
    wxString s, sBrush, sPenCap, sPenJoin, sPenStyle, sLast;

    sBrush = wxS("<g style=\"") + wxBrushString ( m_brush.GetColour(), m_brush.GetStyle() )
            + wxPenString(m_pen.GetColour(), m_pen.GetStyle());

    switch ( m_pen.GetCap() )
    {
        case  wxCAP_PROJECTING :
            sPenCap = wxT("stroke-linecap:square; ");
            break;
        case  wxCAP_BUTT :
            sPenCap = wxT("stroke-linecap:butt; ");
            break;
        case    wxCAP_ROUND :
        default :
            sPenCap = wxT("stroke-linecap:round; ");
    }

    switch ( m_pen.GetJoin() )
    {
        case  wxJOIN_BEVEL :
            sPenJoin = wxT("stroke-linejoin:bevel; ");
            break;
        case  wxJOIN_MITER :
            sPenJoin = wxT("stroke-linejoin:miter; ");
            break;
        case    wxJOIN_ROUND :
        default :
            sPenJoin = wxT("stroke-linejoin:round; ");
    }

    sLast.Printf( wxT("stroke-width:%d\" \n   transform=\"translate(%s %s) scale(%s %s)\">"),
                m_pen.GetWidth(), NumStr(m_logicalOriginX), NumStr(m_logicalOriginY), NumStr(m_scaleX), NumStr(m_scaleY)  );

    s = sBrush + sPenCap + sPenJoin + sPenStyle + sLast + wxT("\n");
    write(s);
}


void wxSVGFileDCImpl::SetFont(const wxFont& font)

{
    m_font = font;
}

// export a bitmap as a raster image in png
bool wxSVGFileDCImpl::DoBlit(wxCoord xdest, wxCoord ydest, wxCoord width, wxCoord height,
                        wxDC* source, wxCoord xsrc, wxCoord ysrc,
                        wxRasterOperationMode logicalFunc /*= wxCOPY*/, bool useMask /*= false*/,
                        wxCoord /*xsrcMask = -1*/, wxCoord /*ysrcMask = -1*/)
{
    if (logicalFunc != wxCOPY)
    {
        wxASSERT_MSG(false, wxT("wxSVGFileDC::DoBlit Call requested nonCopy mode; this is not possible"));
        return false;
    }
    if (useMask != false)
    {
        wxASSERT_MSG(false, wxT("wxSVGFileDC::DoBlit Call requested false mask; this is not possible"));
        return false;
    }
    wxBitmap myBitmap (width, height);
    wxMemoryDC memDC;
    memDC.SelectObject( myBitmap );
    memDC.Blit(0, 0, width, height, source, xsrc, ysrc);
    memDC.SelectObject( wxNullBitmap );
    DoDrawBitmap(myBitmap, xdest, ydest);
    return false;
}

void wxSVGFileDCImpl::DoDrawIcon(const class wxIcon & myIcon, wxCoord x, wxCoord y)
{
    wxBitmap myBitmap (myIcon.GetWidth(), myIcon.GetHeight() );
    wxMemoryDC memDC;
    memDC.SelectObject( myBitmap );
    memDC.DrawIcon(myIcon,0,0);
    memDC.SelectObject( wxNullBitmap );
    DoDrawBitmap(myBitmap, x, y);
}

void wxSVGFileDCImpl::DoDrawBitmap(const class wxBitmap & bmp, wxCoord x, wxCoord y , bool  WXUNUSED(bTransparent) /*=0*/ )
{
    NewGraphicsIfNeeded();

    wxString sTmp, s, sPNG;
    if ( wxImage::FindHandler(wxBITMAP_TYPE_PNG) == NULL )
        wxImage::AddHandler(new wxPNGHandler);

// create suitable file name
    sTmp.Printf ( wxT("_image%d.png"), m_sub_images);
    sPNG = m_filename.BeforeLast(wxT('.')) + sTmp;
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#endif
    while (wxFile::Exists(sPNG) )
    {
        m_sub_images ++;
        sTmp.Printf ( wxT("_image%d.png"), m_sub_images);
        sPNG = m_filename.BeforeLast(wxT('.')) + sTmp;
    }

//create copy of bitmap (wxGTK doesn't like saving a constant bitmap)
    wxBitmap myBitmap = bmp;
//save it
    bool bPNG_OK = myBitmap.SaveFile(sPNG,wxBITMAP_TYPE_PNG);

// reference the bitmap from the SVG doc
// only use filename & ext
    sPNG = sPNG.AfterLast(wxFileName::GetPathSeparator());

// reference the bitmap from the SVG doc
    int w = myBitmap.GetWidth();
    int h = myBitmap.GetHeight();
    sTmp.Printf ( wxT(" <image x=\"%d\" y=\"%d\" width=\"%dpx\" height=\"%dpx\" "), x,y,w,h );
    s += sTmp;
    sTmp.Printf ( wxT(" xlink:href=\"%s\"> \n"), sPNG.c_str() );
    s += sTmp + wxT("<title>Image from wxSVG</title>  </image>") + wxT("\n");

    if (m_OK && bPNG_OK)
    {
        write(s);
    }
    m_OK = m_outfile->IsOk() && bPNG_OK;
}

void wxSVGFileDCImpl::write(const wxString &s)
{
    const wxCharBuffer buf = s.utf8_str();
    m_outfile->Write(buf, strlen((const char *)buf));
    m_OK = m_outfile->IsOk();
}


#ifdef __BORLANDC__
#pragma warn .rch
#pragma warn .ccc
#endif

#endif // wxUSE_SVG

