/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/generic/dcpsg.cpp
// Purpose:     Generic wxPostScriptDC implementation
// Author:      Julian Smart, Robert Roebling, Markus Holzhem
// Modified by:
// Created:     04/01/98
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_PRINTING_ARCHITECTURE && wxUSE_POSTSCRIPT

#include "wx/generic/dcpsg.h"

#ifndef WX_PRECOMP
    #include "wx/intl.h"
    #include "wx/log.h"
    #include "wx/utils.h"
    #include "wx/dcmemory.h"
    #include "wx/math.h"
    #include "wx/image.h"
    #include "wx/icon.h"
#endif // WX_PRECOMP

#include "wx/prntbase.h"
#include "wx/generic/prntdlgg.h"
#include "wx/paper.h"
#include "wx/filename.h"
#include "wx/stdpaths.h"

#ifdef __WXMSW__

#ifdef DrawText
#undef DrawText
#endif

#ifdef StartDoc
#undef StartDoc
#endif

#ifdef GetCharWidth
#undef GetCharWidth
#endif

#ifdef FindWindow
#undef FindWindow
#endif

#endif

//-----------------------------------------------------------------------------
// start and end of document/page
//-----------------------------------------------------------------------------

static const char *wxPostScriptHeaderConicTo = "\
/conicto {\n\
    /to_y exch def\n\
    /to_x exch def\n\
    /conic_cntrl_y exch def\n\
    /conic_cntrl_x exch def\n\
    currentpoint\n\
    /p0_y exch def\n\
    /p0_x exch def\n\
    /p1_x p0_x conic_cntrl_x p0_x sub 2 3 div mul add def\n\
    /p1_y p0_y conic_cntrl_y p0_y sub 2 3 div mul add def\n\
    /p2_x p1_x to_x p0_x sub 1 3 div mul add def\n\
    /p2_y p1_y to_y p0_y sub 1 3 div mul add def\n\
    p1_x p1_y p2_x p2_y to_x to_y curveto\n\
}  bind def\n\
";

static const char *wxPostScriptHeaderEllipse = "\
/ellipsedict 8 dict def\n\
ellipsedict /mtrx matrix put\n\
/ellipse {\n\
    ellipsedict begin\n\
    /endangle exch def\n\
    /startangle exch def\n\
    /yrad exch def\n\
    /xrad exch def\n\
    /y exch def\n\
    /x exch def\n\
    /savematrix mtrx currentmatrix def\n\
    x y translate\n\
    xrad yrad scale\n\
    0 0 1 startangle endangle arc\n\
    savematrix setmatrix\n\
    end\n\
    } def\n\
";

static const char *wxPostScriptHeaderEllipticArc= "\
/ellipticarcdict 8 dict def\n\
ellipticarcdict /mtrx matrix put\n\
/ellipticarc\n\
{ ellipticarcdict begin\n\
  /do_fill exch def\n\
  /endangle exch def\n\
  /startangle exch def\n\
  /yrad exch def\n\
  /xrad exch def \n\
  /y exch def\n\
  /x exch def\n\
  /savematrix mtrx currentmatrix def\n\
  x y translate\n\
  xrad yrad scale\n\
  do_fill { 0 0 moveto } if\n\
  0 0 1 startangle endangle arc\n\
  savematrix setmatrix\n\
  do_fill { fill }{ stroke } ifelse\n\
  end\n\
} def\n";

static const char *wxPostScriptHeaderSpline = "\
/DrawSplineSection {\n\
    /y3 exch def\n\
    /x3 exch def\n\
    /y2 exch def\n\
    /x2 exch def\n\
    /y1 exch def\n\
    /x1 exch def\n\
    /xa x1 x2 x1 sub 0.666667 mul add def\n\
    /ya y1 y2 y1 sub 0.666667 mul add def\n\
    /xb x3 x2 x3 sub 0.666667 mul add def\n\
    /yb y3 y2 y3 sub 0.666667 mul add def\n\
    x1 y1 lineto\n\
    xa ya xb yb x3 y3 curveto\n\
    } def\n\
";

static const char *wxPostScriptHeaderColourImage = "\
% define 'colorimage' if it isn't defined\n\
%   ('colortogray' and 'mergeprocs' come from xwd2ps\n\
%     via xgrab)\n\
/colorimage where   % do we know about 'colorimage'?\n\
  { pop }           % yes: pop off the 'dict' returned\n\
  {                 % no:  define one\n\
    /colortogray {  % define an RGB->I function\n\
      /rgbdata exch store    % call input 'rgbdata'\n\
      rgbdata length 3 idiv\n\
      /npixls exch store\n\
      /rgbindx 0 store\n\
      0 1 npixls 1 sub {\n\
        grays exch\n\
        rgbdata rgbindx       get 20 mul    % Red\n\
        rgbdata rgbindx 1 add get 32 mul    % Green\n\
        rgbdata rgbindx 2 add get 12 mul    % Blue\n\
        add add 64 idiv      % I = .5G + .31R + .18B\n\
        put\n\
        /rgbindx rgbindx 3 add store\n\
      } for\n\
      grays 0 npixls getinterval\n\
    } bind def\n\
\n\
    % Utility procedure for colorimage operator.\n\
    % This procedure takes two procedures off the\n\
    % stack and merges them into a single procedure.\n\
\n\
    /mergeprocs { % def\n\
      dup length\n\
      3 -1 roll\n\
      dup\n\
      length\n\
      dup\n\
      5 1 roll\n\
      3 -1 roll\n\
      add\n\
      array cvx\n\
      dup\n\
      3 -1 roll\n\
      0 exch\n\
      putinterval\n\
      dup\n\
      4 2 roll\n\
      putinterval\n\
    } bind def\n\
\n\
    /colorimage { % def\n\
      pop pop     % remove 'false 3' operands\n\
      {colortogray} mergeprocs\n\
      image\n\
    } bind def\n\
  } ifelse          % end of 'false' case\n\
";

static const char wxPostScriptHeaderReencodeISO1[] =
    "\n/reencodeISO {\n"
"dup dup findfont dup length dict begin\n"
"{ 1 index /FID ne { def }{ pop pop } ifelse } forall\n"
"/Encoding ISOLatin1Encoding def\n"
"currentdict end definefont\n"
"} def\n"
"/ISOLatin1Encoding [\n"
"/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef\n"
"/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef\n"
"/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef\n"
"/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef\n"
"/space/exclam/quotedbl/numbersign/dollar/percent/ampersand/quoteright\n"
"/parenleft/parenright/asterisk/plus/comma/minus/period/slash\n"
"/zero/one/two/three/four/five/six/seven/eight/nine/colon/semicolon\n"
"/less/equal/greater/question/at/A/B/C/D/E/F/G/H/I/J/K/L/M/N\n"
"/O/P/Q/R/S/T/U/V/W/X/Y/Z/bracketleft/backslash/bracketright\n"
"/asciicircum/underscore/quoteleft/a/b/c/d/e/f/g/h/i/j/k/l/m\n"
"/n/o/p/q/r/s/t/u/v/w/x/y/z/braceleft/bar/braceright/asciitilde\n"
"/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef\n"
"/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef/.notdef\n"
"/.notdef/dotlessi/grave/acute/circumflex/tilde/macron/breve\n"
"/dotaccent/dieresis/.notdef/ring/cedilla/.notdef/hungarumlaut\n";

static const char wxPostScriptHeaderReencodeISO2[] =
"/ogonek/caron/space/exclamdown/cent/sterling/currency/yen/brokenbar\n"
"/section/dieresis/copyright/ordfeminine/guillemotleft/logicalnot\n"
"/hyphen/registered/macron/degree/plusminus/twosuperior/threesuperior\n"
"/acute/mu/paragraph/periodcentered/cedilla/onesuperior/ordmasculine\n"
"/guillemotright/onequarter/onehalf/threequarters/questiondown\n"
"/Agrave/Aacute/Acircumflex/Atilde/Adieresis/Aring/AE/Ccedilla\n"
"/Egrave/Eacute/Ecircumflex/Edieresis/Igrave/Iacute/Icircumflex\n"
"/Idieresis/Eth/Ntilde/Ograve/Oacute/Ocircumflex/Otilde/Odieresis\n"
"/multiply/Oslash/Ugrave/Uacute/Ucircumflex/Udieresis/Yacute\n"
"/Thorn/germandbls/agrave/aacute/acircumflex/atilde/adieresis\n"
"/aring/ae/ccedilla/egrave/eacute/ecircumflex/edieresis/igrave\n"
"/iacute/icircumflex/idieresis/eth/ntilde/ograve/oacute/ocircumflex\n"
"/otilde/odieresis/divide/oslash/ugrave/uacute/ucircumflex/udieresis\n"
"/yacute/thorn/ydieresis\n"
        "] def\n\n";

//-------------------------------------------------------------------------------
// wxPostScriptDC
//-------------------------------------------------------------------------------


wxIMPLEMENT_DYNAMIC_CLASS(wxPostScriptDC, wxDC);

wxPostScriptDC::wxPostScriptDC()
              : wxDC(new wxPostScriptDCImpl(this))
{
}

wxPostScriptDC::wxPostScriptDC(const wxPrintData& printData)
              : wxDC(new wxPostScriptDCImpl(this, printData))
{
}

// we don't want to use only 72 dpi from PS print
static const int DPI = 600;
static const double PS2DEV = 600.0 / 72.0;
static const double DEV2PS = 72.0 / 600.0;

#define XLOG2DEV(x)     ((double)(LogicalToDeviceX(x)) * DEV2PS)
#define XLOG2DEVREL(x)  ((double)(LogicalToDeviceXRel(x)) * DEV2PS)
#define YLOG2DEV(x)     ((m_pageHeight - (double)LogicalToDeviceY(x)) * DEV2PS)
#define YLOG2DEVREL(x)  ((double)(LogicalToDeviceYRel(x)) * DEV2PS)


wxIMPLEMENT_ABSTRACT_CLASS(wxPostScriptDCImpl, wxDCImpl);

//-------------------------------------------------------------------------------

wxPostScriptDCImpl::wxPostScriptDCImpl( wxPostScriptDC *owner ) :
   wxDCImpl( owner )
{
    Init();

    m_pageHeight = 842 * PS2DEV;

    m_ok = true;
}

wxPostScriptDCImpl::wxPostScriptDCImpl( wxPostScriptDC *owner, const wxPrintData& data ) :
   wxDCImpl( owner )
{
    Init();

    // this calculates m_pageHeight required for
    // taking the inverted Y axis into account
    SetPrintData( data );

    m_ok = true;
}


wxPostScriptDCImpl::wxPostScriptDCImpl( wxPrinterDC *owner ) :
   wxDCImpl( owner )
{
    Init();

    m_pageHeight = 842 * PS2DEV;

    m_ok = true;
}

wxPostScriptDCImpl::wxPostScriptDCImpl( wxPrinterDC *owner, const wxPrintData& data ) :
   wxDCImpl( owner )
{
    Init();

    // this calculates m_pageHeight required for
    // taking the inverted Y axis into account
    SetPrintData( data );

    m_ok = true;
}

void wxPostScriptDCImpl::Init()
{
    m_pstream = NULL;

    m_currentRed = 0;
    m_currentGreen = 0;
    m_currentBlue = 0;

    m_pageNumber = 0;

    m_clipping = false;

    m_underlinePosition = 0.0;
    m_underlineThickness = 0.0;

}

wxPostScriptDCImpl::~wxPostScriptDCImpl ()
{
    if (m_pstream)
    {
        fclose( m_pstream );
        m_pstream = NULL;
    }
}

bool wxPostScriptDCImpl::IsOk() const
{
  return m_ok;
}

wxRect wxPostScriptDCImpl::GetPaperRect() const
{
    int w = 0;
    int h = 0;
    DoGetSize( &w, &h );
    return wxRect(0,0,w,h);
}

int wxPostScriptDCImpl::GetResolution() const
{
    return DPI;
}

void wxPostScriptDCImpl::DoSetClippingRegion (wxCoord x, wxCoord y, wxCoord w, wxCoord h)
{
    wxCHECK_RET( m_ok , wxT("invalid postscript dc") );

    if (m_clipping)
        DestroyClippingRegion();

    m_clipX1 = x;
    m_clipY1 = y;
    m_clipX2 = x + w;
    m_clipY2 = y + h;

    m_clipping = true;

    wxString buffer;
    buffer.Printf( "gsave\n"
                   "newpath\n"
                   "%f %f moveto\n"
                   "%f %f lineto\n"
                   "%f %f lineto\n"
                   "%f %f lineto\n"
                   "closepath clip newpath\n",
            XLOG2DEV(x),   YLOG2DEV(y),
            XLOG2DEV(x+w), YLOG2DEV(y),
            XLOG2DEV(x+w), YLOG2DEV(y+h),
            XLOG2DEV(x),   YLOG2DEV(y+h) );
    buffer.Replace( ",", "." );
    PsPrint( buffer );
}


void wxPostScriptDCImpl::DestroyClippingRegion()
{
    wxCHECK_RET( m_ok , wxT("invalid postscript dc") );

    if (m_clipping)
    {
        m_clipping = false;
        PsPrint( "grestore\n" );
    }

    wxDCImpl::DestroyClippingRegion();
}

void wxPostScriptDCImpl::Clear()
{
    // This should fail silently to avoid unnecessary
    // asserts
    //    wxFAIL_MSG( wxT("wxPostScriptDCImpl::Clear not implemented.") );
}

bool wxPostScriptDCImpl::DoFloodFill (wxCoord WXUNUSED(x), wxCoord WXUNUSED(y), const wxColour &WXUNUSED(col), wxFloodFillStyle WXUNUSED(style))
{
    wxFAIL_MSG( wxT("wxPostScriptDCImpl::FloodFill not implemented.") );
    return false;
}

bool wxPostScriptDCImpl::DoGetPixel (wxCoord WXUNUSED(x), wxCoord WXUNUSED(y), wxColour * WXUNUSED(col)) const
{
    wxFAIL_MSG( wxT("wxPostScriptDCImpl::GetPixel not implemented.") );
    return false;
}

void wxPostScriptDCImpl::DoCrossHair (wxCoord WXUNUSED(x), wxCoord WXUNUSED(y))
{
    wxFAIL_MSG( wxT("wxPostScriptDCImpl::CrossHair not implemented.") );
}

void wxPostScriptDCImpl::DoDrawLine (wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2)
{
    wxCHECK_RET( m_ok, wxT("invalid postscript dc") );

    if ( m_pen.IsTransparent() )
        return;

    SetPen( m_pen );

    wxString buffer;
    buffer.Printf( "newpath\n"
                   "%f %f moveto\n"
                   "%f %f lineto\n"
                   "stroke\n",
            XLOG2DEV(x1), YLOG2DEV(y1),
            XLOG2DEV(x2), YLOG2DEV(y2) );
    buffer.Replace( ",", "." );
    PsPrint( buffer );

    CalcBoundingBox( x1, y1 );
    CalcBoundingBox( x2, y2 );
}

void wxPostScriptDCImpl::DoDrawArc (wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2, wxCoord xc, wxCoord yc)
{
    wxCHECK_RET( m_ok, wxT("invalid postscript dc") );

    wxCoord dx = x1 - xc;
    wxCoord dy = y1 - yc;
    double radius = sqrt( (double)(dx*dx+dy*dy) );
    double alpha1, alpha2;

    if (x1 == x2 && y1 == y2)
    {
        alpha1 = 0.0;
        alpha2 = 360.0;
    }
    else if ( wxIsNullDouble(radius) )
    {
        alpha1 =
        alpha2 = 0.0;
    }
    else
    {
        alpha1 = (x1 - xc == 0) ?
            (y1 - yc < 0) ? 90.0 : -90.0 :
                wxRadToDeg(-atan2(double(y1-yc), double(x1-xc)));
        alpha2 = (x2 - xc == 0) ?
            (y2 - yc < 0) ? 90.0 : -90.0 :
                wxRadToDeg(-atan2(double(y2-yc), double(x2-xc)));
    }
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
    while (alpha1 <= 0)   alpha1 += 360;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
    while (alpha2 <= 0)   alpha2 += 360; // adjust angles to be between
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
    while (alpha1 > 360)  alpha1 -= 360; // 0 and 360 degree
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
    while (alpha2 > 360)  alpha2 -= 360;

    int i_radius = wxRound( radius );

    if ( m_brush.IsNonTransparent() )
    {
        SetBrush( m_brush );

        wxString buffer;
        buffer.Printf( "newpath\n"
                       "%f %f %f %f %f %f ellipse\n"
                       "%f %f lineto\n"
                       "closepath\n"
                       "fill\n",
                XLOG2DEV(xc), YLOG2DEV(yc),
                XLOG2DEVREL(i_radius), YLOG2DEVREL(i_radius),
                alpha1, alpha2,
                XLOG2DEV(xc), YLOG2DEV(yc) );
        buffer.Replace( ",", "." );
        PsPrint( buffer );

        CalcBoundingBox( xc-i_radius, yc-i_radius );
        CalcBoundingBox( xc+i_radius, yc+i_radius );
    }

    if ( m_pen.IsNonTransparent() )
    {
        SetPen( m_pen );

        wxString buffer;
        buffer.Printf( "newpath\n"
                       "%f %f %f %f %f %f ellipse\n"
                       "stroke\n",
                XLOG2DEV(xc), YLOG2DEV(yc),
                XLOG2DEVREL(i_radius), YLOG2DEVREL(i_radius),
                alpha1, alpha2 );
        buffer.Replace( ",", "." );
        PsPrint( buffer );

        CalcBoundingBox( xc-i_radius, yc-i_radius );
        CalcBoundingBox( xc+i_radius, yc+i_radius );
    }
}

void wxPostScriptDCImpl::DoDrawEllipticArc(wxCoord x,wxCoord y,wxCoord w,wxCoord h,double sa,double ea)
{
    wxCHECK_RET( m_ok, wxT("invalid postscript dc") );

    if ( sa >= 360 || sa <= -360 )
        sa -= int(sa/360)*360;
    if ( ea >= 360 || ea <=- 360 )
        ea -= int(ea/360)*360;
    if ( sa < 0 )
        sa += 360;
    if ( ea < 0 )
        ea += 360;

    if ( wxIsSameDouble(sa, ea) )
    {
        DoDrawEllipse(x,y,w,h);
        return;
    }

    if ( m_brush.IsNonTransparent() )
    {
        SetBrush( m_brush );

        wxString buffer;
        buffer.Printf( "newpath\n"
                       "%f %f %f %f %f %f true ellipticarc\n",
                  XLOG2DEV(x+w/2), YLOG2DEV(y+h/2),
                  XLOG2DEVREL(w/2), YLOG2DEVREL(h/2),
                  sa, ea );
        buffer.Replace( ",", "." );
        PsPrint( buffer );

        CalcBoundingBox( x ,y );
        CalcBoundingBox( x+w, y+h );
    }

    if ( m_pen.IsNonTransparent() )
    {
        SetPen( m_pen );

        wxString buffer;
        buffer.Printf( "newpath\n"
                       "%f %f %f %f %f %f false ellipticarc\n",
                  XLOG2DEV(x+w/2), YLOG2DEV(y+h/2),
                  XLOG2DEVREL(w/2), YLOG2DEVREL(h/2),
                  sa, ea );
        buffer.Replace( ",", "." );
        PsPrint( buffer );

        CalcBoundingBox( x ,y );
        CalcBoundingBox( x+w, y+h );
    }
}

void wxPostScriptDCImpl::DoDrawPoint (wxCoord x, wxCoord y)
{
    wxCHECK_RET( m_ok, wxT("invalid postscript dc") );

    if ( m_pen.IsTransparent() )
        return;

    SetPen (m_pen);

    wxString buffer;
    buffer.Printf( "newpath\n"
                   "%f %f moveto\n"
                   "%f %f lineto\n"
                   "stroke\n",
            XLOG2DEV(x),   YLOG2DEV(y),
            XLOG2DEV(x+1), YLOG2DEV(y) );
    buffer.Replace( ",", "." );
    PsPrint( buffer );

    CalcBoundingBox( x, y );
}

void wxPostScriptDCImpl::DoDrawPolygon (int n, const wxPoint points[], wxCoord xoffset, wxCoord yoffset, wxPolygonFillMode fillStyle)
{
    wxCHECK_RET( m_ok, wxT("invalid postscript dc") );

    if (n <= 0) return;

    if ( m_brush.IsNonTransparent() )
    {
        SetBrush( m_brush );

        PsPrint( "newpath\n" );

        double xx = XLOG2DEV(points[0].x + xoffset);
        double yy = YLOG2DEV(points[0].y + yoffset);

        wxString buffer;
        buffer.Printf( "%f %f moveto\n", xx, yy );
        buffer.Replace( ",", "." );
        PsPrint( buffer );

        CalcBoundingBox( points[0].x + xoffset, points[0].y + yoffset );

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
        for (int i = 1; i < n; i++)
        {
            xx = XLOG2DEV(points[i].x + xoffset);
            yy = YLOG2DEV(points[i].y + yoffset);

            buffer.Printf( "%f %f lineto\n", xx, yy );
            buffer.Replace( ",", "." );
            PsPrint( buffer );

            CalcBoundingBox( points[i].x + xoffset, points[i].y + yoffset);
        }

        PsPrint( (fillStyle == wxODDEVEN_RULE ? "eofill\n" : "fill\n") );
    }

    if ( m_pen.IsNonTransparent() )
    {
        SetPen( m_pen );

        PsPrint( "newpath\n" );

        double xx = XLOG2DEV(points[0].x + xoffset);
        double yy = YLOG2DEV(points[0].y + yoffset);

        wxString buffer;
        buffer.Printf( "%f %f moveto\n", xx, yy );
        buffer.Replace( ",", "." );
        PsPrint( buffer );

        CalcBoundingBox( points[0].x + xoffset, points[0].y + yoffset );

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
        for (int i = 1; i < n; i++)
        {
            xx = XLOG2DEV(points[i].x + xoffset);
            yy = YLOG2DEV(points[i].y + yoffset);

            buffer.Printf( "%f %f lineto\n", xx, yy );
            buffer.Replace( ",", "." );
            PsPrint( buffer );

            CalcBoundingBox( points[i].x + xoffset, points[i].y + yoffset);
        }

        PsPrint( "closepath\n" );
        PsPrint( "stroke\n" );
    }
}

void wxPostScriptDCImpl::DoDrawPolyPolygon (int n, const int count[], const wxPoint points[], wxCoord xoffset, wxCoord yoffset, wxPolygonFillMode fillStyle)
{
    wxCHECK_RET( m_ok, wxT("invalid postscript dc") );

    if (n <= 0) return;

    if ( m_brush.IsNonTransparent() )
    {
        SetBrush( m_brush );

        PsPrint( "newpath\n" );

        int ofs = 0;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
        for (int i = 0; i < n; ofs += count[i++])
        {
            double xx = XLOG2DEV(points[ofs].x + xoffset);
            double yy = YLOG2DEV(points[ofs].y + yoffset);

            wxString buffer;
            buffer.Printf( "%f %f moveto\n", xx, yy );
            buffer.Replace( ",", "." );
            PsPrint( buffer );

            CalcBoundingBox( points[ofs].x + xoffset, points[ofs].y + yoffset );

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
            for (int j = 1; j < count[i]; j++)
            {
                xx = XLOG2DEV(points[ofs+j].x + xoffset);
                yy = YLOG2DEV(points[ofs+j].y + yoffset);

                buffer.Printf( "%f %f lineto\n", xx, yy );
                buffer.Replace( ",", "." );
                PsPrint( buffer );

                CalcBoundingBox( points[ofs+j].x + xoffset, points[ofs+j].y + yoffset);
            }
        }
        PsPrint( (fillStyle == wxODDEVEN_RULE ? "eofill\n" : "fill\n") );
    }

    if ( m_pen.IsNonTransparent() )
    {
        SetPen( m_pen );

        PsPrint( "newpath\n" );

        int ofs = 0;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
        for (int i = 0; i < n; ofs += count[i++])
        {
            double xx = XLOG2DEV(points[ofs].x + xoffset);
            double yy = YLOG2DEV(points[ofs].y + yoffset);

            wxString buffer;
            buffer.Printf( "%f %f moveto\n", xx, yy );
            buffer.Replace( ",", "." );
            PsPrint( buffer );

            CalcBoundingBox( points[ofs].x + xoffset, points[ofs].y + yoffset );

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
            for (int j = 1; j < count[i]; j++)
            {
                xx = XLOG2DEV(points[ofs+j].x + xoffset);
                yy = YLOG2DEV(points[ofs+j].y + yoffset);

                buffer.Printf( "%f %f lineto\n", xx, yy );
                buffer.Replace( ",", "." );
                PsPrint( buffer );

                CalcBoundingBox( points[ofs+j].x + xoffset, points[ofs+j].y + yoffset);
            }
        }
        PsPrint( "closepath\n" );
        PsPrint( "stroke\n" );
    }
}

void wxPostScriptDCImpl::DoDrawLines (int n, const wxPoint points[], wxCoord xoffset, wxCoord yoffset)
{
    wxCHECK_RET( m_ok, wxT("invalid postscript dc") );

    if ( m_pen.IsTransparent() )
        return;

    if (n <= 0) return;

    SetPen (m_pen);

    int i;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
    for ( i =0; i<n ; i++ )
        CalcBoundingBox( points[i].x+xoffset, points[i].y+yoffset );

    wxString buffer;
    buffer.Printf( "newpath\n"
                   "%f %f moveto\n",
              XLOG2DEV(points[0].x+xoffset),
              YLOG2DEV(points[0].y+yoffset) );
    buffer.Replace( ",", "." );
    PsPrint( buffer );

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
    for (i = 1; i < n; i++)
    {
        buffer.Printf( "%f %f lineto\n",
                  XLOG2DEV(points[i].x+xoffset),
                  YLOG2DEV(points[i].y+yoffset) );
        buffer.Replace( ",", "." );
        PsPrint( buffer );
    }

    PsPrint( "stroke\n" );
}

void wxPostScriptDCImpl::DoDrawRectangle (wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
    wxCHECK_RET( m_ok, wxT("invalid postscript dc") );

    width--;
    height--;

    if ( m_brush.IsNonTransparent() )
    {
        SetBrush( m_brush );

        wxString buffer;
        buffer.Printf( "newpath\n"
                       "%f %f moveto\n"
                       "%f %f lineto\n"
                       "%f %f lineto\n"
                       "%f %f lineto\n"
                       "closepath\n"
                       "fill\n",
                XLOG2DEV(x),         YLOG2DEV(y),
                XLOG2DEV(x + width), YLOG2DEV(y),
                XLOG2DEV(x + width), YLOG2DEV(y + height),
                XLOG2DEV(x),         YLOG2DEV(y + height) );
        buffer.Replace( ",", "." );
        PsPrint( buffer );

        CalcBoundingBox( x, y );
        CalcBoundingBox( x + width, y + height );
    }

    if ( m_pen.IsNonTransparent() )
    {
        SetPen (m_pen);

        wxString buffer;
        buffer.Printf( "newpath\n"
                       "%f %f moveto\n"
                       "%f %f lineto\n"
                       "%f %f lineto\n"
                       "%f %f lineto\n"
                       "closepath\n"
                       "stroke\n",
                XLOG2DEV(x),         YLOG2DEV(y),
                XLOG2DEV(x + width), YLOG2DEV(y),
                XLOG2DEV(x + width), YLOG2DEV(y + height),
                XLOG2DEV(x),         YLOG2DEV(y + height) );
        buffer.Replace( ",", "." );
        PsPrint( buffer );

        CalcBoundingBox( x, y );
        CalcBoundingBox( x + width, y + height );
    }
}

void wxPostScriptDCImpl::DoDrawRoundedRectangle (wxCoord x, wxCoord y, wxCoord width, wxCoord height, double radius)
{
    wxCHECK_RET( m_ok, wxT("invalid postscript dc") );

    width--;
    height--;

    if (radius < 0.0)
    {
        // Now, a negative radius is interpreted to mean
        // 'the proportion of the smallest X or Y dimension'
        double smallest = width < height ? width : height;
        radius =  (-radius * smallest);
    }

    wxCoord rad = (wxCoord) radius;

    if ( m_brush.IsNonTransparent() )
    {
        SetBrush( m_brush );

        /* Draw rectangle anticlockwise */
        wxString buffer;
        buffer.Printf( "newpath\n"
                       "%f %f %f 90 180 arc\n"
                       "%f %f lineto\n"
                       "%f %f %f 180 270 arc\n"
                       "%f %f lineto\n"
                       "%f %f %f 270 0 arc\n"
                       "%f %f lineto\n"
                       "%f %f %f 0 90 arc\n"
                       "%f %f lineto\n"
                       "closepath\n"
                       "fill\n",
                XLOG2DEV(x + rad), YLOG2DEV(y + rad), XLOG2DEVREL(rad),
                XLOG2DEV(x), YLOG2DEV(y + height - rad),
                XLOG2DEV(x + rad), YLOG2DEV(y + height - rad), XLOG2DEVREL(rad),
                XLOG2DEV(x + width - rad), YLOG2DEV(y + height),
                XLOG2DEV(x + width - rad), YLOG2DEV(y + height - rad), XLOG2DEVREL(rad),
                XLOG2DEV(x + width), YLOG2DEV(y + rad),
                XLOG2DEV(x + width - rad), YLOG2DEV(y + rad), XLOG2DEVREL(rad),
                XLOG2DEV(x + rad), YLOG2DEV(y) );
        buffer.Replace( ",", "." );
        PsPrint( buffer );

        CalcBoundingBox( x, y );
        CalcBoundingBox( x + width, y + height );
    }

    if ( m_pen.IsNonTransparent() )
    {
        SetPen (m_pen);

        /* Draw rectangle anticlockwise */
        wxString buffer;
        buffer.Printf( "newpath\n"
                       "%f %f %f 90 180 arc\n"
                       "%f %f lineto\n"
                       "%f %f %f 180 270 arc\n"
                       "%f %f lineto\n"
                       "%f %f %f 270 0 arc\n"
                       "%f %f lineto\n"
                       "%f %f %f 0 90 arc\n"
                       "%f %f lineto\n"
                       "closepath\n"
                       "stroke\n",
                XLOG2DEV(x + rad), YLOG2DEV(y + rad), XLOG2DEVREL(rad),
                XLOG2DEV(x), YLOG2DEV(y + height - rad),
                XLOG2DEV(x + rad), YLOG2DEV(y + height - rad), XLOG2DEVREL(rad),
                XLOG2DEV(x + width - rad), YLOG2DEV(y + height),
                XLOG2DEV(x + width - rad), YLOG2DEV(y + height - rad), XLOG2DEVREL(rad),
                XLOG2DEV(x + width), YLOG2DEV(y + rad),
                XLOG2DEV(x + width - rad), YLOG2DEV(y + rad), XLOG2DEVREL(rad),
                XLOG2DEV(x + rad), YLOG2DEV(y) );
        buffer.Replace( ",", "." );
        PsPrint( buffer );

        CalcBoundingBox( x, y );
        CalcBoundingBox( x + width, y + height );
    }
}

void wxPostScriptDCImpl::DoDrawEllipse (wxCoord x, wxCoord y, wxCoord width, wxCoord height)
{
    wxCHECK_RET( m_ok, wxT("invalid postscript dc") );

    width--;
    height--;

    if ( m_brush.IsNonTransparent() )
    {
        SetBrush (m_brush);

        wxString buffer;
        buffer.Printf( "newpath\n"
                       "%f %f %f %f 0 360 ellipse\n"
                       "fill\n",
                XLOG2DEV(x + width / 2), YLOG2DEV(y + height / 2),
                XLOG2DEVREL(width / 2), YLOG2DEVREL(height / 2) );
        buffer.Replace( ",", "." );
        PsPrint( buffer );

        CalcBoundingBox( x - width, y - height );
        CalcBoundingBox( x + width, y + height );
    }

    if ( m_pen.IsNonTransparent() )
    {
        SetPen (m_pen);

        wxString buffer;
        buffer.Printf( "newpath\n"
                       "%f %f %f %f 0 360 ellipse\n"
                       "stroke\n",
                XLOG2DEV(x + width / 2), YLOG2DEV(y + height / 2),
                XLOG2DEVREL(width / 2), YLOG2DEVREL(height / 2) );
        buffer.Replace( ",", "." );
        PsPrint( buffer );

        CalcBoundingBox( x - width, y - height );
        CalcBoundingBox( x + width, y + height );
    }
}

void wxPostScriptDCImpl::DoDrawIcon( const wxIcon& icon, wxCoord x, wxCoord y )
{
    DoDrawBitmap( icon, x, y, true );
}

/* this has to be char, not wxChar */
static const char hexArray[] = "0123456789ABCDEF";

void wxPostScriptDCImpl::DoDrawBitmap( const wxBitmap& bitmap, wxCoord x, wxCoord y, bool WXUNUSED(useMask) )
{
    wxCHECK_RET( m_ok, wxT("invalid postscript dc") );

    if (!bitmap.IsOk()) return;

    wxImage image = bitmap.ConvertToImage();

    if (!image.IsOk()) return;

    wxCoord w = image.GetWidth();
    wxCoord h = image.GetHeight();

    double ww = XLOG2DEVREL(image.GetWidth());
    double hh = YLOG2DEVREL(image.GetHeight());

    double xx = XLOG2DEV(x);
    double yy = YLOG2DEV(y + bitmap.GetHeight());

    wxString buffer;
    buffer.Printf( "/origstate save def\n"
                   "20 dict begin\n"
                   "/pix %d string def\n"
                   "/grays %d string def\n"
                   "/npixels 0 def\n"
                   "/rgbindx 0 def\n"
                   "%f %f translate\n"
                   "%f %f scale\n"
                   "%d %d 8\n"
                   "[%d 0 0 %d 0 %d]\n"
                   "{currentfile pix readhexstring pop}\n"
                   "false 3 colorimage\n",
            w, w, xx, yy, ww, hh, w, h, w, -h, h );
    buffer.Replace( ",", "." );
    PsPrint( buffer );

    unsigned char* data = image.GetData();

    // size of the buffer = width*rgb(3)*hexa(2)+'\n'
    wxCharBuffer charbuffer(w*6 + 1);
    int firstDigit, secondDigit;

    //rows
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
    for (int j = 0; j < h; j++)
    {
        char* bufferindex = charbuffer.data();

        //cols
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
        for (int i = 0; i < w*3; i++)
        {
            firstDigit = (int)(*data/16.0);
            secondDigit = (int)(*data - (firstDigit*16.0));
            *(bufferindex++) = hexArray[firstDigit];
            *(bufferindex++) = hexArray[secondDigit];

            data++;
        }
        *(bufferindex++) = '\n';
        *bufferindex = 0;

        if (m_pstream)
            fwrite( charbuffer, 1, strlen( charbuffer ), m_pstream );
        else
            PsPrint( charbuffer );
    }

    PsPrint( "end\n" );
    PsPrint( "origstate restore\n" );
}

void wxPostScriptDCImpl::SetFont( const wxFont& font )
{
    wxCHECK_RET( m_ok, wxT("invalid postscript dc") );

    if (!font.IsOk())  return;

    m_font = font;

    wxFontStyle Style = m_font.GetStyle();
    wxFontWeight Weight = m_font.GetWeight();

    const char *name;
    switch (m_font.GetFamily())
    {
        case wxTELETYPE:
        case wxMODERN:
        {
            if (Style == wxFONTSTYLE_ITALIC)
            {
                if (Weight == wxFONTWEIGHT_BOLD)
                    name = "/Courier-BoldOblique";
                else
                    name = "/Courier-Oblique";
            }
            else
            {
                if (Weight == wxFONTWEIGHT_BOLD)
                    name = "/Courier-Bold";
                else
                    name = "/Courier";
            }
            break;
        }
        case wxROMAN:
        {
            if (Style == wxFONTSTYLE_ITALIC)
            {
                if (Weight == wxFONTWEIGHT_BOLD)
                    name = "/Times-BoldItalic";
                else
                    name = "/Times-Italic";
            }
            else
            {
                if (Weight == wxFONTWEIGHT_BOLD)
                    name = "/Times-Bold";
                else
                    name = "/Times-Roman";
            }
            break;
        }
        case wxSCRIPT:
        {
            name = "/ZapfChancery-MediumItalic";
            break;
        }
        case wxSWISS:
        default:
        {
            if (Style == wxFONTSTYLE_ITALIC)
            {
                if (Weight == wxFONTWEIGHT_BOLD)
                    name = "/Helvetica-BoldOblique";
                else
                    name = "/Helvetica-Oblique";
            }
            else
            {
                if (Weight == wxFONTWEIGHT_BOLD)
                    name = "/Helvetica-Bold";
                else
                    name = "/Helvetica";
            }
            break;
        }
    }

    // We may legitimately call SetFont before BeginDoc
    if (!m_pstream)
        return;

    PsPrint( name );
    PsPrint( " reencodeISO def\n" );
    PsPrint( name );
    PsPrint( " findfont\n" );


    float size = float(m_font.GetPointSize());
    size = size * GetFontPointSizeAdjustment(DPI);
    wxString buffer;
    buffer.Printf( "%f scalefont setfont\n", size * m_scaleX );
    buffer.Replace( ",", "." );
    PsPrint( buffer );
}

void wxPostScriptDCImpl::SetPen( const wxPen& pen )
{
    wxCHECK_RET( m_ok, wxT("invalid postscript dc") );

    if (!pen.IsOk()) return;

    int oldStyle = m_pen.IsOk() ? m_pen.GetStyle() : wxPENSTYLE_INVALID;
    wxPenCap oldCap = m_pen.IsOk() ? m_pen.GetCap() : wxCAP_INVALID;
    wxPenJoin oldJoin = m_pen.IsOk() ? m_pen.GetJoin() : wxJOIN_INVALID;

    m_pen = pen;
    wxPenCap cap = m_pen.IsOk() ? m_pen.GetCap() : wxCAP_INVALID;
    wxPenJoin join = m_pen.IsOk() ? m_pen.GetJoin() : wxJOIN_INVALID;

    double width;

    if (m_pen.GetWidth() <= 0)
        width = 0.1;
    else
        width = (double) m_pen.GetWidth();

    wxString buffer;
    buffer.Printf( "%f setlinewidth\n", width * DEV2PS * m_scaleX );
    buffer.Replace( ",", "." );
    PsPrint( buffer );

/*
     Line style - WRONG: 2nd arg is OFFSET

     Here, I'm afraid you do not conceive meaning of parameters of 'setdash'
     operator correctly. You should look-up this in the Red Book: the 2nd parame-
     ter is not number of values in the array of the first one, but an offset
     into this description of the pattern. I mean a real *offset* not index
     into array. I.e. If the command is [3 4] 1 setdash   is used, then there
     will be first black line *2* units wxCoord, then space 4 units, then the
     pattern of *3* units black, 4 units space will be repeated.
*/

    static const char *dotted = "[2 5] 2";
    static const char *short_dashed = "[4 4] 2";
    static const char *wxCoord_dashed = "[4 8] 2";
    static const char *dotted_dashed = "[6 6 2 6] 4";

    const char *psdash;

    switch (m_pen.GetStyle())
    {
        case wxPENSTYLE_DOT:        psdash = dotted;         break;
        case wxPENSTYLE_SHORT_DASH: psdash = short_dashed;   break;
        case wxPENSTYLE_LONG_DASH:  psdash = wxCoord_dashed; break;
        case wxPENSTYLE_DOT_DASH:   psdash = dotted_dashed;  break;
        case wxPENSTYLE_USER_DASH:
        {
            wxDash *dashes;
            int nDashes = m_pen.GetDashes (&dashes);
            PsPrint ("[");
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
            for (int i = 0; i < nDashes; ++i)
            {
                buffer.Printf( "%d ", dashes [i] );
                PsPrint( buffer );
            }
            PsPrint ("] 0 setdash\n");
            psdash = 0;
        }
        break;
        case wxPENSTYLE_SOLID:
        case wxPENSTYLE_TRANSPARENT:
        default:              psdash = "[] 0";         break;
    }

    if ( psdash && (oldStyle != m_pen.GetStyle()) )
    {
        PsPrint( psdash );
        PsPrint( " setdash\n" );
    }

    if ( cap != wxCAP_INVALID && cap != oldCap )
    {
        switch ( cap )
        {
            case wxCAP_ROUND:      buffer = "1"; break;
            case wxCAP_PROJECTING: buffer = "2"; break;
            case wxCAP_BUTT:       buffer = "0"; break;

            // This case is just to fix compiler warning, this is impossible
            // due to the test above.
            case wxCAP_INVALID: break;
        }
        buffer << " setlinecap\n";
        PsPrint( buffer );
    }

    if ( join != wxJOIN_INVALID && join != oldJoin )
    {
        switch ( join )
        {
            case wxJOIN_BEVEL: buffer = "2"; break;
            case wxJOIN_ROUND: buffer = "1"; break;
            case wxJOIN_MITER: buffer = "0"; break;
            case wxJOIN_INVALID: break;
        }
        buffer << " setlinejoin\n";
        PsPrint( buffer );
    }

    // Line colour
    unsigned char red = m_pen.GetColour().Red();
    unsigned char blue = m_pen.GetColour().Blue();
    unsigned char green = m_pen.GetColour().Green();

    if (!m_colour)
    {
        // Anything not white is black
        if (! (red == (unsigned char) 255 &&
               blue == (unsigned char) 255 &&
               green == (unsigned char) 255) )
        {
            red = (unsigned char) 0;
            green = (unsigned char) 0;
            blue = (unsigned char) 0;
        }
        // setgray here ?
    }

    if (!(red == m_currentRed && green == m_currentGreen && blue == m_currentBlue))
    {
        double redPS = (double)(red) / 255.0;
        double bluePS = (double)(blue) / 255.0;
        double greenPS = (double)(green) / 255.0;

        buffer.Printf( "%f %f %f setrgbcolor\n", redPS, greenPS, bluePS );
        buffer.Replace( ",", "." );
        PsPrint( buffer );

        m_currentRed = red;
        m_currentBlue = blue;
        m_currentGreen = green;
    }
}

void wxPostScriptDCImpl::SetBrush( const wxBrush& brush )
{
    wxCHECK_RET( m_ok, wxT("invalid postscript dc") );

    if (!brush.IsOk()) return;

    m_brush = brush;

    // Brush colour
    unsigned char red = m_brush.GetColour().Red();
    unsigned char blue = m_brush.GetColour().Blue();
    unsigned char green = m_brush.GetColour().Green();

    if (!m_colour)
    {
        // Anything not white is black
        if (! (red == (unsigned char) 255 &&
               blue == (unsigned char) 255 &&
               green == (unsigned char) 255) )
        {
            red = (unsigned char) 0;
            green = (unsigned char) 0;
            blue = (unsigned char) 0;
        }
        // setgray here ?
    }

    if (!(red == m_currentRed && green == m_currentGreen && blue == m_currentBlue))
    {
        double redPS = (double)(red) / 255.0;
        double bluePS = (double)(blue) / 255.0;
        double greenPS = (double)(green) / 255.0;

        wxString buffer;
        buffer.Printf( "%f %f %f setrgbcolor\n", redPS, greenPS, bluePS );
        buffer.Replace( ",", "." );
        PsPrint( buffer );

        m_currentRed = red;
        m_currentBlue = blue;
        m_currentGreen = green;
    }
}

void wxPostScriptDCImpl::DoDrawText( const wxString& text, wxCoord x, wxCoord y )
{
    wxCHECK_RET( m_ok, wxT("invalid postscript dc") );

    const wxWX2MBbuf textbuf = text.mb_str();
    if ( !textbuf )
        return;

    if (m_textForegroundColour.IsOk())
    {
        unsigned char red = m_textForegroundColour.Red();
        unsigned char blue = m_textForegroundColour.Blue();
        unsigned char green = m_textForegroundColour.Green();

        if (!m_colour)
        {
            // Anything not white is black
            if (! (red == (unsigned char) 255 &&
                        blue == (unsigned char) 255 &&
                        green == (unsigned char) 255))
            {
                red = (unsigned char) 0;
                green = (unsigned char) 0;
                blue = (unsigned char) 0;
            }
        }

        // maybe setgray here ?
        if (!(red == m_currentRed && green == m_currentGreen && blue == m_currentBlue))
        {
            double redPS = (double)(red) / 255.0;
            double bluePS = (double)(blue) / 255.0;
            double greenPS = (double)(green) / 255.0;

            wxString buffer;
            buffer.Printf( "%f %f %f setrgbcolor\n", redPS, greenPS, bluePS );
            buffer.Replace( ",", "." );
            PsPrint( buffer );

            m_currentRed = red;
            m_currentBlue = blue;
            m_currentGreen = green;
        }
    }

    wxCoord text_w, text_h, text_descent;

    GetOwner()->GetTextExtent(text, &text_w, &text_h, &text_descent);

    int size = m_font.GetPointSize();

//    wxCoord by = y + (wxCoord)floor( double(size) * 2.0 / 3.0 ); // approximate baseline
//    commented by V. Slavik and replaced by accurate version
//        - note that there is still rounding error in text_descent!
    wxCoord by = y + size - text_descent; // baseline

    wxString buffer;
    buffer.Printf( "%f %f moveto\n", XLOG2DEV(x), YLOG2DEV(by) );
    buffer.Replace( ",", "." );
    PsPrint( buffer );
    PsPrint( "(" );

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
    for ( const char *p = textbuf; *p != '\0'; p++ )
    {
        int c = (unsigned char)*p;
        if (c == ')' || c == '(' || c == '\\')
        {
            /* Cope with special characters */
            PsPrint( "\\" );
            PsPrint( (char) c );
        }
        else if ( c >= 128 )
        {
            /* Cope with character codes > 127 */
            buffer.Printf( "\\%o", c );
            PsPrint( buffer );
        }
        else
        {
            PsPrint( (char) c );
        }
    }

    PsPrint( ") show\n" );

    if (m_font.GetUnderlined())
    {
        wxCoord uy = (wxCoord)(y + size - m_underlinePosition);

        buffer.Printf( "gsave\n"
                       "%f %f moveto\n"
                       "%f setlinewidth\n"
                       "%f %f lineto\n"
                       "stroke\n"
                       "grestore\n",
                XLOG2DEV(x), YLOG2DEV(uy),
                m_underlineThickness,
                XLOG2DEV(x + text_w), YLOG2DEV(uy) );
        buffer.Replace( ",", "." );
        PsPrint( buffer );
    }

    CalcBoundingBox( x, y );
    CalcBoundingBox( x + size * text.length() * 2/3 , y );
}

void wxPostScriptDCImpl::DoDrawRotatedText( const wxString& text, wxCoord x, wxCoord y, double angle )
{
    if ( wxIsNullDouble(angle) )
    {
        DoDrawText(text, x, y);
        return;
    }

    wxCHECK_RET( m_ok, wxT("invalid postscript dc") );

    SetFont( m_font );

    if (m_textForegroundColour.IsOk())
    {
        unsigned char red = m_textForegroundColour.Red();
        unsigned char blue = m_textForegroundColour.Blue();
        unsigned char green = m_textForegroundColour.Green();

        if (!m_colour)
        {
            // Anything not white is black
            if (! (red == (unsigned char) 255 &&
                   blue == (unsigned char) 255 &&
                   green == (unsigned char) 255))
            {
                red = (unsigned char) 0;
                green = (unsigned char) 0;
                blue = (unsigned char) 0;
            }
        }

        // maybe setgray here ?
        if (!(red == m_currentRed && green == m_currentGreen && blue == m_currentBlue))
        {
            double redPS = (double)(red) / 255.0;
            double bluePS = (double)(blue) / 255.0;
            double greenPS = (double)(green) / 255.0;

            wxString buffer;
            buffer.Printf( "%f %f %f setrgbcolor\n", redPS, greenPS, bluePS );
            buffer.Replace( ",", "." );
            PsPrint( buffer );

            m_currentRed = red;
            m_currentBlue = blue;
            m_currentGreen = green;
        }
    }

    int size = m_font.GetPointSize();

    wxString buffer;
    buffer.Printf( "%f %f moveto\n", XLOG2DEV(x), YLOG2DEV(y));
    buffer.Replace( ",", "." );
    PsPrint( buffer );

    buffer.Printf( "%f rotate\n", angle );
    buffer.Replace( ",", "." );
    PsPrint( buffer );

    PsPrint( "(" );
    const wxWX2MBbuf textbuf = text.mb_str();
    if ( textbuf )
    {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
        for ( const char *p = textbuf; *p != '\0'; p++ )
        {
            int c = (unsigned char)*p;
            if (c == ')' || c == '(' || c == '\\')
            {
                /* Cope with special characters */
                PsPrint( "\\" );
                PsPrint( (char) c );
            }
            else if ( c >= 128 )
            {
                /* Cope with character codes > 127 */
                buffer.Printf( "\\%o", c);
                PsPrint( buffer );
            }
            else
            {
                PsPrint( (char) c );
            }
        }
    }

    PsPrint( ") show\n" );

    buffer.Printf( "%f rotate\n", -angle );
    buffer.Replace( ",", "." );
    PsPrint( buffer );

    if (m_font.GetUnderlined())
    {
        wxCoord uy = (wxCoord)(y + size - m_underlinePosition);
        wxCoord w, h;
        GetOwner()->GetTextExtent(text, &w, &h);

        buffer.Printf(
                "gsave\n"
                "%f %f moveto\n"
                "%f setlinewidth\n"
                "%f %f lineto\n"
                "stroke\n"
                "grestore\n",
                XLOG2DEV(x), YLOG2DEV(uy),
                m_underlineThickness,
                XLOG2DEV(x + w), YLOG2DEV(uy) );
        buffer.Replace( ",", "." );
        PsPrint( buffer );
    }

    CalcBoundingBox( x, y );
    CalcBoundingBox( x + size * text.length() * 2/3 , y );
}

void wxPostScriptDCImpl::SetBackground (const wxBrush& brush)
{
    m_backgroundBrush = brush;
}

void wxPostScriptDCImpl::SetLogicalFunction(wxRasterOperationMode WXUNUSED(function))
{
    wxFAIL_MSG( wxT("wxPostScriptDCImpl::SetLogicalFunction not implemented.") );
}

#if wxUSE_SPLINES
void wxPostScriptDCImpl::DoDrawSpline( const wxPointList *points )
{
    wxCHECK_RET( m_ok, wxT("invalid postscript dc") );

    SetPen( m_pen );

    // a and b are not used
    //double a, b;
    double c, d, x1, y1, x2, y2, x3, y3;
    wxPoint *p, *q;

    wxPointList::compatibility_iterator node = points->GetFirst();
    p = node->GetData();
    x1 = p->x;
    y1 = p->y;

    node = node->GetNext();
    p = node->GetData();
    c = p->x;
    d = p->y;
    x3 =
         #if 0
         a =
         #endif
         (double)(x1 + c) / 2;
    y3 =
         #if 0
         b =
         #endif
         (double)(y1 + d) / 2;

    wxString buffer;
    buffer.Printf( "newpath\n"
                   "%f %f moveto\n"
                   "%f %f lineto\n",
            XLOG2DEV(wxRound(x1)), YLOG2DEV(wxRound(y1)),
            XLOG2DEV(wxRound(x3)), YLOG2DEV(wxRound(y3)) );
    buffer.Replace( ",", "." );
    PsPrint( buffer );

    CalcBoundingBox( (wxCoord)x1, (wxCoord)y1 );
    CalcBoundingBox( (wxCoord)x3, (wxCoord)y3 );

    node = node->GetNext();
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
    while (node)
    {
        q = node->GetData();

        x1 = x3;
        y1 = y3;
        x2 = c;
        y2 = d;
        c = q->x;
        d = q->y;
        x3 = (double)(x2 + c) / 2;
        y3 = (double)(y2 + d) / 2;

        buffer.Printf( "%f %f %f %f %f %f DrawSplineSection\n",
            XLOG2DEV(wxRound(x1)), YLOG2DEV(wxRound(y1)),
            XLOG2DEV(wxRound(x2)), YLOG2DEV(wxRound(y2)),
            XLOG2DEV(wxRound(x3)), YLOG2DEV(wxRound(y3)) );
        buffer.Replace( ",", "." );
        PsPrint( buffer );

        CalcBoundingBox( (wxCoord)x1, (wxCoord)y1 );
        CalcBoundingBox( (wxCoord)x3, (wxCoord)y3 );

        node = node->GetNext();
    }

    /*
       At this point, (x2,y2) and (c,d) are the position of the
       next-to-last and last point respectively, in the point list
     */

    buffer.Printf( "%f %f lineto\nstroke\n", XLOG2DEV(wxRound(c)), YLOG2DEV(wxRound(d)) );
    buffer.Replace( ",", "." );
    PsPrint( buffer );
}
#endif // wxUSE_SPLINES

wxCoord wxPostScriptDCImpl::GetCharWidth() const
{
    // Chris Breeze: reasonable approximation using wxMODERN/Courier
    return (wxCoord) (GetCharHeight() * 72.0 / 120.0);
}

void wxPostScriptDCImpl::SetPrintData(const wxPrintData& data)
{
    m_printData = data;

    wxPaperSize id = m_printData.GetPaperId();
    wxPrintPaperType *paper = wxThePrintPaperDatabase->FindPaperType(id);
    if (!paper) paper = wxThePrintPaperDatabase->FindPaperType(wxPAPER_A4);
    int w = 595;
    int h = 842;
    if (paper)
    {
        w = paper->GetSizeDeviceUnits().x;
        h = paper->GetSizeDeviceUnits().y;
    }

    if (m_printData.GetOrientation() == wxLANDSCAPE)
        m_pageHeight = w * PS2DEV;
    else
        m_pageHeight = h * PS2DEV;
}

void wxPostScriptDCImpl::ComputeScaleAndOrigin()
{
    const wxRealPoint origScale(m_scaleX, m_scaleY);

    wxDCImpl::ComputeScaleAndOrigin();

    // If scale has changed call SetPen to recalculate the line width
    // and SetFont to recalculate font size
    if ( wxRealPoint(m_scaleX, m_scaleY) != origScale && m_pen.IsOk() )
    {
        SetPen( m_pen );
        SetFont( m_font  );
    }
}

void wxPostScriptDCImpl::DoGetSize(int* width, int* height) const
{
    wxPaperSize id = m_printData.GetPaperId();

    wxPrintPaperType *paper = wxThePrintPaperDatabase->FindPaperType(id);

    if (!paper) paper = wxThePrintPaperDatabase->FindPaperType(wxPAPER_A4);

    int w = 595;
    int h = 842;
    if (paper)
    {
        w = paper->GetSizeDeviceUnits().x;
        h = paper->GetSizeDeviceUnits().y;
    }

    if (m_printData.GetOrientation() == wxLANDSCAPE)
    {
        int tmp = w;
        w = h;
        h = tmp;
    }

    if (width)
        *width = wxRound( w * PS2DEV );

    if (height)
        *height = wxRound( h * PS2DEV );
}

void wxPostScriptDCImpl::DoGetSizeMM(int *width, int *height) const
{
    wxPaperSize id = m_printData.GetPaperId();

    wxPrintPaperType *paper = wxThePrintPaperDatabase->FindPaperType(id);

    if (!paper) paper = wxThePrintPaperDatabase->FindPaperType(wxPAPER_A4);

    int w = 210;
    int h = 297;
    if (paper)
    {
        w = paper->GetWidth() / 10;
        h = paper->GetHeight() / 10;
    }

    if (m_printData.GetOrientation() == wxLANDSCAPE)
    {
        int tmp = w;
        w = h;
        h = tmp;
    }

    if (width) *width = w;
    if (height) *height = h;
}

// Resolution in pixels per logical inch
wxSize wxPostScriptDCImpl::GetPPI(void) const
{
    return wxSize( DPI, DPI );
}


bool wxPostScriptDCImpl::StartDoc( const wxString& WXUNUSED(message) )
{
    wxCHECK_MSG( m_ok, false, wxT("invalid postscript dc") );

    if (m_printData.GetPrintMode() != wxPRINT_MODE_STREAM )
    {
        if (m_printData.GetFilename() == wxEmptyString)
        {
            wxString filename = wxFileName::CreateTempFileName( wxT("ps") );
            m_printData.SetFilename(filename);
        }

        m_pstream = wxFopen( m_printData.GetFilename(), wxT("w+") );

        if (!m_pstream)
        {
            wxLogError( _("Cannot open file for PostScript printing!"));
            m_ok = false;
            return false;
        }
    }

    m_ok = true;

    wxString buffer;

    PsPrint( "%!PS-Adobe-2.0\n" );

    PsPrint( "%%Creator: wxWidgets PostScript renderer\n" );

    buffer.Printf( "%%%%CreationDate: %s\n", wxNow() );
    PsPrint( buffer );

    if (m_printData.GetOrientation() == wxLANDSCAPE)
        PsPrint( "%%Orientation: Landscape\n" );
    else
        PsPrint( "%%Orientation: Portrait\n" );

    const wxChar *paper;
    switch (m_printData.GetPaperId())
    {
       case wxPAPER_LETTER: paper = wxT("Letter"); break;       // Letter: paper ""; 8 1/2 by 11 inches
       case wxPAPER_LEGAL: paper = wxT("Legal"); break;         // Legal, 8 1/2 by 14 inches
       case wxPAPER_A4: paper = wxT("A4"); break;               // A4 Sheet, 210 by 297 millimeters
       case wxPAPER_TABLOID: paper = wxT("Tabloid"); break;     // Tabloid, 11 by 17 inches
       case wxPAPER_LEDGER: paper = wxT("Ledger"); break;       // Ledger, 17 by 11 inches
       case wxPAPER_STATEMENT: paper = wxT("Statement"); break; // Statement, 5 1/2 by 8 1/2 inches
       case wxPAPER_EXECUTIVE: paper = wxT("Executive"); break; // Executive, 7 1/4 by 10 1/2 inches
       case wxPAPER_A3: paper = wxT("A3"); break;               // A3 sheet, 297 by 420 millimeters
       case wxPAPER_A5: paper = wxT("A5"); break;               // A5 sheet, 148 by 210 millimeters
       case wxPAPER_B4: paper = wxT("B4"); break;               // B4 sheet, 250 by 354 millimeters
       case wxPAPER_B5: paper = wxT("B5"); break;               // B5 sheet, 182-by-257-millimeter paper
       case wxPAPER_FOLIO: paper = wxT("Folio"); break;         // Folio, 8-1/2-by-13-inch paper
       case wxPAPER_QUARTO: paper = wxT("Quaro"); break;        // Quarto, 215-by-275-millimeter paper
       case wxPAPER_10X14: paper = wxT("10x14"); break;         // 10-by-14-inch sheet
       default: paper = wxT("A4");
    }

    buffer.Printf( "%%%%DocumentPaperSizes: %s\n", paper );
    PsPrint( buffer );

    PsPrint( "%%EndComments\n\n" );

    PsPrint( "%%BeginProlog\n" );
    PsPrint( wxPostScriptHeaderConicTo );
    PsPrint( wxPostScriptHeaderEllipse );
    PsPrint( wxPostScriptHeaderEllipticArc );
    PsPrint( wxPostScriptHeaderColourImage );
    PsPrint( wxPostScriptHeaderReencodeISO1 );
    PsPrint( wxPostScriptHeaderReencodeISO2 );
    if (wxPostScriptHeaderSpline)
        PsPrint( wxPostScriptHeaderSpline );
    PsPrint( "%%EndProlog\n" );

    SetBrush( *wxBLACK_BRUSH );
    SetPen( *wxBLACK_PEN );
    SetBackground( *wxWHITE_BRUSH );
    SetTextForeground( *wxBLACK );

    // set origin according to paper size
    SetDeviceOrigin( 0,0 );

    m_pageNumber = 1;
    return true;
}

void wxPostScriptDCImpl::EndDoc ()
{
    wxCHECK_RET( m_ok, wxT("invalid postscript dc") );

    if (m_clipping)
    {
        m_clipping = false;
        PsPrint( "grestore\n" );
    }

    if ( m_pstream ) {
        fclose( m_pstream );
        m_pstream = NULL;
    }

#if 0
    // THE FOLLOWING HAS BEEN CONTRIBUTED BY Andy Fyfe <andy@hyperparallel.com>
    wxCoord wx_printer_translate_x, wx_printer_translate_y;
    double wx_printer_scale_x, wx_printer_scale_y;

    wx_printer_translate_x = (wxCoord)m_printData.GetPrinterTranslateX();
    wx_printer_translate_y = (wxCoord)m_printData.GetPrinterTranslateY();

    wx_printer_scale_x = m_printData.GetPrinterScaleX();
    wx_printer_scale_y = m_printData.GetPrinterScaleY();

    // Compute the bounding box.  Note that it is in the default user
    // coordinate system, thus we have to convert the values.
    wxCoord minX = (wxCoord) XLOG2DEV(m_minX);
    wxCoord minY = (wxCoord) YLOG2DEV(m_minY);
    wxCoord maxX = (wxCoord) XLOG2DEV(m_maxX);
    wxCoord maxY = (wxCoord) YLOG2DEV(m_maxY);

    // LOG2DEV may have changed the minimum to maximum vice versa
    if ( minX > maxX ) { wxCoord tmp = minX; minX = maxX; maxX = tmp; }
    if ( minY > maxY ) { wxCoord tmp = minY; minY = maxY; maxY = tmp; }

    // account for used scaling (boundingbox is before scaling in ps-file)
    double scale_x = m_printData.GetPrinterScaleX() / ms_PSScaleFactor;
    double scale_y = m_printData.GetPrinterScaleY() / ms_PSScaleFactor;

    wxCoord llx, lly, urx, ury;
    llx = (wxCoord) ((minX+wx_printer_translate_x)*scale_x);
    lly = (wxCoord) ((minY+wx_printer_translate_y)*scale_y);
    urx = (wxCoord) ((maxX+wx_printer_translate_x)*scale_x);
    ury = (wxCoord) ((maxY+wx_printer_translate_y)*scale_y);
    // (end of bounding box computation)


    // If we're landscape, our sense of "x" and "y" is reversed.
    if (m_printData.GetOrientation() == wxLANDSCAPE)
    {
        wxCoord tmp;
        tmp = llx; llx = lly; lly = tmp;
        tmp = urx; urx = ury; ury = tmp;

        // We need either the two lines that follow, or we need to subtract
        // min_x from real_translate_y, which is commented out below.
        llx = llx - (wxCoord)(m_minX*wx_printer_scale_y);
        urx = urx - (wxCoord)(m_minX*wx_printer_scale_y);
    }

    // The Adobe specifications call for integers; we round as to make
    // the bounding larger.
    PsPrintf( wxT("%%%%BoundingBox: %d %d %d %d\n"),
            (wxCoord)floor((double)llx), (wxCoord)floor((double)lly),
            (wxCoord)ceil((double)urx), (wxCoord)ceil((double)ury) );

    // To check the correctness of the bounding box, postscript commands
    // to draw a box corresponding to the bounding box are generated below.
    // But since we typically don't want to print such a box, the postscript
    // commands are generated within comments.  These lines appear before any
    // adjustment of scale, rotation, or translation, and hence are in the
    // default user coordinates.
    PsPrint( "% newpath\n" );
    PsPrintf( wxT("%% %d %d moveto\n"), llx, lly );
    PsPrintf( wxT("%% %d %d lineto\n"), urx, lly );
    PsPrintf( wxT("%% %d %d lineto\n"), urx, ury );
    PsPrintf( wxT("%% %d %d lineto closepath stroke\n"), llx, ury );
#endif

#ifndef __WXMSW__
    wxPostScriptPrintNativeData *data =
        (wxPostScriptPrintNativeData *) m_printData.GetNativeData();

    if (m_ok && (m_printData.GetPrintMode() == wxPRINT_MODE_PRINTER))
    {
        wxString command;
        command += data->GetPrinterCommand();
        command += wxT(" ");
        command += data->GetPrinterOptions();
        command += wxT(" ");
        command += m_printData.GetFilename();

        wxExecute( command, true );
        wxRemoveFile( m_printData.GetFilename() );
    }
#endif
}

void wxPostScriptDCImpl::StartPage()
{
    wxCHECK_RET( m_ok, wxT("invalid postscript dc") );

    wxString buffer;
    buffer.Printf( wxT("%%%%Page: %d\n"), m_pageNumber++ );
    PsPrint( buffer );

#if 0
    wxPostScriptPrintNativeData *data =
        (wxPostScriptPrintNativeData *) m_printData.GetNativeData();

    wxCoord translate_x = (wxCoord)data->GetPrinterTranslateX();
    wxCoord translate_y = (wxCoord)data->GetPrinterTranslateY();

    buffer.Printf( "%d %d translate\n", translate_x, translate_y );
    PsPrint( buffer );

    double scale_x = data->GetPrinterScaleX();
    double scale_y = data->GetPrinterScaleY();

    buffer.Printf( "%f %f scale\n", scale_x, scale_y );
    buffer.Replace( ",", "." );
    PsPrint( buffer );

#endif

    // Each page starts with an "initgraphics" which resets the
    // transformation and so we need to rotate the page for
    // landscape printing)

    // I copied this one from a PostScript tutorial, but to no avail. RR.
    // PsPrint( "90 rotate llx neg ury nef translate\n" );

    if (m_printData.GetOrientation() == wxLANDSCAPE)
        PsPrint( "90 rotate\n" );
}

void wxPostScriptDCImpl::EndPage ()
{
    wxCHECK_RET( m_ok , wxT("invalid postscript dc") );

    PsPrint( "showpage\n" );
}

bool wxPostScriptDCImpl::DoBlit( wxCoord xdest, wxCoord ydest,
                           wxCoord fwidth, wxCoord fheight,
                           wxDC *source,
                           wxCoord xsrc, wxCoord ysrc,
                           wxRasterOperationMode rop,
                           bool WXUNUSED(useMask), wxCoord WXUNUSED(xsrcMask), wxCoord WXUNUSED(ysrcMask) )
{
    wxCHECK_MSG( m_ok, false, wxT("invalid postscript dc") );

    wxCHECK_MSG( source, false, wxT("invalid source dc") );

    // blit into a bitmap
    wxBitmap bitmap( (int)fwidth, (int)fheight );
    wxMemoryDC memDC;
    memDC.SelectObject(bitmap);
    memDC.Blit(0, 0, fwidth, fheight, source, xsrc, ysrc, rop); /* TODO: Blit transparently? */
    memDC.SelectObject(wxNullBitmap);

    //draw bitmap. scaling and positioning is done there
    GetOwner()->DrawBitmap( bitmap, xdest, ydest );

    return true;
}

wxCoord wxPostScriptDCImpl::GetCharHeight() const
{
    if (m_font.IsOk())
        return m_font.GetPointSize();
    else
        return 12;
}

void wxPostScriptDCImpl::PsPrint( const wxString& str )
{
    const wxCharBuffer psdata(str.utf8_str());

    wxPostScriptPrintNativeData *data =
        (wxPostScriptPrintNativeData *) m_printData.GetNativeData();

    switch (m_printData.GetPrintMode())
    {
#if wxUSE_STREAMS
        // append to output stream
        case wxPRINT_MODE_STREAM:
            {
                wxOutputStream* outputstream = data->GetOutputStream();
                wxCHECK_RET( outputstream, wxT("invalid outputstream") );
                outputstream->Write( psdata, strlen( psdata ) );
            }
            break;
#endif // wxUSE_STREAMS

        // save data into file
        default:
            wxCHECK_RET( m_pstream, wxT("invalid postscript dc") );
            fwrite( psdata, 1, strlen( psdata ), m_pstream );
    }
}

void wxPostScriptDCImpl::DoGetTextExtent(const wxString& string,
                                     wxCoord *x, wxCoord *y,
                                     wxCoord *descent, wxCoord *externalLeading,
                                     const wxFont *theFont ) const
{
    const wxFont *fontToUse = theFont;

    if (!fontToUse) fontToUse = &m_font;

    const float fontSize =
        fontToUse->GetPointSize() * GetFontPointSizeAdjustment(72.0);

    if (string.empty())
    {
        if (x) (*x) = 0;
        if (y) (*y) = 0;
        if (descent) (*descent) = 0;
        if (externalLeading) (*externalLeading) = 0;
        return;
    }

   // GTK 2.0

    const wxWX2MBbuf strbuf = string.mb_str();

    // conversion failed (non e.g. ISO characters)
    if ( !strbuf )
        return;

#if !wxUSE_AFM_FOR_POSTSCRIPT
    /* Provide a VERY rough estimate (avoid using it).
     * Produces accurate results for mono-spaced font
     * such as Courier (aka wxMODERN) */

    if ( x )
        *x = strlen (strbuf) * fontSize * 72.0 / 120.0;
    if ( y )
        *y = (wxCoord) (fontSize * 1.32);    /* allow for descender */
    if (descent) *descent = 0;
    if (externalLeading) *externalLeading = 0;
#else

    /* method for calculating string widths in postscript:
    /  read in the AFM (adobe font metrics) file for the
    /  actual font, parse it and extract the character widths
    /  and also the descender. this may be improved, but for now
    /  it works well. the AFM file is only read in if the
    /  font is changed. this may be chached in the future.
    /  calls to GetTextExtent with the font unchanged are rather
    /  efficient!!!
    /
    /  for each font and style used there is an AFM file necessary.
    /  currently i have only files for the roman font family.
    /  I try to get files for the other ones!
    /
    /  CAVE: the size of the string is currently always calculated
    /        in 'points' (1/72 of an inch). this should later on be
    /        changed to depend on the mapping mode.
    /  CAVE: the path to the AFM files must be set before calling this
    /        fun3B3Bction. this is usually done by a call like the following:
    /        wxSetAFMPath("d:\\wxw161\\afm\\");
    /
    /  example:
    /
    /    wxPostScriptDC dc(NULL, true);
    /    if (dc.IsOk()){
    /      wxSetAFMPath("d:\\wxw161\\afm\\");
    /      dc.StartDoc("Test");
    /      dc.StartPage();
    /      wxCoord w,h;
    /      dc.SetFont(new wxFontInfo(10).Family(wxFONTFAMILY_ROMAN));
    /      dc.GetTextExtent("Hallo",&w,&h);
    /      dc.EndPage();
    /      dc.EndDoc();
    /    }
    /
    /  by steve (stefan.hammes@urz.uni-heidelberg.de)
    /  created: 10.09.94
    /  updated: 14.05.95 */

    /* these static vars are for storing the state between calls */
    static int lastFamily= INT_MIN;
    static int lastSize= INT_MIN;
    static int lastStyle= INT_MIN;
    static int lastWeight= INT_MIN;
    static int lastDescender = INT_MIN;
    static int lastWidths[256]; /* widths of the characters */

    double UnderlinePosition = 0.0;
    double UnderlineThickness = 0.0;

    // Get actual parameters
    int Family = fontToUse->GetFamily();
    int Size =   fontToUse->GetPointSize();
    int Style =  fontToUse->GetStyle();
    int Weight = fontToUse->GetWeight();

    // If we have another font, read the font-metrics
    if (Family!=lastFamily || Size!=lastSize || Style!=lastStyle || Weight!=lastWeight)
    {
        // Store actual values
        lastFamily = Family;
        lastSize =   Size;
        lastStyle =  Style;
        lastWeight = Weight;

        const wxChar *name;

        switch (Family)
        {
            case wxMODERN:
            case wxTELETYPE:
            {
                if ((Style == wxFONTSTYLE_ITALIC) && (Weight == wxFONTWEIGHT_BOLD)) name = wxT("CourBoO.afm");
                else if ((Style != wxFONTSTYLE_ITALIC) && (Weight == wxFONTWEIGHT_BOLD)) name = wxT("CourBo.afm");
                else if ((Style == wxFONTSTYLE_ITALIC) && (Weight != wxFONTWEIGHT_BOLD)) name = wxT("CourO.afm");
                else name = wxT("Cour.afm");
                break;
            }
            case wxROMAN:
            {
                if ((Style == wxFONTSTYLE_ITALIC) && (Weight == wxFONTWEIGHT_BOLD)) name = wxT("TimesBoO.afm");
                else if ((Style != wxFONTSTYLE_ITALIC) && (Weight == wxFONTWEIGHT_BOLD)) name = wxT("TimesBo.afm");
                else if ((Style == wxFONTSTYLE_ITALIC) && (Weight != wxFONTWEIGHT_BOLD)) name = wxT("TimesO.afm");
                else name = wxT("TimesRo.afm");
                break;
            }
            case wxSCRIPT:
            {
                name = wxT("Zapf.afm");
                break;
            }
            case wxSWISS:
            default:
            {
                if ((Style == wxFONTSTYLE_ITALIC) && (Weight == wxFONTWEIGHT_BOLD)) name = wxT("HelvBoO.afm");
                else if ((Style != wxFONTSTYLE_ITALIC) && (Weight == wxFONTWEIGHT_BOLD)) name = wxT("HelvBo.afm");
                else if ((Style == wxFONTSTYLE_ITALIC) && (Weight != wxFONTWEIGHT_BOLD)) name = wxT("HelvO.afm");
                else name = wxT("Helv.afm");
                break;
            }
        }

        FILE *afmFile = NULL;

        // Get the directory of the AFM files
        wxString afmName;

        // VZ: I don't know if the cast always works under Unix but it clearly
        //     never does under Windows where the pointer is
        //     wxWindowsPrintNativeData and so calling GetFontMetricPath() on
        //     it just crashes
#ifndef __WIN32__
        wxPostScriptPrintNativeData *data =
            wxDynamicCast(m_printData.GetNativeData(), wxPostScriptPrintNativeData);

        if (data && !data->GetFontMetricPath().empty())
        {
            afmName = data->GetFontMetricPath();
            afmName << wxFILE_SEP_PATH << name;
        }
#endif // __WIN32__

        if ( !afmName.empty() )
            afmFile = wxFopen(afmName, wxT("r"));

        if ( !afmFile )
        {
#if defined(__UNIX__) && !defined(__VMS__)
           afmName = wxGetDataDir();
#else // !__UNIX__
           afmName = wxStandardPaths::Get().GetDataDir();
#endif // __UNIX__/!__UNIX__

           afmName <<  wxFILE_SEP_PATH
#if defined(__LINUX__) || defined(__FREEBSD__)
                   << wxT("gs_afm") << wxFILE_SEP_PATH
#else
                   << wxT("afm") << wxFILE_SEP_PATH
#endif
                   << name;
           afmFile = wxFopen(afmName,wxT("r"));
        }

        /* 2. open and process the file
           /  a short explanation of the AFM format:
           /  we have for each character a line, which gives its size
           /  e.g.:
           /
           /    C 63 ; WX 444 ; N question ; B 49 -14 395 676 ;
           /
           /  that means, we have a character with ascii code 63, and width
           /  (444/1000 * fontSize) points.
           /  the other data is ignored for now!
           /
           /  when the font has changed, we read in the right AFM file and store the
           /  character widths in an array, which is processed below (see point 3.). */
        if (afmFile==NULL)
        {
            wxLogDebug( wxT("GetTextExtent: can't open AFM file '%s'"), afmName.c_str() );
            wxLogDebug( wxT("               using approximate values"));
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
            for (int i=0; i<256; i++) lastWidths[i] = 500; /* an approximate value */
            lastDescender = -150; /* dito. */
        }
        else
        {
            /* init the widths array */
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
            for(int i=0; i<256; i++) lastWidths[i] = INT_MIN;
            /* some variables for holding parts of a line */
            char cString[10], semiString[10], WXString[10];
            char descString[20];
            char upString[30], utString[30];
            char encString[50];
            char line[256];
            int ascii,cWidth;
            /* read in the file and parse it */
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
            while(fgets(line,sizeof(line),afmFile)!=NULL)
            {
                /* A.) check for descender definition */
                if (strncmp(line,"Descender",9)==0)
                {
                    if ((sscanf(line,"%s%d",descString,&lastDescender)!=2) ||
                            (strcmp(descString,"Descender")!=0))
                    {
                        wxLogDebug( wxT("AFM-file '%s': line '%s' has error (bad descender)"), afmName.c_str(),line );
                    }
                }
                /* JC 1.) check for UnderlinePosition */
                else if(strncmp(line,"UnderlinePosition",17)==0)
                {
                    if ((sscanf(line,"%s%lf",upString,&UnderlinePosition)!=2) ||
                            (strcmp(upString,"UnderlinePosition")!=0))
                    {
                        wxLogDebug( wxT("AFM-file '%s': line '%s' has error (bad UnderlinePosition)"), afmName.c_str(), line );
                    }
                }
                /* JC 2.) check for UnderlineThickness */
                else if(strncmp(line,"UnderlineThickness",18)==0)
                {
                    if ((sscanf(line,"%s%lf",utString,&UnderlineThickness)!=2) ||
                            (strcmp(utString,"UnderlineThickness")!=0))
                    {
                        wxLogDebug( wxT("AFM-file '%s': line '%s' has error (bad UnderlineThickness)"), afmName.c_str(), line );
                    }
                }
                /* JC 3.) check for EncodingScheme */
                else if(strncmp(line,"EncodingScheme",14)==0)
                {
                    if ((sscanf(line,"%s%s",utString,encString)!=2) ||
                            (strcmp(utString,"EncodingScheme")!=0))
                    {
                        wxLogDebug( wxT("AFM-file '%s': line '%s' has error (bad EncodingScheme)"), afmName.c_str(), line );
                    }
                    else if (strncmp(encString, "AdobeStandardEncoding", 21))
                    {
                        wxLogDebug( wxT("AFM-file '%s': line '%s' has error (unsupported EncodingScheme %s)"),
                                afmName.c_str(),line, encString);
                    }
                }
                /* B.) check for char-width */
                else if(strncmp(line,"C ",2)==0)
                {
                    if (sscanf(line,"%s%d%s%s%d",cString,&ascii,semiString,WXString,&cWidth)!=5)
                    {
                        wxLogDebug(wxT("AFM-file '%s': line '%s' has an error (bad character width)"),afmName.c_str(),line);
                    }
                    if(strcmp(cString,"C")!=0 || strcmp(semiString,";")!=0 || strcmp(WXString,"WX")!=0)
                    {
                        wxLogDebug(wxT("AFM-file '%s': line '%s' has a format error"),afmName.c_str(),line);
                    }
                    /* printf("            char '%c'=%d has width '%d'\n",ascii,ascii,cWidth); */
                    if (ascii>=0 && ascii<256)
                    {
                        lastWidths[ascii] = cWidth; /* store width */
                    }
                    else
                    {
                        /* MATTHEW: this happens a lot; don't print an error */
                        /* wxLogDebug("AFM-file '%s': ASCII value %d out of range",afmName.c_str(),ascii); */
                    }
                }
                /* C.) ignore other entries. */
            }
            fclose(afmFile);
        }
        /* hack to compute correct values for german 'Umlaute'
           /  the correct way would be to map the character names
           /  like 'adieresis' to corresp. positions of ISOEnc and read
           /  these values from AFM files, too. Maybe later ... */

        // NB: casts to int are needed to suppress gcc 3.3 warnings
        lastWidths[196] = lastWidths[(int)'A'];  // U+00C4 A Umlaute
        lastWidths[228] = lastWidths[(int)'a'];  // U+00E4 a Umlaute
        lastWidths[214] = lastWidths[(int)'O'];  // U+00D6 O Umlaute
        lastWidths[246] = lastWidths[(int)'o'];  // U+00F6 o Umlaute
        lastWidths[220] = lastWidths[(int)'U'];  // U+00DC U Umlaute
        lastWidths[252] = lastWidths[(int)'u'];  // U+00FC u Umlaute
        lastWidths[223] = lastWidths[(int)251];  // U+00DF eszett (scharfes s)

        /* JC: calculate UnderlineThickness/UnderlinePosition */

        // VS: dirty, but is there any better solution?
        double *pt;
        pt = (double*) &m_underlinePosition;
        *pt = YLOG2DEVREL((wxCoord)(UnderlinePosition * fontSize)) / 1000.0f;
        pt = (double*) &m_underlineThickness;
        *pt = YLOG2DEVREL((wxCoord)(UnderlineThickness * fontSize)) / 1000.0f;

    }


    /* 3. now the font metrics are read in, calc size this
       /  is done by adding the widths of the characters in the
       /  string. they are given in 1/1000 of the size! */

    long sum=0;
    float height=fontSize; /* by default */
    unsigned char *p=(unsigned char *)wxMBSTRINGCAST strbuf;
    if(!p)
    {
        // String couldn't be converted which used to SEGV as reported here:
        // http://bugs.debian.org/702378
        // http://trac.wxwidgets.org/ticket/15300
        // Upstream suggests "just return if the conversion failed".
        if (x) (*x) = 0;
        if (y) (*y) = 0;
        if (descent) (*descent) = 0;
        if (externalLeading) (*externalLeading) = 0;
        return;
    }
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
    for(; *p; p++)
    {
        if(lastWidths[*p]== INT_MIN)
        {
            wxLogDebug(wxT("GetTextExtent: undefined width for character '%c' (%d)"), *p,*p);
            sum += lastWidths[(unsigned char)' ']; /* assume space */
        }
        else
        {
            sum += lastWidths[*p];
        }
    }

    double widthSum = sum;
    widthSum *= fontSize;
    widthSum /= 1000.0F;

    /* add descender to height (it is usually a negative value) */
    //if (lastDescender != INT_MIN)
    //{
    //    height += (wxCoord)(((-lastDescender)/1000.0F) * Size); /* MATTHEW: forgot scale */
    //}
    // - commented by V. Slavik - height already contains descender in it
    //   (judging from few experiments)

    /* return size values */
    if ( x )
        *x = (wxCoord)widthSum;
    if ( y )
        *y = (wxCoord)height;

    /* return other parameters */
    if (descent)
    {
        if(lastDescender!=INT_MIN)
        {
            *descent = (wxCoord)(((-lastDescender)/1000.0F) * fontSize); /* MATTHEW: forgot scale */
        }
        else
        {
            *descent = 0;
        }
    }

    /* currently no idea how to calculate this! */
    if (externalLeading) *externalLeading = 0;
#endif
    // Use AFM
}


#endif // wxUSE_PRINTING_ARCHITECTURE && wxUSE_POSTSCRIPT

// vi:sts=4:sw=4:et
