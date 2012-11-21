/* $Id$ */

/* WARNING: The type of JPEG encapsulation defined by the TIFF Version 6.0
   specification is now totally obsolete and deprecated for new applications and
   images. This file was was created solely in order to read unconverted images
   still present on some users' computer systems. It will never be extended
   to write such files. Writing new-style JPEG compressed TIFFs is implemented
   in tif_jpeg.c.

   The code is carefully crafted to robustly read all gathered JPEG-in-TIFF
   testfiles, and anticipate as much as possible all other... But still, it may
   fail on some. If you encounter problems, please report them on the TIFF
   mailing list and/or to Joris Van Damme <info@awaresystems.be>.

   Please read the file called "TIFF Technical Note #2" if you need to be
   convinced this compression scheme is bad and breaks TIFF. That document
   is linked to from the LibTiff site <http://www.remotesensing.org/libtiff/>
   and from AWare Systems' TIFF section
   <http://www.awaresystems.be/imaging/tiff.html>. It is also absorbed
   in Adobe's specification supplements, marked "draft" up to this day, but
   supported by the TIFF community.

   This file interfaces with Release 6B of the JPEG Library written by the
   Independent JPEG Group. Previous versions of this file required a hack inside
   the LibJpeg library. This version no longer requires that. Remember to
   remove the hack if you update from the old version.

   Copyright (c) Joris Van Damme <info@awaresystems.be>
   Copyright (c) AWare Systems <http://www.awaresystems.be/>

   The licence agreement for this file is the same as the rest of the LibTiff
   library.

   IN NO EVENT SHALL JORIS VAN DAMME OR AWARE SYSTEMS BE LIABLE FOR
   ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
   OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
   WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
   LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
   OF THIS SOFTWARE.

   Joris Van Damme and/or AWare Systems may be available for custom
   developement. If you like what you see, and need anything similar or related,
   contact <info@awaresystems.be>.
*/

/* What is what, and what is not?

   This decoder starts with an input stream, that is essentially the JpegInterchangeFormat
   stream, if any, followed by the strile data, if any. This stream is read in
   OJPEGReadByte and related functions.

   It analyzes the start of this stream, until it encounters non-marker data, i.e.
   compressed image data. Some of the header markers it sees have no actual content,
   like the SOI marker, and APP/COM markers that really shouldn't even be there. Some
   other markers do have content, and the valuable bits and pieces of information
   in these markers are saved, checking all to verify that the stream is more or
   less within expected bounds. This happens inside the OJPEGReadHeaderInfoSecStreamXxx
   functions.

   Some OJPEG imagery contains no valid JPEG header markers. This situation is picked
   up on if we've seen no SOF marker when we're at the start of the compressed image
   data. In this case, the tables are read from JpegXxxTables tags, and the other
   bits and pieces of information is initialized to its most basic value. This is
   implemented in the OJPEGReadHeaderInfoSecTablesXxx functions.

   When this is complete, a good and valid JPEG header can be assembled, and this is
   passed through to LibJpeg. When that's done, the remainder of the input stream, i.e.
   the compressed image data, can be passed through unchanged. This is done in
   OJPEGWriteStream functions.

   LibTiff rightly expects to know the subsampling values before decompression. Just like
   in new-style JPEG-in-TIFF, though, or even more so, actually, the YCbCrsubsampling
   tag is notoriously unreliable. To correct these tag values with the ones inside
   the JPEG stream, the first part of the input stream is pre-scanned in
   OJPEGSubsamplingCorrect, making no note of any other data, reporting no warnings
   or errors, up to the point where either these values are read, or it's clear they
   aren't there. This means that some of the data is read twice, but we feel speed
   in correcting these values is important enough to warrant this sacrifice. Allthough
   there is currently no define or other configuration mechanism to disable this behaviour,
   the actual header scanning is build to robustly respond with error report if it
   should encounter an uncorrected mismatch of subsampling values. See
   OJPEGReadHeaderInfoSecStreamSof.

   The restart interval and restart markers are the most tricky part... The restart
   interval can be specified in a tag. It can also be set inside the input JPEG stream.
   It can be used inside the input JPEG stream. If reading from strile data, we've
   consistenly discovered the need to insert restart markers in between the different
   striles, as is also probably the most likely interpretation of the original TIFF 6.0
   specification. With all this setting of interval, and actual use of markers that is not
   predictable at the time of valid JPEG header assembly, the restart thing may turn
   out the Achilles heel of this implementation. Fortunately, most OJPEG writer vendors
   succeed in reading back what they write, which may be the reason why we've been able
   to discover ways that seem to work.

   Some special provision is made for planarconfig separate OJPEG files. These seem
   to consistently contain header info, a SOS marker, a plane, SOS marker, plane, SOS,
   and plane. This may or may not be a valid JPEG configuration, we don't know and don't
   care. We want LibTiff to be able to access the planes individually, without huge
   buffering inside LibJpeg, anyway. So we compose headers to feed to LibJpeg, in this
   case, that allow us to pass a single plane such that LibJpeg sees a valid
   single-channel JPEG stream. Locating subsequent SOS markers, and thus subsequent
   planes, is done inside OJPEGReadSecondarySos.

   The benefit of the scheme is... that it works, basically. We know of no other that
   does. It works without checking software tag, or otherwise going about things in an
   OJPEG flavor specific manner. Instead, it is a single scheme, that covers the cases
   with and without JpegInterchangeFormat, with and without striles, with part of
   the header in JpegInterchangeFormat and remainder in first strile, etc. It is forgiving
   and robust, may likely work with OJPEG flavors we've not seen yet, and makes most out
   of the data.

   Another nice side-effect is that a complete JPEG single valid stream is build if
   planarconfig is not separate (vast majority). We may one day use that to build
   converters to JPEG, and/or to new-style JPEG compression inside TIFF.

   A dissadvantage is the lack of random access to the individual striles. This is the
   reason for much of the complicated restart-and-position stuff inside OJPEGPreDecode.
   Applications would do well accessing all striles in order, as this will result in
   a single sequential scan of the input stream, and no restarting of LibJpeg decoding
   session.
*/

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN

#include "tiffiop.h"
#ifdef OJPEG_SUPPORT

<<<<<<< HEAD
/* Configuration defines here are:
 * JPEG_ENCAP_EXTERNAL: The normal way to call libjpeg, uses longjump. In some environments,
 * 	like eg LibTiffDelphi, this is not possible. For this reason, the actual calls to
 * 	libjpeg, with longjump stuff, are encapsulated in dedicated functions. When
 * 	JPEG_ENCAP_EXTERNAL is defined, these encapsulating functions are declared external
 * 	to this unit, and can be defined elsewhere to use stuff other then longjump.
 * 	The default mode, without JPEG_ENCAP_EXTERNAL, implements the call encapsulators
 * 	here, internally, with normal longjump.
 * SETJMP, LONGJMP, JMP_BUF: On some machines/environments a longjump equivalent is
 * 	conviniently available, but still it may be worthwhile to use _setjmp or sigsetjmp
 * 	in place of plain setjmp. These macros will make it easier. It is useless
 * 	to fiddle with these if you define JPEG_ENCAP_EXTERNAL.
 * OJPEG_BUFFER: Define the size of the desired buffer here. Should be small enough so as to guarantee
 * 	instant processing, optimal streaming and optimal use of processor cache, but also big
 * 	enough so as to not result in significant call overhead. It should be at least a few
 * 	bytes to accomodate some structures (this is verified in asserts), but it would not be
 * 	sensible to make it this small anyway, and it should be at most 64K since it is indexed
 * 	with uint16. We recommend 2K.
 * EGYPTIANWALK: You could also define EGYPTIANWALK here, but it is not used anywhere and has
 * 	absolutely no effect. That is why most people insist the EGYPTIANWALK is a bit silly.
 */

/* define LIBJPEG_ENCAP_EXTERNAL */
#define SETJMP(jbuf) setjmp(jbuf)
#define LONGJMP(jbuf,code) longjmp(jbuf,code)
#define JMP_BUF jmp_buf
#define OJPEG_BUFFER 2048
/* define EGYPTIANWALK */

#define JPEG_MARKER_SOF0 0xC0
#define JPEG_MARKER_SOF1 0xC1
#define JPEG_MARKER_SOF3 0xC3
#define JPEG_MARKER_DHT 0xC4
#define JPEG_MARKER_RST0 0XD0
#define JPEG_MARKER_SOI 0xD8
#define JPEG_MARKER_EOI 0xD9
#define JPEG_MARKER_SOS 0xDA
#define JPEG_MARKER_DQT 0xDB
#define JPEG_MARKER_DRI 0xDD
#define JPEG_MARKER_APP0 0xE0
#define JPEG_MARKER_COM 0xFE

#define FIELD_OJPEG_JPEGINTERCHANGEFORMAT (FIELD_CODEC+0)
#define FIELD_OJPEG_JPEGINTERCHANGEFORMATLENGTH (FIELD_CODEC+1)
#define FIELD_OJPEG_JPEGQTABLES (FIELD_CODEC+2)
#define FIELD_OJPEG_JPEGDCTABLES (FIELD_CODEC+3)
#define FIELD_OJPEG_JPEGACTABLES (FIELD_CODEC+4)
#define FIELD_OJPEG_JPEGPROC (FIELD_CODEC+5)
#define FIELD_OJPEG_JPEGRESTARTINTERVAL (FIELD_CODEC+6)

static const TIFFField ojpegFields[] = {
	{TIFFTAG_JPEGIFOFFSET,1,1,TIFF_LONG8,0,TIFF_SETGET_UINT64,TIFF_SETGET_UNDEFINED,FIELD_OJPEG_JPEGINTERCHANGEFORMAT,TRUE,FALSE,"JpegInterchangeFormat",NULL},
	{TIFFTAG_JPEGIFBYTECOUNT,1,1,TIFF_LONG8,0,TIFF_SETGET_UINT64,TIFF_SETGET_UNDEFINED,FIELD_OJPEG_JPEGINTERCHANGEFORMATLENGTH,TRUE,FALSE,"JpegInterchangeFormatLength",NULL},
	{TIFFTAG_JPEGQTABLES,TIFF_VARIABLE2,TIFF_VARIABLE2,TIFF_LONG8,0,TIFF_SETGET_C32_UINT64,TIFF_SETGET_UNDEFINED,FIELD_OJPEG_JPEGQTABLES,FALSE,TRUE,"JpegQTables",NULL},
	{TIFFTAG_JPEGDCTABLES,TIFF_VARIABLE2,TIFF_VARIABLE2,TIFF_LONG8,0,TIFF_SETGET_C32_UINT64,TIFF_SETGET_UNDEFINED,FIELD_OJPEG_JPEGDCTABLES,FALSE,TRUE,"JpegDcTables",NULL},
	{TIFFTAG_JPEGACTABLES,TIFF_VARIABLE2,TIFF_VARIABLE2,TIFF_LONG8,0,TIFF_SETGET_C32_UINT64,TIFF_SETGET_UNDEFINED,FIELD_OJPEG_JPEGACTABLES,FALSE,TRUE,"JpegAcTables",NULL},
	{TIFFTAG_JPEGPROC,1,1,TIFF_SHORT,0,TIFF_SETGET_UINT16,TIFF_SETGET_UNDEFINED,FIELD_OJPEG_JPEGPROC,FALSE,FALSE,"JpegProc",NULL},
	{TIFFTAG_JPEGRESTARTINTERVAL,1,1,TIFF_SHORT,0,TIFF_SETGET_UINT16,TIFF_SETGET_UNDEFINED,FIELD_OJPEG_JPEGRESTARTINTERVAL,FALSE,FALSE,"JpegRestartInterval",NULL},
};

#ifndef LIBJPEG_ENCAP_EXTERNAL
=======
/* JPEG Compression support, as per the original TIFF 6.0 specification.

   WARNING: KLUDGE ALERT!  The type of JPEG encapsulation defined by the TIFF
                           Version 6.0 specification is now totally obsolete and
   deprecated for new applications and images.  This file is an unsupported hack
   that was created solely in order to read (but NOT write!) a few old,
   unconverted images still present on some users' computer systems.  The code
   isn't pretty or robust, and it won't read every "old format" JPEG-in-TIFF
   file (see Samuel Leffler's draft "TIFF Technical Note No. 2" for a long and
   incomplete list of known problems), but it seems to work well enough in the
   few cases of practical interest to the author; so, "caveat emptor"!  This
   file should NEVER be enhanced to write new images using anything other than
   the latest approved JPEG-in-TIFF encapsulation method, implemented by the
   "tif_jpeg.c" file elsewhere in this library.

   This file interfaces with Release 6B of the JPEG Library written by theu
   Independent JPEG Group, which you can find on the Internet at:
   ftp://ftp.uu.net:/graphics/jpeg/.

   The "C" Preprocessor macros, "[CD]_LOSSLESS_SUPPORTED", are defined by your
   JPEG Library Version 6B only if you have applied a (massive!) patch by Ken
   Murchison of Oceana Matrix Ltd. <ken@oceana.com> to support lossless Huffman
   encoding (TIFF "JPEGProc" tag value = 14).  This patch can be found on the
   Internet at: ftp://ftp.oceana.com/pub/ljpeg-6b.tar.gz.

   Some old files produced by the Wang Imaging application for Microsoft Windows
   apparently can be decoded only with a special patch to the JPEG Library,
   which defines a subroutine named "jpeg_reset_huff_decode()" in its "jdhuff.c"
   module (the "jdshuff.c" module, if Ken Murchison's patch has been applied).
   Unfortunately the patch differs slightly in each case, and some TIFF Library
   have reported problems finding the code, so both versions appear below; you
   should carefully extract and apply only the version that applies to your JPEG
   Library!

   Contributed by Scott Marovich <marovich@hpl.hp.com> with considerable help
   from Charles Auer <Bumble731@msn.com> to unravel the mysteries of image files
   created by the Wang Imaging application for Microsoft Windows.
*/
#if 0  /* Patch for JPEG Library WITHOUT lossless Huffman coding */
*** jdhuff.c.orig	Mon Oct 20 17:51:10 1997
--- jdhuff.c	Sun Nov 11 17:33:58 2001
***************
*** 648,651 ****
--- 648,683 ----
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
    for (i = 0; i < NUM_HUFF_TBLS; i++) {
      entropy->dc_derived_tbls[i] = entropy->ac_derived_tbls[i] = NULL;
    }
  }
+ 
+ /*
+  * BEWARE OF KLUDGE:  This subroutine is a hack for decoding illegal JPEG-in-
+  *                    TIFF encapsulations produced by Microsoft's Wang Imaging
+  * for Windows application with the public-domain TIFF Library.  Based upon an
+  * examination of selected output files, this program apparently divides a JPEG
+  * bit-stream into consecutive horizontal TIFF "strips", such that the JPEG
+  * encoder's/decoder's DC coefficients for each image component are reset before
+  * each "strip".  Moreover, a "strip" is not necessarily encoded in a multiple
+  * of 8 bits, so one must sometimes discard 1-7 bits at the end of each "strip"
+  * for alignment to the next input-Byte storage boundary.  IJG JPEG Library
+  * decoder state is not normally exposed to client applications, so this sub-
+  * routine provides the TIFF Library with a "hook" to make these corrections.
+  * It should be called after "jpeg_start_decompress()" and before
+  * "jpeg_finish_decompress()", just before decoding each "strip" using
+  * "jpeg_read_raw_data()" or "jpeg_read_scanlines()".
+  *
+  * This kludge is not sanctioned or supported by the Independent JPEG Group, and
+  * future changes to the IJG JPEG Library might invalidate it.  Do not send bug
+  * reports about this code to IJG developers.  Instead, contact the author for
+  * advice: Scott B. Marovich <marovich@hpl.hp.com>, Hewlett-Packard Labs, 6/01.
+  */
+ GLOBAL(void)
+ jpeg_reset_huff_decode (register j_decompress_ptr cinfo)
+ { register huff_entropy_ptr entropy = (huff_entropy_ptr)cinfo->entropy;
+   register int ci = 0;
+ 
+   /* Discard encoded input bits, up to the next Byte boundary */
+   entropy->bitstate.bits_left &= ~7;
+   /* Re-initialize DC predictions to 0 */
+   do entropy->saved.last_dc_val[ci] = 0; while (++ci < cinfo->comps_in_scan);
+ }
#endif /* Patch for JPEG Library WITHOUT lossless Huffman coding */
#if 0  /* Patch for JPEG Library WITH lossless Huffman coding */
*** jdshuff.c.orig	Mon Mar 11 16:44:54 2002
--- jdshuff.c	Mon Mar 11 16:44:54 2002
***************
*** 357,360 ****
--- 357,393 ----
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
    for (i = 0; i < NUM_HUFF_TBLS; i++) {
      entropy->dc_derived_tbls[i] = entropy->ac_derived_tbls[i] = NULL;
    }
  }
+ 
+ /*
+  * BEWARE OF KLUDGE:  This subroutine is a hack for decoding illegal JPEG-in-
+  *                    TIFF encapsulations produced by Microsoft's Wang Imaging
+  * for Windows application with the public-domain TIFF Library.  Based upon an
+  * examination of selected output files, this program apparently divides a JPEG
+  * bit-stream into consecutive horizontal TIFF "strips", such that the JPEG
+  * encoder's/decoder's DC coefficients for each image component are reset before
+  * each "strip".  Moreover, a "strip" is not necessarily encoded in a multiple
+  * of 8 bits, so one must sometimes discard 1-7 bits at the end of each "strip"
+  * for alignment to the next input-Byte storage boundary.  IJG JPEG Library
+  * decoder state is not normally exposed to client applications, so this sub-
+  * routine provides the TIFF Library with a "hook" to make these corrections.
+  * It should be called after "jpeg_start_decompress()" and before
+  * "jpeg_finish_decompress()", just before decoding each "strip" using
+  * "jpeg_read_raw_data()" or "jpeg_read_scanlines()".
+  *
+  * This kludge is not sanctioned or supported by the Independent JPEG Group, and
+  * future changes to the IJG JPEG Library might invalidate it.  Do not send bug
+  * reports about this code to IJG developers.  Instead, contact the author for
+  * advice: Scott B. Marovich <marovich@hpl.hp.com>, Hewlett-Packard Labs, 6/01.
+  */
+ GLOBAL(void)
+ jpeg_reset_huff_decode (register j_decompress_ptr cinfo)
+ { register shuff_entropy_ptr entropy = (shuff_entropy_ptr)
+                                        ((j_lossy_d_ptr)cinfo->codec)->entropy_private;
+   register int ci = 0;
+ 
+   /* Discard encoded input bits, up to the next Byte boundary */
+   entropy->bitstate.bits_left &= ~7;
+   /* Re-initialize DC predictions to 0 */
+   do entropy->saved.last_dc_val[ci] = 0; while (++ci < cinfo->comps_in_scan);
+ }
#endif /* Patch for JPEG Library WITH lossless Huffman coding */
>>>>>>> sync with upstream
#include <setjmp.h>
#endif

/* We undefine FAR to avoid conflict with JPEG definition */

#ifdef FAR
#undef FAR
#endif

/*
  Libjpeg's jmorecfg.h defines INT16 and INT32, but only if XMD_H is
  not defined.  Unfortunately, the MinGW and Borland compilers include
  a typedef for INT32, which causes a conflict.  MSVC does not include
  a conficting typedef given the headers which are included.
*/
#if defined(__BORLANDC__) || defined(__MINGW32__)
# define XMD_H 1
#endif

/* Define "boolean" as unsigned char, not int, per Windows custom. */
#if defined(__WIN32__) && !defined(__MINGW32__)
# ifndef __RPCNDR_H__            /* don't conflict if rpcndr.h already read */
   typedef unsigned char boolean;
# endif
# define HAVE_BOOLEAN            /* prevent jmorecfg.h from redefining it */
#endif

#include "jpeglib.h"
#include "jerror.h"

typedef struct jpeg_error_mgr jpeg_error_mgr;
typedef struct jpeg_common_struct jpeg_common_struct;
typedef struct jpeg_decompress_struct jpeg_decompress_struct;
typedef struct jpeg_source_mgr jpeg_source_mgr;

typedef enum {
	osibsNotSetYet,
	osibsJpegInterchangeFormat,
	osibsStrile,
	osibsEof
} OJPEGStateInBufferSource;

typedef enum {
	ososSoi,
	ososQTable0,ososQTable1,ososQTable2,ososQTable3,
	ososDcTable0,ososDcTable1,ososDcTable2,ososDcTable3,
	ososAcTable0,ososAcTable1,ososAcTable2,ososAcTable3,
	ososDri,
	ososSof,
	ososSos,
	ososCompressed,
	ososRst,
	ososEoi
} OJPEGStateOutState;

typedef struct {
	TIFF* tif;
	#ifndef LIBJPEG_ENCAP_EXTERNAL
	JMP_BUF exit_jmpbuf;
	#endif
	TIFFVGetMethod vgetparent;
	TIFFVSetMethod vsetparent;
	TIFFPrintMethod printdir;
	uint64 file_size;
	uint32 image_width;
	uint32 image_length;
	uint32 strile_width;
	uint32 strile_length;
	uint32 strile_length_total;
	uint8 samples_per_pixel;
	uint8 plane_sample_offset;
	uint8 samples_per_pixel_per_plane;
	uint64 jpeg_interchange_format;
	uint64 jpeg_interchange_format_length;
	uint8 jpeg_proc;
	uint8 subsamplingcorrect;
	uint8 subsamplingcorrect_done;
	uint8 subsampling_tag;
	uint8 subsampling_hor;
	uint8 subsampling_ver;
	uint8 subsampling_force_desubsampling_inside_decompression;
	uint8 qtable_offset_count;
	uint8 dctable_offset_count;
	uint8 actable_offset_count;
	uint64 qtable_offset[3];
	uint64 dctable_offset[3];
	uint64 actable_offset[3];
	uint8* qtable[4];
	uint8* dctable[4];
	uint8* actable[4];
	uint16 restart_interval;
	uint8 restart_index;
	uint8 sof_log;
	uint8 sof_marker_id;
	uint32 sof_x;
	uint32 sof_y;
	uint8 sof_c[3];
	uint8 sof_hv[3];
	uint8 sof_tq[3];
	uint8 sos_cs[3];
	uint8 sos_tda[3];
	struct {
		uint8 log;
		OJPEGStateInBufferSource in_buffer_source;
		uint32 in_buffer_next_strile;
		uint64 in_buffer_file_pos;
		uint64 in_buffer_file_togo;
	} sos_end[3];
	uint8 readheader_done;
	uint8 writeheader_done;
	uint16 write_cursample;
	uint32 write_curstrile;
	uint8 libjpeg_session_active;
	uint8 libjpeg_jpeg_query_style;
	jpeg_error_mgr libjpeg_jpeg_error_mgr;
	jpeg_decompress_struct libjpeg_jpeg_decompress_struct;
	jpeg_source_mgr libjpeg_jpeg_source_mgr;
	uint8 subsampling_convert_log;
	uint32 subsampling_convert_ylinelen;
	uint32 subsampling_convert_ylines;
	uint32 subsampling_convert_clinelen;
	uint32 subsampling_convert_clines;
	uint32 subsampling_convert_ybuflen;
	uint32 subsampling_convert_cbuflen;
	uint32 subsampling_convert_ycbcrbuflen;
	uint8* subsampling_convert_ycbcrbuf;
	uint8* subsampling_convert_ybuf;
	uint8* subsampling_convert_cbbuf;
	uint8* subsampling_convert_crbuf;
	uint32 subsampling_convert_ycbcrimagelen;
	uint8** subsampling_convert_ycbcrimage;
	uint32 subsampling_convert_clinelenout;
	uint32 subsampling_convert_state;
	uint32 bytes_per_line;   /* if the codec outputs subsampled data, a 'line' in bytes_per_line */
	uint32 lines_per_strile; /* and lines_per_strile means subsampling_ver desubsampled rows     */
	OJPEGStateInBufferSource in_buffer_source;
	uint32 in_buffer_next_strile;
	uint32 in_buffer_strile_count;
	uint64 in_buffer_file_pos;
	uint8 in_buffer_file_pos_log;
	uint64 in_buffer_file_togo;
	uint16 in_buffer_togo;
	uint8* in_buffer_cur;
	uint8 in_buffer[OJPEG_BUFFER];
	OJPEGStateOutState out_state;
	uint8 out_buffer[OJPEG_BUFFER];
	uint8* skip_buffer;
} OJPEGState;

static int OJPEGVGetField(TIFF* tif, uint32 tag, va_list ap);
static int OJPEGVSetField(TIFF* tif, uint32 tag, va_list ap);
static void OJPEGPrintDir(TIFF* tif, FILE* fd, long flags);

static int OJPEGFixupTags(TIFF* tif);
static int OJPEGSetupDecode(TIFF* tif);
static int OJPEGPreDecode(TIFF* tif, uint16 s);
static int OJPEGPreDecodeSkipRaw(TIFF* tif);
static int OJPEGPreDecodeSkipScanlines(TIFF* tif);
static int OJPEGDecode(TIFF* tif, uint8* buf, tmsize_t cc, uint16 s);
static int OJPEGDecodeRaw(TIFF* tif, uint8* buf, tmsize_t cc);
static int OJPEGDecodeScanlines(TIFF* tif, uint8* buf, tmsize_t cc);
static void OJPEGPostDecode(TIFF* tif, uint8* buf, tmsize_t cc);
static int OJPEGSetupEncode(TIFF* tif);
static int OJPEGPreEncode(TIFF* tif, uint16 s);
static int OJPEGEncode(TIFF* tif, uint8* buf, tmsize_t cc, uint16 s);
static int OJPEGPostEncode(TIFF* tif);
static void OJPEGCleanup(TIFF* tif);

static void OJPEGSubsamplingCorrect(TIFF* tif);
static int OJPEGReadHeaderInfo(TIFF* tif);
static int OJPEGReadSecondarySos(TIFF* tif, uint16 s);
static int OJPEGWriteHeaderInfo(TIFF* tif);
static void OJPEGLibjpegSessionAbort(TIFF* tif);

static int OJPEGReadHeaderInfoSec(TIFF* tif);
static int OJPEGReadHeaderInfoSecStreamDri(TIFF* tif);
static int OJPEGReadHeaderInfoSecStreamDqt(TIFF* tif);
static int OJPEGReadHeaderInfoSecStreamDht(TIFF* tif);
static int OJPEGReadHeaderInfoSecStreamSof(TIFF* tif, uint8 marker_id);
static int OJPEGReadHeaderInfoSecStreamSos(TIFF* tif);
static int OJPEGReadHeaderInfoSecTablesQTable(TIFF* tif);
static int OJPEGReadHeaderInfoSecTablesDcTable(TIFF* tif);
static int OJPEGReadHeaderInfoSecTablesAcTable(TIFF* tif);

static int OJPEGReadBufferFill(OJPEGState* sp);
static int OJPEGReadByte(OJPEGState* sp, uint8* byte);
static int OJPEGReadBytePeek(OJPEGState* sp, uint8* byte);
static void OJPEGReadByteAdvance(OJPEGState* sp);
static int OJPEGReadWord(OJPEGState* sp, uint16* word);
static int OJPEGReadBlock(OJPEGState* sp, uint16 len, void* mem);
static void OJPEGReadSkip(OJPEGState* sp, uint16 len);

static int OJPEGWriteStream(TIFF* tif, void** mem, uint32* len);
static void OJPEGWriteStreamSoi(TIFF* tif, void** mem, uint32* len);
static void OJPEGWriteStreamQTable(TIFF* tif, uint8 table_index, void** mem, uint32* len);
static void OJPEGWriteStreamDcTable(TIFF* tif, uint8 table_index, void** mem, uint32* len);
static void OJPEGWriteStreamAcTable(TIFF* tif, uint8 table_index, void** mem, uint32* len);
static void OJPEGWriteStreamDri(TIFF* tif, void** mem, uint32* len);
static void OJPEGWriteStreamSof(TIFF* tif, void** mem, uint32* len);
static void OJPEGWriteStreamSos(TIFF* tif, void** mem, uint32* len);
static int OJPEGWriteStreamCompressed(TIFF* tif, void** mem, uint32* len);
static void OJPEGWriteStreamRst(TIFF* tif, void** mem, uint32* len);
static void OJPEGWriteStreamEoi(TIFF* tif, void** mem, uint32* len);

#ifdef LIBJPEG_ENCAP_EXTERNAL
extern int jpeg_create_decompress_encap(OJPEGState* sp, jpeg_decompress_struct* cinfo);
extern int jpeg_read_header_encap(OJPEGState* sp, jpeg_decompress_struct* cinfo, uint8 require_image);
extern int jpeg_start_decompress_encap(OJPEGState* sp, jpeg_decompress_struct* cinfo);
extern int jpeg_read_scanlines_encap(OJPEGState* sp, jpeg_decompress_struct* cinfo, void* scanlines, uint32 max_lines);
extern int jpeg_read_raw_data_encap(OJPEGState* sp, jpeg_decompress_struct* cinfo, void* data, uint32 max_lines);
extern void jpeg_encap_unwind(TIFF* tif);
#else
static int jpeg_create_decompress_encap(OJPEGState* sp, jpeg_decompress_struct* j);
static int jpeg_read_header_encap(OJPEGState* sp, jpeg_decompress_struct* cinfo, uint8 require_image);
static int jpeg_start_decompress_encap(OJPEGState* sp, jpeg_decompress_struct* cinfo);
static int jpeg_read_scanlines_encap(OJPEGState* sp, jpeg_decompress_struct* cinfo, void* scanlines, uint32 max_lines);
static int jpeg_read_raw_data_encap(OJPEGState* sp, jpeg_decompress_struct* cinfo, void* data, uint32 max_lines);
static void jpeg_encap_unwind(TIFF* tif);
#endif

static void OJPEGLibjpegJpegErrorMgrOutputMessage(jpeg_common_struct* cinfo);
static void OJPEGLibjpegJpegErrorMgrErrorExit(jpeg_common_struct* cinfo);
static void OJPEGLibjpegJpegSourceMgrInitSource(jpeg_decompress_struct* cinfo);
static boolean OJPEGLibjpegJpegSourceMgrFillInputBuffer(jpeg_decompress_struct* cinfo);
static void OJPEGLibjpegJpegSourceMgrSkipInputData(jpeg_decompress_struct* cinfo, long num_bytes);
static boolean OJPEGLibjpegJpegSourceMgrResyncToRestart(jpeg_decompress_struct* cinfo, int desired);
static void OJPEGLibjpegJpegSourceMgrTermSource(jpeg_decompress_struct* cinfo);

int
TIFFInitOJPEG(TIFF* tif, int scheme)
{
	static const char module[]="TIFFInitOJPEG";
	OJPEGState* sp;

	assert(scheme==COMPRESSION_OJPEG);

        /*
	 * Merge codec-specific tag information.
	 */
	if (!_TIFFMergeFields(tif, ojpegFields, TIFFArrayCount(ojpegFields))) {
		TIFFErrorExt(tif->tif_clientdata, module,
		    "Merging Old JPEG codec-specific tags failed");
		return 0;
	}

	/* state block */
	sp=_TIFFmalloc(sizeof(OJPEGState));
	if (sp==NULL)
	{
		TIFFErrorExt(tif->tif_clientdata,module,"No space for OJPEG state block");
		return(0);
	}
	_TIFFmemset(sp,0,sizeof(OJPEGState));
	sp->tif=tif;
	sp->jpeg_proc=1;
	sp->subsampling_hor=2;
	sp->subsampling_ver=2;
	TIFFSetField(tif,TIFFTAG_YCBCRSUBSAMPLING,2,2);
	/* tif codec methods */
	tif->tif_fixuptags=OJPEGFixupTags;  
	tif->tif_setupdecode=OJPEGSetupDecode;
	tif->tif_predecode=OJPEGPreDecode;
	tif->tif_postdecode=OJPEGPostDecode;  
	tif->tif_decoderow=OJPEGDecode;  
	tif->tif_decodestrip=OJPEGDecode;  
	tif->tif_decodetile=OJPEGDecode;  
	tif->tif_setupencode=OJPEGSetupEncode;
	tif->tif_preencode=OJPEGPreEncode;
	tif->tif_postencode=OJPEGPostEncode;
	tif->tif_encoderow=OJPEGEncode;  
	tif->tif_encodestrip=OJPEGEncode;  
	tif->tif_encodetile=OJPEGEncode;  
	tif->tif_cleanup=OJPEGCleanup;
	tif->tif_data=(uint8*)sp;
	/* tif tag methods */
	sp->vgetparent=tif->tif_tagmethods.vgetfield;
	tif->tif_tagmethods.vgetfield=OJPEGVGetField;
	sp->vsetparent=tif->tif_tagmethods.vsetfield;
	tif->tif_tagmethods.vsetfield=OJPEGVSetField;
	sp->printdir=tif->tif_tagmethods.printdir;
	tif->tif_tagmethods.printdir=OJPEGPrintDir;
	/* Some OJPEG files don't have strip or tile offsets or bytecounts tags.
	   Some others do, but have totally meaningless or corrupt values
	   in these tags. In these cases, the JpegInterchangeFormat stream is
	   reliable. In any case, this decoder reads the compressed data itself,
	   from the most reliable locations, and we need to notify encapsulating
	   LibTiff not to read raw strips or tiles for us. */
	tif->tif_flags|=TIFF_NOREADRAW;
	return(1);
}

static int
OJPEGVGetField(TIFF* tif, uint32 tag, va_list ap)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	switch(tag)
	{
		case TIFFTAG_JPEGIFOFFSET:
			*va_arg(ap,uint64*)=(uint64)sp->jpeg_interchange_format;
			break;
		case TIFFTAG_JPEGIFBYTECOUNT:
			*va_arg(ap,uint64*)=(uint64)sp->jpeg_interchange_format_length;
			break;
		case TIFFTAG_YCBCRSUBSAMPLING:
			if (sp->subsamplingcorrect_done==0)
				OJPEGSubsamplingCorrect(tif);
			*va_arg(ap,uint16*)=(uint16)sp->subsampling_hor;
			*va_arg(ap,uint16*)=(uint16)sp->subsampling_ver;
			break;
		case TIFFTAG_JPEGQTABLES:
			*va_arg(ap,uint32*)=(uint32)sp->qtable_offset_count;
			*va_arg(ap,void**)=(void*)sp->qtable_offset; 
			break;
		case TIFFTAG_JPEGDCTABLES:
			*va_arg(ap,uint32*)=(uint32)sp->dctable_offset_count;
			*va_arg(ap,void**)=(void*)sp->dctable_offset;  
			break;
		case TIFFTAG_JPEGACTABLES:
			*va_arg(ap,uint32*)=(uint32)sp->actable_offset_count;
			*va_arg(ap,void**)=(void*)sp->actable_offset;
			break;
		case TIFFTAG_JPEGPROC:
			*va_arg(ap,uint16*)=(uint16)sp->jpeg_proc;
			break;
		case TIFFTAG_JPEGRESTARTINTERVAL:
			*va_arg(ap,uint16*)=sp->restart_interval;
			break;
		default:
			return (*sp->vgetparent)(tif,tag,ap);
	}
	return (1);
}

static int
OJPEGVSetField(TIFF* tif, uint32 tag, va_list ap)
{
	static const char module[]="OJPEGVSetField";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint32 ma;
	uint64* mb;
	uint32 n;
	switch(tag)
	{
		case TIFFTAG_JPEGIFOFFSET:
			sp->jpeg_interchange_format=(uint64)va_arg(ap,uint64);
			break;
		case TIFFTAG_JPEGIFBYTECOUNT:
			sp->jpeg_interchange_format_length=(uint64)va_arg(ap,uint64);
			break;
		case TIFFTAG_YCBCRSUBSAMPLING:
			sp->subsampling_tag=1;
			sp->subsampling_hor=(uint8)va_arg(ap,uint16_vap);
			sp->subsampling_ver=(uint8)va_arg(ap,uint16_vap);
			tif->tif_dir.td_ycbcrsubsampling[0]=sp->subsampling_hor;
			tif->tif_dir.td_ycbcrsubsampling[1]=sp->subsampling_ver;
			break;
		case TIFFTAG_JPEGQTABLES:
			ma=(uint32)va_arg(ap,uint32);
			if (ma!=0)
			{
				if (ma>3)
				{
					TIFFErrorExt(tif->tif_clientdata,module,"JpegQTables tag has incorrect count");
					return(0);
				}
				sp->qtable_offset_count=(uint8)ma;
				mb=(uint64*)va_arg(ap,uint64*);
				for (n=0; n<ma; n++)
					sp->qtable_offset[n]=mb[n];
			}
			break;
		case TIFFTAG_JPEGDCTABLES:
			ma=(uint32)va_arg(ap,uint32);
			if (ma!=0)
			{
				if (ma>3)
				{
					TIFFErrorExt(tif->tif_clientdata,module,"JpegDcTables tag has incorrect count");
					return(0);
				}
				sp->dctable_offset_count=(uint8)ma;
				mb=(uint64*)va_arg(ap,uint64*);
				for (n=0; n<ma; n++)
					sp->dctable_offset[n]=mb[n];
			}
			break;
		case TIFFTAG_JPEGACTABLES:
			ma=(uint32)va_arg(ap,uint32);
			if (ma!=0)
			{
				if (ma>3)
				{
					TIFFErrorExt(tif->tif_clientdata,module,"JpegAcTables tag has incorrect count");
					return(0);
				}
				sp->actable_offset_count=(uint8)ma;
				mb=(uint64*)va_arg(ap,uint64*);
				for (n=0; n<ma; n++)
					sp->actable_offset[n]=mb[n];
			}
			break;
		case TIFFTAG_JPEGPROC:
			sp->jpeg_proc=(uint8)va_arg(ap,uint16_vap);
			break;
		case TIFFTAG_JPEGRESTARTINTERVAL:
			sp->restart_interval=(uint16)va_arg(ap,uint16_vap);
			break;
		default:
			return (*sp->vsetparent)(tif,tag,ap);
	}
	TIFFSetFieldBit(tif,TIFFFieldWithTag(tif,tag)->field_bit);
	tif->tif_flags|=TIFF_DIRTYDIRECT;
	return(1);
}

static void
OJPEGPrintDir(TIFF* tif, FILE* fd, long flags)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint8 m;
	(void)flags;
	assert(sp!=NULL);
	if (TIFFFieldSet(tif,FIELD_OJPEG_JPEGINTERCHANGEFORMAT))
		fprintf(fd,"  JpegInterchangeFormat: " TIFF_UINT64_FORMAT "\n",(TIFF_UINT64_T)sp->jpeg_interchange_format);  
	if (TIFFFieldSet(tif,FIELD_OJPEG_JPEGINTERCHANGEFORMATLENGTH))
		fprintf(fd,"  JpegInterchangeFormatLength: " TIFF_UINT64_FORMAT "\n",(TIFF_UINT64_T)sp->jpeg_interchange_format_length);  
	if (TIFFFieldSet(tif,FIELD_OJPEG_JPEGQTABLES))
	{
		fprintf(fd,"  JpegQTables:");
		for (m=0; m<sp->qtable_offset_count; m++)
			fprintf(fd," " TIFF_UINT64_FORMAT,(TIFF_UINT64_T)sp->qtable_offset[m]);
		fprintf(fd,"\n");
	}
	if (TIFFFieldSet(tif,FIELD_OJPEG_JPEGDCTABLES))
	{
		fprintf(fd,"  JpegDcTables:");
		for (m=0; m<sp->dctable_offset_count; m++)
			fprintf(fd," " TIFF_UINT64_FORMAT,(TIFF_UINT64_T)sp->dctable_offset[m]);
		fprintf(fd,"\n");
	}
	if (TIFFFieldSet(tif,FIELD_OJPEG_JPEGACTABLES))
	{
		fprintf(fd,"  JpegAcTables:");
		for (m=0; m<sp->actable_offset_count; m++)
			fprintf(fd," " TIFF_UINT64_FORMAT,(TIFF_UINT64_T)sp->actable_offset[m]);
		fprintf(fd,"\n");
	}
	if (TIFFFieldSet(tif,FIELD_OJPEG_JPEGPROC))
		fprintf(fd,"  JpegProc: %u\n",(unsigned int)sp->jpeg_proc);
	if (TIFFFieldSet(tif,FIELD_OJPEG_JPEGRESTARTINTERVAL))
		fprintf(fd,"  JpegRestartInterval: %u\n",(unsigned int)sp->restart_interval);
	if (sp->printdir)
		(*sp->printdir)(tif, fd, flags);
}

static int
OJPEGFixupTags(TIFF* tif)
{
	(void) tif;
	return(1);
}

static int
OJPEGSetupDecode(TIFF* tif)
{
	static const char module[]="OJPEGSetupDecode";
	TIFFWarningExt(tif->tif_clientdata,module,"Depreciated and troublesome old-style JPEG compression mode, please convert to new-style JPEG compression and notify vendor of writing software");
	return(1);
}

static int
OJPEGPreDecode(TIFF* tif, uint16 s)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint32 m;
	if (sp->subsamplingcorrect_done==0)
		OJPEGSubsamplingCorrect(tif);
	if (sp->readheader_done==0)
	{
		if (OJPEGReadHeaderInfo(tif)==0)
			return(0);
	}
	if (sp->sos_end[s].log==0)
	{
		if (OJPEGReadSecondarySos(tif,s)==0)
			return(0);
	}
	if isTiled(tif)
		m=tif->tif_curtile;
	else
		m=tif->tif_curstrip;
	if ((sp->writeheader_done!=0) && ((sp->write_cursample!=s) || (sp->write_curstrile>m)))
	{
		if (sp->libjpeg_session_active!=0)
			OJPEGLibjpegSessionAbort(tif);
		sp->writeheader_done=0;
	}
	if (sp->writeheader_done==0)
	{
		sp->plane_sample_offset=(uint8)s;
		sp->write_cursample=s;
		sp->write_curstrile=s*tif->tif_dir.td_stripsperimage;
		if ((sp->in_buffer_file_pos_log==0) ||
		    (sp->in_buffer_file_pos-sp->in_buffer_togo!=sp->sos_end[s].in_buffer_file_pos))
		{
			sp->in_buffer_source=sp->sos_end[s].in_buffer_source;
			sp->in_buffer_next_strile=sp->sos_end[s].in_buffer_next_strile;
			sp->in_buffer_file_pos=sp->sos_end[s].in_buffer_file_pos;
			sp->in_buffer_file_pos_log=0;
			sp->in_buffer_file_togo=sp->sos_end[s].in_buffer_file_togo;
			sp->in_buffer_togo=0;
			sp->in_buffer_cur=0;
		}
		if (OJPEGWriteHeaderInfo(tif)==0)
			return(0);
	}
	while (sp->write_curstrile<m)          
	{
		if (sp->libjpeg_jpeg_query_style==0)
		{
			if (OJPEGPreDecodeSkipRaw(tif)==0)
				return(0);
		}
		else
		{
			if (OJPEGPreDecodeSkipScanlines(tif)==0)
				return(0);
		}
		sp->write_curstrile++;
	}
	return(1);
}

static int
OJPEGPreDecodeSkipRaw(TIFF* tif)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint32 m;
	m=sp->lines_per_strile;
	if (sp->subsampling_convert_state!=0)
	{
		if (sp->subsampling_convert_clines-sp->subsampling_convert_state>=m)
		{
			sp->subsampling_convert_state+=m;
			if (sp->subsampling_convert_state==sp->subsampling_convert_clines)
				sp->subsampling_convert_state=0;
			return(1);
		}
		m-=sp->subsampling_convert_clines-sp->subsampling_convert_state;
		sp->subsampling_convert_state=0;
	}
	while (m>=sp->subsampling_convert_clines)
	{
		if (jpeg_read_raw_data_encap(sp,&(sp->libjpeg_jpeg_decompress_struct),sp->subsampling_convert_ycbcrimage,sp->subsampling_ver*8)==0)
			return(0);
		m-=sp->subsampling_convert_clines;
	}
	if (m>0)
	{
		if (jpeg_read_raw_data_encap(sp,&(sp->libjpeg_jpeg_decompress_struct),sp->subsampling_convert_ycbcrimage,sp->subsampling_ver*8)==0)
			return(0);
		sp->subsampling_convert_state=m;
	}
	return(1);
}

static int
OJPEGPreDecodeSkipScanlines(TIFF* tif)
{
	static const char module[]="OJPEGPreDecodeSkipScanlines";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint32 m;
	if (sp->skip_buffer==NULL)
	{
		sp->skip_buffer=_TIFFmalloc(sp->bytes_per_line);
		if (sp->skip_buffer==NULL)
		{
			TIFFErrorExt(tif->tif_clientdata,module,"Out of memory");
			return(0);
		}
	}
	for (m=0; m<sp->lines_per_strile; m++)
	{
		if (jpeg_read_scanlines_encap(sp,&(sp->libjpeg_jpeg_decompress_struct),&sp->skip_buffer,1)==0)
			return(0);
	}
	return(1);
}

static int
OJPEGDecode(TIFF* tif, uint8* buf, tmsize_t cc, uint16 s)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	(void)s;
	if (sp->libjpeg_jpeg_query_style==0)
	{
		if (OJPEGDecodeRaw(tif,buf,cc)==0)
			return(0);
	}
	else
	{
		if (OJPEGDecodeScanlines(tif,buf,cc)==0)
			return(0);
	}
	return(1);
}

static int
OJPEGDecodeRaw(TIFF* tif, uint8* buf, tmsize_t cc)
{
	static const char module[]="OJPEGDecodeRaw";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint8* m;
	tmsize_t n;
	uint8* oy;
	uint8* ocb;
	uint8* ocr;
	uint8* p;
	uint32 q;
	uint8* r;
	uint8 sx,sy;
	if (cc%sp->bytes_per_line!=0)
	{
		TIFFErrorExt(tif->tif_clientdata,module,"Fractional scanline not read");
		return(0);
	}
	assert(cc>0);
	m=buf;
	n=cc;
	do
	{
		if (sp->subsampling_convert_state==0)
		{
			if (jpeg_read_raw_data_encap(sp,&(sp->libjpeg_jpeg_decompress_struct),sp->subsampling_convert_ycbcrimage,sp->subsampling_ver*8)==0)
				return(0);
		}
		oy=sp->subsampling_convert_ybuf+sp->subsampling_convert_state*sp->subsampling_ver*sp->subsampling_convert_ylinelen;
		ocb=sp->subsampling_convert_cbbuf+sp->subsampling_convert_state*sp->subsampling_convert_clinelen;
		ocr=sp->subsampling_convert_crbuf+sp->subsampling_convert_state*sp->subsampling_convert_clinelen;
		p=m;
		for (q=0; q<sp->subsampling_convert_clinelenout; q++)
		{
			r=oy;
			for (sy=0; sy<sp->subsampling_ver; sy++)
			{
				for (sx=0; sx<sp->subsampling_hor; sx++)
					*p++=*r++;
				r+=sp->subsampling_convert_ylinelen-sp->subsampling_hor;
			}
			oy+=sp->subsampling_hor;
			*p++=*ocb++;
			*p++=*ocr++;
		}
		sp->subsampling_convert_state++;
		if (sp->subsampling_convert_state==sp->subsampling_convert_clines)
			sp->subsampling_convert_state=0;
		m+=sp->bytes_per_line;
		n-=sp->bytes_per_line;
	} while(n>0);
	return(1);
}

static int
OJPEGDecodeScanlines(TIFF* tif, uint8* buf, tmsize_t cc)
{
	static const char module[]="OJPEGDecodeScanlines";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint8* m;
	tmsize_t n;
	if (cc%sp->bytes_per_line!=0)
	{
		TIFFErrorExt(tif->tif_clientdata,module,"Fractional scanline not read");
		return(0);
	}
	assert(cc>0);
	m=buf;
	n=cc;
	do
	{
		if (jpeg_read_scanlines_encap(sp,&(sp->libjpeg_jpeg_decompress_struct),&m,1)==0)
			return(0);
		m+=sp->bytes_per_line;
		n-=sp->bytes_per_line;
	} while(n>0);
	return(1);
}

static void
OJPEGPostDecode(TIFF* tif, uint8* buf, tmsize_t cc)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	(void)buf;
	(void)cc;
	sp->write_curstrile++;
	if (sp->write_curstrile%tif->tif_dir.td_stripsperimage==0)  
	{
		assert(sp->libjpeg_session_active!=0);
		OJPEGLibjpegSessionAbort(tif);
		sp->writeheader_done=0;
	}
}

static int
OJPEGSetupEncode(TIFF* tif)
{
	static const char module[]="OJPEGSetupEncode";
	TIFFErrorExt(tif->tif_clientdata,module,"OJPEG encoding not supported; use new-style JPEG compression instead");
	return(0);
}

static int
OJPEGPreEncode(TIFF* tif, uint16 s)
{
	static const char module[]="OJPEGPreEncode";
	(void)s;
	TIFFErrorExt(tif->tif_clientdata,module,"OJPEG encoding not supported; use new-style JPEG compression instead");
	return(0);
}

static int
OJPEGEncode(TIFF* tif, uint8* buf, tmsize_t cc, uint16 s)
{
	static const char module[]="OJPEGEncode";
	(void)buf;
	(void)cc;
	(void)s;
	TIFFErrorExt(tif->tif_clientdata,module,"OJPEG encoding not supported; use new-style JPEG compression instead");
	return(0);
}

static int
OJPEGPostEncode(TIFF* tif)
{
	static const char module[]="OJPEGPostEncode";
	TIFFErrorExt(tif->tif_clientdata,module,"OJPEG encoding not supported; use new-style JPEG compression instead");
	return(0);
}

static void
OJPEGCleanup(TIFF* tif)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	if (sp!=0)
	{
		tif->tif_tagmethods.vgetfield=sp->vgetparent;
		tif->tif_tagmethods.vsetfield=sp->vsetparent;
		tif->tif_tagmethods.printdir=sp->printdir;
		if (sp->qtable[0]!=0)
			_TIFFfree(sp->qtable[0]);
		if (sp->qtable[1]!=0)
			_TIFFfree(sp->qtable[1]);
		if (sp->qtable[2]!=0)
			_TIFFfree(sp->qtable[2]);
		if (sp->qtable[3]!=0)
			_TIFFfree(sp->qtable[3]);
		if (sp->dctable[0]!=0)
			_TIFFfree(sp->dctable[0]);
		if (sp->dctable[1]!=0)
			_TIFFfree(sp->dctable[1]);
		if (sp->dctable[2]!=0)
			_TIFFfree(sp->dctable[2]);
		if (sp->dctable[3]!=0)
			_TIFFfree(sp->dctable[3]);
		if (sp->actable[0]!=0)
			_TIFFfree(sp->actable[0]);
		if (sp->actable[1]!=0)
			_TIFFfree(sp->actable[1]);
		if (sp->actable[2]!=0)
			_TIFFfree(sp->actable[2]);
		if (sp->actable[3]!=0)
			_TIFFfree(sp->actable[3]);
		if (sp->libjpeg_session_active!=0)
			OJPEGLibjpegSessionAbort(tif);
		if (sp->subsampling_convert_ycbcrbuf!=0)
			_TIFFfree(sp->subsampling_convert_ycbcrbuf);
		if (sp->subsampling_convert_ycbcrimage!=0)
			_TIFFfree(sp->subsampling_convert_ycbcrimage);
		if (sp->skip_buffer!=0)
			_TIFFfree(sp->skip_buffer);
		_TIFFfree(sp);
		tif->tif_data=NULL;
		_TIFFSetDefaultCompressionState(tif);
	}
}

static void
OJPEGSubsamplingCorrect(TIFF* tif)
{
	static const char module[]="OJPEGSubsamplingCorrect";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint8 mh;
	uint8 mv;
        _TIFFFillStriles( tif );
        
	assert(sp->subsamplingcorrect_done==0);
	if ((tif->tif_dir.td_samplesperpixel!=3) || ((tif->tif_dir.td_photometric!=PHOTOMETRIC_YCBCR) &&
	    (tif->tif_dir.td_photometric!=PHOTOMETRIC_ITULAB)))
	{
		if (sp->subsampling_tag!=0)
			TIFFWarningExt(tif->tif_clientdata,module,"Subsampling tag not appropriate for this Photometric and/or SamplesPerPixel");
		sp->subsampling_hor=1;
		sp->subsampling_ver=1;
		sp->subsampling_force_desubsampling_inside_decompression=0;
	}
	else
	{
		sp->subsamplingcorrect_done=1;
		mh=sp->subsampling_hor;
		mv=sp->subsampling_ver;
		sp->subsamplingcorrect=1;
		OJPEGReadHeaderInfoSec(tif);
		if (sp->subsampling_force_desubsampling_inside_decompression!=0)
		{
			sp->subsampling_hor=1;
			sp->subsampling_ver=1;
		}
		sp->subsamplingcorrect=0;
		if (((sp->subsampling_hor!=mh) || (sp->subsampling_ver!=mv)) && (sp->subsampling_force_desubsampling_inside_decompression==0))
		{
			if (sp->subsampling_tag==0)
				TIFFWarningExt(tif->tif_clientdata,module,"Subsampling tag is not set, yet subsampling inside JPEG data [%d,%d] does not match default values [2,2]; assuming subsampling inside JPEG data is correct",sp->subsampling_hor,sp->subsampling_ver);
			else
				TIFFWarningExt(tif->tif_clientdata,module,"Subsampling inside JPEG data [%d,%d] does not match subsampling tag values [%d,%d]; assuming subsampling inside JPEG data is correct",sp->subsampling_hor,sp->subsampling_ver,mh,mv);
		}
		if (sp->subsampling_force_desubsampling_inside_decompression!=0)
		{
			if (sp->subsampling_tag==0)
				TIFFWarningExt(tif->tif_clientdata,module,"Subsampling tag is not set, yet subsampling inside JPEG data does not match default values [2,2] (nor any other values allowed in TIFF); assuming subsampling inside JPEG data is correct and desubsampling inside JPEG decompression");
			else
				TIFFWarningExt(tif->tif_clientdata,module,"Subsampling inside JPEG data does not match subsampling tag values [%d,%d] (nor any other values allowed in TIFF); assuming subsampling inside JPEG data is correct and desubsampling inside JPEG decompression",mh,mv);
		}
		if (sp->subsampling_force_desubsampling_inside_decompression==0)
		{
			if (sp->subsampling_hor<sp->subsampling_ver)
				TIFFWarningExt(tif->tif_clientdata,module,"Subsampling values [%d,%d] are not allowed in TIFF",sp->subsampling_hor,sp->subsampling_ver);
		}
	}
	sp->subsamplingcorrect_done=1;
}

static int
OJPEGReadHeaderInfo(TIFF* tif)
{
	static const char module[]="OJPEGReadHeaderInfo";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	assert(sp->readheader_done==0);
	sp->image_width=tif->tif_dir.td_imagewidth;
	sp->image_length=tif->tif_dir.td_imagelength;
	if isTiled(tif)
	{
		sp->strile_width=tif->tif_dir.td_tilewidth;
		sp->strile_length=tif->tif_dir.td_tilelength;
		sp->strile_length_total=((sp->image_length+sp->strile_length-1)/sp->strile_length)*sp->strile_length;
	}
	else
	{
		sp->strile_width=sp->image_width;
		sp->strile_length=tif->tif_dir.td_rowsperstrip;
		sp->strile_length_total=sp->image_length;
	}
	if (tif->tif_dir.td_samplesperpixel==1)
	{
		sp->samples_per_pixel=1;
		sp->plane_sample_offset=0;
		sp->samples_per_pixel_per_plane=sp->samples_per_pixel;
		sp->subsampling_hor=1;
		sp->subsampling_ver=1;
	}
	else
	{
		if (tif->tif_dir.td_samplesperpixel!=3)
		{
			TIFFErrorExt(tif->tif_clientdata,module,"SamplesPerPixel %d not supported for this compression scheme",sp->samples_per_pixel);
			return(0);
		}
		sp->samples_per_pixel=3;
		sp->plane_sample_offset=0;
		if (tif->tif_dir.td_planarconfig==PLANARCONFIG_CONTIG)
			sp->samples_per_pixel_per_plane=3;
		else
			sp->samples_per_pixel_per_plane=1;
	}
	if (sp->strile_length<sp->image_length)
	{
		if (sp->strile_length%(sp->subsampling_ver*8)!=0)
		{
			TIFFErrorExt(tif->tif_clientdata,module,"Incompatible vertical subsampling and image strip/tile length");
			return(0);
		}
		sp->restart_interval=((sp->strile_width+sp->subsampling_hor*8-1)/(sp->subsampling_hor*8))*(sp->strile_length/(sp->subsampling_ver*8));
	}
	if (OJPEGReadHeaderInfoSec(tif)==0)
		return(0);
	sp->sos_end[0].log=1;
	sp->sos_end[0].in_buffer_source=sp->in_buffer_source;
	sp->sos_end[0].in_buffer_next_strile=sp->in_buffer_next_strile;
	sp->sos_end[0].in_buffer_file_pos=sp->in_buffer_file_pos-sp->in_buffer_togo;
	sp->sos_end[0].in_buffer_file_togo=sp->in_buffer_file_togo+sp->in_buffer_togo; 
	sp->readheader_done=1;
	return(1);
}

static int
OJPEGReadSecondarySos(TIFF* tif, uint16 s)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint8 m;
	assert(s>0);
	assert(s<3);
	assert(sp->sos_end[0].log!=0);
	assert(sp->sos_end[s].log==0);
	sp->plane_sample_offset=s-1;
	while(sp->sos_end[sp->plane_sample_offset].log==0)
		sp->plane_sample_offset--;
	sp->in_buffer_source=sp->sos_end[sp->plane_sample_offset].in_buffer_source;
	sp->in_buffer_next_strile=sp->sos_end[sp->plane_sample_offset].in_buffer_next_strile;
	sp->in_buffer_file_pos=sp->sos_end[sp->plane_sample_offset].in_buffer_file_pos;
	sp->in_buffer_file_pos_log=0;
	sp->in_buffer_file_togo=sp->sos_end[sp->plane_sample_offset].in_buffer_file_togo;
	sp->in_buffer_togo=0;
	sp->in_buffer_cur=0;
	while(sp->plane_sample_offset<s)
	{
		do
		{
			if (OJPEGReadByte(sp,&m)==0)
				return(0);
			if (m==255)
			{
				do
				{
					if (OJPEGReadByte(sp,&m)==0)
						return(0);
					if (m!=255)
						break;
				} while(1);
				if (m==JPEG_MARKER_SOS)
					break;
			}
		} while(1);
		sp->plane_sample_offset++;
		if (OJPEGReadHeaderInfoSecStreamSos(tif)==0)
			return(0);
		sp->sos_end[sp->plane_sample_offset].log=1;
		sp->sos_end[sp->plane_sample_offset].in_buffer_source=sp->in_buffer_source;
		sp->sos_end[sp->plane_sample_offset].in_buffer_next_strile=sp->in_buffer_next_strile;
		sp->sos_end[sp->plane_sample_offset].in_buffer_file_pos=sp->in_buffer_file_pos-sp->in_buffer_togo;
		sp->sos_end[sp->plane_sample_offset].in_buffer_file_togo=sp->in_buffer_file_togo+sp->in_buffer_togo;
	}
	return(1);
}

static int
OJPEGWriteHeaderInfo(TIFF* tif)
{
	static const char module[]="OJPEGWriteHeaderInfo";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint8** m;
	uint32 n;
	/* if a previous attempt failed, don't try again */
	if (sp->libjpeg_session_active != 0) 
		return 0;
	sp->out_state=ososSoi;
	sp->restart_index=0;
	jpeg_std_error(&(sp->libjpeg_jpeg_error_mgr));
	sp->libjpeg_jpeg_error_mgr.output_message=OJPEGLibjpegJpegErrorMgrOutputMessage;
	sp->libjpeg_jpeg_error_mgr.error_exit=OJPEGLibjpegJpegErrorMgrErrorExit;
	sp->libjpeg_jpeg_decompress_struct.err=&(sp->libjpeg_jpeg_error_mgr);
	sp->libjpeg_jpeg_decompress_struct.client_data=(void*)tif;
	if (jpeg_create_decompress_encap(sp,&(sp->libjpeg_jpeg_decompress_struct))==0)
		return(0);
	sp->libjpeg_session_active=1;
	sp->libjpeg_jpeg_source_mgr.bytes_in_buffer=0;
	sp->libjpeg_jpeg_source_mgr.init_source=OJPEGLibjpegJpegSourceMgrInitSource;
	sp->libjpeg_jpeg_source_mgr.fill_input_buffer=OJPEGLibjpegJpegSourceMgrFillInputBuffer;
	sp->libjpeg_jpeg_source_mgr.skip_input_data=OJPEGLibjpegJpegSourceMgrSkipInputData;
	sp->libjpeg_jpeg_source_mgr.resync_to_restart=OJPEGLibjpegJpegSourceMgrResyncToRestart;
	sp->libjpeg_jpeg_source_mgr.term_source=OJPEGLibjpegJpegSourceMgrTermSource;
	sp->libjpeg_jpeg_decompress_struct.src=&(sp->libjpeg_jpeg_source_mgr);
	if (jpeg_read_header_encap(sp,&(sp->libjpeg_jpeg_decompress_struct),1)==0)
		return(0);
	if ((sp->subsampling_force_desubsampling_inside_decompression==0) && (sp->samples_per_pixel_per_plane>1))
	{
		sp->libjpeg_jpeg_decompress_struct.raw_data_out=1;
#if JPEG_LIB_VERSION >= 70
		sp->libjpeg_jpeg_decompress_struct.do_fancy_upsampling=FALSE;
#endif
		sp->libjpeg_jpeg_query_style=0;
		if (sp->subsampling_convert_log==0)
		{
			assert(sp->subsampling_convert_ycbcrbuf==0);
			assert(sp->subsampling_convert_ycbcrimage==0);
			sp->subsampling_convert_ylinelen=((sp->strile_width+sp->subsampling_hor*8-1)/(sp->subsampling_hor*8)*sp->subsampling_hor*8);
			sp->subsampling_convert_ylines=sp->subsampling_ver*8;
			sp->subsampling_convert_clinelen=sp->subsampling_convert_ylinelen/sp->subsampling_hor;
			sp->subsampling_convert_clines=8;
			sp->subsampling_convert_ybuflen=sp->subsampling_convert_ylinelen*sp->subsampling_convert_ylines;
			sp->subsampling_convert_cbuflen=sp->subsampling_convert_clinelen*sp->subsampling_convert_clines;
			sp->subsampling_convert_ycbcrbuflen=sp->subsampling_convert_ybuflen+2*sp->subsampling_convert_cbuflen;
			sp->subsampling_convert_ycbcrbuf=_TIFFmalloc(sp->subsampling_convert_ycbcrbuflen);
			if (sp->subsampling_convert_ycbcrbuf==0)
			{
				TIFFErrorExt(tif->tif_clientdata,module,"Out of memory");
				return(0);
			}
			sp->subsampling_convert_ybuf=sp->subsampling_convert_ycbcrbuf;
			sp->subsampling_convert_cbbuf=sp->subsampling_convert_ybuf+sp->subsampling_convert_ybuflen;
			sp->subsampling_convert_crbuf=sp->subsampling_convert_cbbuf+sp->subsampling_convert_cbuflen;
			sp->subsampling_convert_ycbcrimagelen=3+sp->subsampling_convert_ylines+2*sp->subsampling_convert_clines;
			sp->subsampling_convert_ycbcrimage=_TIFFmalloc(sp->subsampling_convert_ycbcrimagelen*sizeof(uint8*));
			if (sp->subsampling_convert_ycbcrimage==0)
			{
				TIFFErrorExt(tif->tif_clientdata,module,"Out of memory");
				return(0);
			}
			m=sp->subsampling_convert_ycbcrimage;
			*m++=(uint8*)(sp->subsampling_convert_ycbcrimage+3);
			*m++=(uint8*)(sp->subsampling_convert_ycbcrimage+3+sp->subsampling_convert_ylines);
			*m++=(uint8*)(sp->subsampling_convert_ycbcrimage+3+sp->subsampling_convert_ylines+sp->subsampling_convert_clines);
			for (n=0; n<sp->subsampling_convert_ylines; n++)
				*m++=sp->subsampling_convert_ybuf+n*sp->subsampling_convert_ylinelen;
			for (n=0; n<sp->subsampling_convert_clines; n++)
				*m++=sp->subsampling_convert_cbbuf+n*sp->subsampling_convert_clinelen;
			for (n=0; n<sp->subsampling_convert_clines; n++)
				*m++=sp->subsampling_convert_crbuf+n*sp->subsampling_convert_clinelen;
			sp->subsampling_convert_clinelenout=((sp->strile_width+sp->subsampling_hor-1)/sp->subsampling_hor);
			sp->subsampling_convert_state=0;
			sp->bytes_per_line=sp->subsampling_convert_clinelenout*(sp->subsampling_ver*sp->subsampling_hor+2);
			sp->lines_per_strile=((sp->strile_length+sp->subsampling_ver-1)/sp->subsampling_ver);
			sp->subsampling_convert_log=1;
		}
	}
	else
	{
		sp->libjpeg_jpeg_decompress_struct.jpeg_color_space=JCS_UNKNOWN;
		sp->libjpeg_jpeg_decompress_struct.out_color_space=JCS_UNKNOWN;
		sp->libjpeg_jpeg_query_style=1;
		sp->bytes_per_line=sp->samples_per_pixel_per_plane*sp->strile_width;
		sp->lines_per_strile=sp->strile_length;
	}
	if (jpeg_start_decompress_encap(sp,&(sp->libjpeg_jpeg_decompress_struct))==0)
		return(0);
	sp->writeheader_done=1;
	return(1);
}

static void
OJPEGLibjpegSessionAbort(TIFF* tif)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	assert(sp->libjpeg_session_active!=0);
	jpeg_destroy((jpeg_common_struct*)(&(sp->libjpeg_jpeg_decompress_struct)));
	sp->libjpeg_session_active=0;
}

static int
<<<<<<< HEAD
OJPEGReadHeaderInfoSec(TIFF* tif)
{
	static const char module[]="OJPEGReadHeaderInfoSec";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint8 m;
	uint16 n;
	uint8 o;
	if (sp->file_size==0)
		sp->file_size=TIFFGetFileSize(tif);
	if (sp->jpeg_interchange_format!=0)
	{
		if (sp->jpeg_interchange_format>=sp->file_size)
		{
			sp->jpeg_interchange_format=0;
			sp->jpeg_interchange_format_length=0;
		}
		else
		{
			if ((sp->jpeg_interchange_format_length==0) || (sp->jpeg_interchange_format+sp->jpeg_interchange_format_length>sp->file_size))
				sp->jpeg_interchange_format_length=sp->file_size-sp->jpeg_interchange_format;
		}
	}
	sp->in_buffer_source=osibsNotSetYet;
	sp->in_buffer_next_strile=0;
	sp->in_buffer_strile_count=tif->tif_dir.td_nstrips;
	sp->in_buffer_file_togo=0;
	sp->in_buffer_togo=0;
	do
	{
		if (OJPEGReadBytePeek(sp,&m)==0)
			return(0);
		if (m!=255)
			break;
		OJPEGReadByteAdvance(sp);
		do
		{
			if (OJPEGReadByte(sp,&m)==0)
				return(0);
		} while(m==255);
		switch(m)
		{
			case JPEG_MARKER_SOI:
				/* this type of marker has no data, and should be skipped */
				break;
			case JPEG_MARKER_COM:
			case JPEG_MARKER_APP0:
			case JPEG_MARKER_APP0+1:
			case JPEG_MARKER_APP0+2:
			case JPEG_MARKER_APP0+3:
			case JPEG_MARKER_APP0+4:
			case JPEG_MARKER_APP0+5:
			case JPEG_MARKER_APP0+6:
			case JPEG_MARKER_APP0+7:
			case JPEG_MARKER_APP0+8:
			case JPEG_MARKER_APP0+9:
			case JPEG_MARKER_APP0+10:
			case JPEG_MARKER_APP0+11:
			case JPEG_MARKER_APP0+12:
			case JPEG_MARKER_APP0+13:
			case JPEG_MARKER_APP0+14:
			case JPEG_MARKER_APP0+15:
				/* this type of marker has data, but it has no use to us (and no place here) and should be skipped */
				if (OJPEGReadWord(sp,&n)==0)
					return(0);
				if (n<2)
				{
					if (sp->subsamplingcorrect==0)
						TIFFErrorExt(tif->tif_clientdata,module,"Corrupt JPEG data");
					return(0);
				}
				if (n>2)
					OJPEGReadSkip(sp,n-2);
				break;
			case JPEG_MARKER_DRI:
				if (OJPEGReadHeaderInfoSecStreamDri(tif)==0)
					return(0);
				break;
			case JPEG_MARKER_DQT:
				if (OJPEGReadHeaderInfoSecStreamDqt(tif)==0)
					return(0);
				break;
			case JPEG_MARKER_DHT:
				if (OJPEGReadHeaderInfoSecStreamDht(tif)==0)
					return(0);
				break;
			case JPEG_MARKER_SOF0:
			case JPEG_MARKER_SOF1:
			case JPEG_MARKER_SOF3:
				if (OJPEGReadHeaderInfoSecStreamSof(tif,m)==0)
					return(0);
				if (sp->subsamplingcorrect!=0)
					return(1);
				break;
			case JPEG_MARKER_SOS:
				if (sp->subsamplingcorrect!=0)
					return(1);
				assert(sp->plane_sample_offset==0);
				if (OJPEGReadHeaderInfoSecStreamSos(tif)==0)
					return(0);
				break;
			default:
				TIFFErrorExt(tif->tif_clientdata,module,"Unknown marker type %d in JPEG data",m);
				return(0);
		}
	} while(m!=JPEG_MARKER_SOS);
	if (sp->subsamplingcorrect)
		return(1);
	if (sp->sof_log==0)
	{
		if (OJPEGReadHeaderInfoSecTablesQTable(tif)==0)
			return(0);
		sp->sof_marker_id=JPEG_MARKER_SOF0;
		for (o=0; o<sp->samples_per_pixel; o++)
			sp->sof_c[o]=o;
		sp->sof_hv[0]=((sp->subsampling_hor<<4)|sp->subsampling_ver);
		for (o=1; o<sp->samples_per_pixel; o++)
			sp->sof_hv[o]=17;
		sp->sof_x=sp->strile_width;
		sp->sof_y=sp->strile_length_total;
		sp->sof_log=1;
		if (OJPEGReadHeaderInfoSecTablesDcTable(tif)==0)
			return(0);
		if (OJPEGReadHeaderInfoSecTablesAcTable(tif)==0)
			return(0);
		for (o=1; o<sp->samples_per_pixel; o++)
			sp->sos_cs[o]=o;
	}
	return(1);
}
=======
alloc_downsampled_buffers(TIFF *tif,jpeg_component_info *comp_info,
                          int num_components)
  { register OJPEGState *sp = OJState(tif);

    sp->samplesperclump = 0;
    if (num_components > 0)
      { tsize_t size = sp->cinfo.comm.is_decompressor
#                    ifdef D_LOSSLESS_SUPPORTED
                     ? sp->cinfo.d.min_codec_data_unit
#                    else
                     ? DCTSIZE
#                    endif
#                    ifdef C_LOSSLESS_SUPPORTED
                     : sp->cinfo.c.data_unit;
#                    else
                     : DCTSIZE;
#                    endif
        int ci = 0;
        register jpeg_component_info *compptr = comp_info;

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
        do
          { JSAMPARRAY buf;

            sp->samplesperclump +=
              compptr->h_samp_factor * compptr->v_samp_factor;
#           if defined(C_LOSSLESS_SUPPORTED) || defined(D_LOSSLESS_SUPPORTED)
            if (!(buf = CALLJPEG(sp,0,(*sp->cinfo.comm.mem->alloc_sarray)(&sp->cinfo.comm,JPOOL_IMAGE,compptr->width_in_data_units*size,compptr->v_samp_factor*size))))
#           else
            if (!(buf = CALLJPEG(sp,0,(*sp->cinfo.comm.mem->alloc_sarray)(&sp->cinfo.comm,JPOOL_IMAGE,compptr->width_in_blocks*size,compptr->v_samp_factor*size))))
#           endif
              return 0;
            sp->ds_buffer[ci] = buf;
          }
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
        while (++compptr,++ci < num_components);
      };
    return 1;
  }
#ifdef never

/* JPEG Encoding begins here. */

/*ARGSUSED*/ static int
OJPEGEncode(register TIFF *tif,tidata_t buf,tsize_t cc,tsample_t s)
  { tsize_t rows;                          /* No. of unprocessed rows in file */
    register OJPEGState *sp = OJState(tif);

 /* Encode a chunk of pixels, where returned data is NOT down-sampled (the
    standard case).  The data is expected to be written in scan-line multiples.
 */
    if (cc % sp->bytesperline) TIFFWarning(tif->tif_name,no_write_frac);
    if ( (cc /= bytesperline)      /* No. of complete rows in caller's buffer */
       > (rows = sp->cinfo.c.image_height - sp->cinfo.c.next_scanline)
       ) cc = rows;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
    while (--cc >= 0)
      {
        if (   CALLJPEG(sp,-1,jpeg_write_scanlines(&sp->cinfo.c,(JSAMPARRAY)&buf,1))
            != 1
           ) return 0;
        ++tif->tif_row;
        buf += sp->bytesperline;
      };
    return 1;
  }

/*ARGSUSED*/ static int
OJPEGEncodeRaw(register TIFF *tif,tidata_t buf,tsize_t cc,tsample_t s)
  { tsize_t rows;                          /* No. of unprocessed rows in file */
    JDIMENSION lines_per_MCU, size;
    register OJPEGState *sp = OJState(tif);

 /* Encode a chunk of pixels, where returned data is down-sampled as per the
    sampling factors.  The data is expected to be written in scan-line
    multiples.
 */
    cc /= sp->bytesperline;
    if (cc % sp->bytesperline) TIFFWarning(tif->tif_name,no_write_frac);
    if ( (cc /= bytesperline)      /* No. of complete rows in caller's buffer */
       > (rows = sp->cinfo.c.image_height - sp->cinfo.c.next_scanline)
       ) cc = rows;
#   ifdef C_LOSSLESS_SUPPORTED
    lines_per_MCU = sp->cinfo.c.max_samp_factor*(size = sp->cinfo.d.data_unit);
#   else
    lines_per_MCU = sp->cinfo.c.max_samp_factor*(size = DCTSIZE);
#   endif
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
    while (--cc >= 0)
      { int ci = 0, clumpoffset = 0;
        register jpeg_component_info *compptr = sp->cinfo.c.comp_info;

     /* The fastest way to separate the data is to make 1 pass over the scan
        line for each row of each component.
     */
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
        do
          { int ypos = 0;

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
            do
              { int padding;
                register JSAMPLE *inptr = (JSAMPLE*)buf + clumpoffset,
                                 *outptr =
                  sp->ds_buffer[ci][sp->scancount*compptr->v_samp_factor+ypos];
             /* Cb,Cr both have sampling factors 1, so this is correct */
                register int clumps_per_line =
                  sp->cinfo.c.comp_info[1].downsampled_width,
                             xpos;

                padding = (int)
#                         ifdef C_LOSSLESS_SUPPORTED
                          ( compptr->width_in_data_units * size
#                         else
                          ( compptr->width_in_blocks * size
#                         endif
                          - clumps_per_line * compptr->h_samp_factor
                          );
                if (compptr->h_samp_factor == 1) /* Cb & Cr fast path */
                  do *outptr++ = *inptr;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                  while ((inptr += sp->samplesperclump),--clumps_per_line > 0);
                else /* general case */
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                  do
                    {
                      xpos = 0;
                      do *outptr++ = inptr[xpos];
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                      while (++xpos < compptr->h_samp_factor);
                    }
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                  while ((inptr += sp->samplesperclump),--clumps_per_line > 0);
                xpos = 0; /* Pad each scan line as needed */
                do outptr[0] = outptr[-1]; while (++outptr,++xpos < padding);
                clumpoffset += compptr->h_samp_factor;
              }
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
            while (++ypos < compptr->v_samp_factor);
          }
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
        while (++compptr,++ci < sp->cinfo.c.num_components);
        if (++sp->scancount >= size)
          {
            if (   CALLJPEG(sp,-1,jpeg_write_raw_data(&sp->cinfo.c,sp->ds_buffer,lines_per_MCU))
                != lines_per_MCU
               ) return 0;
            sp->scancount = 0;
          };
        ++tif->tif_row++
        buf += sp->bytesperline;
      };
    return 1;
  }
>>>>>>> sync with upstream

static int
OJPEGReadHeaderInfoSecStreamDri(TIFF* tif)
{
	/* this could easilly cause trouble in some cases... but no such cases have occured sofar */
	static const char module[]="OJPEGReadHeaderInfoSecStreamDri";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint16 m;
	if (OJPEGReadWord(sp,&m)==0)
		return(0);
	if (m!=4)
	{
		TIFFErrorExt(tif->tif_clientdata,module,"Corrupt DRI marker in JPEG data");
		return(0);
	}
	if (OJPEGReadWord(sp,&m)==0)
		return(0);
	sp->restart_interval=m;
	return(1);
}

static int
OJPEGReadHeaderInfoSecStreamDqt(TIFF* tif)
{
	/* this is a table marker, and it is to be saved as a whole for exact pushing on the jpeg stream later on */
	static const char module[]="OJPEGReadHeaderInfoSecStreamDqt";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint16 m;
	uint32 na;
	uint8* nb;
	uint8 o;
	if (OJPEGReadWord(sp,&m)==0)
		return(0);
	if (m<=2)
	{
		if (sp->subsamplingcorrect==0)
			TIFFErrorExt(tif->tif_clientdata,module,"Corrupt DQT marker in JPEG data");
		return(0);
	}
	if (sp->subsamplingcorrect!=0)
		OJPEGReadSkip(sp,m-2);
	else
	{
		m-=2;
		do
		{
			if (m<65)
			{
				TIFFErrorExt(tif->tif_clientdata,module,"Corrupt DQT marker in JPEG data");
				return(0);
			}
			na=sizeof(uint32)+69;
			nb=_TIFFmalloc(na);
			if (nb==0)
			{
				TIFFErrorExt(tif->tif_clientdata,module,"Out of memory");
				return(0);
			}
			*(uint32*)nb=na;
			nb[sizeof(uint32)]=255;
			nb[sizeof(uint32)+1]=JPEG_MARKER_DQT;
			nb[sizeof(uint32)+2]=0;
			nb[sizeof(uint32)+3]=67;
			if (OJPEGReadBlock(sp,65,&nb[sizeof(uint32)+4])==0) {
				_TIFFfree(nb);
				return(0);
			}
			o=nb[sizeof(uint32)+4]&15;
			if (3<o)
			{
				TIFFErrorExt(tif->tif_clientdata,module,"Corrupt DQT marker in JPEG data");
				_TIFFfree(nb);
				return(0);
			}
			if (sp->qtable[o]!=0)
				_TIFFfree(sp->qtable[o]);
			sp->qtable[o]=nb;
			m-=65;
		} while(m>0);
	}
	return(1);
}

static int
<<<<<<< HEAD
OJPEGReadHeaderInfoSecStreamDht(TIFF* tif)
{
	/* this is a table marker, and it is to be saved as a whole for exact pushing on the jpeg stream later on */
	/* TODO: the following assumes there is only one table in this marker... but i'm not quite sure that assumption is guaranteed correct */
	static const char module[]="OJPEGReadHeaderInfoSecStreamDht";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint16 m;
	uint32 na;
	uint8* nb;
	uint8 o;
	if (OJPEGReadWord(sp,&m)==0)
		return(0);
	if (m<=2)
	{
		if (sp->subsamplingcorrect==0)
			TIFFErrorExt(tif->tif_clientdata,module,"Corrupt DHT marker in JPEG data");
		return(0);
	}
	if (sp->subsamplingcorrect!=0)
	{
		OJPEGReadSkip(sp,m-2);
	}
	else
	{
		na=sizeof(uint32)+2+m;
		nb=_TIFFmalloc(na);
		if (nb==0)
		{
			TIFFErrorExt(tif->tif_clientdata,module,"Out of memory");
			return(0);
		}
		*(uint32*)nb=na;
		nb[sizeof(uint32)]=255;
		nb[sizeof(uint32)+1]=JPEG_MARKER_DHT;
		nb[sizeof(uint32)+2]=(m>>8);
		nb[sizeof(uint32)+3]=(m&255);
		if (OJPEGReadBlock(sp,m-2,&nb[sizeof(uint32)+4])==0)
			return(0);
		o=nb[sizeof(uint32)+4];
		if ((o&240)==0)
		{
			if (3<o)
			{
				TIFFErrorExt(tif->tif_clientdata,module,"Corrupt DHT marker in JPEG data");
				return(0);
			}
			if (sp->dctable[o]!=0)
				_TIFFfree(sp->dctable[o]);
			sp->dctable[o]=nb;
		}
		else
		{
			if ((o&240)!=16)
			{
				TIFFErrorExt(tif->tif_clientdata,module,"Corrupt DHT marker in JPEG data");
				return(0);
			}
			o&=15;
			if (3<o)
			{
				TIFFErrorExt(tif->tif_clientdata,module,"Corrupt DHT marker in JPEG data");
				return(0);
			}
			if (sp->actable[o]!=0)
				_TIFFfree(sp->actable[o]);
			sp->actable[o]=nb;
		}
	}
	return(1);
}
=======
OJPEGPostEncode(register TIFF *tif)
  { register OJPEGState *sp = OJState(tif);

 /* Finish up at the end of a strip or tile. */

    if (sp->scancount > 0) /* emit partial buffer of down-sampled data */
      { JDIMENSION n;

#       ifdef C_LOSSLESS_SUPPORTED
        if (   sp->scancount < sp->cinfo.c.data_unit
            && sp->cinfo.c.num_components > 0
           )
#       else
        if (sp->scancount < DCTSIZE && sp->cinfo.c.num_components > 0)
#       endif
          { int ci = 0,                            /* Pad the data vertically */
#           ifdef C_LOSSLESS_SUPPORTED
                size = sp->cinfo.c.data_unit;
#           else
                size = DCTSIZE;
#           endif
            register jpeg_component_info *compptr = sp->cinfo.c.comp_info;

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
            do
#              ifdef C_LOSSLESS_SUPPORTED
               { tsize_t row_width = compptr->width_in_data_units
#              else
                 tsize_t row_width = compptr->width_in_blocks
#              endif
                   *size*sizeof(JSAMPLE);
                 int ypos = sp->scancount*compptr->v_samp_factor;

                 do _TIFFmemcpy( (tdata_t)sp->ds_buffer[ci][ypos]
                               , (tdata_t)sp->ds_buffer[ci][ypos-1]
                               , row_width
                               );
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                 while (++ypos < compptr->v_samp_factor*size);
               }
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
            while (++compptr,++ci < sp->cinfo.c.num_components);
          };
        n = sp->cinfo.c.max_v_samp_factor*size;
        if (CALLJPEG(sp,-1,jpeg_write_raw_data(&sp->cinfo.c,sp->ds_buffer,n)) != n)
          return 0;
      };
    return CALLVJPEG(sp,jpeg_finish_compress(&sp->cinfo.c));
  }
#endif /* never */

/* JPEG Decoding begins here. */

/*ARGSUSED*/ static int
OJPEGDecode(register TIFF *tif,tidata_t buf,tsize_t cc,tsample_t s)
  { tsize_t bytesperline = isTiled(tif)
                         ? TIFFTileRowSize(tif)
                         : tif->tif_scanlinesize,
            rows;                          /* No. of unprocessed rows in file */
    register OJPEGState *sp = OJState(tif);

 /* Decode a chunk of pixels, where the input data has not NOT been down-
    sampled, or else the TIFF Library's client has used the "JPEGColorMode" TIFF
    pseudo-tag to request that the JPEG Library do color-space conversion; this
    is the normal case.  The data is expected to be read in scan-line multiples,
    and this subroutine is called for both pixel-interleaved and separate color
    planes.

    WARNING:  Unlike "OJPEGDecodeRawContig()", below, the no. of Bytes in each
              decoded row is calculated here as "bytesperline" instead of
    using "sp->bytesperline", which might be a little smaller.  This can
    occur for an old tiled image whose width isn't a multiple of 8 pixels.
    That's illegal according to the TIFF Version 6 specification, but some
    test files, like "zackthecat.tif", were built that way.  In those cases,
    we want to embed the image's true width in our caller's buffer (which is
    presumably allocated according to the expected tile width) by
    effectively "padding" it with unused Bytes at the end of each row.
 */
    if ( (cc /= bytesperline)      /* No. of complete rows in caller's buffer */
       > (rows = sp->cinfo.d.output_height - sp->cinfo.d.output_scanline)
       ) cc = rows;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
    while (--cc >= 0)
      {
        if (   CALLJPEG(sp,-1,jpeg_read_scanlines(&sp->cinfo.d,(JSAMPARRAY)&buf,1))
            != 1
           ) return 0;
        buf += bytesperline;
        ++tif->tif_row;
      };

 /* BEWARE OF KLUDGE:  If our input file was produced by Microsoft's Wang
                       Imaging for Windows application, the DC coefficients of
    each JPEG image component (Y,Cb,Cr) must be reset at the end of each TIFF
    "strip", and any JPEG data bits remaining in the current Byte of the
    decoder's input buffer must be discarded.  To do so, we create an "ad hoc"
    interface in the "jdhuff.c" module of IJG JPEG Library Version 6 (module
    "jdshuff.c", if Ken Murchison's lossless-Huffman patch is applied), and we
    invoke that interface here after decoding each "strip".
 */
    if (sp->is_WANG) jpeg_reset_huff_decode(&sp->cinfo.d);
    return 1;
  }

/*ARGSUSED*/ static int
OJPEGDecodeRawContig(register TIFF *tif,tidata_t buf,tsize_t cc,tsample_t s)
  { tsize_t rows;                          /* No. of unprocessed rows in file */
    JDIMENSION lines_per_MCU, size;
    register OJPEGState *sp = OJState(tif);

 /* Decode a chunk of pixels, where the input data has pixel-interleaved color
    planes, some of which have been down-sampled, but the TIFF Library's client
    has NOT used the "JPEGColorMode" TIFF pseudo-tag to request that the JPEG
    Library do color-space conversion.  In other words, we must up-sample/
    expand/duplicate image components according to the image's sampling factors,
    without changing its color space.  The data is expected to be read in scan-
    line multiples.
 */
    if ( (cc /= sp->bytesperline)  /* No. of complete rows in caller's buffer */
       > (rows = sp->cinfo.d.output_height - sp->cinfo.d.output_scanline)
       ) cc = rows;
    lines_per_MCU = sp->cinfo.d.max_v_samp_factor
#   ifdef D_LOSSLESS_SUPPORTED
                  * (size = sp->cinfo.d.min_codec_data_unit);
#   else
                  * (size = DCTSIZE);
#   endif
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
    while (--cc >= 0)
      { int clumpoffset, ci;
        register jpeg_component_info *compptr;

        if (sp->scancount >= size) /* reload downsampled-data buffers */
          {
            if (   CALLJPEG(sp,-1,jpeg_read_raw_data(&sp->cinfo.d,sp->ds_buffer,lines_per_MCU))
                != lines_per_MCU
               ) return 0;
            sp->scancount = 0;
          };

     /* The fastest way to separate the data is: make 1 pass over the scan
        line for each row of each component.
     */
        clumpoffset = ci = 0;
        compptr = sp->cinfo.d.comp_info;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
        do
          { int ypos = 0;

            if (compptr->h_samp_factor == 1) /* fast path */
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
              do
                { register JSAMPLE *inptr =
                    sp->ds_buffer[ci][sp->scancount*compptr->v_samp_factor+ypos],
                                   *outptr = (JSAMPLE *)buf + clumpoffset;
                  register int clumps_per_line = compptr->downsampled_width;

                  do *outptr = *inptr++;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                  while ((outptr += sp->samplesperclump),--clumps_per_line > 0);
                }
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
              while ( (clumpoffset += compptr->h_samp_factor)
                    , ++ypos < compptr->v_samp_factor
                    );
            else /* general case */
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
              do
                { register JSAMPLE *inptr =
                    sp->ds_buffer[ci][sp->scancount*compptr->v_samp_factor+ypos],
                                   *outptr = (JSAMPLE *)buf + clumpoffset;
                  register int clumps_per_line = compptr->downsampled_width;

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                  do
                    { register int xpos = 0;

                      do outptr[xpos] = *inptr++;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                      while (++xpos < compptr->h_samp_factor);
                    }
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                  while ((outptr += sp->samplesperclump),--clumps_per_line > 0);
                }
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
              while ( (clumpoffset += compptr->h_samp_factor)
                    , ++ypos < compptr->v_samp_factor
                    );
          }
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
        while (++compptr,++ci < sp->cinfo.d.num_components);
        ++sp->scancount;
        buf += sp->bytesperline;
        ++tif->tif_row;
      };

 /* BEWARE OF KLUDGE:  If our input file was produced by Microsoft's Wang
                       Imaging for Windows application, the DC coefficients of
    each JPEG image component (Y,Cb,Cr) must be reset at the end of each TIFF
    "strip", and any JPEG data bits remaining in the current Byte of the
    decoder's input buffer must be discarded.  To do so, we create an "ad hoc"
    interface in the "jdhuff.c" module of IJG JPEG Library Version 6 (module
    "jdshuff.c", if Ken Murchison's lossless-Huffman patch is applied), and we
    invoke that interface here after decoding each "strip".
 */
    if (sp->is_WANG) jpeg_reset_huff_decode(&sp->cinfo.d);
    return 1;
  }

/*ARGSUSED*/ static int
OJPEGDecodeRawSeparate(TIFF *tif,register tidata_t buf,tsize_t cc,tsample_t s)
  { tsize_t rows;                          /* No. of unprocessed rows in file */
    JDIMENSION lines_per_MCU,
               size,                                             /* ...of MCU */
               v;                   /* Component's vertical up-sampling ratio */
    register OJPEGState *sp = OJState(tif);
    register jpeg_component_info *compptr = sp->cinfo.d.comp_info + s;

 /* Decode a chunk of pixels, where the input data has separate color planes,
    some of which have been down-sampled, but the TIFF Library's client has NOT
    used the "JPEGColorMode" TIFF pseudo-tag to request that the JPEG Library
    do color-space conversion.  The data is expected to be read in scan-line
    multiples.
 */
    v = sp->cinfo.d.max_v_samp_factor/compptr->v_samp_factor;
    if ( (cc /= compptr->downsampled_width) /* No. of rows in caller's buffer */
       > (rows = (sp->cinfo.d.output_height-sp->cinfo.d.output_scanline+v-1)/v)
       ) cc = rows; /* No. of rows of "clumps" to read */
    lines_per_MCU = sp->cinfo.d.max_v_samp_factor
#   ifdef D_LOSSLESS_SUPPORTED
                  * (size = sp->cinfo.d.min_codec_data_unit);
#   else
                  * (size = DCTSIZE);
#   endif
 L: if (sp->scancount >= size) /* reload downsampled-data buffers */
      {
        if (   CALLJPEG(sp,-1,jpeg_read_raw_data(&sp->cinfo.d,sp->ds_buffer,lines_per_MCU))
            != lines_per_MCU
           ) return 0;
        sp->scancount = 0;
      };
    rows = 0;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
    do
      { register JSAMPLE *inptr =
          sp->ds_buffer[s][sp->scancount*compptr->v_samp_factor + rows];
        register int clumps_per_line = compptr->downsampled_width;

        do *buf++ = *inptr++; while (--clumps_per_line > 0); /* Copy scanline */
        tif->tif_row += v;
        if (--cc <= 0) return 1; /* End of caller's buffer? */
      }
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
    while (++rows < compptr->v_samp_factor);
    ++sp->scancount;
    goto L;
  }

/* "OJPEGSetupDecode()" temporarily forces the JPEG Library to use the following
   subroutine as a "dummy" input reader in order to fool the library into
   thinking that it has read the image's first "Start of Scan" (SOS) marker, so
   that it initializes accordingly.
*/
/*ARGSUSED*/ METHODDEF(int)
fake_SOS_marker(j_decompress_ptr cinfo){return JPEG_REACHED_SOS;}

/*ARGSUSED*/ METHODDEF(int)
suspend(j_decompress_ptr cinfo){return JPEG_SUSPENDED;}

/* The JPEG Library's "null" color-space converter actually re-packs separate
   color planes (it's native image representation) into a pixel-interleaved,
   contiguous plane.  But if our TIFF Library client is tryng to process a
   PLANARCONFIG_SEPARATE image, we don't want that; so here are modifications of
   code in the JPEG Library's "jdcolor.c" file, which simply copy Bytes to a
   color plane specified by the current JPEG "scan".
*/
METHODDEF(void)
ycc_rgb_convert(register j_decompress_ptr cinfo,JSAMPIMAGE in,JDIMENSION row,
                register JSAMPARRAY out,register int nrows)
  { typedef struct                /* "jdcolor.c" color-space conversion state */
      {

     /* WARNING:  This declaration is ugly and dangerous!  It's supposed to be
                  private to the JPEG Library's "jdcolor.c" module, but we also
        need it here.  Since the library's copy might change without notice, be
        sure to keep this one synchronized or the following code will break!
     */
        struct jpeg_color_deconverter pub; /* Public fields */
     /* Private state for YCC->RGB conversion */
        int *Cr_r_tab,   /* ->Cr to R conversion table */
            *Cb_b_tab;   /* ->Cb to B conversion table */
        INT32 *Cr_g_tab, /* ->Cr to G conversion table */
              *Cb_g_tab; /* ->Cb to G conversion table */
      } *my_cconvert_ptr;
    my_cconvert_ptr cconvert = (my_cconvert_ptr)cinfo->cconvert;
    JSAMPARRAY irow0p = in[0] + row;
    register JSAMPLE *range_limit = cinfo->sample_range_limit;
    register JSAMPROW outp, Y;

    switch (cinfo->output_scan_number - 1)
      { JSAMPARRAY irow1p, irow2p;
        register INT32 *table0, *table1;
        SHIFT_TEMPS

        case RGB_RED  : irow2p = in[2] + row;
                        table0 = (INT32 *)cconvert->Cr_r_tab;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                        while (--nrows >= 0)
                          { register JSAMPROW Cr = *irow2p++;
                             register int i = cinfo->output_width;

                             Y = *irow0p++;
                             outp = *out++;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                             while (--i >= 0)
                               *outp++ = range_limit[*Y++ + table0[*Cr++]];
                          };
                        return;
        case RGB_GREEN: irow1p = in[1] + row;
                        irow2p = in[2] + row;
                        table0 = cconvert->Cb_g_tab;
                        table1 = cconvert->Cr_g_tab;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                        while (--nrows >= 0)
                          { register JSAMPROW Cb = *irow1p++,
                                              Cr = *irow2p++;
                             register int i = cinfo->output_width;

                             Y = *irow0p++;
                             outp = *out++;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                             while (--i >= 0)
                               *outp++ =
                                 range_limit[ *Y++
                                            + RIGHT_SHIFT(table0[*Cb++]+table1[*Cr++],16)
                                            ];
                          };
                        return;
        case RGB_BLUE : irow1p = in[1] + row;
                        table0 = (INT32 *)cconvert->Cb_b_tab;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                        while (--nrows >= 0)
                          { register JSAMPROW Cb = *irow1p++;
                             register int i = cinfo->output_width;

                             Y = *irow0p++;
                             outp = *out++;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                             while (--i >= 0)
                               *outp++ = range_limit[*Y++ + table0[*Cb++]];
                          }
      }
  }

METHODDEF(void)
null_convert(register j_decompress_ptr cinfo,JSAMPIMAGE in,JDIMENSION row,
             register JSAMPARRAY out,register int nrows)
  { register JSAMPARRAY irowp = in[cinfo->output_scan_number - 1] + row;

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
    while (--nrows >= 0) _TIFFmemcpy(*out++,*irowp++,cinfo->output_width);
  }
>>>>>>> sync with upstream

static int
OJPEGReadHeaderInfoSecStreamSof(TIFF* tif, uint8 marker_id)
{
	/* this marker needs to be checked, and part of its data needs to be saved for regeneration later on */
	static const char module[]="OJPEGReadHeaderInfoSecStreamSof";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint16 m;
	uint16 n;
	uint8 o;
	uint16 p;
	uint16 q;
	if (sp->sof_log!=0)
	{
		TIFFErrorExt(tif->tif_clientdata,module,"Corrupt JPEG data");
		return(0);
	}
	if (sp->subsamplingcorrect==0)
		sp->sof_marker_id=marker_id;
	/* Lf: data length */
	if (OJPEGReadWord(sp,&m)==0)
		return(0);
	if (m<11)
	{
		if (sp->subsamplingcorrect==0)
			TIFFErrorExt(tif->tif_clientdata,module,"Corrupt SOF marker in JPEG data");
		return(0);
	}
	m-=8;
	if (m%3!=0)
	{
		if (sp->subsamplingcorrect==0)
			TIFFErrorExt(tif->tif_clientdata,module,"Corrupt SOF marker in JPEG data");
		return(0);
	}
	n=m/3;
	if (sp->subsamplingcorrect==0)
	{
		if (n!=sp->samples_per_pixel)
		{
			TIFFErrorExt(tif->tif_clientdata,module,"JPEG compressed data indicates unexpected number of samples");
			return(0);
		}
	}
	/* P: Sample precision */
	if (OJPEGReadByte(sp,&o)==0)
		return(0);
	if (o!=8)
	{
		if (sp->subsamplingcorrect==0)
			TIFFErrorExt(tif->tif_clientdata,module,"JPEG compressed data indicates unexpected number of bits per sample");
		return(0);
	}
	/* Y: Number of lines, X: Number of samples per line */
	if (sp->subsamplingcorrect)
		OJPEGReadSkip(sp,4);
	else
	{
		/* Y: Number of lines */
		if (OJPEGReadWord(sp,&p)==0)
			return(0);
		if (((uint32)p<sp->image_length) && ((uint32)p<sp->strile_length_total))
		{
			TIFFErrorExt(tif->tif_clientdata,module,"JPEG compressed data indicates unexpected height");
			return(0);
		}
		sp->sof_y=p;
		/* X: Number of samples per line */
		if (OJPEGReadWord(sp,&p)==0)
			return(0);
		if (((uint32)p<sp->image_width) && ((uint32)p<sp->strile_width))
		{
			TIFFErrorExt(tif->tif_clientdata,module,"JPEG compressed data indicates unexpected width");
			return(0);
		}
		if ((uint32)p>sp->strile_width)
		{
			TIFFErrorExt(tif->tif_clientdata,module,"JPEG compressed data image width exceeds expected image width");
			return(0);
		}
		sp->sof_x=p;
	}
	/* Nf: Number of image components in frame */
	if (OJPEGReadByte(sp,&o)==0)
		return(0);
	if (o!=n)
	{
		if (sp->subsamplingcorrect==0)
			TIFFErrorExt(tif->tif_clientdata,module,"Corrupt SOF marker in JPEG data");
		return(0);
	}
	/* per component stuff */
	/* TODO: double-check that flow implies that n cannot be as big as to make us overflow sof_c, sof_hv and sof_tq arrays */
	for (q=0; q<n; q++)
	{
		/* C: Component identifier */
		if (OJPEGReadByte(sp,&o)==0)
			return(0);
		if (sp->subsamplingcorrect==0)
			sp->sof_c[q]=o;
		/* H: Horizontal sampling factor, and V: Vertical sampling factor */
		if (OJPEGReadByte(sp,&o)==0)
			return(0);
		if (sp->subsamplingcorrect!=0)
		{
			if (q==0)
			{
				sp->subsampling_hor=(o>>4);
				sp->subsampling_ver=(o&15);
				if (((sp->subsampling_hor!=1) && (sp->subsampling_hor!=2) && (sp->subsampling_hor!=4)) ||
					((sp->subsampling_ver!=1) && (sp->subsampling_ver!=2) && (sp->subsampling_ver!=4)))
					sp->subsampling_force_desubsampling_inside_decompression=1;
			}
			else
			{
				if (o!=17)
					sp->subsampling_force_desubsampling_inside_decompression=1;
			}
		}
		else
		{
			sp->sof_hv[q]=o;
			if (sp->subsampling_force_desubsampling_inside_decompression==0)
			{
				if (q==0)
				{
					if (o!=((sp->subsampling_hor<<4)|sp->subsampling_ver))
					{
						TIFFErrorExt(tif->tif_clientdata,module,"JPEG compressed data indicates unexpected subsampling values");
						return(0);
					}
				}
				else
				{
					if (o!=17)
					{
						TIFFErrorExt(tif->tif_clientdata,module,"JPEG compressed data indicates unexpected subsampling values");
						return(0);
					}
				}
			}
		}
		/* Tq: Quantization table destination selector */
		if (OJPEGReadByte(sp,&o)==0)
			return(0);
		if (sp->subsamplingcorrect==0)
			sp->sof_tq[q]=o;
	}
	if (sp->subsamplingcorrect==0)
		sp->sof_log=1;
	return(1);
}

<<<<<<< HEAD
static int
OJPEGReadHeaderInfoSecStreamSos(TIFF* tif)
{
	/* this marker needs to be checked, and part of its data needs to be saved for regeneration later on */
	static const char module[]="OJPEGReadHeaderInfoSecStreamSos";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint16 m;
	uint8 n;
	uint8 o;
	assert(sp->subsamplingcorrect==0);
	if (sp->sof_log==0)
	{
		TIFFErrorExt(tif->tif_clientdata,module,"Corrupt SOS marker in JPEG data");
		return(0);
	}
	/* Ls */
	if (OJPEGReadWord(sp,&m)==0)
		return(0);
	if (m!=6+sp->samples_per_pixel_per_plane*2)
	{
		TIFFErrorExt(tif->tif_clientdata,module,"Corrupt SOS marker in JPEG data");
		return(0);
	}
	/* Ns */
	if (OJPEGReadByte(sp,&n)==0)
		return(0);
	if (n!=sp->samples_per_pixel_per_plane)
	{
		TIFFErrorExt(tif->tif_clientdata,module,"Corrupt SOS marker in JPEG data");
		return(0);
	}
	/* Cs, Td, and Ta */
	for (o=0; o<sp->samples_per_pixel_per_plane; o++)
	{
		/* Cs */
		if (OJPEGReadByte(sp,&n)==0)
			return(0);
		sp->sos_cs[sp->plane_sample_offset+o]=n;
		/* Td and Ta */
		if (OJPEGReadByte(sp,&n)==0)
			return(0);
		sp->sos_tda[sp->plane_sample_offset+o]=n;
	}
	/* skip Ss, Se, Ah, en Al -> no check, as per Tom Lane recommendation, as per LibJpeg source */
	OJPEGReadSkip(sp,3);
	return(1);
}
=======
 /* Initialize decompression parameters that won't be overridden by JPEG Library
    defaults set during the "jpeg_read_header()" call, below.
 */
    segment_width = td->td_imagewidth;
    if (isTiled(tif))
      {
        if (sp->is_WANG) /* we don't know how to handle it */
          {
            TIFFError(module,"Tiled Wang image not supported");
            return 0;
          };

     /* BOGOSITY ALERT!  "TIFFTileRowSize()" seems to work fine for modern JPEG-
                         in-TIFF encapsulations where the image width--like the
        tile width--is a multiple of 8 or 16 pixels.  But image widths and
        heights are aren't restricted to 8- or 16-bit multiples, and we need
        the exact Byte count of decompressed scan lines when we call the JPEG
        Library.  At least one old file ("zackthecat.tif") in the TIFF Library
        test suite has widths and heights slightly less than the tile sizes, and
        it apparently used the bogus computation below to determine the number
        of Bytes per scan line (was this due to an old, broken version of
        "TIFFhowmany()"?).  Before we get here, "OJPEGSetupDecode()" verified
        that our image uses 8-bit samples, so the following check appears to
        return the correct answer in all known cases tested to date.
     */
        if (is_JFIF || (segment_width & 7) == 0)
          sp->bytesperline = TIFFTileRowSize(tif); /* Normal case */
        else
          {
            /* Was the file-encoder's segment-width calculation bogus? */
            segment_width = (segment_width/sp->h_sampling + 1) * sp->h_sampling;
            sp->bytesperline = segment_width * td->td_samplesperpixel;
          }
      }
    else sp->bytesperline = TIFFVStripSize(tif,1);

 /* BEWARE OF KLUDGE:  If we have JPEG Interchange File Format (JFIF) image,
                       then we want to read "metadata" in the bit-stream's
    header and validate it against corresponding information in TIFF records.
    But if we have a *really old* JPEG file that's not JFIF, then we simply
    assign TIFF-record values to JPEG Library variables without checking.
 */
    if (is_JFIF) /* JFIF image */
      { unsigned char *end_of_data;
        int subsampling_factors;
        register unsigned char *p;
        register int i;

     /* WARNING:  Although the image file contains a JFIF bit stream, it might
                  also contain some old TIFF records causing "OJPEGVSetField()"
        to have allocated quantization or Huffman decoding tables.  But when the
        JPEG Library reads and parses the JFIF header below, it reallocate these
        tables anew without checking for "dangling" pointers, thereby causing a
        memory "leak".  We have enough information to potentially deallocate the
        old tables here, but unfortunately JPEG Library Version 6B uses a "pool"
        allocator for small objects, with no deallocation procedure; instead, it
        reclaims a whole pool when an image is closed/destroyed, so well-behaved
        TIFF client applications (i.e., those which close their JPEG images as
        soon as they're no longer needed) will waste memory for a short time but
        recover it eventually.  But ill-behaved TIFF clients (i.e., those which
        keep many JPEG images open gratuitously) can exhaust memory prematurely.
        If the JPEG Library ever implements a deallocation procedure, insert
        this clean-up code:
     */
#       ifdef someday
        if (sp->jpegtablesmode & JPEGTABLESMODE_QUANT) /* free quant. tables */
          { register int i = 0;

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
            do
              { register JQUANT_TBL *q;

                if (q = sp->cinfo.d.quant_tbl_ptrs[i])
                  {
                    jpeg_free_small(&sp->cinfo.comm,q,sizeof *q);
                    sp->cinfo.d.quant_tbl_ptrs[i] = 0;
                  }
              }
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
            while (++i < NUM_QUANT_TBLS);
          };
        if (sp->jpegtablesmode & JPEGTABLESMODE_HUFF) /* free Huffman tables */
          { register int i = 0;

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
            do
              { register JHUFF_TBL *h;

                if (h = sp->cinfo.d.dc_huff_tbl_ptrs[i])
                  {
                    jpeg_free_small(&sp->cinfo.comm,h,sizeof *h);
                    sp->cinfo.d.dc_huff_tbl_ptrs[i] = 0;
                  };
                if (h = sp->cinfo.d.ac_huff_tbl_ptrs[i])
                  {
                    jpeg_free_small(&sp->cinfo.comm,h,sizeof *h);
                    sp->cinfo.d.ac_huff_tbl_ptrs[i] = 0;
                  }
              }
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
            while (++i < NUM_HUFF_TBLS);
          };
#       endif /* someday */

     /* Since we might someday wish to try rewriting "old format" JPEG-in-TIFF
        encapsulations in "new format" files, try to synthesize the value of a
        modern "JPEGTables" TIFF record by scanning the JPEG data from just past
        the "Start of Information" (SOI) marker until something other than a
        legitimate "table" marker is found, as defined in ISO IS 10918-1
        Appending B.2.4; namely:

        -- Define Quantization Table (DQT)
        -- Define Huffman Table (DHT)
        -- Define Arithmetic Coding table (DAC)
        -- Define Restart Interval (DRI)
        -- Comment (COM)
        -- Application data (APPn)

        For convenience, we also accept "Expansion" (EXP) markers, although they
        are apparently not a part of normal "table" data.
     */
        sp->jpegtables = p = (unsigned char *)sp->src.next_input_byte;
        end_of_data = p + sp->src.bytes_in_buffer;
        p += 2;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
        while (p < end_of_data && p[0] == 0xFF)
          switch (p[1])
            {
              default  : goto L;
              case 0xC0: /* SOF0  */
              case 0xC1: /* SOF1  */
              case 0xC2: /* SOF2  */
              case 0xC3: /* SOF3  */
              case 0xC4: /* DHT   */
              case 0xC5: /* SOF5  */
              case 0xC6: /* SOF6  */
              case 0xC7: /* SOF7  */
              case 0xC9: /* SOF9  */
              case 0xCA: /* SOF10 */
              case 0xCB: /* SOF11 */
              case 0xCC: /* DAC   */
              case 0xCD: /* SOF13 */
              case 0xCE: /* SOF14 */
              case 0xCF: /* SOF15 */
              case 0xDB: /* DQT   */
              case 0xDD: /* DRI   */
              case 0xDF: /* EXP   */
              case 0xE0: /* APP0  */
              case 0xE1: /* APP1  */
              case 0xE2: /* APP2  */
              case 0xE3: /* APP3  */
              case 0xE4: /* APP4  */
              case 0xE5: /* APP5  */
              case 0xE6: /* APP6  */
              case 0xE7: /* APP7  */
              case 0xE8: /* APP8  */
              case 0xE9: /* APP9  */
              case 0xEA: /* APP10 */
              case 0xEB: /* APP11 */
              case 0xEC: /* APP12 */
              case 0xED: /* APP13 */
              case 0xEE: /* APP14 */
              case 0xEF: /* APP15 */
              case 0xFE: /* COM   */
                         p += (p[2] << 8 | p[3]) + 2;
            };
     L: if (p - (unsigned char *)sp->jpegtables > 2) /* fake "JPEGTables" */
          {

         /* In case our client application asks, pretend that this image file
            contains a modern "JPEGTables" TIFF record by copying to a buffer
            the initial part of the JFIF bit-stream that we just scanned, from
            the SOI marker through the "metadata" tables, then append an EOI
            marker and flag the "JPEGTables" TIFF record as "present".
         */
            sp->jpegtables_length = p - (unsigned char*)sp->jpegtables + 2;
            p = sp->jpegtables;
            if (!(sp->jpegtables = _TIFFmalloc(sp->jpegtables_length)))
              {
                TIFFError(module,no_jtable_space);
                return 0;
              };
            _TIFFmemcpy(sp->jpegtables,p,sp->jpegtables_length-2);
            p = (unsigned char *)sp->jpegtables + sp->jpegtables_length;
            p[-2] = 0xFF; p[-1] = JPEG_EOI; /* Append EOI marker */
            TIFFSetFieldBit(tif,FIELD_JPEGTABLES);
            tif->tif_flags |= TIFF_DIRTYDIRECT;
          }
        else sp->jpegtables = 0; /* Don't simulate "JPEGTables" */
        if (   CALLJPEG(sp,-1,jpeg_read_header(&sp->cinfo.d,TRUE))
            != JPEG_HEADER_OK
           ) return 0;
        if (   sp->cinfo.d.image_width  != segment_width
            || sp->cinfo.d.image_height != td->td_imagelength 
           )
          {
            TIFFError(module,"Improper JPEG strip/tile size");
            return 0;
          };
        if (sp->cinfo.d.num_components != td->td_samplesperpixel)
          {
            TIFFError(module,"Improper JPEG component count");
            return 0;
          };
        if (sp->cinfo.d.data_precision != td->td_bitspersample)
          {
            TIFFError(module,"Improper JPEG data precision");
            return 0;
          };

     /* Check that JPEG image components all have the same subsampling factors
        declared (or defaulted) in the TIFF file, since TIFF Version 6.0 is more
        restrictive than JPEG:  Only the 0th component may have horizontal and
        vertical subsampling factors other than <1,1>.
     */
        subsampling_factors = sp->h_sampling << 3 | sp->v_sampling;
        i = 0;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
        do
          {
            if (   ( sp->cinfo.d.comp_info[i].h_samp_factor << 3
                   | sp->cinfo.d.comp_info[i].v_samp_factor
                   )
                != subsampling_factors
               )
              {
                TIFFError(module,"Improper JPEG subsampling factors");
                return 0;
              };
            subsampling_factors = 011; /* Required for image components > 0 */
          }
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
        while (++i < sp->cinfo.d.num_components);
      }
    else /* not JFIF image */
      { int (*save)(j_decompress_ptr cinfo) = sp->cinfo.d.marker->read_markers;
        register int i;

     /* We're not assuming that this file's JPEG bit stream has any header
        "metadata", so fool the JPEG Library into thinking that we read a
        "Start of Input" (SOI) marker and a "Start of Frame" (SOFx) marker, then
        force it to read a simulated "Start of Scan" (SOS) marker when we call
        "jpeg_read_header()" below.  This should cause the JPEG Library to
        establish reasonable defaults.
     */
        sp->cinfo.d.marker->saw_SOI =       /* Pretend we saw SOI marker */
        sp->cinfo.d.marker->saw_SOF = TRUE; /* Pretend we saw SOF marker */
        sp->cinfo.d.marker->read_markers =
          sp->is_WANG ? suspend : fake_SOS_marker;
        sp->cinfo.d.global_state = DSTATE_INHEADER;
        sp->cinfo.d.Se = DCTSIZE2-1; /* Suppress JPEG Library warning */
        sp->cinfo.d.image_width  = segment_width;
        sp->cinfo.d.image_height = td->td_imagelength;

     /* The following color-space initialization, including the complicated
        "switch"-statement below, essentially duplicates the logic used by the
        JPEG Library's "jpeg_init_colorspace()" subroutine during compression.
     */
        sp->cinfo.d.num_components = td->td_samplesperpixel;
        sp->cinfo.d.comp_info = (jpeg_component_info *)
          (*sp->cinfo.d.mem->alloc_small)
            ( &sp->cinfo.comm
            , JPOOL_IMAGE
            , sp->cinfo.d.num_components * sizeof *sp->cinfo.d.comp_info
            );
        i = 0;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
        do
          {
            sp->cinfo.d.comp_info[i].component_index = i;
            sp->cinfo.d.comp_info[i].component_needed = TRUE;
            sp->cinfo.d.cur_comp_info[i] = &sp->cinfo.d.comp_info[i];
          }
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
        while (++i < sp->cinfo.d.num_components);
        switch (jpeg_color_space)
          {
            case JCS_UNKNOWN  :
              i = 0;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
              do
                {
                  sp->cinfo.d.comp_info[i].component_id = i;
                  sp->cinfo.d.comp_info[i].h_samp_factor =
                  sp->cinfo.d.comp_info[i].v_samp_factor = 1;
                }
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
              while (++i < sp->cinfo.d.num_components);
              break;
            case JCS_GRAYSCALE:
              sp->cinfo.d.comp_info[0].component_id =
              sp->cinfo.d.comp_info[0].h_samp_factor =
              sp->cinfo.d.comp_info[0].v_samp_factor = 1;
              break;
            case JCS_RGB      :
              sp->cinfo.d.comp_info[0].component_id = 'R';
              sp->cinfo.d.comp_info[1].component_id = 'G';
              sp->cinfo.d.comp_info[2].component_id = 'B';
              i = 0;
              do sp->cinfo.d.comp_info[i].h_samp_factor =
                 sp->cinfo.d.comp_info[i].v_samp_factor = 1;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
              while (++i < sp->cinfo.d.num_components);
              break;
            case JCS_CMYK     :
              sp->cinfo.d.comp_info[0].component_id = 'C';
              sp->cinfo.d.comp_info[1].component_id = 'M';
              sp->cinfo.d.comp_info[2].component_id = 'Y';
              sp->cinfo.d.comp_info[3].component_id = 'K';
              i = 0;
              do sp->cinfo.d.comp_info[i].h_samp_factor =
                 sp->cinfo.d.comp_info[i].v_samp_factor = 1;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
              while (++i < sp->cinfo.d.num_components);
              break;
            case JCS_YCbCr    :
              i = 0;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
              do
                {
                  sp->cinfo.d.comp_info[i].component_id = i+1;
                  sp->cinfo.d.comp_info[i].h_samp_factor =
                  sp->cinfo.d.comp_info[i].v_samp_factor = 1;
                  sp->cinfo.d.comp_info[i].quant_tbl_no =
                  sp->cinfo.d.comp_info[i].dc_tbl_no =
                  sp->cinfo.d.comp_info[i].ac_tbl_no = i > 0;
                }
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
              while (++i < sp->cinfo.d.num_components);
              sp->cinfo.d.comp_info[0].h_samp_factor = sp->h_sampling;
              sp->cinfo.d.comp_info[0].v_samp_factor = sp->v_sampling;
          };
        sp->cinfo.d.comps_in_scan = td->td_planarconfig == PLANARCONFIG_CONTIG
                                  ? sp->cinfo.d.num_components
                                  : 1;
        i = CALLJPEG(sp,-1,jpeg_read_header(&sp->cinfo.d,!sp->is_WANG));
        sp->cinfo.d.marker->read_markers = save; /* Restore input method */
        if (sp->is_WANG) /* produced by Wang Imaging on Microsoft Windows */
          {
            if (i != JPEG_SUSPENDED) return 0;

         /* BOGOSITY ALERT!  Files prooduced by the Wang Imaging application for
                             Microsoft Windows are a special--and, technically
            illegal--case.  A JPEG SOS marker and rest of the data stream should
            be located at the end of the file, in a position identified by the
            0th Strip offset.
         */
            i = td->td_nstrips - 1;
            sp->src.next_input_byte = tif->tif_base + td->td_stripoffset[0];
            sp->src.bytes_in_buffer = td->td_stripoffset[i] -
              td->td_stripoffset[0] + td->td_stripbytecount[i];
            i = CALLJPEG(sp,-1,jpeg_read_header(&sp->cinfo.d,TRUE));
          };
        if (i != JPEG_HEADER_OK) return 0;
      };

 /* Some of our initialization must wait until the JPEG Library is initialized
    above, in order to override its defaults.
 */
    if (   (sp->cinfo.d.raw_data_out = downsampled_output)
        && !alloc_downsampled_buffers(tif,sp->cinfo.d.comp_info,
                                      sp->cinfo.d.num_components)
       ) return 0;
    sp->cinfo.d.jpeg_color_space = jpeg_color_space;
    sp->cinfo.d.out_color_space = out_color_space;
    sp->cinfo.d.dither_mode = JDITHER_NONE; /* Reduce image "noise" */
    sp->cinfo.d.two_pass_quantize = FALSE;

 /* If the image consists of separate, discontiguous TIFF "samples" (= color
    planes, hopefully = JPEG "scans"), then we must use the JPEG Library's
    "buffered image" mode to decompress the entire image into temporary buffers,
    because the JPEG Library must parse the entire JPEG bit-stream in order to
    be satsified that it has a complete set of color components for each pixel,
    but the TIFF Library must allow our client to extract 1 component at a time.
    Initializing the JPEG Library's "buffered image" mode is tricky:  First, we
    start its decompressor, then we tell the decompressor to "consume" (i.e.,
    buffer) the entire bit-stream.

    WARNING:  Disabling "fancy" up-sampling seems to slightly reduce "noise" for
              certain old Wang Imaging files, but it absolutely *must* be
    enabled if the image has separate color planes, since in that case, the JPEG
    Library doesn't use an "sp->cinfo.d.cconvert" structure (so de-referencing
    this pointer below will cause a fatal crash) but writing our own code to up-
    sample separate color planes is too much work for right now.  Maybe someday?
 */
    sp->cinfo.d.do_fancy_upsampling = /* Always let this default (to TRUE)? */
    sp->cinfo.d.buffered_image = td->td_planarconfig == PLANARCONFIG_SEPARATE;
    if (!CALLJPEG(sp,0,jpeg_start_decompress(&sp->cinfo.d))) return 0;
    if (sp->cinfo.d.buffered_image) /* separate color planes */
      {
        if (sp->cinfo.d.raw_data_out)
          tif->tif_decoderow = tif->tif_decodestrip = tif->tif_decodetile =
            OJPEGDecodeRawSeparate;
        else
          {
            tif->tif_decoderow = tif->tif_decodestrip = tif->tif_decodetile =
              OJPEGDecode;

         /* In JPEG Library Version 6B, color-space conversion isn't implemented
            for separate color planes, so we must do it ourself if our TIFF
            client doesn't want to:
         */
            sp->cinfo.d.cconvert->color_convert =
              sp->cinfo.d.jpeg_color_space == sp->cinfo.d.out_color_space
              ? null_convert : ycc_rgb_convert;
          };
    L3: switch (CALLJPEG(sp,0,jpeg_consume_input(&sp->cinfo.d)))
          {
            default              : goto L3;

         /* If no JPEG "End of Information" (EOI) marker is found when bit-
            stream parsing ends, check whether we have enough data to proceed
            before reporting an error.
         */
            case JPEG_SUSPENDED  : if (  sp->cinfo.d.input_scan_number
                                        *sp->cinfo.d.image_height
                                       + sp->cinfo.d.input_iMCU_row
                                        *sp->cinfo.d.max_v_samp_factor
#                                       ifdef D_LOSSLESS_SUPPORTED
                                        *sp->cinfo.d.data_units_in_MCU
                                        *sp->cinfo.d.min_codec_data_unit
#                                       else
                                        *sp->cinfo.d.blocks_in_MCU
                                        *DCTSIZE
#                                       endif
                                      < td->td_samplesperpixel
                                       *sp->cinfo.d.image_height
                                      )
                                     {
                                       TIFFError(tif->tif_name,
                                         "Premature end of JPEG bit-stream");
                                       return 0;
                                     }
            case JPEG_REACHED_EOI: ;
          }
      }
    else /* pixel-interleaved color planes */
      tif->tif_decoderow = tif->tif_decodestrip = tif->tif_decodetile =
        downsampled_output ? OJPEGDecodeRawContig : OJPEGDecode;
    return 1;
#   undef td
  }
>>>>>>> sync with upstream

static int
OJPEGReadHeaderInfoSecTablesQTable(TIFF* tif)
{
	static const char module[]="OJPEGReadHeaderInfoSecTablesQTable";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint8 m;
	uint8 n;
	uint32 oa;
	uint8* ob;
	uint32 p;
	if (sp->qtable_offset[0]==0)
	{
		TIFFErrorExt(tif->tif_clientdata,module,"Missing JPEG tables");
		return(0);
	}
	sp->in_buffer_file_pos_log=0;
	for (m=0; m<sp->samples_per_pixel; m++)
	{
		if ((sp->qtable_offset[m]!=0) && ((m==0) || (sp->qtable_offset[m]!=sp->qtable_offset[m-1])))
		{
			for (n=0; n<m-1; n++)
			{
				if (sp->qtable_offset[m]==sp->qtable_offset[n])
				{
					TIFFErrorExt(tif->tif_clientdata,module,"Corrupt JpegQTables tag value");
					return(0);
				}
			}
			oa=sizeof(uint32)+69;
			ob=_TIFFmalloc(oa);
			if (ob==0)
			{
				TIFFErrorExt(tif->tif_clientdata,module,"Out of memory");
				return(0);
			}
			*(uint32*)ob=oa;
			ob[sizeof(uint32)]=255;
			ob[sizeof(uint32)+1]=JPEG_MARKER_DQT;
			ob[sizeof(uint32)+2]=0;
			ob[sizeof(uint32)+3]=67;
			ob[sizeof(uint32)+4]=m;
			TIFFSeekFile(tif,sp->qtable_offset[m],SEEK_SET); 
			p=TIFFReadFile(tif,&ob[sizeof(uint32)+5],64);
			if (p!=64)
				return(0);
			sp->qtable[m]=ob;
			sp->sof_tq[m]=m;
		}
		else
			sp->sof_tq[m]=sp->sof_tq[m-1];
	}
	return(1);
}

static int
OJPEGReadHeaderInfoSecTablesDcTable(TIFF* tif)
{
	static const char module[]="OJPEGReadHeaderInfoSecTablesDcTable";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint8 m;
	uint8 n;
	uint8 o[16];
	uint32 p;
	uint32 q;
	uint32 ra;
	uint8* rb;
	if (sp->dctable_offset[0]==0)
	{
		TIFFErrorExt(tif->tif_clientdata,module,"Missing JPEG tables");
		return(0);
	}
	sp->in_buffer_file_pos_log=0;
	for (m=0; m<sp->samples_per_pixel; m++)
	{
		if ((sp->dctable_offset[m]!=0) && ((m==0) || (sp->dctable_offset[m]!=sp->dctable_offset[m-1])))
		{
			for (n=0; n<m-1; n++)
			{
				if (sp->dctable_offset[m]==sp->dctable_offset[n])
				{
					TIFFErrorExt(tif->tif_clientdata,module,"Corrupt JpegDcTables tag value");
					return(0);
				}
			}
			TIFFSeekFile(tif,sp->dctable_offset[m],SEEK_SET);
			p=TIFFReadFile(tif,o,16);
			if (p!=16)
				return(0);
			q=0;
			for (n=0; n<16; n++)
				q+=o[n];
			ra=sizeof(uint32)+21+q;
			rb=_TIFFmalloc(ra);
			if (rb==0)
			{
				TIFFErrorExt(tif->tif_clientdata,module,"Out of memory");
				return(0);
			}
			*(uint32*)rb=ra;
			rb[sizeof(uint32)]=255;
			rb[sizeof(uint32)+1]=JPEG_MARKER_DHT;
			rb[sizeof(uint32)+2]=((19+q)>>8);
			rb[sizeof(uint32)+3]=((19+q)&255);
			rb[sizeof(uint32)+4]=m;
			for (n=0; n<16; n++)
				rb[sizeof(uint32)+5+n]=o[n];
			p=TIFFReadFile(tif,&(rb[sizeof(uint32)+21]),q);
			if (p!=q)
				return(0);
			sp->dctable[m]=rb;
			sp->sos_tda[m]=(m<<4);
		}
<<<<<<< HEAD
		else
			sp->sos_tda[m]=sp->sos_tda[m-1];
	}
	return(1);
}
=======
	  }
          return v32;

     /* BEWARE OF KLUDGE:  According to Charles Auer <Bumble731@msn.com>, if our
                           input is a multi-image (multi-directory) JPEG-in-TIFF
        file is produced by the Wang Imaging application on Microsoft Windows,
        for some reason the first directory excludes the vendor-specific "WANG
        PageControl" tag (32934) that we check below, so the only other way to
        identify these directories is apparently to look for a software-
        identification tag with the substring, "Wang Labs".  Single-image files
        can apparently pass both tests, which causes no harm here, but what a
        mess this is!
     */
        case TIFFTAG_SOFTWARE              :
        {
            char *software;

            v32 = (*sp->vsetparent)(tif,tag,ap);
            if( TIFFGetField( tif, TIFFTAG_SOFTWARE, &software )
                && strstr( software, "Wang Labs" ) )
                sp->is_WANG = 1;
            return v32;
        }

        case TIFFTAG_JPEGPROC              :
        case TIFFTAG_JPEGIFOFFSET          :
        case TIFFTAG_JPEGIFBYTECOUNT       :
        case TIFFTAG_JPEGRESTARTINTERVAL   :
        case TIFFTAG_JPEGLOSSLESSPREDICTORS:
        case TIFFTAG_JPEGPOINTTRANSFORM    :
        case TIFFTAG_JPEGQTABLES           :
        case TIFFTAG_JPEGDCTABLES          :
        case TIFFTAG_JPEGACTABLES          :
        case TIFFTAG_WANG_PAGECONTROL      :
        case TIFFTAG_JPEGCOLORMODE         : ;
      };
    v32 = va_arg(ap,uint32); /* No. of values in this TIFF record */

    /* This switch statement is added for OJPEGVSetField */
    if(v32 !=0){
        switch(tag){
            case TIFFTAG_JPEGPROC:
                sp->jpegproc=v32;
                break;
            case TIFFTAG_JPEGIFOFFSET:
                sp->jpegifoffset=v32;
		break;
            case TIFFTAG_JPEGIFBYTECOUNT:
		sp->jpegifbytecount=v32;
		break;
            case TIFFTAG_JPEGRESTARTINTERVAL:
		sp->jpegrestartinterval=v32;
		break;
            case TIFFTAG_JPEGLOSSLESSPREDICTORS:
		sp->jpeglosslesspredictors_length=v32;
		break;
            case TIFFTAG_JPEGPOINTTRANSFORM:
		sp->jpegpointtransform_length=v32;
		break;
            case TIFFTAG_JPEGQTABLES:
		sp->jpegqtables_length=v32;
		break;
            case TIFFTAG_JPEGACTABLES:
		sp->jpegactables_length=v32;
		break;
            case TIFFTAG_JPEGDCTABLES:
		sp->jpegdctables_length=v32;
		break;
            default:
		break;
        }
    }

 /* BEWARE:  The following actions apply only if we are reading a "source" TIFF
             image to be decompressed for a client application program.  If we
    ever enhance this file's CODEC to write "destination" JPEG-in-TIFF images,
    we'll need an "if"- and another "switch"-statement below, because we'll
    probably want to store these records' values in some different places.  Most
    of these need not be parsed here in order to decode JPEG bit stream, so we
    set boolean flags to note that they have been seen, but we otherwise ignore
    them.
 */
    switch (tag)
      { JHUFF_TBL **h;

     /* Validate the JPEG-process code. */

        case TIFFTAG_JPEGPROC              :
          switch (v32)
            {
              default               : TIFFError(tif->tif_name,
                                        "Unknown JPEG process");
                                      return 0;
#             ifdef C_LOSSLESS_SUPPORTED

           /* Image uses (lossy) baseline sequential coding. */

              case JPEGPROC_BASELINE: sp->cinfo.d.process = JPROC_SEQUENTIAL;
                                      sp->cinfo.d.data_unit = DCTSIZE;
                                      break;

           /* Image uses (lossless) Huffman coding. */

              case JPEGPROC_LOSSLESS: sp->cinfo.d.process = JPROC_LOSSLESS;
                                      sp->cinfo.d.data_unit = 1;
#             else /* not C_LOSSLESS_SUPPORTED */
              case JPEGPROC_LOSSLESS: TIFFError(JPEGLib_name,
                                        "Does not support lossless Huffman coding");
                                      return 0;
              case JPEGPROC_BASELINE: ;
#             endif /* C_LOSSLESS_SUPPORTED */
            };
          break;

     /* The TIFF Version 6.0 specification says that if the value of a TIFF
        "JPEGInterchangeFormat" record is 0, then we are to behave as if this
        record were absent; i.e., the data does *not* represent a JPEG Inter-
        change Format File (JFIF), so don't even set the boolean "I've been
        here" flag below.  Otherwise, the field's value represents the file
        offset of the JPEG SOI marker.
     */
        case TIFFTAG_JPEGIFOFFSET          :
          if (v32)
            {
              sp->src.next_input_byte = tif->tif_base + v32;
              break;
            };
          return 1;
        case TIFFTAG_JPEGIFBYTECOUNT       :
          sp->src.bytes_in_buffer = v32;
          break;

     /* The TIFF Version 6.0 specification says that if the JPEG "Restart"
        marker interval is 0, then the data has no "Restart" markers; i.e., we
        must behave as if this TIFF record were absent.  So, don't even set the
        boolean "I've been here" flag below.
     */
     /*
      * Instead, set the field bit so TIFFGetField can get whether or not
      * it was set.
      */
        case TIFFTAG_JPEGRESTARTINTERVAL   :
          if (v32)
              sp->cinfo.d.restart_interval = v32;
              break;
     /* The TIFF Version 6.0 specification says that this tag is supposed to be
        a vector containing a value for each image component, but for lossless
        Huffman coding (the only JPEG process defined by the specification for
        which this tag should be needed), ISO IS 10918-1 uses only a single
        value, equivalent to the "Ss" field in a JPEG bit-stream's "Start of
        Scan" (SOS) marker.  So, we extract the first vector element and ignore
        the rest.  (I hope this is correct!)
     */
        case TIFFTAG_JPEGLOSSLESSPREDICTORS:
           if (v32)
             {
               sp->cinfo.d.Ss = *va_arg(ap,uint16 *);
               sp->jpeglosslesspredictors = 
		    _TIFFmalloc(sp->jpeglosslesspredictors_length
				* sizeof(uint16));
               if(sp->jpeglosslesspredictors==NULL){return(0);}
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
               for(i2=0;i2<sp->jpeglosslesspredictors_length;i2++){
                ((uint16*)sp->jpeglosslesspredictors)[i2] =
			((uint16*)sp->cinfo.d.Ss)[i2];
               }
               sp->jpeglosslesspredictors_length*=sizeof(uint16);
               break;
             };
           return v32;

     /* The TIFF Version 6.0 specification says that this tag is supposed to be
        a vector containing a value for each image component, but for lossless
        Huffman coding (the only JPEG process defined by the specification for
        which this tag should be needed), ISO IS 10918-1 uses only a single
        value, equivalent to the "Al" field in a JPEG bit-stream's "Start of
        Scan" (SOS) marker.  So, we extract the first vector element and ignore
        the rest.  (I hope this is correct!)
     */
        case TIFFTAG_JPEGPOINTTRANSFORM    :
           if (v32)
             {
               sp->cinfo.d.Al = *va_arg(ap,uint16 *);
               sp->jpegpointtransform =
		    _TIFFmalloc(sp->jpegpointtransform_length*sizeof(uint16));
               if(sp->jpegpointtransform==NULL){return(0);}
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
               for(i2=0;i2<sp->jpegpointtransform_length;i2++) {
                ((uint16*)sp->jpegpointtransform)[i2] =
			((uint16*)sp->cinfo.d.Al)[i2];
               }
               sp->jpegpointtransform_length*=sizeof(uint16);
               break;
             }
           return v32;

     /* We have a vector of offsets to quantization tables, so load 'em! */

        case TIFFTAG_JPEGQTABLES           :
          if (v32)
            { uint32 *v;
              int i;
              if (v32 > NUM_QUANT_TBLS)
                {
                  TIFFError(tif->tif_name,"Too many quantization tables");
                  return 0;
                };
              i = 0;
              v = va_arg(ap,uint32 *);
                sp->jpegqtables=_TIFFmalloc(64*sp->jpegqtables_length);
                if(sp->jpegqtables==NULL){return(0);}
                tiffoff = TIFFSeekFile(tif, 0, SEEK_CUR);
                bufoff=0;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                for(i2=0;i2<sp->jpegqtables_length;i2++){
                    TIFFSeekFile(tif, v[i2], SEEK_SET);
                    TIFFReadFile(tif, &(((unsigned char*)(sp->jpegqtables))[bufoff]),
				 64);
                    bufoff+=64;
                }
                sp->jpegqtables_length=bufoff;
                TIFFSeekFile(tif, tiffoff, SEEK_SET);

              do /* read quantization table */
                { register UINT8 *from = tif->tif_base + *v++;
                  register UINT16 *to;
                  register int j = DCTSIZE2;

                  if (!( sp->cinfo.d.quant_tbl_ptrs[i]
                       = CALLJPEG(sp,0,jpeg_alloc_quant_table(&sp->cinfo.comm))
                       )
                     )
                    {
                      TIFFError(JPEGLib_name,"No space for quantization table");
                      return 0;
                    };
                  to = sp->cinfo.d.quant_tbl_ptrs[i]->quantval;
                  do *to++ = *from++; while (--j > 0);
                }
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
              while (++i < v32);
              sp->jpegtablesmode |= JPEGTABLESMODE_QUANT;
            };
          break;

     /* We have a vector of offsets to DC Huffman tables, so load 'em! */

        case TIFFTAG_JPEGDCTABLES          :
          h = sp->cinfo.d.dc_huff_tbl_ptrs;
          goto L;

     /* We have a vector of offsets to AC Huffman tables, so load 'em! */

        case TIFFTAG_JPEGACTABLES          :
          h = sp->cinfo.d.ac_huff_tbl_ptrs;
       L: if (v32)
            { uint32 *v;
              int i;
              if (v32 > NUM_HUFF_TBLS)
                {
                  TIFFError(tif->tif_name,"Too many Huffman tables");
                  return 0;
                };
              v = va_arg(ap,uint32 *);
                if(tag == TIFFTAG_JPEGDCTABLES) {
                    sp->jpegdctables=_TIFFmalloc(272*sp->jpegdctables_length);
                    if(sp->jpegdctables==NULL){return(0);}
                    tiffoff = TIFFSeekFile(tif, 0, SEEK_CUR);
                    bufoff=0;
                    code_count=0;                
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                    for(i2=0;i2<sp->jpegdctables_length;i2++){
                        TIFFSeekFile(tif, v[i2], SEEK_SET);
                        TIFFReadFile(tif,
				     &(((unsigned char*)(sp->jpegdctables))[bufoff]),
				     16);
                        code_count=0;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                        for(k2=0;k2<16;k2++){
                            code_count+=((unsigned char*)(sp->jpegdctables))[k2+bufoff];
                        }
                        TIFFReadFile(tif,
				     &(((unsigned char*)(sp->jpegdctables))[bufoff+16]),
				     code_count);
                        bufoff+=16;
                        bufoff+=code_count;
                    }
                    sp->jpegdctables_length=bufoff;
                    TIFFSeekFile(tif, tiffoff, SEEK_SET);
                }
                if(tag==TIFFTAG_JPEGACTABLES){
                    sp->jpegactables=_TIFFmalloc(272*sp->jpegactables_length);
                    if(sp->jpegactables==NULL){return(0);}
                    tiffoff = TIFFSeekFile(tif, 0, SEEK_CUR);
                    bufoff=0;
                    code_count=0;                
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                    for(i2=0;i2<sp->jpegactables_length;i2++){
                        TIFFSeekFile(tif, v[i2], SEEK_SET);
                        TIFFReadFile(tif, &(((unsigned char*)(sp->jpegactables))[bufoff]), 16);
                        code_count=0;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                        for(k2=0;k2<16;k2++){
                            code_count+=((unsigned char*)(sp->jpegactables))[k2+bufoff];
                        }
                        TIFFReadFile(tif, &(((unsigned char*)(sp->jpegactables))[bufoff+16]), code_count);
                        bufoff+=16;
                        bufoff+=code_count;
                    }
                    sp->jpegactables_length=bufoff;
                    TIFFSeekFile(tif, tiffoff, SEEK_SET);
                }
              i = 0;
              do /* copy each Huffman table */
                { int size = 0;
                  register UINT8 *from = tif->tif_base + *v++, *to;
                  register int j = sizeof (*h)->bits;

               /* WARNING:  This code relies on the fact that an image file not
                            "memory mapped" was read entirely into a single
                  buffer by "TIFFInitOJPEG()", so we can do a fast memory-to-
                  memory copy here.  Each table consists of 16 Bytes, which are
                  suffixed to a 0 Byte when copied, followed by a variable
                  number of Bytes whose length is the sum of the first 16.
               */
                  if (!( *h
                       = CALLJPEG(sp,0,jpeg_alloc_huff_table(&sp->cinfo.comm))
                       )
                     )
                    {
                      TIFFError(JPEGLib_name,"No space for Huffman table");
                      return 0;
                    };
                  to = (*h++)->bits;
                  *to++ = 0;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                  while (--j > 0) size += *to++ = *from++; /* Copy 16 Bytes */
                  if (size > sizeof (*h)->huffval/sizeof *(*h)->huffval)
                    {
                      TIFFError(tif->tif_name,"Huffman table too big");
                      return 0;
                    };
                  if ((j = size) > 0) do *to++ = *from++; while (--j > 0);
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                  while (++size <= sizeof (*h)->huffval/sizeof *(*h)->huffval)
                    *to++ = 0; /* Zero the rest of the table for cleanliness */
                }
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
              while (++i < v32);
              sp->jpegtablesmode |= JPEGTABLESMODE_HUFF;
            };
          break;

     /* The following vendor-specific TIFF tag occurs in (highly illegal) files
        produced by the Wang Imaging application for Microsoft Windows.  These
        can apparently have several "pages", in which case this tag specifies
        the offset of a "page control" structure, which we don't currently know
        how to handle.  0 indicates a 1-page image with no "page control", which
        we make a feeble effort to handle.
     */
        case TIFFTAG_WANG_PAGECONTROL      :
          if (v32 == 0) v32 = -1;
          sp->is_WANG = v32;
          tag = TIFFTAG_JPEGPROC+FIELD_WANG_PAGECONTROL-FIELD_JPEGPROC;
          break;

     /* This pseudo tag indicates whether our caller is expected to do YCbCr <->
        RGB color-space conversion (JPEGCOLORMODE_RAW <=> 0) or whether we must
        ask the JPEG Library to do it (JPEGCOLORMODE_RGB <=> 1).
     */
        case TIFFTAG_JPEGCOLORMODE         :
          sp->jpegcolormode = v32;

       /* Mark the image to indicate whether returned data is up-sampled, so
          that "TIFF{Strip,Tile}Size()" reflect the true amount of data present.
       */
          v32 = tif->tif_flags; /* Save flags temporarily */
          tif->tif_flags &= ~TIFF_UPSAMPLED;
          if (   td->td_photometric == PHOTOMETRIC_YCBCR
              &&    (td->td_ycbcrsubsampling[0]<<3 | td->td_ycbcrsubsampling[1])
                 != 011
              && sp->jpegcolormode == JPEGCOLORMODE_RGB
             ) tif->tif_flags |= TIFF_UPSAMPLED;

       /* If the up-sampling state changed, re-calculate tile size. */

          if ((tif->tif_flags ^ v32) & TIFF_UPSAMPLED)
            {
              tif->tif_tilesize = isTiled(tif) ? TIFFTileSize(tif) : (tsize_t) -1;
              tif->tif_flags |= TIFF_DIRTYDIRECT;
            };
          return 1;
      };
    TIFFSetFieldBit(tif,tag-TIFFTAG_JPEGPROC+FIELD_JPEGPROC);
    return 1;
#   undef td
  }
>>>>>>> sync with upstream

static int
OJPEGReadHeaderInfoSecTablesAcTable(TIFF* tif)
{
	static const char module[]="OJPEGReadHeaderInfoSecTablesAcTable";
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint8 m;
	uint8 n;
	uint8 o[16];
	uint32 p;
	uint32 q;
	uint32 ra;
	uint8* rb;
	if (sp->actable_offset[0]==0)
	{
		TIFFErrorExt(tif->tif_clientdata,module,"Missing JPEG tables");
		return(0);
	}
	sp->in_buffer_file_pos_log=0;
	for (m=0; m<sp->samples_per_pixel; m++)
	{
		if ((sp->actable_offset[m]!=0) && ((m==0) || (sp->actable_offset[m]!=sp->actable_offset[m-1])))
		{
			for (n=0; n<m-1; n++)
			{
				if (sp->actable_offset[m]==sp->actable_offset[n])
				{
					TIFFErrorExt(tif->tif_clientdata,module,"Corrupt JpegAcTables tag value");
					return(0);
				}
			}
			TIFFSeekFile(tif,sp->actable_offset[m],SEEK_SET);  
			p=TIFFReadFile(tif,o,16);
			if (p!=16)
				return(0);
			q=0;
			for (n=0; n<16; n++)
				q+=o[n];
			ra=sizeof(uint32)+21+q;
			rb=_TIFFmalloc(ra);
			if (rb==0)
			{
				TIFFErrorExt(tif->tif_clientdata,module,"Out of memory");
				return(0);
			}
			*(uint32*)rb=ra;
			rb[sizeof(uint32)]=255;
			rb[sizeof(uint32)+1]=JPEG_MARKER_DHT;
			rb[sizeof(uint32)+2]=((19+q)>>8);
			rb[sizeof(uint32)+3]=((19+q)&255);
			rb[sizeof(uint32)+4]=(16|m);
			for (n=0; n<16; n++)
				rb[sizeof(uint32)+5+n]=o[n];
			p=TIFFReadFile(tif,&(rb[sizeof(uint32)+21]),q);
			if (p!=q)
				return(0);
			sp->actable[m]=rb;
			sp->sos_tda[m]=(sp->sos_tda[m]|m);
		}
		else
			sp->sos_tda[m]=(sp->sos_tda[m]|(sp->sos_tda[m-1]&15));
	}
	return(1);
}

static int
OJPEGReadBufferFill(OJPEGState* sp)
{
	uint16 m;
	tmsize_t n;
	/* TODO: double-check: when subsamplingcorrect is set, no call to TIFFErrorExt or TIFFWarningExt should be made
	 * in any other case, seek or read errors should be passed through */
	do
	{
		if (sp->in_buffer_file_togo!=0)
		{
			if (sp->in_buffer_file_pos_log==0)
			{
				TIFFSeekFile(sp->tif,sp->in_buffer_file_pos,SEEK_SET);
				sp->in_buffer_file_pos_log=1;
			}
			m=OJPEG_BUFFER;
			if ((uint64)m>sp->in_buffer_file_togo)
				m=(uint16)sp->in_buffer_file_togo;
			n=TIFFReadFile(sp->tif,sp->in_buffer,(tmsize_t)m);
			if (n==0)
				return(0);
			assert(n>0);
			assert(n<=OJPEG_BUFFER);
			assert(n<65536);
			assert((uint64)n<=sp->in_buffer_file_togo);
			m=(uint16)n;
			sp->in_buffer_togo=m;
			sp->in_buffer_cur=sp->in_buffer;
			sp->in_buffer_file_togo-=m;
			sp->in_buffer_file_pos+=m;
			break;
		}
		sp->in_buffer_file_pos_log=0;
		switch(sp->in_buffer_source)
		{
			case osibsNotSetYet:
				if (sp->jpeg_interchange_format!=0)
				{
					sp->in_buffer_file_pos=sp->jpeg_interchange_format;
					sp->in_buffer_file_togo=sp->jpeg_interchange_format_length;
				}
				sp->in_buffer_source=osibsJpegInterchangeFormat;
				break;
			case osibsJpegInterchangeFormat:
				sp->in_buffer_source=osibsStrile;
			case osibsStrile:
				if (!_TIFFFillStriles( sp->tif ) 
				    || sp->tif->tif_dir.td_stripoffset == NULL
				    || sp->tif->tif_dir.td_stripbytecount == NULL)
					return 0;

				if (sp->in_buffer_next_strile==sp->in_buffer_strile_count)
					sp->in_buffer_source=osibsEof;
				else
				{
					sp->in_buffer_file_pos=sp->tif->tif_dir.td_stripoffset[sp->in_buffer_next_strile];
					if (sp->in_buffer_file_pos!=0)
					{
						if (sp->in_buffer_file_pos>=sp->file_size)
							sp->in_buffer_file_pos=0;
						else if (sp->tif->tif_dir.td_stripbytecount==NULL)
							sp->in_buffer_file_togo=sp->file_size-sp->in_buffer_file_pos;
						else
						{
							if (sp->tif->tif_dir.td_stripbytecount == 0) {
								TIFFErrorExt(sp->tif->tif_clientdata,sp->tif->tif_name,"Strip byte counts are missing");
								return(0);
							}
							sp->in_buffer_file_togo=sp->tif->tif_dir.td_stripbytecount[sp->in_buffer_next_strile];
							if (sp->in_buffer_file_togo==0)
								sp->in_buffer_file_pos=0;
							else if (sp->in_buffer_file_pos+sp->in_buffer_file_togo>sp->file_size)
								sp->in_buffer_file_togo=sp->file_size-sp->in_buffer_file_pos;
						}
					}
					sp->in_buffer_next_strile++;
				}
				break;
			default:
				return(0);
		}
	} while (1);
	return(1);
}

static int
OJPEGReadByte(OJPEGState* sp, uint8* byte)
{
	if (sp->in_buffer_togo==0)
	{
		if (OJPEGReadBufferFill(sp)==0)
			return(0);
		assert(sp->in_buffer_togo>0);
	}
	*byte=*(sp->in_buffer_cur);
	sp->in_buffer_cur++;
	sp->in_buffer_togo--;
	return(1);
}

static int
OJPEGReadBytePeek(OJPEGState* sp, uint8* byte)
{
	if (sp->in_buffer_togo==0)
	{
		if (OJPEGReadBufferFill(sp)==0)
			return(0);
		assert(sp->in_buffer_togo>0);
	}
	*byte=*(sp->in_buffer_cur);
	return(1);
}

static void
OJPEGReadByteAdvance(OJPEGState* sp)
{
	assert(sp->in_buffer_togo>0);
	sp->in_buffer_cur++;
	sp->in_buffer_togo--;
}

static int
OJPEGReadWord(OJPEGState* sp, uint16* word)
{
	uint8 m;
	if (OJPEGReadByte(sp,&m)==0)
		return(0);
	*word=(m<<8);
	if (OJPEGReadByte(sp,&m)==0)
		return(0);
	*word|=m;
	return(1);
}

static int
OJPEGReadBlock(OJPEGState* sp, uint16 len, void* mem)
{
	uint16 mlen;
	uint8* mmem;
	uint16 n;
	assert(len>0);
	mlen=len;
	mmem=mem;
	do
	{
		if (sp->in_buffer_togo==0)
		{
			if (OJPEGReadBufferFill(sp)==0)
				return(0);
			assert(sp->in_buffer_togo>0);
		}
		n=mlen;
		if (n>sp->in_buffer_togo)
			n=sp->in_buffer_togo;
		_TIFFmemcpy(mmem,sp->in_buffer_cur,n);
		sp->in_buffer_cur+=n;
		sp->in_buffer_togo-=n;
		mlen-=n;
		mmem+=n;
	} while(mlen>0);
	return(1);
}

static void
OJPEGReadSkip(OJPEGState* sp, uint16 len)
{
	uint16 m;
	uint16 n;
	m=len;
	n=m;
	if (n>sp->in_buffer_togo)
		n=sp->in_buffer_togo;
	sp->in_buffer_cur+=n;
	sp->in_buffer_togo-=n;
	m-=n;
	if (m>0)
	{
		assert(sp->in_buffer_togo==0);
		n=m;
		if ((uint64)n>sp->in_buffer_file_togo)
			n=(uint16)sp->in_buffer_file_togo;
		sp->in_buffer_file_pos+=n;
		sp->in_buffer_file_togo-=n;
		sp->in_buffer_file_pos_log=0;
		/* we don't skip past jpeginterchangeformat/strile block...
		 * if that is asked from us, we're dealing with totally bazurk
		 * data anyway, and we've not seen this happening on any
		 * testfile, so we might as well likely cause some other
		 * meaningless error to be passed at some later time
		 */
	}
}

static int
OJPEGWriteStream(TIFF* tif, void** mem, uint32* len)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	*len=0;
	do
	{
		assert(sp->out_state<=ososEoi);
		switch(sp->out_state)
		{
			case ososSoi:
				OJPEGWriteStreamSoi(tif,mem,len);
				break;
			case ososQTable0:
				OJPEGWriteStreamQTable(tif,0,mem,len);
				break;
			case ososQTable1:
				OJPEGWriteStreamQTable(tif,1,mem,len);
				break;
			case ososQTable2:
				OJPEGWriteStreamQTable(tif,2,mem,len);
				break;
			case ososQTable3:
				OJPEGWriteStreamQTable(tif,3,mem,len);
				break;
			case ososDcTable0:
				OJPEGWriteStreamDcTable(tif,0,mem,len);
				break;
			case ososDcTable1:
				OJPEGWriteStreamDcTable(tif,1,mem,len);
				break;
			case ososDcTable2:
				OJPEGWriteStreamDcTable(tif,2,mem,len);
				break;
			case ososDcTable3:
				OJPEGWriteStreamDcTable(tif,3,mem,len);
				break;
			case ososAcTable0:
				OJPEGWriteStreamAcTable(tif,0,mem,len);
				break;
			case ososAcTable1:
				OJPEGWriteStreamAcTable(tif,1,mem,len);
				break;
			case ososAcTable2:
				OJPEGWriteStreamAcTable(tif,2,mem,len);
				break;
			case ososAcTable3:
				OJPEGWriteStreamAcTable(tif,3,mem,len);
				break;
			case ososDri:
				OJPEGWriteStreamDri(tif,mem,len);
				break;
			case ososSof:
				OJPEGWriteStreamSof(tif,mem,len);
				break;
			case ososSos:
				OJPEGWriteStreamSos(tif,mem,len);
				break;
			case ososCompressed:
				if (OJPEGWriteStreamCompressed(tif,mem,len)==0)
					return(0);
				break;
			case ososRst:
				OJPEGWriteStreamRst(tif,mem,len);
				break;
			case ososEoi:
				OJPEGWriteStreamEoi(tif,mem,len);
				break;
		}
	} while (*len==0);
	return(1);
}

static void
OJPEGWriteStreamSoi(TIFF* tif, void** mem, uint32* len)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	assert(OJPEG_BUFFER>=2);
	sp->out_buffer[0]=255;
	sp->out_buffer[1]=JPEG_MARKER_SOI;
	*len=2;
	*mem=(void*)sp->out_buffer;
	sp->out_state++;
}

static void
OJPEGWriteStreamQTable(TIFF* tif, uint8 table_index, void** mem, uint32* len)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	if (sp->qtable[table_index]!=0)
	{
		*mem=(void*)(sp->qtable[table_index]+sizeof(uint32));
		*len=*((uint32*)sp->qtable[table_index])-sizeof(uint32);
	}
	sp->out_state++;
}

static void
OJPEGWriteStreamDcTable(TIFF* tif, uint8 table_index, void** mem, uint32* len)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	if (sp->dctable[table_index]!=0)
	{
		*mem=(void*)(sp->dctable[table_index]+sizeof(uint32));
		*len=*((uint32*)sp->dctable[table_index])-sizeof(uint32);
	}
	sp->out_state++;
}

static void
OJPEGWriteStreamAcTable(TIFF* tif, uint8 table_index, void** mem, uint32* len)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	if (sp->actable[table_index]!=0)
	{
		*mem=(void*)(sp->actable[table_index]+sizeof(uint32));
		*len=*((uint32*)sp->actable[table_index])-sizeof(uint32);
	}
	sp->out_state++;
}

static void
OJPEGWriteStreamDri(TIFF* tif, void** mem, uint32* len)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	assert(OJPEG_BUFFER>=6);
	if (sp->restart_interval!=0)
	{
		sp->out_buffer[0]=255;
		sp->out_buffer[1]=JPEG_MARKER_DRI;
		sp->out_buffer[2]=0;
		sp->out_buffer[3]=4;
		sp->out_buffer[4]=(sp->restart_interval>>8);
		sp->out_buffer[5]=(sp->restart_interval&255);
		*len=6;
		*mem=(void*)sp->out_buffer;
	}
	sp->out_state++;
}

static void
OJPEGWriteStreamSof(TIFF* tif, void** mem, uint32* len)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint8 m;
	assert(OJPEG_BUFFER>=2+8+sp->samples_per_pixel_per_plane*3);
	assert(255>=8+sp->samples_per_pixel_per_plane*3);
	sp->out_buffer[0]=255;
	sp->out_buffer[1]=sp->sof_marker_id;
	/* Lf */
	sp->out_buffer[2]=0;
	sp->out_buffer[3]=8+sp->samples_per_pixel_per_plane*3;
	/* P */
	sp->out_buffer[4]=8;
	/* Y */
	sp->out_buffer[5]=(sp->sof_y>>8);
	sp->out_buffer[6]=(sp->sof_y&255);
	/* X */
	sp->out_buffer[7]=(sp->sof_x>>8);
	sp->out_buffer[8]=(sp->sof_x&255);
	/* Nf */
	sp->out_buffer[9]=sp->samples_per_pixel_per_plane;
	for (m=0; m<sp->samples_per_pixel_per_plane; m++)
	{
		/* C */
		sp->out_buffer[10+m*3]=sp->sof_c[sp->plane_sample_offset+m];
		/* H and V */
		sp->out_buffer[10+m*3+1]=sp->sof_hv[sp->plane_sample_offset+m];
		/* Tq */
		sp->out_buffer[10+m*3+2]=sp->sof_tq[sp->plane_sample_offset+m];
	}
	*len=10+sp->samples_per_pixel_per_plane*3;
	*mem=(void*)sp->out_buffer;
	sp->out_state++;
}

static void
OJPEGWriteStreamSos(TIFF* tif, void** mem, uint32* len)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	uint8 m;
	assert(OJPEG_BUFFER>=2+6+sp->samples_per_pixel_per_plane*2);
	assert(255>=6+sp->samples_per_pixel_per_plane*2);
	sp->out_buffer[0]=255;
	sp->out_buffer[1]=JPEG_MARKER_SOS;
	/* Ls */
	sp->out_buffer[2]=0;
	sp->out_buffer[3]=6+sp->samples_per_pixel_per_plane*2;
	/* Ns */
	sp->out_buffer[4]=sp->samples_per_pixel_per_plane;
	for (m=0; m<sp->samples_per_pixel_per_plane; m++)
	{
		/* Cs */
		sp->out_buffer[5+m*2]=sp->sos_cs[sp->plane_sample_offset+m];
		/* Td and Ta */
		sp->out_buffer[5+m*2+1]=sp->sos_tda[sp->plane_sample_offset+m];
	}
	/* Ss */
	sp->out_buffer[5+sp->samples_per_pixel_per_plane*2]=0;
	/* Se */
	sp->out_buffer[5+sp->samples_per_pixel_per_plane*2+1]=63;
	/* Ah and Al */
	sp->out_buffer[5+sp->samples_per_pixel_per_plane*2+2]=0;
	*len=8+sp->samples_per_pixel_per_plane*2;
	*mem=(void*)sp->out_buffer;
	sp->out_state++;
}

static int
OJPEGWriteStreamCompressed(TIFF* tif, void** mem, uint32* len)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	if (sp->in_buffer_togo==0)
	{
		if (OJPEGReadBufferFill(sp)==0)
			return(0);
		assert(sp->in_buffer_togo>0);
	}
	*len=sp->in_buffer_togo;
	*mem=(void*)sp->in_buffer_cur;
	sp->in_buffer_togo=0;
	if (sp->in_buffer_file_togo==0)
	{
		switch(sp->in_buffer_source)
		{
			case osibsStrile:
				if (sp->in_buffer_next_strile<sp->in_buffer_strile_count)
					sp->out_state=ososRst;
				else
					sp->out_state=ososEoi;
				break;
			case osibsEof:
				sp->out_state=ososEoi;
				break;
			default:
				break;
		}
	}
	return(1);
}

static void
OJPEGWriteStreamRst(TIFF* tif, void** mem, uint32* len)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	assert(OJPEG_BUFFER>=2);
	sp->out_buffer[0]=255;
	sp->out_buffer[1]=JPEG_MARKER_RST0+sp->restart_index;
	sp->restart_index++;
	if (sp->restart_index==8)
		sp->restart_index=0;
	*len=2;
	*mem=(void*)sp->out_buffer;
	sp->out_state=ososCompressed;
}

static void
OJPEGWriteStreamEoi(TIFF* tif, void** mem, uint32* len)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	assert(OJPEG_BUFFER>=2);
	sp->out_buffer[0]=255;
	sp->out_buffer[1]=JPEG_MARKER_EOI;
	*len=2;
	*mem=(void*)sp->out_buffer;
}

#ifndef LIBJPEG_ENCAP_EXTERNAL
static int
jpeg_create_decompress_encap(OJPEGState* sp, jpeg_decompress_struct* cinfo)
{
	return(SETJMP(sp->exit_jmpbuf)?0:(jpeg_create_decompress(cinfo),1));
}
#endif

#ifndef LIBJPEG_ENCAP_EXTERNAL
static int
jpeg_read_header_encap(OJPEGState* sp, jpeg_decompress_struct* cinfo, uint8 require_image)
{
	return(SETJMP(sp->exit_jmpbuf)?0:(jpeg_read_header(cinfo,require_image),1));
}
#endif

#ifndef LIBJPEG_ENCAP_EXTERNAL
static int
jpeg_start_decompress_encap(OJPEGState* sp, jpeg_decompress_struct* cinfo)
{
	return(SETJMP(sp->exit_jmpbuf)?0:(jpeg_start_decompress(cinfo),1));
}
#endif

#ifndef LIBJPEG_ENCAP_EXTERNAL
static int
jpeg_read_scanlines_encap(OJPEGState* sp, jpeg_decompress_struct* cinfo, void* scanlines, uint32 max_lines)
{
	return(SETJMP(sp->exit_jmpbuf)?0:(jpeg_read_scanlines(cinfo,scanlines,max_lines),1));
}
#endif

#ifndef LIBJPEG_ENCAP_EXTERNAL
static int
jpeg_read_raw_data_encap(OJPEGState* sp, jpeg_decompress_struct* cinfo, void* data, uint32 max_lines)
{
	return(SETJMP(sp->exit_jmpbuf)?0:(jpeg_read_raw_data(cinfo,data,max_lines),1));
}
#endif

#ifndef LIBJPEG_ENCAP_EXTERNAL
static void
jpeg_encap_unwind(TIFF* tif)
{
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	LONGJMP(sp->exit_jmpbuf,1);
}
#endif

static void
OJPEGLibjpegJpegErrorMgrOutputMessage(jpeg_common_struct* cinfo)
{
	char buffer[JMSG_LENGTH_MAX];
	(*cinfo->err->format_message)(cinfo,buffer);
	TIFFWarningExt(((TIFF*)(cinfo->client_data))->tif_clientdata,"LibJpeg","%s",buffer);
}

static void
OJPEGLibjpegJpegErrorMgrErrorExit(jpeg_common_struct* cinfo)
{
	char buffer[JMSG_LENGTH_MAX];
	(*cinfo->err->format_message)(cinfo,buffer);
	TIFFErrorExt(((TIFF*)(cinfo->client_data))->tif_clientdata,"LibJpeg","%s",buffer);
	jpeg_encap_unwind((TIFF*)(cinfo->client_data));
}

static void
OJPEGLibjpegJpegSourceMgrInitSource(jpeg_decompress_struct* cinfo)
{
	(void)cinfo;
}

static boolean
OJPEGLibjpegJpegSourceMgrFillInputBuffer(jpeg_decompress_struct* cinfo)
{
	TIFF* tif=(TIFF*)cinfo->client_data;
	OJPEGState* sp=(OJPEGState*)tif->tif_data;
	void* mem=0;
	uint32 len=0U;
	if (OJPEGWriteStream(tif,&mem,&len)==0)
	{
		TIFFErrorExt(tif->tif_clientdata,"LibJpeg","Premature end of JPEG data");
		jpeg_encap_unwind(tif);
	}
	sp->libjpeg_jpeg_source_mgr.bytes_in_buffer=len;
	sp->libjpeg_jpeg_source_mgr.next_input_byte=mem;
	return(1);
}

static void
OJPEGLibjpegJpegSourceMgrSkipInputData(jpeg_decompress_struct* cinfo, long num_bytes)
{
	TIFF* tif=(TIFF*)cinfo->client_data;
	(void)num_bytes;
	TIFFErrorExt(tif->tif_clientdata,"LibJpeg","Unexpected error");
	jpeg_encap_unwind(tif);
}

static boolean
OJPEGLibjpegJpegSourceMgrResyncToRestart(jpeg_decompress_struct* cinfo, int desired)
{
	TIFF* tif=(TIFF*)cinfo->client_data;
	(void)desired;
	TIFFErrorExt(tif->tif_clientdata,"LibJpeg","Unexpected error");
	jpeg_encap_unwind(tif);
	return(0);
}

static void
OJPEGLibjpegJpegSourceMgrTermSource(jpeg_decompress_struct* cinfo)
{
	(void)cinfo;
}

#endif


/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 8
 * fill-column: 78
 * End:
 */
