#include "ToUnicodeConverter.h"
#include "ErrorCodeException.h"
#include <TxtGlue.h>

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
		UInt16 sourceBytes = UInt16(sourceEnd - sourceStart);
		UInt16 destBytes = UInt16((destEnd - destStart) * sizeof(UnicodeChar));
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
