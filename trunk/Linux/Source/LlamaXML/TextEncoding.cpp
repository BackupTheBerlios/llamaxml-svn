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
#include "LlamaXML/PlatformConfig.h"
#include <iconv.h>


namespace LlamaXML {

#if __linux__
    static const char * gUCS2 = "UCS-2";
#else
    static const char * gUCS2 = "UCS-2-INTERNAL";
#endif
	
	TextEncoding::TextEncoding()
	{
	}

	TextEncoding TextEncoding::Application() {
		return TextEncoding("");
	}

	TextEncoding TextEncoding::System() {
		return TextEncoding("");
	}

	TextEncoding TextEncoding::WebCharset(const UnicodeChar * name)
	{
		std::string asciiName;
		while (*name) {
			asciiName.append(1, char(*name++));
		}
		return TextEncoding(asciiName.c_str());
	}
	
#if 0
	String TextEncoding::Name() const
	{
		switch (mEncoding) {
			case 932: 
				return String::Localized("Japanese (Shift-JIS)", "");
			case 1251: 
				return String::Localized("Cyrillic (Windows)", "");
			case 28595: 
				return String::Localized("Cyrillic (ISO)", "");
			case 20866: 
				return String::Localized("Russian (KOI8-R)", "");
			case 855: 
				return String::Localized("OEM Cyrillic", "");
			case 866: 
				return String::Localized("Cyrillic (DOS)", "");
			case 10007: 
				return String::Localized("Cyrillic (Mac)", "");
			case 10017: 
				return String::Localized("Ukrainian (Mac)", "");
			case 38598: 
				return String::Localized("Hebrew (ISO-Logical)", "");
			case 28598: 
				return String::Localized("Hebrew (ISO-Visual)", "");
			case 1255: 
				return String::Localized("Hebrew (Windows)", "");
			case 936: 
				return String::Localized("Chinese Simplified (GB2312)", "");
			case 52936: 
				return String::Localized("Chinese Simplified (HZ)", "");
			case 1252: 
				return String::Localized("Western European (Windows)", "");
			case 20127: 
				return String::Localized("US-ASCII", "");
			case 20936:
				return String::Localized("Chinese Simplified (GB2312-80)", "");
			case 950:
				return String::Localized("Chinese Traditional (Big5)", "");
			case 21866:
				return String::Localized("Ukrainian (KOI8-U)", "");
			case 51932:
				return String::Localized("Japanese (EUC)", "");
			case 1253:
				return String::Localized("Greek (Windows)", "");
			case 28599:
				return String::Localized("Turkish (ISO)", "");
			case 65000: 
				return String::Localized("Unicode (UTF-7)", "");
			case 65001: 
				return String::Localized("Unicode (UTF-8)", "");
			case 1201: 
				return String::Localized("Unicode (Big-Endian)", "");
			case 1200: 
				return String::Localized("Unicode", "");
			default:
				return String::Localized("unknown", "");
		}
	}
#endif
	
	bool TextEncoding::IsAvailable() const
	{
            // See if the encoding is available by tring to convert from it to UCS2.
            iconv_t converter = iconv_open(gUCS2, mName.c_str());

            if (converter == (iconv_t)-1) {
                return false;
            }
            else {
                iconv_close(converter);
                return true;
            }
	}
	
	
	TextEncoding TextEncoding::WindowsLatin1()
	{
		return TextEncoding("CP1252");
	}
	
	
	TextEncoding TextEncoding::ISOLatin1()
	{
		return TextEncoding("ISO-8859-1");
	}

	
	TextEncoding TextEncoding::PalmLatin1()
	{
		return TextEncoding("CP1252");
	}
	
	
	TextEncoding TextEncoding::ShiftJIS()
	{
		return TextEncoding("SHIFT_JIS");
	}
	
	
	TextEncoding TextEncoding::WindowsShiftJIS()
	{
		return TextEncoding("CP932");
	}
	
	
	TextEncoding TextEncoding::PalmShiftJIS()
	{
		// Palm Shift JIS is based on Windows CP 932, but is not identical.
		return TextEncoding("CP932");
	}
	
	
	TextEncoding TextEncoding::ASCII()
	{
		return TextEncoding("ASCII");
	}
	
	
	TextEncoding TextEncoding::UCS2()
	{
		// UCS2 is essentially UTF16 without surrogate pairs.
		// Windows doesn't seem to define this subset explicitly, so just
		// use UTF16.
		return TextEncoding(gUCS2);
	}
	
	
	TextEncoding TextEncoding::UTF7()
	{
		return TextEncoding("UTF-7");
	}
	
	
	TextEncoding TextEncoding::UTF8()
	{
		return TextEncoding("UTF-8");
	}
	
	
	TextEncoding TextEncoding::UTF16()
	{
		return TextEncoding("UTF-16");
	}
	
	
	TextEncoding TextEncoding::UTF16BE()
	{
		return TextEncoding("UTF-16BE");
	}
	
	
	TextEncoding TextEncoding::UTF16LE()
	{
		return TextEncoding("UTF-16LE");
	}
	
	
	TextEncoding TextEncoding::UTF32()
	{
		return TextEncoding("UCS-4-INTERNAL");
	}
	
	
	TextEncoding TextEncoding::UTF32BE()
	{
		return TextEncoding("UCS-4BE");
	}
	
	
	TextEncoding TextEncoding::UTF32LE()
	{
		return TextEncoding("UCS-4LE");
	}

}
