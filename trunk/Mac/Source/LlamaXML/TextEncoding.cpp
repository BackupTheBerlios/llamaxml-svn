/*
 * Copyright (C) 2005 Llamagraphics, Inc.
 * 
 * This file is part of LlamaXML.
 *
 * LlamaXML is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * LlamaXML is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
 * USA.
 *
 * LlamaXML is also available under an alternative commercial license for
 * developers who need the flexibility to include this library in proprietary
 * applications.  Visit http://www.llamagraphics.com/LlamaXML/ for more 
 * information.
 */

#include "LlamaXML/TextEncoding.h"
#include "LlamaXML/XMLException.h"
#include <ctype.h>


#if UNIVERSAL_INTERFACES_VERSION < 0x0400
	enum {
	  kISOLatinHebrewImplicitOrderVariant = 0,
	  kISOLatinHebrewVisualOrderVariant = 1,
	  kISOLatinHebrewExplicitOrderVariant = 2
	};
	
	/* Variants of kTextEncodingWindowsLatin1*/
	enum {
	  kWindowsLatin1StandardVariant = 0,
	  kWindowsLatin1PalmVariant     = 1     /* PalmSource variant of cp1252*/
	};

	/* Variants of kTextEncodingDOSJapanese*/
	enum {
	  kDOSJapaneseStandardVariant   = 0,
	  kDOSJapanesePalmVariant       = 1     /* PalmSource variant of cp932*/
	};

	/* Variants of kTextEncodingISOLatin1*/
	enum {
	  kISOLatin1StandardVariant     = 0,
	  kISOLatin1MusicCDVariant      = 1
	};

	/*
	   Variants of ShiftJIS
	   The DOSVariant is like kTextEncodingDOSJapanese, but with the
	   basic ShiftJIS part mapped as per kTextEncodingShiftJIS.
	*/
	enum {
	  kShiftJIS_BasicVariant        = 0,
	  kShiftJIS_DOSVariant          = 1,
	  kShiftJIS_MusicCDVariant      = 2     /* MusicShiftJIS, per RIS-506 (RIAJ)*/
	};

	enum {
	  kUnicodeUTF16Format           = 0,    /* UTF16 form (16-bit units), native or external byte order (see below)*/
	  kUnicodeUTF32Format           = 3,    /* UTF32 form (32-bit units), native or external byte order (see below)*/
	  kUnicodeUTF16BEFormat         = 4,    /* UTF16 form, explicit big-endian byte order, no BOM*/
	  kUnicodeUTF16LEFormat         = 5,    /* UTF16 form, explicit little-endian byte order, no BOM*/
	  kUnicodeUTF32BEFormat         = 6,    /* UTF32 form, explicit big-endian byte order, no BOM*/
	  kUnicodeUTF32LEFormat         = 7,    /* UTF32 form, explicit little-endian byte order, no BOM*/
	  kUnicodeSCSUFormat            = 8     /* Std. Compression Scheme for Unicode, Unicode Tech Std. #6*/
	                                        /* Note for kUnicodeUTF16Format and kUnicodeUTF32Format:*/
	                                        /* - An array of UTF16Char (UniChar) or UTF32Char is normally understood to use "internal" or*/
	                                        /* platform-native byte ordering for kUnicodeUTF16Format and kUnicodeUTF32Format; the array MAY*/
	                                        /* begin with byte-order mark (BOM), but the BOM should match the internal ordering.*/
	                                        /* - If an array of bytes (such as char *) that can be in various encodings is specified to be*/
	                                        /* in Unicode with kUnicodeUTF16Format or kUnicodeUTF32Format (not explicitly BE or LE), then it*/
	                                        /* is assumed to use "external" byte ordering, which means: If there is a BOM at the beginning*/
	                                        /* of text, the BOM specifies the byte ordering, otherwise big-endian is assumed.*/
	};

	/* Variants of kTextEncodingMacGreek*/
	enum {
	  kMacGreekDefaultVariant       = 0,    /* meta value, maps to 1 or 2 depending on System*/
	  kMacGreekNoEuroSignVariant    = 1,    /* Mac OS version < 9.2.2, 0x9C is SOFT HYPHEN, 0xFF is undefined*/
	  kMacGreekEuroSignVariant      = 2     /* Mac OS version >= 9.2.2, 0x9C is EURO SIGN, 0xFF is SOFT HYPHEN*/
	};

	/* Misc standards begin at 0xA00*/
	enum {
	  kTextEncodingVISCII           = 0x0A07, /* RFC 1456, Vietnamese*/
	  kTextEncodingKOI8_U           = 0x0A08, /* RFC 2319, Ukrainian*/
	  kTextEncodingBig5_E           = 0x0A09 /* Taiwan Big-5E standard*/
	};
#endif


namespace LlamaXML {

	TextEncoding::TextEncoding()
	: mEncoding(::CreateTextEncoding(kTextEncodingUnknown, kTextEncodingDefaultVariant, kTextEncodingDefaultFormat))
	{
	}
	
	
	TextEncoding::TextEncoding(TextEncodingBase base, TextEncodingVariant variant,
			TextEncodingFormat format)
	: mEncoding(::CreateTextEncoding(base, variant, format))
	{
	}
	

#if 0
	String TextEncoding::Name() const
	{
		if (mEncoding == ::CreateTextEncoding(kTextEncodingMacUkrainian, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat)) {
			return String::Localized("Ukrainian (Mac OS)", "display name of kTextEncodingMacUkrainian");
		}
		else {
			UniChar encodingName[64];
			ByteCount nameLengthInBytes = 0;
			RegionCode actualRegion;
			::TextEncoding actualEncoding;
			
			ThrowIfError(::GetTextEncodingName(mEncoding, kTextEncodingFullName,
				kTextRegionDontCare, 
				::CreateTextEncoding(kTextEncodingUnicodeDefault, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat),
				sizeof(encodingName), &nameLengthInBytes,
				&actualRegion, &actualEncoding,
				reinterpret_cast<TextPtr>(encodingName)));
			
			return String(encodingName, nameLengthInBytes / 2);
		}
	}
#endif
	
	bool TextEncoding::IsAvailable() const
	{
		// We're assuming here that if an encoding is convertable to anything,
		// it's convertable to Unicode.
	#if TARGET_API_MAC_CARBON
		ItemCount numberOfConversions = 0;
		
		OSStatus status = ::TECCountDestinationTextEncodings(mEncoding, &numberOfConversions);
		return (status == noErr) && (numberOfConversions > 0);
	#else
		TextToUnicodeInfo info;
		
		if (::CreateTextToUnicodeInfoByEncoding(mEncoding, &info) == noErr) {
			::DisposeTextToUnicodeInfo(&info);
			return true;
		}
		else return false;
	#endif
	}
	
	
	TextEncoding TextEncoding::System()
	{
#if TARGET_API_MAC_CARBON
		return TextEncoding(::CFStringGetSystemEncoding());
#else
		::TextEncoding encoding;
		ThrowIfError(::UpgradeScriptInfoToTextEncoding(smSystemScript, 
			kTextLanguageDontCare, kTextRegionDontCare, nil, &encoding));
		return TextEncoding(encoding);
#endif
	}
	
	
	TextEncoding TextEncoding::Application()
	{
#if TARGET_API_MAC_CARBON
		return TextEncoding(::GetApplicationTextEncoding());
#else
		::TextEncoding encoding;
		ThrowIfError(::UpgradeScriptInfoToTextEncoding(smSystemScript, 
			kTextLanguageDontCare, kTextRegionDontCare, nil, &encoding));
		return TextEncoding(encoding);
#endif
	}
	
	
	TextEncoding TextEncoding::WindowsLatin1()
	{
		return Fallback(
			TextEncoding(kTextEncodingWindowsLatin1, kWindowsLatin1StandardVariant,
				kTextEncodingDefaultFormat),
			ISOLatin1());
	}
	
	
	TextEncoding TextEncoding::ISOLatin1()
	{
		return TextEncoding(kTextEncodingISOLatin1, kISOLatin1StandardVariant,
			kTextEncodingDefaultFormat);
	}
	
	
	TextEncoding TextEncoding::PalmLatin1()
	{
		return Fallback(
			TextEncoding(kTextEncodingWindowsLatin1, kWindowsLatin1PalmVariant,
				kTextEncodingDefaultFormat),
			WindowsLatin1());
	}
	
	
	TextEncoding TextEncoding::ShiftJIS()
	{
		return TextEncoding(kTextEncodingShiftJIS, kShiftJIS_BasicVariant,
			kTextEncodingDefaultFormat);
	}
	
	
	TextEncoding TextEncoding::WindowsShiftJIS()
	{
		return Fallback(
			TextEncoding(kTextEncodingShiftJIS, kShiftJIS_DOSVariant,
				kTextEncodingDefaultFormat),
			ShiftJIS());
	}
	
	
	TextEncoding TextEncoding::PalmShiftJIS()
	{
		return Fallback(
			TextEncoding(kTextEncodingDOSJapanese, kDOSJapanesePalmVariant,
				kTextEncodingDefaultFormat),
			WindowsShiftJIS());
	}
	
	
	TextEncoding TextEncoding::ASCII()
	{
		return TextEncoding(kTextEncodingUS_ASCII, kTextEncodingDefaultVariant,
			kTextEncodingDefaultFormat);
	}
	
	
	TextEncoding TextEncoding::UCS2()
	{
		// UCS2 is essentially UTF16 without surrogate pairs.
		// Apple doesn't seem to define this subset explicitly, so just
		// use UTF16.
		return TextEncoding(kTextEncodingUnicodeDefault, kUnicodeNoSubset,
			kUnicodeUTF16Format);
	}
	
	
	TextEncoding TextEncoding::UTF7()
	{
		return TextEncoding(kTextEncodingUnicodeDefault, kUnicodeNoSubset,
			kUnicodeUTF7Format);
	}
	
	
	TextEncoding TextEncoding::UTF8()
	{
		return TextEncoding(kTextEncodingUnicodeDefault, kUnicodeNoSubset,
			kUnicodeUTF8Format);
	}
	
	
	TextEncoding TextEncoding::UTF16()
	{
		return TextEncoding(kTextEncodingUnicodeDefault, kUnicodeNoSubset,
			kUnicodeUTF16Format);
	}
	
	
	TextEncoding TextEncoding::UTF16BE()
	{
		return TextEncoding(kTextEncodingUnicodeDefault, kUnicodeNoSubset,
			kUnicodeUTF16BEFormat);
	}
	
	
	TextEncoding TextEncoding::UTF16LE()
	{
		return TextEncoding(kTextEncodingUnicodeDefault, kUnicodeNoSubset,
			kUnicodeUTF16LEFormat);
	}
	
	
	TextEncoding TextEncoding::UTF32()
	{
		return TextEncoding(kTextEncodingUnicodeDefault, kUnicodeNoSubset,
			kUnicodeUTF32Format);
	}
	
	
	TextEncoding TextEncoding::UTF32BE()
	{
		return TextEncoding(kTextEncodingUnicodeDefault, kUnicodeNoSubset,
			kUnicodeUTF32BEFormat);
	}
	
	
	TextEncoding TextEncoding::UTF32LE()
	{
		return TextEncoding(kTextEncodingUnicodeDefault, kUnicodeNoSubset,
			kUnicodeUTF32LEFormat);
	}
		
	
	TextEncoding TextEncoding::Macintosh(::TextEncoding encoding)
	{
		return TextEncoding(encoding);
	}
	
	
	bool TextEncoding::EqualIgnoringCase(const UnicodeChar * a, const char * b) {
		while (*a && *b) {
			if (*a > 127) return false;
			if (tolower(char(*a++)) != tolower(char(*b++))) return false;
		}
		return (*a == 0) && (*b == 0);
	}
	
	
	TextEncoding TextEncoding::WebCharset(const UnicodeChar * name) {
		if (EqualIgnoringCase(name, "ISO-8859-1")) {
			// For some reason, Apple thinks that "ISO-8859-1"" refers to kTextEncodingWindowsLatin1
			// rather than kTextEncodingISOLatin1.
			return ISOLatin1();
		}
		else if (EqualIgnoringCase(name, "Shift_JIS")) {
			// For some reason, Apple thinks that "Shift_JIS" refers to kShiftJIS_DOSVariant
			// rather than kShiftJIS_BasicVariant.
			return ShiftJIS();
		}
		else {
			// Quick conversion from Unicode to ASCII
			Str255 pascalName;
			pascalName[0] = 0;
			while (*name && (pascalName[0] < 255)) {
				pascalName[++pascalName[0]] = uint8_t(*name++);
			}
			::TextEncoding macEncoding;
			ThrowIfXMLError(::TECGetTextEncodingFromInternetName(&macEncoding, pascalName));
			return TextEncoding(macEncoding);
		}
	}
	
	
	TextEncoding TextEncoding::Fallback(TextEncoding a, TextEncoding b)
	{
		if (a.IsAvailable()) return a;
		else if (b.IsAvailable()) return b;
		else return a;
	}
	
	
	TextEncoding TextEncoding::Fallback(TextEncoding a, TextEncoding b, TextEncoding c)
	{
		if (a.IsAvailable()) return a;
		else if (b.IsAvailable()) return b;
		else if (c.IsAvailable()) return c;
		else return a;
	}
	
	
	TextEncoding TextEncoding::Fallback(TextEncoding a, TextEncoding b, TextEncoding c,
			TextEncoding d)
	{
		if (a.IsAvailable()) return a;
		else if (b.IsAvailable()) return b;
		else if (c.IsAvailable()) return c;
		else if (d.IsAvailable()) return d;
		else return a;
	}
	

#if 0
	TextEncoding TextEncoding::Palm(UInt8 encoding)
	{
		switch (encoding) {
			// Latin Palm OS character encoding, and subsets.
			// PalmOS variant of CP1252, with 10 extra Greek characters
			case charEncodingPalmGSM:
				ThrowError(kTextUnsupportedEncodingErr);
			// PalmOS version of CP1252
			case charEncodingPalmLatin:
				return PalmLatin1();
			// Windows variant of 8859-1
			case charEncodingCP1252:
				return WindowsLatin1();
			// ISO 8859 Part 1
			case charEncodingISO8859_1:
				return ISOLatin1();
			// ISO 646-1991
			case charEncodingAscii:
				return ASCII();

			// Japanese Palm OS character encoding, and subsets.
			// PalmOS version of CP932
			case charEncodingPalmSJIS:
				return PalmShiftJIS();
			// Windows variant of ShiftJIS
			case charEncodingCP932:
				return WindowsShiftJIS();
			// Encoding for JIS 0208-1990 + 1-byte katakana
			case charEncodingShiftJIS:
				return ShiftJIS();

			// Unicode character encodings
			// UCS2 is UTF16 minus the surrogate pairs
			case charEncodingUCS2:
				return UCS2();
			case charEncodingUTF8:
				return UTF8();
			case charEncodingUTF7:
				return UTF7();
			case charEncodingUTF16:
				return UTF16();
			case charEncodingUTF16BE:
				return UTF16BE();
			case charEncodingUTF16LE:
				return UTF16LE();
			case charEncodingUTF32:
				return UTF32();
			case charEncodingUTF32BE:
				return UTF32BE();
			case charEncodingUTF32LE:
				return UTF32LE();
			// A variant of UTF7 used by the IMAP mail protocol to encode
			// mail folder names.  Documented in RFC 2060.  This does not
			// seem to be supported by the Macintosh text encoding converter.
			case charEncodingUTF7_IMAP:
				ThrowError(kTextUnsupportedEncodingErr);
			// I really don't know how this is different from UTF32...
			case charEncodingUCS4:
				return UTF32();

			// Latin character encodings
			case charEncodingCP850:
				return TextEncoding(kTextEncodingDOSLatin1, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingCP437:
				return TextEncoding(kTextEncodingDOSLatinUS, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingCP865:
				return TextEncoding(kTextEncodingDOSNordic, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingCP860:
				return TextEncoding(kTextEncodingDOSPortuguese, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingCP861:
				return TextEncoding(kTextEncodingDOSIcelandic, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingCP863:
				return TextEncoding(kTextEncodingDOSCanadianFrench, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingCP775:
				return TextEncoding(kTextEncodingDOSBalticRim, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingMacIslande:
				return TextEncoding(kTextEncodingMacIcelandic, kMacIcelandicStdDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingMacintosh:
				return TextEncoding(kTextEncodingMacRoman, kMacRomanDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingCP1257:
				return TextEncoding(kTextEncodingWindowsBalticRim, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingISO8859_3:
				return TextEncoding(kTextEncodingISOLatin3, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingISO8859_4:
				return TextEncoding(kTextEncodingISOLatin4, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);

			// Extended Latin character encodings
			case charEncodingISO8859_2:
				return TextEncoding(kTextEncodingISOLatin2, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingCP1250:
				return TextEncoding(kTextEncodingWindowsLatin2, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingCP852:
				return TextEncoding(kTextEncodingDOSLatin2, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			// This doesn't seem to be implemented on the Mac, though there's
			// a description of it at http://www.kiv.zcu.cz/~luki/cs/cs-encodings-faq.html
			case charEncodingXKamenicky:
				ThrowError(kTextUnsupportedEncodingErr);
			case charEncodingMacXCroate:
				return TextEncoding(kTextEncodingMacCroatian, kMacCroatianDefaultVariant,
					kTextEncodingDefaultFormat);
			// I just can't figure out which encoding this refers to.
			// It seems like Mac OS X Latin-2, but I don't know what that is.
			case charEncodingMacXLat2:
				ThrowError(kTextUnsupportedEncodingErr);
			case charEncodingMacXRomania:
				return TextEncoding(kTextEncodingMacRomanian, kMacRomanianDefaultVariant,
					kTextEncodingDefaultFormat);
			// I think this must be the character set used by GSM phones in Europe.
			// http://www.csoft.co.uk/character_sets/gsm.htm
			case charEncodingGSM:
				ThrowError(kTextUnsupportedEncodingErr);

			// Japanese character encodings
			case charEncodingEucJp:
				return TextEncoding(kTextEncodingEUC_JP, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingISO2022Jp:
				return TextEncoding(kTextEncodingISO_2022_JP, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			// This isn't so much an encoding as a heuristic...
			case charEncodingXAutoJp:
				ThrowError(kTextUnsupportedEncodingErr);

			// Greek character encodings
			case charEncodingISO8859_7:
				return TextEncoding(kTextEncodingISOLatinGreek, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingCP1253:
				return TextEncoding(kTextEncodingWindowsGreek, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingCP869:
				return TextEncoding(kTextEncodingDOSGreek2, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingCP737:
				return TextEncoding(kTextEncodingDOSGreek, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingMacXGr:
				return TextEncoding(kTextEncodingMacGreek, kMacGreekDefaultVariant,
					kTextEncodingDefaultFormat);

			// Cyrillic character encodings
			case charEncodingCP1251:
				return TextEncoding(kTextEncodingWindowsCyrillic, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingISO8859_5:
				return TextEncoding(kTextEncodingISOLatinCyrillic, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingKoi8R:
				return TextEncoding(kTextEncodingKOI8_R, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			// I'm just guessing by elimination that they mean RFC 2319, Ukrainian here.
			case charEncodingKoi8:
				return TextEncoding(kTextEncodingKOI8_U, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingCP855:
				return TextEncoding(kTextEncodingDOSCyrillic, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingCP866:
				return TextEncoding(kTextEncodingDOSRussian, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingMacCyr:
				return TextEncoding(kTextEncodingMacCyrillic, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingMacUkraine:
				return TextEncoding(kTextEncodingMacUkrainian, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);

			// Turkish character encodings
			case charEncodingCP1254:
				return TextEncoding(kTextEncodingWindowsLatin5, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingISO8859_9:
				return TextEncoding(kTextEncodingISOLatin5, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingCP857:
				return TextEncoding(kTextEncodingDOSTurkish, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingMacTurc:
				return TextEncoding(kTextEncodingMacTurkish, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			// This seems to be DOS Latin 3, but it's not listed in the Mac tables.
			case charEncodingCP853:
				ThrowError(kTextUnsupportedEncodingErr);

			// Arabic character encodings
			// The Mac tables don't make any distinction between ISO 8859-6, Asmo 708,
			// and DOS 708.  I hope it's OK to merge them...
			case charEncodingISO8859_6:
			case charEncodingAsmo708:
			case charEncodingAsmo708Plus:
			case charEncodingAsmo708Fr:
				return TextEncoding(kTextEncodingISOLatinArabic, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingCP1256:
				return TextEncoding(kTextEncodingWindowsArabic, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingCP864:
				return TextEncoding(kTextEncodingDOSArabic, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingMacAra:
				return TextEncoding(kTextEncodingMacArabic, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);

			// Simplified Chinese character encodings
			case charEncodingGB2312:
				return TextEncoding(kTextEncodingGB_2312_80, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingHZ:
				return TextEncoding(kTextEncodingHZ_GB_2312, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingGBK:
				return TextEncoding(kTextEncodingGBK_95, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			// I'm guessing that this is a Palm variation on GB 2312...
			case charEncodingPalmGB:
				return TextEncoding(kTextEncodingGB_2312_80, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);

			// Traditional Chinese character encodings
			case charEncodingBig5:
				return TextEncoding(kTextEncodingBig5, kBig5_BasicVariant,
					kTextEncodingDefaultFormat);
			case charEncodingBig5_HKSCS:
				return TextEncoding(kTextEncodingBig5_HKSCS_1999, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			// I'm just guessing that by "Plus" Palm means the ETenVariant that Apple
			// says "adds kana, Cyrillic, radicals, etc with hi bytes C6-C8,F9"
			case charEncodingBig5Plus:
				return TextEncoding(kTextEncodingBig5, kBig5_ETenVariant,
					kTextEncodingDefaultFormat);
			// No clue what Palm changed, so just use Big-5.
			case charEncodingPalmBig5:
				return TextEncoding(kTextEncodingBig5, kBig5_BasicVariant,
					kTextEncodingDefaultFormat);
			case charEncodingISO2022CN:
				return TextEncoding(kTextEncodingISO_2022_CN, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);

			// Vietnamese character encodings
			case charEncodingViscii:
				return TextEncoding(kTextEncodingVISCII, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			// VIQR stands for VIetnamese Quoted-Readable,
			// which is a convention to write Vietnamese letters using 7-bit ASCII.
			// http://www.nonsong.org/viqr.html
			case charEncodingViqr:
				ThrowError(kTextUnsupportedEncodingErr);
			// Also known as VPS
			// http://homepages.cwi.nl/~dik/english/codes/stand.html#vncii
			case charEncodingVncii:
				ThrowError(kTextUnsupportedEncodingErr);
			// Several web site seem to think Vietnet is the same as VIQR
			case charEncodingVietnet:
				ThrowError(kTextUnsupportedEncodingErr);
			case charEncodingCP1258:
				return TextEncoding(kTextEncodingWindowsVietnamese, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);

			// Korean character encodings
			case charEncodingEucKr:
				return TextEncoding(kTextEncodingEUC_KR, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingCP949:
				return TextEncoding(kTextEncodingDOSKorean, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			case charEncodingISO2022Kr:
				return TextEncoding(kTextEncodingISO_2022_KR, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);

			// Hebrew character encodings
			case charEncodingISO8859_8I:
				return TextEncoding(kTextEncodingISOLatinHebrew, kISOLatinHebrewImplicitOrderVariant,
					kTextEncodingDefaultFormat);
			// Comments in the Mac TextCommon.h make it clear that the "default"
			// variant of ISO 8859-8 is visual ordering.
			case charEncodingISO8859_8:
				return TextEncoding(kTextEncodingISOLatinHebrew, kISOLatinHebrewVisualOrderVariant,
					kTextEncodingDefaultFormat);
			case charEncodingCP1255:
				return TextEncoding(kTextEncodingWindowsHebrew, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			// There's no explicit visual variant for cp1255, so I'm hoping that
			// using the default variant is better than throwing an exception...
			case charEncodingCP1255V:
				return TextEncoding(kTextEncodingWindowsHebrew, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);

			// Thai character encodings
			// TIS-620, http://www.langbox.com/codeset/tis620.html
			case charEncodingTis620:
				ThrowError(kTextUnsupportedEncodingErr);
			case charEncodingCP874:
				return TextEncoding(kTextEncodingDOSThai, kTextEncodingDefaultVariant,
					kTextEncodingDefaultFormat);
			
			default:
				ThrowError(kTextUnsupportedEncodingErr);
		}
		
		// not reached
		return TextEncoding();
	}
#endif
	

#if LLAMA_UNIT_TESTS && 0
	static void TestTextEncoding()
	{
		{
			TextEncoding windowsLatin1 = TextEncoding::WindowsLatin1();
			TestBlock block(DebugFormat("Testing ^") << windowsLatin1.Name());
			TestAssert(windowsLatin1.Name() == String::Constant("Western (Windows Latin 1)"));
			TestAssert(windowsLatin1.IsAvailable());
		}
		
		{
			TextEncoding palmLatin1 = TextEncoding::PalmLatin1();
			TestBlock block(DebugFormat("Testing ^") << palmLatin1.Name());
			// TestAssert(palmLatin1.Name() == String::Constant("Western (Palm Latin 1)"));
			TestAssert(palmLatin1.IsAvailable());
			TestAssert(String("abc", palmLatin1.AsMacTextEncoding()) == String::Constant("abc"));
		}
	}
	
	static UnitTest::Register gRegisterTextEncodingTest("TextEncoding", TestTextEncoding);
#endif

}
