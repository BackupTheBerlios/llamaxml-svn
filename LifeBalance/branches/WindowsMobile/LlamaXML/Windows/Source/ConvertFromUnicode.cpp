/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#include "ConvertFromUnicode.h"
#include "XMLException.h"
#include <Windows.h>

namespace LlamaXML {

	ConvertFromUnicode::ConvertFromUnicode(TextEncoding destinationEncoding)
	: mDestinationEncoding(destinationEncoding)
	{
	}
	
	ConvertFromUnicode::~ConvertFromUnicode() {
	}
		
	void ConvertFromUnicode::Convert(const UnicodeChar * & sourceStart,
		const UnicodeChar * sourceEnd, char * & destStart,
		char * destEnd)
	{
		while ((sourceStart < sourceEnd) && (destStart < destEnd)) {
			int result = ::WideCharToMultiByte(mDestinationEncoding, 0, sourceStart, 1,
				destStart, destEnd - destStart, 0, 0);
			if (result > 0) {
				destStart += result;
				sourceStart += 1;
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