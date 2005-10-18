/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#include "LlamaXML/ConvertToUnicode.h"
#include "LlamaXML/XMLException.h"

namespace LlamaXML {

	ConvertToUnicode::ConvertToUnicode(TextEncoding sourceEncoding)
	{
		Reset(sourceEncoding);
	}
	

	void ConvertToUnicode::Reset(TextEncoding sourceEncoding)
	{
		mSourceEncoding = sourceEncoding;
		mNewConversion = true;
	}
		
	void ConvertToUnicode::Convert(const char * & sourceStart,
		const char * sourceEnd, UnicodeChar * & destStart,
		UnicodeChar * destEnd)
	{
#if PALMOS_SDK_VERSION >= 0x600
		typedef size_t TxtConvertLength;
#else
		typedef uint16_t TxtConvertLength;
#endif
		TxtConvertLength sourceBytes = TxtConvertLength((sourceEnd - sourceStart) * sizeof(*sourceStart));
		TxtConvertLength destBytes = TxtConvertLength((destEnd - destStart) * sizeof(*destStart));
		Err err = ::TxtConvertEncoding(mNewConversion, &mState,
			sourceStart, &sourceBytes, mSourceEncoding.AsPalmCharEncoding(),
			reinterpret_cast<Char *>(destStart),
			&destBytes,
			charEncodingUCS2, 0, 0);
		if ((err != errNone) && (err != txtErrConvertOverflow) &&
			(err != txtErrConvertUnderflow)) {
			ThrowXMLError(err);
		}
		sourceStart += (sourceBytes / sizeof(*sourceStart));
		destStart += (destBytes / sizeof(*destStart));
		mNewConversion = false;
	}
}
