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

#include "LlamaXML/ConvertString.h"
#include "LlamaXML/ConvertToUnicode.h"
#include "LlamaXML/ConvertFromUnicode.h"

#define countof(x) (sizeof(x) / sizeof(*x))

namespace LlamaXML {
	
	size_t UnicodeStringLength(const UnicodeChar * s) {
		size_t result = 0;
		while (*s++) ++result;
		return result;
	}
	
	UnicodeString ToUnicode(const char * start, const char * end, TextEncoding sourceEncoding) {
		ConvertToUnicode convert(sourceEncoding);
		UnicodeChar buffer[256];
		UnicodeString result;
		while (start < end) {
			UnicodeChar * bufferPtr = buffer;
			convert.Convert(start, end, bufferPtr, buffer + countof(buffer));
			result.append(buffer, bufferPtr);
		}
		return result;
	}
	
	UnicodeString ToUnicode(const char * s, TextEncoding sourceEncoding) {
		return ToUnicode(s, s + std::strlen(s), sourceEncoding);
	}
	
	UnicodeString ToUnicode(const std::string & s, TextEncoding sourceEncoding) {
		return ToUnicode(s.data(), s.data() + s.size(), sourceEncoding);
	}
	
	std::string FromUnicode(const UnicodeChar * start, const UnicodeChar * end, TextEncoding destinationEncoding) {
		ConvertFromUnicode convert(destinationEncoding);
		char buffer[256];
		std::string result;
		while (start < end) {
			char * bufferPtr = buffer;
			convert.Convert(start, end, bufferPtr, buffer + countof(buffer));
			result.append(buffer, bufferPtr);
		}
		return result;
	}
	
	std::string FromUnicode(const UnicodeChar * s, TextEncoding destinationEncoding) {
		return FromUnicode(s, s + UnicodeStringLength(s), destinationEncoding);
	}
	
	std::string FromUnicode(const UnicodeString & s, TextEncoding destinationEncoding) {
		return FromUnicode(s.data(), s.data() + s.size(), destinationEncoding);
	}
	
}