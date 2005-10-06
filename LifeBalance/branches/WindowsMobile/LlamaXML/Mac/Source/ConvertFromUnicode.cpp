/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#include "ConvertFromUnicode.h"
#include "XMLException.h"

namespace LlamaXML {

	ConvertFromUnicode::ConvertFromUnicode(TextEncoding destinationEncoding)
	: mDestinationEncoding(destinationEncoding),
	  mState(0)
	{
		ThrowIfXMLError(::CreateUnicodeToTextInfoByEncoding(mDestinationEncoding, &mState));
	}
	
	ConvertFromUnicode::~ConvertFromUnicode() {
	    ::DisposeUnicodeToTextInfo(&mState);
	}
		
	void ConvertFromUnicode::Convert(const UnicodeChar * & sourceStart,
		const UnicodeChar * sourceEnd, char * & destStart,
		char * destEnd)
	{
		ByteCount sourceRead = 0;
		ByteCount destWritten = 0;
		OSStatus status = ::ConvertFromUnicodeToText(mState, (sourceEnd - sourceStart) * sizeof(*sourceStart), sourceStart,
			kUnicodeUseFallbacksMask | kUnicodeKeepInfoMask | kUnicodeLooseMappingsMask | kUnicodeStringUnterminatedMask,
			0, NULL, NULL, NULL,
			(destEnd - destStart) * sizeof(*destStart), &sourceRead, &destWritten, destStart);
		if ((status == noErr) || (status == kTECArrayFullErr) || (status == kTECPartialCharErr) || (status == kTECIncompleteElementErr)
			|| (status == kTECUsedFallbacksStatus) || (status == kTECOutputBufferFullStatus)) {
			sourceStart += sourceRead;
			destStart += (destWritten / sizeof(*destStart));
		}
		else ThrowXMLError(status);
	}
}
