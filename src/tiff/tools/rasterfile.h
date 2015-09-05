/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/* $Header: /cvs/libtiff/tools/rasterfile.h,v 1.3 2003/11/12 19:14:33 dron Exp $ */

/*
 * Description of header for files containing raster images
 */
struct rasterfile {
	char	ras_magic[4];		/* magic number */
	long	ras_width;		/* width (pixels) of image */
	long	ras_height;		/* height (pixels) of image */
	long	ras_depth;		/* depth (1, 8, or 24 bits) of pixel */
	long	ras_length;		/* length (bytes) of image */
	long	ras_type;		/* type of file; see RT_* below */
	long	ras_maptype;		/* type of colormap; see RMT_* below */
	long	ras_maplength;		/* length (bytes) of following map */
	/* color map follows for ras_maplength bytes, followed by image */
};
#define	RAS_MAGIC	"\x59\xa6\x6a\x95"
#define	RAS_MAGIC_INV	"\x95\x6a\xa6\x59"

	/* Sun supported ras_type's */
#define RT_OLD		0	/* Raw pixrect image in 68000 byte order */
#define RT_STANDARD	1	/* Raw pixrect image in 68000 byte order */
#define RT_BYTE_ENCODED	2	/* Run-length compression of bytes */
#define RT_EXPERIMENTAL 0xffff	/* Reserved for testing */

	/* Sun registered ras_maptype's */
#define RMT_RAW		2
	/* Sun supported ras_maptype's */
#define RMT_NONE	0	/* ras_maplength is expected to be 0 */
#define RMT_EQUAL_RGB	1	/* red[ras_maplength/3],green[],blue[] */

/*
 * NOTES:
 * 	Each line of the image is rounded out to a multiple of 16 bits.
 *   This corresponds to the rounding convention used by the memory pixrect
 *   package (/usr/include/pixrect/memvar.h) of the SunWindows system.
 *	The ras_encoding field (always set to 0 by Sun's supported software)
 *   was renamed to ras_length in release 2.0.  As a result, rasterfiles
 *   of type 0 generated by the old software claim to have 0 length; for
 *   compatibility, code reading rasterfiles must be prepared to compute the
 *   true length from the width, height, and depth fields.
 */
