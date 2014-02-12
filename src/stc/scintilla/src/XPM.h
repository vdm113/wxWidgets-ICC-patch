/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

// Scintilla source code edit control
/** @file XPM.h
 ** Define a class that holds data in the X Pixmap (XPM) format.
 **/
// Copyright 1998-2003 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef XPM_H
#define XPM_H

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif

/**
 * Hold a pixmap in XPM format.
 */
class XPM {
	int pid;		// Assigned by container
	int height;
	int width;
	int nColours;
	char *data;
	char codeTransparent;
	char *codes;
	ColourDesired *colours;
	ColourDesired ColourDesiredFromCode(int ch) const;
	ColourDesired ColourFromCode(int ch) const;
	void FillRun(Surface *surface, int code, int startX, int y, int x);
	char **lines;
	ColourDesired *colourCodeTable[256];
public:
	XPM(const char *textForm);
	XPM(const char *const *linesForm);
	~XPM();
	void Init(const char *textForm);
	void Init(const char *const *linesForm);
	void Clear();
	/// Decompose image into runs and use FillRectangle for each run
	void Draw(Surface *surface, PRectangle &rc);
	char **InLinesForm() { return lines; }
	void SetId(int pid_) { pid = pid_; }
	int GetId() const { return pid; }
	int GetHeight() const { return height; }
	int GetWidth() const { return width; }
	void PixelAt(int x, int y, ColourDesired &colour, bool &transparent) const;
	static const char **LinesFormFromTextForm(const char *textForm);
};

/**
 * A collection of pixmaps indexed by integer id.
 */
class XPMSet {
	XPM **set;	///< The stored XPMs.
	int len;	///< Current number of XPMs.
	int maximum;	///< Current maximum number of XPMs, increased by steps if reached.
	int height;	///< Memorize largest height of the set.
	int width;	///< Memorize largest width of the set.
public:
	XPMSet();
	~XPMSet();
	/// Remove all XPMs.
	void Clear();
	/// Add a XPM.
	void Add(int ident, const char *textForm);
	/// Get XPM by id.
	XPM *Get(int ident);
	/// Give the largest height of the set.
	int GetHeight();
	/// Give the largest width of the set.
	int GetWidth();
};

/**
 * An translucent image stoed as a sequence of RGBA bytes.
 */
class RGBAImage {
	// Private so RGBAImage objects can not be copied
	RGBAImage(const RGBAImage &);
	RGBAImage &operator=(const RGBAImage &);
	int height;
	int width;
	std::vector<unsigned char> pixelBytes;
public:
	RGBAImage(int width_, int height_, const unsigned char *pixels_);
	RGBAImage(const XPM &xpm);
	virtual ~RGBAImage();
	int GetHeight() const { return height; }
	int GetWidth() const { return width; }
	int CountBytes() const;
	const unsigned char *Pixels() const;
	void SetPixel(int x, int y, ColourDesired colour, int alpha=0xff); 
};

/**
 * A collection of RGBAImage pixmaps indexed by integer id.
 */
class RGBAImageSet {
	typedef std::map<int, RGBAImage*> ImageMap;
	ImageMap images;
	mutable int height;	///< Memorize largest height of the set.
	mutable int width;	///< Memorize largest width of the set.
public:
	RGBAImageSet();
	~RGBAImageSet();
	/// Remove all images.
	void Clear();
	/// Add an image.
	void Add(int ident, RGBAImage *image);
	/// Get image by id.
	RGBAImage *Get(int ident);
	/// Give the largest height of the set.
	int GetHeight() const;
	/// Give the largest width of the set.
	int GetWidth() const;
};

#ifdef SCI_NAMESPACE
}
#endif

#endif
