/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

// Scintilla source code edit control
// Encoding: UTF-8
/** @file CaseConvert.cxx
 ** Case fold characters and convert them to upper or lower case.
 ** Tables automatically regenerated by scripts/GenerateCaseConvert.py
 ** Should only be rarely regenerated for new versions of Unicode.
 **/
// Copyright 2013 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <cstring>

#include <vector>
#include <algorithm>

#include "StringCopy.h"
#include "CaseConvert.h"
#include "UniConversion.h"
#include "UnicodeFromUTF8.h"

#ifdef SCI_NAMESPACE
using namespace Scintilla;
#endif

namespace {
	// Use an unnamed namespace to protect the declarations from name conflicts

// Unicode code points are ordered by groups and follow patterns.
// Most characters (pitch==1) are in ranges for a particular alphabet and their
// upper case forms are a fixed distance away.
// Another pattern (pitch==2) is where each lower case letter is preceded by
// the upper case form. These are also grouped into ranges.

int symmetricCaseConversionRanges[] = {
//lower, upper, range length, range pitch
//++Autogenerated -- start of section automatically generated
//**\(\*\n\)
97,65,26,1,
224,192,23,1,
248,216,7,1,
257,256,24,2,
314,313,8,2,
331,330,23,2,
462,461,8,2,
479,478,9,2,
505,504,20,2,
547,546,9,2,
583,582,5,2,
945,913,17,1,
963,931,9,1,
985,984,12,2,
1072,1040,32,1,
1104,1024,16,1,
1121,1120,17,2,
1163,1162,27,2,
1218,1217,7,2,
1233,1232,44,2,
1377,1329,38,1,
7681,7680,75,2,
7841,7840,48,2,
7936,7944,8,1,
7952,7960,6,1,
7968,7976,8,1,
7984,7992,8,1,
8000,8008,6,1,
8032,8040,8,1,
8560,8544,16,1,
9424,9398,26,1,
11312,11264,47,1,
11393,11392,50,2,
11520,4256,38,1,
42561,42560,23,2,
42625,42624,12,2,
42787,42786,7,2,
42803,42802,31,2,
42879,42878,5,2,
42913,42912,5,2,
65345,65313,26,1,
66600,66560,40,1,

//--Autogenerated -- end of section automatically generated
};

// Code points that are symmetric but don't fit into a range of similar characters
// are listed here.

int symmetricCaseConversions[] = {
//lower, upper
//++Autogenerated -- start of section automatically generated
//**1 \(\*\n\)
255,376,
307,306,
309,308,
311,310,
378,377,
380,379,
382,381,
384,579,
387,386,
389,388,
392,391,
396,395,
402,401,
405,502,
409,408,
410,573,
414,544,
417,416,
419,418,
421,420,
424,423,
429,428,
432,431,
436,435,
438,437,
441,440,
445,444,
447,503,
454,452,
457,455,
460,458,
477,398,
499,497,
501,500,
572,571,
575,11390,
576,11391,
578,577,
592,11375,
593,11373,
594,11376,
595,385,
596,390,
598,393,
599,394,
601,399,
603,400,
608,403,
611,404,
613,42893,
614,42922,
616,407,
617,406,
619,11362,
623,412,
625,11374,
626,413,
629,415,
637,11364,
640,422,
643,425,
648,430,
649,580,
650,433,
651,434,
652,581,
658,439,
881,880,
883,882,
887,886,
891,1021,
892,1022,
893,1023,
940,902,
941,904,
942,905,
943,906,
972,908,
973,910,
974,911,
983,975,
1010,1017,
1016,1015,
1019,1018,
1231,1216,
7545,42877,
7549,11363,
8017,8025,
8019,8027,
8021,8029,
8023,8031,
8048,8122,
8049,8123,
8050,8136,
8051,8137,
8052,8138,
8053,8139,
8054,8154,
8055,8155,
8056,8184,
8057,8185,
8058,8170,
8059,8171,
8060,8186,
8061,8187,
8112,8120,
8113,8121,
8144,8152,
8145,8153,
8160,8168,
8161,8169,
8165,8172,
8526,8498,
8580,8579,
11361,11360,
11365,570,
11366,574,
11368,11367,
11370,11369,
11372,11371,
11379,11378,
11382,11381,
11500,11499,
11502,11501,
11507,11506,
11559,4295,
11565,4301,
42874,42873,
42876,42875,
42892,42891,
42897,42896,
42899,42898,

//--Autogenerated -- end of section automatically generated
};

// Characters that have complex case conversions are listed here.
// This includes cases where more than one character is needed for a conversion,
// folding is different to lowering, or (as appropriate) upper(lower(x)) != x or
// lower(upper(x)) != x.

const char *complexCaseConversions =
// Original | Folded | Upper | Lower |
//++Autogenerated -- start of section automatically generated
//**2 \(\*\n\)
"\xc2\xb5|\xce\xbc|\xce\x9c||"
"\xc3\x9f|ss|SS||"
"\xc4\xb0|i\xcc\x87||i\xcc\x87|"
"\xc4\xb1||I||"
"\xc5\x89|\xca\xbcn|\xca\xbcN||"
"\xc5\xbf|s|S||"
"\xc7\x85|\xc7\x86|\xc7\x84|\xc7\x86|"
"\xc7\x88|\xc7\x89|\xc7\x87|\xc7\x89|"
"\xc7\x8b|\xc7\x8c|\xc7\x8a|\xc7\x8c|"
"\xc7\xb0|j\xcc\x8c|J\xcc\x8c||"
"\xc7\xb2|\xc7\xb3|\xc7\xb1|\xc7\xb3|"
"\xcd\x85|\xce\xb9|\xce\x99||"
"\xce\x90|\xce\xb9\xcc\x88\xcc\x81|\xce\x99\xcc\x88\xcc\x81||"
"\xce\xb0|\xcf\x85\xcc\x88\xcc\x81|\xce\xa5\xcc\x88\xcc\x81||"
"\xcf\x82|\xcf\x83|\xce\xa3||"
"\xcf\x90|\xce\xb2|\xce\x92||"
"\xcf\x91|\xce\xb8|\xce\x98||"
"\xcf\x95|\xcf\x86|\xce\xa6||"
"\xcf\x96|\xcf\x80|\xce\xa0||"
"\xcf\xb0|\xce\xba|\xce\x9a||"
"\xcf\xb1|\xcf\x81|\xce\xa1||"
"\xcf\xb4|\xce\xb8||\xce\xb8|"
"\xcf\xb5|\xce\xb5|\xce\x95||"
"\xd6\x87|\xd5\xa5\xd6\x82|\xd4\xb5\xd5\x92||"
"\xe1\xba\x96|h\xcc\xb1|H\xcc\xb1||"
"\xe1\xba\x97|t\xcc\x88|T\xcc\x88||"
"\xe1\xba\x98|w\xcc\x8a|W\xcc\x8a||"
"\xe1\xba\x99|y\xcc\x8a|Y\xcc\x8a||"
"\xe1\xba\x9a|a\xca\xbe|A\xca\xbe||"
"\xe1\xba\x9b|\xe1\xb9\xa1|\xe1\xb9\xa0||"
"\xe1\xba\x9e|ss||\xc3\x9f|"
"\xe1\xbd\x90|\xcf\x85\xcc\x93|\xce\xa5\xcc\x93||"
"\xe1\xbd\x92|\xcf\x85\xcc\x93\xcc\x80|\xce\xa5\xcc\x93\xcc\x80||"
"\xe1\xbd\x94|\xcf\x85\xcc\x93\xcc\x81|\xce\xa5\xcc\x93\xcc\x81||"
"\xe1\xbd\x96|\xcf\x85\xcc\x93\xcd\x82|\xce\xa5\xcc\x93\xcd\x82||"
"\xe1\xbe\x80|\xe1\xbc\x80\xce\xb9|\xe1\xbc\x88\xce\x99||"
"\xe1\xbe\x81|\xe1\xbc\x81\xce\xb9|\xe1\xbc\x89\xce\x99||"
"\xe1\xbe\x82|\xe1\xbc\x82\xce\xb9|\xe1\xbc\x8a\xce\x99||"
"\xe1\xbe\x83|\xe1\xbc\x83\xce\xb9|\xe1\xbc\x8b\xce\x99||"
"\xe1\xbe\x84|\xe1\xbc\x84\xce\xb9|\xe1\xbc\x8c\xce\x99||"
"\xe1\xbe\x85|\xe1\xbc\x85\xce\xb9|\xe1\xbc\x8d\xce\x99||"
"\xe1\xbe\x86|\xe1\xbc\x86\xce\xb9|\xe1\xbc\x8e\xce\x99||"
"\xe1\xbe\x87|\xe1\xbc\x87\xce\xb9|\xe1\xbc\x8f\xce\x99||"
"\xe1\xbe\x88|\xe1\xbc\x80\xce\xb9|\xe1\xbc\x88\xce\x99|\xe1\xbe\x80|"
"\xe1\xbe\x89|\xe1\xbc\x81\xce\xb9|\xe1\xbc\x89\xce\x99|\xe1\xbe\x81|"
"\xe1\xbe\x8a|\xe1\xbc\x82\xce\xb9|\xe1\xbc\x8a\xce\x99|\xe1\xbe\x82|"
"\xe1\xbe\x8b|\xe1\xbc\x83\xce\xb9|\xe1\xbc\x8b\xce\x99|\xe1\xbe\x83|"
"\xe1\xbe\x8c|\xe1\xbc\x84\xce\xb9|\xe1\xbc\x8c\xce\x99|\xe1\xbe\x84|"
"\xe1\xbe\x8d|\xe1\xbc\x85\xce\xb9|\xe1\xbc\x8d\xce\x99|\xe1\xbe\x85|"
"\xe1\xbe\x8e|\xe1\xbc\x86\xce\xb9|\xe1\xbc\x8e\xce\x99|\xe1\xbe\x86|"
"\xe1\xbe\x8f|\xe1\xbc\x87\xce\xb9|\xe1\xbc\x8f\xce\x99|\xe1\xbe\x87|"
"\xe1\xbe\x90|\xe1\xbc\xa0\xce\xb9|\xe1\xbc\xa8\xce\x99||"
"\xe1\xbe\x91|\xe1\xbc\xa1\xce\xb9|\xe1\xbc\xa9\xce\x99||"
"\xe1\xbe\x92|\xe1\xbc\xa2\xce\xb9|\xe1\xbc\xaa\xce\x99||"
"\xe1\xbe\x93|\xe1\xbc\xa3\xce\xb9|\xe1\xbc\xab\xce\x99||"
"\xe1\xbe\x94|\xe1\xbc\xa4\xce\xb9|\xe1\xbc\xac\xce\x99||"
"\xe1\xbe\x95|\xe1\xbc\xa5\xce\xb9|\xe1\xbc\xad\xce\x99||"
"\xe1\xbe\x96|\xe1\xbc\xa6\xce\xb9|\xe1\xbc\xae\xce\x99||"
"\xe1\xbe\x97|\xe1\xbc\xa7\xce\xb9|\xe1\xbc\xaf\xce\x99||"
"\xe1\xbe\x98|\xe1\xbc\xa0\xce\xb9|\xe1\xbc\xa8\xce\x99|\xe1\xbe\x90|"
"\xe1\xbe\x99|\xe1\xbc\xa1\xce\xb9|\xe1\xbc\xa9\xce\x99|\xe1\xbe\x91|"
"\xe1\xbe\x9a|\xe1\xbc\xa2\xce\xb9|\xe1\xbc\xaa\xce\x99|\xe1\xbe\x92|"
"\xe1\xbe\x9b|\xe1\xbc\xa3\xce\xb9|\xe1\xbc\xab\xce\x99|\xe1\xbe\x93|"
"\xe1\xbe\x9c|\xe1\xbc\xa4\xce\xb9|\xe1\xbc\xac\xce\x99|\xe1\xbe\x94|"
"\xe1\xbe\x9d|\xe1\xbc\xa5\xce\xb9|\xe1\xbc\xad\xce\x99|\xe1\xbe\x95|"
"\xe1\xbe\x9e|\xe1\xbc\xa6\xce\xb9|\xe1\xbc\xae\xce\x99|\xe1\xbe\x96|"
"\xe1\xbe\x9f|\xe1\xbc\xa7\xce\xb9|\xe1\xbc\xaf\xce\x99|\xe1\xbe\x97|"
"\xe1\xbe\xa0|\xe1\xbd\xa0\xce\xb9|\xe1\xbd\xa8\xce\x99||"
"\xe1\xbe\xa1|\xe1\xbd\xa1\xce\xb9|\xe1\xbd\xa9\xce\x99||"
"\xe1\xbe\xa2|\xe1\xbd\xa2\xce\xb9|\xe1\xbd\xaa\xce\x99||"
"\xe1\xbe\xa3|\xe1\xbd\xa3\xce\xb9|\xe1\xbd\xab\xce\x99||"
"\xe1\xbe\xa4|\xe1\xbd\xa4\xce\xb9|\xe1\xbd\xac\xce\x99||"
"\xe1\xbe\xa5|\xe1\xbd\xa5\xce\xb9|\xe1\xbd\xad\xce\x99||"
"\xe1\xbe\xa6|\xe1\xbd\xa6\xce\xb9|\xe1\xbd\xae\xce\x99||"
"\xe1\xbe\xa7|\xe1\xbd\xa7\xce\xb9|\xe1\xbd\xaf\xce\x99||"
"\xe1\xbe\xa8|\xe1\xbd\xa0\xce\xb9|\xe1\xbd\xa8\xce\x99|\xe1\xbe\xa0|"
"\xe1\xbe\xa9|\xe1\xbd\xa1\xce\xb9|\xe1\xbd\xa9\xce\x99|\xe1\xbe\xa1|"
"\xe1\xbe\xaa|\xe1\xbd\xa2\xce\xb9|\xe1\xbd\xaa\xce\x99|\xe1\xbe\xa2|"
"\xe1\xbe\xab|\xe1\xbd\xa3\xce\xb9|\xe1\xbd\xab\xce\x99|\xe1\xbe\xa3|"
"\xe1\xbe\xac|\xe1\xbd\xa4\xce\xb9|\xe1\xbd\xac\xce\x99|\xe1\xbe\xa4|"
"\xe1\xbe\xad|\xe1\xbd\xa5\xce\xb9|\xe1\xbd\xad\xce\x99|\xe1\xbe\xa5|"
"\xe1\xbe\xae|\xe1\xbd\xa6\xce\xb9|\xe1\xbd\xae\xce\x99|\xe1\xbe\xa6|"
"\xe1\xbe\xaf|\xe1\xbd\xa7\xce\xb9|\xe1\xbd\xaf\xce\x99|\xe1\xbe\xa7|"
"\xe1\xbe\xb2|\xe1\xbd\xb0\xce\xb9|\xe1\xbe\xba\xce\x99||"
"\xe1\xbe\xb3|\xce\xb1\xce\xb9|\xce\x91\xce\x99||"
"\xe1\xbe\xb4|\xce\xac\xce\xb9|\xce\x86\xce\x99||"
"\xe1\xbe\xb6|\xce\xb1\xcd\x82|\xce\x91\xcd\x82||"
"\xe1\xbe\xb7|\xce\xb1\xcd\x82\xce\xb9|\xce\x91\xcd\x82\xce\x99||"
"\xe1\xbe\xbc|\xce\xb1\xce\xb9|\xce\x91\xce\x99|\xe1\xbe\xb3|"
"\xe1\xbe\xbe|\xce\xb9|\xce\x99||"
"\xe1\xbf\x82|\xe1\xbd\xb4\xce\xb9|\xe1\xbf\x8a\xce\x99||"
"\xe1\xbf\x83|\xce\xb7\xce\xb9|\xce\x97\xce\x99||"
"\xe1\xbf\x84|\xce\xae\xce\xb9|\xce\x89\xce\x99||"
"\xe1\xbf\x86|\xce\xb7\xcd\x82|\xce\x97\xcd\x82||"
"\xe1\xbf\x87|\xce\xb7\xcd\x82\xce\xb9|\xce\x97\xcd\x82\xce\x99||"
"\xe1\xbf\x8c|\xce\xb7\xce\xb9|\xce\x97\xce\x99|\xe1\xbf\x83|"
"\xe1\xbf\x92|\xce\xb9\xcc\x88\xcc\x80|\xce\x99\xcc\x88\xcc\x80||"
"\xe1\xbf\x93|\xce\xb9\xcc\x88\xcc\x81|\xce\x99\xcc\x88\xcc\x81||"
"\xe1\xbf\x96|\xce\xb9\xcd\x82|\xce\x99\xcd\x82||"
"\xe1\xbf\x97|\xce\xb9\xcc\x88\xcd\x82|\xce\x99\xcc\x88\xcd\x82||"
"\xe1\xbf\xa2|\xcf\x85\xcc\x88\xcc\x80|\xce\xa5\xcc\x88\xcc\x80||"
"\xe1\xbf\xa3|\xcf\x85\xcc\x88\xcc\x81|\xce\xa5\xcc\x88\xcc\x81||"
"\xe1\xbf\xa4|\xcf\x81\xcc\x93|\xce\xa1\xcc\x93||"
"\xe1\xbf\xa6|\xcf\x85\xcd\x82|\xce\xa5\xcd\x82||"
"\xe1\xbf\xa7|\xcf\x85\xcc\x88\xcd\x82|\xce\xa5\xcc\x88\xcd\x82||"
"\xe1\xbf\xb2|\xe1\xbd\xbc\xce\xb9|\xe1\xbf\xba\xce\x99||"
"\xe1\xbf\xb3|\xcf\x89\xce\xb9|\xce\xa9\xce\x99||"
"\xe1\xbf\xb4|\xcf\x8e\xce\xb9|\xce\x8f\xce\x99||"
"\xe1\xbf\xb6|\xcf\x89\xcd\x82|\xce\xa9\xcd\x82||"
"\xe1\xbf\xb7|\xcf\x89\xcd\x82\xce\xb9|\xce\xa9\xcd\x82\xce\x99||"
"\xe1\xbf\xbc|\xcf\x89\xce\xb9|\xce\xa9\xce\x99|\xe1\xbf\xb3|"
"\xe2\x84\xa6|\xcf\x89||\xcf\x89|"
"\xe2\x84\xaa|k||k|"
"\xe2\x84\xab|\xc3\xa5||\xc3\xa5|"
"\xef\xac\x80|ff|FF||"
"\xef\xac\x81|fi|FI||"
"\xef\xac\x82|fl|FL||"
"\xef\xac\x83|ffi|FFI||"
"\xef\xac\x84|ffl|FFL||"
"\xef\xac\x85|st|ST||"
"\xef\xac\x86|st|ST||"
"\xef\xac\x93|\xd5\xb4\xd5\xb6|\xd5\x84\xd5\x86||"
"\xef\xac\x94|\xd5\xb4\xd5\xa5|\xd5\x84\xd4\xb5||"
"\xef\xac\x95|\xd5\xb4\xd5\xab|\xd5\x84\xd4\xbb||"
"\xef\xac\x96|\xd5\xbe\xd5\xb6|\xd5\x8e\xd5\x86||"
"\xef\xac\x97|\xd5\xb4\xd5\xad|\xd5\x84\xd4\xbd||"

//--Autogenerated -- end of section automatically generated
;

class CaseConverter : public ICaseConverter {
	// Maximum length of a case conversion result is 6 bytes in UTF-8
	enum { maxConversionLength=6 };
	struct ConversionString {
		char conversion[maxConversionLength+1];
		ConversionString() {
			conversion[0] = '\0';
		}
	};
	// Conversions are initially store in a vector of structs but then decomposed into
	// parallel arrays as that is about 10% faster to search.
	struct CharacterConversion {
		int character;
		ConversionString conversion;
		CharacterConversion(int character_=0, const char *conversion_="") : character(character_) {
			StringCopy(conversion.conversion, conversion_);
		}
		bool operator<(const CharacterConversion &other) const {
			return character < other.character;
		}
	};
	typedef std::vector<CharacterConversion> CharacterToConversion;
	CharacterToConversion characterToConversion;
	// The parallel arrays
	std::vector<int> characters;
	std::vector<ConversionString> conversions;

public:
	CaseConverter() {
	}
	bool Initialised() const {
		return characters.size() > 0;
	}
	void Add(int character, const char *conversion) {
		characterToConversion.push_back(CharacterConversion(character, conversion));
	}
	const char *Find(int character) {
		const std::vector<int>::iterator it = std::lower_bound(characters.begin(), characters.end(), character);
		if (it == characters.end())
			return 0;
		else if (*it == character)
			return conversions[it - characters.begin()].conversion;
		else
			return 0;
	}
	size_t CaseConvertString(char *converted, size_t sizeConverted, const char *mixed, size_t lenMixed) {
		size_t lenConverted = 0;
		size_t mixedPos = 0;
		unsigned char bytes[UTF8MaxBytes + 1];
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma unroll
#   pragma swp
#endif
		while (mixedPos < lenMixed) {
			const unsigned char leadByte = static_cast<unsigned char>(mixed[mixedPos]);
			const char *caseConverted = 0;
			size_t lenMixedChar = 1;
			if (UTF8IsAscii(leadByte)) {
				caseConverted = Find(leadByte);
			} else {
				bytes[0] = leadByte;
				const int widthCharBytes = UTF8BytesOfLead[leadByte];
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma unroll
#   pragma swp
#endif
				for (int b=1; b<widthCharBytes; b++) {
					bytes[b] = (mixedPos+b < lenMixed) ? mixed[mixedPos+b] : 0;
				}
				int classified = UTF8Classify(bytes, widthCharBytes);
				if (!(classified & UTF8MaskInvalid)) {
					// valid UTF-8
					lenMixedChar = classified & UTF8MaskWidth;
					int character = UnicodeFromUTF8(bytes);
					caseConverted = Find(character);
				}
			}
			if (caseConverted) {
				// Character has a conversion so copy that conversion in
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma unroll
#   pragma swp
#endif
				while (*caseConverted) {
					converted[lenConverted++] = *caseConverted++;
					if (lenConverted >= sizeConverted)
						return 0;
				}
			} else {
				// Character has no conversion so copy the input to output
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma unroll
#   pragma swp
#endif
				for (size_t i=0; i<lenMixedChar; i++) {
					converted[lenConverted++] = mixed[mixedPos+i];
					if (lenConverted >= sizeConverted)
						return 0;
				}
			}
			mixedPos += lenMixedChar;
		}
		return lenConverted;
	}
	void FinishedAdding() {
		std::sort(characterToConversion.begin(), characterToConversion.end());
		characters.reserve(characterToConversion.size());
		conversions.reserve(characterToConversion.size());
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma unroll
#   pragma swp
#endif
		for (CharacterToConversion::iterator it = characterToConversion.begin(); it != characterToConversion.end(); ++it) {
			characters.push_back(it->character);
			conversions.push_back(it->conversion);
		}
		// Empty the original calculated data completely
		CharacterToConversion().swap(characterToConversion);
	}
};

CaseConverter caseConvFold;
CaseConverter caseConvUp;
CaseConverter caseConvLow;

void UTF8FromUTF32Character(int uch, char *putf) {
	size_t k = 0;
	if (uch < 0x80) {
		putf[k++] = static_cast<char>(uch);
	} else if (uch < 0x800) {
		putf[k++] = static_cast<char>(0xC0 | (uch >> 6));
		putf[k++] = static_cast<char>(0x80 | (uch & 0x3f));
	} else if (uch < 0x10000) {
		putf[k++] = static_cast<char>(0xE0 | (uch >> 12));
		putf[k++] = static_cast<char>(0x80 | ((uch >> 6) & 0x3f));
		putf[k++] = static_cast<char>(0x80 | (uch & 0x3f));
	} else {
		putf[k++] = static_cast<char>(0xF0 | (uch >> 18));
		putf[k++] = static_cast<char>(0x80 | ((uch >> 12) & 0x3f));
		putf[k++] = static_cast<char>(0x80 | ((uch >> 6) & 0x3f));
		putf[k++] = static_cast<char>(0x80 | (uch & 0x3f));
	}
	putf[k] = 0;
}

void AddSymmetric(enum CaseConversion conversion, int lower,int upper) {
	char lowerUTF8[UTF8MaxBytes+1];
	UTF8FromUTF32Character(lower, lowerUTF8);
	char upperUTF8[UTF8MaxBytes+1];
	UTF8FromUTF32Character(upper, upperUTF8);

	switch (conversion) {
	case CaseConversionFold:
		caseConvFold.Add(upper, lowerUTF8);
		break;
	case CaseConversionUpper:
		caseConvUp.Add(lower, upperUTF8);
		break;
	case CaseConversionLower:
		caseConvLow.Add(upper, lowerUTF8);
		break;
	}
}

void SetupConversions(enum CaseConversion conversion) {
	// First initialize for the symmetric ranges
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma unroll
#   pragma swp
#endif
	for (size_t i=0; i<ELEMENTS(symmetricCaseConversionRanges);) {
		int lower = symmetricCaseConversionRanges[i++];
		int upper = symmetricCaseConversionRanges[i++];
		int length = symmetricCaseConversionRanges[i++];
		int pitch = symmetricCaseConversionRanges[i++];
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma unroll
#   pragma swp
#endif
		for (int j=0; j<length*pitch; j+=pitch) {
			AddSymmetric(conversion, lower+j, upper+j);
		}
	}
	// Add the symmetric singletons
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma unroll
#   pragma swp
#endif
	for (size_t i=0; i<ELEMENTS(symmetricCaseConversions);) {
		int lower = symmetricCaseConversions[i++];
		int upper = symmetricCaseConversions[i++];
		AddSymmetric(conversion, lower, upper);
	}
	// Add the complex cases
	const char *sComplex = complexCaseConversions;
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma unroll
#   pragma swp
#endif
	while (*sComplex) {
		// Longest ligature is 3 character so 5 for safety
		const size_t lenUTF8 = 5*UTF8MaxBytes+1;
		char originUTF8[lenUTF8];
		char foldedUTF8[lenUTF8];
		char lowerUTF8[lenUTF8];
		char upperUTF8[lenUTF8];
		size_t i = 0;
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma unroll
#   pragma swp
#endif
		while (*sComplex && *sComplex != '|') {
			originUTF8[i++] = *sComplex;
			sComplex++;
		}
		sComplex++;
		originUTF8[i] = 0;
		i = 0;
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma unroll
#   pragma swp
#endif
		while (*sComplex && *sComplex != '|') {
			foldedUTF8[i++] = *sComplex;
			sComplex++;
		}
		sComplex++;
		foldedUTF8[i] = 0;
		i = 0;
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma unroll
#   pragma swp
#endif
		while (*sComplex && *sComplex != '|') {
			upperUTF8[i++] = *sComplex;
			sComplex++;
		}
		sComplex++;
		upperUTF8[i] = 0;
		i = 0;
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma unroll
#   pragma swp
#endif
		while (*sComplex && *sComplex != '|') {
			lowerUTF8[i++] = *sComplex;
			sComplex++;
		}
		sComplex++;
		lowerUTF8[i] = 0;

		int character = UnicodeFromUTF8(reinterpret_cast<unsigned char *>(originUTF8));

		if (conversion == CaseConversionFold && foldedUTF8[0]) {
			caseConvFold.Add(character, foldedUTF8);
		}

		if (conversion == CaseConversionUpper && upperUTF8[0]) {
			caseConvUp.Add(character, upperUTF8);
		}

		if (conversion == CaseConversionLower && lowerUTF8[0]) {
			caseConvLow.Add(character, lowerUTF8);
		}
	}

	switch (conversion) {
	case CaseConversionFold:
		caseConvFold.FinishedAdding();
		break;
	case CaseConversionUpper:
		caseConvUp.FinishedAdding();
		break;
	case CaseConversionLower:
		caseConvLow.FinishedAdding();
		break;
	}
}

CaseConverter *ConverterForConversion(enum CaseConversion conversion) {
	switch (conversion) {
	case CaseConversionFold:
		return &caseConvFold;
	case CaseConversionUpper:
		return &caseConvUp;
	case CaseConversionLower:
		return &caseConvLow;
	}
	return 0;
}

}

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif

ICaseConverter *ConverterFor(enum CaseConversion conversion) {
	CaseConverter *pCaseConv = ConverterForConversion(conversion);
	if (!pCaseConv->Initialised())
		SetupConversions(conversion);
	return pCaseConv;
}

const char *CaseConvert(int character, enum CaseConversion conversion) {
	CaseConverter *pCaseConv = ConverterForConversion(conversion);
	if (!pCaseConv->Initialised())
		SetupConversions(conversion);
	return pCaseConv->Find(character);
}

size_t CaseConvertString(char *converted, size_t sizeConverted, const char *mixed, size_t lenMixed, enum CaseConversion conversion) {
	CaseConverter *pCaseConv = ConverterForConversion(conversion);
	if (!pCaseConv->Initialised())
		SetupConversions(conversion);
	return pCaseConv->CaseConvertString(converted, sizeConverted, mixed, lenMixed);
}

#ifdef SCI_NAMESPACE
}
#endif
