/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#include "ConvertToUnicode.h"
#include "XMLException.h"

namespace LlamaXML {

	ConvertToUnicode::ConvertToUnicode(TextEncoding sourceEncoding)
	: mSourceEncoding(sourceEncoding),
	  mState(0)
	{
		ThrowIfXMLError(::CreateTextToUnicodeInfoByEncoding(mSourceEncoding, &mState));
	}
	
	ConvertToUnicode::~ConvertToUnicode() {
		::DisposeTextToUnicodeInfo(&mState);
	}

	void ConvertToUnicode::Reset(TextEncoding sourceEncoding)
	{
		::DisposeTextToUnicodeInfo(&mState);
		mState = 0;
		mSourceEncoding = sourceEncoding;
		ThrowIfXMLError(::CreateTextToUnicodeInfoByEncoding(mSourceEncoding, &mState));
	}
		
	void ConvertToUnicode::Convert(const char * & sourceStart,
		const char * sourceEnd, UnicodeChar * & destStart,
		UnicodeChar * destEnd)
	{
		ByteCount sourceRead = 0;
		ByteCount unicodeLen = 0;
		OSStatus status = ::ConvertFromTextToUnicode(mState, sourceEnd - sourceStart, sourceStart,
			kUnicodeUseFallbacksMask | kUnicodeKeepInfoMask | kUnicodeLooseMappingsMask | kUnicodeStringUnterminatedMask,
			0, NULL, NULL, NULL,
			(destEnd - destStart) * sizeof(UnicodeChar), &sourceRead, &unicodeLen, destStart);
		if ((status == noErr) || (status == kTECArrayFullErr) || (status == kTECPartialCharErr) || (status == kTECIncompleteElementErr)
			|| (status == kTECUsedFallbacksStatus) || (status == kTECOutputBufferFullStatus)) {
			sourceStart += sourceRead;
			destStart += (unicodeLen / sizeof(UnicodeChar));
		}
		else ThrowXMLError(status);
	}
}
