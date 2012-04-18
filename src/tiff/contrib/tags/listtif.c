/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

/*
 * listtif.c -- lists a tiff file.
 */

#include "xtiffio.h"
#include <stdlib.h>

void main(int argc,char *argv[])
{
	char *fname="newtif.tif";
	int flags;

	TIFF *tif=(TIFF*)0;  /* TIFF-level descriptor */

	if (argc>1) fname=argv[1];
	
	tif=XTIFFOpen(fname,"r");
	if (!tif) goto failure;
	
	/* We want the double array listed */
	flags = TIFFPRINT_MYMULTIDOUBLES;
	
	TIFFPrintDirectory(tif,stdout,flags);
	XTIFFClose(tif);
	exit (0);
	
failure:
	printf("failure in listtif\n");
	if (tif) XTIFFClose(tif);
	exit (-1);
}

