/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#include "LlamaXML/ConvertFromUnicode.h"
#include "LlamaXML/XMLException.h"

namespace LlamaXML {

	ConvertFromUnicode::ConvertFromUnicode(TextEncoding destinationEncoding)
	: mDestinationEncoding(destinationEncoding),
	  mState(0)
	{
		ThrowIfXMLError(::TECCreateConverter(&mState, TextEncoding::UCS2(), mDestinationEncoding));
	}
	
	ConvertFromUnicode::~ConvertFromUnicode() {
		::TECDisposeConverter(mState);
	}
		
	void ConvertFromUnicode::Convert(const UnicodeChar * & sourceStart,
		const UnicodeChar * sourceEnd, char * & destStart,
		char * destEnd)
	{
		ByteCount sourceRead = 0;
		ByteCount destWritten = 0;
		OSStatus status = ::TECConvertText(mState, reinterpret_cast<ConstTextPtr>(sourceStart), (sourceEnd - sourceStart) * sizeof(*sourceStart),
		    &sourceRead, reinterpret_cast<TextPtr>(destStart), (destEnd - destStart) * sizeof(*destStart), &destWritten);
		if ((status == noErr) || (status == kTECArrayFullErr) || (status == kTECPartialCharErr) || (status == kTECIncompleteElementErr)
			|| (status == kTECUsedFallbacksStatus) || (status == kTECOutputBufferFullStatus)) {
			sourceStart += sourceRead / sizeof(*sourceStart);
			destStart += destWritten / sizeof(*destStart);
		}
		else ThrowXMLError(status);
	}
}
