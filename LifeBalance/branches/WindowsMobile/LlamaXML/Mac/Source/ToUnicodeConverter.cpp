/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#include "ToUnicodeConverter.h"
#include "ErrorCodeException.h"

namespace LlamaXML {

	ToUnicodeConverter::ToUnicodeConverter(TextEncoding sourceEncoding)
	: mSourceEncoding(sourceEncoding),
	  mState(0)
	{
		ThrowIfError(::CreateTextToUnicodeInfoByEncoding(mSourceEncoding, &mState));
	}
	
	ToUnicodeConverter::~ToUnicodeConverter() {
		::DisposeTextToUnicodeInfo(&mState);
	}

	void ToUnicodeConverter::Reset(TextEncoding sourceEncoding)
	{
		::DisposeTextToUnicodeInfo(&mState);
		mState = 0;
		mSourceEncoding = sourceEncoding;
		ThrowIfError(::CreateTextToUnicodeInfoByEncoding(mSourceEncoding, &mState));
	}
		
	void ToUnicodeConverter::Convert(char * & sourceStart,
		char * sourceEnd, UnicodeChar * & destStart,
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
		else ThrowError(status);
	}
}
