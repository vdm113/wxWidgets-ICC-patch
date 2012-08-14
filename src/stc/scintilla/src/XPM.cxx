/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP /* nevermind */
#endif

// Scintilla source code edit control
/** @file XPM.cxx
 ** Define a class that holds data in the X Pixmap (XPM) format.
 **/
// Copyright 1998-2003 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <string.h>
#include <stdlib.h>

#include <vector>
#include <map>

#include "Platform.h"

#include "XPM.h"

#ifdef SCI_NAMESPACE
using namespace Scintilla;
#endif

static const char *NextField(const char *s) {
	// In case there are leading spaces in the string
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	while (*s && *s == ' ') {
		s++;
	}
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	while (*s && *s != ' ') {
		s++;
	}
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	while (*s && *s == ' ') {
		s++;
	}
	return s;
}

// Data lines in XPM can be terminated either with NUL or "
static size_t MeasureLength(const char *s) {
	size_t i = 0;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	while (s[i] && (s[i] != '\"'))
		i++;
	return i;
}

ColourDesired XPM::ColourDesiredFromCode(int ch) const {
	return *colourCodeTable[ch];
}

ColourDesired XPM::ColourFromCode(int ch) const {
	return *colourCodeTable[ch];
}

void XPM::FillRun(Surface *surface, int code, int startX, int y, int x) {
	if ((code != codeTransparent) && (startX != x)) {
		PRectangle rc(startX, y, x, y+1);
		surface->FillRectangle(rc, ColourFromCode(code));
	}
}

XPM::XPM(const char *textForm) :
	data(0), codes(0), colours(0), lines(0) {
	Init(textForm);
}

XPM::XPM(const char *const *linesForm) :
	data(0), codes(0), colours(0), lines(0) {
	Init(linesForm);
}

XPM::~XPM() {
	Clear();
}

void XPM::Init(const char *textForm) {
	Clear();
	// Test done is two parts to avoid possibility of overstepping the memory
	// if memcmp implemented strangely. Must be 4 bytes at least at destination.
	if ((0 == memcmp(textForm, "/* X", 4)) && (0 == memcmp(textForm, "/* XPM */", 9))) {
		// Build the lines form out of the text form
		const char **linesForm = LinesFormFromTextForm(textForm);
		if (linesForm != 0) {
			Init(linesForm);
			delete []linesForm;
		}
	} else {
		// It is really in line form
		Init(reinterpret_cast<const char * const *>(textForm));
	}
}

void XPM::Init(const char *const *linesForm) {
	Clear();
	height = 1;
	width = 1;
	nColours = 1;
	data = NULL;
	codeTransparent = ' ';
	codes = NULL;
	colours = NULL;
	lines = NULL;
	if (!linesForm)
		return;

	const char *line0 = linesForm[0];
	width = atoi(line0);
	line0 = NextField(line0);
	height = atoi(line0);
	line0 = NextField(line0);
	nColours = atoi(line0);
	line0 = NextField(line0);
	if (atoi(line0) != 1) {
		// Only one char per pixel is supported
		return;
	}
	codes = new char[nColours];
	colours = new ColourDesired[nColours];

	int strings = 1+height+nColours;
	lines = new char *[strings];
	size_t allocation = 0;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	for (int i=0; i<strings; i++) {
		allocation += MeasureLength(linesForm[i]) + 1;
	}
	data = new char[allocation];
	char *nextBit = data;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	for (int j=0; j<strings; j++) {
		lines[j] = nextBit;
		size_t len = MeasureLength(linesForm[j]);
		memcpy(nextBit, linesForm[j], len);
		nextBit += len;
		*nextBit++ = '\0';
	}

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	for (int code=0; code<256; code++) {
		colourCodeTable[code] = 0;
	}

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	for (int c=0; c<nColours; c++) {
		const char *colourDef = linesForm[c+1];
		codes[c] = colourDef[0];
		colourDef += 4;
		if (*colourDef == '#') {
			colours[c].Set(colourDef);
		} else {
			colours[c] = ColourDesired(0xff, 0xff, 0xff);
			codeTransparent = codes[c];
		}
		colourCodeTable[static_cast<unsigned char>(codes[c])] = &(colours[c]);
	}
}

void XPM::Clear() {
	delete []data;
	data = 0;
	delete []codes;
	codes = 0;
	delete []colours;
	colours = 0;
	delete []lines;
	lines = 0;
}

void XPM::Draw(Surface *surface, PRectangle &rc) {
	if (!data || !codes || !colours || !lines) {
		return;
	}
	// Centre the pixmap
	int startY = rc.top + (rc.Height() - height) / 2;
	int startX = rc.left + (rc.Width() - width) / 2;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	for (int y=0; y<height; y++) {
		int prevCode = 0;
		int xStartRun = 0;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
		for (int x=0; x<width; x++) {
			int code = lines[y+nColours+1][x];
			if (code != prevCode) {
				FillRun(surface, prevCode, startX + xStartRun, startY + y, startX + x);
				xStartRun = x;
				prevCode = code;
			}
		}
		FillRun(surface, prevCode, startX + xStartRun, startY + y, startX + width);
	}
}

void XPM::PixelAt(int x, int y, ColourDesired &colour, bool &transparent) const {
	if (!data || !codes || !colours || !lines || (x<0) || (x >= width) || (y<0) || (y >= height)) {
		colour = 0;
		transparent = true;
		return;
	}
	int code = lines[y+nColours+1][x];
	transparent = code == codeTransparent;
	if (transparent) {
		colour = 0;
	} else {
		colour = ColourDesiredFromCode(code).AsLong();
	}
}

const char **XPM::LinesFormFromTextForm(const char *textForm) {
	// Build the lines form out of the text form
	const char **linesForm = 0;
	int countQuotes = 0;
	int strings=1;
	int j=0;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	for (; countQuotes < (2*strings) && textForm[j] != '\0'; j++) {
		if (textForm[j] == '\"') {
			if (countQuotes == 0) {
				// First field: width, height, number of colors, chars per pixel
				const char *line0 = textForm + j + 1;
				// Skip width
				line0 = NextField(line0);
				// Add 1 line for each pixel of height
				strings += atoi(line0);
				line0 = NextField(line0);
				// Add 1 line for each colour
				strings += atoi(line0);
				linesForm = new const char *[strings];
				if (linesForm == 0) {
					break;	// Memory error!
				}
			}
			if (countQuotes / 2 >= strings) {
				break;	// Bad height or number of colors!
			}
			if ((countQuotes & 1) == 0) {
				linesForm[countQuotes / 2] = textForm + j + 1;
			}
			countQuotes++;
		}
	}
	if (textForm[j] == '\0' || countQuotes / 2 > strings) {
		// Malformed XPM! Height + number of colors too high or too low
		delete []linesForm;
		linesForm = 0;
	}
	return linesForm;
}

// In future, may want to minimize search time by sorting and using a binary search.

XPMSet::XPMSet() : set(0), len(0), maximum(0), height(-1), width(-1) {
}

XPMSet::~XPMSet() {
	Clear();
}

void XPMSet::Clear() {
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	for (int i = 0; i < len; i++) {
		delete set[i];
	}
	delete []set;
	set = 0;
	len = 0;
	maximum = 0;
	height = -1;
	width = -1;
}

void XPMSet::Add(int ident, const char *textForm) {
	// Invalidate cached dimensions
	height = -1;
	width = -1;

	// Replace if this id already present
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	for (int i = 0; i < len; i++) {
		if (set[i]->GetId() == ident) {
			set[i]->Init(textForm);
			return;
		}
	}

	// Not present, so add to end
	XPM *pxpm = new XPM(textForm);
	if (pxpm) {
		pxpm->SetId(ident);
		if (len == maximum) {
			maximum += 64;
			XPM **setNew = new XPM *[maximum];
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
			for (int i = 0; i < len; i++) {
				setNew[i] = set[i];
			}
			delete []set;
			set = setNew;
		}
		set[len] = pxpm;
		len++;
	}
}

XPM *XPMSet::Get(int ident) {
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	for (int i = 0; i < len; i++) {
		if (set[i]->GetId() == ident) {
			return set[i];
		}
	}
	return 0;
}

int XPMSet::GetHeight() {
	if (height < 0) {
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
		for (int i = 0; i < len; i++) {
			if (height < set[i]->GetHeight()) {
				height = set[i]->GetHeight();
			}
		}
	}
	return (height > 0) ? height : 0;
}

int XPMSet::GetWidth() {
	if (width < 0) {
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
		for (int i = 0; i < len; i++) {
			if (width < set[i]->GetWidth()) {
				width = set[i]->GetWidth();
			}
		}
	}
	return (width > 0) ? width : 0;
}

RGBAImage::RGBAImage(int width_, int height_, const unsigned char *pixels_) :
	height(height_), width(width_) {
	if (pixels_) {
		pixelBytes.assign(pixels_, pixels_ + CountBytes());
	} else {
		pixelBytes.resize(CountBytes());
	}
}

RGBAImage::RGBAImage(const XPM &xpm) {
	height = xpm.GetHeight();
	width = xpm.GetWidth();
	pixelBytes.resize(CountBytes());
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	for (int y=0; y<height; y++) {
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
		for (int x=0; x<width; x++) {
			ColourDesired colour;
			bool transparent = false;
			xpm.PixelAt(x, y, colour, transparent);
			SetPixel(x, y, colour, transparent ? 0 : 255);
		}
	}
}

RGBAImage::~RGBAImage() {
}

int RGBAImage::CountBytes() const {
	return width * height * 4;
}

const unsigned char *RGBAImage::Pixels() const {
	return &pixelBytes[0];
}

void RGBAImage::SetPixel(int x, int y, ColourDesired colour, int alpha) {
	unsigned char *pixel = &pixelBytes[0] + (y*width+x) * 4;
	// RGBA
	pixel[0] = colour.GetRed();
	pixel[1] = colour.GetGreen();
	pixel[2] = colour.GetBlue();
	pixel[3] = alpha;
}

RGBAImageSet::RGBAImageSet() : height(-1), width(-1){
}

RGBAImageSet::~RGBAImageSet() {
	Clear();
}

/// Remove all images.
void RGBAImageSet::Clear() {
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
	for (ImageMap::iterator it=images.begin(); it != images.end(); ++it) {
		delete it->second;
		it->second = 0;
	}
	images.clear();
	height = -1;
	width = -1;
}

/// Add an image.
void RGBAImageSet::Add(int ident, RGBAImage *image) {
	ImageMap::iterator it=images.find(ident);
	if (it == images.end()) {
		images[ident] = image;
	} else {
		delete it->second;
		it->second = image;
	}
	height = -1;
	width = -1;
}

/// Get image by id.
RGBAImage *RGBAImageSet::Get(int ident) {
	ImageMap::iterator it = images.find(ident);
	if (it != images.end()) {
		return it->second;
	}
	return NULL;
}

/// Give the largest height of the set.
int RGBAImageSet::GetHeight() const {
	if (height < 0) {
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
		for (ImageMap::const_iterator it=images.begin(); it != images.end(); ++it) {
			if (height < it->second->GetHeight()) {
				height = it->second->GetHeight();
			}
		}
	}
	return (height > 0) ? height : 0;
}

/// Give the largest width of the set.
int RGBAImageSet::GetWidth() const {
	if (width < 0) {
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
		for (ImageMap::const_iterator it=images.begin(); it != images.end(); ++it) {
			if (width < it->second->GetWidth()) {
				width = it->second->GetWidth();
			}
		}
	}
	return (width > 0) ? width : 0;
}
