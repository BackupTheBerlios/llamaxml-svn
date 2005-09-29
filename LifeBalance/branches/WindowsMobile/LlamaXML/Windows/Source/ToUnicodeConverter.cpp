/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#include "ToUnicodeConverter.h"
#include "XMLException.h"
#include <Windows.h>

namespace LlamaXML {

	ToUnicodeConverter::ToUnicodeConverter(TextEncoding sourceEncoding)
	: mSourceEncoding(sourceEncoding)
	{
	}
	
	ToUnicodeConverter::~ToUnicodeConverter() {
	}

	void ToUnicodeConverter::Reset(TextEncoding sourceEncoding)
	{
	}
		
	void ToUnicodeConverter::Convert(const char * & sourceStart, const char * sourceEnd,
		UnicodeChar * & destStart, UnicodeChar * destEnd)
	{
		while ((sourceStart < sourceEnd) && (destStart < destEnd)) {
			const char * charEnd = sourceStart;
			while (true) {
				if (charEnd >= sourceEnd) return;
				if (! ::IsDBCSLeadByteEx(mSourceEncoding, *charEnd++)) break;
			}
			int result = ::MultiByteToWideChar(mSourceEncoding, 0, sourceStart, charEnd - sourceStart,
				destStart, destEnd - destStart);
			if (result > 0) {
				destStart += result;
				sourceStart = charEnd;
			}
			else {
				if (result == 0) {
					result = ::GetLastError();
				}
				if (result != ERROR_INSUFFICIENT_BUFFER) ThrowXMLError(result);
			}
		}
	}
}
