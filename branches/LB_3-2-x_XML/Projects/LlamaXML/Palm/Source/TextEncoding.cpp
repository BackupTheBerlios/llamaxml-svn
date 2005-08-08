/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#include "TextEncoding.h"
#include <string>
#include <PalmOS.h>


namespace LlamaXML {
	
	TextEncoding TextEncoding::System()
	{
#if PALMOS_SDK_VERSION >= 0x600
		LmLocaleType locale;
		return TextEncoding(::LmGetSystemLocale(&locale));
#else
		UInt32 encoding;
		if (::FtrGet(sysFtrCreator, sysFtrNumEncoding, &encoding) == errNone) {
			return TextEncoding(CharEncodingType(encoding));
		}
		else {
			return TextEncoding(charEncodingPalmLatin);
		}
#endif
	}
	
	
		
	TextEncoding TextEncoding::WebCharset(const UnicodeChar * name)
	{
		// Do a crude conversion of Unicode to ASCII
		std::string buffer;
		while (*name) {
			buffer += char(*name++);
		}
		
		if (::StrCaselessCompare(buffer.c_str(), "Windows-1252") == 0) {
			return TextEncoding(charEncodingPalmLatin);
		}
		else if (::StrCaselessCompare(buffer.c_str(), "Shift_JIS") == 0) {
			return TextEncoding(charEncodingPalmSJIS);
		}
		else if (::StrCaselessCompare(buffer.c_str(), "Windows-932") == 0) {
			return TextEncoding(charEncodingPalmSJIS);
		}
		else if (::StrCaselessCompare(buffer.c_str(), "Big5") == 0) {
			return TextEncoding(charEncodingPalmBig5);
		}
		else if (::StrCaselessCompare(buffer.c_str(), "GB2312") == 0) {
			return TextEncoding(charEncodingPalmGB);
		}
		else {
			uint32_t romVersion = 0;
			if ((::FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion) == 0)
				&& (romVersion >=  sysMakeROMVersion(4,0,0,sysROMStageRelease,0))) {
				return TextEncoding(::TxtNameToEncoding(buffer.c_str()));
			}
			else {
				return TextEncoding(charEncodingUnknown);
			}
		}
	}
	
}
