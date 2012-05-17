/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif defined(__INTEL_COMPILER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/* $Id$ */


/*
 * Copyright (c) 1990-1997 Sam Leffler
 * Copyright (c) 1991-1997 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Sam Leffler and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Sam Leffler and Silicon Graphics.
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 * 
 * IN NO EVENT SHALL SAM LEFFLER OR SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gl.h>
#include <ctype.h>

#include "tiffio.h"

typedef unsigned char u_char;
typedef unsigned long u_long;

#define	streq(a,b)	(strcmp(a,b) == 0)
#define	strneq(a,b,n)	(strncmp(a,b,n) == 0)

uint32	rowsperstrip = (uint32) -1;
uint16	compression = COMPRESSION_PACKBITS;
uint16	config = PLANARCONFIG_CONTIG;
uint16	predictor = 0;
int	xmaxscreen;
int	ymaxscreen;
uint16	photometric = PHOTOMETRIC_RGB;
int	jpegcolormode = JPEGCOLORMODE_RGB;
int	quality = 75;		/* JPEG quality */

static	void usage(void);
static	void tiffsv(char*, int, int, int, int);

int
main(int argc, char* argv[])
{
	int c;
	extern int optind;
	extern char* optarg;

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	while ((c = getopt(argc, argv, "c:p:r:")) != -1)
		switch (c) {
		case 'b':		/* save as b&w */
			photometric = PHOTOMETRIC_MINISBLACK;
			break;
		case 'c':		/* compression scheme */
			if (streq(optarg, "none"))
				compression = COMPRESSION_NONE;
			else if (streq(optarg, "packbits"))
				compression = COMPRESSION_PACKBITS;
			else if (strneq(optarg, "jpeg", 4)) {
				char* cp = strchr(optarg, ':');
				if (cp && isdigit(cp[1]))
					quality = atoi(cp+1);
				if (cp && strchr(cp, 'r'))
					jpegcolormode = JPEGCOLORMODE_RAW;
				compression = COMPRESSION_JPEG;
			} else if (strneq(optarg, "lzw", 3)) {
				char* cp = strchr(optarg, ':');
				if (cp)
					predictor = atoi(cp+1);
				compression = COMPRESSION_LZW;
			} else
				usage();
			break;
		case 'p':		/* planar configuration */
			if (streq(optarg, "separate"))
				config = PLANARCONFIG_SEPARATE;
			else if (streq(optarg, "contig"))
				config = PLANARCONFIG_CONTIG;
			else
				usage();
			break;
		case 'r':		/* rows/strip */
			rowsperstrip = atoi(optarg);
			break;
		case '?':
			usage();
			/*NOTREACHED*/
		}
	if (argc - optind != 1 && argc - optind != 5)
		usage();
	xmaxscreen = getgdesc(GD_XPMAX)-1;
	ymaxscreen = getgdesc(GD_YPMAX)-1;
	foreground();
	noport();
	winopen("tiffsv");
	if (argc - optind == 5)
		tiffsv(argv[optind],
		    atoi(argv[optind+1]), atoi(argv[optind+2]),
		    atoi(argv[optind+3]), atoi(argv[optind+4]));
	else
		tiffsv(argv[optind], 0, xmaxscreen, 0, ymaxscreen);
	return (0);
}

char* stuff[] = {
"usage: tiffsv [options] outimage.tif [x1 x2 y1 y2] [-b]",
"where options are:",
" -p contig	pack samples contiguously (e.g. RGBRGB...)",
" -p separate	store samples separately (e.g. RRR...GGG...BBB...)",
"",
" -r #		make each strip have no more than # rows",
"",
" -c lzw[:opts]	compress output with Lempel-Ziv & Welch encoding",
" -c jpeg[:opts]compress output with JPEG encoding",
" -c packbits	compress output with packbits encoding",
" -c none	use no compression algorithm on output",
"",
"JPEG options:",
" #		set compression quality level (0-100, default 75)",
" r		output color image as RGB rather than YCbCr",
"",
"LZW options:",
" #		set predictor value for Lempel-Ziv & Welch encoding",
"For example, -c lzw:2 to get LZW-encoded data with horizontal differencing",
NULL
};

static void
usage(void)
{
	char buf[BUFSIZ];
	int i;

	setbuf(stderr, buf);
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	for (i = 0; stuff[i] != NULL; i++)
		fprintf(stderr, "%s\n", stuff[i]);
	exit(-1);
}

static void
svRGBSeparate(TIFF* tif, u_long* ss, int xsize, int ysize)
{
	tsize_t stripsize = TIFFStripSize(tif);
	u_char *rbuf = (u_char *)_TIFFmalloc(3*stripsize);
	u_char *gbuf = rbuf + stripsize;
	u_char *bbuf = gbuf + stripsize;
	register int y;

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	for (y = 0; y <= ysize; y += rowsperstrip) {
		u_char *rp, *gp, *bp;
		register int x;
		register uint32 n;

		n = rowsperstrip;
		if (n > ysize-y+1)
			n = ysize-y+1;
		rp = rbuf; gp = gbuf; bp = bbuf;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
		do {
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
			for (x = 0; x <= xsize; x++) {
				u_long v = ss[x];
				rp[x] = v;
				gp[x] = v >> 8;
				bp[x] = v >> 16;
			}
			rp += xsize+1, gp += xsize+1, bp += xsize+1;
			ss += xsize+1;
		} while (--n);
		if (TIFFWriteEncodedStrip(tif, TIFFComputeStrip(tif,y,0),
		    rbuf, stripsize) < 0)
			break;
		if (TIFFWriteEncodedStrip(tif, TIFFComputeStrip(tif,y,1),
		    gbuf, stripsize) < 0)
			break;
		if (TIFFWriteEncodedStrip(tif, TIFFComputeStrip(tif,y,2),
		    bbuf, stripsize) < 0)
			break;
	}
	_TIFFfree(rbuf);
}

static void
svRGBContig(TIFF* tif, u_long* ss, int xsize, int ysize)
{
	register int x, y;
	tsize_t stripsize = TIFFStripSize(tif);
	u_char *strip = (u_char *)_TIFFmalloc(stripsize);

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	for (y = 0; y <= ysize; y += rowsperstrip) {
		register u_char *pp = strip;
		register uint32 n;

		n = rowsperstrip;
		if (n > ysize-y+1)
			n = ysize-y+1;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
		do {
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
			for (x = 0; x <= xsize; x++) {
				u_long v = ss[x];
				pp[0] = v;
				pp[1] = v >> 8;
				pp[2] = v >> 16;
				pp += 3;
			}
			ss += xsize+1;
		} while (--n);
		if (TIFFWriteEncodedStrip(tif, TIFFComputeStrip(tif,y,0),
		    strip, stripsize) < 0)
			break;
	}
	_TIFFfree(strip);
}

#undef RED
#undef GREEN
#undef BLUE
#define	CVT(x)	(((x)*255)/100)
#define	RED	CVT(28)		/* 28% */
#define	GREEN	CVT(59)		/* 59% */
#define	BLUE	CVT(11)		/* 11% */

static void
svGrey(TIFF* tif, u_long* ss, int xsize, int ysize)
{
	register int x, y;
	u_char *buf = (u_char *)_TIFFmalloc(TIFFScanlineSize(tif));

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	for (y = 0; y <= ysize; y++) {
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
		for (x = 0; x <= xsize; x++) {
			u_char *cp = (u_char *)&ss[x];
			buf[x] = (RED*cp[3] + GREEN*cp[2] + BLUE*cp[1]) >> 8;
		}
		if (TIFFWriteScanline(tif, buf, (uint32) y, 0) < 0)
			break;
		ss += xsize+1;
	}
	_TIFFfree(buf);
}

#define	MIN(a,b)	((a)<(b)?(a):(b))
#define	ABS(x)		((x)<0?-(x):(x))

static void
tiffsv(char* name, int x1, int x2, int y1, int y2)
{
	TIFF *tif;
	int xsize, ysize;
	int xorg, yorg;
	u_long *scrbuf;

	xorg = MIN(x1,x2);
	yorg = MIN(y1,y2);
	if (xorg<0)
		xorg = 0;
	if (yorg<0)
		yorg = 0;
	xsize = ABS(x2-x1);
	ysize = ABS(y2-y1);
	if (xorg+xsize > xmaxscreen)
		xsize = xmaxscreen-xorg;
	if (yorg+ysize > ymaxscreen)
		ysize = ymaxscreen-yorg;
	tif = TIFFOpen(name, "w");
	TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, (uint32) (xsize+1));
	TIFFSetField(tif, TIFFTAG_IMAGELENGTH, (uint32) (ysize+1));
	TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
	TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL,
	    photometric == PHOTOMETRIC_RGB ? 3 : 1);
	TIFFSetField(tif, TIFFTAG_PLANARCONFIG, config);
	TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
	switch (compression) {
	case COMPRESSION_JPEG:
		if (photometric == PHOTOMETRIC_RGB && jpegcolormode == JPEGCOLORMODE_RGB)
			photometric = PHOTOMETRIC_YCBCR;
		TIFFSetField(tif, TIFFTAG_JPEGQUALITY, quality);
		TIFFSetField(tif, TIFFTAG_JPEGCOLORMODE, jpegcolormode);
		break;
	case COMPRESSION_LZW:
		if (predictor != 0)
			TIFFSetField(tif, TIFFTAG_PREDICTOR, predictor);
		break;
	}
	TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
	TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_BOTLEFT);
	rowsperstrip = TIFFDefaultStripSize(tif, rowsperstrip);
	TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rowsperstrip);
	scrbuf = (u_long *)_TIFFmalloc((xsize+1)*(ysize+1)*sizeof (u_long));
	readdisplay(xorg, yorg, xorg+xsize, yorg+ysize, scrbuf, RD_FREEZE);
	if (photometric == PHOTOMETRIC_RGB) {
		if (config == PLANARCONFIG_SEPARATE)
			svRGBSeparate(tif, scrbuf, xsize, ysize);
		else
			svRGBContig(tif, scrbuf, xsize, ysize);
	} else
		svGrey(tif, scrbuf, xsize, ysize);
	(void) TIFFClose(tif);
	_TIFFfree((char *)scrbuf);
}
