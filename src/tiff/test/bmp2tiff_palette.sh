#!/bin/sh
# Generated file, master is Makefile.am
. ${srcdir:-.}/common.sh
infile="$IMG_PALETTE_1C_8B_BMP"
outfile="o-bmp2tiff_palette.tiff"
f_test_convert "$BMP2TIFF" $infile $outfile
f_tiffinfo_validate $outfile
