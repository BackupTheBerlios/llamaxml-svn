/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
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