/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#include "ToUnicodeConverter.h"
#include "ErrorCodeException.h"

namespace LlamaXML {

	ToUnicodeConverter::ToUnicodeConverter(TextEncoding sourceEncoding)
	{
		Reset(sourceEncoding);
	}
	

	void ToUnicodeConverter::Reset(TextEncoding sourceEncoding)
	{
		mSourceEncoding = sourceEncoding;
		mNewConversion = true;
	}
		
	void ToUnicodeConverter::Convert(char * & sourceStart,
		char * sourceEnd, UnicodeChar * & destStart,
		UnicodeChar * destEnd)
	{
#if PALMOS_SDK_VERSION >= 0x600
		typedef size_t TxtConvertLength;
#else
		typedef uint16_t TxtConvertLength;
#endif
		TxtConvertLength sourceBytes = TxtConvertLength(sourceEnd - sourceStart);
		TxtConvertLength destBytes = TxtConvertLength((destEnd - destStart) * sizeof(UnicodeChar));
		Err err = ::TxtConvertEncoding(mNewConversion, &mState,
			sourceStart, &sourceBytes, mSourceEncoding.AsPalmCharEncoding(),
			reinterpret_cast<Char *>(destStart),
			&destBytes,
			charEncodingUCS2, 0, 0);
		if ((err != errNone) && (err != txtErrConvertOverflow) &&
			(err != txtErrConvertUnderflow)) {
			ThrowError(err);
		}
		sourceStart += sourceBytes;
		destStart += (destBytes / sizeof(UnicodeChar));
		mNewConversion = false;
	}
}
