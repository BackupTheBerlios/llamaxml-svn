/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#include "LlamaXML/ConvertToUnicode.h"
#include "LlamaXML/XMLException.h"
#include "LlamaXML/TextEncoding.h"

namespace LlamaXML {

	ConvertToUnicode::ConvertToUnicode(TextEncoding sourceEncoding)
	: mSourceEncoding(sourceEncoding),
	  mState(0)
	{
		ThrowIfXMLError(::TECCreateConverter(&mState, sourceEncoding, TextEncoding::UCS2()));
	}
	
	ConvertToUnicode::~ConvertToUnicode() {
		::TECDisposeConverter(mState);
	}

	void ConvertToUnicode::Reset(TextEncoding sourceEncoding)
	{
		::TECDisposeConverter(mState);
		mState = 0;
		mSourceEncoding = sourceEncoding;
		ThrowIfXMLError(::TECCreateConverter(&mState, sourceEncoding, TextEncoding::UCS2()));
	}
		
	void ConvertToUnicode::Convert(const char * & sourceStart,
		const char * sourceEnd, UnicodeChar * & destStart,
		UnicodeChar * destEnd)
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
