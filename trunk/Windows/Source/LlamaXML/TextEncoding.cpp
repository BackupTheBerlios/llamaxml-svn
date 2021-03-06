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
#include "LlamaXML/Component.h"
#include <Mlang.h>


namespace LlamaXML {

	TextEncoding::TextEncoding()
	: mCodePage(UINT(-1))
	{
	}

	TextEncoding TextEncoding::Application() {
		return TextEncoding(::GetACP());
	}

	TextEncoding TextEncoding::System() {
		return TextEncoding(::GetACP());
	}

	static const UnicodeChar * sDesiredName;
	static UINT sMatchingCodePage;

	static BOOL CALLBACK EnumCodePagesProc(LPTSTR lpCodePageString) {
		UINT cp = 0;
		while (*lpCodePageString) {
			cp = (10 * cp) + (*lpCodePageString++ - L'0');
		}
		CPINFO info;
		BOOL found = ::GetCPInfo(cp, &info);
		return TRUE;
	}


	TextEncoding TextEncoding::WebCharset(const UnicodeChar * name)
	{
		// This method of converting encoding names to code pages
		// supported on all Windows machines with IE 5.0 or later installed,
		// and on some Windows CE devices.  However, we can't depend on it,
		// so fall back to hard-coded names if using IMultiLanguage2 fails.
		try {
			Component<IMultiLanguage2> multiLanguage(CLSID_CMultiLanguage);
			Component<IEnumCodePage> enumCodePage;

			ThrowIfXMLError(multiLanguage->EnumCodePages(0, LOCALE_INVARIANT, enumCodePage.Adopt()));

			MIMECPINFO cpInfo;
			ULONG cpCount;
			while (enumCodePage->Next(1, &cpInfo, &cpCount) == S_OK) {
				if (::_wcsicmp(cpInfo.wszWebCharset, name) == 0) {
					return TextEncoding(cpInfo.uiCodePage);
				}
			}
		}
		catch (...) {
		}

		if (_wcsicmp(name, L"UTF-8") == 0) {
			return UTF8();
		}
		else if (_wcsicmp(name, L"UTF-16") == 0) {
			return UTF16();
		}
		else if (_wcsicmp(name, L"ISO-10646-UCS-2") == 0) {
			return UCS2();
		}
		else if (_wcsicmp(name, L"ISO-8859-1") == 0) {
			return ISOLatin1();
		}
		else if (_wcsicmp(name, L"Shift_JIS") == 0) {
			return ShiftJIS();
		}
		else if (_wcsicmp(name, L"Windows-1252") == 0) {
			return WindowsLatin1();
		}
		else {
			throw XMLException(0, "Unknown encoding");
		}
	}
	
#if 0
	String TextEncoding::Name() const
	{
		switch (mCodePage) {
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
		return ::IsValidCodePage(mCodePage) == TRUE;
	}
	
	
	TextEncoding TextEncoding::WindowsLatin1()
	{
		return TextEncoding(1252);
	}
	
	
	TextEncoding TextEncoding::ISOLatin1()
	{
		if (::IsValidCodePage(28591)) {
			return TextEncoding(28591);
		}
		else {
			return TextEncoding(1252);
		}
	}

	
	TextEncoding TextEncoding::PalmLatin1()
	{
		return TextEncoding(1252);
	}
	
	
	TextEncoding TextEncoding::ShiftJIS()
	{
		return TextEncoding(932);
	}
	
	
	TextEncoding TextEncoding::WindowsShiftJIS()
	{
		return TextEncoding(932);
	}
	
	
	TextEncoding TextEncoding::PalmShiftJIS()
	{
		return TextEncoding(932);
	}
	
	
	TextEncoding TextEncoding::ASCII()
	{
		return TextEncoding(20127);
	}
	
	
	TextEncoding TextEncoding::UCS2()
	{
		// UCS2 is essentially UTF16 without surrogate pairs.
		// Windows doesn't seem to define this subset explicitly, so just
		// use UTF16.
		return TextEncoding(1200);
	}
	
	
	TextEncoding TextEncoding::UTF7()
	{
		return TextEncoding(65000);
	}
	
	
	TextEncoding TextEncoding::UTF8()
	{
		return TextEncoding(65001);
	}
	
	
	TextEncoding TextEncoding::UTF16()
	{
		return TextEncoding(1200);
	}
	
	
	TextEncoding TextEncoding::UTF16BE()
	{
		return TextEncoding(1201);
	}
	
	
	TextEncoding TextEncoding::UTF16LE()
	{
		return TextEncoding(1200);
	}
	
	
	TextEncoding TextEncoding::UTF32()
	{
		return TextEncoding(12000);
	}
	
	
	TextEncoding TextEncoding::UTF32BE()
	{
		return TextEncoding(12001);
	}
	
	
	TextEncoding TextEncoding::UTF32LE()
	{
		return TextEncoding(12000);
	}
		
	
	TextEncoding TextEncoding::Windows(UINT codePage)
	{
		return TextEncoding(codePage);
	}

}
