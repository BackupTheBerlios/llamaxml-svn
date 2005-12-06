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
#include <string>
#include <PalmOS.h>


namespace LlamaXML {

#if PALMOS_SDK_VERSION >= 0x600
	typedef size_t TextSize;
#else
	typedef UInt16 TextSize;
#endif
	
	TextEncoding TextEncoding::WindowsLatin1() {
		TextEncoding encoding = TextEncoding(charEncodingCP1252);
		if (encoding.IsAvailable()) return encoding;
		return TextEncoding(charEncodingPalmLatin);
	}
	
	
	TextEncoding TextEncoding::ISOLatin1() {
		TextEncoding encoding = TextEncoding(charEncodingISO8859_1);
		if (encoding.IsAvailable()) return encoding;
		encoding = TextEncoding(charEncodingCP1252);
		if (encoding.IsAvailable()) return encoding;
		return TextEncoding(charEncodingPalmLatin);
	}
	
	
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
			return WindowsLatin1();
		}
		else if (::StrCaselessCompare(buffer.c_str(), "Shift_JIS") == 0) {
			return ShiftJIS();
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
		else if (::StrCaselessCompare(buffer.c_str(), "ISO-8859-1") == 0) {
			return ISOLatin1();
		}
		else if (::StrCaselessCompare(buffer.c_str(), "UTF-16") == 0) {
			return UTF16();
		}
		else {
			CharEncodingType encoding = charEncodingUnknown;
			uint32_t romVersion = 0;
			if ((::FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion) == 0)
				&& (romVersion >=  sysMakeROMVersion(4,0,0,sysROMStageRelease,0))) {
				encoding = ::TxtNameToEncoding(buffer.c_str());
			}
			if (encoding == charEncodingUnknown) {
				throw XMLException(txtErrUnknownEncoding, "unknown encoding");
			}
			return TextEncoding(encoding);
		}
	}
	
	
	bool TextEncoding::IsAvailable() const {
		// I don't know of any way to tell if a particular encoding is supported or not
		// other than to try using it.
		TxtConvertStateType state;
		TextSize srcBytes = 1;
		char buffer[8];
		TextSize dstBytes = sizeof(buffer);
		Err result = ::TxtConvertEncoding(true, &state,
			" ", &srcBytes, charEncodingPalmLatin,
			buffer, &dstBytes, mEncoding,
			NULL, 0);
		return result == errNone;
	}
	
}
