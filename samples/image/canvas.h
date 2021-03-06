/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        samples/image/canvas.h
// Purpose:     sample showing operations with wxImage
// Author:      Robert Roebling
// Modified by: Francesco Montorsi
// Created:     1998
// Copyright:   (c) 1998-2005 Robert Roebling
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "wx/scrolwin.h"

//-----------------------------------------------------------------------------
// MyCanvas
//-----------------------------------------------------------------------------

class MyCanvas: public wxScrolledWindow
{
public:
    MyCanvas( wxWindow *parent, wxWindowID, const wxPoint &pos, const wxSize &size );
    ~MyCanvas();

    void OnPaint( wxPaintEvent &event );
    void CreateAntiAliasedBitmap();

    wxBitmap  my_horse_png;
    wxBitmap  my_horse_jpeg;
    wxBitmap  my_horse_gif;
    wxBitmap  my_horse_bmp;
    wxBitmap  my_horse_bmp2;
    wxBitmap  my_horse_pcx;
    wxBitmap  my_horse_pnm;
    wxBitmap  my_horse_tiff;
    wxBitmap  my_horse_tga;
    wxBitmap  my_horse_xpm;
    wxBitmap  my_horse_ico32;
    wxBitmap  my_horse_ico16;
    wxBitmap  my_horse_ico;
    wxBitmap  my_horse_cur;
    wxBitmap  my_png_from_res,
              my_png_from_mem;

    wxBitmap  my_smile_xbm;
    wxBitmap  my_square;
    wxBitmap  my_anti;

    wxBitmap  my_horse_asciigrey_pnm;
    wxBitmap  my_horse_rawgrey_pnm;

    wxBitmap  colorized_horse_jpeg;
    wxBitmap  my_cmyk_jpeg;

    wxBitmap my_toucan;
    wxBitmap my_toucan_flipped_horiz;
    wxBitmap my_toucan_flipped_vert;
    wxBitmap my_toucan_flipped_both;
    wxBitmap my_toucan_grey;
    wxBitmap my_toucan_head;
    wxBitmap my_toucan_scaled_normal;
    wxBitmap my_toucan_scaled_high;
    wxBitmap my_toucan_blur;

    int xH, yH;
    int m_ani_images;
    wxBitmap *my_horse_ani;

private:
    wxBitmap m_bmpSmileXpm;
    wxIcon   m_iconSmileXpm;

    wxDECLARE_EVENT_TABLE();
};
