/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#include "LlamaXML/ConvertFromUnicode.h"
#include "LlamaXML/XMLException.h"

namespace LlamaXML {

	ConvertFromUnicode::ConvertFromUnicode(TextEncoding destinationEncoding)
	: mDestinationEncoding(destinationEncoding),
	  mNewConversion(true)
	{
	}
		
	void ConvertFromUnicode::Convert(const UnicodeChar * & sourceStart,
		const UnicodeChar * sourceEnd, char * & destStart,
		char * destEnd)
	{
#if PALMOS_SDK_VERSION >= 0x600
		typedef size_t TxtConvertLength;
#else
		typedef uint16_t TxtConvertLength;
#endif
		TxtConvertLength sourceBytes = TxtConvertLength((sourceEnd - sourceStart) * sizeof(*sourceStart));
		TxtConvertLength destBytes = TxtConvertLength((destEnd - destStart) * sizeof(*destStart));
		Err err = ::TxtConvertEncoding(mNewConversion, &mState,
			reinterpret_cast<const Char *>(sourceStart), &sourceBytes,
			charEncodingUCS2,
			reinterpret_cast<Char *>(destStart),
			&destBytes,
			mDestinationEncoding.AsPalmCharEncoding(), 0, 0);
		if ((err != errNone) && (err != txtErrConvertOverflow) &&
			(err != txtErrConvertUnderflow)) {
			ThrowXMLError(err);
		}
		sourceStart += (sourceBytes / sizeof(*sourceStart));
		destStart += (destBytes / sizeof(*destStart));
		mNewConversion = false;
	}
}
